/***************************************************************************
                          quantacommon.cpp  -  description
                             -------------------
    begin                : Sat Jul 27 2002
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
//qt includes
#include <qstring.h>
#include <qdict.h>

//kde includes
#include <kurl.h>

#include "quantacommon.h"

uint tagsCase;
uint attrsCase;
bool useCloseTag;
bool useAutoCompletion;
QString globalDataDir;

QDict<AttributeList> *tagsDict;


QuantaCommon::QuantaCommon(){
}

QuantaCommon::~QuantaCommon(){
}

/** convert tag to upper or lower case */
QString QuantaCommon::tagCase( const QString tag)
{
  QString sTag = tag;

  switch (tagsCase)
  {
    case 1: sTag = tag.lower();
            break;
    case 2: sTag = tag.upper();
  }
  return sTag;
}

/** convert attr of tag to upper or lower case */
QString QuantaCommon::attrCase( const QString attr)
{
  QString sAttr = attr;

  switch (attrsCase)
  {
    case 1: sAttr = attr.lower();
    case 2: sAttr = attr.upper();
  }
  return sAttr;
}

/** Set's up the url correctly from urlString. */
void QuantaCommon::setUrl(KURL &url, QString urlString)
{
 if (urlString.at(0) == '/')
 {
   url.setPath(urlString);
 }
 else
 {
   url = urlString;
 }
}
