/***************************************************************************
                          parser.h  -  description
                             -------------------
    begin                : Sun Sep 1 2002
    copyright            : (C) 2002 by Andras Mantia
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
	
  bool scriptParser(Node *startNode);
  /** Parse the whole text from Document w and build the internal structure tree from Nodes */  
	Node *newParse( Document *w);
  void parseInside(Node *startNode);
	Node *parse( Document *w);
  /** Print the doc structure tree to the standard output.
      Only for debugging purposes. */
  void coutTree(Node *node, int indent);
  /** Delete the internal m_node */
  void deleteNode();
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

  /** Recursive parsing algorithm. Actually this does the parsing and tree building. */
  Node * subParse( Node* parent , int &line, int &col);
  void parse2();
  /** Go to the next column, or to the next line if we are at the end of line */
  void nextPos(int &line, int &col);
  void rebuildDTDList();
};

#endif
