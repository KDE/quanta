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
	
	Node *parse( Document *w);
  void coutTree(Node *node, int indent);
/*	int xy2pos( int x, int y );
	int pos2y(int pos);
	int pos2x(int pos);*/
	
  /** No descriptions */
  void deleteNode();

	QString m_text;  //FIXME: having an internal copy of text is absolutely useless
private:
  enum { Unknown = 0, XmlTag,  XmlTagEnd, Text, Comment, Script }; // types of token

  Node* m_node;
  QString m_dtdName;
  Document *write;

  Node * subParse( Node* parent , int &line, int &col);
  void nextPos(int &line, int &col);
  int tokenType(Tag *tag); // return type of next token
};

#endif
