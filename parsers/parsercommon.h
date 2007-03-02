/***************************************************************************
                          parsercommon.h  -  description
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

#ifndef PARSERCOMMON_H
#define PARSERCOMMON_H

//qt includes
#include <qptrlist.h>
#include <qstringlist.h>

//own includes
#include "tag.h"

class Document;
struct DTDStruct;
class Node;
class KDirWatch;

namespace ParserCommon{
  extern QStringList includedFiles;
  extern QPtrList<const DTDStruct> includedFilesDTD; 
  extern KDirWatch *includeWatch;
  
  //this methods may go in a common class as well             
  QString getLine(Document *write, int line, int endLine, int endCol);
  /** Appends a text area to a text node. */
  void appendAreaToTextNode(Document *write, const AreaStruct &area, Node *node);
  /** Creates a text/empty node between node and the provided position */
  Node* createTextNode(Document *write, Node *node, int eLine, int eCol, Node *parentNode);
  /** Creates a head node for special areas.
      area: the area belonging to this node
      areaName: the special area name (type)
      dtd: the parent DTD
      parentNode: the parent of the node
      currentNode: the last child of the parent, if it exists
  */
  Node* createScriptTagNode(Document *write, const AreaStruct &area, const QString &areaName,
                            const DTDStruct *dtd, Node *parentNode, Node *currentNode);

/** Print the doc structure tree to the standard output.
    Only for debugging purposes. */
  void coutTree(Node *node, int indent);
  void verifyTree(Node *node);
}
 
#endif
