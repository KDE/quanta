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
class NodeSelection;
class NodeSelectionInd;

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
   * @return Returns the current type of the NodeModif.
   */
  int type() {return m_type;}

  /**
   * For all type : Location of the Node added/modified/removed/moved.
   * @param location The location of the Node, cf kafkaCommon::getNodeLocation()
   */
  void setLocation(QValueList<int> location) {m_location = location;}

  /**
   * @return Returns the location of the Node which have been modified.
   */
  QValueList<int>& location() {return m_location;}

  /**
   * For Node move : Final location of the Node moved.
   * @param location The final location of the Node moved, cf kafkaCommon::getNodeLocation()
   */
  void setFinalLocation(QValueList<int> location) {m_finalLocation = location;}

  /**
   * @return Returns the final location of the Node which was moved.
   */
  QValueList<int>& finalLocation() {return m_finalLocation;}

  /**
   * For Node deletion: Store the deleted Node.
   * @param node The deleted node.
   */
  void setNode(Node *node);

  /**
   * @return Returns the deleted Node.
   */
  Node *node() {return m_node;}

  /**
   * For Node modification : Store the old tag
   * @param tag The original tag.
   */
  void setTag(Tag *tag);

  /**
   * @return Returns the original Tag.
   */
  Tag *tag() {return m_tag;}

  /**
          * TODO:REMOVE
   * For non-XmlEnd Node deletion without its children.
   * @param childrenNumber The number of children which are moved up
   * at the location where was the deleted Node.
   */
  void setChildrenMovedUp(int childrenNumber) {m_childrenMovedUp = childrenNumber;}

  /**
          * TODO:REMOVE
   * @return Returns the number of childs which were moved up.
   */
  int childrenMovedUp() {return m_childrenMovedUp;}

  /**
          * TODO:REMOVE
   * For XmlEnd Node deletion : number of  moved down
   * @param number The number of right neighbours which are moved down.
   */
  void setNeighboursMovedDown(int number) {m_neighboursMovedDown = number;}

  /**
          * TODO:REMOVE
   * @return Returns the number of right neighbours which were moved down.
   */
  int neighboursMovedDown() {return m_neighboursMovedDown;}

  /** All the possible Node modifications */
  enum NodeModification {
    //A complete Node Tree is added. Implemented.
    NodeTreeAdded = 0,
    //A Node and its childs are added. Implemented.
    NodeAndChildsAdded,
    //A Node is added. Implemented.
    NodeAdded,
    //WARNING : do not use this if the node type or the node name change.
    //A Node is modified. Implemented.
    NodeModified,
    //A Node is removed. Implemented.
    NodeRemoved,
    //A Node and its childs are removed. Implemented.
    NodeAndChildsRemoved,
    //The complete Node tree is removed. Implemented.
    NodeTreeRemoved,
    //Moving a Node from one location to another. Implemented.
    NodeMoved,
    //Moving a Node and its children from one location to another.
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
 * A NodeModifsSet contains all the Node modifications made by one user input, and the 
 * cursor and selection location before and after the user input.
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
  QPtrList<NodeModif> & nodeModifList() {return m_nodeModifList;}

  /**
   * Set the Modified flag BEFORE the user input.
   */
  void setIsModifiedBefore(bool isModifiedBefore) {m_isModifiedBefore = isModifiedBefore;}

  /**
   * @return Returns the Modified flag BEFORE the user input.
   */
  bool isModifiedBefore(){return m_isModifiedBefore;}

  /**
   * Set the Modified flag AFTER the user input.
   */
  void setIsModifiedAfter(bool isModifiedAfter) {m_isModifiedAfter = isModifiedAfter;}

  /**
   * @return Returns the Modified flag AFTER the user input.
   */
  bool isModifiedAfter(){return m_isModifiedAfter;}

  /**
   * Set a description to the user input.
   */
  void setDescription(const QString &description) {m_description = description;}

  /**
   * @return Returns the description of the user input.
   */
  QString description() {return m_description;}

  /**
   * Set the coordinates of the selection before the user input.
   */
  void setSelectionBefore(NodeSelectionInd* selectionBefore) {m_selectionBefore = selectionBefore;}
    
  /**
   * @return Return the selection before the Node modifications.
   */
  NodeSelectionInd *selectionBefore() {return m_selectionBefore;}
  
  /**
   * Set the coordinates of the selection after the user input.
   */
  void setSelectionAfter(NodeSelectionInd* selectionAfter) {m_selectionAfter = selectionAfter;}

  /**
   * @return Return the selection after the Node modifications.
   */
  NodeSelectionInd *selectionAfter() {return m_selectionAfter;}
  
  /**
   * We can separate the NodeModif into two categories ; the Node modifs made by the user
   * and then the indentation.
   * Call this function when we have finished with the first category.
   */
  void startOfIndentation() {m_indentationStartOffset = m_nodeModifList.count();}
  
  /**
   * @return Return the offset (starting from 0) where the indentation NodeModifs begin in the list of NodeModif.
   * Return -1 if it hasn't begin to add the indentation NodeModifs.
   */
  int indentationStartOffset() {return m_indentationStartOffset;}
  

private:
  QPtrList<NodeModif> m_nodeModifList;
  bool m_isModifiedBefore, m_isModifiedAfter;
  QString m_description;
  NodeSelectionInd *m_selectionBefore, *m_selectionAfter;
  int m_indentationStartOffset, m_type;
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
   * @param selection If given, and if modifLocation == SourceModif, it will synchronize the cursor
   * and the selection to 'selection'. Only work when the focus is in VPL for the moment.
   * cf undoRedo::modificationLocation.
   */
  void addNewModifsSet(NodeModifsSet *modifs, int modifLocation, NodeSelection *selection = 0L, bool encodeText = true);

  /**
   * TEMPORARY function.
   * First we will only enable undoRedo in VPL : this class will only log changes
   * made in VPL.
   * This function specify if we should log the changes submitted to addNewModifsSet or not.
   * @param True => enable, false => disable
   */
  void turnOn(bool on);

  /**
   * TEMPORARY function.
   * @return Returns true if the changes are logged.
   */
  bool turnedOn() {return m_loggingEnabled;}

  /**
          * TODO:REMOVE
   * Ignores the ModifSet that will come in the number'th position. Useful when
   * KTextEditor::EditInterface::insertText() is called before parser::rebuild() and
   * thus parser::rebuild will be called two times.
   * @param number Specifies the position of the ModifsSet to ignore.
   */
  void dontAddModifsSet(int number) {m_dontAddModifSet = number;}

  /**
          * TODO:REMOVE
   * Merges the next ModifsSet with the previous one. Useful when autocompletion
   * makes parser::rebuild() to be called again.
   */
  void mergeNextModifsSet() {m_mergeNext = true;}

  /**
   * Makes the undo operation.
   * @param kafkaUndo Specifies if the undo operation is done in the kafka view.
   * @return Returns true if a previous undo operation is available.
   */
  bool undo();

  /**
   * Makes the redo operation.
   * @param kafkaUndo Specifies if the undo operation is done in the kafka view.
   * @return Returns true if a next redo operation is available.
   */
  bool redo();

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
   * @param selection Synchronize the selection and the cursor position to "selection".
   */
  void syncKafkaCursorAndSelection(NodeSelection *selection);

  /**
   * Synchronize the cursor position and the selection of the quanta view by translating
   * the cursor position and selection of the kafka view.
   */
  void syncQuantaCursorAndSelection();

  /**
  * Reload kafka from the current document.
  * @param force If set to true, it will reload kafka even if it is already up to date.
  * @param selection If given, it will synchronize the selection and the cursor position to 'selection'.
  */
  void reloadKafkaEditor(bool force = false, NodeSelection *selection = 0L);

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
   * @param nodeModif The nodeModif to undo.
   * @return Returns true if the undo has correctly worked.
   */
  bool undoNodeModif(NodeModif *nodeModif);

  /**
   * Convenient function which call undoNodeModif,
   * while changing the type of the NodeModifs to make them redo.
   */
  bool redoNodeModif(NodeModif *nodeModif);

  /**
   * This is one of the main functions which apply the changes needed to undo a nodeModif
   * in the kafka tree.
   * @param _nodeModif The nodeModif to undo.
   * @return Returns true if the undo has correctly worked.
   */
  bool undoNodeModifInKafka(NodeModif *nodeModif);

  /**
   * Convenient function which call undoNodeModifInKafka,
   * while changing the type of the NodeModifs to make them redo.
   */
  bool redoNodeModifInKafka(NodeModif *nodeModif);

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
  bool m_loggingEnabled;
};

#endif
