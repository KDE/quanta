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
class KURL;
class KStandardDirs;
class QWidget;
class Tag;

class KConfig;
class KPopupMenu;

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
          bool replaceNotInEncoding; ///< replace characters with their entity number if they cannot be saved in the current encoding of the document.
          bool smartTagInsertion; //enable/disable smartTagInsertion
        };

typedef struct DirInfo{
      QString mimeType;
      QString preText;
      QString postText;
      bool    usePrePostText;
    };

/**Some common, mostly static functions.
  *@author Andras Mantia
  */

class QuantaCommon {
public:
  QuantaCommon();
  ~QuantaCommon();

  /** convert tag to upper or lower case */
  static QString tagCase( const QString& tag);
  /** convert tag to upper or lower case */
  static QString attrCase( const QString& attr);
/** returns the attribute value in quoted form, by taking care of the
quotation setting*/
  static QString quoteAttributeValue(const QString& value);
  /** Set's up the url correctly from urlString. */
  static void setUrl(KURL &url, const QString& urlString);
  /** No descriptions */
  static bool isSingleTag(const QString& dtdName, const QString& tag);
  /** No descriptions */
  static bool isOptionalTag(const QString& dtdName, const QString& tag);
  /** No descriptions */
  static bool isKnownTag(const QString& dtdName, const QString& tag);
  /** No descriptions */
  static AttributeList* tagAttributes(const QString& dtdName, const QString& tag);
  /** Returns the QTag object for the tag "tag" from the DTD named "dtdname". */
  static QTag* tagFromDTD(const QString& dtdName, const QString& tag);
  /** Returns the QTag object for the tag "tag" from the DTD. */
  static QTag* tagFromDTD(const DTDStruct* dtd, const QString& tag);
  /** Returns the QTag object for the node "node" from node's DTD. */
  static QTag* tagFromDTD(Node *node);
  /** Returns an XML style string containing the GUI for attributes. */
  static QString xmlFromAttributes(AttributeList* attributes);
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
  static QString makeRxCompatible(const QString& s);
  /** Returns the translated a_str in English. A "back-translation" useful e.g in case of CSS elements selected from a listbox. */
  static QString i18n2normal(const QString& a_str);

/** No descriptions */
  static void normalizeStructure(QString f,QStringList& l);
  /**Returns true if tag2 is the closing pair of tag1. It's namespace aware.*/
  static bool closesTag(Tag *tag1, Tag *tag2);
  static bool closesTag(QString namespaceName, QString tagName, bool caseSensitive,
    QString namespaceName2, QString tagName2, bool caseSensitive2);
  static int denyBinaryInsert(QWidget *window);
  static void removeCommentsAndQuotes(QString& str, const DTDStruct* dtd);
  static bool insideCommentsOrQuotes(int position, const QString &string, const DTDStruct *dtd);
  /** Calls a Quanta DCOP method.
  * @param interface the DCOP interface the method belongs to
  * @param method the DCOP method name (with the argument types)
  * @param arguments comma separated list of argument
  * @return the return value of the DCOP caller
  */
  static DCOPReply callDCOPMethod(const QString& interface, const QString& method, const QString& arguments);
  /** Checks if url exists and shows a question about overwriting it.
  *  @param url the url to check
  *  @return true if the user answered yes, false otherwise.
  */
  static bool checkOverwrite(const KURL& url, QWidget *window);

  /**
   * Same as KConfigBase::readPathListEntry, but resolves symlinks
   * @param config
   * @param pKey
   * @return
   */
  static QStringList readPathListEntry(KConfig *config, const QString &pKey);

    /**
   * Same as KConfigBase::readPathEntry, but resolves symlinks
   * @param config
   * @param pKey
   * @return
     */
  static QString readPathEntry(KConfig *config, const QString &pKey);

  /**
   * Returns the HTML encoding string for character with the specified code.
   * If no such literal encoding is known (like &auml;), the numeric encoding
   * is returned: &#code;
   * @param code the numeric code of the character
   * @return the encoded string
   */
  static QString encodedChar(uint code);

};

#endif
