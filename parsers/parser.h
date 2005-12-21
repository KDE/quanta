/***************************************************************************
                          parser.h  -  description
                             -------------------
    begin                : Sun Sep 1 2002
    copyright            : (C) 2002, 2003, 2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include <qobject.h>
#include <qdict.h>
#include <qstringlist.h>
#include <qmap.h>
#include <qguardedptr.h>

#include <qvaluestack.h>

#include "node.h"
#include "tag.h"
#include "qtag.h"

/**
  *@author Andras Mantia
  */

class Document;
class KDirWatch;
class QRegExp;
class NodeModifsSet;
class SAParser;

typedef QMap<QString, GroupElementMapList> IncludedGroupElements;
typedef QMap<QString, IncludedGroupElements> IncludedGroupElementsMap;


class Parser: public QObject {

Q_OBJECT

public:
  Parser();
  ~Parser();

  /** Parse a string, using as start position sLine, sCol. */
  Node *parseArea(int startLine, int startCol, int endLine, int endCol, Node **lastNode, Node *a_node = 0L);

  /** Parse the whole text from Document w and build the internal structure tree
  from Nodes. Set force to true if you want to avoid the possible checks. */
  Node *parse(Document *w, bool force = false);

  /** Returns the node for position (line, column). As more than one node can
  contain the same area, it return the "deepest" node. */
  Node *nodeAt(int line, int col, bool findDeepest = true, bool exact = false);

  /** Rebuild the nodes */
  Node *rebuild(Document *w);
  /** No descriptions */
  const DTDStruct * currentDTD(int line, int col);
  /** Remove the found groups from the memeber variables */
  void clearGroups();
  void parseIncludedFiles();

  /** Enable/Disable parsing. */
  void setSAParserEnabled(bool enabled);
  void setParsingEnabled(bool enabled) {m_parsingEnabled = enabled;}
  bool isParsingEnabled() {return m_parsingEnabled;}
  void setParsingNeeded(bool needed) {m_parsingNeeded = needed;}
  bool parsingNeeded() {return m_parsingNeeded;}
  /**
   * This function is ESSENTIAL : when one modify baseNode, one MUST use
   * this function to set the internal parser RootNode pointer to the same Node as
   * baseNode. If one forget, some strange sigserv errors concerning inexisting tags
   * (Node->tag == 0) will occurs.
   * Crash errors of Parser::nodeAt is a good sign of a missing setRootNode
   */
  void setRootNode(Node* node) {m_node = node;} //TODO: check if m_saParser should be updated or not!
  void synchParseInDetail();

  IncludedGroupElementsMap includedMap;
  bool parsingEnabled;
  bool m_treeReparsed;

public slots:
  void slotParseInDetail();
 /** Remove the found groups from the memeber variables */
  void cleanGroups();

private slots:
  void slotIncludedFileChanged(const QString& fileName);

signals:
  void nodeTreeChanged();
  void rebuildStructureTree(bool);

private:
  Node* m_node;       //the internal Node pointer
  QString m_dtdName;  //the DTD name of write
  const DTDStruct* m_dtd; //the dtd used for main parsing
  QGuardedPtr<Document> write;    //pointer to the parsed document
  int maxLines; // how many lines are in the current document
  int oldMaxLines;
  int treeSize;
  QMap<QString, XMLStructGroup>::ConstIterator xmlGroupIt;
  bool m_parsingEnabled;
  bool m_parsingNeeded;

  /** Clears the group elements found in the included files */
  void clearIncludedGroupElements();
  void parseIncludedFile(const QString &fileName, const DTDStruct *dtd);
  /** Searches for scripts inside the text from startNode. It looks only for the
  script begin/and delimiters, and not for the <script> or other special tags.
  Useful when parsing for script inside the xml tags.
  Returns: true if a script area is found, false if the parsed text does not
  contain any scripts. */
  bool parseScriptInsideTag(Node *startNode);

  /** Parses the node for XML groups (specific tags)*/
  void parseForXMLGroup(Node *node);
  /** Determines the area that should be reparsed.
      w: the document we are working on
      area: the invalid areas
      firstNode: the first unchanged node before the current position
      lastNode: the first unchanged node after the current position
      Returns: true if an area was found, false otherwise => require full parsing
   */
  bool invalidArea(Document *w, AreaStruct &area, Node **firstNode, Node **lastNode);

  /** Deletes all the nodes between the firstNode and lastNode and keeps the tree's consistency.
      modifs is the class recording these changes for the undo/redo system, cf undoredo.h */
  void deleteNodes(Node *firstNode, Node *lastNode, NodeModifsSet *modifs);

  /**
   * This function must be called before reparsing : it log in the undo/redo system
   * that the whole Node tree is reloaded.
   * @param modifs This class record all the changes made.
   * @param w modifs will be inserted in w's undoredo list.
   */
  void logReparse(NodeModifsSet *modifs, Document *w);


  SAParser *m_saParser; //the special area parser object

  /** Maybe we should move to a separate, special area parsing class */
  Node* specialAreaParsingDone(int &lastLine, int &lastCol);

  bool m_parseIncludedFiles;
};



#endif
