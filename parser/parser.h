/***************************************************************************
                          parser.h  -  description
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

#ifndef PARSER_H
#define PARSER_H

#include <qdict.h>
#include <qstringlist.h>

#include "node.h"
#include "tag.h"


/**
  *@author Dmitry Poplavsky, Andras Mantia
  */

class Document;

class Parser {
public: 
	Parser();
	~Parser();
	
	Node *parse( QString text, QString dtdName = QString::null);
	int xy2pos( int x, int y );
	int pos2y(int pos);
	int pos2x(int pos);
	
	QString tagsListForPoint(int pos);
	bool textChanged() {return m_textChanged;}
  /** No descriptions */
  void deleteNode();

	QString m_text;
private:


  Node * subParse( Node* parent , QString tag = QString::null );
  QString subList( Node* node, int pos);

  enum { EndText = 0, Text,  TagStart, TagEnd, Comment, PHP }; // types of token
  int tokenType(); // return type of next token

  int skipSpaces(); // return pos of next nonspace symbol

  void parseText();
  Tag* parseTag();
  QString parseTagEnd();
  void parseComment();
  void parsePHP();

  int pos;
  int lastpos;
  int lasttype;
  Node* m_node;
  QString m_dtdName;
	bool m_textChanged;

};

#endif
