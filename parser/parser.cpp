/***************************************************************************
                          parser.cpp  -  description
                             -------------------
    begin                : Sun Sep 1 2002
    copyright            : (C) 2002 by Andras Mantia
    email                : amantia@freemail.hu
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
#include <qregexp.h>
#include <qcstring.h>
#include <qstringlist.h>
#include <qstrlist.h>

//standard library includes
#include <stdio.h>
#include <ctype.h>
#include <iostream.h>

//app includes
#include "parser.h"
#include "node.h"
#include "tag.h"
#include "qtag.h"
#include "../resource.h"
#include "../quantacommon.h"
#include "../document.h"

Parser::Parser()
{
  m_text = QString::null;
  m_node = 0L;
}

Parser::~Parser()
{
}

/** Parse the whole text from Document w and build the internal structure tree from Nodes */
Node *Parser::parse(Document *w)
{
  write = w;
  m_dtdName = w->getDTDIdentifier();
  m_text = w->editIf->text();
//  if (m_node) delete m_node;
  int line = 0;
  int col = 0;
  m_node = subParse(0L, line, col);

//  coutTree(m_node,0); //debug printout

  return m_node;
}

/** Print the doc structure tree to the standard output.
    Only for debugging purposes. */
void Parser::coutTree(Node *node, int indent)
{
 while (node)
 {
   for (int i =0; i < indent; i++)
    cout << " ";
   if (node->tag->type != Tag::Text) cout << node->tag->name;
   else cout << node->tag->tagStr();
   cout << " (" << node->tag->type << ")\n";
   if (node->child) coutTree(node->child, indent + 4);
   node = node->next;
 }
}

/** Recursive parsing algorithm. Actually this does the parsing and tree building. */
Node * Parser::subParse( Node * parent, int &line, int &col )
{
  Node * node = 0L;
  Node * prevNode = 0L;
  Node * firstNode = 0L;

  while (line < (int)write->editIf->numLines())
  {
    Tag *tag = write->tagAt(line, col);
    if (tag)
    {
      switch ( tag->type )
      {
        case Tag::XmlTag :
             {
               if (prevNode && !prevNode->tag->name.startsWith("/"))
               {
                 prevNode->tag->closingMissing = true; //prevNode is single...
               }
               if ( parent )   // check if this tag stop area of previous
               {
                 QTag *qTag = QuantaCommon::tagFromDTD(m_dtdName, parent->tag->name);
                 if ( qTag )
                 {
                   QString searchFor = (qTag->parentDTD->caseSensitive)?tag->name:tag->name.upper();
                   if ( qTag->stoppingTags.contains( searchFor ) )
                   {
                     parent->tag->closingMissing = true; //parent is single...
                     col--;
                     return firstNode;
                   }
                 }
               } //if (parent)

               Node *tnode = new Node( parent );
               if ( !firstNode ) firstNode = tnode;

               tnode->tag = tag;
               tnode->next = 0L;
               if ( prevNode ) prevNode->next = tnode;
               tnode->prev = prevNode;
               prevNode = tnode;

               if ( !tag->single && !QuantaCommon::isSingleTag(m_dtdName, tag->name) )  // not single tag
               {
                 tag->endPos(line, col);
                 nextPos(line, col);
                 tnode->child = subParse( tnode , line, col ); //tag can have other elements inside
               } else
               {
                 tag->endPos(line, col);
                }
               if (tnode->next) prevNode = tnode->next;
               break;
             } //case XMLTag
      case Tag::XmlTagEnd:
             {
               if (parent)
               {
                 DTDStruct *dtd = dtds->find(m_dtdName);
                 if (!dtd) dtd = dtds->find(defaultDocType);
                 QString startName = (dtd->caseSensitive) ? parent->tag->name: parent->tag->name.upper();
                 QString endName = (dtd->caseSensitive) ? tag->name: tag->name.upper();
                 if ("/"+startName == endName)
                 {
                   Node *tnode = new Node( parent->parent );
                   tnode->tag = tag;
                   parent->next = tnode;
                   tnode->prev = parent;
                   tag->endPos(line, col);
                   return firstNode;
                 }
               }
               break;
             }

      case Tag::Comment:
      case Tag::Text:
           {
             node = new Node( parent );
             if ( !firstNode )
                 firstNode = node;
             if ( prevNode )
                 prevNode->next = node;
             node->tag = tag;
             node->prev = prevNode;
             node->next = 0L;
             prevNode = node;
             tag->endPos(line, col);
             break;
           }
      case Tag::ScriptStructureBegin:
           {
             node = new Node(parent);
             if ( !firstNode )
                 firstNode = node;
             if ( prevNode )
                 prevNode->next = node;
             node->tag = tag;
             node->prev = prevNode;
             node->next = 0L;
             prevNode = node;
             tag->endPos(line, col);
             nextPos(line, col);
             node->child = subParse( node , line, col );
             if (node->next) prevNode = node->next;
             break;
           }
      case Tag::ScriptStructureEnd:
           {
             if (parent)
             {
               Node *node = new Node( parent->parent );
               node->tag = tag;
               parent->next = node;
               node->prev = parent;
               tag->endPos(line, col);
               return firstNode;
             }
             break;
           }
      case 100:
           {
             tag->endPos(line, col);
             delete tag;
             break;
           }

      }

    } //if tag;
    nextPos(line, col);
  } // while
  return firstNode;
}

/** Delete the internal m_node */
void Parser::deleteNode()
{
  if (m_node)
  {
    delete m_node;
    m_node = 0L;
  }
}

/** Go to the next column, or to the next line if we are at the end of line */
void Parser::nextPos(int &line, int &col)
{
  if (col < write->editIf->lineLength(line) - 1)
  {
      col++;
  } else
  {
      col = 0;
      line++;
  }
}
/** Update the internal node tree starting from the specified node. */
void Parser::update(Node *fromNode)
{
  if (fromNode)
  {
    int bLine, bCol, eLine, eCol;
    bLine = bCol = 0;
    if (fromNode->prev)
    {
      Node *node = fromNode->prev;
      while (node->child) node = node->child;
      node->tag->endPos(bLine, bCol);
      nextPos(bLine, bCol);
    }
    Tag *oldTag = fromNode->tag;
    oldTag->endPos(eLine, eCol);
    Tag *newTag = write->tagAt(bLine, bCol);
    int nLine, nCol; //the new ending position of the tag
    newTag->endPos(nLine, nCol);
    if (nLine != eLine || nCol != eCol) //the end of tag has moved, so move the following also
    {
      fromNode->tag = newTag;
      delete oldTag;
      if (fromNode->child) update(fromNode->child);
      else if (fromNode->next) update(fromNode->next);
    }
  }
}
