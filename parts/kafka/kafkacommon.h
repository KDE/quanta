/***************************************************************************
                               kafkacommon.h
                             -------------------

    copyright            : (C) 2003 - Nicolas Deschildre
    email                : nicolasdchd@ifrance.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KAFKACOMMON_H
#define KAFKACOMMON_H

#include <qvaluelist.h>
#include <dom/dom_node.h>

namespace DOM
{
	class Document;
}
class Node;
class NodeModifsSet;
class Document;
class DTDStruct;

/**
 * For heavy debug including Node Tree in stdout printing, a DOM::Node tree widget.
 */
#define HEAVY_DEBUG

/**
 * Light debugging, including functions name in stdout printing.
 */
#define LIGHT_DEBUG


/** This class gathers all the basic functions needed by kafka. */

class kafkaCommon
{
public:
	kafkaCommon() {}
	~kafkaCommon() {}

	/** ----------------------- NODE TREE NAVIGATION -------------------------------------*/

	/**
	 * This function returns the next Node after node : the first child of
	 * node if available, else its next sibling if available, else the next
	 * available next sibling of a parent of node.
	 * @param _node It is the Node from which we want the next Node.
	 * @param goUp This boolean specifies if we should go up (torwards the root Node)
	 * or down in the tree.
	 * For a normal use, It must be set to false at the
	 * beginning and then the same boolean must be used when using
	 * several times this function.
	 * @param endNode Specifies at which Node the search should end.
	 * @return Returns the next Node.
	 */
	static Node* getNextNode(Node *node, bool &goUp, Node *endNode = 0L);

	/**
	 * It behaves essentially like the above function except that it won't return any empty Nodes.
	 */
	static Node* getNextNodeNE(Node *node, bool &goUp, Node *endNode = 0L);

	/**
	 * This function returns the prev Node after node.
	 */
	static Node* getPrevNode(Node *node, Node *endNode = 0L);

	/**
	 * It behaves essentially like the above function except that it won't return any empty Nodes.
	 */
	static Node* getPrevNodeNE(Node *node, Node *endNode = 0L);

	/**
	 * This function returns the next DOM::Node after node : the first child of
	 * DOM::Node if available, else its next sibling if available, else the next
	 * available next sibling of a parent of node.
	 * @param node The DOM::Node the search starts from.
	 * @param goUp This boolean specifies if we should go up or down in the tree.
	 * For a normal use, It must be set to false at the
	 * beginning and then the same boolean must be used when using
	 * several times this function.
	 * @param returnParentNode Specifies if there are no child and next sibling, if
	 * we should return the parent.
	 * @param endNode Specifies at which DOM::Node the search should end. It is useful
	 * when setting returnParentNode to false.
	 * @return the next Node.
	 */
	static DOM::Node getNextDomNode(DOM::Node node, bool &goUp, bool returnParentNode = false,
		DOM::Node endNode = DOM::Node());


	/** ----------------------- NODE INDENTATION STUFF -------------------------------------*/

	/**
	 * This function takes care to modify the current node or/and the next sibling in order to have a nice
	 * indentation. WARNING it doesn't create the necessary Nodes.
	 * @param node Apply nice indentation to this Node.
	 * @param nbOfSpaces Specifies the number of spaces the minimal indentation must be.
	 * @param nbOfTabs Specifies the number of tabs the minimal indentation must be.
	 */
	static void applyIndentation(Node *node, int nbOfSpaces, int nbOfTabs);

	/**
	 * Create/Delete the necessary Empty Nodes between n1 and n2 so that a nice indentation can be
	 * generated by the undoRedo system. The undoRedo system can't create itself these Nodes
	 * because it will create them only during the synchronization, making all the QValueList<int>
	 * Nodes location wrong.
	 * WARNING n1 and n2 must be siblings or parent-child. If there are sibling and n1 is a XmlTag,
	 * n1 should not have non empty childs.
	 * @param n1 The start node.
	 * @param n2 The end node.
	 */
	static void fitIndentationNodes(Node *n1, Node *n2);

	/**
	 * Fits the Nodes positions after a change in the Node tree.
	 * @param startNode The Node where the update of the Node positions starts.
	 * @param colMovement The number of columns that should be
	 * added/retrieved from the column position. It is the difference of the new last char col position and the
	 * old last char col position.
	 * @param lineMovement The number of lines that should be
	 * added/retrieved from the line position. It is the difference of the number of lines of the new tag string
	 * and the number of lines of the old tag string.
	 * @param colEnd The column position where the update should stop.
	 * @param lineEnd The line position where the update should stop.
	 */
	static void fitsNodesPosition(Node* startNode, int colMovement, int lineMovement = 0,
		int colEnd = -2, int lineEnd = -2);

	/**
	 * Get the display type of a Node. NOT a official list, more a little hack to
	 * handle the indentation.
	 * @param nodeName The name of the Node.
	 * @param closingNodeToo Specifies if we return the display of the corresponding Node of closing Nodes.
	 * @return Returns the type.
	 */
	static int getNodeDisplay(const QString &nodeName, bool closingNodeToo = false);

	//the enumeration of the different display types
	enum nodeDisplay
	{
		noneDisplay = 0,
		inlineDisplay,
		blockDisplay
	};


	/** ----------------------- NODE TREE MODIFICATIONS -------------------------------------*/

	/**
	 * Create a simple Node, without taking care of building the closing Node.
	 */
	static Node* createNode(const QString &nodeName, const QString &tagString, int nodeType, Document *doc);

	/**
	 * Insert node in the tree. WARNING This function will log that node was added.
	 * It will also try to merge text/Empty Nodes.
	 * @param node The node to insert.
	 * @param parentNode This Node will be the parent of node.
	 * @param nextSibling This Node will be the next Sibling of Node. If null, node will be appened at
	 * the child list of parentNode.
	 * @param modifs The changes made are logged into modifs.
	 * @param merge Try to merge with the siblings if possible.
	 * @return Returns a pointer to the node inserted.
	 */
	static Node* insertNode(Node *node, Node* parentNode, Node* nextSibling, NodeModifsSet &modifs,
		bool merge = true);

	/**
	 * It behaves essentially like the above function except that it can "surround" a set of Nodes with the
	 * new Node. Thus, the closing Node is created if necessary.
	 * nextSibling and nextEndSibling MUST have the same parent. If not, use the
	 * DTDinsertNode.
	 * This function does not try to know if the location of the new Node is valid.
	 * @param newNode The new Node to insert.
	 * @param parent The parent of the Node.
	 * @param nextSibling The next sibling of the Node.
	 * @param nextEndSibling The next sibling of the closing Node if created. If nextEndSibling ==
	 * nextSibling, the closing Node will be placed at the right of the newly created Node.
	 * All the Nodes between the new Node and its closing Tag will be moved as childs of the new Node.
	 * @param modifs The changes made are logged into modifs.
	 * @return Returns a pointer to the node inserted.
	 */
	static Node *insertNode(Node *newNode, Node *parent, Node *nextSibling, Node *nextEndSibling,
		NodeModifsSet &modifs);

	/**
	 * It behaves essentially like the above function except that it can split the endNodeToSurround and
	 * startNodeToSurround if necessary, according to the offsets.
	 * startNodeToSurround et endNodeToSurround MUST have the same parent. If not, use the last
	 * DTDinsertNode.
	 * This function does not try to know if the location of the new Node is valid.
	 * @param startNodeToSurround The first Node which will be enclosed by the new Node.
	 * @param endNodeToSurround The last Node which will be enclosed by the new Node.
	 * @param startOffset The first Node will be splitted at offset startOffset, the right part will be enclosed.
	 * @param endOffset The last Node will be splitted at offset endOffset, the left part will be enclosed.
	 */
	static Node* insertNode(Node *newNode, Node *parent, Node *startNodeToSurround,
		Node *endNodeToSurround, int startOffset, int endOffset, NodeModifsSet &modifs);

	/**
	 * It behaves essentially like the above function except that it will insert the new Node only if the DTD
	 * allows it, and if generateElements is set to true, it will try to add new Element to be DTD compliant
	 * e.g. surrounding text by TABLE will make TD and TR to be created and inserted to be DTD compliant.
	 * @param generateElements Specifies if we should try add elements to be DTD compliant if newNode
	 * can't be inserted.
	 * @return Returns false if it wasn't possible to insert the tag because e.g. of an invalid parent.
	 */
	static bool DTDinsertNode(Node *newNode, Node *parent, Node *startNodeToSurround,
		Node *endNodeToSurround, int startOffset, int endOffset, NodeModifsSet &modifs,
		bool generateElements = true);

	/**
	 * It behaves essentially like the above function except that the new Tag can surround any subtree. If
	 * necessary, several copies of the Node will be used.
	 * This function takes care of the DTD validity of the Nodes created.
	 * This is the key function making the toolbars working.
	 * @param startNode The first Node which must be surrounded by the new Node.
	 * @param startOffset If firstNode is a text, specify at which offset the new Node must begin to surround.
	 * @param endNode The last Node which must be surrounded by the new Node.
	 * @param endOffset If endNode is a text, specify at which offset the new Node must stop to surround.
	 * @return Returns false if it wasn't possible to insert the tag because e.g. of an invalid parent.
	 */
	static bool DTDinsertNode(Node *newNode, Node *startNode, int startOffset, Node *endNode,
		int endOffset, NodeModifsSet &modifs);

	/**
	 * Create a Node of name nodeName, of type nodeType, (see tag.h) connected to the document doc,
	 * and nextSibling as Node's next sibling.
	 * This function does not try to know if the location of the new Node is valid.
	 * @param nodeName The node's name of the node to create.
	 * @param tagString The string of the tag.
	 * @param nodeType The type of the Node cf Tag::TokenType.
	 * @param doc The Node belongs to this Document.
	 * @param parent The parent of the Node.
	 * @param nextSibling The next sibling of the Node.
	 * @return Returns a pointer to the newly created Node.
	 */
	static Node *createAndInsertNode(const QString &nodeName, const QString &tagString, int nodeType,
		Document *doc, Node* parent, Node* nextSibling, NodeModifsSet &modifs, bool merge = true);

	/**
	 * It behaves essentially like the above function except that it reate its closing Node if necessary
	 *  and then insert them with parent as Node's parent.
	 * nextSibling and nextEndSibling MUST have the same parent. If not, use the
	 * DTDcreateAndInsertNode.
	 * @param nextEndSibling The next sibling of the closing Node if created. If nextEndSibling ==
	 * nextSibling, the closing Node will be placed at the right of the newly created Node.
	 * All the Nodes between the new Node and its closing Tag will be moved as childs of the new Node.
	 * @param modifs The changes made are logged into modifs.
	 */
	static Node *createAndInsertNode(const QString &nodeName, const QString &tagString, int nodeType,
		Document *doc, Node *parent, Node *nextSibling, Node *nextEndSibling, NodeModifsSet &modifs);

	/**
	 * It behaves essentially like the above function except that if necessary, it will split the Nodes.
	 * startNodeToSurround et endNodeToSurround MUST have the same parent. If not, use the
	 * DTDcreateAndInsertNode.
	 * This function does not try to know if the location of the new Node is valid.
	 * @param startNodeToSurround The first Node which will be enclosed by the new Node.
	 * @param endNodeToSurround The last Node which will be enclosed by the new Node.
	 * @param startOffset The first Node will be splitted at offset startOffset, the right part will be enclosed.
	 * @param endOffset The last Node will be splitted at offset endOffset, the left part will be enclosed.
	 */
	static Node *createAndInsertNode(const QString &nodeName, const QString &tagString, int nodeType,
	Document *doc, Node *parent, Node *startNodeToSurround, Node *endNodeToSurround, int startOffset,
	int endOffset, NodeModifsSet &modifs);

	/**
	 * It behaves essentially like the above function except that it will insert the new Node only if the DTD
	 * allows it, and if generateElements is set to true, it will try to add new Element to be DTD compliant
	 * e.g. surrounding text by TABLE will make TD and TR to be created and inserted to be DTD compliant.
	 * @param generateElements Specifies if we should try add elements to be DTD compliant if newNode
	 * can't be inserted.
	 * @return Returns false if it wasn't possible to insert the tag because e.g. of an invalid parent.
	 */
	static bool DTDcreateAndInsertNode(const QString &nodeName, const QString &tagString, int nodeType,
	Document *doc, Node *parent, Node *startNodeToSurround, Node *endNodeToSurround, int startOffset,
	int endOffset, NodeModifsSet &modifs, bool generateElements = true);

	/**
	 * It behaves essentially like the above function except that the new Tag can surround any subtree. If
	 * necessary, several copies of the Node will be used.
	 * This function takes care of the DTD validity of the Nodes created.
	 * This is the key function making the toolbars working.
	 * @param startNode The first Node which must be surrounded by the new Node.
	 * @param startOffset If firstNode is a text, specify at which offset the new Node must begin to surround.
	 * @param endNode The last Node which must be surrounded by the new Node.
	 * @param endOffset If endNode is a text, specify at which offset the new Node must stop to surround.
	 * @return Returns false if it wasn't possible to insert the tag because e.g. of an invalid parent.
	 */
	static bool DTDcreateAndInsertNode(const QString &nodeName, const QString &tagString, int nodeType,
	Document *doc, Node *startNode, int startOffset, Node *endNode, int endOffset, NodeModifsSet &modifs);

	/**
	 * For internal use. From startNode to endNode, it add where possible/necessary a new Node in order
	 * to surround the maximum of Nodes. This is used by the above function. This function calls itself.
	 * @param currentNode This node is currently examined.
	 * @param addingStarted Specifies if we have begun to add the new Node.
	 * @level The relative level of the current Node Sibling (level 0 : root Node, level 1 : childs, and so on...)
	 * MUST BE set to 0.
	 */
	static bool addNodeRecursively(Node *newNode, Node* startNode, Node *endNode, Node* currentNode,
		bool &addingStarted, int level, NodeModifsSet &modifs);

	/**
	 * Create a copy of Node. It use the Node copy operator and add some kafka-specific flags :
	 * It set the cleanStrBuilt to true;
	 * @param node The node to duplicate.
	 * @return Returns the duplicated Node. I wonder if i should always write so obvious things ;-)
	 */
	static Node *duplicateNode(Node *node);

	/**
	 * Extract a Node from the Node Tree. WARNING this will log that the Node was removed.
	 * @param node The node to delete.
	 * @param modifs The changes made are logged into modifs.
	 * @param deleteChilds If we remove or move up the children. WARNING: it don't check
	 * if the children of node are legal childs of the parent of node.
	 * @return Returns the node extracted with its childs
	 */
	static Node* extractNode(Node *node, NodeModifsSet &modifs, bool removeChildren = true);

	/**
	 * Extract and delete Node from the Tree.
	 * It behaves essentially the above function except that it also delete node.
	 * @param deleteClosingTag Delete the closingTag if node isn't single.
	 */
	static void extractAndDeleteNode(Node *node, NodeModifsSet &modifs, bool deleteChildren = true,
		bool deleteClosingTag = true, bool mergeAndFormat = true);

	/**
	 * Moves a Node somewhere else.
	 * @param nodeToMove The node to move :-)
	 * @param newParent The new parent of nodeToMove.
	 * @param newNextSibling The new next Sibling of nodeToMove.
	 * @param modifs The changes made are logged into modifs.
	 */
	static void moveNode(Node *nodeToMove, Node *newParent, Node *newNextSibling, NodeModifsSet &modifs);

	/**
	 * Split a Text Node at offset offset. If offset or n is invalid, nothing is done.
	 * @param n The Node to split.
	 * @param offset Where to split the node.
	 * @param modifs The change made are logged into modifs.
	 * @return Returns if the node was splitted.
	 */
	static bool splitNode(Node *n, int offset, NodeModifsSet &modifs);

	/**
	 * If n and n2 are both Text or Empty Nodes, merge them into one.
	 * @param modifs The changes made are logged into modifs.
	 * @return Returns true if the Nodes were merged, else false.
	 */
	static bool mergeNodes(Node *n, Node *n2, NodeModifsSet &modifs);


	/** ----------------------- NODE MODIFICATIONS -------------------------------------*/

	/**
	 * Computes the end position of a string starting at pos (bLine, bCol).
	 * @param tagString The tagString, representing usually a tag string ;-)
	 * @param bLine The line of the first letter of tagString.
	 * @param bCol The column of the first letter of tagString.
	 * @param eLine Returns the line of the last letter of tagString.
	 * @param eCol Returns the col of the last letter of tagString.
	 */
	static void getEndPosition(const QString & tagString, int bLine, int bCol, int &eLine, int &eCol);

	/**
	 * It behaves essentially like the above function except that the string is the Tag String of Node.
	 * @param node The tag string is taken from node.
	 */
	static void getEndPosition(Node *node, int bLine, int bCol, int &eLine, int &eCol);

	/**
	 * Set the tag string of node, and update the start/end position of the Node.
	 * @param node The node which get the new tag string.
	 * @param newTagString The new tag String :-)
	 */
	static void setTagString(Node *node, const QString &newTagString);

	/**
	 * This function behaves essentially like the above function except that all the others Nodes' position
	 * are updated too.
	 */
	static void setTagStringAndFitsNodes(Node *node, const QString &newTagString);

	/**
	 * Gets the location of a Node in a pointer-independant suit of ints e.g. 1,3,5 means
	 * that the node is the fifth child of the third child of the root Node. Efficient when
	 * deleting the Node tree and rebuilding it when switching between Documents.
	 * @param node The Node we want the location.
	 * @return Returns the location.
	 */
	static QValueList<int> getLocation(Node* node);

	/**
	 * It behaves essentially like the above function except that it operate on DOM::Nodes.
	 */
	static QValueList<int> getLocation(DOM::Node domNode);

	/**
	 * Get the node corresponding to a location. See the above function.
	 * @param loc We want the Node from this location.
	 * @return Returns the Node at location loc.
	 */
	static Node* getNodeFromLocation(QValueList<int> loc);

	/**
	 * It behaves essentially like the above function except that it operate on DOM::Nodes.
	 * @rootNode It needs the root Node of the DOM::Node Tree i.e. the document() Node.
	 */
	static DOM::Node getNodeFromLocation(QValueList<int> loc, DOM::Node rootNode);

	/**
	 * Get the node corresponding to a sublocation.
	 * @param loc A location of a Node.
	 * @locOffset We want the (totalNumberOfParent - locOffset)th parent of Node.
	 * @return Returns a parent of the node pointed by loc.
	 */
	static Node* getNodeFromSubLocation(QValueList<int> loc, int locOffset);


	/** ----------------- DOM::NODE TREE MODIFICATIONS --------------------*/

	/**
	 * Insert a DOM::Node in the DOM::Node tree. It takes care to handle the exceptions.
	 * @param node The node to insert.
	 * @param parent The new parent of node. If null, insert node at the top level.
	 * @param nextSibling The new next sibling of node. If null, append node at the end of the child list.
	 * @param rootNode The root DOM::Node of the DOM::Node tree. Usefull when no parent is provided.
	 * @return Returns true if the operation was successfull.
	 */
	static bool insertDomNode(DOM::Node node, DOM::Node parent = DOM::Node(),
		DOM::Node nextSibling = DOM::Node(), DOM::Node rootNode = DOM::Node());


	/**
	 * Removes a DOM::Node from the DOM::Node Tree. It takes care to handle the exceptions.
	 * @param node The Node to remove from the tree.
	 * @retun Returns true if the operation was successfull..
	 */
	static bool removeDomNode(DOM::Node node);


	/** --------------------- DOM::NODE MODIFICATIONS ---------------------- */

	/**
	 * Create a new DOM::Node. It takes care to check if nodeName is valid.
	 * @param nodeName The DOM::Node's name.
	 * @param dtd The currently used dtd.
	 * @param rootNode The rootNode is needed in order to create a new DOM::Node.
	 * @return Returns the DOM::Node created or a null DOM::Node if nodeName is invalid.
	 */
	static DOM::Node createDomNode(const QString &nodeName, DTDStruct* dtd, DOM::Document rootNode);

	/**
	 * It behaves essentially like the above function.
	 * @param node The DOM::Node will be created with node's name.
	 */
	static DOM::Node createDomNode(Node *node, DOM::Document rootNode);

	/**
	 * Create a new Text Node.
	 * @param textString The text inside the new text DOM::Node.
	 * @param rootNode The rootNode is needed in order to create a new Text DOM::Node.
	 * @return a new text DOM::Node.
	 */
	static DOM::Node createTextDomNode(const QString &textString, DOM::Document rootNode);

	/**
	 * Create a new attribute and check if the attrName can have this attribute.
	 * @param nodeName The node name of the DOM::Node which will get this attribute.
	 * @param dtd The currently used dtd.
	 * @param attrName The name of the new attribute.
	 * @param attrValue The value of the new attribute.
	 * @param rootNode The rootNode is needed in order to create a new Attribute.
	 * @return Returns the new Attribute or a null DOM::Node if attrName is invalid.
	 */
	static DOM::Node createDomNodeAttribute(const QString &nodeName, DTDStruct* dtd,
		const QString &attrName, const QString &attrValue, DOM::Document rootNode);

	/**
	 * It behaves essentially like the above function.
	 * @param node The corresponding DOM::Node of node will get the attribute. It don't add the attribute.
	 */
	static DOM::Node createDomNodeAttribute(Node* node, const QString &attrName, DOM::Document rootNode);

	/**
	 * It behaves essentially like the above function except that it use the DOM::Node->Node* link to get the
	 * corresponding Node. So be sure that the link is set.
	 * @param node The node which will get the attribute. It don't add the attribute.
	 */
	//static DOM::node createDomNodeAttribute(DOM::Node node, const QString &attrName,
	//	DOM::Document rootNode);

	/**
	 * Append a new attribute to a DOM::Node.
	 * @param node The node which will get the new attribute.
	 * @param attr The new attribute to add.
	 * @return Returns if the operation was successfull.
	 */
	static bool insertDomNodeAttribute(DOM::Node node, DOM::Node attr);

	/**
	 * It behaves essentially like the above function except that if the attribute doesn't exist, it will create it,
	 * and then it fills the attribute with attrValue.
	 * @param nodeName The name of the Node corresponding to node.
	 * @param dtd The currently used DTD.
	 * @param attrName The name of the (new) Attribute.
	 * @param attrValue The value of the new Attribute.
	 * @param rootNode The rootNode is needed in order to create a new Attribute.
	 * @return Returns if the operation was successfull.
	 */
	static bool editDomNodeAttribute(DOM::Node node, const QString &nodeName, DTDStruct* dtd,
		const QString &attrName, const QString &attrValue, DOM::Document rootNode);

	/**
	 * It behaves essentially like the above function.
	 * @param node The DOM::Node comes from this node.
	 */
	static bool editDomNodeAttribute(DOM::Node domNode, Node* node,
		const QString &attrName, const QString &attrValue, DOM::Document rootNode);

	/** ----------------------- MISCELLANEOUS -------------------------------------*/

	/**
	 * TEMPORARY, HTML specific
	 * @return Returns true if it is a inline Node. Official List, unlike getNodeDisplay().
	 */
	 static bool isInline(const QString &nodename);

	/**
	 * Prints in stdout the current Node tree.
	 */
	static void coutTree(Node *node, int indent);


};

#endif
