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
#include "../parts/kafka/nodeproperties.h"
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
 kNodeProperties *kafkaAddon;
 #endif
 QListViewItem *listItem;
 Tag *tag;
 Tag *groupTag;
 bool closesPrevious; //this node "closes" the tag from previous node
 bool opened;
 bool removeAll; //set to false if you don't want to remove the "next" and "child" when deleting the node.
 bool insideSpecial; //true if the node is part of a special area
 QString fileName; //the node is in this file. If empty, it's in the current document
};

#endif
