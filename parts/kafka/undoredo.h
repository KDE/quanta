/***************************************************************************
                               undoredo.h
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

#ifndef UNDOREDO_H
#define UNDOREDO_H

#include <qvaluelist.h>
#include <qptrlist.h>
#include <qobject.h>
#include "node.h"

class Document;

/**
 * The basic unit of the undo/redo system : a Node modification.
 */
class NodeModif
{
public:
	NodeModif();
	~NodeModif();

	/**
	 * For all NodeModifs : Type of the Node modification : Added, removed, modified, moved,...
	 * cf the NodeModification enumeration.
	 * WARNING The type MUST be set first!!
	 * @param type The type of the NodeModif, as described in the NodeModification enumeration.
	 */
	void setType(int type){m_type = type;}

	/**
	 * Returns the current type of the NodeModif.
	 */
	int type() {return m_type;}

	/**
	 * For all type : Location of the Node added/modified/removed/moved.
	 * @param location The location of the Node, cf kafkaCommon::getNodeLocation()
	 */
	void setLocation(QValueList<int> location) {m_location = location;}

	/**
	 * Returns the location of the Node which have been modified.
	 */
	QValueList<int> location() {return m_location;}

	/**
	 * For Node move : Final location of the Node moved.
	 * @param location The final location of the Node moved, cf kafkaCommon::getNodeLocation()
	 */
	void setFinalLocation(QValueList<int> location) {m_finalLocation = location;}

	/**
	 * Returns the final location of the Node which was moved.
	 */
	QValueList<int> finalLocation() {return m_finalLocation;}

	/**
	 * For Node deletion: Store the deleted Node.
	 * @param node The deleted node.
	 */
	void setNode(Node *node);

	/**
	 * Returns the deleted Node.
	 */
	Node *node() {return m_node;}

	/**
	 * For Node modification : Store the old tag
	 * @param tag The original tag.
	 */
	void setTag(Tag *tag);

	/**
	 * Returns the original Tag.
	 */
	Tag *tag() {return m_tag;}

	/**
	 * For non-XmlEnd Node deletion without its children.
	 * @param childrenNumber The number of children which are moved up
	 * at the location where was the deleted Node.
	 */
	void setChildrenMovedUp(int childrenNumber) {m_childrenMovedUp = childrenNumber;}

	/**
	 * Returns the number of childs which were moved up.
	 */
	int childrenMovedUp() {return m_childrenMovedUp;}

	/**
	 * For XmlEnd Node deletion : number of  moved down
	 * @param number The number of right neighbours which are moved down.
	 */
	void setNeighboursMovedDown(int number) {m_neighboursMovedDown = number;}

	/**
	 * Returns the number of right neighbours which were moved down.
	 */
	int neighboursMovedDown() {return m_neighboursMovedDown;}

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
		// WARNING : node movement is only node-based, e.g. you can't use it for Drag'n'Drop
		//use NodeRemoved and NodeAdded instead
		//WARNING : NodeMoved not implemented, because it has no use for the moment.
		NodeMoved,
		//WARNING : NodeAndChildsMoved not implemented for the same reason.
		NodeAndChildsMoved
	};

	private:
	int m_type;
	QValueList<int> m_location, m_finalLocation;
	Node *m_node;
	Tag *m_tag;
	int m_childrenMovedUp;
	int m_neighboursMovedDown;
};

/**
 * A NodeModifsSet contains all the Node modifications made by one user input.
 */
class NodeModifsSet
{
public:
	NodeModifsSet();
	~NodeModifsSet();

	/**
	 * Add a new NodeModif to the list of NodeModifs.
	 */
	void addNodeModif(NodeModif *nodeModif) {m_nodeModifList.append(nodeModif);}

	/**
	 * Returns the list of NodeModifs.
	 */
	QPtrList<NodeModif> nodeModifList() {return m_nodeModifList;}

	/**
	 * Set the Modified flag AFTER the user input.
	 */
	void setIsModified(bool isModified) {m_isModified = isModified;}

	/**
	 * Returns the Modified flag.
	 */
	bool isModified(){return m_isModified;}

	/**
	 * Set a description to the user input.
	 */
	void setDescription(const QString &description) {m_description = description;}

	/**
	 * Returns the description of the user input.
	 */
	QString description() {return m_description;}

private:
	QPtrList<NodeModif> m_nodeModifList;
	bool m_isModified;
	QString m_description;

	/**TODO:see later for a common cursor position. */
	uint cursorX;
	uint cursorY;
	/** The position of the cursor after the user input */
	uint cursorX2;
	uint cursorY2;

};

/**
 * This class, basically a new undo/redo system, also helps KafkaDocument to synchronize the
 * kafka and quanta view.
 */
class undoRedo : public QObject
{
public:
	/**
	 * Creates a document specific undo/redo system.
	 * @param doc The Document the undo/redo system is linked to.
	 */
	undoRedo(Document *doc);

	~undoRedo();

	/** The enumeration of all possible location where modifications can occur */
	enum modificationLocation {
		// A modification was made in the source view (kate).
		SourceModif = 0,
		// A modification was made directly in the node Tree.
		NodeTreeModif,
		// A modification was made in the VPL view (kafka).
		KafkaModif
	};

	/**
	 * Adds a new set of Node modification. This should be called whenever
	 * the kafka/quanta editor is modified.
	 * @param modifs The new modification set to add to the undo/redo stack.
	 * @param modifLocation Specifies where the modification was made
	 * cf undoRedo::modificationLocation.
	 */
	void addNewModifsSet(NodeModifsSet *modifs, int modifLocation);

	/**
	 * Ignores the ModifSet that will come in the number'th position. Useful when
	 * KTextEditor::EditInterface::insertText() is called before parser::rebuild() and
	 * thus parser::rebuild will be called two times.
	 * @param number Specifies the position of the ModifsSet to ignore.
	 */
	 void dontAddModifsSet(int number) {m_dontAddModifSet = number;}

	/**
	 * Merges the next ModifsSet with the previous one. Useful when autocompletion
	 * makes parser::rebuild() to be called again.
	 */
	 void mergeNextModifsSet() {m_mergeNext = true;}

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
	  * Synchronize the cursor position and the selection of the kafka view by translating
	  * the cursor position and selection of the quanta view.
	  */
	void syncKafkaCursorAndSelection();

	 /**
	  * Synchronize the cursor position and the selection of the quanta view by translating
	  * the cursor position and selection of the kafka view.
	  */
	void syncQuantaCursorAndSelection();

        /**
	 * Reload kafka from the current document.
	 * @param force If set to true, it will reload kafka even if it is already up to date.
	 * @param syncKafkaCursor If set to true, it will try to sync the kafka cursor from the
	 * quanta one.
	 */
	void reloadKafkaEditor(bool force = false, bool syncKafkaCursor = true);

	/**
	 * Reload the quanta editor from kafka.
	 * @param force If set to true, it will reload quanta even if it is already up to date.
	 * @param syncQuantaCursor If set to true, it will try to sync the quanta cursor from
	 * the kafka one.
         * @param encodeText Usually when a Text Node has the dirty flag (cleanStrBuilt), it means that
         * it was modified in VPL and thus it contains some unencoded text : we have to encode it. (e.g.
         * whitespace => &nbsp;) But some functions calling this function might not want that because
         * the text is already encoded e.g. codeFormatting().
	 */
	void reloadQuantaEditor(bool force = false, bool syncQuantaCursor = true, bool encodeText = true);
        
        /**
         * Format the code of the document. It simply call reloadQuantaEditor() after having
         * set the dirty flag to every Node.
         */
        void codeFormatting();

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
	/**
	 * The main undoRedo list which contains the NodeModifsSet.
	 */
	QPtrList<NodeModifsSet> m_undoList;
	/**
	 * The undoRedo list iterators which point the current location of each component in
	 * the undoRedo list.
	 * documentIterator point the current location of the Node Tree.
	 * sourceIterator point the current location of the source view (kate).
	 * kafkaIterator point the current location of the VPL view (kafka).
	 */
	QPtrListIterator<NodeModifsSet> documentIterator;
	QPtrListIterator<NodeModifsSet> sourceIterator;
	QPtrListIterator<NodeModifsSet> kafkaIterator;
	int m_listLimit;
	bool m_merging;
	bool addingText;
	QValueList<int> m_currentLoc;
	Document *m_doc;
	bool m_mergeNext;
	int m_dontAddModifSet;
};

#endif
