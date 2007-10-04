/***************************************************************************
                          parser.cpp  -  description
                             -------------------
    begin                : Sun Sep 1 2002
    copyright            : (C) 2002, 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

//qt includes
#include <qeventloop.h>
#include <qstring.h>
#include <qpoint.h>
#include <qregexp.h>
#include <qcstring.h>
#include <qstringlist.h>
#include <qstrlist.h>
#include <qdatetime.h>
#include <qfile.h>
#include <qtextcodec.h>
#include <qvaluelist.h>
#include <qvaluestack.h>

//standard library includes
#include <stdio.h>
#include <ctype.h>
//#include <iostream.h>

//app includes
#include "parser.h"
#include "saparser.h"
#include "parsercommon.h"
#include "node.h"
#include "tag.h"
#include "resource.h"
#include "quantaview.h"
#include "quantacommon.h"
#include "document.h"
#include "qextfileinfo.h"


#include "kafkacommon.h"
#include "undoredo.h"

#include "dtds.h"
#include "structtreetag.h"

#include "viewmanager.h"

//kde includes
#include <kapplication.h>
#include <kdebug.h>
#include <kdirwatch.h>
#include <kiconloader.h>
#include <klocale.h>
#include <ktexteditor/document.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/encodinginterface.h>
#include <ktexteditor/viewcursorinterface.h>

extern GroupElementMapList globalGroupMap;
static const QChar space(' ');

extern int NN;
extern QMap<Node*, int> nodes;

Parser::Parser()
{
  m_node = 0L;
  write = 0L;
  oldMaxLines = 0;
  m_parsingEnabled = true;
  m_parsingNeeded = true;
  m_parseIncludedFiles = true;
  m_saParser = new SAParser();
  connect(m_saParser, SIGNAL(rebuildStructureTree(bool)), SIGNAL(rebuildStructureTree(bool)));
  connect(m_saParser, SIGNAL(cleanGroups()), SLOT(cleanGroups()));
  ParserCommon::includeWatch = new KDirWatch();
  connect(ParserCommon::includeWatch, SIGNAL(dirty(const QString&)), SLOT(slotIncludedFileChanged(const QString&)));
}

Parser::~Parser()
{
  delete m_saParser;
}

/** Parse a string, using as start position sLine, sCol. */
Node *Parser::parseArea(int startLine, int startCol, int endLine, int endCol, Node **lastNode, Node *a_node)
{
  //first parse as an XML document
  QString textLine;
  textLine.fill(space, startCol);
  int line = startLine;
  int col = 0;
  int tagStartLine = 0;
  int tagEndLine, tagEndCol;
  int tagStartPos, specialStartPos;
  int lastLineLength;
 // if (endCol == 0)
  if (endLine > maxLines)
  {
    if (endLine > 0)
        endLine--;
    lastLineLength = write->editIf->lineLength(endLine) - 1;
    endCol = lastLineLength + 1;
  } else
    lastLineLength = write->editIf->lineLength(endLine) - 1;
  int specialAreaCount = m_dtd->specialAreas.count();
  bool nodeFound = false;
  bool goUp;
  Node *rootNode = 0L;
  Node *parentNode = a_node;
  Node *currentNode = a_node;
  if (currentNode && (currentNode->tag->type != Tag::XmlTag ||
      currentNode->tag->single))
      parentNode = currentNode->parent;
  Tag *tag = 0L;
  QTag *qTag = 0L;
  textLine.append(write->text(startLine, startCol, startLine, write->editIf->lineLength(startLine)));
  if (line == endLine)
  {
    if (endCol > 0)
      textLine.truncate(endCol + 1);
    else
      textLine = "";
  }
  if (m_dtd->family == Xml)
  {
    while (line <= endLine)
    {
      nodeFound = false;
      goUp = false;
      //find the first "<" and the first special area start definition in this line
      tagStartPos = textLine.find('<', col);
      specialStartPos = specialAreaCount ? textLine.find(m_dtd->specialAreaStartRx, col): -1;
      //if the special area start definition is before the first "<" it means
      //that we have found a special area
      if ( specialStartPos != -1 &&
          (specialStartPos <= tagStartPos || tagStartPos == -1) )
      {
        currentNode = ParserCommon::createTextNode(write, currentNode, line, specialStartPos, parentNode);
        if (!rootNode)
            rootNode = currentNode;
        QString foundText = m_dtd->specialAreaStartRx.cap();
        //create a toplevel node for the special area
        AreaStruct area(line, specialStartPos, line, specialStartPos + foundText.length() - 1);
        Node *node = ParserCommon::createScriptTagNode(write, area, foundText, m_dtd, parentNode, currentNode);
        if (node->parent && node->prev == node->parent) //some strange cases, but it's possible, eg.: <a href="<? foo ?>""></a><input size="<? foo ?>" >
        {
           node->prev->next = 0L;
           node->prev = 0L;
         }
        if (node->tag->name.lower().startsWith("comment"))
          node->tag->type = Tag::Comment;

        if (!rootNode)
          rootNode = node;

        area.eLine = endLine;
        area.eCol = endCol;
        currentNode = m_saParser->parseArea(area, foundText, "", node, false, true);
        line = m_saParser->lastParsedLine();
        textLine = ParserCommon::getLine(write, line, endLine, endCol);
        col = m_saParser->lastParsedColumn() + 1;
        continue;
      } else
      //if we have found an XML tag start ("<")
      if ( tagStartPos != -1 /*&&
          (tagStartPos < specialStartPos || specialStartPos == -1) */)
      {
        int openNum = 1;
        tagStartLine = line;
        tagEndLine = endLine;
        tagEndCol = lastLineLength;
        int sCol = tagStartPos + 1;
        int firstStartCol = lastLineLength + 1;
        int firstStartLine = endLine;
        bool firstOpenFound = false;
        bool insideSingleQuotes = false;
        bool insideDoubleQuotes = false;
        //find the matching ">" in the document
        while (line <= endLine && openNum > 0 && !firstOpenFound)
        {
          textLine = ParserCommon::getLine(write, line, endLine, endCol);
          uint textLineLen = textLine.length();
          for (uint i = sCol; i < textLineLen; i++)
          {
            if (i == 0 || (i > 0 && textLine[i-1] != '\\'))
            {
              if (textLine[i] == '\'' && !insideDoubleQuotes)
                insideSingleQuotes = !insideSingleQuotes;
              if (textLine[i] == '"' && !insideSingleQuotes)
                insideDoubleQuotes = !insideDoubleQuotes;
            }
            if (!insideSingleQuotes && !insideDoubleQuotes)
            {
                if (textLine[i] == '<')
                {
                  openNum++;
                  if (!firstOpenFound &&
                      (i < textLineLen -1 && (textLine[i + 1] == '/' || textLine[i + 1].isLetter()) ||
                       i == textLineLen -1)
                     )
                  {
                    firstStartCol = i;
                    firstStartLine = line;
                    firstOpenFound = true;
                    break;
                  }
                } else
                if (textLine[i] == '>') openNum--;
            }
            if (openNum == 0)
            {
              tagEndCol = i;
              tagEndLine = line;
              break;
            }
          }
          sCol = 0;
          if (openNum != 0)
              line++;
        }
        //the matching closing tag was not found
        if (openNum != 0)
        {
          tagEndLine = firstStartLine;
          tagEndCol = firstStartCol - 1;
          if (tagEndCol < 0)
          {
            tagEndLine--;
            if (tagEndLine < 0)
                tagEndLine = 0;
            tagEndCol = write->editIf->lineLength(tagEndLine);
          }
          line = tagEndLine;
          textLine = ParserCommon::getLine(write, line, endLine, endCol);
        }
        col = tagEndCol;
        nodeFound = true;
        //build an xml tag node here
        AreaStruct area(tagStartLine, tagStartPos, tagEndLine, tagEndCol);
        tag = new Tag(area, write, m_dtd, true);
        QString tagStr = tag->tagStr();
        tag->type = Tag::XmlTag;
        tag->validXMLTag = (openNum == 0);
        tag->single = QuantaCommon::isSingleTag(m_dtd->name, tag->name);
        if (tag->isClosingTag())
        {
          tag->type = Tag::XmlTagEnd;
          tag->single = true;
        }
        if (tagStr.right(2) == "/>" || tag->name.isEmpty())
        {
          tag->single = true;
          if (tag->name.length() > 1 && tag->name.endsWith("/"))
              tag->name.truncate(tag->name.length() - 1);
        }
        //the tag we found indicates the beginning of a special area, like <script type=... >
        if (m_dtd->specialTags.contains(tag->name.lower()) && !tag->single)
        {
        //TODO: handle goUp here
          Node *node = new Node(parentNode);
          nodeNum++;
          node->tag = tag;
          node->insideSpecial = true;
          if (currentNode && currentNode != parentNode)
          {
            currentNode->next = node;
            node->prev = currentNode;
          } else
          {
            if (parentNode)
                parentNode->child = node;
          }
          if (!rootNode)
              rootNode = node;
          //find the DTD that needs to be used for the special area
          QString tmpStr = m_dtd->specialTags[tag->name.lower()];
          int defPos = tmpStr.find('[');
          QString defValue;
          if (defPos != 0)
          {
            defValue = tmpStr.mid(defPos+1, tmpStr.findRev(']')-defPos-1).stripWhiteSpace();
            tmpStr = tmpStr.left(defPos);
          }
          QString s = tag->attributeValue(tmpStr);
          if (s.isEmpty())
            s = defValue;
          const DTDStruct *dtd = DTDs::ref()->find(s);
          if (!dtd)
              dtd = m_dtd;
          //a trick here: replace the node's DTD with this one //Note: with the new SAParser, the top level nodes must be Tag::ScriptTag-s!
         // const DTDStruct *savedDTD = node->tag->dtd;
          node->tag->setDtd(dtd);
          node->tag->type = Tag::ScriptTag;
          //now parse the special area
          area.bLine = area.eLine;
          area.bCol = area.eCol + 1;
          area.eLine = endLine;
          area.eCol = endCol;
          currentNode = m_saParser->parseArea(area, "", "</"+tag->name+"\\s*>", node, false, true);
          //restore & set the new variables
         // node->tag->dtd = savedDTD;
          line = m_saParser->lastParsedLine();
          textLine = ParserCommon::getLine(write, line, endLine, endCol);
          col = m_saParser->lastParsedColumn();
          continue;
        }

        qTag = 0L;
        goUp = ( parentNode &&
                ( (tag->type == Tag::XmlTagEnd && QuantaCommon::closesTag(parentNode->tag, tag)
                  ) ||
                    parentNode->tag->single )
              );
        if (parentNode && !goUp)
        {
          qTag = QuantaCommon::tagFromDTD(m_dtd, parentNode->tag->name);
          if ( qTag )
          {
            QString searchFor = (m_dtd->caseSensitive)?tag->name:tag->name.upper();
            searchFor.remove('/');
            if ( qTag->stoppingTags.contains(searchFor))
            {
              parentNode->tag->closingMissing = true; //parent is single...
              goUp = true;
            }
          }
        }
      }

      col++;
      if (nodeFound)
      {
        //first create a text/empty node between the current position and the last node
        Node *savedParentNode = parentNode;
        currentNode = ParserCommon::createTextNode(write, currentNode, tagStartLine, tagStartPos, parentNode);
        if (savedParentNode != parentNode)
            goUp = false;
        if (!rootNode)
            rootNode = currentNode;

        Node *node = 0L;
        if (goUp)
        {
          //handle cases like <ul><li></ul>
          if (tag->type == Tag::XmlTagEnd && !QuantaCommon::closesTag(parentNode->tag, tag))
          {
            while ( parentNode->parent &&
                    QuantaCommon::closesTag(parentNode->parent->tag, tag)
                  )
            {
              parentNode = parentNode->parent;
            }
          } else
          if (qTag && tag->type != Tag::XmlTagEnd)
          {
            //handle the case when a tag is a stopping tag for parent, and grandparent and so on.
            Node *n = parentNode;
            QString searchFor = (m_dtd->caseSensitive)?tag->name:tag->name.upper();
            while (qTag && n)
            {
              qTag = QuantaCommon::tagFromDTD(m_dtd, n->tag->name);
              if ( qTag )
              {
                if ( qTag->stoppingTags.contains(searchFor) )
                {
                  n->tag->closingMissing = true; //parent is single...
                  if (n->parent)
                    parentNode = n;
                  n = n->parent;
                } else
                {
                  break;
                }
              }
            }
          }

          node = new Node(parentNode->parent);
          nodeNum++;
          node->prev = parentNode;
          parentNode->next = node;
          parentNode = parentNode->parent;
          node->closesPrevious = true;
        } else
        {
          node = new Node(parentNode);
          nodeNum++;
          if (currentNode && currentNode != parentNode)
          {
            currentNode->next = node;
            node->prev = currentNode;
          } else
          {
            if (parentNode)
            {
              if (!parentNode->child)
                parentNode->child = node;
              else
              {
                Node *n = parentNode->child;
                while (n->next)
                  n = n->next;
                n->next = node;
                node->prev = n;
              }
            }
          }
        }
        if (!tag->single)
            parentNode = node;

        node->tag = tag;
        if (tag->type == Tag::NeedsParsing)
        {
          if (tag->name.lower().startsWith("comment"))
          {
#ifdef DEBUG_PARSER
            kdDebug(24000) << "COMMENT!" << endl;
#endif
            node->tag->type = Tag::Comment;
          }
        }
        else if (tag->type == Tag::XmlTag)
            {
              parseForXMLGroup(node);
              //search for scripts inside the XML tag
              parseScriptInsideTag(node);
            }

        currentNode = node;
        if (!rootNode)
            rootNode = node;
      } else
      {
        line++;
        col = 0;
        textLine = ParserCommon::getLine(write, line, endLine, endCol);
        //kdDebug(24000) << "Line " << line << endl;
      }

    }
  }

  int el = 0;
  int ec = -1;
  if (currentNode)
  {
    currentNode->tag->endPos(el, ec);
  }

  if (m_dtd->family == Script)
  {
    if (ec == -1)
        ec = 0;
    AreaStruct area(el, ec, endLine, endCol);
#ifdef DEBUG_PARSER
//     kdDebug(24000) << "Calling cleanGroups from Parser::parseArea" << endl;
#endif
    cleanGroups();
    m_saParser->setParsingEnabled(true);
    currentNode = m_saParser->parseArea(area, "", "", parentNode, true, true); //TODO: don't parse in detail here
    m_saParser->setParsingEnabled(false);
    el = m_saParser->lastParsedLine();
    ec = m_saParser->lastParsedColumn();
  } else
  if (endLine == maxLines && endCol == write->editIf->lineLength(maxLines) - 1)
  {
  //create a text node from the last tag until the end of file
    if (el == endLine && ec == endCol)
    {
      el = endLine + 1;
      ec = 0;
    } else
    {
      el = endLine;
      ec = endCol + 1;
    }
    currentNode = ParserCommon::createTextNode(write, currentNode, el, ec, parentNode);
  } else
  if (el != endLine || ec != endCol)
  {
    if (currentNode && currentNode->tag->type == Tag::ScriptTag)
    {
      parentNode = currentNode;
      currentNode = 0L;
    }
    currentNode = ParserCommon::createTextNode(write, currentNode, endLine, endCol, parentNode);
  }
  if (!rootNode)
    rootNode = currentNode;
  *lastNode = currentNode;
  return rootNode;
}

/** Parse the whole text from Document w and build the internal structure tree
    from Nodes */
Node *Parser::parse(Document *w, bool force)
{
  QTime t;
  t.start();
  QuantaView *view = ViewManager::ref()->activeView();
  //If VPL is loaded, there shouldn't be any rebuild
  if(view && view->hadLastFocus() == QuantaView::VPLFocus && !force)
    return m_node;

  if(!m_parsingEnabled && !force)
    return baseNode;

  bool saParserEnabled = m_saParser->parsingEnabled();
  m_saParser->setParsingEnabled(false);
  m_saParser->init(0L, w);
 // clearGroups();
  if (baseNode)
  {
     kdDebug(24000) << "Node objects before delete = " << NN << " ; list count = " << nodes.count() << endl;
     //kdDebug(24000) << "baseNode before delete = " << baseNode << endl;
     //ParserCommon::coutTree(m_node, 2);
     Node::deleteNode(baseNode);
     baseNode = 0L;
     kdDebug(24000) << "Node objects after delete = " << NN << " ; list count = " << nodes.count() << endl;
/*     QMap<Node*, int> nList = nodes;
     for (QValueList<Node*>::ConstIterator it = nList.constBegin(); it != nList.constEnd(); ++it)
     Node::deleteNode(*it);
     kdDebug(24000) << "Node objects after cleanup = " << NN << " ; list count = " << nodes.count() << endl;       */
  }
  m_node = 0L;

  Node *lastNode;
  write = w;
  m_dtd = w->defaultDTD();
  w->resetDTEPs();
  maxLines = w->editIf->numLines() - 1;
  parsingEnabled = true;
  nodeNum = 0;
  if (maxLines >= 0)
      m_node = parseArea(0, 0, maxLines, w->editIf->lineLength(maxLines) - 1, &lastNode);
  kdDebug(24000) << "Parsing time ("<< maxLines << " lines): " << t.elapsed() << " ms\n";
  if (!m_node)
  {
    m_node = ParserCommon::createTextNode(w, 0L, maxLines, w->editIf->lineLength(maxLines), 0L);
  }
  m_parsingNeeded = false;

//  treeSize = 0;
//  kdDebug(24000) << "Basenode : " << m_node << endl;
//  ParserCommon::coutTree(m_node, 2);
//  kdDebug(24000) << "Size of tree: " << treeSize << endl;

//FIXME: What is the use of two pointer to the same Node???
 baseNode = m_node;
     kdDebug(24000) << "NN after parse = " << NN << "baseNode : " << baseNode << endl;
 m_saParser->init(m_node, w);

 //We need to reload Kafka to refresh the DOM::Node->Node links.
 //FIXME: make a function which only update the DOM::Node->Node links.
  if (view)
    view->reloadVPLView(true);

 emit nodeTreeChanged();
 if (saParserEnabled)
   QTimer::singleShot(0, this, SLOT(slotParseInDetail()));
 return m_node;
}




/** No descriptions */
const DTDStruct * Parser::currentDTD(int line, int col)
{
  const DTDStruct *dtd = m_dtd;
  Node *node = nodeAt(line, col, false, true);
  if (node)
  {
    dtd = node->tag->dtd();
  }

  return dtd;
}

/** Returns the node for position (line, column). As more than one node can
contain the same area, it return the "deepest" node. */
Node *Parser::nodeAt(int line, int col, bool findDeepest, bool exact)
{
  if (!write)
      return 0L;
  if (!baseNode)
   baseNode = parse(write, true); //FIXME: this most likely hides a bug: new documents are not parsed

  Node *node = m_node;
  int bl, bc, el, ec;
  int result;

  while (node)
  {
    node->tag->beginPos(bl, bc);
    bc++;
    Node *n = node->nextNotChild();
    if (n && n->tag)
    {
      n->tag->beginPos(el, ec);
    } else
    {
       el = write->editIf->numLines();
       ec = 0;
    }
    result = QuantaCommon::isBetween(line, col, bl, bc, el, ec);
    if ( result == 0)
    {
      if (node->child)
      {
        node = node->child;
      } else
      {
        if (node->parent)
        {
          int parentEl, parentEc;
          node->parent->tag->endPos(parentEl, parentEc);
          if (!exact && QuantaCommon::isBetween(line, col, bl, bc, parentEl, parentEc) == 0)
          {
            node = node->parent;
          }
        }
        break; //we found the node
      }
    } else
    if (result == -1)
    {
      if (node->parent)
          node = node->parent;
      break; //we found the node
    } else
    {
       node = node->next;
    }
  }

  bc = ec = el = bl = 0;
  if (node)
  {
    node->tag->beginPos(bl, bc);
    node->tag->endPos(el, ec);
  }
  if (node && node->tag->type == Tag::Empty &&
     (findDeepest || (bl == el && ec < bc)) )
  {
     if (node->parent)
     {
       node = node->parent;
     } else
     if (node->prev)
     {
       node = node->prev;
     }
  } else
  if (node && (el < line || (el == line && ec + 1 < col)))
  {
    Node *n = node->nextSibling();
    if (n /*&& n->nextSibling()*/) //don't set it to the last, always empty node
      node = n;
  }
  return node;
}
void Parser::logReparse(NodeModifsSet *modifs, Document *w)
{

  NodeModif *modif;
  if (baseNode)
  {
    Node *n = baseNode;
    while (n)
    {
      n->detachNode();
      n = n->nextSibling();
    }
    modif = new NodeModif();
    modif->setType(NodeModif::NodeTreeRemoved);
    modif->setNode(baseNode);
    modifs->addNodeModif(modif);
    baseNode = 0L;
  }
  modif = new NodeModif();
  modif->setType(NodeModif::NodeTreeAdded);
  modifs->addNodeModif(modif);
  w->docUndoRedo->addNewModifsSet(modifs, undoRedo::SourceModif);
}

bool Parser::invalidArea(Document *w, AreaStruct &area, Node **firstNode, Node **lastNode)
{
  oldMaxLines = maxLines;
  maxLines = w->editIf->numLines() - 1;
  uint line, col;
  w->viewCursorIf->cursorPositionReal(&line, &col);
  Node *node = nodeAt(line, col, false);
  int bl, bc, el, ec;
  QString text;
  QString tagStr;
  area.bLine = area.bCol = 0;
  area.eLine = maxLines;
  area.eCol = w->editIf->lineLength(maxLines) - 1;
  if (area.eCol < 0)
    area.eCol = 0;
  if (node)
    node->tag->beginPos(area.bLine, area.bCol);

  Node *startNode = node;
 //find the first unchanged (non empty) node backwards and store it as firstNode
  *firstNode = 0L;
  while (node)
  {
    node->tag->beginPos(bl, bc);
    node->tag->endPos(el, ec);
    if (node->tag->type != Tag::Empty
        && !node->insideSpecial && node->tag->validXMLTag  //TODO:remove when script reparsing is supported
       )
    {
      text = w->text(bl, bc, el, ec);
      tagStr = node->tag->tagStr();
      if (tagStr == text)
      {
        *firstNode = node;
        //firstNode might not be the first unchanged Node e.g. text Nodes
        while (*firstNode)
        {
          if((*firstNode)->tag->type != Tag::Text)
            break;
          (*firstNode)->tag->endPos(el, ec);
          text = w->text(el, ec + 1, el, ec + 1);
          if (text == "<")
            break;
          else// a character has been added at the end of the text : this node is modified
            *firstNode = (*firstNode)->previousSibling();
        }
        break;
      } else
      {
        node = node->previousSibling(); //the tag content is different
      }
    } else
    {
      node = node->previousSibling(); //the tag is empty, ignore it
    }
  }
  //find the first unchanged (non empty) node forward and store it as lastNode
  //move the nodes if they were shifted
  bool moveNodes = false; //do we need to move the nodes?
  int lineDiff = maxLines - oldMaxLines; //lines are shifted with this amount
  node = startNode;
  *lastNode = 0L;
  while (node)
  {
    node->tag->beginPos(bl, bc);
    node->tag->endPos(el, ec);
    if (!moveNodes)
    {
      if (node->tag->type != Tag::Empty
          && !node->insideSpecial && node->tag->validXMLTag //TODO:remove when script reparsing is supported
        )
      {
        text = w->text(bl + lineDiff, bc, el + lineDiff, ec);
        tagStr = node->tag->tagStr();
        if (tagStr == text)
        {
          if (!(*lastNode))
              *lastNode = node;

          if (lineDiff != 0)
          {
            moveNodes = true;
            node->tag->setTagPosition(bl + lineDiff, bc, el + lineDiff, ec);
          } else
          {
            break; //lastNode found
          }
        }
      }
     } else
     {
       node->tag->setTagPosition(bl + lineDiff, bc, el + lineDiff, ec);
     }
     node = node->nextSibling();
  }

  if (*firstNode)
    node = (*firstNode)->nextSibling(); //the first changed node
  else
    return false;
  if (node)
    node->tag->beginPos(area.bLine, area.bCol);
  if (*lastNode)
  {
    (*lastNode)->tag->beginPos(area.eLine, area.eCol);
    if (area.eCol > 0)
      area.eCol--;
  }
  return true;
}

void Parser::deleteNodes(Node *firstNode, Node *lastNode, NodeModifsSet *modifs)
{
  Node *nextNode, *child, *parent, *next, *prev;
  int i, j;
  Node *node = firstNode;
  bool closesPrevious = false;
  NodeModif *modif;

  //delete all the nodes between the firstNode and lastNode
  while (node && node != lastNode )
  {
    nextNode = node->nextSibling();
    node->removeAll = false;
    child = node->child;
    parent = node->parent;
    next = node->next;
    prev = node->prev;
    closesPrevious = node->closesPrevious;
    if (nextNode && nextNode->prev == node)
    {
      nextNode->prev = prev;
    }
    if (nextNode && nextNode->parent == node)
    {
      nextNode->parent = parent;
    }
    if (next)
      next->prev = prev;
    if (prev && prev->next == node)
    {
      prev->next = next;
    }
    if (next && next->closesPrevious)
      next->closesPrevious = false;

    modif = new NodeModif();
    modif->setType(NodeModif::NodeRemoved);
    modif->setLocation(kafkaCommon::getLocation(node));
    if (prev && prev->next == node)
      prev->next = 0L;
    if(parent && parent->child == node)
      parent->child = 0L;
    node->parent = 0L;
    node->next = 0L;
    node->prev = 0L;
    
    //delete node;
    node->detachNode();
    modif->setNode(node);

    node = 0L;
    i = 0;
    j = 0;
    if (!closesPrevious)
    {
      //move the children up one level
      Node *n = child;
      Node *m = child;
      while (n)
      {
        m = n;
        n->parent = parent;
        n = n->next;
        i++;
      }
      //connect the first child to the tree (after prev, or as the first child of the parent)
      if (prev && child)
      {
        prev->next = child;
        child->prev = prev;
        if (next) //the last child is just before the next
        {
          m->next = next;
          next->prev = m;
        }
      } else
      {
        if (!child)      //when there is no child, connect the next as the first child of the parent
          child = next;
        else
        if (next)
        {
          n = child;
          while (n->next)
            n = n->next;
          n->next = next;
          next->prev = n;
        }
        if (parent && !parent->child)
        {
          parent->child = child;
        }
      }
    } else
    {
      //change the parent of children, so the prev will be the new parent
      if (child)
      {
        Node *n = child;
        Node *m = child;
        while (n)
        {
          m = n;
          n->parent = prev;
          n = n->next;
          i++;
        }
        if (prev->child)
        {
          n = prev;
          while (n->child)
          {
            n = n->child;
            while (n->next)
              n = n->next;
          }
          n->next = child;
          child->prev = n;
        } else
        {
          prev->child = child;
        }
      }
      //move down the nodes starting with next one level and append to the list of children of prev
      if (next)
      {
        if (prev->child) //if the previous node has a child, append the next node after the last child
        {
            Node *n = prev;
            while (n->child)
            {
              n = n->child;
              while (n->next)
                n = n->next;
            }
            next->prev = n;
            n->next = next;
        } else // else append it as the first child of the previous
        {
          prev->child = next;
          next->prev = 0L;
        }
        //all the nodes after the previous are going UNDER the previous, as the one closing node was deleted
        //and the tree starting with next is moved under prev (see the above lines)
        prev->next = 0L;
        Node *n = next;
        while (n)
        {
          n->parent = prev;
          n = n->next;
          j++;
        }

      }
    }

    modif->setChildrenMovedUp(i);
    modif->setNeighboursMovedDown(j);
    modifs->addNodeModif(modif);

    node = nextNode;

 //   kdDebug(24000)<< "Node removed!" << endl;
//    ParserCommon::coutTree(m_node, 2);
  }
//  ParserCommon::coutTree(m_node, 2);
}

Node *Parser::rebuild(Document *w)
{
 kdDebug(24000) << "Rebuild started. " << endl;
 QTime t;
 t.start();
 bool saParserEnabled = m_saParser->parsingEnabled();

  //If VPL is loaded, there shouldn't be any rebuild
  if(ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
    return m_node;

 NodeModifsSet *modifs = new NodeModifsSet();
 NodeModif *modif;

// kdDebug(24000)<< "Node *Parser::rebuild()" << endl;
 modifs->setIsModifiedAfter(w->isModified());

 //**kdDebug(24000)<< "************* Begin User Modification *****************" << endl;
  //debug!
  //ParserCommon::coutTree(m_node, 2);//*/
 if (w != write || !m_node) //the document has changed or the top node does not exists => parse completely
 {
  logReparse(modifs, w);
   return parse(w);
 } else
 {
   m_saParser->setParsingEnabled(false);
   m_saParser->init(0L, w);
   parsingEnabled = true;
   QString text;
   QString tagStr;

   Node *firstNode = 0L;
   Node *lastNode = 0L;
   Node *node = 0L;

   AreaStruct area(0, 0, 0, 0);
   if ( !invalidArea(w, area, &firstNode, &lastNode) ||
        (area.eLine < area.bLine || (area.eLine == area.bLine && area.eCol <= area.bCol)) //something strange has happened, like moving text with D&D inside the editor
      )
   {
      logReparse(modifs, w);
       m_saParser->setParsingEnabled(saParserEnabled);
       Node *n = parse(w, true);
       return n;
   }

   kdDebug(24000) << QString("Invalid area: %1,%2,%3,%4").arg(area.bLine).arg(area.bCol).arg(area.eLine).arg(area.eCol) << "\n";

//    kdDebug(24000) << "lastNode1: " << lastNode << " " << lastNode->tag << endl;
   deleteNodes(firstNode->nextSibling(), lastNode, modifs);
//    kdDebug(24000) << "lastNode2: " << lastNode << " " << lastNode->tag << endl;

   firstNode->child = 0L;
   Node *lastInserted = 0L;
   //this makes sure that the first found node it put right after the firstNode
   if (firstNode->next && firstNode->next == lastNode)
   {
      firstNode->next->prev = 0L;
      firstNode->next = 0L;
   }
   node = parseArea(area.bLine, area.bCol, area.eLine, area.eCol, &lastInserted, firstNode);

   Node *swapNode = firstNode->nextSibling();
   Node *p = (lastInserted)?lastInserted->nextSibling():lastInserted;
   while(swapNode != p)
   {
      modif = new NodeModif();
      modif->setType(NodeModif::NodeAdded);
      modif->setLocation(kafkaCommon::getLocation(swapNode));
      modifs->addNodeModif(modif);
      swapNode = swapNode->nextSibling();
   }
   //another stange case: the parsed area contains a special area without end
   if (!node)
   {
     if (lastNode)
     {
       if (lastNode->prev )
           lastNode->prev->next = 0L;
       if (lastNode->parent && lastNode->parent->child == lastNode)
           lastNode->parent->child = 0L;
     }
     Node::deleteNode(lastNode);
     nodeNum--;
     lastNode = 0L;
     logReparse(modifs, w);
     m_saParser->setParsingEnabled(saParserEnabled);
     return parse(w);
   }
//    kdDebug(24000) << "lastNode3: " << lastNode << " " << lastNode->tag << endl;
   bool goUp;
   if (lastNode && lastInserted)
   {
//      kdDebug(24000) << "lastNode4: " << lastNode << " " << lastNode->tag << endl;
      //merge the nodes if they are both of type Text or Empty
      if ( (lastInserted->tag->type == Tag::Empty || lastInserted->tag->type == Tag::Text) &&
            (lastNode->tag->type == Tag::Empty || lastNode->tag->type == Tag::Text))
      {
        if (lastNode->prev)
          lastNode->prev->next = 0L;
        lastNode->prev = lastInserted->prev;
        if (lastInserted->prev)
            lastInserted->prev->next = lastNode;
        lastNode->parent = lastInserted->parent;
        lastInserted->tag->beginPos(area.bLine, area.bCol);
        lastNode->tag->endPos(area.eLine, area.eCol);
        Tag *_tag = new Tag(*(lastNode->tag));
        lastNode->tag->setTagPosition(area);
        QString s = write->text(area);
        lastNode->tag->setStr(s);
        if (!s.simplifyWhiteSpace().isEmpty())
        {
          lastNode->tag->type = Tag::Text;
        } else
        {
          lastNode->tag->type = Tag::Empty;
        }
        if (lastInserted->parent && lastInserted->parent->child == lastInserted)
            //lastInserted->parent->child = lastInserted->next; lastInserted has no next!
           lastInserted->parent->child = lastNode;

        //here, lastNode is at the pos of lastInserted.
        modif = new NodeModif();
        modif->setType(NodeModif::NodeRemoved);
        modif->setLocation(kafkaCommon::getLocation(lastNode));

        if(lastInserted->prev)
          lastInserted->prev->next = 0L;
        if(lastInserted->parent && lastInserted->parent->child == lastInserted)
          lastInserted->parent->child = 0L;
        lastInserted->prev = 0L;
        lastInserted->next = 0L;
        lastInserted->parent = 0L;
        lastInserted->child = 0L;
//        delete lastInserted;

        lastInserted->detachNode();
        modif->setNode(lastInserted);
        modifs->addNodeModif(modif);

        modif = new NodeModif();
        modif->setType(NodeModif::NodeModified);
        modif->setLocation(kafkaCommon::getLocation(lastNode));
        modif->setTag(_tag);
        modifs->addNodeModif(modif);

        lastInserted = lastNode;
        lastNode = lastNode->nextNotChild();
      }

    node = lastInserted;

//     kdDebug(24000) << "lastNode5: " << lastNode << " " << lastNode->tag << endl;
    QTag *qTag = 0L;
    while (node && lastNode)
    {
//       kdDebug(24000) << "lastNode6: " << lastNode << " " << lastNode->tag << endl;
      qTag = 0L;
      goUp = ( node->parent &&
               ( (lastNode->tag->type == Tag::XmlTagEnd && QuantaCommon::closesTag(node->parent->tag, lastNode->tag) ) ||
                  node->parent->tag->single )
             );
      if (node->parent && !goUp)
      {
        qTag = QuantaCommon::tagFromDTD(m_dtd, node->parent->tag->name);
        if ( qTag )
        {
          QString searchFor = (m_dtd->caseSensitive)?lastNode->tag->name:lastNode->tag->name.upper();
          searchFor.remove('/');
          if ( qTag->stoppingTags.contains( searchFor ) )
          {
            node->parent->tag->closingMissing = true; //parent is single...
            goUp = true;
          }
        }
      }
      if (goUp &&
          ( (m_dtd->caseSensitive && node->tag->name == node->parent->tag->name) ||
            (!m_dtd->caseSensitive && node->tag->name.lower() == node->parent->tag->name.lower())) )
          goUp = false; //it can happen that the tag closes the previous and not the parent

      if (goUp) //lastnode closes the node->parent
      {
          //handle cases like <ul><li></ul>
          if (lastNode->tag->type == Tag::XmlTagEnd &&
              !QuantaCommon::closesTag(node->parent->tag, lastNode->tag))
          {
            while ( node->parent->parent &&
                    QuantaCommon::closesTag(node->parent->parent->tag, lastNode->tag)
                  )
            {
              node = node->parent;
            }
          } else
          if (qTag && lastNode->tag->type != Tag::XmlTagEnd)
          {
            //handle the case when a tag is a stopping tag for parent, and grandparent and so on. I'm not sure it's needed here, but anyway...
            Node *n = node->parent;
            QString searchFor = (m_dtd->caseSensitive) ? lastNode->tag->name : lastNode->tag->name.upper();
            while (qTag && n)
            {
              qTag = QuantaCommon::tagFromDTD(m_dtd, n->tag->name);
              if ( qTag )
              {
                if ( qTag->stoppingTags.contains(searchFor) )
                {
                  n->tag->closingMissing = true; //parent is single...
                  if (n->parent)
                    node = n;
                  n = n->parent;
                } else
                {
                  break;
                }
              }
            }
          }
        if (lastNode->prev && lastNode->prev->next == lastNode)
          lastNode->prev->next = 0L;
        if (lastNode->parent && lastNode->parent->child == lastNode)
            lastNode->parent->child = 0L;
        if (node->parent)
          node->parent->next = lastNode;
        lastNode->prev = node->parent;
        if (node->parent)
          lastNode->parent = node->parent->parent;
        else
          lastNode->parent = 0L;
        node->next = 0L;
        lastNode->closesPrevious = true;
      } else
      {
        if (lastNode->prev && lastNode->prev->next == lastNode)
            lastNode->prev->next = 0L;
        node->next = lastNode;
        lastNode->prev = node;
        lastNode->parent = node->parent;
//         kdDebug(24000) << "lastNode7: " << lastNode << " " << lastNode->tag << endl;
      }
      node = lastNode;
      lastNode = lastNode->nextNotChild();
      //For some reason this can happen, the lastNode can point to an invalid place. 
      //To avoid crashes, forget the rebuild and do a full parse instead.
      if (!nodes.contains(lastNode)) 
      {
        kdDebug(24000) << "Lastnode is invalid, do a full reparse!" << endl;
        logReparse(modifs, w);
        m_saParser->setParsingEnabled(saParserEnabled);
        Node *n = parse(w, true);
        return n;
      }
/*      if (lastNode)
        QString s = lastNode->tag->tagStr();*/
    }
 }
/*   kdDebug(24000)<< "END"<< endl;
   ParserCommon::coutTree(baseNode,  2);
   kdDebug(24000)<< "************* End User Modification *****************" << endl;*/

   w->docUndoRedo->addNewModifsSet(modifs, undoRedo::SourceModif);
 }
  kdDebug(24000) << "Rebuild: " << t.elapsed() << " ms; baseNode=" << baseNode << "\n";
 
//   ParserCommon::verifyTree(m_node);
/* treeSize = 0;
 ParserCommon::coutTree(m_node, 2);
 kdDebug(24000) << "Size of tree: " << treeSize << endl;*/

 m_saParser->init(m_node, w);
 if (saParserEnabled)
   QTimer::singleShot(0, this, SLOT(slotParseInDetail()));
 emit nodeTreeChanged();
 m_parsingNeeded = false;
 return m_node;
}

void Parser::clearGroups()
{
#ifdef DEBUG_PARSER
//   kdDebug(24000) << "clearGroups " << endl;
#endif
  GroupElementMapList::Iterator it;
  GroupElementList::Iterator elementIt;
  GroupElementList *list;
  int count = 0;
  for (it = globalGroupMap.begin(); it != globalGroupMap.end(); ++it)
  {
    list = & it.data();
    //Clear the group element list and also remove the group tag which
    //was created in parseForXMLGroup/parseForScriptGroup methods.
    elementIt = list->begin();
    while (elementIt != list->end())
    {
      GroupElement *groupElement = (*elementIt);
#ifdef DEBUG_PARSER
      kdDebug(24001) << "GroupElement deleted: " <<groupElement << " "<< groupElement->tag->area().bLine << " " << groupElement->tag->area().bCol << " "<< groupElement->tag->area().eLine << " "<< groupElement->tag->area().eCol << " " << groupElement->tag->tagStr() << " " << groupElement->type << endl;
#endif
      //kdDebug(24000) << "usertagcount: " << groupElement->tag->write()->userTagList.count() << endl;
      groupElement->tag->write()->userTagList.remove(groupElement->tag->name.lower());
      if (!groupElement->deleted)
      {
        Node *n = groupElement->node;
        n->m_groupElements.clear();
      }
      groupElement->group = 0L;
      delete groupElement->tag;
      groupElement->tag = 0L;
      elementIt = list->erase(elementIt);
      delete groupElement;
      groupElement = 0L;
      count++;
    }
  }
#ifdef DEBUG_PARSER
//       kdDebug(24000) << count << " GroupElement deleted (clearGroups)." << endl;
#endif
  globalGroupMap.clear();
  clearIncludedGroupElements();
    
  ParserCommon::includedFiles.clear();
  ParserCommon::includedFilesDTD.clear();
  delete ParserCommon::includeWatch;
  ParserCommon::includeWatch = new KDirWatch();
  connect(ParserCommon::includeWatch, SIGNAL(dirty(const QString&)), SLOT(slotIncludedFileChanged(const QString&)));
  m_parseIncludedFiles = true;
}

void Parser::cleanGroups()
{
#ifdef DEBUG_PARSER
//   kdDebug(24000) << "cleanGroups " << endl;
#endif
  GroupElementMapList::Iterator it;
  GroupElementList::Iterator elementIt;
  GroupElementList *list;
  int count = 0;
  for (it = globalGroupMap.begin(); it != globalGroupMap.end(); ++it)
  {
    list = & it.data();
    //Clear the group element list and also remove the group tag which
    //was created in parseForXMLGroup/parseForScriptGroup methods.
    elementIt = list->begin();
    while (elementIt != list->end())
    {
      GroupElement *groupElement = (*elementIt);
      if (groupElement->deleted)
       {
#ifdef DEBUG_PARSER
         kdDebug(24001) << "GroupElement deleted: " <<groupElement << " "<< groupElement->tag->area().bLine << " " << groupElement->tag->area().bCol << " "<< groupElement->tag->area().eLine << " "<< groupElement->tag->area().eCol << " " << groupElement->tag->tagStr() << " " << groupElement->type << endl;
#endif
         groupElement->tag->write()->userTagList.remove(groupElement->tag->name.lower());
         groupElement->group = 0L;
         delete groupElement->tag;
         groupElement->tag = 0L;
         elementIt = list->erase(elementIt);
         delete groupElement;
         groupElement = 0L;
         count++;
       } else
       {
         ++elementIt;
       }
    }
  }
#ifdef DEBUG_PARSER
//       kdDebug(24000) << count << " GroupElement deleted (cleanGroups)." << endl;
#endif
  if (m_parseIncludedFiles)
  {
      delete ParserCommon::includeWatch;
      ParserCommon::includeWatch = new KDirWatch();
      connect(ParserCommon::includeWatch, SIGNAL(dirty(const QString&)), SLOT(slotIncludedFileChanged(const QString&)));
      parseIncludedFiles();
  }
}

void Parser::clearIncludedGroupElements()
{
  uint listCount;
  IncludedGroupElementsMap::Iterator includedMapIt;
  for (includedMapIt = includedMap.begin(); includedMapIt != includedMap.end(); ++includedMapIt)
  {
    IncludedGroupElements::Iterator elementsIt;
    for (elementsIt = includedMapIt.data().begin(); elementsIt != includedMapIt.data().end(); ++elementsIt)
    {
      GroupElementMapList::Iterator it;
      for (it = elementsIt.data().begin(); it != elementsIt.data().end(); ++it)
      {
        listCount = it.data().count();
        for (uint i = 0 ; i < listCount; i++)
        {
          GroupElement *groupElement = it.data()[i];
          groupElement->node->tag->write()->userTagList.remove(groupElement->node->tag->name.lower());
          Node::deleteNode(it.data()[i]->node);
          delete it.data()[i];
        }
      }
    }
  }
  includedMap.clear();
}

void Parser::parseIncludedFiles()
{
#ifdef DEBUG_PARSER
  kdDebug(24000) << "parseIncludedFiles" << endl;
#endif
  clearIncludedGroupElements();
  uint listCount;
  if (write->url().isLocalFile())
  {
    listCount = ParserCommon::includedFiles.count();
    for (uint i = 0; i < listCount; i++)
    {
      parseIncludedFile(ParserCommon::includedFiles[i], ParserCommon::includedFilesDTD.at(i));
    }
    if (listCount > 0)
      m_parseIncludedFiles = false;
  }
  emit rebuildStructureTree(true);
}

//structure used to temporary store the position of the groupelements in the searchFor
//included file as a string
struct GroupElementPosition{
  GroupElement *element;
  int startPos;
  int endPos;
};

void Parser::parseIncludedFile(const QString& fileName, const DTDStruct *dtd)
{
#ifdef DEBUG_PARSER
  kdDebug(24000) << "parseIncludedFile: " << fileName << endl;
#endif  
  StructTreeGroup group;
  QString content;
  QFile file(fileName);
  if (file.open(IO_ReadOnly))
  {
    IncludedGroupElements *elements = &includedMap[fileName];
    QTextStream str(&file);
    QString encoding;
    KTextEditor::EncodingInterface* encodingIf = dynamic_cast<KTextEditor::EncodingInterface*>(write->doc());
    if (encodingIf)
      encoding = encodingIf->encoding();
    if (encoding.isEmpty())
      encoding = "utf8";  //final fallback
    str.setCodec(QTextCodec::codecForName(encoding));
    content = str.read();
    file.close();
    if (dtd->specialAreas.count())
    {
      int areaPos = 0;
      int lastAreaPos = 0;
      QString foundStr;
      QString specialEndStr;
      while (areaPos != -1)
      {
        areaPos = content.find(dtd->specialAreaStartRx, lastAreaPos);
        if (areaPos != -1)
        {
          foundStr = dtd->specialAreaStartRx.cap();
          specialEndStr = dtd->specialAreas[foundStr];
          int areaPos2 = content.find(specialEndStr, areaPos);
          if (areaPos2 == -1)
          {
            areaPos2 = content.length();
            foundStr = content.mid(areaPos, areaPos2 - areaPos + 1);
            areaPos = -1;
          } else
          {
            foundStr = content.mid(areaPos, areaPos2 - areaPos + 1);
            lastAreaPos = areaPos2 + 1;
          }
          QuantaCommon::removeCommentsAndQuotes(foundStr, dtd);
          
          //gather the starting position of structures
          QValueList<uint> structPositions;
          int structPos = 0;
          while (structPos !=-1)
          {
            structPos = foundStr.find(dtd->structBeginStr, structPos);
            if (structPos != -1)
            {
              structPositions.append(structPos);
              structPos += dtd->structBeginStr.length();
            }
          }          
          
          QValueList<GroupElementPosition> gPositions;
          //go through the list of found structures and search for groups
          int structStartPosition = 0; //from where to start the group search. This is before the structure begin position
          QString savedStr = foundStr;
          for (uint i = 0; i < structPositions.count(); i++)
          {
            foundStr = savedStr;
            uint structBeginPos = structPositions[i];
            structPos = structBeginPos;
            int openNum = 1;
            int pos = structPos + dtd->structBeginStr.length();
            //find the corresponding structure closing string
            while (openNum !=0 && pos != -1)
            {
              pos = dtd->structRx.search(foundStr, pos);
              if (pos != -1)
              {
                if (dtd->structRx.cap() == dtd->structBeginStr)
                    openNum++;
                else
                    openNum--;
                pos++;
              }
            }
            if (pos == -1)
                pos = foundStr.length();
            int structEndPos = pos;
            foundStr = foundStr.left(pos);
            QString spaces;
            spaces.fill(' ', pos - structPos + 1);
            foundStr.replace(structPos, pos - structPos + 1, spaces);
            
            //FIXME: This code replaces the content between ( ) with
            //empty spaces. This is quite PHP (or functions) //specific, and it's done in order to not find variables
            //declared as function arguments. A generic way is needed
            //to exclude unwanted areas.
            int openBracketPos = foundStr.findRev(dtd->structKeywordsRx, structPos);
            openBracketPos = foundStr.find('(', openBracketPos);
            openNum = 1;
            if (openBracketPos != -1)
            {
              openBracketPos++;
              int closeBracketPos = openBracketPos;
              while (closeBracketPos < structPos && openNum !=0)
              {
                if (foundStr[closeBracketPos] == '(')
                    openNum++;
                if (foundStr[closeBracketPos] == ')')
                    openNum--;
                closeBracketPos++;
              }
              closeBracketPos--;
              spaces.fill(' ', closeBracketPos - openBracketPos);
              foundStr.replace(openBracketPos, closeBracketPos - openBracketPos, spaces);
            }

            //now check which groups are present in this area
            structPos =  pos + 1;
            QValueList<StructTreeGroup>::ConstIterator it;
            for (it = dtd->structTreeGroups.begin(); it != dtd->structTreeGroups.end(); ++it)
            {
              group = *it;
              if (!group.hasDefinitionRx)
                continue;
              int pos = structStartPosition;
              while (pos != -1)
              {
                pos = group.definitionRx.search(foundStr, pos);
                if (pos != -1)
                {
                  int l;
                  QString ss = group.definitionRx.cap();
                  if (group.definitionRx.pos(1) > pos)
                  {
                    pos = group.definitionRx.pos(1);
                    l = group.definitionRx.cap(1).length();
                    ss = group.definitionRx.cap(1);
                  }
                  else
                  {
                    l = group.definitionRx.cap().length();
                  }
                  QString s = content.mid(areaPos + pos, l);
                  pos += l;
                  if (!(*elements)[group.name].contains(s))
                  {
                    Tag *tag = new Tag();
                    tag->name = s;
                    tag->setDtd(dtd);
                    tag->setWrite(write);
                    QString s2 = content.left(areaPos + pos);
                    int newLineNum = s2.contains('\n');
                    int tmpCol = s2.length() - s2.findRev('\n') - 1;
                    tag->setTagPosition(newLineNum, tmpCol - s.length(), newLineNum, tmpCol);
                    Node *node = new Node(0L);
                    node->tag = tag;
                    node->fileName = fileName;
                    GroupElement *groupElement = new GroupElement;
                    groupElement->node = node;
                    groupElement->parentNode = 0L;
                    int minPos = areaPos + pos + 1;
                    for (QValueList<GroupElementPosition>::Iterator gPosIt = gPositions.begin(); gPosIt != gPositions.end(); ++gPosIt)
                    {
                      GroupElementPosition gPos = (*gPosIt);
                      if ( (areaPos + pos > gPos.startPos) && (areaPos + pos < gPos.endPos) && (gPos.startPos < minPos))
                      {
                        groupElement->parentNode = gPos.element->node;          
                        minPos = gPos.startPos; 
                      }
                    }
                    GroupElementList *groupElementList = &(*elements)[group.name][s];
                    groupElementList->append(groupElement);
                    
                    GroupElementPosition gPos;
                    gPos.startPos = areaPos + pos;
                    gPos.endPos = structEndPos;
                    gPos.element = groupElement;
                    gPositions.append(gPos);
                    
                    if (group.appendToTags)
                    {
                      QTag *qTag = new QTag();
                      qTag->setName(s.left(s.find('(')));
                      qTag->className = "";
                      if (groupElement->parentNode)
                        qTag->className = groupElement->parentNode->tag->name;
                      write->userTagList.replace(s.lower(), qTag);
                    }
                  }
                }
              }
            } //for
            structStartPosition = structBeginPos + 1;
          }
        } //if (areaPos != -1)
      }// while (areaPos != -1)
    }
  }
}

void Parser::slotIncludedFileChanged(const QString& fileName)
{
  int pos = ParserCommon::includedFiles.findIndex(fileName);
  if (pos != -1)
  {
    const DTDStruct *dtd = ParserCommon::includedFilesDTD.at(pos);
    if (dtd)
    {
      IncludedGroupElements::Iterator elementsIt;
      for (elementsIt = includedMap[fileName].begin(); elementsIt != includedMap[fileName].end(); ++elementsIt)
      {
        GroupElementMapList::Iterator it;
        for (it = elementsIt.data().begin(); it != elementsIt.data().end(); ++it)
        {
          uint listCount = it.data().count();
          for (uint i = 0 ; i < listCount; i++)
          {
            Node::deleteNode(it.data()[i]->node);
            delete it.data()[i];
          }
        }
      }
      includedMap[fileName].clear();
      parseIncludedFile(fileName, dtd);
    }
  }
}


void Parser::parseForXMLGroup(Node *node)
{
  xmlGroupIt = node->tag->dtd()->xmlStructTreeGroups.find(node->tag->name.lower());
  if (xmlGroupIt != node->tag->dtd()->xmlStructTreeGroups.end())
  {
    XMLStructGroup group = xmlGroupIt.data();
    Tag *newTag = new Tag(*node->tag);
    QString title = "";
    QStringList::Iterator it;
    for (it = group.attributes.begin(); it != group.attributes.end(); ++it)
    {
      if (newTag->hasAttribute(*it))
      {
          title.append(newTag->attributeValue(*it).left(100));
          title.append(" | ");
      }
    }
    title = title.left(title.length()-3);
    title.remove('\n');
    newTag->name = title;

    GroupElement *groupElement = new GroupElement;
    groupElement->deleted = false;
    groupElement->tag = newTag;
    groupElement->node = node;
    groupElement->parentNode = 0L;
    groupElement->global = true;
    groupElement->group = const_cast<XMLStructGroup*>(&(xmlGroupIt.data()));
    node->m_groupElements.append(groupElement);
    GroupElementList* groupElementList = & (globalGroupMap[group.name + "|" + title]);
    groupElementList->append(groupElement);
  }
}


bool Parser::parseScriptInsideTag(Node *startNode)
{
  bool found = false;
  const DTDStruct *dtd = startNode->tag->dtd();
  if (dtd->specialAreas.count())
  {
    QString foundText;
    QString s;
    QString specialEndStr;
    QString text = startNode->tag->tagStr();

    int pos = 0;
    int col = startNode->tag->structBeginStr.length();
    int bl, bc, el, ec;
    int node_bl, node_bc, node_el, node_ec;
    int n;
    startNode->tag->beginPos(node_bl, node_bc);
    startNode->tag->endPos(node_el, node_ec);
    Node *currentNode = 0L;

    while (pos != -1)
    {
      pos = text.find(dtd->specialAreaStartRx, col);
      if (pos != -1)
      {
        foundText = dtd->specialAreaStartRx.cap();
        //Calculate the beginning coordinates
        s = text.left(pos);
        n = s.contains('\n');
        bl = node_bl + n;
        if (n > 0)
        {
          bc = pos - s.findRev('\n') - 1;
        } else
        {
          bc = node_bc + pos;
        }
        //What is the closing string?
        specialEndStr = dtd->specialAreas[foundText];

        el = bl;
        ec = bc + foundText.length() - 1;
        AreaStruct area(bl, bc, el, ec);
        currentNode = ParserCommon::createScriptTagNode(write, area, foundText, dtd, startNode, currentNode);
        currentNode->specialInsideXml = true;

        found = true;
        AreaStruct area2(bl, bc, node_el, node_ec);
        int lastLine, lastCol;
        m_saParser->setSpecialInsideXml(true);
        currentNode = m_saParser->parseArea(area2, foundText, "", currentNode, true, true);
        m_saParser->setSpecialInsideXml(false);
        lastLine = m_saParser->lastParsedLine();
        lastCol = m_saParser->lastParsedColumn();
        col = write->text(node_bl, node_bc, lastLine, lastCol).length();
        int firstSpecialAttrIndex = startNode->tag->attributeIndexAtPos(bl, bc);
        if (firstSpecialAttrIndex != -1)
        {
          int lastSpecialAttrIndex = startNode->tag->attributeIndexAtPos(lastLine, lastCol);
          for (int i = firstSpecialAttrIndex; i <= lastSpecialAttrIndex; i++)
          {
            startNode->tag->setAttributeSpecial(i, true);
          }
        }
      }
    }
  }
  return found;
}

void Parser::slotParseInDetail()
{
  m_saParser->parseInDetail(false);
}

void Parser::synchParseInDetail()
{
  m_saParser->parseInDetail(true);
}

void Parser::setSAParserEnabled(bool enabled)
{
  m_saParser->setParsingEnabled(enabled);
  //kapp->processEvents(QEventLoop::ExcludeUserInput | QEventLoop::ExcludeSocketNotifiers); //this makes sure that the parsing is really disabled
}

#include "parser.moc"
