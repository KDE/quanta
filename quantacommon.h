/***************************************************************************
                          quantacommon.h  -  description
                             -------------------
    begin                : Sat Jul 27 2002
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

#ifndef QUANTACOMMON_H
#define QUANTACOMMON_H

#include "parser/qtag.h"
#include "parser/node.h"
#include "parser/parser.h"

/**Some common, mostly static functions.
  *@author Andras Mantia
  */

#define DEFAULT_DTD QString("-//W3C//DTD HTML 4.01 Transitional//EN")
#define TAGS_MENU_PLACE 6
#define PLUGINS_MENU_PLACE 7

class QString;
class KURL;
class KStandardDirs;
class QWidget;
class KSpellConfig;

//Quanta main configuration structure
typedef struct QConfig{
          //Tag style options
          uint tagCase;
          uint attrCase;
          QString attrValueQuotation;
          bool closeOptionalTags;
          bool closeTags;
          bool updateClosingTags;

          //editor settings
          bool useAutoCompletion;
          bool enableDTDToolbar;
          QString defaultEncoding;
          bool lineNumbers;
          bool iconBar;
          bool dynamicWordWrap;

          //parser options
          bool instantUpdate;
          bool showEmptyNodes;
          bool showClosingTags;
          uint refreshFrequency;
          QString defaultDocType;
          uint expandLevel;
          
          //environment options
          QString globalDataDir;         //not stored, initialized on app startup
          QStringList pluginSearchPaths; //global but read from plugins.rc
          QString markupMimeTypes;
          QString scriptMimeTypes;
          QString imageMimeTypes;
          QString textMimeTypes;
          QString previewPosition;

          //spelling options
          KSpellConfig *spellConfig;
        };


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
  /** Returns a pointer to a KStandardDirs object usable for plugin searchup. type is the plugin binary type (exe or lib). The returned 
pointer must be deleted by the caller!! */
  static KStandardDirs* pluginDirs(const char *type);

  /** Return true, if the url has the mimetype starting with type. */
  static bool checkMimeGroup(const KURL& url, const QString& type);
  /** Return true, if the url has the mimetype type. */
  static bool checkMimeType(const KURL& url, const QString& type);
  /** Return true, if the url has exactly the mimetype type. */
  static bool checkExactMimeType(const KURL& url, const QString& type);
  /** Returns the url without the filename. */
  static KURL convertToPath(const KURL& url);
  /** Return a string to be used when an url is saved to the project file.
      Returns url.url() if it's an absolute url and
      url.path() if the url is relative */
  static QString qUrl(const KURL& url);
  /** No descriptions */
  static void dirCreationError(QWidget *widget, const KURL& url);
  /**
  Adds the backslash before the special chars (like ?, *, . ) so the returned string can be used in regular expressions.
  */
  static QString makeRxCompatible(const QString s);
  /** Returns the translated a_str in English. A "back-translation" useful e.g in case of CSS elements selected from a listbox. */
  static QString i18n2normal(const QString& a_str);

};

#endif
