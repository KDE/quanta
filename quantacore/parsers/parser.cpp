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

//app includes

#include "parser.h"

#include "editorsource.h"
#include "groupelement.h"
#include "groupstorage.h"

#include "saparser.h"
#include "parsercommon.h"
#include "node.h"
#include "tag.h"
#include "quantacommon.h"
// #include "extfileinfo.h"
#include "dtds.h"

#include "parseresult.h"


//qt includes
#include <QEventLoop>
#include <QString>
#include <qpoint.h>
#include <QRegExp>
#include <QStringList>
#include <QDateTime>
#include <QFile>
#include <QTextCodec>
#include <QList>
#include <qstack.h>
#include <QTimer>

//standard library includes
#include <stdio.h>
#include <ctype.h>
//#include <iostream.h>

//kde includes
#include <kapplication.h>
#include <kdebug.h>
#include <kdirwatch.h>
#include <kiconloader.h>
#include <klocale.h>
#include <ktexteditor/document.h>
#include <kglobal.h>


static const QChar space(' ');

int nodeNum;

Parser::Parser()
{
  m_document = 0L;
  m_source = 0L;

  m_base = 0L;
  oldMaxLines = 0;
//   m_parsingEnabled = true;
  m_parseIncludedFiles = true;
  m_saParser = new SAParser();
  connect(m_saParser, SIGNAL(finishedParsing(const EditorSource *, const ParseResult  *)), SIGNAL(finishedParsing(const EditorSource *, const ParseResult  *)));
  connect(m_saParser, SIGNAL(groupsParsed(const EditorSource *, const ParseResult *)), SIGNAL(groupsParsed(const EditorSource *, const ParseResult *)));
  connect(m_saParser, SIGNAL(cleanGroups()), SLOT(cleanGroups()));
  ParserCommon::includeWatch = new KDirWatch();
  connect(ParserCommon::includeWatch, SIGNAL(dirty(const QString&)), SLOT(slotIncludedFileChanged(const QString&)));
}

Parser::~Parser()
{
  delete m_saParser;
}

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
    lastLineLength = m_source->lineLength(endLine) - 1;
    endCol = lastLineLength + 1;
  } else
    lastLineLength = m_source->lineLength(endLine) - 1;
  int specialAreaCount = m_dtd->specialAreas.count();
  bool nodeFound = false;
  bool goUp;
  Node *rootNode = 0L;
  Node *parentNode = a_node;
  Node *currentNode = a_node;
  if (currentNode && (!currentNode->tag->isType(Tag::XmlTag) ||
      currentNode->tag->single))
      parentNode = currentNode->parent;
  Tag *tag = 0L;
  QTag *qTag = 0L;
  textLine.append(m_source->text(startLine, startCol, startLine, m_source->lineLength(startLine)));
  if (line == endLine)
  {
    if (endCol > 0)
      textLine.truncate(endCol + 1);
    else
      textLine = "";
  }
  if (m_dtd->family == DTDStruct::Xml)
  {
    while (line <= endLine)
    {
      nodeFound = false;
      goUp = false;
      //find the first "<" and the first special area start definition in this line
      tagStartPos = textLine.indexOf('<', col);
      specialStartPos = specialAreaCount ? textLine.indexOf(m_dtd->specialAreaStartRx, col): -1;
      //if the special area start definition is before the first "<" it means
      //that we have found a special area
      if ( specialStartPos != -1 &&
          (specialStartPos <= tagStartPos || tagStartPos == -1) )
      {
        currentNode = ParserCommon::createTextNode(m_source, m_dtd, currentNode, line, specialStartPos, parentNode);
        if (!rootNode)
            rootNode = currentNode;
        QString foundText = m_dtd->specialAreaStartRx.cap();
        //create a toplevel node for the special area
        AreaStruct area(line, specialStartPos, line, specialStartPos + foundText.length() - 1);
        Node *node = ParserCommon::createScriptTagNode(area, foundText, m_dtd, parentNode, currentNode);
        if (node->parent && node->prev == node->parent) //some strange cases, but it's possible, eg.: <a href="<? foo ?>""></a><input size="<? foo ?>" >
        {
           node->prev->next = 0L;
           node->prev = 0L;
         }
        if (node->tag->name().lower().startsWith("comment"))
          node->tag->setType(Tag::Comment);

        if (!rootNode)
          rootNode = node;

        area.setEnd(endLine, endCol);
        currentNode = m_saParser->parseArea(area, foundText, "", node, false, true);
        line = m_saParser->lastParsedLine();
        textLine = m_source->getLine(line, endLine, endCol);
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
          textLine = m_source->getLine( line, endLine, endCol);
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
            tagEndCol = m_source->lineLength(tagEndLine);
          }
          line = tagEndLine;
          textLine = m_source->getLine( line, endLine, endCol);
        }
        col = tagEndCol;
        nodeFound = true;
        //build an xml tag node here
        AreaStruct area(tagStartLine, tagStartPos, tagEndLine, tagEndCol);
        QString tagStr = m_source->text(area);
        tag = new Tag(area, tagStr, m_dtd, true);
        tag->setType(Tag::XmlTag);
        tag->validXMLTag = (openNum == 0);
        tag->single = m_dtd->isSingleTag(tag->name());
        if (tag->isClosingTag())
        {
          tag->setType(Tag::XmlTagEnd);
          tag->single = true;
        }
        if (tagStr.endsWith("/>") || tag->name().isEmpty())
        {
          tag->single = true;
          QString name = tag->name();
          if (name.length() > 1 && name.endsWith('/'))
          {
            name.truncate(name.length() - 1);
            tag->setName(name);
          }
        }
        appendUserTag(tag);
        //the tag we found indicates the beginning of a special area, like <script type=... >
        if (m_dtd->specialTags.contains(tag->name().toLower()) && !tag->single)
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
          QString tmpStr = m_dtd->specialTags[tag->name().toLower()];
          int defPos = tmpStr.indexOf('[');
          QString defValue;
          if (defPos != 0)
          {
            defValue = tmpStr.mid(defPos+1, tmpStr.lastIndexOf(']')-defPos-1).trimmed();
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
          node->tag->setType(Tag::ScriptTag);
          //now parse the special area
/*          area.bLine = area.eLine;
          area.bCol = area.eCol + 1;
          area.eLine = endLine;
          area.eCol = endCol;*/
          area.setStart(area.eLine(), area.eCol() + 1);
          area.setEnd(endLine, endCol);
          currentNode = m_saParser->parseArea(area, "", "</" + tag->name() +"\\s*>", node, false, true);
          //restore & set the new variables
         // node->tag->dtd = savedDTD;
          line = m_saParser->lastParsedLine();
          textLine = m_source->getLine(line, endLine, endCol);
          col = m_saParser->lastParsedColumn();
          continue;
        }

        qTag = 0L;
        goUp = ( parentNode &&
                ( (tag->isType(Tag::XmlTagEnd) && tag->doesClose(parentNode->tag)
                  ) ||
                    parentNode->tag->single )
              );
        if (parentNode && !goUp)
        {
          qTag = QTag::tagFromDTD(m_dtd, parentNode->tag->name());
          if ( qTag )
          {
            QString searchFor = tag->name(m_dtd->caseSensitive);
            if ( qTag->stoppingTags.contains( searchFor ) )
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
        currentNode = ParserCommon::createTextNode(m_source, m_dtd, currentNode, tagStartLine, tagStartPos, parentNode);
        if (savedParentNode != parentNode)
            goUp = false;
        if (!rootNode)
            rootNode = currentNode;

        Node *node = 0L;
        if (goUp)
        {
          //handle cases like <ul><li></ul>
          if (tag->isType(Tag::XmlTagEnd) && !tag->doesClose(parentNode->tag))
          {
            while ( parentNode->parent &&
                    tag->doesClose(parentNode->parent->tag)
                  )
            {
              parentNode = parentNode->parent;
            }
          } else
          if (qTag && !tag->isType(Tag::XmlTagEnd))
          {
            //handle the case when a tag is a stopping tag for parent, and grandparent and so on.
            Node *n = parentNode;
            QString searchFor = tag->name(m_dtd->caseSensitive);
            while (qTag && n)
            {
              qTag = QTag::tagFromDTD(m_dtd, n->tag->name());
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
        if (tag->isType(Tag::NeedsParsing))
        {
          if (tag->name().lower().startsWith("comment"))
          {
#ifdef DEBUG_PARSER
            kdDebug(24000) << "COMMENT!" << endl;
#endif
            node->tag->setType(Tag::Comment);
          }
        }
        else if (tag->isType(Tag::XmlTag))
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
        textLine = m_source->getLine( line, endLine, endCol);
      }

    }
  }

  int el = 0;
  int ec = -1;
  if (currentNode)
  {
    currentNode->tag->endPos(el, ec);
  }

  if (m_dtd->family == DTDStruct::Script)
  {
    if (ec == -1)
        ec = 0;
    AreaStruct area(el, ec, endLine, endCol);
#ifdef DEBUG_PARSER
    kDebug(24000) << "Calling cleanGroups from Parser::parseArea";
#endif
    cleanGroups();
    m_saParser->setParsingEnabled(true);
    currentNode = m_saParser->parseArea(area, "", "", parentNode, true, true); //TODO: don't parse in detail here
    m_saParser->setParsingEnabled(false);
    el = m_saParser->lastParsedLine();
    ec = m_saParser->lastParsedColumn();
  } else
  if (endLine == maxLines && endCol == m_source->lineLength(maxLines) - 1)
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
    currentNode = ParserCommon::createTextNode(m_source, m_dtd, currentNode, el, ec, parentNode);
  } else
  if (el != endLine || ec != endCol)
  {
    if (currentNode && currentNode->tag->isType(Tag::ScriptTag))
    {
      parentNode = currentNode;
      currentNode = 0L;
    }
    currentNode = ParserCommon::createTextNode(m_source, m_dtd, currentNode, endLine, endCol, parentNode);
  }
  if (!rootNode)
    rootNode = currentNode;
  *lastNode = currentNode;
  return rootNode;
}

void Parser::parse(EditorSource *source, ParseResult *base, const DTDStruct *dtd, bool detailed)
{
  m_source = source;
  QTime t;
  t.start();

  m_base = base;
  m_dtd = dtd;
  //always disable and stop special area parsing before starting to parse
  m_saParser->setParsingEnabled(false);
  m_saParser->init(m_base, m_source, m_dtd);
 // clearGroups();
  //delete the whole tree first
  if (m_base->baseNode)
  {
    m_base->baseNode->deleteNode(m_base->groupStorage);
    m_base->baseNode= 0L;
  }

//clear the DTEP list
  m_base->dtepList.clear();
  m_base->dtepList.append(m_dtd);
  Node *lastNode;
  maxLines = m_source->numLines() - 1;
  parsingEnabled = true;
  nodeNum = 0;
  if (maxLines >= 0)
      m_base->baseNode = parseArea(0, 0, maxLines, m_source->lineLength(maxLines) - 1, &lastNode);
  kDebug(24000) << "Parsing time ("<< maxLines << " lines): " << t.elapsed() << " ms\n";
  if (!m_base->baseNode)
  {
    m_base->baseNode = ParserCommon::createTextNode(m_source, m_dtd, 0L, maxLines, m_source->lineLength(maxLines), 0L);
  }
//  treeSize = 0;
//  kDebug(24000) << "Basenode : " << m_base->baseNode;
  //Node::coutTree(m_base->baseNode, 2);
//  kDebug(24000) << "Size of tree: " << treeSize;

 m_saParser->init(m_base, m_source, m_dtd);

 //We need to reload Kafka to refresh the DOM::Node->Node links.
 //FIXME: make a function which only update the DOM::Node->Node links.
 /*FIXME if (view)
    view->reloadVPLView(true);*/

 emit nodeTreeChanged();
 if (detailed)
   QTimer::singleShot(0, this, SLOT(slotParseInDetail()));
}



// void Parser::logReparse(NodeModifsSet *modifs, Document *w)
// {
//   NodeModif *modif;
//   if (baseNode)
//   {
//     Node *n = baseNode;
//     while (n)
//     {
//       n->detachNode();
//       n = n->nextSibling();
//     }
//     modif = new NodeModif();
//     modif->setType(NodeModif::NodeTreeRemoved);
//     modif->setNode(baseNode);
//     modifs->addNodeModif(modif);
//     baseNode = 0L;
//   }
//   modif = new NodeModif();
//   modif->setType(NodeModif::NodeTreeAdded);
//   modifs->addNodeModif(modif);
//   w->docUndoRedo->addNewModifsSet(modifs, undoRedo::SourceModif);
// }

bool Parser::invalidArea(AreaStruct &area, Node **firstNode, Node **lastNode)
{
  oldMaxLines = maxLines;
  maxLines = m_source->numLines() - 1;
  uint line, col;
  m_source->cursorPositionReal(line, col);
  Node *node = Node::nodeAt(m_base->baseNode, line, col, false);
  int bl, bc, el, ec;
  QString text;
  QString tagStr;
  area.setStart(0, 0);
  area.setEnd(maxLines, qMax(m_source->lineLength(maxLines) - 1, 0));
/*  if (area.eCol < 0)
    area.eCol = 0;*/
  if (node)
    area.start = node->tag->beginPos();

  Node *startNode = node;
 //find the first unchanged (non empty) node backwards and store it as firstNode
  *firstNode = 0L;
  while (node)
  {
    node->tag->beginPos(bl, bc);
    node->tag->endPos(el, ec);
    if (!node->tag->isType(Tag::Empty)
        && !node->insideSpecial && node->tag->validXMLTag  //TODO:remove when script reparsing is supported
       )
    {
      text = m_source->text(bl, bc, el, ec);
      tagStr = node->tag->tagStr();
      if (tagStr == text)
      {
        *firstNode = node;
        //firstNode might not be the first unchanged Node e.g. text Nodes
        while (*firstNode)
        {
          if(!(*firstNode)->tag->isType(Tag::Text))
            break;
          (*firstNode)->tag->endPos(el, ec);
          text = m_source->text(el, ec + 1, el, ec + 1);
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
/*    node->tag->beginPos(bl, bc);
    node->tag->endPos(el, ec);*/
    AreaStruct newArea = node->tag->area();
    newArea.start.rx() += lineDiff;
    newArea.end.rx() += lineDiff;
    if (!moveNodes)
    {
      if (!node->tag->isType(Tag::Empty)
          && !node->insideSpecial && node->tag->validXMLTag //TODO:remove when script reparsing is supported
        )
      {
        text = m_source->text(newArea);
        tagStr = node->tag->tagStr();
        if (tagStr == text)
        {
          if (!(*lastNode))
              *lastNode = node;

          if (lineDiff != 0)
          {
            moveNodes = true;
            node->tag->setTagPosition(newArea);
          } else
          {
            break; //lastNode found
          }
        }
      }
     } else
     {
       node->tag->setTagPosition(newArea);
     }
     node = node->nextSibling();
  }

  if (*firstNode)
    node = (*firstNode)->nextSibling(); //the first changed node
  else
    return false;
  if (node)
    area.start = node->tag->beginPos();
  if (*lastNode)
  {
    area.end = (*lastNode)->tag->beginPos();
    if (area.eCol() > 0)
//       area.eCol--;
      area.end.ry()--;
  }
  return true;
}

void Parser::deleteNodes(Node *firstNode, Node *lastNode/*FIXME, NodeModifsSet *modifs*/)
{
  Node *nextNode, *child, *parent, *next, *prev;
  int i, j;
  Node *node = firstNode;
  bool closesPrevious = false;
//FIXME  NodeModif *modif;
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

/*FIXME    modif = new NodeModif();
    modif->setType(NodeModif::NodeRemoved);
    modif->setLocation(kafkaCommon::getLocation(node));*/
    if (prev && prev->next == node)
      prev->next = 0L;
    if(parent && parent->child == node)
      parent->child = 0L;
    node->parent = 0L;
    node->next = 0L;
    node->prev = 0L;
//    node->detachNode();
    node->deleteNode(m_base->groupStorage);
    //FIXME modif->setNode(node);
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
/* FIXME   modif->setChildrenMovedUp(i);
    modif->setNeighboursMovedDown(j);
    modifs->addNodeModif(modif);*/
    node = nextNode;

 //   kDebug(24000)<< "Node removed!";
//    Node::coutTree(m_node, 2);
  }
//  Node::coutTree(m_node, 2);
}

void Parser::rebuild(EditorSource *source, ParseResult *base, const DTDStruct *dtd, bool detailed)
{
 kDebug(24000) << "Rebuild started. ";
 QTime t;
 t.start();
 m_source = source;
 m_base = base;
 m_dtd = dtd;
/*
FIXME
  //If VPL is loaded, there shouldn't be any rebuild
  if(ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
    return m_node;

 NodeModifsSet *modifs = new NodeModifsSet();
 NodeModif *modif;


// kDebug(24000)<< "Node *Parser::rebuild()";
 modifs->setIsModifiedAfter(w->isModified());
*/
 //**kDebug(24000)<< "************* Begin User Modification *****************";
  //debug!
  //Node::coutTree(m_node, 2);//*/
 if (/*FIXME w != write || */!m_base->baseNode) //the document has changed or the top node does not exists => parse completely
 {
//FIXME  logReparse(modifs, w);
   return parse(source, base, dtd, true);
 } else
 {
  //always disable and stop special area parsing before starting to parse
   m_saParser->setParsingEnabled(false);
   m_saParser->init(m_base, m_source, m_dtd);
   parsingEnabled = true;
   QString text;
   QString tagStr;

   Node *firstNode = 0L;
   Node *lastNode = 0L;
   Node *node = 0L;

   AreaStruct area(0, 0, 0, 0);
   if ( !invalidArea(area, &firstNode, &lastNode) ||
        (area.eLine() < area.bLine() || (area.eLine() == area.bLine() && area.eCol() <= area.bCol())) //something strange has happened, like moving text with D&D inside the editor
      )
   {
//FIXME       logReparse(modifs, w);
     return parse(source, base, dtd, true);
   }

   kDebug(24000) << QString("Invalid area: %1,%2,%3,%4").arg(area.bLine()).arg(area.bCol()).arg(area.eLine()).arg(area.eCol()) << "\n";

   deleteNodes(firstNode->nextSibling(), lastNode/* FIXME, modifs*/);


   firstNode->child = 0L;
   Node *lastInserted = 0L;
   //this makes sure that the first found node it put right after the firstNode
   if (firstNode->next && firstNode->next == lastNode)
   {
      firstNode->next->prev = 0L;
      firstNode->next = 0L;
   }
   node = parseArea(area.bLine(), area.bCol(), area.eLine(), area.eCol(), &lastInserted, firstNode);

//FIXME   Node *swapNode = firstNode->nextSibling();
   Node *p = (lastInserted)?lastInserted->nextSibling():lastInserted;
/* FIXME  while(swapNode != p)
   {
      modif = new NodeModif();
      modif->setType(NodeModif::NodeAdded);
      modif->setLocation(kafkaCommon::getLocation(swapNode));
      modifs->addNodeModif(modif);
      swapNode = swapNode->nextSibling();
   }*/
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
     lastNode->deleteNode(m_base->groupStorage);
     nodeNum--;
     lastNode = 0L;
//FIXME       logReparse(modifs, w);
     return parse(source, base, dtd, true);
   }

   bool goUp;
   if (lastNode && lastInserted)
   {
      //merge the nodes if they are both of type Text or Empty
      if ( (lastInserted->tag->isType(Tag::Empty) || lastInserted->tag->isType(Tag::Text)) &&
            (lastNode->tag->isType(Tag::Empty) || lastNode->tag->isType(Tag::Text)))
      {
        if (lastNode->prev)
          lastNode->prev->next = 0L;
        lastNode->prev = lastInserted->prev;
        if (lastInserted->prev)
            lastInserted->prev->next = lastNode;
        lastNode->parent = lastInserted->parent;
//         lastInserted->tag->beginPos(area.bLine, area.bCol);
        area.start = lastInserted->tag->beginPos();
//         lastNode->tag->endPos(area.eLine, area.eCol);
        area.end = lastNode->tag->endPos();
        Tag *_tag = new Tag(*(lastNode->tag));
        lastNode->tag->setTagPosition(area);
        QString s = m_source->text(area);
        lastNode->tag->setStr(s);
        if (!s.simplified().isEmpty())
        {
          lastNode->tag->setType(Tag::Text);
        } else
        {
          lastNode->tag->setType(Tag::Empty);
        }
        if (lastInserted->parent && lastInserted->parent->child == lastInserted)
            //lastInserted->parent->child = lastInserted->next; lastInserted has no next!
           lastInserted->parent->child = lastNode;

        //here, lastNode is at the pos of lastInserted.
/* FIXME       modif = new NodeModif();
        modif->setType(NodeModif::NodeRemoved);
        modif->setLocation(kafkaCommon::getLocation(lastNode));*/
        if(lastInserted->prev)
          lastInserted->prev->next = 0L;
        if(lastInserted->parent && lastInserted->parent->child == lastInserted)
          lastInserted->parent->child = 0L;
        lastInserted->prev = 0L;
        lastInserted->next = 0L;
        lastInserted->parent = 0L;
        lastInserted->child = 0L;
        lastInserted->deleteNode(m_base->groupStorage);
//        lastInserted->detachNode();
/*FIXME
        modif->setNode(lastInserted);
        modifs->addNodeModif(modif);

        modif = new NodeModif();
        modif->setType(NodeModif::NodeModified);
        modif->setLocation(kafkaCommon::getLocation(lastNode));
        modif->setTag(_tag);
        modifs->addNodeModif(modif);
*/
        lastInserted = lastNode;
        lastNode = lastNode->nextNotChild();
      }

    node = lastInserted;

    QTag *qTag = 0L;
    while (node && lastNode)
    {
      qTag = 0L;
      goUp = ( node->parent &&
               ( (lastNode->tag->isType(Tag::XmlTagEnd) && lastNode->tag->doesClose(node->parent->tag) ) ||
                  node->parent->tag->single )
             );
      if (node->parent && !goUp)
      {
        qTag = QTag::tagFromDTD(m_dtd, node->parent->tag->name());
        if ( qTag )
        {
          QString searchFor = lastNode->tag->name(m_dtd->caseSensitive);
          if ( qTag->stoppingTags.contains( searchFor ) )
          {
            node->parent->tag->closingMissing = true; //parent is single...
            goUp = true;
          }
        }
      }
      if (goUp &&
          ( node->tag->name(m_dtd->caseSensitive) == node->parent->tag->name(m_dtd->caseSensitive) ))
          goUp = false; //it can happen that the tag closes the previous and not the parent

    if (goUp) //lastnode closes the node->parent
    {
        //handle cases like <ul><li></ul>
        if (lastNode->tag->isType(Tag::XmlTagEnd) &&
            !lastNode->tag->doesClose(node->parent->tag))
        {
          while ( node->parent->parent &&
                  lastNode->tag->doesClose(node->parent->parent->tag)
                )
          {
            node = node->parent;
          }
        } else
        if (qTag && !lastNode->tag->isType(Tag::XmlTagEnd))
        {
          //handle the case when a tag is a stopping tag for parent, and grandparent and so on. I'm not sure it's needed here, but anyway...
          Node *n = node->parent;
          QString searchFor = lastNode->tag->name(m_dtd->caseSensitive);
          while (qTag && n)
          {
            qTag = QTag::tagFromDTD(m_dtd, n->tag->name());
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
    }
    node = lastNode;
    lastNode = lastNode->nextNotChild();
   }
 }
/*   kDebug(24000)<< "END";
   Node::coutTree(baseNode,  2);
   kDebug(24000)<< "************* End User Modification *****************";*/
//FIXME   w->docUndoRedo->addNewModifsSet(modifs, undoRedo::SourceModif);
 }
  kDebug(24000) << "Rebuild: " << t.elapsed() << " ms \n";

//  treeSize = 0;
//  Node::coutTree(m_base->baseNode, 2);
//  kDebug(24000) << "Size of tree: " << treeSize;
// cout << "\n"; */

 m_saParser->init(m_base, m_source, m_dtd);
 if (detailed)
   QTimer::singleShot(0, this, SLOT(slotParseInDetail()));
 emit nodeTreeChanged();
}

void Parser::clearGroups()
{
  m_base->groupStorage->clear(&m_base->userTags);

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
  m_base->groupStorage->cleanUp(&m_base->userTags);

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
/* FIXME  uint listCount;
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
//FIXME          groupElement->node->tag->write()->userTagList.remove(groupElement->node->tag->name.toLower());
          delete it.data()[i]->node;
          delete it.data()[i];
        }
      }
    }
  }
  includedMap.clear();*/
}

void Parser::parseIncludedFiles()
{
#ifdef DEBUG_PARSER
  kDebug(24000) << "parseIncludedFiles";
#endif
  clearIncludedGroupElements();
  uint listCount;
//FIXME  if (write->url().isLocalFile())
  {
    listCount = ParserCommon::includedFiles.count();
    for (uint i = 0; i < listCount; i++)
    {
      parseIncludedFile(ParserCommon::includedFiles[i], ParserCommon::includedFilesDTD.at(i));
    }
    if (listCount > 0)
      m_parseIncludedFiles = false;
  }
  emit groupsParsed(m_source, m_base);
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
#if 0
FIXME
#ifdef DEBUG_PARSER
  kDebug(24000) << "parseIncludedFile: " << fileName;
#endif
  StructTreeGroup group;
  QString content;
  QFile file(fileName);
  if (file.open(IO_ReadOnly))
  {
    IncludedGroupElements *elements = &includedMap[fileName];
    QTextStream str(&file);
    QString encoding;
/*FIXME    KTextEditor::EncodingInterface* encodingIf = dynamic_cast<KTextEditor::EncodingInterface*>(write->doc());
    if (encodingIf)
      encoding = encodingIf->encoding();*/
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
        areaPos = content.indexOf(dtd->specialAreaStartRx, lastAreaPos);
        if (areaPos != -1)
        {
          foundStr = dtd->specialAreaStartRx.cap();
          specialEndStr = dtd->specialAreas[foundStr];
          int areaPos2 = content.indexOf(specialEndStr, areaPos);
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
            structPos = foundStr.indexOf(dtd->structBeginStr, structPos);
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
              pos = dtd->structRx.indexIn(foundStr, pos);
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
            int openBracketPos = foundStr.lastIndexOf(dtd->structKeywordsRx, structPos);
            openBracketPos = foundStr.indexOf('(', openBracketPos);
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
                pos = group.definitionRx.indexIn(foundStr, pos);
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
                    QString s2 = content.left(areaPos + pos);
                    int newLineNum = s2.contains('\n');
                    int tmpCol = s2.length() - s2.lastIndexOf('\n') - 1;
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
                      qTag->setName(s.left(s.indexOf('(')));
                      qTag->className = "";
                      if (groupElement->parentNode)
                        qTag->className = groupElement->parentNode->tag->name;
//FIXME                      write->userTagList.insert(s.toLower(), qTag);
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
#endif
}

void Parser::slotIncludedFileChanged(const QString& fileName)
{
#if 0
FIXME
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
            delete it.data()[i]->node;
            delete it.data()[i];
          }
        }
      }
      includedMap[fileName].clear();
      parseIncludedFile(fileName, dtd);
    }
  }
#endif
}


void Parser::parseForXMLGroup(Node *node)
{
  xmlGroupIt = node->tag->dtd()->xmlStructTreeGroups.find(node->tag->name().toLower());
  if (xmlGroupIt != node->tag->dtd()->xmlStructTreeGroups.end())
  {
    XMLStructGroup group = xmlGroupIt.value();
    Tag *newTag = new Tag(*node->tag);
    QString title = "";
    QStringList::Iterator end(group.attributes.end());
    for (QStringList::Iterator it = group.attributes.begin(); it != end; ++it)
    {
      if (newTag->hasAttribute(*it))
      {
          title.append(newTag->attributeValue(*it).left(100));
          title.append(" | ");
      }
    }
    title = title.left(title.length()-3);
    title.remove('\n');
    newTag->setName(title);

    GroupElement *groupElement = new GroupElement;
    groupElement->deleted = false;
    groupElement->tag = newTag;
    groupElement->node = node;
    groupElement->parentNode = 0L;
    groupElement->global = true;
    groupElement->group = const_cast<XMLStructGroup*>(&(xmlGroupIt.value()));
    m_base->groupStorage->insert(group.name, title, groupElement);
/*    node->m_groupElements.append(groupElement);
    GroupElementList* groupElementList = & (globalGroupMap[group.name + "|" + title]);
    groupElementList->append(groupElement);*/
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
      pos = text.indexOf(dtd->specialAreaStartRx, col);
      if (pos != -1)
      {
        foundText = dtd->specialAreaStartRx.cap();
        //Calculate the beginning coordinates
        s = text.left(pos);
        n = s.count('\n');
        bl = node_bl + n;
        if (n > 0)
        {
          bc = pos - s.lastIndexOf('\n') - 1;
        } else
        {
          bc = node_bc + pos;
        }
        //What is the closing string?
        specialEndStr = dtd->specialAreas[foundText];

        el = bl;
        ec = bc + foundText.length() - 1;
        AreaStruct area(bl, bc, el, ec);
        currentNode = ParserCommon::createScriptTagNode(area, foundText, dtd, startNode, currentNode);
        currentNode->specialInsideXml = true;

        found = true;
        AreaStruct area2(bl, bc, node_el, node_ec);
        int lastLine, lastCol;
        m_saParser->setSpecialInsideXml(true);
        currentNode = m_saParser->parseArea(area2, foundText, "", currentNode, true, true);
        m_saParser->setSpecialInsideXml(false);
        lastLine = m_saParser->lastParsedLine();
        lastCol = m_saParser->lastParsedColumn();
        col = m_source->text(node_bl, node_bc, lastLine, lastCol).length();
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

void Parser::appendUserTag(Tag *tag)
{
  if (tag->isType(Tag::XmlTag) && tag->validXMLTag)
  {
    QString tagName = tag->name().toLower();
    //add the new xml tags to the userTagList
    if (!tag->dtd()->isKnownTag(tagName))
    {
      QTag *newTag = m_base->userTags[tagName];
      bool insertNew = !newTag;
      if (insertNew)
      {
        newTag = new QTag();
        newTag->setName(tag->name());
        newTag->parentDTD = m_dtd;
      }
      int attrCount = tag->attrCount();
      for (int i = 0; i < attrCount; i++)
      {
        Attribute *attr = new Attribute;
        attr->name = tag->attribute(i);
        attr->values.append(tag->attributeValue(i));
        newTag->addAttribute(attr);
        delete attr;
      }
      if (insertNew)
      {
        m_base->userTags.insert(tagName, newTag);
      }
    }
  }

}

void Parser::slotParseInDetail()
{
  m_saParser->parseInDetail(false);
}

void Parser::synchParseInDetail()
{
  m_saParser->parseInDetail(false);
}

void Parser::setSAParserEnabled(bool enabled)
{
  m_saParser->setParsingEnabled(enabled);
}

#include "parser.moc"
