/***************************************************************************
                          parser.cpp  -  description
                             -------------------
    begin                : Sun Apr 16 2000
    copyright            : (C) 2000 by Dmitry Poplavsky
    email                : pdima@mail.univ.kiev.ua
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "parser.h"
#include "node.h"
#include "tag.h"

#include <qstring.h>
#include <qregexp.h>
#include <qcstring.h>

#include <stdio.h>
#include <ctype.h>

extern QStrList *tagsList;
extern QStrList *singleTags;
extern QStrList *optionalTags;

Parser::Parser()
{
  s = 0;
  pos = 0;
  node = 0L;

  textChanged = true;

  QStrList * list = new QStrList();
  list->append("p");
  list->append("li");
  list->append("td");
  list->append("tr");
  list->append("th");
  list->append("dt");
  list->append("dd");
  tagsStop.insert("p",list);

  list = new QStrList();
  list->append("li");
  tagsStop.insert("li",list);
  
  list = new QStrList();
  list->append("select");
  tagsStop.insert("select",list);

  list = new QStrList();
  list->append("td");
  list->append("tr");
  list->append("th");
  tagsStop.insert("td",list);

  list = new QStrList();
  list->append("tr");
  list->append("tfoot");
  tagsStop.insert("tr",list);

  list = new QStrList();
  list->append("dt");
  list->append("dd");
  tagsStop.insert("dt",list);

  list = new QStrList();
  list->append("dt");
  list->append("dd");
  tagsStop.insert("dd",list);
}

Parser::~Parser()
{
	if (s) delete [] s;
}


Node *Parser::parse( const char *text)
{
	if ( s )
		if ( !strcmp(s,text) ) {
			textChanged = false;
			return node;
		}
	textChanged = true;
		
	if (s) delete [] s;
	if (node) delete node;
	s = new char[ strlen(text)+1 ];
	strcpy(s,text);
  pos = 0;

  node = subParse(0L);
  return node;
}

int Parser::xy2pos( int x, int y )
{
  int pos;
	const char *stmp = this->s;
	
	for ( int i=0; i<y && stmp; i++ ){
		stmp = strstr( stmp, "\n");
		if (stmp)
			stmp++;
  }

	if ( stmp )	{
		const char *nextline = strstr(stmp,"\n");
	  if ( nextline && nextline-stmp >= x )
			pos = stmp-s+x-1;
		else
			pos = nextline-s;
	}
	else
		pos = x;
//	printf("x: %d \t y: %d \t pos: %d\n",x,y,pos);	
//	fflush(stdout);
	return (pos);
}

QString Parser::tagsListForPoint(int pos)
{
	return subList(node,pos);
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
			s = node->tag.name + " / " + s;
	}
	
	return s;
}


Node * Parser::subParse( Node * parent, const char *tag )
{
  Node * node = 0L;
  Node * prevNode = 0L;
  Node * firstNode = 0L;

  while (1) {
    int type = tokenType();
    lastpos = pos;

    switch ( type ) {
      case EndText:
        return firstNode;
        break;

      case TagStart : {

        int oldpos = pos;
        Tag tagData = parseTag();

        if ( tag ) {  // check if this tag stop area of previous
          QStrList *list = tagsStop[tag];
          if ( list )
            if ( list->find( tagData.name ) != -1 ) {
              pos = oldpos;  // return to pos before tag
              //delete tagData;
              return firstNode;
            }
        }

        Node *tnode = new Node( parent );
        if ( !firstNode )
           firstNode = tnode;

        tnode->tag = tagData; // !!!

        tnode->start = oldpos;
        tnode->startContext = pos;
        tnode->type = Node::tTag;
        if ( prevNode )
          prevNode->next = tnode;
        prevNode = tnode;

        if ( singleTags->find(tagData.name.upper()) == -1 ) { // not single tag
          tnode->child = subParse( tnode , tagData.name );

        	tnode->endContext = pos-1;
        	tnode->end = pos-1;

        	if ( tokenType() == TagEnd ) {
          	int oldpos = pos;
          	QString tagend = parseTagEnd();
          	if ( tagend != tnode->tag.name ) {
           	 pos = oldpos;
           	 //delete tagData;
           	 return firstNode;
          	}
        	}
        }
        else { // single tag
        	tnode->endContext = pos-1;
        }
        tnode->end = pos-1;
        //delete tagData;
        }
        break;

      case Text:
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

      case Comment:
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

      case PHP:
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

      case TagEnd :
        return firstNode;
    }
  } // while

}

int Parser::tokenType()
{
  pos = skipSpaces();

  if ( s[pos] == 0 )
    return EndText;

  if ( s[pos] != '<')
    return Text;

  if ( strncmp( s+pos, "</", 2 ) == 0 )
    return TagEnd;

  if ( strncmp( s+pos, "<!--", 4 ) == 0 )
    return Comment;

  if ( strncmp( s+pos, "<?", 2 ) == 0 )
    return PHP;

  return TagStart;
}

int Parser::skipSpaces()
{
  int tpos = pos;
  while ( isspace(s[tpos]) && s[tpos] ) tpos++;
  return tpos;
}

Tag Parser::parseTag()
{
  Tag tag;// = new Tag();
  int tpos = pos+1;
  while ( s[tpos]!='>' && s[tpos] ) tpos++;

  tag.parseStr( QCString( s+pos+1, tpos-pos ).data() );

  pos = tpos;
  if ( s[pos] ) pos++;

  return tag;
}

QString Parser::parseTagEnd()
{
  int tpos = pos+2;
  int start = tpos;
  while ( s[tpos]!='>' && s[tpos] ) tpos++;
  pos = tpos;
  if ( s[pos])  pos++;

  int len = pos-start;

  return QCString( s+start, len ).lower();
}

void Parser::parseText()
{
  while ( s[pos]!='<' && s[pos] ) pos++;
}


void Parser::parseComment()
{
  while ( strncmp( s+pos, "-->", 3) && s[pos] ) pos++;
  if ( s[pos] )
    pos+=3;
}

void Parser::parsePHP()
{
  while ( strncmp( s+pos, "?>", 2) && s[pos] ) pos++;
  if ( s[pos] )
    pos+=2;
}

/**  */
int Parser::pos2y( int pos )
{
	int endLineCount = 0;
	for (int i=0; i<=pos && s[i]; i++)
		if (s[i]=='\n')
			endLineCount++;
	return endLineCount;
}

int Parser::pos2x( int pos )
{
	int i;
	for (i=pos; s[i]!='\n' && i; i--);
	return pos-i;
}
