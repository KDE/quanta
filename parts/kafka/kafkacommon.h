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

class Node;
namespace DOM
{
	class Node;
}
class NodeModifsSet;
class Document;

/**
 * For heavy debug including Node Tree in stdout printing, a DOM::Node tree widget.
 */
//#define HEAVY_DEBUG

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

	/**
	 * This function returns the next Node after node : the first child of
	 * node if available, else its next sibling if available, else the next
	 * available next sibling of a parent of node.
	 * @param _node It is the Node from which we want the next Node.
	 * @param goUp This boolean specifies if we should go up or down in the tree.
	 * For a normal use, It must be set to false at the
	 * beginning and then the same boolean must be used when using
	 * several times this function.
	 * @param endNode Specifies at which Node the search should end.
	 * @return Returns the next Node.
	 */
	static Node* getNextNode(Node *node, bool &goUp, Node *endNode = 0L);

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
	static DOM::Node getNextDomNode(DOM::Node node, bool &goUp, bool returnParentNode = false, DOM::Node endNode = DOM::Node());

	/**
	 * Fits the Nodes position after a change in the Node tree.
	 * @param startNode The Node where the update starts.
	 * @param colMovement The number of columns that should be
	 * added/retrieved from the column position.
	 * @param lineMovement The number of lines that should be
	 * added/retrieved from the line position.
	 * @param colEnd The column position where the update should stop.
	 * @param lineEnd The line position where the update should stop.
	 */
	static void fitsNodesPosition(Node* startNode, int colMovement, int lineMovement = 0,
		int colEnd = -2, int lineEnd = -2);

	/**
	 * Gets the location of a Node in a pointer-independant suit of ints e.g. 1,3,5 means
	 * that the node is the fifth child of the third child of the root Node. Efficient when
	 * deleting the Node tree and rebuilding it when switching between Documents.
	 * @param node The Node we want the location.
	 * @return Returns the location.
	 */
	static QValueList<int> getLocation(Node* node);

	/**
	 * Get the node corresponding to a location. See the above function.
	 * @param loc We want the Node from this location.
	 * @return Returns the Node at location loc.
	 */
	static Node* getNodeFromLocation(QValueList<int> loc);

	/**
	 * Get the node corresponding to a sublocation.
	 * @param loc A location of a Node.
	 * @locOffset We want the (totalNumberOfParent - locOffset)th parent of Node.
	 * @return Returns a parent of the node pointed by loc.
	 */
	static Node* getNodeFromSubLocation(QValueList<int> loc, int locOffset);

	/**
	 * Create a simple Node, without taking care of building the closing Node.
	 */
	static Node* createNode(QString nodeName, QString tagString, int nodeType, Document *doc);

	/**
	 * Insert node in the tree. WARNING This function will log that node was created.
	 * @param node The node to insert.
	 * @param parentNode This Node will be the parent of node.
	 * @param nextSibling This Node will be the next Sibling of Node. If null, node will be appened at
	 * the child list of parentNode.
	 * @param modifs The changes made are logged into modifs.
	 */
	static void insertNode(Node *node, Node* parentNode, Node* nextSibling, NodeModifsSet &modifs);

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
	 * @return Returns a pointer to the newly created Node.
	 */
	static Node *insertNode(Node *newNode, Node *parent, Node *nextSibling, Node *nextEndSibling,
		NodeModifsSet &modifs);

	/**
	 * It behaves essentially like the above function except that it can split the endNodeToSurround and
	 * startNodeToSurround if necessary, according to the offsets.
	 * startNodeToSurround et endNodeToSurround MUST have the same parent. If not, use the
	 * DTDinsertNode.
	 * This function does not try to know if the location of the new Node is valid.
	 * @param startNodeToSurround The first Node which will be enclosed by the new Node.
	 * @param endNodeToSurround The last Node which will be enclosed by the new Node.
	 * @param startOffset The first Node will be splitted at offset startOffset, the right part will be enclosed.
	 * @param endOffset The last Node will be splitted at offset endOffset, the left part will be enclosed.
	 */
	static bool insertNode(Node *newNode, Node *parent, Node *startNodeToSurround,
		Node *endNodeToSurround, int startOffset, int endOffset, NodeModifsSet &modifs);

	/**
	 * It behaves essentially like the above function except that it will insert the new Node only if the DTD
	 * allows it.
	 * @return Returns false if it wasn't possible to insert the tag because e.g. of an invalid parent.
	 */
	static bool DTDinsertNode(Node *newNode, Node *parent, Node *startNodeToSurround,
		Node *endNodeToSurround, int startOffset, int endOffset, NodeModifsSet &modifs);

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
	 * create its closing Node if necessary and then insert them with parent as Node's parent.
	 * nextSibling and nextEndSibling MUST have the same parent. If not, use the
	 * DTDcreateAndInsertNode.
	 * This function does not try to know if the location of the new Node is valid.
	 * @param nodeName The node's name of the node to create.
	 * @param tagString The string of the tag.
	 * @param nodeType The type of the Node cf Tag::TokenType.
	 * @param doc The Node belongs to this Document.
	 * @param parent The parent of the Node.
	 * @param nextSibling The next sibling of the Node.
	 * @param nextEndSibling The next sibling of the closing Node if created. If nextEndSibling ==
	 * nextSibling, the closing Node will be placed at the right of the newly created Node.
	 * All the Nodes between the new Node and its closing Tag will be moved as childs of the new Node.
	 * @param modifs The changes made are logged into modifs.
	 * @return Returns a pointer to the newly created Node.
	 */
	static Node *createAndInsertNode(QString nodeName, QString tagString, int nodeType, Document *doc,
		Node *parent, Node *nextSibling, Node *nextEndSibling, NodeModifsSet &modifs);

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
	static Node *createAndInsertNode(QString nodeName, QString tagString, int nodeType, Document *doc,
		Node *parent, Node *startNodeToSurround, Node *endNodeToSurround, int startOffset, int endOffset,
		NodeModifsSet &modifs);

	/**
	 * It behaves essentially like the above function except that it will insert the new Node only if the DTD
	 * allows it.
	 * @return Returns false if it wasn't possible to insert the tag because e.g. of an invalid parent.
	 */
	static bool DTDcreateAndInsertNode(QString nodeName, QString tagString, int nodeType, Document *doc,
		Node *parent, Node *startNodeToSurround, Node *endNodeToSurround, int startOffset, int endOffset,
		NodeModifsSet &modifs);

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
	static bool DTDcreateAndInsertNode(QString nodeName, QString tagString, int nodeType, Document *doc,
		Node *startNode, int startOffset, Node *endNode, int endOffset, NodeModifsSet &modifs);

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
	 * Create a copy of Node. This is a custom copy function with some pointers set to NULL like listItem
	 * @param node The node to duplicate.
	 * @return Returns the duplicated Node. I wonder if i should always write so obvious things ;-)
	 */
	static Node *duplicateNode(Node *node);

	/**
	 * Extract a Node from the Node Tree. WARNING this will log that the Node was deleted.
	 * @param node The node to delete.
	 * @param modifs The changes made are logged into modifs.
	 * @param deleteChilds If we delete or move up the children. WARNING: it don't check
	 * if the children of node are legal childs of the parent of node.
	 * @param mergeAndFormat Specify if we should merge Text/Empty Noded and apply formatting
	 * by creating some Empty Nodes.
	 * @return Returns the node extracted with its childs
	 */
	static Node* extractNode(Node *node, NodeModifsSet &modifs, bool deleteChildren = true,
		bool mergeAndFormat = true);

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

	/**
	 * Get the display type of a Node. NOT a official list, more a little hack to
	 * handle the indentation.
	 * @param nodeName The name of the Node.
	 * @return Returns the type.
	 */
	static int getNodeType(QString nodeName);

	//the enumeration of the different display types
	enum nodeType
	{
		noneDisplay = 0,
		inlineDisplay,
		blockDisplay
	};

	/**
	 * Prints in stdout the current Node tree.
	 */
	static void coutTree(Node *node, int indent);


};

#endif
