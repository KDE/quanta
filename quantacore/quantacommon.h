/***************************************************************************
                          quantacommon.h  -  description
                             -------------------
    begin                : Sat Jul 27 2002
    copyright            : (C) 2002, 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef QUANTACOMMON_H
#define QUANTACOMMON_H

#include <kdeversion.h>

#include "qtag.h"
#include "node.h"
#include "parser.h"

/**Some common, mostly static functions.
  *@author Andras Mantia
  */

#define DEFAULT_DTD QString("-//W3C//DTD HTML 4.01 Transitional//EN")

class QString;
class DCOPReply;
class KUrl;
class KStandardDirs;
class QWidget;
class Tag;


/** Describes one abbreviation group */
class Abbreviation{
public:
/*A list with abbreviations in the for of: <template templatename, code> */
  QMap<QString, QString> abbreviations;
  QStringList dteps;
};


//Quanta main configuration structure
class QConfig{
public:
          //Tag style options
          uint tagCase;
          uint attrCase;
          QChar attrValueQuotation;
          bool closeOptionalTags;
          bool closeTags;
          bool updateClosingTags;

          //editor settings
          bool useAutoCompletion;
          bool enableDTDToolbar;
          QString defaultEncoding;

          //parser options
          bool instantUpdate;
          bool showEmptyNodes;
          bool showClosingTags;
          uint refreshFrequency;
          QString defaultDocType;
          uint expandLevel;
          bool showDTDSelectDialog;
          QString showCloseButtons; ///< can be Disabled, ShowAlways or ShowDelayed
          uint toolviewTabs; ///< how does the toolview tabs look like

         //kafka sync options
          bool quantaRefreshOnFocus;
          int quantaRefreshDelay;
          bool kafkaRefreshOnFocus;
          int kafkaRefreshDelay;
          
          //kafka indentation options
          bool inlineNodeIndentation;

          //environment options
          QString globalDataDir;         //not stored, initialized on app startup
          QStringList pluginSearchPaths; //global but read from plugins.rc
          QString markupMimeTypes;
          QString scriptMimeTypes;
          QString imageMimeTypes;
          QString textMimeTypes;
          QString previewPosition;
          QString docPosition;
          QString windowLayout;
          uint autosaveInterval;
          QString backupDirPath;
          QString quantaPID;
          bool showHiddenFiles;  ///< show hidden files in files treeview?
          bool saveTrees;         ///< save tree status for local trees?
          QMap<QString, Abbreviation> abbreviations; ///< the abbreviation groups
          bool replaceAccented; ///< replace or not the accented characters
          bool smartTagInsertion; //enable/disable smartTagInsertion
        };


/**Some common, mostly static functions.
  *@author Andras Mantia
  */

class QuantaCommon {
public:
  QuantaCommon();
  ~QuantaCommon();

  /** convert tag to upper or lower case
  moved to tag.cpp
  */
//  static QString tagCase( const QString& tag);
  
  /** convert tag to upper or lower case 
  moved to TagAttr.h
  */
//  static QString attrCase( const QString& attr);


/** returns the attribute value in quoted form, by taking care of the
quotation setting*/
//   static QString quoteAttributeValue(const QString& value);
  /** No descriptions 
  
  moved to dtdstruct
  */
//   static bool isSingleTag(const QString& dtdName, const QString& tag);
  /**   moved to dtdstruct
*/
//   static bool isOptionalTag(const QString& dtdName, const QString& tag);
  /**   moved to dtdstruct
*/
//   static bool isKnownTag(const QString& dtdName, const QString& tag);
  /**   moved to dtdstruct
*/
//   static AttributeList* tagAttributes(const QString& dtdName, const QString& tag);
  
  /** Returns the QTag object for the tag "tag" from the DTD named "dtdname". 
  
moved to qtag.cpp

*/
//  static QTag* tagFromDTD(const QString& dtdName, const QString& tag);
  
  /** Returns the QTag object for the tag "tag" from the DTD. 
  
moved to qtag.cpp

*/
//  static QTag* tagFromDTD(const DTDStruct* dtd, const QString& tag);
  
  /** Returns the QTag object for the node "node" from node's DTD. 
  
moved to qtag.cpp

*/
//  static QTag* tagFromDTD(Node *node);
  
  /** Returns an XML style string containing the GUI for attributes. */
  static QString xmlFromAttributes(AttributeList* attributes);
  
  /** Returns 0 if the (line,col) is inside the area specified by the other arguments,
      -1 if it is before the area and 1 if it is after. */
//  moved to areastruct.h  !!!!!!!!!!!!
//  static int isBetween(int line, int col, int bLine, int bCol, int eLine, int eCol);
  
  
  /** Returns a pointer to a KStandardDirs object usable for plugin searchup. type is the plugin binary type (exe or lib). The returned
pointer must be deleted by the caller!! */
  static KStandardDirs* pluginDirs(const char *type);

  /** Return true, if the url has the mimetype starting with type. */
  static bool checkMimeGroup(const KUrl& url, const QString& type);
  /** Return true, if the url has the mimetype type. */
//   static bool checkMimeType(const KUrl& url, const QString& type);

  /** Return true, if the url has exactly the mimetype type. 
  
     use: KMimeType::findByURL(url)->name() == type
  */
//   static bool checkExactMimeType(const KUrl& url, const QString& type);
  
  
  /** Return a string to be used when an url is saved to the project file.
      Returns url.url() if it's an absolute url and
      url.path() if the url is relative 
      FIXME remove of not needed
            @deprecated don't store a relative path in KUrl!
  */
  static QString qUrl(const KUrl& url);
  
  /** No descriptions 
       @deprecated use QuantaNetAccess::mkdir and get the dialog for free
*/
  static void dirCreationError(QWidget *widget, const KUrl& url);
  
  /**
  Adds the backslash before the special chars (like ?, *, . ) so the returned string can be used in regular expressions.
  */
  static QString makeRxCompatible(const QString& s);
  
  /** Returns the translated a_str in English. A "back-translation" useful e.g in case of CSS elements selected from a listbox. */
  static QString i18n2normal(const QString& a_str);

  /** No descriptions */
  static void normalizeStructure(QString f,QStringList& l);
  
  static void removeCommentsAndQuotes(QString& str, const DTDStruct* dtd);
  static bool insideCommentsOrQuotes(int position, const QString &string, const DTDStruct *dtd);
  /** Calls a Quanta DCOP method.
  * @param interface the DCOP interface the method belongs to
  * @param method the DCOP method name (with the argument types)
  * @param arguments comma separated list of argument
  * @return the return value of the DCOP caller
  */
  static DCOPReply callDCOPMethod(const QString& interface, const QString& method, const QString& arguments);

  /** Find the word until the first word boundary backwards.
   * 
   * @param textToSearch The text to look up into
   * @param withHyphen allow a hyphen inside of a word or not
   * @return the found word
   */
  static QString findWordRev(const QString& textToSearch, bool withHyphen);
};

#endif
