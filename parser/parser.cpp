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
  pos = 0;
  m_node = 0L;
  m_textChanged = true;
}

Parser::~Parser()
{
}


Node *Parser::parse( QString text, QString dtdName)
{
  m_dtdName = dtdName;

  if ( m_text == text )
  {
		m_textChanged = false;
	}
  else
  {
  	m_textChanged = true;
  	m_text = text;	
	 	if (m_node) delete m_node;
    pos = 0;
    m_node = subParse(0L);
  }
  return m_node;
}


QString Parser::tagsListForPoint(int pos)
{
	return subList(m_node,pos);
}

QString Parser::subList( Node* node, int pos)
{
	QString s = "";
	if ( !node )
		return s;
		
	if ( node->child )
		s = s + subList( node->child, pos);
	if ( node->next )
		s = s + subList( node->next, pos);
		
	if ( node->type == Node::tTag ) {
		if ( pos>node->start && pos<node->end )
			s = node->tag->name + " / " + s;
	}
	
	return s;
}


Node * Parser::subParse( Node * parent, QString tag )
{
  Node * node = 0L;
  Node * prevNode = 0L;
  Node * firstNode = 0L;

  while (1)
  {
    int type = tokenType();
    lastpos = pos;

    switch ( type )
    {
      case EndText:
           {
             return firstNode;
             break;
           }
      case TagStart :
           {
             int oldpos = pos;
             Tag *tagData = parseTag();

             if ( !tag.isEmpty() )   // check if this tag stop area of previous
             {
               QTag *qTag = QuantaCommon::tagFromDTD(m_dtdName, tag);
               if ( qTag )
               {
                 QString searchFor = (qTag->parentDTD->caseSensitive)?tagData->name:tagData->name.upper();
                 if ( qTag->stoppingTags.contains( searchFor ) )
                 {
                   pos = oldpos;  // return to pos before tag
                   delete (tagData);
                   return firstNode;
                 }
               }
             }

             Node *tnode = new Node( parent );
             if ( !firstNode )
                firstNode = tnode;

             tnode->tag = tagData;
             tnode->start = oldpos;
             tnode->startContext = pos;
             tnode->type = Node::tTag;
             if ( prevNode )
                 prevNode->next = tnode;
             prevNode = tnode;

             if ( !tagData->single && !QuantaCommon::isSingleTag(m_dtdName, tagData->name) )  // not single tag
             {
               tnode->child = subParse( tnode , tagData->name.lower() );
               tnode->endContext = pos-1;
        	     tnode->end = pos-1;

        	     if ( tokenType() == TagEnd )
               {
          	     int oldpos = pos;
          	     QString tagend = parseTagEnd();
          	     if ( tagend.lower() != tnode->tag->name.lower() )
                 {
           	        pos = oldpos;
           	        //delete tagData;
           	        return firstNode;
          	     }
        	     }
             }
             else // single tag
             {
        	     tnode->endContext = pos-1;
             }
             tnode->end = pos-1;
            //delete tagData;
             break;
           }

      case Text:
           {
             parseText();
             node = new Node( parent );
             if ( !firstNode )
                 firstNode = node;
             node->start = lastpos;
             node->end = pos-1;
             node->type = Node::tText;
             if ( prevNode )
                 prevNode->next = node;
             prevNode = node;
             break;
           }

      case Comment:
           {
             parseComment();
             node = new Node( parent );
             if ( !firstNode )
                 firstNode = node;
             node->start = lastpos;
             node->startContext = lastpos+4;
             node->end = pos-1;
             node->endContext = pos-4;
             node->type = Node::tComment;
             if ( prevNode )
                 prevNode->next = node;
             prevNode = node;
             break;
           }

      case PHP:
           {
             parsePHP();
             node = new Node( parent );
             if ( !firstNode )
                firstNode = node;
             node->start = lastpos;
             node->end = pos-1;
             node->startContext = lastpos+2;
             node->endContext = pos-3;
             node->type = Node::tPHP;
             if ( prevNode )
               prevNode->next = node;
             prevNode = node;
             break;
           }

      case TagEnd :
           return firstNode;
    }
  } // while

}

int Parser::tokenType()
{
  skipSpaces();

  if ( pos >= (int) m_text.length() )
    return EndText;

  static QChar ch1('<');
  if ( m_text[pos] != ch1 )
    return Text;

  static QChar ch2('/');
  if ( m_text[pos+1] == ch2 )
    return TagEnd;

  static QChar ch3('?');
  if ( m_text[pos+1] == ch3 )
    return PHP;

  static QString str1("<!--");
  if ( m_text.mid(pos,4) ==  str1 )
    return Comment;


  return TagStart;
}

int Parser::skipSpaces()
{
  while ( m_text[pos].isSpace() ) pos++;
  return pos;
}

Tag* Parser::parseTag()
{
  Tag *tag = new Tag();
  int tpos = pos+1;
  static QChar ch1('>');
  while ( m_text[tpos] != ch1 && !m_text[tpos].isNull() ) tpos++;

  tag->parseStr( m_text.mid( pos+1, tpos-pos-1 ) );

  pos = tpos;
  if ( !m_text[pos].isNull() ) pos++;

  return tag;
}

QString Parser::parseTagEnd()
{
  int tpos = pos+2;
  int start = tpos;
  while ( m_text[tpos] !='>' && !m_text[tpos].isNull() ) tpos++;
  pos = tpos;
  int len = pos-start;

  if ( !m_text[pos].isNull() ) pos++;

  return m_text.mid( start, len ).lower();
}

void Parser::parseText()
{
  int len = m_text.length();
  static QChar ch1('<');
  while ( m_text[pos] != ch1 && pos < len ) pos++;
}


void Parser::parseComment()
{
  int len = m_text.length();
  static QString str1("-->");
  while ( m_text.mid(pos,3) != str1 && pos < len ) pos++;
  if ( pos < len )
    pos+=3;
}

void Parser::parsePHP()
{
  int len = m_text.length();

  static QString str1("?>");
  while ( m_text.mid(pos,2) != str1 && pos < len ) pos++;
  if ( pos < len )
    pos+=2;
}

/**  */
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
/** No descriptions */
void Parser::deleteNode()
{
  if (m_node)
  {
    delete m_node;
    m_node = 0L;
  }
}
