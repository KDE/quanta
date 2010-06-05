/***************************************************************************
                          parser.h  -  description
                             -------------------
    begin                : Sun Sep 1 2002
    copyright            : (C) 2002, 2003 by Andras Mantia <amantia@kde.org>
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
  from Nodes */
  Node *parse(Document *w);

  /** Returns the node for position (line, column). As more than one node can
  contain the same area, it return the "deepest" node. */
  Node *nodeAt(int line, int col, bool findDeepest = true);

  /** Rebuild the nodes */
  Node *rebuild(Document *w);
  /** No descriptions */
  DTDStruct * currentDTD(int line, int col);
  /** Remove the found groups from the memeber variables */
  void clearGroups();
  void parseIncludedFiles();
  void removeCommentsAndQuotes(QString& str, DTDStruct* dtd);

  /** Enable/Disable parsing. */
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
  void setRootNode(Node* node) {m_node = node;}
  /** Print the doc structure tree to the standard output.
      Only for debugging purposes. */
  void coutTree(Node *node, int indent);

  GroupElementMapList m_groups; //a list of groups (variables, inclusions)
  QStringList includedFiles;
  QPtrList<DTDStruct> includedFilesDTD;
  IncludedGroupElementsMap includedMap;
  bool parsingEnabled;
  int nodeNum;
  QStringList selectors; //holds the CSS selectors

private slots:
  void slotIncludedFileChanged(const QString& fileName);
  
signals:
  void nodeTreeChanged();  

private:
  Node* m_node;       //the internal Node pointer
  QString m_dtdName;  //the DTD name of write
  DTDStruct *m_dtd; //the dtd used for main parsing
  QGuardedPtr<Document> write;    //pointer to the parsed document
  int maxLines; // how many lines are in the current document
  int oldMaxLines;
  int treeSize;
  KDirWatch *includeWatch;
  QMap<QString, XMLStructGroup>::ConstIterator xmlGroupIt;
  QRegExp m_quotesRx;
  bool m_parsingEnabled;
  bool m_parsingNeeded;

  void parseIncludedFile(const QString &fileName, DTDStruct *dtd);
  /** Searches for scripts inside the text from startNode. It looks only for the
  script begin/and delimiters, and not for the <script> or other special tags.
  Useful when parsing for script inside the xml tags.
  Returns: true if a script area is found, false if the parsed text does not
  contain any scripts. */
  bool parseScriptInsideTag(Node *startNode);

/*
  Parses the document for special areas (eg. scripts).
  specialArea: the area (start/end position) in the document that may contain the special
               area. It may end before the end position.
  areaStartString: the special area starting string
  forcedAreaEndString: force this as the special area ending string.
  parentNode: the Node under where the special area goes
  lastLine: will contain the line where the special area ends
  lastCol: will contain the column where the special area ends
*/
  Node* parseSpecialArea(const AreaStruct &specialArea,
                         const QString &areaStartString,
                         const QString &forcedAreaEndString,
                         Node *parentNode,
                         int& lastLine, int& lastCol);
  QString getLine(int line, int endLine, int endCol);
  /** Appends a text area to a text node. */
  void appendAreaToTextNode(const AreaStruct &area, Node *node);
  /** Creates a text/empty node between node and the provided position */
  Node* createTextNode(Node *node, int eLine, int eCol, Node *parentNode);
  /** Creates a head node for special areas.
      area: the area belonging to this node
      areaName: the special area name (type)
      dtd: the parent DTD
      parentNode: the parent of the node
      currentNode: the last child of the parent, if it exists
  */
  Node* createScriptTagNode(const AreaStruct &area, const QString &areaName, DTDStruct *dtd, Node *parentNode, Node *currentNode);
  /** Parses the node for XML groups (specific tags)*/
  void parseForXMLGroup(Node *node);
  /** Parses the node for Script groups (functions, variables, selectors, etc.) */
  void parseForScriptGroup(Node *node);
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

  struct ContextStruct{
    int type;
    AreaStruct area;
    QString startString;
    Node *parentNode;
    Node *lastNode;
  };

  enum ContextType {
    Unknown = 0,
    Text,
    Comment,
    QuotedString,
    Group
  };


};

#endif
