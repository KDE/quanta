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
	/** For Node deletion: Store the deleted Node. */
	Node *node;
	/** For Node modification : Store the old tag */
	Tag *tag;
	/** For non-XmlEnd Node deletion without its childs : number of childs which are moved up*/
	int childsNumber;
	/** For XmlEnd Node deletion : number of right neighbours moved down*/
	int childsNumber2;
};

/**
 * A NodeModifsSet contains all the Node modifications made by one user input.
 */
typedef struct NodeModifsSet
{
	/** The list of Node Modification resulting of one user input. */
	QValueList<NodeModif> NodeModifList;
	/** The position of the cursor before the user input. */
	uint cursorX;
	uint cursorY;
	/** The position of the cursor after the user input */
	uint cursorX2;
	uint cursorY2;
};

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
	 * Ignores the ModifSet that will come in the number'th position. Useful when
	 * KTextEditor::EditInterface::insertText() is called before parser::rebuild() and
	 * thus parser::rebuild will be called two times.
	 * @param number Specifies the position of the ModifsSet to ignore.
	 */
	 void dontAddModifsSet(int number) {_dontAddModifSet = number;}

	/**
	 * Merges the next ModifsSet with the previous one. Useful when autocompletion
	 * makes parser::rebuild() to be called again.
	 */
	 void mergeNextModifsSet() {_mergeNext = true;}

	/**
	 * Makes the undo operation.
	 * @param kafkaUndo Specifies if the undo operation is done in the kafka view.
	 * @return Returns true if a previous undo operation is available.
	 */
	bool undo(bool kafkaUndo);

	/**
	 * Makes the redo operation.
	 * @param kafkaUndo Specifies if the undo operation is done in the kafka view.
	 * @return Returns true if a next redo operation is available.
	 */
	bool redo(bool kafkaUndo);

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
		//A complete Node Tree is added. Implemented.
		NodeTreeAdded = 0,
		//WARNING : not tested yet.
		//A Node and its childs are added. Implemented.
		NodeAndChildsAdded,
		//A Node is added. Implemented.
		NodeAdded,
		//WARNING : do not use this if the node type or the node name change.
		//A Node is modified. Implemented.
		NodeModified,
		//a Node is removed. Implemented.
		NodeRemoved,
		//WARNING : not tested yet.
		//A Node and its childs are removed. Implemented.
		NodeAndChildsRemoved,
		//The complete Node tree is removed. Implemented.
		NodeTreeRemoved,
		/** WARNING : node movement is only node-based, e.g. you can't use it for Drag'n'Drop
		use NodeRemoved and NodeAdded instead */
		//WARNING : NodeMoved not implemented, because it has no use for the moment.
		NodeMoved,
		//WARNING : NodeAndChildsMoved not implemented for the same reason.
		NodeAndChildsMoved
	};

private:

	/**
	 * This is the main function which apply the changes needed to undo a nodeModif.
	 * @param _nodeModif The nodeModif to undo.
	 * @param Specifies if the undo operation is done in the kafka view. This implies some
	 * additionnal tasks like tag String generation.
	 * @return Returns true if the undo has correctly worked.
	 */
	bool UndoNodeModif(NodeModif &_nodeModif, bool kafkaUndo);

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
	bool _mergeNext;
	int _dontAddModifSet;
};

#endif
