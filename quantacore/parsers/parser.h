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
#include <qstringlist.h>
#include <qmap.h>
#include <qpointer.h>

#include "node.h"
#include "tag.h"
#include "qtag.h"

/**
  *@author Andras Mantia
  */

class EditorSource;

class KDirWatch;
class QRegExp;
class NodeModifsSet;
class ParseResult;
class SAParser;

namespace KTextEditor
{
  class Document;
}

// typedef QMap<QString, GroupElementMapList> IncludedGroupElements;
// typedef QMap<QString, IncludedGroupElements> IncludedGroupElementsMap;


class Parser: public QObject {

Q_OBJECT

public:
  Parser();
  ~Parser();

  /**
   * Request a full parsing.
   * @param source the source to be parsed
   * @param base the base node of the current node tree with additional information. base.baseNode is 0L if no node tree exists for the current source.
   * @param dtd the main DTD of the source
   * @param detailed enable or disable detailed (special area) parsing
   */
  void parse(EditorSource *source, ParseResult *base, const DTDStruct *dtd, bool detailed);

  /**
   * Rebuilds the node tree for the source. Tries to parse as small amount as possible, but sometimes a full parsing cannot be avoided, so the whole node tree will be changed.
   * @param source the source to be parsed
   * @param base the base node of the current node tree with additional information. base.baseNode is 0L if no node tree exists for the current source.
   * @param dtd the main DTD of the source
   * @param detailed enable or disable detailed (special area) parsing
   */
  void rebuild(EditorSource *source, ParseResult *base, const DTDStruct *dtd, bool detailed);

  /** Remove the found groups from the memeber variables */
  void clearGroups();
  void parseIncludedFiles();

//   void setParsingEnabled(bool enabled) {m_parsingEnabled = enabled;}
//   bool isParsingEnabled() {return m_parsingEnabled;}

  void synchParseInDetail();

//FIXME  IncludedGroupElementsMap includedMap;
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
  void finishedParsing(const EditorSource *, const ParseResult  *);
  void groupsParsed(const EditorSource *, const ParseResult  *);

private:
  /** Parse an area in the source.
   * @param startLine the starting line of the area
   * @param startCol  the starting column of the area
   * @param endLine the ending line of the area
   * @param endCol the ending column of the area
   * @param lastNode will contain the last parsed node
   * @param node the node used as a parent/start for the tree
   * @return the head node of the parsed area
   */
  Node *parseArea(int startLine, int startCol, int endLine, int endCol, Node **lastNode, Node *node = 0L);
  /** Enable/Disable parsing. 
   * 
   * @param enabled true to enable, false to disable and stop the special area parser.
   */
  void setSAParserEnabled(bool enabled);

  /**
   * Apppends the tag and its attributes to the user tag list if the tag cannot be find in the current DTEP
   * @param tag the tag to check
   */
  void appendUserTag(Tag *tag);

  ParseResult* m_base;     ///the internal result pointer
  const DTDStruct* m_dtd; ///<the DTD used for main parsing
  int maxLines; // how many lines are in the current document
  int oldMaxLines;
  int treeSize;
  QMap<QString, XMLStructGroup>::ConstIterator xmlGroupIt;
//   bool m_parsingEnabled;

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
  bool invalidArea(AreaStruct &area, Node **firstNode, Node **lastNode);

  /** Deletes all the nodes between the firstNode and lastNode and keeps the tree's consistency.
      modifs is the class recording these changes for the undo/redo system, cf undoredo.h */
  void deleteNodes(Node *firstNode, Node *lastNode/*FIXME, NodeModifsSet *modifs*/);

  /**
   * This function must be called before reparsing : it log in the undo/redo system
   * that the whole Node tree is reloaded.
   * @param modifs This class record all the changes made.
   * @param w modifs will be inserted in w's undoredo list.
   */
  /* FIXME void logReparse(NodeModifsSet *modifs, Document *w); */


  SAParser *m_saParser; //the special area parser object

  /** Maybe we should move to a separate, special area parsing class */
  Node* specialAreaParsingDone(int &lastLine, int &lastCol);

  bool m_parseIncludedFiles;

  KTextEditor::Document *m_document;
  EditorSource *m_source;
};



#endif
