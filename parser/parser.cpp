/***************************************************************************
                          parser.cpp  -  description
                             -------------------
    begin                : Sun Apr 16 2000
    copyright            : (C) 2000 by Dmitry Poplavsky
                           (C) 2002 by Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qstring.h>
#include <qregexp.h>
#include <qcstring.h>
#include <qstringlist.h>
#include <qstrlist.h>

#include <stdio.h>
#include <ctype.h>
#include <iostream.h>

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

Node *Parser::parse(Document *w)
{
  write = w;
  m_dtdName = w->getDTDIdentifier();
//  if (m_node) delete m_node;
  int line = 0;
  int col = 0;
  m_node = subParse(0L, line, col);

  coutTree(m_node,0); //debug printout

  return m_node;
}
void Parser::coutTree(Node *node, int indent)
{
 while (node)
 {
   for (int i =0; i < indent; i++)
    cout << " ";
   if (node->tag->type != "text") cout << node->tag->name;
   else cout << node->tag->tagStr();
   cout << " (" << node->tag->type << ")\n";
   if (node->child) coutTree(node->child, indent + 4);
   node = node->next;
 }
}

Node * Parser::subParse( Node * parent, int &line, int &col )
{
  Node * node = 0L;
  Node * prevNode = 0L;
  Node * firstNode = 0L;

  while (line < write->editIf->numLines())
  {
    Tag *tag = write->tagAt(line, col);
    if (tag)
    {
      int type = tokenType(tag);

      switch ( type )
      {
        case XmlTag :
             {
               if ( parent )   // check if this tag stop area of previous
               {
                 QTag *qTag = QuantaCommon::tagFromDTD(m_dtdName, parent->tag->name);
                 if ( qTag )
                 {
                   QString searchFor = (qTag->parentDTD->caseSensitive)?tag->name:tag->name.upper();
                   if ( qTag->stoppingTags.contains( searchFor ) )
                   {
                     return firstNode; //parent will have 0 as child
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
                 tag->getTagEndPos(line, col);
                 nextPos(line, col);
                 tnode->child = subParse( tnode , line, col );
               } else
               {
                 tag->getTagEndPos(line, col);
                }
               if (tnode->next) prevNode = tnode->next;
               break;
             } //case XMLTag
      case XmlTagEnd:
             {
               if (parent)
               {
                 if ("/"+parent->tag->name == tag->name)
                 {
                   Node *tnode = new Node( parent->parent );
                   tnode->tag = tag;
                   parent->next = tnode;
                   tnode->prev = parent;
                   tag->getTagEndPos(line, col);
                   return firstNode;
                 }
               }
               break;
             }

      case Text:
           {
             node = new Node( parent );
             if ( !firstNode )
                 firstNode = node;
             if ( prevNode )
                 prevNode->next = node;
             node->tag = tag;
             node->prev = prevNode;
             prevNode = node;
             tag->getTagEndPos(line, col);
             break;
           }

      }

    } //if tag;
    nextPos(line, col);
  } // while
  return firstNode;
}

int Parser::tokenType(Tag *tag)
{
  if (tag->type == "xmltag")
   return XmlTag;
  if (tag->type == "xmltagend")
   return XmlTagEnd;
  if (tag->type == "text")
   return Text;
  if (tag->type == "comment")
   return Comment;
  if (tag->type == "script")
   return Script;

  return Unknown;
}

/*
int Parser::pos2y( int pos )
{
	int endLineCount = 0;
	if ( pos<0 ) pos = 0;
	
	for (int i=0; i<=pos && !m_text[i].isNull(); i++)
		if (m_text[i]=='\n')
			endLineCount++;
	return endLineCount;
}

int Parser::pos2x( int pos )
{
	int i;
	if ( pos<0 ) pos = 0;
	for (i=pos; m_text[i]!='\n' && i; i--);
	return pos-i;
}

int Parser::xy2pos( int x, int y )
{
  int pos = 0;
  QStringList slist = QStringList::split('\n',m_text,true);

  if ( y > (int) slist.count() )
  	y = slist.count();

  for ( int i=0; i<y; i++ )
  	 pos += slist[i].length()+1;

  int len = slist[y].length();
  if ( len>x )
  	pos+=x;
  else
  	pos+=len;
  	
	//printf("x: %d \t y: %d \t pos: %d\n",x,y,pos);	
	//fflush(stdout);
	
	return (pos);
}
              */
/** No descriptions */
void Parser::deleteNode()
{
  if (m_node)
  {
    delete m_node;
    m_node = 0L;
  }
}

void Parser::nextPos(int &line, int &col)
{
  if (col < write->editIf->lineLength(line))
  {
      col++;
  } else
  {
      col = 0;
      line++;
  }
}
