/***************************************************************************
                          quantacommon.h  -  description
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

#ifndef QUANTACOMMON_H
#define QUANTACOMMON_H

#include "parser/qtag.h"

class QString;
class KURL;
/**Some common, mostly static functions.
  *@author Andras Mantia
  */

class QuantaCommon {
public: 
	QuantaCommon();
	~QuantaCommon();

  /** convert tag to upper or lower case */
  static QString tagCase( const QString tag);
  /** convert tag to upper or lower case */
  static QString attrCase( const QString attr);
  /** Set's up the url correctly from urlString. */
  static void setUrl(KURL &url, QString urlString);
  /** No descriptions */
  static bool isSingleTag(QString dtdName, QString tag);
  /** No descriptions */
  static bool isOptionalTag(QString dtdName, QString tag);
  /** No descriptions */
  static bool isKnownTag(QString dtdName, QString tag);
  /** No descriptions */
  static AttributeList*  tagAttributes(QString dtdName, QString tag);
  /** Returns the QTag object for the tag "tag" from the DTD named "dtdname". */
  static QTag* tagFromDTD(QString dtdName, QString tag);
  /** Returns an XML style string containing the GUI for attributes. */
  static QString xmlFromAttributes(AttributeList* attributes);
  /** Returns list of values for attribute */
  static QStringList* tagAttributeValues(QString dtdName, QString tag, QString attribute);
  /** Returns the DTD name (identifier) corresponding to the DTD's nickname */
  static QString getDTDNameFromNickName(QString nickName);
  /** Returns the DTD iddentifier from the given nickname */
  static QString getDTDNickNameFromName(QString name);

};

#endif
