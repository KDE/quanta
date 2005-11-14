/***************************************************************************
                               kafkacommon.h
                             -------------------

    copyright            : (C) 2003, 2004 - Nicolas Deschildre
    email                : ndeschildre@kdewebdev.org
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
class NodeSelection;
class NodeSelectionInd;
class Document;
struct DTDStruct;

/**
 * For heavy debug including Node Tree in stdout printing, a DOM::Node tree widget.
 */
//#define HEAVY_DEBUG

/**
 * Light debugging, including functions name in stdout printing.
 */
//#define LIGHT_DEBUG


/** This class gathers all the basic functions needed by kafka. 
 * It's very useful for manipulating nodes.
 */

class kafkaCommon
{
public:
	kafkaCommon() {}
	~kafkaCommon() {}

	/** ----------------------- NODE & DOM::NODE TREE NAVIGATION -------------------------------------*/

	/**
	 * This function returns the next Node after node : the first child of
	 * node if available, else its next sibling if available, else the next
	 * available next sibling of a parent of node.
	 * @param _node It is the Node from which we want the next Node.
	 * @param goUp This boolean specifies if we should go up (torwards the root Node)
	 * i.e. not looking at the childs of node, or make a standart iteration.
	 * For a normal use, It must be set to false at the
	 * beginning and then the same boolean must be used when using
	 * several times this function.
	 * @param endNode Specifies at which Node the search should end.
	 * @return Returns the next Node.
	 */
	static Node* getNextNode(Node *node, bool &goUp, Node *endNode = 0L);

	/**
	 * It behaves essentially like the above function except that it will skip empty Nodes.
	 */
	static Node* getNextNodeNE(Node *node, bool &goUp, Node *endNode = 0L);

	/**
	 * This function returns the prev Node after node.
	 */
	static Node* getPrevNode(Node *node, Node *endNode = 0L);

	/**
	 * It behaves essentially like the above function except that it will skip empty Nodes.
	 */
	static Node* getPrevNodeNE(Node *node, Node *endNode = 0L);

    
    /**
     * Returns the first common parent to startNode and endNode that isn't inline.
     * @param startNode Start node.
     * @param endNode End node.
     * @param commonParentStartChildLocation  Is the child of commonParent which is parent of startNode
     * @param commonParentEndChildLocation Is the child of commonParent which is parent of endNode
     * @param nodeSubtree When startNode and endNode doesn't belong to the current document tree. Pass 0 if they do.
     * @return The first, non inline, common parent of startNode and endNode.
     */
    static Node* DTDGetNonInlineCommonParent(Node* startNode, Node* endNode, 
                                             QValueList<int>& commonParentStartChildLocation, 
                                             QValueList<int>& commonParentEndChildLocation, Node* nodeSubtree);

    /**
     * Same as above, only that the common parent can be inline.
     */
    static Node* DTDGetCommonParent(Node* startNode, Node* endNode, 
                                    QValueList<int>& commonParentStartChildLocation, 
                                    QValueList<int>& commonParentEndChildLocation, Node* nodeSubtree);

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

	/**
	 * This function returns the previous Node after node.
	 * @return Returns the previous DOM::Node of node.
	 */
	static DOM::Node getPrevDomNode(DOM::Node node, DOM::Node endNode = DOM::Node());
 
     /**
      * If node is not a text node or the cursor is at the end of node's tag string, this method return the next
      * text node at offset 0, or a null pointer.
      * This is particular useful when finding start and end nodes of a selection, because you can have a closing node
      * as the start node, etc.
      * @param startNode The current start node. startNode isn't changed inside the method.
      * @param startOffset The current start offset. startOffset is changed inside the method.
      * @return The next text node or a null pointer.
      */
     static Node* getCorrectStartNode(Node* startNode, int& startOffset);
    
     /**
      * If node is not a text node or the cursor is at the beggining of node's tag string, this method return the previous
      * text node at offset at the end of the tag string, or a null pointer.
      * This is particular useful when finding start and end nodes of a selection, because you can have a closing node
      * as the start node, etc.
      * @param endNode The current start node.
      * @param endOffset The current start offset.
      * @return The next text node or a null pointer.
      */
     static Node* getCorrectEndNode(Node* endNode, int& endOffset);
     
     /**
      * Get the first child of commonParent which is parent of node
      * @param node 
      * @param commonParent 
      * @return 
      */
     static Node* getCommonParentChild(Node* node, Node* commonParent);


	/** ----------------------- NODE INDENTATION STUFF -------------------------------------*/

	/**
	 * This function takes care to modify the current node or/and the next sibling in order to have a nice
	 * indentation. WARNING it doesn't create the necessary Nodes. Call fitIndentationNodes first.
	 * @param node Apply nice indentation to this Node.
	 * @param nbOfSpaces Specifies the number of spaces the minimal indentation must be.
	 * @param nbOfTabs Specifies the number of tabs the minimal indentation must be.
     * @param modifs The changes made are logged into modifs.
     * @param inlineNodeIndentation 
     * If true: 
     *      <body>Text</body> 
     * else: 
     *      <body>
     *          Text
     *      </body>
	 */
	static void applyIndentation(Node *node, int nbOfSpaces, int nbOfTabs, NodeModifsSet *modifs, 
                                 bool inlineNodeIndentation = false);

	/**
	 * Create/Delete the necessary Empty Nodes between n1 and n2 so that a nice indentation can be
	 * generated by the undoRedo system. The undoRedo system can't create itself these Nodes
	 * because it will create them only during the synchronization, making all the QValueList<int>
	 * Nodes location wrong.
	 * WARNING n1 and n2 must be siblings or parent-child. If there are sibling and n1 is a XmlTag,
	 * n1 should not have non empty childs.
	 * @param n1 The start node.
	 * @param n2 The end node.
         * @param modifs The changes made are logged into modifs.
	 */
	static void fitIndentationNodes(Node *n1, Node *n2, NodeModifsSet *modifs);

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
	 * Get the display type of a Node. NOT an official list, more a little hack to
	 * handle the indentation. Text are inline. The rest return an error.
	 * @param closingNodeToo Specifies if we consider that closing Node have the same type as
	 * their opening tag.
	 * @return Returns the type.
	 */
	static int getNodeDisplay(Node *node, bool closingNodeToo);

	//the enumeration of the different display types
	enum nodeDisplay
	{
		noneDisplay = 0,
		inlineDisplay,
		blockDisplay,
		errorDisplay
	};

	/**
	 * Remove the indentation whitespaces in a string
	 * e.g. this function returns : " a b  cd " for parameter: "     a b  cd     "
	 * @param string The text to modify.
	 * @param removeAllSpacesAtTheLeft Specifies if it should remove ALL spaces in the left
	 * unlike the above example.
	 * @param removeAllSpacesAtTheRight Specifies if it should remove ALL spaces in the right
	 * unlike the above example.
	 * @return Returns the modified string.
	 */
	static QString removeUnnecessaryWhitespaces(const QString &string,
		bool removeAllSpacesAtTheLeft = false, bool removeAllSpacesAtTheRight = false);


	/** ----------------------- NODE TREE MODIFICATIONS -------------------------------------*/

	/**
	 * Create a simple Node, without taking care of building the closing Node.
	 * @param nodeName The name of the Node.
	 * @param tagString The String of the tag as it will appears in the editor.
	 * @param nodeType The node type, cf Tag::TokenType
	 * @param doc The document the Node will belong to.
	 * @return Returns the newly created node.
	 */
	static Node* createNode(const QString &nodeName, const QString &tagString, int nodeType,
		Document *doc);

    /**
     * Restore a Node that has been pasted, i.e., his doc and dtd pointers.
     * @param node The Node to be restored.
     * @param doc The document the Node will belong to.
     */
    static void restorePastedNode(Node* node, Document* doc);
    
    /**
	 * Create a !doctype Node with all the necessary attributes. It has a child and a closing Node.
	 * @param doc It needs the document where the !doctype node will be inserted in order to
	 * build the right attributes.
	 */
	static Node *createDoctypeNode(Document *doc);

	/**
	 * Create a <?xml ... ?> Node. It has a child and a closing Node.
	 * @param doc It needs the document where the xml node will be inserted.
	 * @param encoding The encoding to use (usually get it with quantaApp->defaultEncoding())
	 */
	static Node *createXmlDeclarationNode(Document *doc, const QString &encoding);

	/**
	 * Create a node subtree which contains the mandatory Nodes in order to be DTD compliant.
	 * e.g. TABLE alone isn't DTD compliant, this function will return TABLE->TR->TD.
	 * WARNING : it won't log change thus node must NOT be in the Node tree.
	 * @param node The root Node of the Node subtree.
	 * @param doc The document the Node subtree will belong to.
	 * @return Returns the last Node of the subtree or node if there was nothing to add.
	 */
	static Node* createMandatoryNodeSubtree(Node *node, Document *doc);

	/**
	 * Insert node in the tree. WARNING This function will log that node was added.
	 * WARNING : baseNode is used as the rootNode.
	 * It will also try to merge text/Empty Nodes.
	 * @param node The node to insert.
	 * @param parentNode This Node will be the parent of node.
	 * @param nextSibling This Node will be the next Sibling of Node. If null, node will be appended at
	 * the child list of parentNode.
	 * TODO: @param rootNode The rootNode of the tree we want to insert the Node (usually &baseNode).
	 * @param modifs The changes made are logged into modifs. Put 0L if you don't want to log
	 * and if you know what you're doing!
	 * @param merge Try to merge with the siblings if possible.
	 * @return Returns a pointer to the node inserted.
	 */
	static Node* insertNode(Node *node, Node* parentNode, Node* nextSibling,
                            NodeModifsSet *modifs/**, Node **rootNode*/, bool merge = true);

    static Node* insertNode(Node *node, Node* parentNode, Node* nextSibling, NodeSelection& selection, 
                            NodeModifsSet *modifs, bool merge = true);
 
	/**
	 * It behaves essentially like the above function except that it can "surround" a set of Nodes with the
	 * new Node. Thus, the closing Node is created if necessary.
	 * nextSibling and nextEndSibling MUST have the same parent. If not, use the
	 * DTDinsertNode.
	 * This function does not try to know if the location of the new Node is DTD valid.
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
		NodeModifsSet *modifs, bool merge = true);

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
		Node *endNodeToSurround, int startOffset, int endOffset, NodeModifsSet *modifs);

	/**
	 * It behaves essentially like the above function except that it will insert the new Node only
	 * if the DTD allows it. The new Tag can surround any subtree. If
	 * necessary, several copies of the Node will be used.
	 * This function takes care of the DTD validity of the Nodes created.
	 * It will build the necessary mandatory Nodes (e.g. insertion of TABLE will also insert TR and TD).
	 * This is the key function making the toolbars working.
	 * @param startNode The first Node which must be surrounded by the new Node.
	 * @param startOffset If firstNode is a text, specify at which offset the new Node must begin to surround.
	 * @param endNode The last Node which must be surrounded by the new Node.
	 * @param endOffset If endNode is a text, specify at which offset the new Node must stop to surround.
	 * @param doc The document is needed in order to build the mandatory Node tree if necessary.
	 * <TEMPORARY> : We want to keep track of the cursor position. TODO : cursor class
	 * @param cursorNode The cursor is inside cursorNode.
	 * @param cursorOffset The offset of the cursor inside cursorNode.
	 * </TEMPORARY>
	 * @return Returns false if it wasn't possible to insert the tag because e.g. of an invalid parent.
	 */
	static bool DTDinsertNode(Node *newNode, Node *startNode, int startOffset, Node *endNode,
		int endOffset, Document *doc, Node **cursorNode, long &cursorOffset, NodeModifsSet *modifs);

	/**
	 * It behaves essentially like the above function except that it will try first to remove newNode
	 * from the area, by calling DTDExtractNode. If newNode wasn't present, it will then insert it by
	 * calling DTDinsertNode.
	 * This is the key function making the toolbars working.
     * @return Returns true if a modification was done (Node inserted/removed)
	 */
	static bool DTDinsertRemoveNode(Node *newNode, Node *startNode, int startOffset, Node *endNode,
		int endOffset, Document *doc, Node **cursorNode, long &cursorOffset, NodeModifsSet *modifs);

	/**
	 * Insert a node subtree in the tree. WARNING This function will log that the nodes were added.
	 * WARNING : baseNode is used as the rootNode.
	 * It will also try to merge text/Empty Nodes.
	 * @param node The root node of the Node subtree to insert.
	 * @param parentNode This Node will be the parent of node.
	 * @param nextSibling This Node will be the next Sibling of Node. If null, node will be appended at
	 * the child list of parentNode.
	 * @param modifs The changes made are logged into modifs. Put 0L if you don't want to log
	 * and if you know what you're doing!
	 * @param merge Try to merge with the siblings if possible.
	 * @return Returns a pointer to the node inserted.
	 */
	static Node* insertNodeSubtree(Node *node, Node* parentNode, Node* nextSibling,
		NodeModifsSet *modifs, bool merge = true);

	/**
	 * It behaves essentially like the above function except that it can "surround" a set of Nodes with the
	 * new Node. Thus, the closing Node is created if necessary.
	 * nextSibling and nextEndSibling MUST have the same parent. If not, use the
	 * DTDinsertNode.
	 * The Node Subtree MUST be a single-Node-per-parent subtree.
	 * This function does not try to know if the location of the new Node is DTD valid.
	 * @param node The root node of the Node subtree to insert.
	 * @param parent The parent of the Node.
	 * @param nextSibling The next sibling of the Node.
	 * @param nextEndSibling The next sibling of the closing Node if created. If nextEndSibling ==
	 * nextSibling, the closing Node will be placed at the right of the newly created Node.
	 * All the Nodes between the new Node and its closing Tag will be moved as childs of the
	 * last Node of the Node subtree..
	 * @param modifs The changes made are logged into modifs.
	 * @return Returns a pointer to the node inserted.
	 */
	static Node* insertNodeSubtree(Node *node, Node* parentNode, Node* nextSibling,
                                   Node* nextEndSibling, NodeModifsSet *modifs, bool merge = true);

	/**
     * Split the Nodes as necessary, then check that the subtree is allowed to be inserted 
     * and then insert the subtree. 
     * @param node The root node of the Node subtree to insert.
     * @param selection contains the cursor Node where the insertion will take place.
     * @param modifs The changes made are logged into modifs.
     */
    static Node* DTDInsertNodeSubtree(Node *node, NodeSelectionInd& selection, 
                                      Node **cursorNode, long& cursorOffset, NodeModifsSet *modifs);
	
    static Node* DTDInsertNodeSubtree(Node* newNode, Node* parentNode, Node* nextSibling, 
                                      NodeSelection& cursorHolder, NodeModifsSet *modifs);
    
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
		Document *doc, Node* parent, Node* nextSibling, NodeModifsSet *modifs, bool merge = true);

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
		Document *doc, Node *parent, Node *nextSibling, Node *nextEndSibling, NodeModifsSet *modifs);

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
	static Node *createAndInsertNode(const QString &nodeName, const QString &tagString,
		int nodeType, Document *doc, Node *parent, Node *startNodeToSurround,
		Node *endNodeToSurround, int startOffset, int endOffset, NodeModifsSet *modifs);

	/**
	 * It behaves essentially like the above function except that it will insert the new Node only
	 * if the DTD allows it. The new Tag can surround any subtree. If
	 * necessary, several copies of the Node will be used.
	 * This function takes care of the DTD validity of the Nodes created.
	 * It will build the necessary mandatory Nodes (e.g. insertion of TABLE will also insert TR and TD).
	 * This is the key function making the toolbars working.
	 * @param startNode The first Node which must be surrounded by the new Node.
	 * @param startOffset If firstNode is a text, specify at which offset the new Node must begin to surround.
	 * @param endNode The last Node which must be surrounded by the new Node.
	 * @param endOffset If endNode is a text, specify at which offset the new Node must stop to surround.
	 * @return Returns false if it wasn't possible to insert the tag because e.g. of an invalid parent.
	 */
	static bool DTDcreateAndInsertNode(const QString &nodeName, const QString &tagString, int nodeType,
		Document *doc, Node *startNode, int startOffset, Node *endNode, int endOffset,
		Node **cursorNode, long &cursorOffset, NodeModifsSet *modifs);

	/**
	 * For internal use. From startNode to endNode, it add where possible/necessary a new Node in order
	 * to surround the maximum of Nodes. This is used by the above function. This function calls itself.
	 * @param newNode The root Node of the node subtree to insert.
	 * @param leafNode The leaf Node of the node subtree to insert.
	 * @param startExaminationNode It will start examine Nodes from startExaminationNode.
	 * @param endExaminationNode It will stop examine Nodes from endExaminationNode.
	 * @param startNode This function will start adding newNode from startNode.
	 * @param endNode This function will stop adding newNode at endNode.
	 * @param currentNode This node is currently examined.
	 * @param examinationStarted Specifies if we have begun to examine the Nodes.
	 * @param addingStarted Specifies if we have begun to add the new Node.
         * @param nodeInserted Returns true if newNode was inserted at least once. Set to false before calling the function.
	 * @level The relative level of the current Node Sibling (level 0 : root Node, level 1 : childs, and so on...)
	 * MUST BE set to 0.
	 */
	static bool addNodeRecursively(Node *newNode, Node *leafNode, Node *startExaminationNode,
		Node *endExaminationNode, Node* startNode, Node *endNode, Node* currentNode,
		bool &examinationStarted, bool &addingStarted, bool &nodeInserted, int level, NodeModifsSet *modifs);

	/**
	 * Create a copy of Node. It use the Node copy operator and add some kafka-specific flags :
	 * It set the node->tag->cleanStrBuilt and node->tag->indentationDone to false;
	 * @param node The node to duplicate.
	 * @return Returns the duplicated Node. I wonder if i should always write so obvious things ;-)
	 */
	static Node *duplicateNode(Node *node);

	/**
	 * It behaves essentially like the above function except that it can handle a node Subtree.
	 * INNEFICIENT for bi
	 */
    static Node *duplicateNodeSubtree(Node *node, bool childAndClosingTagOnly = false);
 
    /**
     * Returns the closing tag of node or its last child or itself.
     * @param node 
     * @return 
     */
    static Node* getLastChild(Node* node);

	/**
	 * Extract a Node from the Node Tree. WARNING this will log that the Node was removed.
	 * This mean that the undo/redo system will delete it when necessary so don't reuse it!!!!
	 * @param node The node to delete.
	 * @param modifs The changes made are logged into modifs.
	 * @param extractChilds If we extract or move up the children. WARNING: it don't check
	 * if the children of node are legal childs of the parent of node.
	 * @param removeClosingTag Extract the closingTag if node isn't single and is Tag::XmlTag.
	 * TODO: @param removeEmbeddedTags Specifies if we delete the embedded Nodes e.g.
	 * <a href="<? boo ?>" > : the PHP block is an embedded block.
	 * @return Returns the node extracted with its childs
	 */
	static Node* extractNode(Node *node, NodeModifsSet *modifs, bool extractChildren = true,
		bool extractClosingTag = false/**, bool removeEmbeddedTags = false*/);

	/**
	 * It behaves essentially like the above function.
	 * Extract and BUT NOT DELETE RIGHT NOW node from the Tree. The undo/redo system will delete it
	 * when necessary.
	 * TODO: remove it, and use extractNode instead.
	 * @param deleteClosingTag Delete the closingTag if node isn't single.
	 */
	static void extractAndDeleteNode(Node *node, NodeModifsSet *modifs, bool deleteChildren = true,
		bool deleteClosingTag = true, bool mergeAndFormat = true);
    
    /**
     * Extract a node subtree in the tree. WARNING This function will log that the nodes were added.
     * This funtion not only extract the start node but also will extract inline parents.
     * @param startNode The node from which we start the removal.
     * @param startOffset The offset of startNode from which we start the removal.
     * @param endNode The node from which we end the removal.
     * @param endOffset The offset of endNode from which we end the removal.
     * @param cursorNode The cursor is inside cursorNode.
     * @param cursorOffset The offset of the cursor inside cursorNode.
     * @return Returns a pointer to the node inserted.
     */
    static Node* DTDExtractNodeSubtree(Node *startNode, int startOffset, Node *endNode, int endOffset, 
                                       Node **cursorNode, long &cursorOffset, NodeModifsSet *modifs, bool extractInlineParentNodes = true);
    
    /**
     * Similar to the above function but it operates on the given node tree. See DTDGetNodeSubtree.
     * @param nodeSubtree The Node tree on which we're going to make the removal.
     * @return Returns a pointer to the node inserted.
     */
    static Node* DTDExtractNodeSubtree(Node *startNode, int startOffset, Node *endNode, int endOffset, 
                                       Node* nodeSubtree, NodeModifsSet* modifs, bool extractInlineParentNodes = true);
    
    static Node* extractNodeSubtreeAux(Node* commonParentStartChild, Node* commonParentEndChild, NodeModifsSet* modifs);
    
    /**
     * It behaves essentially like the above function. Provided for convenience.
     */
    static Node* DTDRemoveSelection(NodeSelectionInd& selection, 
                                    Node **cursorNode, long& cursorOffset, NodeModifsSet *modifs, bool extractInlineParentNodes = true);

    /**
     * Get a node subtree from the tree. It is similar to extractNodeSubtree() 
     * but it doesn't extract anything. 
     * It's useful to get a copy of the Node subtree from a selection, for example.
     * This funtion not only extract the start node but also will extract inline parents.
     * @param startNode The starting Node.
     * @param startOffset If firstNode is a text, specify at which offset the new start Node will be splitted.
     * @param endNode The ending Node.
     * @param endOffset If endNode is a text, specify at which offset the new end Node will be splitted.
     * @return Returns a pointer to the Node subtree.
     */
    static Node* getNodeSubtree(Node *startNode, int startOffset, Node *endNode, int endOffset, bool extractInlineParentNodes = true);
	
    /**
	 * An enumeration of all the possible return states of DTDExtractNode
	 */
	enum extractNodeStatus
	{
		//The node to extract was not found.
		nothingExtracted = 0,
		//The extract operation stopped because of a DTD error : if the node was removed, the child
		//weren't able to be childs of the node's parent, according to the DTD. Should not occur
		//except really bad HTML.
		extractionStoppedDueToBadNodes,
		//everything has gone fine
		extractionDone,
		//Invalid start or end position, or the given Node was a block.
		extractionBadParameters
	};

	/**
	 * This function will try to extract the node nodeName (of type XmlTag) from a given subtree,
	 * according to the DTD. If the DTD don't allow it, it won't remove it.
	 * This function is only interesting for the removal of Inline Nodes thus it will return an error if
	 * a block nodeName is submitted.
	 * TODO: AVOID splitting of Node when the DTD don't allow the removal.
	 * @param nodeName The name of the Node to remove (must be inline).
	 * @param doc It is needed to get the DTD informations.
	 * @param startNode The node from which we start the removal.
	 * @param startOffset The offset of startNode from which we start the removal.
	 * @param endNode The node from which we end the removal.
	 * @param endOffset The offset of endNode from which we end the removal.
	 * <TEMPORARY> : We want to keep track of the cursor position. TODO : cursor class
	 * @param cursorNode The cursor is inside cursorNode.
	 * @param cursorOffset The offset of the cursor inside cursorNode.
	 * </TEMPORARY>
	 * @param modifs The usual modifs to log the modifications made for the undo/redo system.
	 * @return Returns a kafkaCommon::extractNodeStatus.
	 */
	static int DTDExtractNode(const QString &nodeName, Document *doc, Node *startNode,
		int startOffset, Node *endNode, int endOffset, Node **cursorNode, long &cursorOffset,
		NodeModifsSet *modifs);

	/**
	 * Moves a Node somewhere else.
	 * @param nodeToMove The node to move :-)
	 * @param newParent The new parent of nodeToMove.
     * @param newNextSibling The new next Sibling of nodeToMove. If null, node will be appended at
     * the child list of parentNode.
	 * @param modifs The changes made are logged into modifs.
	 * @param merge Specifies if it should try to merge the Node at its new location.
	 */
	static void moveNode(Node *nodeToMove, Node *newParent, Node *newNextSibling,
                         NodeModifsSet *modifs, bool merge = true, bool moveClosingNode = false);

    static void moveNode(Node *nodeToMove, Node *newParent, Node *newNextSibling, NodeSelection& cursorHolder,
                         NodeModifsSet *modifs, bool merge = true, bool moveClosingNode = false);

	/**
	 * Split a Text Node at offset offset. If offset or n is invalid, nothing is done.
	 * @param n The Node to split.
	 * @param offset Where to split the node.
	 * @param modifs The change made are logged into modifs.
	 * @return Returns if the node was splitted.
	 */
	static bool splitNode(Node *n, int offset, NodeModifsSet *modifs);
 
    /**
     * This method takes care of spliting start and end nodes, if needed, finding the commonParent, 
     * commonParentStartChild and commonParentEndChild and split the start and end node subtrees, 
     * calling splitStartNodeSubtree and splitEndNodeSubtree.
     * The following tree:
     *  <body>
     *      <b>                     --> commonParent
     *          <i>                 --> commonParentStartChild
     *              select|
     *              here            --> startNode  
     *          </i>
     *          continue
     *          <u>                 --> commonParentEndChild
     *              stop|           --> endNode
     *              more
     *          </u>
     *          text
     *      </b>
     *  <body>
     * Is changed to:
     *  <body>
     *      <b>
     *          <i>
     *              select|
     *          </i>
     *      </b>
     *      <b>
     *          <i>
     *              here
     *          </i>
     *          continue
     *          <u>
     *              stop
     *          </u>
     *      </b>
     *      <b>
     *          <u>
     *              more
     *          </u>
     *          text
     *      </b>
     *  </body>
     * @param startNode The node where a selection starts, for example.
     * @param startOffset
     * @param endNode The node where a selection ends, for example.
     * @param endOffset
     * @param commonParent This is the common parent between start and end node. 
     * If 0, it tries to find the commonParent, else it uses the passed node.
     * @param commonParentStartChildLocation The first child of commonParent which is parent of startNode is stored here.
     * @param commonParentEndChildLocation The first child of commonParent which is parent of endNode is stored here.
     * @param cursorNode The cursor node is stored here.
     * @param cursorOffset The cursor offset is stored here.
     * @param subTree The node corresponding to the start of a subtree that doesn't belong to the current document, or 0.
     * @param modifs The changes made are logged into modifs.
     */
    static void splitStartAndEndNodeSubtree(Node*& startNode, int startOffset, Node*& endNode, int endOffset, Node*& commonParent, 
                                            QValueList<int>& commonParentStartChildLocation, 
                                            QValueList<int>& commonParentEndChildLocation, 
                                            NodeSelection& cursorHolder, 
                                            Node* subTree, NodeModifsSet* modifs, bool extractInlineParentNodes = true);

	/**
	 * If n and n2 are both Text or Empty Nodes, merge them into one.
	 * WARNING if merging occurs, n2 is deleted.
	 * @param modifs The changes made are logged into modifs.
	 * @param mergeTextOnly Specify if we should only merge text Nodes, not empty ones.
	 * @return Returns true if the Nodes were merged, else false.
	 */
	static bool mergeNodes(Node *n, Node *n2, NodeModifsSet *modifs, bool mergeTextOnly = false);

    static bool mergeNodes(Node *n, Node *n2, NodeSelection& cursorHolder, NodeModifsSet *modifs, bool mergeTextOnly = false);

	/**
	 * This function will navigate through the Nodes from startNode to endNode and
	 * merge identical inline Nodes as well as text Nodes.
	 * @param startNode The node from which the merge starts.
	 * @param endNode The node from which the merge ends.
	 * @param modifs The usual modifs, to log the changes.
	 * <TEMPORARY> : We want to keep track of the cursor position. TODO : cursor class
	 * @param cursorNode The cursor is inside cursorNode.
	 * @param cursorOffset The offset of the cursor inside cursorNode.
	 * </TEMPORARY>
	 */
	static void mergeInlineNode(Node *startNode, Node *endNode, Node **cursorNode,
		long &cursorOffset, NodeModifsSet *modifs);


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
         * @param modifs The changes made are logged into modifs.
	 */
	static void setTagString(Node *node, const QString &newTagString, NodeModifsSet* modifs);

	/**
	 * This function behaves essentially like the above function except that all the others Nodes' position
	 * are updated too.
	 */
	static void setTagStringAndFitsNodes(Node *node, const QString &newTagString, NodeModifsSet* modifs);
        
    /**
     * This function behaves exactly like Node::editAttribute except that the change is logged inside a NodeModifsSet.
     */
    static void editNodeAttribute(Node* node, const QString& name, const QString& value, NodeModifsSet* modifs);

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
     * Similar to the above function but instead of using baseNode it uses the passes Node tree.
     * @param nodeTree Node tree where to get the location.
     */
    static Node* getNodeFromLocation(QValueList<int> loc, Node* nodeTree);

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

    static Node* getNodeFromSubLocation(QValueList<int> loc, int locOffset, Node* nodeTree);

	/**
	 * A enumeration for kafkaCommon::compareNodePosition().
	 */
	enum position
	{
		//It means that it is a previous sibling (not the dom/dom_node.h definition, but rather
		// the node.h definition)
		isBefore = 0,
		//It is the same Node.
		isAtTheSamePosition,
		//It means that it is a next sibling (in the node.h way).
		isAfter,
		//guess what?
		positionError
	};

	/**
	 * Compare the position of two Nodes.
	 * e.g. (pos1)->next = (pos2); compareNodePosition(n1, n2) == kafkaCommon::before.
	 * @param pos1 The location of the Node to compare.
	 * @param pos2 The location of the Node to be compared to.
	 * @return Return a kafkaCommon::position flag.
	 */
	static int compareNodePosition(QValueList<int> pos1, QValueList<int> pos2);

	/**
	 * It behave essentially like the above function except that it is based on Nodes.
	 */
	static int compareNodePosition(Node *n1, Node *n2);

	/**
	 * Compare n1 and n2's node type, node name, and node attributes.
	 * @return Returns true if there are indentical.
	 */
	static bool compareNodes(Node *n1, Node *n2);

	/**
	 * Get the node's depth in the tree.
	 * @param node The node we want the depth.
	 * @return Returns the depth of node. It is basically the number of parents of node.
	 * It will return 0 if node has no parent Nodes, and -1 if node doesn't exists.
	 */
	static int nodeDepth(Node *node);

	/**
	 * Looks if node has a parent which is named name.
	 * @return Returns the first parent which is named name or 0L if not found.
	 */
	static Node* hasParent(Node *node, const QString &name);

    /**
     * Tries to find the common parent to startNode and endNode, in the same conditions as above.
     */
    static Node* hasParent(Node* startNode, Node* endNode, const QString &name);

    
	/** ----------------- DOM::NODE TREE MODIFICATIONS --------------------*/

	/**
	 * Insert a DOM::Node in the DOM::Node tree. It takes care to handle the exceptions.
	 * WARNING : The postEnhancement is not done (cf htmlenhancer.h)
	 * Prefer using KafkaDocument::insertDomNode()
	 * @param node The node to insert.
	 * @param parent The new parent of node. If null, insert node at the top level.
	 * @param nextSibling The new next sibling of node. If null, append node at the end of the child list.
	 * @param rootNode The root DOM::Node of the DOM::Node tree. Useful when no parent is provided.
	 * @return Returns true if the operation was successfull.
	 */
	static bool insertDomNode(DOM::Node node, DOM::Node parent = DOM::Node(),
		DOM::Node nextSibling = DOM::Node(), DOM::Node rootNode = DOM::Node());


	/**
	 * Removes a DOM::Node from the DOM::Node Tree. It takes care to handle the exceptions.
	 * WARNING : The postUnenhancement is not done (cf htmlenhancer.h)
	 * Prefer using KafkaDocument::removeDomNode()
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
	static DOM::Node createDomNode(const QString &nodeName, const DTDStruct* dtd, DOM::Document rootNode);

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
	static DOM::Node createDomNodeAttribute(const QString &nodeName, const DTDStruct* dtd,
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
	static bool editDomNodeAttribute(DOM::Node node, const QString &nodeName, const DTDStruct* dtd,
		const QString &attrName, const QString &attrValue, DOM::Document rootNode);

	/**
	 * It behaves essentially like the above function.
	 * @param node The DOM::Node comes from this node.
	 */
	static bool editDomNodeAttribute(DOM::Node domNode, Node* node,
		const QString &attrName, const QString &attrValue, DOM::Document rootNode);
	
    /**
	 * Looks if domNode has a parent which is named name.
	 * @return Returns the first parent which is named name or an empty DOM::Node if not found.
	 */
	static DOM::Node hasParent(DOM::Node domNode, const QString &name);

	/**
	 * Returns the position of the child domNode.
	 * @param domNode This is the DOM::Node we want the position.
	 * @return Returns the position of domNode inside domNode's parent's children or -1 if not found.
	 */
	static int childPosition(DOM::Node domNode);

	/**
	 * Returns the position'th child of parentNode.
	 * @param parentNode The parent Node of the node to return.
	 * @param position We return the position'th child Node.
	 * @param fallback If set to true, it will always return a valid Node (except if there is no child!!)
	 */
	static DOM::Node getChildNode(DOM::Node parentNode, int position, bool fallback = false);

	/**
	 * Specify if a DOM::Node is inline (as specified in isInline()) or text.
	 * @return true if it is an inline Node.
	 */
	static bool isInline(DOM::Node domNode);

	/**
	 * Specify if parent supports the siblings DOM::Nodes starting from startNode to endNode
	 * according to the DTD dtd.
	 */
	static bool parentSupports(DOM::Node parent, DOM::Node startNode, DOM::Node endNode,
		const DTDStruct* dtd);
    

	/** ----------------------- MISCELLANEOUS -------------------------------------*/

	/**
	 * TEMPORARY, HTML specific
	 * @return Returns true if it is a inline Node. Official DTD List, unlike getNodeDisplay().
	 */
	 static bool isInline(const QString &nodename);

	/**
	 * Prints in stdout the current DOM::Node tree.
	 * @param rootNode The root Node of the DOM::Node Tree (usually document())
	 * @param indent The indentation.
	 */
	static void coutDomTree(DOM::Node rootNode, int indent);

	/**
	 * Prints in stdout the current Node tree.
	 * @param node The startNode
	 * @param indent The number of little dots per parent relationship.
	 */
	static void coutTree(Node *node, int indent);
 
    /**
     * Returns whether a range is surrounded by a tag.
     * @param start_node The start of the range to be checked.
     * @param end_node The end of the range to be checked.
     * @param tag_name The name of the tag, e.g., "strong".
     * @return  -1 is not inside tag_name
     *          1 is inside tag_name
     *          0 mixed 
     */
    static int isInsideTag(Node* start_node, Node* end_node, QString const& tag_name);
    
    static int isInsideTag(Node* start_node, Node* end_node, QString const& tag_name, 
                           QString const& attribute_name, QString const& attribute_value);
    
    /**
     * Return whether the offset is placed between two words in a text node.
     * @pre node is a Node of type text.
     * @pre offset >= 0
     * @param node The text node to be checked.
     * @param offset The position in text we want to see if it's between words.
     * @return true if is a space between words or if it's in the limit of a word.
     */
    static bool isBetweenWords(Node* node, int offset);

    /**
     * Set node and offset to the beggining of the word
     * @pre node is a text node.
     * @pre isBetweenWords
     * @param node The text node, which will be changed (or not) to the start of the word.
     * @param offset The current offset of the text node which will be changed (or not) to the start of the word.
     * @return the offset of the beggining of the word
     */
    static void getStartOfWord(Node*& node, int& offset);

    /**
     * Same as above, but will get the end of the word
     */
    static void getEndOfWord(Node*& node, int& offset);

    /**
     * Set node and offset to the beggining of the paragraph.
     * The distinction between inline/block nodes is used here.
     * @param node 
     * @param offset 
     */
    static void getStartOfParagraph(Node*& node, int& offset);
    
    static void getEndOfParagraph(Node*& node, int& offset);
    
private:
    /**
     * Split the last valid start parent (commonParentStartChild) into two.
     * This and the method above are related and are used in sequence.
     * The following tree:
     *  <body>
     *      <b>                     --> commonParent
     *          <i>                 --> commonParentStartChild
     *              select|
     *              here            --> startNode
     *          </i>
     *          continue
     *          <u>
     *          stop|more
     *          </u>
     *          text
     *      </b>
     *  <body>
     * Is changed to:
     *  <body>
     *      <b>
     *          <i>
     *              select|
     *          </i>
     *      </b>
     *      <b>
     *          <i>
     *              here
     *          </i>
     *          continue
     *          <u>
     *              stop|more
     *          </u>
     *          text
     *      </b>
     *  </body>
     * @param startNode The node where a selection starts, for example.
     * @param commonParent This is the common parent between start and end node.
     * @param commonParentStartChildLocation The first child of commonParent which is parent of startNode
     * @param modifs The changes made are logged into modifs.
     */
    static void splitStartNodeSubtree(Node* startNode, Node* commonParent, 
                                      QValueList<int>& commonParentStartChildLocation, NodeModifsSet* modifs);
    /**
     * Split the last valid start parent (commonParentStartChild) into two.
     * The following tree:
     *  <body>
     *      <b>                     --> commonParent
     *          <i>                 --> commonParentStartChild
     *              select|
     *              here     
     *          </i>
     *          continue
     *          <u>                 --> commonParentEndChild
     *              stop|           --> endNode
     *              more
     *          </u>
     *          text
     *      </b>
     *  <body>
     * Is changed to:
     *  <body>
     *      <b>
     *          <i>
     *              select|here
     *          </i>
     *          continue
     *          <u>
     *              stop|
     *          </u>
     *      </b>
     *      <b>
     *          <u>
     *              more
     *          </u>
     *          text
     *      </b>
     *  </body>
     * @param endNode The node where a selection ends, for example.
     * @param commonParent This is the common parent between start and end node.
     * @param commonParentStartChildLocation The first child of commonParent which is parent of startNode.
     * @param commonParentEndChildLocation The first child of commonParent which is parent of endNode.
     * @param subTree True if we are dealing with a tree that doesn't belong to the current document.
     * @param modifs The changes made are logged into modifs.
     */
    static void splitEndNodeSubtree(Node* endNode, Node* commonParent, 
                                    QValueList<int>& commonParentStartChildLocation, 
                                    QValueList<int>& commonParentEndChildLocation, 
                                    bool subTree, NodeModifsSet* modifs);
};

#endif
