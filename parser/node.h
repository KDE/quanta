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
    bool deleted;
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
 //If Node is of type XmlTag or ScriptTag, return the corresponding XmlTagEnd if available
 Node *getClosingNode();
 //If Node is of type XmlTagEnd, return the corresponding XmlTag or ScriptTag if available
 Node *getOpeningNode();

 /** The Node link skipping Empty Nodes. */
 //Returns the first next non-Empty Node
 Node *nextNE();
 //Returns the first prev non-Empty Node
 Node *prevNE();
 //Returns the first non-Empty child
 Node *firstChildNE();
 //Returns the last non-Empty child
 Node *lastChildNE();

/**
 * The main problem manipulating the default links prev/next/parent/child is that we often want
 * to manipulate only the "significant" Nodes e.g. XmlTag, Text, ScriptNode, like in a DOM::Node tree.
 * These functions, prefixed with "S" which stands for "simplified" or "significant", will only return
 * and manipulate XmlTag, Text and ScriptNode.
 */
 //Returns the first significant previous sibling.
  Node *SPrev();
 //Returns the first significant next sibling.
  Node *SNext();
 //Returns the first significant child.
  Node *SFirstChild();
 //Returns the last significant child.
  Node *SLastChild();



 int size();

#ifdef BUILD_KAFKAPART
//set/get the corresponding DOM::Node of this node.
// A node can correspont to several DOM::Node so we keep a pointer to the root Node
// and the leaf Node.
 DOM::Node* rootNode() {return m_rootNode;}
 DOM::Node* leafNode() { return m_leafNode;}
 void setRootNode(DOM::Node *rootNode) {m_rootNode = rootNode;}
 void setLeafNode(DOM::Node *leafNode) {m_leafNode = leafNode;}
 Node* _closingNode;
#endif
 QListViewItem *listItem; ///<points to the listview item which represents this node in the structure tree
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
// globalGroupMap
 QPtrList<GroupElementList> groupElementLists;

 #ifdef BUILD_KAFKAPART
private:
  DOM::Node *m_rootNode, *m_leafNode;
#endif
};

#endif
