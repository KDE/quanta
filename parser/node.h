/***************************************************************************
                          node.h  -  description
                             -------------------
    begin                : Sun Apr 16 2000
    copyright            : (C) 2000 by Dmitry Poplavsky
                           (C) 2001-2003 Andras Mantia <amantia@freemail.hu>
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

class Tag;
class QListViewItem;

#ifdef BUILD_KAFKAPART
#include <dom/dom_node.h>
#endif

class Node {

public:
 Node( Node *parent );
 ~Node();

 Node *next;
 Node *prev;
 Node *parent;
 Node *child;

 Node *nextSibling();
 Node *previousSibling();
 int size();

 #ifdef BUILD_KAFKAPART
 //temporary, will be replaced by a struct pointer to avoid too much space usage
 DOM::Node kafkaNode;
 DOM::Node kafkaNodeEnd;
 #endif
 QListViewItem *listItem;
 Tag *tag;
 bool closesPrevious; //this node "closes" the tag from previous node
 bool opened;
 bool removeAll; //set to false if you don't want to remove the "next" and "child" when deleting the node.
 bool insideSpecial; //true if the node is part of a special area
};

#endif
