/***************************************************************************
                          node.h  -  description
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

#ifndef NODE_H
#define NODE_H


/**
  *@author Dmitry Poplavsky
  */

#include "tag.h"

class Node {

public:
	Node( Node *parent );
	~Node();
	
	//enum { tDefault = 0 , tText = 1 , tTag = 2, tComment, tPHP };
	
	Node *next;
	Node *prev;
	Node *parent;
	Node *child;
	
	//int type;  // Tag, Text, ...
	//int start; //  pos in string
	//int end;
	//int startContext;
	//int endContext;
	
	Tag *tag;
  bool opened;
};

#endif
