/***************************************************************************
    begin                : Mon Jun 27 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef PARSERESULT_H
#define PARSERESULT_H


#include "dtdstruct.h"

#include <QList>

#include <kurl.h>

class DomModel;

/** Describes a parser result.*/
struct ParseResult 
{
  ParseResult() : model(0L), baseURL(KUrl()) {};
  
  /** the base of the node tree resulted from a parsing */
  DomModel *model; 
  
  
  /** a list with the DTEPs present in the node tree  */
  QList<const DTDStruct*> dtepList;
  
  /** a list with the tags that were found during parsing, but are not part of the parsing DTEP. The key of the dictionary is the tag name in lowercase. */
  QHash<QString, QTag*> userTags;
  
  /** the base url for all relative path entries in the document.
   *  this is usually the path to the document but can be something else
   *  if the BASE tag is present in the header
   */
   KUrl baseURL;
  
};
#endif
