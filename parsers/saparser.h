/***************************************************************************
                          saparser.h  -  description
                             -------------------
    begin                : Wed Feb 11 2004
    copyright            : (C) 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SAPARSER_H
#define SAPARSER_H

//qt includes
#include <qobject.h>
#include <qregexp.h>
#include <qvaluestack.h>

//own includes
#include "node.h" //for GroupElementMapList
#include "tag.h" //for AreaStruct

//forward definitions
struct DTDStruct;
class Document;
class QString;
class QStringList;
class KDirWatch;


/**
  This class is used to parse a special area (script) in the document.
*/
class SAParser: public QObject
{
  Q_OBJECT

public:  
  SAParser();
  virtual ~SAParser();
  
  void setParsingEnabled(bool enabled);
  void init(Node *node, Document *write);
/*
  Parses the document for special areas (eg. scripts).
  specialArea: the area (start/end position) in the document that may contain the special
               area. It may end before the end position.
  areaStartString: the special area starting string
  forcedAreaEndString: force this as the special area ending string.
  parentNode: the Node under where the special area goes
  fullParse: the script node will be fully parsed for groups, structures or so. If false, only the script beginning and end will be determined.
  synchronous: if true, the function does not return until the parsing is finished, otherwise
  return immediately.
  return value: in synchronous case returns the last inserted node, otherwise return 0L.
*/
  Node* parseArea(const AreaStruct &specialArea,
                  const QString &areaStartString,
                  const QString &forcedAreaEndString,
                  Node *parentNode,
                  bool fullParse, bool synchronous);
  /** Returns the line where the last parsing run ended. */                  
  int lastParsedLine() {return m_lastParsedLine;}
  /** Returns the column where the last parsing run ended. */
  int lastParsedColumn() {return m_lastParsedCol;}
                  
  void parseInDetail(bool synchronous);     
  
  QStringList* selectors() {return m_selectors;}
  
  KDirWatch *includeWatch;

private slots:
  /** Parses one line and calls itself with a singleshot timer to parse the next line. */
  bool slotParseOneLine();
  void slotParseForScriptGroup();
  void slotParseNodeInDetail();
  
signals:
  void rebuildStructureTree();  
  
private:
  //private methods
  Node* parsingDone();
  void parseForScriptGroup(Node *node);

  
  //private structures
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
  
  //private member variables
  bool m_parsingEnabled;  
  bool m_synchronous;
  Document* m_write;
  Node* m_baseNode;
  Node* m_lastParsedNode;
  Node* m_currentNode; ///< the currently parsed script node for details. Changes only after the whole area between m_currentNode and m_currentNode->next is parsed.
  int m_lastParsedLine, m_lastParsedCol;
  const DTDStruct *m_dtd;
  QStringList *m_selectors; //holds the CSS selectors
  QRegExp m_quotesRx;

  bool s_contextFound;
  ContextStruct s_currentContext;
  Node *s_parentNode;
  bool s_searchContent;
  QString s_searchedString;
  QString s_textLine;
  int s_line, s_col;
  int s_endLine, s_endCol;
  bool s_fullParse;
  QString s_areaEndString;
  bool s_searchForAreaEnd;
  bool s_searchForForcedAreaEnd;
  QRegExp s_forcedAreaRx;
  const DTDStruct *s_dtd;
  bool s_searchForSpecialAreas;
  ContextStruct s_context;
  QValueStack<ContextStruct> s_contextStack;
  ContextStruct s_previousContext;
  Node *s_currentNode; ///< the current detailed node while parsing for details
  Node *s_returnNode;
  bool s_useReturnVars;
  Node *s_next;

  Node* g_node;
  Node* g_endNode;
  
};

#endif
