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

#include <kdeversion.h>

#include "parser/qtag.h"
#include "parser/node.h"
#include "parser/parser.h"

/**Some common, mostly static functions.
  *@author Andras Mantia
  */

#if !defined(KDE_MAKE_VERSION)
#define KDE_MAKE_VERSION( a,b,c ) (((a) << 16) | ((b) << 8) | (c))
#endif

#define DEFAULT_DTD QString("-//W3C//DTD HTML 4.01 Transitional//EN")
#define TAGS_MENU_PLACE 6
#define PLUGINS_MENU_PLACE 7

class QString;
class KURL;
class KStandardDirs;
class QWidget;
class KSpellConfig;

//TODO: remove once KDE 3.1.x is not supported
#include <kaction.h>
class KPopupMenu;

//Quanta main configuration structure
typedef struct QConfig{
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
          bool showDTDSelectDialog;

#ifdef BUILD_KAFKAPART
         //kafka sync options
        bool quantaRefreshOnFocus;
        int quantaRefreshDelay;
        bool kafkaRefreshOnFocus;
        int kafkaRefreshDelay;
#endif

          //environment options
          QString globalDataDir;         //not stored, initialized on app startup
          QStringList pluginSearchPaths; //global but read from plugins.rc
          QString markupMimeTypes;
          QString scriptMimeTypes;
          QString imageMimeTypes;
          QString textMimeTypes;
          QString previewPosition;
          QString windowLayout;
	  int autosaveInterval;
          QString autosaveEntryList;
	  QString autosaveEntryKey;

          //spelling options
          KSpellConfig *spellConfig;
        };

#if KDE_VERSION < KDE_MAKE_VERSION(3,1,90)
typedef struct {
     bool text : 1;
     enum { NoCompression=0, GZipCompression } compression : 4;
     unsigned int dummy : 27;
  } Format;
#endif

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
  static QTag* tagFromDTD(DTDStruct* dtd, const QString& tag);
  /** Returns an XML style string containing the GUI for attributes. */
  static QString xmlFromAttributes(AttributeList* attributes);
  /** Returns the DTD name (identifier) corresponding to the DTD's nickname */
  static QString getDTDNameFromNickName(const QString& nickName);
  /** Returns the DTD iddentifier from the given nickname */
  static QString getDTDNickNameFromName(const QString& name);
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

#if KDE_VERSION < KDE_MAKE_VERSION(3,1,90)
  static Format findFormatByFileContent( const QString& fileName );
  static QString obscure( const QString &str );
#endif
/** No descriptions */
  static void normalizeStructure(QString f,QStringList& l);
};


//TODO: remove once KDE 3.1.x is not supported
//backported classes from CVS HEAD. I just don't want to create new files for
//this temporary present classes.
class KQPasteAction: public KAction
{
    Q_OBJECT
public:
    KQPasteAction( const QString& text, const QString& icon, const KShortcut& cut,
                  const QObject* receiver, const char* slot,
                  QObject* parent = 0, const char* name = 0 );

    virtual ~KQPasteAction();
    virtual int plug( QWidget *widget, int index = -1 );

protected slots:
    void menuAboutToShow();
    void menuItemActivated( int id);

private:
    KPopupMenu *m_popup;
};

/**
 *  This class is an action to handle a recent files submenu.
 *  The best way to create the action is to use KStdAction::openRecent.
 *  Then you simply need to call loadEntries on startup, saveEntries
 *  on shutdown, addURL when your application loads/saves a file.
 *
 *  @author Michael Koch
 *  @short Recent files action
 */
class KQRecentFilesAction : public KListAction  // TODO public KSelectAction
{
  Q_OBJECT
  Q_PROPERTY( uint maxItems READ maxItems WRITE setMaxItems )
public:
  /**
   *  @param text The text that will be displayed.
   *  @param pix The dynamically loaded icon that goes with this action.
   *  @param cut The corresponding keyboard accelerator (shortcut).
   *  @param receiver The SLOT's parent.
   *  @param slot The SLOT to invoke when a URL is selected.
   *  Its signature is of the form slotURLSelected( const KURL & ).
   *  @param parent This action's parent.
   *  @param name An internal name for this action.
   *  @param maxItems The maximum number of files to display
   */
  KQRecentFilesAction( const QString& text, const QString& pix, const KShortcut& cut,
                      const QObject* receiver, const char* slot,
                      QObject* parent, const char* name = 0,
                      uint maxItems = 10 );

  /**
   *  Destructor.
   */
  virtual ~KQRecentFilesAction();

  virtual int plug( QWidget *widget, int index = -1 );

  /**
   *  Returns the maximum of items in the recent files list.
   */
  uint maxItems() const;

public slots:
  /**
   *  Sets the maximum of items in the recent files list.
   *  The default for this value is 10 set in the constructor.
   *
   *  If this value is lesser than the number of items currently
   *  in the recent files list the last items are deleted until
   *  the number of items are equal to the new maximum.
   */
  void setMaxItems( uint maxItems );

  /**
   *  Loads the recent files entries from a given KConfig object.
   *  You can provide the name of the group used to load the entries.
   *  If the groupname is empty, entries are load from a group called 'RecentFiles'
   *
   *  This method does not effect the active group of KConfig.
   */
  void loadEntries( KConfig* config, QString groupname=QString::null );

  /**
   *  Saves the current recent files entries to a given KConfig object.
   *  You can provide the name of the group used to load the entries.
   *  If the groupname is empty, entries are saved to a group called 'RecentFiles'
   *
   *  This method does not effect the active group of KConfig.
   */
  void saveEntries( KConfig* config, QString groupname=QString::null );

  /**
   *  Add URL to recent files list.
   *
   *  @param url The URL of the file
   */
  void addURL( const KURL& url );

  /**
   *  Remove an URL from the recent files list.
   *
   *  @param url The URL of the file
   */
  void removeURL( const KURL& url );

  /**
   *  Removes all entries from the recent files list.
   */
  void clearURLList();

signals:

  /**
   *  This signal gets emited when the user selects an URL.
   *
   *  @param url The URL thats the user selected.
   */
  void urlSelected( const KURL& url );

protected slots:
  void itemSelected( const QString& string );
  void menuAboutToShow();
  void menuItemActivated( int id );
  void slotClicked();
  virtual void slotActivated();

private:
  void init();

  /**
    * The popup menu that is shown when clicking (some time) on the toolbar
    * button. You may want to plug items into it on creation, or connect to
    * aboutToShow for a more dynamic menu.
    */
  KPopupMenu *popupMenu() const;

  class KQRecentFilesActionPrivate;
  KQRecentFilesActionPrivate *d;
};

#endif
