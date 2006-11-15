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

#include "quantaexport.h"

#include <QString>
#include <qmap.h>

class QDomElement;

class AreaStruct;
class GroupStorage;
class Tag;
class StructTreeGroup;
class XMLStructGroup;


/**
 * A Node is a basic unit of a Tree. It keeps track of his parent, his left neighbour, his right neighbour
 * and his first child.
 * It contains some functions to navigate through the tree, but some more are located at kafkacommon.h
 * (and should be moved here...)
 * It also contains a pointer to a Tag object which contains information about the contents of the Node.
 * We use this class to represent the XML/SGML document as a tree ( a DOM like tree) when each Node represent
 * a part of the document ( A tag, a text, ... see tag.h)
 * The tree is built with the parser (see parser.h)
 */

class KDEVQUANTA_EXPORT Node {

public:
  explicit Node( Node *parent );

  /**
   * Deletes a node and removes the references of it from the passed GroupStorage object.
   * @param groupStorage the GroupStorage object. If null or not specified the node will be simply deleted.
   */
  void deleteNode(GroupStorage *groupStorage = 0L);

  /**
    * Copy everything from node except prev, next, child, parent, listItem, group, groupTag, which are set to Null
    * The groupElementsList is cleared.
    */
  void operator =(Node* node);
  
//  /** For Kafka copy/paste */
/*  void save(QDomElement& element) const;
  bool load(QDomElement const& element);*/
  
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

//   void detachNode();

  /** Returns the node for a position in a node tree. As more than one node can contain the same area, it return the "deepest" node. 
    * 
    * @param base the base of the node tree that is searched
    * @param line the line of the node that we want to get
    * @param col the column of the node that we want to get
    * @param findDeepest true if we need the deepest node
    * @param exact if false and the found node is inside an area that is covered by the parent node, the parent is returned. If true, the node found is returned. Example: 
    @code: <a href="<? echo $i?>">
    If the cursor is inside the "echo" and the @param exac is true, the node for "<?echo $i ?>" is returned, otherwise the node for the "a" tag is returned.
    * @return the node, if found, the base node specified if no nodes were found
    */
  static Node *nodeAt(Node *base, int line, int col, bool findDeepest = true, bool exact = false);

  /**
   * Prints the node tree to the standard debug output.
   *
   * @param node  the base of the tree
   * @param indent the indentation level of the print
   */
  static void coutTree(Node *node, int indent);

//   int size();
  
  Node* _closingNode;
  
  /**
    * The contents of the Node is inside the Tag. Should _never_ be null.
    */
  Tag *tag;
  
  bool closesPrevious; //this node "closes" the tag from previous node
  mutable bool opened;
  bool removeAll; //set to false if you don't want to remove the "next" and "child" when deleting the node.
  bool insideSpecial; //true if the node is part of a special area
  bool specialInsideXml; //< true if the node is a special area defined inside a tag, like the PHP in <a href="<? echo $a ?>">
  QString fileName; //the node is in this file. If empty, it's in the current document
 // QValueList<GroupElement*> m_groupElements; ///< all the group elements pointing to this node
  
  /** detect the area from the tag. Starts with the first character of the opening tag
   *  and reaches until corresponding closing tag.
   *  The parameter decides if the closing tag is included or not.
   *  It detects if the closing tag is missing or we have a single tag.
   *
   * @param excludeClosing 
   * @return the area from the opening tag to the closing tag
   */
  AreaStruct tagArea(bool excludeClosing = false) const;

private:
  /**
   * Deletes the node. It is private as one should not use directly. Use deleteNode() instead.
   */
  ~Node();

   GroupStorage *m_groupStorage;

  
};

#endif
