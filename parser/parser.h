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

#include <qdict.h>
#include <qstringlist.h>

#include "node.h"
#include "tag.h"
#include "qtag.h"


/**
  *@author Andras Mantia
  */

class Document;

struct DTDListNode {
    DTDStruct *dtd;
    QString startText;
    QString endText;
    int bLine, bCol;
    int eLine, eCol;
  };

class Parser {
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

  /** Parse the whole text from Document w and build the internal structure tree
  from Nodes */
	Node *parse( Document *w);

  /** Parses the found special (like script, css and such) areas.*/
  void specialAreaParser(Node *startNode);

  /** Returns the node for position (line, column). As more than one node can
  contain the same area, it return the "deepest" node. */
  Node *nodeAt(int line, int col);

  /** Clear the parser internal text, thus forcing the reparsing. */
  void clear();

  /** Builds an internal tree to reflect the areas where each real & pseudo dtd is active. */
  void parseForDTD(Document *w, bool force = false);

  /** No descriptions */
  DTDStruct * currentDTD(int line, int col);

	QString m_text;  //FIXME: having an internal copy of text is absolutely useless

private:
  Node* m_node;       //the internal Node pointer
  QString m_dtdName;  //the DTD name of write
  DTDStruct *m_dtd; //the dtd used for main parsing
  Document *write;    //pointer to the parsed document
  int maxLines; // how many lines are in the current document
  int oldMaxLines;
  QValueList<DTDListNode> dtdList;

  /** Print the doc structure tree to the standard output.
      Only for debugging purposes. */
  void coutTree(Node *node, int indent);
  void rebuildDTDList();
};

#endif
