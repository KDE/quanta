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
#include <qstring.h>
#include <qregexp.h>
#include <qcstring.h>
#include <qstringlist.h>
#include <qstrlist.h>
#include <qdatetime.h>
#include <qfile.h>
#include <qvaluelist.h>
#include <qvaluestack.h>

//standard library includes
#include <stdio.h>
#include <ctype.h>
//#include <iostream.h>

//app includes
#include "parser.h"
#include "node.h"
#include "tag.h"
#include "../resource.h"
#include "../quantaview.h"
#include "../quantacommon.h"
#include "../document.h"
#include "../quanta.h"
#include "../qextfileinfo.h"
#ifdef BUILD_KAFKAPART
#include "../parts/kafka/kafkacommon.h"
#endif

//kde includes
#include <kapplication.h>
#include <kdebug.h>
#include <kdirwatch.h>
#include <kiconloader.h>
#include <klocale.h>

static const QChar space(' ');

Parser::Parser()
{
  m_node = 0L;
  write = 0L;
  oldMaxLines = 0;
  includeWatch = 0L;
  m_groups.clear();
  m_quotesRx = QRegExp("\"|'");
  m_parsingEnabled = true;
}

Parser::~Parser()
{
  delete includeWatch;
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
  textLine.append(write->text(startLine, startCol, startLine, write->editIf->lineLength(startLine)));
  if (line == endLine)
  {
    if (endCol > 0)
      textLine.truncate(endCol + 1);
    else
      textLine = "";
  }
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
      currentNode = createTextNode(currentNode, line, specialStartPos, parentNode);
      if (!rootNode)
          rootNode = currentNode;
      QString foundText = m_dtd->specialAreaStartRx.cap();
      //create a toplevel node for the special area
      AreaStruct area(line, specialStartPos, line, specialStartPos + foundText.length() - 1);
      Node *node = createScriptTagNode(area, foundText, m_dtd, parentNode, currentNode);
      if (node->tag->name.lower().startsWith("comment"))
        node->tag->type = Tag::Comment;

      if (!rootNode)
        rootNode = node;

      int el, ec;
      area.eLine = endLine;
      area.eCol = endCol;
      currentNode = parseSpecialArea(area, foundText, "", node, el, ec);
      line = el;
      textLine = getLine(line, endLine, endCol);
      col = ec;
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
      //fint he matching ">" in the document
      while (line <= endLine && openNum > 0)
      {
        textLine = getLine(line, endLine, endCol);
        for (uint i = sCol; i < textLine.length(); i++)
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
                if (!firstOpenFound)
                {
                  firstStartCol = i;
                  firstStartLine = line;
                  firstOpenFound = true;
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
        textLine = getLine(line, endLine, endCol);
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
      if (tagStr.right(2) == "/>")
      {
        tag->single = true;
        if (tag->name.length() > 1 && tag->name.endsWith("/"))
            tag->name.truncate(tag->name.length() - 1);
      }
      //the tag we found indicates the beginning of a special area, like <script type=... >
      if (m_dtd->specialTags.contains(tag->name.lower()))
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
        QString s = tag->attributeValue(m_dtd->specialTags[tag->name.lower()]);
        DTDStruct *dtd = dtds->find(s);
        if (!dtd)
            dtd = m_dtd;
        //a trick here: replace the node's DTD with this one
        DTDStruct *savedDTD = node->tag->dtd;
        node->tag->dtd = dtd;
        //now parse the special area
        int el, ec;
        area.bLine = area.eLine;
        area.bCol = area.eCol + 1;
        area.eLine = endLine;
        area.eCol = endCol;
        currentNode = parseSpecialArea(area, "", "</"+tag->name+"\\s*>", node, el, ec);
        //restore & set the new variables
        node->tag->dtd = savedDTD;
        line = el;
        textLine = getLine(line, endLine, endCol);
        col = ec;
        continue;
      }

      goUp = ( parentNode &&
               ( (tag->type == Tag::XmlTagEnd && QuantaCommon::closesTag(parentNode->tag, tag)
                ) ||
                  parentNode->tag->single )
             );
      if (parentNode && !goUp)
      {
        QTag *qTag = QuantaCommon::tagFromDTD(m_dtd, parentNode->tag->name);
        if ( qTag )
        {
          QString searchFor = (m_dtd->caseSensitive)?tag->name:tag->name.upper();
          searchFor.remove('/');
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
      currentNode = createTextNode(currentNode, tagStartLine, tagStartPos, parentNode);
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
              parentNode->child = node;
        }
      }
      if (!tag->single)
          parentNode = node;

      node->tag = tag;
      if (tag->type == Tag::NeedsParsing)
      {
        if (tag->name.lower().startsWith("comment"))
        {
          kdDebug(24000) << "COMMENT!" << endl;
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
      textLine = getLine(line, endLine, endCol);
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
    currentNode = parseSpecialArea(area, "", "", parentNode, el, ec);
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
    currentNode = createTextNode(currentNode, el, ec, parentNode);
  } else
  if (el != endLine || ec != endCol)
    currentNode = createTextNode(currentNode, endLine, endCol, parentNode);
  if (!rootNode)
    rootNode = currentNode;
  *lastNode = currentNode;
  return rootNode;
}

/** Parse the whole text from Document w and build the internal structure tree
    from Nodes */
Node *Parser::parse(Document *w)
{
  QTime t;
  t.start();

  if(!m_parsingEnabled)
    return baseNode;

  if (baseNode)
  {
    delete baseNode;
    baseNode = 0L;
    m_node = 0L;
  }

  Node *lastNode;
  write = w;
  m_dtd = w->defaultDTD();
  maxLines = w->editIf->numLines() - 1;
  clearGroups();
  parsingEnabled = true;
  nodeNum = 0;
  if (maxLines >= 0)
      m_node = parseArea(0, 0, maxLines, w->editIf->lineLength(maxLines) - 1, &lastNode);
  kdDebug(24000) << "Parsing time ("<< maxLines << " lines): " << t.elapsed() << " ms\n";
//  t.restart();
  t.restart();
  parseIncludedFiles();
  kdDebug(24000) << "External parser: " << t.elapsed() << " ms\n";
 /*
 treeSize = 0;
 coutTree(m_node, 2);
 kdDebug(24000) << "Size of tree: " << treeSize << endl;
 */

//FIXME: What is the use of two pointer to the same Node???
baseNode = m_node;

#ifdef BUILD_KAFKAPART
 //We need to reload Kafka to refresh the DOM::Node->Node links.
 //FIXME: make a function which only update the DOM::Node->Node links.
 quantaApp->view()->reloadKafkaView(true);
#endif

 emit nodeTreeChanged();
 return m_node;
}

/** Print the doc structure tree to the standard output.
    Only for debugging purposes. */

void Parser::coutTree(Node *node, int indent)
{
         QString output;
        int bLine, bCol, eLine, eCol, j;
        if(!node)
                kdDebug(24000)<< "undoRedo::coutTree() - bad node!" << endl;
        while (node)
        {
                output = "";
                output.fill('.', indent);
                node->tag->beginPos(bLine, bCol);
                node->tag->endPos(eLine, eCol);
                if (node->tag->type != Tag::Text)
                        output += node->tag->name.replace('\n'," ");
                else
                        output+= node->tag->tagStr().replace('\n'," ");
                kdDebug(24000) << output <<" (" << node->tag->type << ") at pos " <<
                        bLine << ":" << bCol << " - " << eLine << ":" << eCol << endl;
                for(j = 0; j < node->tag->attrCount(); j++)
                {
                        kdDebug(24000)<< " attr" << j << " " <<
                                node->tag->getAttribute(j).nameLine << ":" <<
                                node->tag->getAttribute(j).nameCol << " - " <<
                                node->tag->getAttribute(j).valueLine << ":" <<
                                node->tag->getAttribute(j).valueCol << endl;
                }

                if (node->child)
                        coutTree(node->child, indent + 4);
                node = node->next;
        }
}


/** No descriptions */
DTDStruct * Parser::currentDTD(int line, int col)
{
  DTDStruct *dtd = m_dtd;
  Node *node = nodeAt(line, col, false);
  if (node)
  {
    dtd = node->tag->dtd;
  }

  return dtd;
}

/** Returns the node for position (line, column). As more than one node can
contain the same area, it return the "deepest" node. */
Node *Parser::nodeAt(int line, int col, bool findDeepest)
{
  if (!write)
      return 0L;
  Node *node = m_node;
  int bl, bc, el, ec;
  int result;

  while (node)
  {
    node->tag->beginPos(bl, bc);
    bc++;
    if (node->nextNotChild())
    {
      node->nextNotChild()->tag->beginPos(el, ec);
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
        break; //we found the node
      }
    } else
    if (result == -1)
    {
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
    if (n && n->nextSibling()) //don't set it to the last, always empty node
      node = n;
  }
  return node;
}
void Parser::logReparse(NodeModifsSet *modifs, Document *w)
{
#ifdef BUILD_KAFKAPART
  NodeModif *modif;
  if (baseNode)
  {
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
#else
  Q_UNUSED(modifs);
  Q_UNUSED(w);
#endif
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
#ifdef BUILD_KAFKAPART
  NodeModif *modif;
#endif
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
    if (next)
      next->prev = prev;
    if (prev && prev->next == node)
    {
        prev->next = next;
    }
    if (next && next->closesPrevious)
      next->closesPrevious = false;

#ifdef BUILD_KAFKAPART
    modif = new NodeModif();
    modif->setType(NodeModif::NodeRemoved);
    modif->setLocation(kafkaCommon::getLocation(node));
    node->parent = 0L;
    node->next = 0L;
    node->prev = 0L;
    node->child = 0L;
    modif->setNode(node);
#else
    delete node;
    nodeNum--;
#endif
    node = 0L;
    i = 0;
    j = 0;
    if (child)
    {
      Node *n = child;
      Node *m = child;
      while (n)
      {
        m = n;
        n->parent = parent;
        n = n->next;
        i++;
      }
      if (prev)
      {
        if (!closesPrevious)
        {
          child->prev = prev;
          prev->next = child;
        }
      } else
      {
        if (parent)
            parent->child = child;
      }
      prev = m;
    }

    if (prev)
    {
      node = prev;
      node->next = next;
      if (next)
          next->prev = prev;
      while (node->child)
      {
        node = node->child;
        while (node->next)
              node = node->next;
      }
      if (next && closesPrevious) //if there is a node after the deleted one and the deleted node closed the previous one
      {
        if (prev->child) //if the previous node has a child, append the next node after the last child
        {
            next->prev = node;
            node->next = next;
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
    } else
    {
      if (next && closesPrevious)
      {
        Node *n = next;
        while (n)
        {
          n->parent = prev;
          n = n->next;
        }
        next->prev = 0L;
      }
      node = parent;
      if (node)
          node->child = next;
    }
#ifdef BUILD_KAFKAPART
    modif->setChildrenMovedUp(i);
    modif->setNeighboursMovedDown(j);
    modifs->addNodeModif(modif);
#endif
    node = nextNode;
/*
    kdDebug(24000)<< "Node removed!" << endl;
    coutTree(m_node, 2);
*/
  }
#ifndef BUILD_KAFKAPART
  Q_UNUSED(modifs);
#endif
}

Node *Parser::rebuild(Document *w)
{
 QTime t;
 t.start();
#ifdef BUILD_KAFKAPART
 NodeModifsSet *modifs = new NodeModifsSet();
 NodeModif *modif;
#endif

#ifdef BUILD_KAFKAPART
// kdDebug(24000)<< "Node *Parser::rebuild()" << endl;
 modifs->setIsModified(w->isModified());
#endif
 /**kdDebug(24000)<< "************* Begin User Modification *****************" << endl;
  //debug!
  coutTree(m_node, 2);*/
 if (w != write || !m_node) //the document has changed or the top node does not exists => parse completely
 {
#ifdef BUILD_KAFKAPART
  logReparse(modifs, w);
#endif
   return parse(w);
 } else
 {
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
#ifdef BUILD_KAFKAPART
     logReparse(modifs, w);
#endif
     return parse(w);
   }

   kdDebug(24000) << QString("Invalid area: %1,%2,%3,%4").arg(area.bLine).arg(area.bCol).arg(area.eLine).arg(area.eCol) << "\n";

#ifdef BUILD_KAFKAPART
   deleteNodes(firstNode->nextSibling(), lastNode, modifs);
#else
   deleteNodes(firstNode->nextSibling(), lastNode, 0L);
#endif

   firstNode->child = 0L;
   Node *lastInserted = 0L;
   node = parseArea(area.bLine, area.bCol, area.eLine, area.eCol, &lastInserted, firstNode);

#ifdef BUILD_KAFKAPART
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
#endif
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
     delete lastNode;
     nodeNum--;
     lastNode = 0L;
#ifdef BUILD_KAFKAPART
   logReparse(modifs, w);
#endif
   return parse(w);
   }

   bool goUp;
   if (lastNode && lastInserted)
   {
      //merge the nodes if they are both of type Text or Empty
      if ( (lastInserted->tag->type == Tag::Empty || lastInserted->tag->type == Tag::Text) &&
            (lastNode->tag->type == Tag::Empty || lastNode->tag->type == Tag::Text))
      {
        lastNode->prev = lastInserted->prev;
        if (lastInserted->prev)
            lastInserted->prev->next = lastNode;
        lastNode->parent = lastInserted->parent;
        lastInserted->tag->beginPos(area.bLine, area.bCol);
        lastNode->tag->endPos(area.eLine, area.eCol);
#ifdef BUILD_KAFKAPART
        Tag *_tag = new Tag(*(lastNode->tag));
#endif
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
#ifdef BUILD_KAFKAPART
        //here, lastNode is at the pos of lastInserted.
        modif = new NodeModif();
	modif->setType(NodeModif::NodeRemoved);
        modif->setLocation(kafkaCommon::getLocation(lastNode));
        lastInserted->prev = 0L;
        lastInserted->next = 0L;
        lastInserted->parent = 0L;
        lastInserted->child = 0L;
        modif->setNode(lastInserted);
        modifs->addNodeModif(modif);

	modif = new NodeModif();
	modif->setType(NodeModif::NodeModified);
        modif->setLocation(kafkaCommon::getLocation(lastNode));
        modif->setTag(_tag);
        modifs->addNodeModif(modif);
#else
	lastInserted->removeAll = false;
        delete lastInserted;
        nodeNum--;
#endif
        lastInserted = lastNode;
        lastNode = lastNode->nextNotChild();
      }

    node = lastInserted;

    while (node && lastNode)
    {
      goUp = ( node->parent &&
               ( (lastNode->tag->type == Tag::XmlTagEnd && QuantaCommon::closesTag(node->parent->tag, lastNode->tag) ) ||
                  node->parent->tag->single )
             );
      if (node->parent && !goUp)
      {
        QTag *qTag = QuantaCommon::tagFromDTD(m_dtd, node->parent->tag->name);
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
/*   kdDebug(24000)<< "END"<< endl;
   coutTree(baseNode,  2);
   kdDebug(24000)<< "************* End User Modification *****************" << endl;*/
#ifdef BUILD_KAFKAPART
   w->docUndoRedo->addNewModifsSet(modifs, undoRedo::SourceModif);
#endif
 }
  kdDebug(24000) << "Rebuild: " << t.elapsed() << " ms \n";

/* treeSize = 0;
 coutTree(m_node, 2);
 kdDebug(24000) << "Size of tree: " << treeSize << endl; */
// cout << "\n";

 emit nodeTreeChanged();
 return m_node;
}

void Parser::clearGroups()
{
  GroupElementMapList::Iterator it;
  GroupElementList::Iterator elementIt;
  GroupElementList *list;
  for (it = m_groups.begin(); it != m_groups.end(); ++it)
  {
    list = & it.data();
    //Clear the group element list and also remove the group tag which
    //was created in parseForXMLGroup/parseForScriptGroup methods.
    for (elementIt = list->begin(); elementIt != list->end(); ++elementIt)
    {
      (*elementIt).node->groupElementLists.clear();
      (*elementIt).node->groupTag = 0L;
      delete (*elementIt).tag;
    }
  }
  m_groups.clear();
  includedFiles.clear();
  includedFilesDTD.clear();
  selectors.clear();
  delete includeWatch;
  includeWatch = new KDirWatch();
  connect(includeWatch, SIGNAL(dirty(const QString&)), SLOT(slotIncludedFileChanged(const QString&)));
}

void Parser::parseIncludedFiles()
{
  IncludedGroupElementsMap::Iterator includedMapIt;
  uint listCount;
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
          delete it.data()[i].node;
        }
      }
    }
  }
  includedMap.clear();
  if (write->url().isLocalFile())
  {
    listCount = includedFiles.count();
    for (uint i = 0; i < listCount; i++)
    {
      parseIncludedFile(includedFiles[i], includedFilesDTD.at(i));
    }
  }
}

void Parser::parseIncludedFile(const QString& fileName, DTDStruct *dtd)
{
  StructTreeGroup group;
  QString content;
  QFile file(fileName);
  if (file.open(IO_ReadOnly))
  {
    IncludedGroupElements *elements = &includedMap[fileName];
    QTextStream str(&file);
    str.setEncoding(QTextStream::UnicodeUTF8);
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
          removeCommentsAndQuotes(foundStr, dtd);
          //find the structure blocks and remove the text inside them
          int structPos = 0;
          while (structPos !=-1)
          {
            structPos = foundStr.find(dtd->structBeginStr, structPos);
            if (structPos != -1)
            {
              int openNum = 1;
              int pos = structPos + dtd->structBeginStr.length();
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
              foundStr.replace(structPos, pos - structPos + 1, &space, pos - structPos + 1);
              int openBracketPos = foundStr.findRev(dtd->structKeywordsRx, structPos);
              openBracketPos = foundStr.find('(', openBracketPos);
              openNum = 1;
              if (openBracketPos != -1)
              {
                int closeBracketPos = openBracketPos;
                while (closeBracketPos < structPos && openNum !=0)
                {
                  if (foundStr[closeBracketPos] == '(')
                      openNum++;
                  if (foundStr[closeBracketPos] == ')')
                      openNum--;
                  closeBracketPos++;
                }
                foundStr.replace(openBracketPos, closeBracketPos - openBracketPos, &space, closeBracketPos - openBracketPos);
              }

              structPos =  pos + 1;
            }
          }
          QValueList<StructTreeGroup>::Iterator it;
          for (it = dtd->structTreeGroups.begin(); it != dtd->structTreeGroups.end(); ++it)
          {
            group = *it;
            int pos = 0;
            while (pos != -1)
            {
              pos = group.searchRx.search(foundStr, pos);
              if (pos != -1)
              {
                QString s = content.mid(areaPos + pos, group.searchRx.matchedLength());
                pos += s.length();
                s.remove(group.clearRx);
                if (!(*elements)[group.name].contains(s))
                {
                  Tag *tag = new Tag();
                  tag->name = s;
                  QString s2 = content.left(areaPos + pos);
                  int newLineNum = s2.contains('\n');
                  int tmpCol = s2.length() - s2.findRev('\n') - 1;
                  tag->setTagPosition(newLineNum, tmpCol - s.length(), newLineNum, tmpCol);
                  Node *node = new Node(0L);
                  node->tag = tag;
                  node->fileName = fileName;
                  GroupElement groupElement;
                  groupElement.node = node;
                  groupElement.parentNode = 0L;
                  GroupElementList *groupElementList = &(*elements)[group.name][s];
                  groupElementList->append(groupElement);
                }
              }
            }
          } //for
        } //if (areaPos != -1)
      }// while (areaPos != -1)
    }
  }
}

void Parser::slotIncludedFileChanged(const QString& fileName)
{
  int pos = includedFiles.findIndex(fileName);
  if (pos != -1)
  {
    DTDStruct *dtd = includedFilesDTD.at(pos);
    if (dtd)
    {
      includedMap[fileName].clear();
      parseIncludedFile(fileName, dtd);
    }
  }
}

void Parser::removeCommentsAndQuotes(QString &str, DTDStruct *dtd)
{
 //Replace all the commented strings and the escaped quotation marks (\", \')
 // with spaces so they will not mess up our parsing
 int pos = 0;
 int l;
 QString s;
 while (pos != -1)
 {
   pos = dtd->commentsStartRx.search(str, pos);
   if (pos != -1)
   {
     s = dtd->commentsStartRx.cap();
     if (s == "\\\"" || s == "\\'")
     {
       str[pos] = space;
       str[pos+1] = space;
       pos += 2;
     } else
     {
       s = dtd->comments[s];
       l = str.find(s, pos);
       l = (l == -1) ? str.length() : l;
       for (int i = pos; i < l ; i++)
       {
         str[i] = space;
       }
       pos = l + s.length();
     }
   }
 }

 //Now replace the quoted strings with spaces
 const QRegExp strRx("(\"[^\"]*\"|'[^']*')");
 pos = 0;
 while (pos != -1)
 {
   pos = strRx.search(str, pos);
   if (pos != -1)
   {
    l = strRx.matchedLength();
    for (int i = pos; i < pos + l ; i++)
    {
      str[i] = space;
    }
    pos += l;
   }
 }

}

void Parser::parseForXMLGroup(Node *node)
{
  xmlGroupIt = node->tag->dtd->xmlStructTreeGroups.find(node->tag->name.lower());
  if (xmlGroupIt != node->tag->dtd->xmlStructTreeGroups.end())
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
    node->groupTag = newTag;
    node->group = const_cast<XMLStructGroup*>(&(xmlGroupIt.data()));
  }
}

void Parser::parseForScriptGroup(Node *node)
{
  int bl, bc, el, ec;
  int pos;
  QString title;
  QString tmpStr;
  StructTreeGroup group;
  GroupElement groupElement;
  GroupElementList* groupElementList;
  KURL baseURL = QExtFileInfo::path(write->url());
  QString str = node->tag->cleanStr;
  QString tagStr = node->tag->tagStr();
  DTDStruct* dtd = node->tag->dtd;
  node->tag->beginPos(bl, bc);
  QValueList<StructTreeGroup>::Iterator it;
  for (it = dtd->structTreeGroups.begin(); it != dtd->structTreeGroups.end(); ++it)
  {
    group = *it;
    if (!group.hasSearchRx ||
        node->tag->type == Tag::XmlTag ||
        node->tag->type == Tag::XmlTagEnd ||
        node->tag->type == Tag::Comment ||
        node->tag->type == Tag::Empty ||
        ( group.tagType != Tag::Text && node->tag->type != group.tagType)
        )
      continue;
    pos = 0;
    group.searchRx.setMinimal(group.isMinimalSearchRx);
    while (pos != -1)
    {
      pos = group.searchRx.search(str, pos);
      if (pos != -1) //the Node is part of this group
      {
        title = tagStr.mid(pos, group.searchRx.matchedLength());
        node->tag->beginPos(bl, bc);
        tmpStr = tagStr.left(pos);
        int newLines = tmpStr.contains('\n');
        bl += newLines;
        int l = tmpStr.findRev('\n'); //the last EOL
        bc = (l == -1) ? bc + pos : pos - l - 1;
        newLines = title.contains('\n');
        l = title.length();
        el = bl + newLines;
        ec = (newLines > 0) ? l - title.findRev('\n') : bc + l - 1;
        pos += l;
        AreaStruct area(bl, bc, el, ec);
        //get the list of elements which are present in this group and
        //have the same title. For example get the list of all group
        //element which are variable and the matched string was "$i"
        QString s = title;
        groupElementList = & (m_groups[group.name + "|" + s.remove(group.clearRx)]);
        GroupElementList::Iterator elIt;
        //Create a new tag which point to the exact location of the matched string.
        //For example when the group defined PHP variables it
        //points to "$i" in a node which originally contained "print $i + 1"
        Tag *newTag = new Tag(*node->tag);
        newTag->setTagPosition(area);
        newTag->setStr(title);
        newTag->name = s;
        
        if (dtd->name == "text/css" && group.name == "Selectors")
        {
          selectors.append(s);
        }
        if (dtd->name == "php" && group.name == "Functions")
        {
          QTag *qTag = write->userTagList.find(s.lower());
          if (!qTag)
          {
            QTag *qTag = new QTag();
            qTag->setName(s.left(s.find('(')));
            write->userTagList.insert(s.lower(), qTag);
          }          
        }
        
        groupElement.tag = newTag;
        groupElement.node = node;
        //Find out if the current node is inside a script structure or not.
        //This is used to define local/global scope of the group elements.
        Node *tmpNode = node;
        while (tmpNode && tmpNode->tag->dtd == dtd && tmpNode->tag->type != Tag::ScriptStructureBegin)
        {
          tmpNode = tmpNode->parent;
        }
        if (tmpNode && tmpNode->tag->type == Tag::ScriptStructureBegin)
        {
          groupElement.parentNode = tmpNode;
        } else
        {
          groupElement.parentNode = 0L;
        }
        groupElement.global = true;
        tmpNode = node;
        while (tmpNode && tmpNode->tag->dtd == dtd)
        {
          if ( tmpNode->tag->type == Tag::ScriptStructureBegin && tmpNode->tag->dtd->localScopeKeywordsRx.search(tmpNode->tag->cleanStr) != -1)
          {
            groupElement.global = false;
            groupElement.parentNode = tmpNode;
            break;
          }
          tmpNode = tmpNode->parent;
        }
        //store the pointer to the group element list where this node was put
        //used to clear the corresponding entry from the group element lists
        //when the node is deleted (eg. $i was deleted, so it should be deleted
        //from the "variables | $i" group element list as well)
        node->groupElementLists.append(groupElementList);
        groupElementList->append(groupElement);
        //if a filename may be present in the title, extract it
        if (group.hasFileName && group.parseFile)
        {
          s.remove(group.fileNameRx);
          KURL url;
          QuantaCommon::setUrl(url, s.stripWhiteSpace());
          url = QExtFileInfo::toAbsolute(url, baseURL);
          includedFiles += url.path();
          includedFilesDTD.append(dtd);
          includeWatch->addFile(url.path());
        }
        node->groupTag = newTag;
      }
    }
  }
}

Node* Parser::parseSpecialArea(const AreaStruct &specialArea,
                               const QString &areaStartString,
                               const QString &forcedAreaEndString,
                               Node *parentNode,
                               int &lastLine, int &lastCol)
{
  int startLine = specialArea.bLine;
  int startCol = specialArea.bCol;
  int endLine = specialArea.eLine;
  int endCol = specialArea.eCol;
  //kdDebug(24000) << QString("Starting to parse at %1, %2 for %3").arg(startLine).arg(startCol).arg(areaStartString) << endl;
  QString areaEndString;
  bool searchForAreaEnd = false;
  bool searchForForcedAreaEnd = false;
  QRegExp forcedAreaRx;
  DTDStruct *dtd = 0L;
  if (parentNode && !areaStartString.isEmpty())
  {
    dtd = dtds->find(parentNode->tag->dtd->specialAreaNames[areaStartString]);
    areaEndString = parentNode->tag->dtd->specialAreas[areaStartString];
    searchForAreaEnd = true;
  }
  if (!forcedAreaEndString.isEmpty())
  {
    forcedAreaRx.setPattern(forcedAreaEndString);
    forcedAreaRx.setCaseSensitive(m_dtd->caseSensitive);
    searchForForcedAreaEnd = true;
    searchForAreaEnd = false;
    if (parentNode)
      dtd = parentNode->tag->dtd;
  }
  bool searchContent = true;
  if (parentNode && parentNode->tag->type == Tag::Comment)
    searchContent = false;
  if (!dtd)
  {
    if (parentNode)
      dtd = parentNode->tag->dtd; //fallback, usually when the special area is a comment
    else
      dtd = m_dtd; //fallback when there is no parentNode
  }
  bool searchForSpecialAreas = (dtd->specialAreas.count() > 0);
  bool contextFound;
  int col = startCol + areaStartString.length();
  int line = startLine;
  QString textLine = write->text(startLine, 0, startLine, write->editIf->lineLength(startLine));
  if (line == endLine)
  {
    if (endCol > 0)
      textLine.truncate(endCol);
    else
      textLine = "";
  }
  QString searchedString;
  ContextStruct context;
  QValueStack<ContextStruct> contextStack;
  ContextStruct previousContext;
  previousContext.type = Unknown;
  ContextStruct currentContext;
  currentContext.type = Text;
  currentContext.area.bLine = line;
  currentContext.area.bCol = col;
  currentContext.area.eLine = currentContext.area.eCol = -1;
  currentContext.parentNode = parentNode;
  Node *currentNode = parentNode;
  while (line <= endLine)
  {
    contextFound = false;
    switch (currentContext.type)
    {
      case Group:
      case Text: {
        int areaEndPos = -1;
        int quotedStringPos = -1;
        int commentPos = -1;
        int groupKeywordPos = -1;
        if (searchContent || (parentNode && parentNode->tag->dtd->family == Xml))
        {
          //search for different contexts
          if (searchContent) //search for quoted strings, comments, groups only in non-comment special areas
          {
            quotedStringPos = textLine.find(m_quotesRx, col); //quoted strings
            searchedString = textLine.left(quotedStringPos);
            commentPos = searchedString.find(dtd->commentsStartRx, col); //comments
            searchedString = textLine.left(commentPos);
            groupKeywordPos = searchedString.find(dtd->structRx, col); //groups, like { }
          } else
            searchedString = textLine;
          int specialAreaPos = -1;
          if (searchForSpecialAreas)  //special area inside special area
          {
            searchedString = textLine.left(groupKeywordPos);
            specialAreaPos = searchedString.find(dtd->specialAreaStartRx, col);
          }
          if (searchForAreaEnd) //the end of the special area
          {
            searchedString = textLine.left(specialAreaPos);
            areaEndPos = searchedString.find(areaEndString, col);
          } else
          if (searchForForcedAreaEnd) //the end of the special area if a forcing string was specified
          {
            searchedString = textLine.left(specialAreaPos);
            areaEndPos = searchedString.find(forcedAreaRx, col);
            if (areaEndPos != -1)
              areaEndString = forcedAreaRx.cap();
          }
          //check which context was found first
          if (quotedStringPos != -1)  //is it a quoted string?
          {
            if ( (quotedStringPos < commentPos || commentPos == -1) &&
                (quotedStringPos < groupKeywordPos || groupKeywordPos == -1) &&
                (quotedStringPos < specialAreaPos || specialAreaPos == -1) &&
                (quotedStringPos < areaEndPos || areaEndPos == -1) )
            {
              context.type = QuotedString;
              context.area.bCol = quotedStringPos;
              context.startString = textLine.mid(quotedStringPos, 1);
              contextFound = true;
            }
          }
          if (!contextFound && commentPos != -1)  //is it a comment?
          {
            if ( (commentPos < groupKeywordPos || groupKeywordPos == -1) &&
                (commentPos < specialAreaPos || specialAreaPos == -1) &&
                (commentPos < areaEndPos || areaEndPos == -1) )
            {
              context.type = Comment;
              context.area.bCol = commentPos;
              context.startString = dtd->commentsStartRx.cap();
              contextFound = true;
            }
          }
          if (!contextFound && groupKeywordPos != -1) //is it a group structure?
          {
            if  ( (groupKeywordPos < specialAreaPos || specialAreaPos == -1) &&
                  (groupKeywordPos < areaEndPos || areaEndPos == -1) )
            {
              QString foundText = dtd->structRx.cap();
              if (foundText == dtd->structBeginStr)
              {
                context.type = Group;
                context.area.bCol = groupKeywordPos;
                context.startString = foundText;
                //create a text node until the struct. beginning
                currentContext.area.eLine = line;
                currentContext.area.eCol = groupKeywordPos + foundText.length() - 1;
                if (currentNode &&
                    (currentNode->tag->type == Tag::Text ||
                     currentNode->tag->type == Tag::Empty) )
                  appendAreaToTextNode(currentContext.area, currentNode);
                else
                  currentNode = createTextNode(currentNode, line, currentContext.area.eCol + 1, currentContext.parentNode);

                currentNode->tag->type = Tag::ScriptStructureBegin;
                currentNode->tag->single = false;
                currentNode->insideSpecial = true;
                currentContext.lastNode = currentNode;

                contextStack.push(currentContext);
                currentContext.parentNode = currentNode;
                col = context.area.bCol + context.startString.length();
                currentContext.area.bLine = line;
                currentContext.area.bCol = col;
                currentContext.type = Group;
                continue;
              } else  //it's a closing group structure element (like "}")
              {
                if (currentContext.type != Group)
                {
                  col = groupKeywordPos + foundText.length();
                  continue;
                }
                currentContext.area.eLine = line;
                currentContext.area.eCol = groupKeywordPos - 1;
                //kdDebug(24000) << QString("Group Struct context: %1, %2, %3, %4").arg( currentContext.bLine).arg(currentContext.bCol).arg(currentContext.eLine).arg(currentContext.eCol) << endl;

                if (currentNode &&
                    (currentNode->tag->type == Tag::Text ||
                     currentNode->tag->type == Tag::Empty) )
                  appendAreaToTextNode(currentContext.area, currentNode);
                else
                  currentNode = createTextNode(currentNode, line, groupKeywordPos, currentContext.parentNode);
                currentNode->insideSpecial = true;

                previousContext = contextStack.pop();
                currentContext.parentNode = previousContext.parentNode;
                currentContext.lastNode = previousContext.lastNode;
                currentContext.type = previousContext.type;
                currentContext.area.bLine = line;
                currentContext.area.bCol =  groupKeywordPos + foundText.length();
                currentContext.area.eLine = currentContext.area.eCol = -1;
                currentContext.startString = previousContext.startString;
                col = currentContext.area.bCol;

                Tag *tag = new Tag();
                tag->name = foundText;
                tag->setStr(foundText);
                tag->setWrite(write);
                tag->setTagPosition(line, groupKeywordPos, line, col);
                tag->dtd = dtd;
                tag->type = Tag::ScriptStructureEnd;
                tag->single = true;
                Node *node = new Node(currentContext.parentNode);
                nodeNum++;
                node->tag = tag;
                node->insideSpecial = true;
                if (currentContext.parentNode && !currentContext.parentNode->child)
                {
                  currentContext.parentNode->child = node;
                }
                else if (currentContext.lastNode)
                {
                  node->prev = currentContext.lastNode;
                  currentContext.lastNode->next = node;
                }
                currentNode = node;

                continue;
              }
            }
          }
          if (!contextFound && specialAreaPos != -1) //is it a special area?
          {
            if (specialAreaPos < areaEndPos || areaEndPos == -1)
            {
              QString foundText = dtd->specialAreaStartRx.cap();
              currentContext.area.eLine = line;
              currentContext.area.eCol = specialAreaPos - 1;
              if ( currentNode &&
                  (currentNode->tag->type == Tag::Text ||
                   currentNode->tag->type == Tag::Empty) )
                appendAreaToTextNode(currentContext.area, currentNode);
              else
                currentNode = createTextNode(currentNode, line, specialAreaPos, currentContext.parentNode);
              currentNode->insideSpecial = true;
              //create a toplevel node for the included special area
              AreaStruct area(line, specialAreaPos, line, specialAreaPos + foundText.length() - 1);
              Node *node = createScriptTagNode(area, foundText, dtd, currentContext.parentNode, currentNode);

            //  //kdDebug(24000) << "Going down one level..." << endl;
              AreaStruct area2(line, specialAreaPos, endLine, endCol);
              currentNode = parseSpecialArea(area2, foundText, "", node, line, col);
              currentContext.area.bLine = line;
              currentContext.area.bCol = col + 1;
              textLine = getLine(line, endLine, endCol);
              col++;
              continue;
            }
          }
        } else //when we look only for the area end string
        if (searchForAreaEnd)
        {
          areaEndPos = textLine.find(areaEndString, col);
        } else
        if (searchForForcedAreaEnd)
        {
          areaEndPos = textLine.find(forcedAreaRx, col);
          if (areaEndPos != -1)
            areaEndString = forcedAreaRx.cap();
        }

        if (!contextFound && areaEndPos != -1) //did we find the end of the special area?
        {
          lastLine = line;
          lastCol = areaEndPos + areaEndString.length() - 1;

          currentContext.area.eLine = line;
          currentContext.area.eCol = areaEndPos - 1;
          if ( currentNode &&
              (currentNode->tag->type == Tag::Text ||
               currentNode->tag->type == Tag::Empty) )
            appendAreaToTextNode(currentContext.area, currentNode);
          else
            currentNode = createTextNode(currentNode, line, areaEndPos, parentNode);
          currentNode->insideSpecial = true;
          //kdDebug(24000) << QString("Special area %1 ends at %2, %3").arg(dtd->name).arg(line).arg(lastCol) << endl;

          //create a closing node for the special area
          Tag *tag = new Tag();
          tag->setTagPosition(line, areaEndPos, line, lastCol);
          tag->parse(areaEndString, write);
          tag->dtd = dtd;
          tag->type = Tag::XmlTagEnd;
          tag->validXMLTag = false; //FIXME: this is more or less a workaround. We should introduce and handle Tag::ScriptTagEnd
          tag->single = true;
          //at this point parentNode = the opening node of the special area (eg. <?)
          //and it should always exist
          Node *node = new Node(parentNode->parent);
          nodeNum++;
          parentNode->next = node;
          node->prev = parentNode;
          node->tag = tag;
          node->closesPrevious = true;

          Node *n = parentNode->child;
          while (n)
          {
            if (n->tag->type == Tag::Text || n->tag->type == Tag::ScriptStructureBegin)
              parseForScriptGroup(n);
            if (n != currentNode)  
              n = n->nextSibling();
            else 
              break;  
          }

          return node;
        }
        if (contextFound)
        {
          context.area.bLine = line;
          context.area.eLine = context.area.eCol = -1;
          context.parentNode = currentContext.parentNode;
          currentContext.area.eLine = context.area.bLine;
          currentContext.area.eCol = context.area.bCol - 1;
       //   currentContext.parentNode = parentNode;
          contextStack.push(currentContext);

          if (currentNode &&
             (currentNode->tag->type == Tag::Text || currentNode->tag->type == Tag::Empty) )
          {
            appendAreaToTextNode(currentContext.area, currentNode);
            currentNode->insideSpecial = true;
          } else
          {
            //create a tag from the currentContext
            Tag *tag = new Tag(currentContext.area, write, dtd);
            QString tagStr = tag->tagStr();
            tag->cleanStr = tagStr;
            removeCommentsAndQuotes(tag->cleanStr, dtd);
            if (tagStr.simplifyWhiteSpace().isEmpty())
            {
              tag->type = Tag::Empty;
            } else
            {
              tag->type = Tag::Text;
            }
            tag->single = true;
            //create a node with the above tag
            Node *node = new Node(currentContext.parentNode);
            nodeNum++;
            node->tag = tag;
            node->insideSpecial = true;
            if (currentContext.parentNode && !currentContext.parentNode->child)
            {
              currentContext.parentNode->child = node;
            }
            else if (currentNode)
            {
              node->prev = currentNode;
              currentNode->next = node;
            }
            currentNode = node;
          }
          //kdDebug(24000) << QString("%1 context: %2, %3, %4, %5").arg(currentContext.type).arg( currentContext.bLine).arg(currentContext.bCol).arg(currentContext.eLine).arg(currentContext.eCol) << endl;

          currentContext = context;
          col = context.area.bCol + context.startString.length();
        } else
        {
          line++;
          col = 0;
          textLine = getLine(line, endLine, endCol);
        }
        break;
      }
     case QuotedString:
      {
       //TODO: find a better, faster solution...
        QRegExp endQuotationRx("[^\\\\]" + currentContext.startString +"|^" + currentContext.startString);
        QString s = textLine.mid(col);
        int pos = s.find(endQuotationRx);
        if (pos != -1)
        {
          pos += col;
          if (pos != 0)
            pos++;
          currentContext.area.eLine = line;
          currentContext.area.eCol = pos;
          //kdDebug(24000) << QString("Quoted String context: %1, %2, %3, %4").arg( currentContext.bLine).arg(currentContext.bCol).arg(currentContext.eLine).arg(currentContext.eCol) << endl;
          if ( currentNode  &&
              (currentNode->tag->type == Tag::Text ||
               currentNode->tag->type == Tag::Empty) )
            appendAreaToTextNode(currentContext.area, currentNode);
          else
            currentNode = createTextNode(0L, line, pos, currentContext.parentNode);
          currentNode->insideSpecial = true;

          previousContext = contextStack.pop();
          currentContext.parentNode = previousContext.parentNode;
          currentContext.type = previousContext.type;
          currentContext.area.bLine = line;
          currentContext.area.bCol = pos + 1;
          currentContext.area.eLine = currentContext.area.eCol = -1;
          currentContext.startString = previousContext.startString;
          col = pos + 1;
        } else
        {
          line++;
          col = 0;
          textLine = getLine(line, endLine, endCol);
        }
        break;
      }
     case Comment:
      {
        int pos = textLine.find(dtd->comments[currentContext.startString], col);
        if (pos == -1 && dtd->comments[currentContext.startString] == "\n")
          pos = textLine.length();
        if (pos != -1)
        {
          currentContext.area.eLine = line;
          currentContext.area.eCol = pos + dtd->comments[currentContext.startString].length() - 1;
          currentContext.type = previousContext.type;
          //kdDebug(24000) << QString("Comment context: %1, %2, %3, %4").arg( currentContext.bLine).arg(currentContext.bCol).arg(currentContext.eLine).arg(currentContext.eCol) << endl;

          //create a tag with the comment
          Tag *tag = new Tag(currentContext.area, write, dtd);
          tag->type = Tag::Comment;
          tag->single = true;
          //create a node with the above tag
          Node *node = new Node(currentContext.parentNode);
          nodeNum++;
          node->tag = tag;
          node->insideSpecial = true;
          if (currentNode)
          {
            currentNode->next = node;
            node->prev = currentNode;
          }
          currentNode = node;

          previousContext = contextStack.pop();
          currentContext.parentNode = previousContext.parentNode;
          currentContext.type = previousContext.type;
          currentContext.area.bLine = line;
          currentContext.area.bCol = currentContext.area.eCol + 1;
          currentContext.area.eLine = currentContext.area.eCol = -1;
          currentContext.startString = previousContext.startString;
          col = currentContext.area.bCol;
        } else
        {
          line++;
          col = 0;
          textLine = getLine(line, endLine, endCol);
        }
        break;
      }
     default:
      {
        line++;
        col = 0;
        textLine = getLine(line, endLine, endCol);
      }
   }

  }
  if (!currentNode)
  {
    currentNode = createTextNode(parentNode, endLine, endCol, parentNode);
    currentNode->insideSpecial = true;
  }
  Node *n;
  if (parentNode)
  {
     n = parentNode->child;
  } else
  {
    n = currentNode;
    while (n->prev)
      n = n->prev;
    currentNode = n;
  }
  while (n)
  {
    if (n->tag->type == Tag::Text || n->tag->type == Tag::ScriptStructureBegin)
      parseForScriptGroup(n);
    n = n->nextSibling();
  }
  lastLine = endLine;
  lastCol = endCol;
  return currentNode;
}

bool Parser::parseScriptInsideTag(Node *startNode)
{
  bool found = false;
  DTDStruct *dtd = startNode->tag->dtd;
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
        currentNode = createScriptTagNode(area, foundText, dtd, startNode, currentNode);

        found = true;
        AreaStruct area2(bl, bc, node_el, node_ec);
        int lastLine, lastCol;
        currentNode = parseSpecialArea(area2, foundText, "", currentNode, lastLine, lastCol);
        col = write->text(node_bl, node_bc, lastLine, lastCol).length();
      }
    }
  }
  return found;
}

QString Parser::getLine(int line, int endLine, int endCol)
{
  QString textLine = write->editIf->textLine(line);
  if (line == endLine)
  {
    if (endCol >0)
      textLine.truncate(endCol + 1);
    else
      textLine = "";
  }
  return textLine;
}

void Parser::appendAreaToTextNode(const AreaStruct &area, Node *node)
{
  QString tagStr = write->text(area);
  QString cleanStr = node->tag->cleanStr;
  node->tag->setStr(node->tag->tagStr() + tagStr);
  if (node->tag->type == Tag::Empty)
  {
    QString s = tagStr;
    if (s.simplifyWhiteSpace().isEmpty())
    {
      node->tag->type = Tag::Empty;
    } else
    {
      node->tag->type = Tag::Text;
    }
  }
  QString cleanedTagStr = tagStr;
  removeCommentsAndQuotes(cleanedTagStr, node->tag->dtd);
  node->tag->cleanStr = cleanStr + cleanedTagStr;
  int bLine, bCol;
  node->tag->beginPos(bLine, bCol);
  node->tag->setTagPosition(bLine, bCol, area.eLine, area.eCol);
}

Node* Parser::createTextNode(Node *node, int eLine, int eCol, Node *parentNode)
{
  Tag *textTag = 0L;
  Node *textNode = 0L;
  int bLine = 0;
  int bCol = 0;
  DTDStruct *dtd = m_dtd;
  if (node)
  {
    node->tag->endPos(bLine, bCol);
  }
  if (parentNode)
    dtd = parentNode->tag->dtd;
  eCol--;
  if (bLine == 0 && bCol == 0)
    bCol = -1;
  if ( !(bLine == eLine && bCol == eCol) )
  {
    AreaStruct area(bLine, bCol + 1, eLine, eCol);
    textTag = new Tag(area, write, dtd);
    QString s = textTag->tagStr();
    textTag->single = true;
    if (s.simplifyWhiteSpace().isEmpty())
    {
      textTag->type = Tag::Empty;
    } else
    {
      textTag->type = Tag::Text;
    }

    if (parentNode && parentNode->tag->single)
    {
      textNode = new Node(parentNode->parent);
      nodeNum++;
      textNode->prev = parentNode;
      parentNode->next = textNode;
      parentNode = parentNode->parent;
    } else
    {
      if ( node &&
          (node->tag->type == Tag::Empty ||
           node->tag->type == Tag::Text) )     //merge two consquent text or empty nodes
      {
        AreaStruct area(bLine, bCol, eLine, eCol);
        appendAreaToTextNode(area, node);
        delete textTag;
        textTag = 0L;
      } else
      {
        textNode = new Node(parentNode);
        nodeNum++;
        if (node && node != parentNode)
        {
          node->next = textNode;
          textNode->prev = node;
        } else
        {
          if (parentNode)
          {
            Node *n = parentNode->child;
            while (n && n->next)
              n = n->next;
            if (!n)
              parentNode->child = textNode;
            else
            {
              n->next = textNode;
              textNode->prev = n;
            }
          }
        }
      }
    }
    if (textTag)
    {
      textNode->tag = textTag;
      node = textNode;
    }
  }
  return node;
}

Node* Parser::createScriptTagNode(const AreaStruct &area, const QString &areaName, DTDStruct *dtd, Node *parentNode, Node *currentNode)
{
  Tag *tag = new Tag();
  tag->setTagPosition(area);
  tag->setStr(areaName);
  tag->setWrite(write);
  tag->dtd = dtds->find(dtd->specialAreaNames[areaName]);
  if (!tag->dtd)
      tag->dtd = dtd;
  tag->name = i18n("%1 block").arg(dtd->specialAreaNames[areaName].upper());
  tag->type = Tag::ScriptTag;

  Node *node = new Node(parentNode);
  nodeNum++;
  node->tag = tag;
  node->insideSpecial = true;
  if (parentNode && !parentNode->child)
  {
    parentNode->child = node;
  } else
  if (currentNode)
  {
    node->prev = currentNode;
    currentNode->next = node;
  }
  return node;
}


#include "parser.moc"
