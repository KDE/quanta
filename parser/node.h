/***************************************************************************
                          node.h  -  description
                             -------------------
    begin                : Sun Apr 16 2000
    copyright            : (C) 2000 by Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2001-2003 Andras Mantia <amantia@kde.org>
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

 /**
  * Copy everything from node except prev, next, child, parent, listItem, group, groupTag, which are set to Null
  * The groupElementsList is cleared.
  */
 void operator =(Node* node);

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

/** DOM like functions cf dom/dom_node.h */
 QString nodeName();
 QString nodeValue();
 void setNodeValue(QString value);
 Node* parentNode() {return parent;}
 Node* firstChild() {return child;}
 Node* lastChild();
 Node* DOMpreviousSibling() {return prev;}
 Node* DOMnextSibling() {return next;}
 /**Node* insertBefore(Node *newChild, Node *refChild);
 Node* replaceChild(Node *newChild, Node *oldChild);
 Node* removeChild(Node *oldChild);
 Node* appendChild(Node *newChild);*/
 bool hasChildNodes() {return child;}

 /** Others functions. */
 #ifdef BUILD_KAFKAPART
 // check if Node has node in its child subtree (and grand-child,...)
 bool hasForChild(Node *node);
 #endif
 void setParent(Node *nodeParent) {parent = nodeParent;}
 //If Node is of type XmlTag, return the corresponding XmlTagEnd if available
 Node *getClosingNode();
 //Return the first next non-Empty Node
 Node *nextNE();
 //Return the first prev non-Empty Node
 Node *prevNE();

 int size();

 #ifdef BUILD_KAFKAPART
 DOM::Node _rootNode;
 DOM::Node _leafNode;
 Node* _closingNode;
 #endif
 QListViewItem *listItem; //points to the listview item which represents this node in the structure tree
 Tag *tag;
 bool closesPrevious; //this node "closes" the tag from previous node
 bool opened;
 bool removeAll; //set to false if you don't want to remove the "next" and "child" when deleting the node.
 bool insideSpecial; //true if the node is part of a special area
 QString fileName; //the node is in this file. If empty, it's in the current document

//The below fields are set during parsing for XML groups. See the
//description.rc and the Parser::parseForXMLGroup(Node *node)
 Tag *groupTag; //points to a copy of the node tag. The name of the tag is set according to the container group rules.
 XMLStructGroup *group; //points to the container group

//pointer list to all of the group element lists. For example, the node is part of
//"function | foo" and "variable | $i" group. In this case the list has two pointers.
//if "$i" appeared more than once in the node, the second pointer points to a list
//with more than one elements. The original group element lists are in
// Parser::m_groups
 QPtrList<GroupElementList> groupElementLists;
};

#endif
