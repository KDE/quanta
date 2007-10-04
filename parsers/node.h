/***************************************************************************
                          node.h  -  description
                             -------------------
    begin                : Sun Apr 16 2000
    copyright            : (C) 2000 by Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2001-2004 Andras Mantia <amantia@kde.org>
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

#include <qptrlist.h>
#include <qvaluelist.h>
#include <qmap.h>
#include <dom/dom_node.h>

class QDomElement;
class QListViewItem;

class Tag;
class Node;
class StructTreeGroup;
class XMLStructGroup;

struct GroupElement{
    /*The node which contains the element */
    Node *node;
    /* The tag which point to the actual place in the node */
    Tag *tag;
    /*The parent node indicating the beginning of a structure */
    Node *parentNode;
    bool global;
    bool deleted;
    QString type;
    XMLStructGroup *group; ///<is part of this group
  };

typedef QValueList<GroupElement*> GroupElementList;
typedef QMap<QString, GroupElementList> GroupElementMapList;

/**
 * A Node is a basic unit of a Tree. It keeps track of his parent, his left neighbour, his right neighbour
 * and his first child.
 * It contains some functions to navigate through the tree, but some more are located at kafkacommon.h
 * (and should be moved here...)
 * It also contains a pointer to a Tag object which contains informations about the contents of the Node.
 * We use this class to represent the XML/SGML document as a tree ( a DOM like tree) when each Node represent
 * a part of the document ( A tag, a text, ... see tag.h)
 * The tree is built with the parser (see parser.h)
 */

class Node {

public:
 Node( Node *parent );
 ~Node();
 
 /** 
  * Deletes the node. Use this instead of delete node; as it checkes if there
  * node was really allocated or not and avoid nasty crashes.
  * @return true - if node existed and is deleted
  *         false - if the node did not exist
  */
 static bool deleteNode(Node *node);

 /**
  * Copy everything from node except prev, next, child, parent, listItem, group, groupTag, which are set to Null
  * The groupElementsList is cleared.
  */
 void operator =(Node* node);

 /** For Kafka copy/paste */
 void save(QDomElement& element) const;
 bool load(QDomElement const& element);

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
 void setNodeValue(const QString &value);
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
 // check if Node has node in its child subtree (and grand-child,...)
 bool hasForChild(Node *node);
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
  void detachNode();



 int size();

//set/get the corresponding DOM::Node of this node.
//See more informations about rootNode/leafNode below.
 DOM::Node* rootNode() {return m_rootNode;}
 DOM::Node* leafNode() { return m_leafNode;}
 void setRootNode(DOM::Node *rootNode) {m_rootNode = rootNode;}
 void setLeafNode(DOM::Node *leafNode) {m_leafNode = leafNode;}
 Node* _closingNode;

 /**
  * The contents of the Node is inside the Tag. Should _never_ be null.
  */
 Tag *tag;

 QValueList<QListViewItem *> listItems; ///<points to the listview items which represents this node in the structure tree
 QListViewItem *mainListItem; ///< the main listview item (from under the root node) associated with this node
 bool closesPrevious; //this node "closes" the tag from previous node
 bool opened;
 bool removeAll; //set to false if you don't want to remove the "next" and "child" when deleting the node.
 bool insideSpecial; //true if the node is part of a special area
 bool specialInsideXml; //< true if the node is a special area defined inside a tag, like the PHP in <a href="<? echo $a ?>">
 QString fileName; //the node is in this file. If empty, it's in the current document
 QValueList<GroupElement*> m_groupElements; ///< all the group elements pointing to this node

private:
  /**
   * For VPL use.
   * Usually for a XmlTag or Text Node there is one corresponding DOM::Node. But sdmetimes there are more
   * e.g. in the DOM::Node tree the TABLE DOM::Node require the TBODY DOM::Node even if not necessary according
   * to the specs. So m_rootNode points to the TABLE DOM::Node and m_leafNode points to the TBODY DOM::Node.
   */
  DOM::Node *m_rootNode, *m_leafNode;
};

#endif
