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
#include "qstring.h"

//kde includes
#include "klocale.h"
#include <ktexteditor/document.h>

//own includes
#include "parsercommon.h"
#include "node.h"
#include "qtag.h"
#include "quantacommon.h"
#include "dtds.h"

#include "editorsource.h"

class KDirWatch;

//FIXME int nodeNum; //for memory debugging - remove if not needed

namespace ParserCommon {
 QStringList includedFiles;
 QList<const DTDStruct*> includedFilesDTD;
 KDirWatch *includeWatch;

void appendAreaToTextNode(const QString &areaString, const AreaStruct &area, Node *node)
{
  QString cleanStr = node->tag->cleanStr;
  node->tag->setStr(node->tag->tagStr() + areaString);
  if (node->tag->isType(Tag::Empty))
  {
    QString s = areaString;
    if (s.simplified().isEmpty())
    {
      node->tag->setType(Tag::Empty);
    } else
    {
      node->tag->setType(Tag::Text);
    }
  }
  QString cleanedTagStr = areaString;
  QuantaCommon::removeCommentsAndQuotes(cleanedTagStr, node->tag->dtd());
  node->tag->cleanStr = cleanStr + cleanedTagStr;
  AreaStruct newArea;
  newArea.start = node->tag->beginPos();
  newArea.end = area.end;
  node->tag->setTagPosition(newArea);
}

Node* createTextNode(EditorSource *source, const DTDStruct *dtd, Node *node, int eLine, int eCol, Node *parentNode)
{
  if (eCol == 0 && eLine == 0)
    return 0L;
  Tag *textTag = 0L;
  Node *textNode = 0L;
  int bLine = 0;
  int bCol = 0;
  if (node)
  {
    node->tag->endPos(bLine, bCol);
  } else
  if (parentNode)
    parentNode->tag->endPos(bLine, bCol);
  if (parentNode)
    dtd = parentNode->tag->dtd();
  source->getPreviousPosition(eLine, eCol);
  if (bLine == 0 && bCol == 0)
    bCol = -1;
  if ( !(bLine == eLine && bCol == eCol) )
  {
    AreaStruct area(bLine, bCol + 1, eLine, eCol);
    QString s = source->text(area);
    textTag = new Tag(area, s, dtd);
    textTag->single = true;
    if (s.simplified().isEmpty())
    {
      textTag->setType(Tag::Empty);
    } else
    {
      textTag->setType(Tag::Text);
    }

    if (parentNode && parentNode->tag->single)
    {
      textNode = new Node(parentNode->parent);
//FIXME      nodeNum++;
      textNode->prev = parentNode;
      parentNode->next = textNode;
      parentNode = parentNode->parent;
    } else
    {
      if ( node &&
          (node->tag->isType(Tag::Empty) ||
           node->tag->isType(Tag::Text)) )     //merge two consquent text or empty nodes
      {
        AreaStruct area(bLine, bCol, eLine, eCol);
        QString s = source->text(area);
        appendAreaToTextNode(s, area, node);
        delete textTag;
        textTag = 0L;
      } else
      {
        textNode = new Node(parentNode);
//FIXME        nodeNum++;
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

Node* createScriptTagNode(const AreaStruct &area, const QString &areaName,
                          const DTDStruct *dtd, Node *parentNode, Node *currentNode)
{
  Tag *tag = new Tag();
  tag->setTagPosition(area);
  tag->setStr(areaName);
  const DTDStruct *d = DTDs::ref()->find(dtd->specialAreaNames[areaName]);
  if (d)
     tag->setDtd(d);
  else
      tag->setDtd(dtd);
  tag->setName(i18n("[%1]").arg(dtd->specialAreaNames[areaName].toUpper()));
  tag->setType(Tag::ScriptTag);
  tag->validXMLTag = false;

  Node *node = new Node(parentNode);
//FIXME  nodeNum++;
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

}
