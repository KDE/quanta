/***************************************************************************
                          parser.h  -  description
                             -------------------
    begin                : Sun Sep 1 2002
    copyright            : (C) 2002, 2003 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

struct DTDListNode {
    DTDStruct *dtd;
    QString startText;
    QString endText;
    int bLine, bCol;
    int eLine, eCol;
  };

typedef QMap<QString, GroupElementMapList> IncludedGroupElements;
typedef QMap<QString, IncludedGroupElements> IncludedGroupElementsMap;

class Parser: public QObject {

Q_OBJECT

public:
  Parser();
  ~Parser();

  /** Searches for scripts inside the text from startNode. It looks only for the
  script begin/and delimiters, and not for the <script> or other special tags.
  Useful when parsing for script inside scripts, or inside the quoted attribute
  values of the xml tags.
  Returns: true if a script area is found, false if the parsed text does not
  contain any scripts. */
  bool scriptParser(Node *startNode);

  /** Parse a string, using as start position sLine, sCol. */
  Node *parseArea(int startLine, int startCol, int endLine, int endCol, Node **lastNode, Node *a_node = 0L);

  /** Parse the whole text from Document w and build the internal structure tree
  from Nodes */
  Node *parse(Document *w);

  /** Parses the found special (like script, css and such) areas.*/
  Node* specialAreaParser(Node *startNode);

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
  void parseForXMLGroup(Node *node);
  void parseForScriptGroup(Node *node);
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

private slots:
  void slotIncludedFileChanged(const QString& fileName);

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

  void parseIncludedFile(const QString& fileName, DTDStruct *dtd);

};

#endif
