/***************************************************************************
                          parsercommon.cpp  -  description
                             -------------------
    begin                : Wed Feb 11 2004
    copyright            : (C) 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//qt includes
#include <qstring.h>

//kde includes
#include <kdebug.h>
#include <klocale.h>
#include <ktexteditor/editinterface.h>

//own includes
#include "parsercommon.h"
#include "node.h"
#include "document.h"
#include "qtag.h"
#include "quantacommon.h"
#include "resource.h"
#include "dtds.h"

class KDirWatch;

int nodeNum; //for memory debugging - remove if not needed

namespace ParserCommon {
 QStringList includedFiles;
 QPtrList<const DTDStruct> includedFilesDTD;
 KDirWatch *includeWatch;

 //common methods.
QString getLine(Document *write, int line, int endLine, int endCol)
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

void appendAreaToTextNode(Document *write, const AreaStruct &area, Node *node)
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
  QuantaCommon::removeCommentsAndQuotes(cleanedTagStr, node->tag->dtd());
  node->tag->cleanStr = cleanStr + cleanedTagStr;
  int bLine, bCol;
  node->tag->beginPos(bLine, bCol);
  node->tag->setTagPosition(bLine, bCol, area.eLine, area.eCol);
}

Node* createTextNode(Document *write, Node *node, int eLine, int eCol, Node *parentNode)
{
  Tag *textTag = 0L;
  Node *textNode = 0L;
  int bLine = 0;
  int bCol = 0;
  const DTDStruct *dtd = write->defaultDTD();
  if (node)
  {
    node->tag->endPos(bLine, bCol);
  } else
  if (parentNode)
    parentNode->tag->endPos(bLine, bCol);
  if (parentNode)
    dtd = parentNode->tag->dtd();
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
        appendAreaToTextNode(write, area, node);
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

Node* createScriptTagNode(Document *write, const AreaStruct &area, const QString &areaName,
                          const DTDStruct *dtd, Node *parentNode, Node *currentNode)
{
  Tag *tag = new Tag();
  tag->setTagPosition(area);
  tag->setStr(areaName);
  tag->setWrite(write);
  const DTDStruct *d = DTDs::ref()->find(dtd->specialAreaNames[areaName]);
  if (d)
     tag->setDtd(d);
  else
      tag->setDtd(dtd);
  tag->name = i18n("%1 block").arg(dtd->specialAreaNames[areaName].upper());
  tag->type = Tag::ScriptTag;
  tag->validXMLTag = false;

  Node *node = new Node(parentNode);
  nodeNum++;
  node->tag = tag;
  node->insideSpecial = true;
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
  } else
  if (currentNode)
  {
    node->prev = currentNode;
    currentNode->next = node;
  }
  return node;
}

void coutTree(Node *node, int indent)
{
    QString output;
    int bLine, bCol, eLine, eCol;
    if (!node)
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
                    bLine << ":" << bCol << " - " << eLine << ":" << eCol << " This: "<< node << " Parent: " << node->parent << " Prev: " << node->prev << " Next: " << node->next << " Child: " << node->child << " Tag:" << node->tag << endl;
 /*           for(j = 0; j < node->tag->attrCount(); j++)
            {
                    kdDebug(24000)<< " attr" << j << " " <<
                            node->tag->getAttribute(j).nameLine << ":" <<
                            node->tag->getAttribute(j).nameCol << " - " <<
                            node->tag->getAttribute(j).valueLine << ":" <<
                            node->tag->getAttribute(j).valueCol << endl;
            }
*/
            if (node->child)
                    coutTree(node->child, indent + 4);
            node = node->next;
    }
}

void verifyTree(Node *node)
{
  QString output;
  int bLine, bCol, eLine, eCol;
  while (node)
  {
    if (!node->tag)
    {
      kdDebug(24000) << "Bad node: " << node << endl;
      kdDebug(24000) << "Parent: " << node->parent << " " << node->parent->tag->tagStr() << endl;
    }      
    if (node->child)
      verifyTree(node->child);
    node = node->next;
  }
}

}
