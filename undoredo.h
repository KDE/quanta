/***************************************************************************
                               undoredo.h
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

#ifndef UNDOREDO_H
#define UNDOREDO_H

#include <qvaluelist.h>
#include "parser/node.h"

class Document;

/**
 * The basic unit of the undo/redo system : a Node modification.
 */
typedef struct NodeModif
{
	/** For all : Type of the Node modification : Added, removed, modified, moved,...
	with or without the Node's childs */
	int type;
	/** For all: Location of the Node added/modified/removed/moved */
	QValueList<int> location;
	/** For Node move : Initial location of the Node moved */
	QValueList<int> location2;
	/** For Node modification : Store the old Node. */
	Node *node;
	/** For Node modification : Store the old tag */
	Tag *tag;
	/** For Node removal without its childs : number of childs */
	int childsNumber;
};

/**
 * A NodeModifsSet contains all the Node modifications made by one user input.
 */
typedef QValueList<NodeModif> NodeModifsSet;

/**
 * A new Node-based undo/redo system.
 */
class undoRedo : public QValueList<NodeModifsSet>
{
public:
	/**
	 * Creates a document specific undo/redo system.
	 * @param doc The Document the undo/redo system is linked to.
	 */
	undoRedo(Document *doc);

	~undoRedo();

	/**
	 * Adds a new set of Node modification. This should be called whenever
	 * the kafka/quanta editor is modified.
	 * @param modifs The new modification set to add to the undo/redo stack.
	 */
	void addNewModifsSet(NodeModifsSet modifs);

	/**
	 * Makes the undo operation on the Quanta editor view.
	 * @return Returns true if a previous undo operation is available.
	 */
	bool undo();

	/**
	 * Makes the redo operation on the Quanta editor view.
	 * @return Returns true if a next redo operation is available.
	 */
	bool redo();

	/**
	  * Gets the location of a Node in a pointer-independant suit of ints e.g. 1,3,5 means
	  * that the node is the fifth child of the third child of the root Node. Efficient when
	  * deleting the Node tree and rebuilding it when switching between Documents.
	  * @param _node The Node we want the location.
	  * @return Returns the location.
	 */
	QValueList<int> getLocation(Node * _node);

	/**
	 * See the above function.
	 * @param loc We want the Node from this location.
	 * @return Returns the Node at location loc.
	 */
	Node * getNodeFromLocation(QValueList<int> loc);

	/** All the possible Node modifications */
	enum NodeModification {
		//A complete Node Tree is added.
		NodeTreeAdded = 0,
		NodeAndChildsAdded,
		NodeAdded,
		NodeModified,
		NodeRemoved,
		NodeAndChildsRemoved,
		//The complete Node tree is removed.
		NodeTreeRemoved,
		/** WARNING : node movement is only node-based, e.g. you can't use it for Drag'n'Drop
		use NodeRemoved and NodeAdded instead */
		//WARNING : NodeMoved not implemented, because it has no use for the moment.
		NodeMoved,
		//WARNING : NodeAndChildsMoved not implemented for the same reason.
		NodeAndChildsMoved,
		//A NodeSubtreeMoved is a move of a subtree composed of one Node and all its childs and all its right neighbours.
		NodeSubtreeMoved
	};

private:

	/**
	 * Fits the Nodes position when a change occurs in the kafka tree.
	 * @param _startNode The Node where the update starts
	 * @param colMovement The number of column that should be
	 * added/retrieved from the column position
	 * @param lineMovement The number of lines that should be
	 * added/retrieved from the line position
	 * @param colEnd The column position where the update should stop
	 * @param lineEnd The line position where the update should stop
	 */
	void fitsNodesPosition(Node* _startNode, int colMovement,
		int lineMovement = 0, int colEnd = -2, int lineEnd = -2);

	/**
	 * This function returns the next Node after _node.
	 * @param _node It is the Node from which we want the next Node.
	 * @param goingTowardsRootNode This boolean specifies if we should go up or down in the tree.
	 * For a normal use, It must be set to false at the
	 * beginning and then the same boolean must be used when using
	 * several times this function.
	 * @param endNode Specifies at which Node the search should end.
	 * @return Returns the next Node.
	 */
	Node *getNextNode(Node *_node, bool &goingTowardsRootNode, Node *endNode = 0L);

	/**
	 * Prints in stdout a debugging flow.
	 */
	void debugOutput();

	/**
	 * Prints in stdout the current Node tree
	 */
	void coutTree(Node *node, int indent);

private:
	int _listLimit;
	QValueList<NodeModifsSet>::iterator editorIterator;
	QValueList<NodeModifsSet>::iterator kafkaIterator;
	Document *_doc;
};

#endif
