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
#include <qobject.h>
#include "../../parser/node.h"

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
	/** The state of the document after the user input */
	bool isModified;
	/** The description of the user input. For a future Undo history */
	QString description;
};

/**
 * This class, basically a new undo/redo system, also help WKafkaPart to synchronize the
 * kafka and quanta view.
 */
class undoRedo : public QValueList<NodeModifsSet>, public QObject
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
	 * @param kafkaModifSet Specifies if the ModifSet comes from kafka.
	 */
	void addNewModifsSet(NodeModifsSet modifs, bool kafkaModifSet);

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
	 * Synchronize the kafka view with the quanta view by applying the NodeModifs
	 * which have occured since the last synchronization.
	 * @return Returns if the synchronization was successful.
	 */
	 bool syncKafkaView();

	/**
	 * Synchronize the quanta view with the kafka view by applying the NodeModifs
	 * which have occured since the last synchronization.
	 * @return Returns if the synchronization was successful.
	 */
	 bool syncQuantaView();

        /**
	 * Reload kafka from the current document.
	 * @param force If set to true, it will reload kafka even if it is already up to date.
	 */
	 void reloadKafkaEditor(bool force = false);

	/**
	 * Reload the quanta editor from kafka.
	 * @param force If set to true, it will reload quanta even if it is already up to date.
	 */
	void reloadQuantaEditor(bool force = false);		 
	 
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

public slots:

	/**
	 * Called by quantaApp whenever the current file is saved. The isModified
	 * flag of each NodeModisSet is updated.
	 */
	void fileSaved();

	/**
	 * Called when the kafkaPart is loaded.
	 */
	 void kafkaLoaded();

private:

	/**
	 * This is one of the main functions which apply the changes needed to undo a nodeModif
	 * in the text and in the Node tree.
	 * @param _nodeModif The nodeModif to undo.
	 * @param undoTextModifs Specifies if the undo operation should undo the text modifications.
	 * @param generateText Specifies if the text of the Tags should be generated.
	 * @return Returns true if the undo has correctly worked.
	 */
	bool UndoNodeModif(NodeModif &_nodeModif, bool undoTextModifs = true, bool generateText = false);

	/**
	 * Convenient function which call UndoNodeModif,
	 * while changing the type of the NodeModifs to make them redo.
	 */
	bool RedoNodeModif(NodeModif &_nodeModif, bool undoTextModifs = true, bool generateText = false);

	/**
	 * This is one of the main functions which apply the changes needed to undo a nodeModif
	 * in the kafka tree.
	 * @param _nodeModif The nodeModif to undo.
	 * @return Returns true if the undo has correctly worked.
	 */
	bool UndoNodeModifInKafka(NodeModif &_nodeModif);

	/**
	 * Convenient function which call UndoNodeModifInKafka,
	 * while changing the type of the NodeModifs to make them redo.
	 */
	bool RedoNodeModifInKafka(NodeModif &_nodeModif);

	/**
	 * Prints in stdout a debugging flow.
	 */
	void debugOutput();

private:
	int _listLimit;
	bool _merging;
	bool addingText;
	QValueList<int> _currentLoc;
	QValueList<NodeModifsSet>::iterator editorIterator;
	QValueList<NodeModifsSet>::iterator kafkaIterator;
	Document *_doc;
	bool _mergeNext;
	int _dontAddModifSet;
};

#endif
