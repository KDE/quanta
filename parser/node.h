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

#include <qptrlist.h>
#include <qvaluelist.h>
#include <qmap.h>

class Tag;
class QListViewItem;
struct XMLStructGroup;

#ifdef BUILD_KAFKAPART
#include <dom/dom_node.h>

typedef struct domNodeStruct
   {
    int type;
    DOM::Node domNode;
    int beginOffset;
    int endOffset;
   };
#endif

class Node;

struct GroupElement{
    /*The node which contains the element */
    Node *node;
    /* The tag which point to the actual place in the node */
    Tag *tag;
    /*The parent node indicating the beginning of a structure */
    Node *parentNode;
    bool global;
  };

typedef QValueList<GroupElement> GroupElementList;
typedef QMap<QString, GroupElementList> GroupElementMapList;

class Node {

public:
 Node( Node *parent );
 ~Node();

 Node *next;
 Node *prev;
 Node *parent;
 Node *child;

 /** Returns the child if available, else the next node, else the next node of the first parent which has one, else 0L.
       WARNING: it doesn't behave like DOM::Node::nextSibling() which give the next Node, or 0L if there is no next Node */
 Node *nextSibling();
 Node *previousSibling();
 /** Returns the next node, or the parent's next, if next doesn't exists,
  or the granparent's next, if parent's next doesn't exists, etc. */
 Node *nextNotChild();
 int size();

 #ifdef BUILD_KAFKAPART
 QList<domNodeStruct> vList;
 QList<domNodeStruct> hList;
 DOM::Node _rootNode;
 DOM::Node _leafNode;
 Node* _closingNode;
 #endif
 QListViewItem *listItem;
 Tag *tag;
 Tag *groupTag;
 bool closesPrevious; //this node "closes" the tag from previous node
 bool opened;
 bool removeAll; //set to false if you don't want to remove the "next" and "child" when deleting the node.
 bool insideSpecial; //true if the node is part of a special area
 QString fileName; //the node is in this file. If empty, it's in the current document
 XMLStructGroup *group;
 QPtrList<GroupElementList> groupElementLists;
 GroupElementList::Iterator groupElementIt;
};

#endif
