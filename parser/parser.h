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

struct GroupElement{
    /* The node which point to the exact place inside the parentNode */
    Node *node;
    /*The node which contains the element */
    Node *originalNode;
    /*The parent node indicating the beginning of a structure */
    Node *parentNode;
    bool global;
  };

typedef QValueList<GroupElement> GroupElementList;
typedef QMap<QString, GroupElementList> GroupElementMapList;
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

  /** Parse for groups (variables, inclusions) in the node. */
  void parseForGroups(Node *node);
  /** Parse for groups (variables, inclusions) in the node tree. */
  void parseForGroups();
  /** Remove the found groups from the memeber variables */
  void clearGroups();
  void parseIncludedFiles();
  void removeCommentsAndQuotes(QString& str, DTDStruct* dtd);

  QMap<QString, GroupElementMapList> m_groups; //a list of groups (variables, inclusions)
  QStringList includedFiles;
  QPtrList<DTDStruct> includedFilesDTD;
  IncludedGroupElementsMap includedMap;

private slots:
  void slotIncludedFileChanged(const QString& fileName);

private:
  Node* m_node;       //the internal Node pointer
  QString m_dtdName;  //the DTD name of write
  DTDStruct *m_dtd; //the dtd used for main parsing
  Document *write;    //pointer to the parsed document
  int maxLines; // how many lines are in the current document
  int oldMaxLines;
  int treeSize;
  KDirWatch *includeWatch;

  /** Print the doc structure tree to the standard output.
      Only for debugging purposes. */
  void coutTree(Node *node, int indent);
  void parseIncludedFile(const QString& fileName, DTDStruct *dtd);

};

#endif
