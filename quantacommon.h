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
#include "parser/node.h"
#include "parser/parser.h"

/**Some common, mostly static functions.
  *@author Andras Mantia
  */

#define DEFAULT_DTD QString("-//W3C//DTD HTML 4.01 Transitional//EN")

//Quanta main configuration structure
typedef struct QConfig{
          //Tag style options
          uint tagCase;
          uint attrCase;
          QString attrValueQuotation;
          bool closeOptionalTags;
          bool closeTags;

          //editor settings
          bool useAutoCompletion;
          bool enableDTDToolbar;
          QString defaultEncoding;

          //parser options
          bool useMimeTypes;
          uint refreshFrequency;
          QString defaultDocType;

          //environment options
          QString globalDataDir;

        }; 

class QuantaApp;
class QString;
class KURL;

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
  /** Returns the QTag object for the tag "tag" from the DTD. */
  static QTag* tagFromDTD(DTDStruct * dtd, QString tag);
  /** Returns an XML style string containing the GUI for attributes. */
  static QString xmlFromAttributes(AttributeList* attributes);
  /** Returns list of values for attribute */
  static QStringList* tagAttributeValues(QString dtdName, QString tag, QString attribute);
  /** Returns the DTD name (identifier) corresponding to the DTD's nickname */
  static QString getDTDNameFromNickName(QString nickName);
  /** Returns the DTD iddentifier from the given nickname */
  static QString getDTDNickNameFromName(QString name);
  /** Returns 0 if the (line,col) is inside the area specified by the other arguments,
      -1 if it is before the area and 1 if it is after. */
  static int isBetween(int line, int col, int bLine, int bCol, int eLine, int eCol);
#if 0 // Redundant in new plugin architecture [MB]
  /** Check if plugin is available or not. */
  static bool pluginAvailable(const QString& name);
#endif

public: // Public attributes
  /**  */
  static QuantaApp *quantaApp;
};

#endif
