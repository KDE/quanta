/***************************************************************************
                          quanta.h  -  description
                             -------------------
    begin                : ?? ???  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2001-2003 by Andras Mantia <amantia@freemail.hu>
    email                : pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUANTA_H
#define QUANTA_H

#define IDS_STATUS      1
#define IDS_INS_OVR     2
#define IDS_MODIFIED    3
#define IDS_STATUS_CLM  4
#define IDS_DEFAULT     "Ready."

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// include files for Qt
#include <qmap.h>
#include <qvaluelist.h>
#include <qstrlist.h>
#include <qptrlist.h>

// include files for KDE
#include <kapplication.h>
#include <kdockwidget.h>
#include "kqapp.h"
#include "widgets/whtmlpart.h"
#include <kparts/browserextension.h>
#include <kate/document.h>
#include <ktempfile.h>

//app includes
#include "parser/qtag.h"
#include "treeviews/filestreeview.h"
#include "quantacommon.h"
#include "qextfileinfo.h"
#include "resource.h"
#include "dcopwindowmanagerif.h"

// forward declaration
class QuantaPluginInterface;

class QuantaDoc;
class QuantaView;

class QTabWidget;
class QWidgetStack;
class QListViewItem;

class QPopupMenu;

class WHTMLPart;
class KHTMLView;

class WKafkaPart;

class KAction;
class KToggleAction;
class KSelectAction;
class KRecentFilesAction;

class Node;
class Parser;
class DocTreeView;
class FilesTreeView;
class StructTreeView;
class ProjectTreeView;
class TemplatesTreeView;
class TagAttributeTree;
class Project;
class GrepDialog;
class MessageOutput;
class QDomDocument;
class Document;

class PHP3Debugger;
class PHP4Debugger;
class SpellChecker;

typedef struct ToolbarEntry{
  KXMLGUIClient *guiClient;
  QDomDocument *dom;
  QPopupMenu *menu;
  KURL url;
  QString name;
  bool user;
  bool visible;
};

/**
  * The base class for Quanta application windows.
  */
class QuantaApp : public KDockMainWindow, virtual public DCOPWindowManagerIf
{
  Q_OBJECT

public:
  QuantaApp();
  ~QuantaApp();

  QuantaDoc  *doc() const {return m_doc; }
  QuantaView *view() const {return m_view;}
  Project *project() const {return m_project; }
  QPopupMenu *tagsMenu() const {return m_tagsMenu;}
  QPopupMenu *pluginMenu() const {return m_pluginMenu;}
  KConfig *config() const {return m_config;}
  QWidgetStack *rightWidget() const {return rightWidgetStack;}
  QWidgetStack *bottomWidget() const {return bottomWidgetStack;}

//TODO: check if we really need these "get" methods (and get rid o get)
  SpellChecker *spellChecker() const {return m_spellChecker;}
  TemplatesTreeView *gettTab() const {return tTab;}
  StructTreeView *getsTab() const {return sTab;}
  MessageOutput *getMessageOutput() const {return messageOutput;}

  QPopupMenu *toolbarMenu(const QString& name);
  ToolbarEntry *toolbarByURL(const KURL& url);

  QWidgetStack *widgetStackOfHtmlPart();

  /** Returns the output dock widget */
  KDockWidget *outputDockWidget() const { return bottdock; }

  /** Loads the initial project */
  void loadInitialProject(const QString& url);

  QWidget* createContainer(QWidget *parent, int index, const QDomElement &element, int &id );
  void removeContainer(QWidget *container, QWidget *parent, QDomElement &element, int id );
  /** Reads the DTD info from the file, tries to find the correct DTD and builds the tag/attribute list from the DTD file. */
  void processDTD(const QString& documentType = QString::null);
/** Returns the project's base URL if it exists, the HOME dir if there is no project and no opened document (or the current opened document was not saved yet), and the base URL of the opened document, if it is saved somewhere. */
  KURL projectBaseURL() const;

  KURL::List userToolbarFiles();
  /** Returns the project (if there is one loaded) or global default encoding. */
  QString defaultEncoding();
  /** Returns the interface number for the currently active editor. */
  int currentEditorIfNum() const;
  /** Return the URL of the currently active document */
  QString currentURL() const;
  /** Return the URL of the currently project */
  QString projectURL() const;
  /** Return the list of opened URLs and their editor interface numbers*/
  QStringList openedURLs() const;

  /** reparse current document and initialize node. */
  void reparse(bool force);

  bool structTreeVisible() const;

signals: // Signals
  /** The tree views should be updated due to some changes on the disk. */
  void reloadTreeviews();

public slots:
  /** Delayed initialization. */
  void initQuanta();

  void slotFileNew();
  void slotFileOpen();
  void slotFileOpen( const KURL &, const QString& );
  void slotFileSave();
  bool slotFileSaveAs();
  void slotFileSaveAsLocalTemplate();
  void slotFileSaveAsProjectTemplate();
  void slotFileSaveSelectionAsLocalTemplate();
  void slotFileSaveSelectionAsProjectTemplate();
  void slotFileSaveAll();
  void slotFileClose();
  void slotFileCloseAll();
  void slotFilePrint();
  void slotFileNext();
  void slotFilePrev();
  void slotFileQuit();

  void slotImageOpen(const KURL&);
  void slotFileOpenRecent(const KURL&);
  void slotInsertTag(const KURL&, DirInfo);

  void slotEditFindInFiles();
  void slotToolSyntaxCheck();
  void openLastFiles();
  /// open url in documentation window
  void openDoc(QString url );

  void slotContextHelp();

  void slotBack();
  void slotForward();

  void slotViewToolBar();
  void slotViewStatusBar();

  void statusBarTimeout();
  void slotStatusMsg(const QString &text);

  void slotNewStatus();
  void slotNewLineColumn();
  void slotUpdateStatus(QWidget*);
  void slotClosePage(QWidget*);

  void slotDockStatusChanged();
  void slotDockChanged();

  /** repaint preview */
  void repaintPreview( bool clear = false);
  void slotActivatePreview();
  /** show preview ( F6 )*/
  void slotShowPreview();
  /** slot WYSIWYG */
  void slotShowKafkaPart();
  void slotShowProjectTree();
  void slotShowTemplatesTree();
  /** Repaint preview ( slot ) */
  void slotViewRepaint();

  void slotShowBottDock(bool force = false);
  void slotShowFTabDock();
  void slotShowPTabDock();
  void slotShowTTabDock();
  void slotShowSTabDock();
  void slotShowATabDock();
  void slotShowDTabDock();

  void viewMenuAboutToShow();
  void settingsMenuAboutToShow();
  void setEOLMenuAboutToShow();
  void slotContextMenuAboutToShow();
  void bookmarkMenuAboutToShow();
  void gotoBookmark(int n);

  void slotMessageWidgetEnable();
  void slotMessageWidgetDisable();

  /** options slots */
  void slotOptions();
  void slotOptionsConfigureKeys();
  void slotOptionsConfigureToolbars();
  void slotNewToolbarConfig();
  /** Configure toolbars, show defaultToolbar by default */
  void slotConfigureToolbars(const QString& defaultToolbar = QString::null);
  void slotOptionsConfigureActions();

  void enablePhp3Debug(bool);
  void enablePhp4Debug(bool);

  void setCursorPosition(int row, int col );
  void gotoFileAndLine(const QString& filename, int line );

  void selectArea(int line1, int col1, int line2, int col2);
  void autoComplete();

  void slotShowOpenFileList();
  /** No descriptions */
  void slotNewProjectLoaded();
  /** No descriptions */
  void slotInsertFile(const KURL&);
  /** No descriptions */
  void slotSyntaxCheckDone();
  /** Sends a toolbar in mail. */
  void slotSendToolbar();
  /** Removes a user toolbar from the toolbars. */
  void slotRemoveToolbar();
  /** Adds a new, empty toolbar. */
  void slotAddToolbar();
  /** Saves a toolbar as local specific. */
  void slotSaveLocalToolbar();
  /** Saves a toolbar as project specific. */
  void slotSaveProjectToolbar();
  /** Load an user toolbar from the disk. */
  void slotLoadToolbarFile(const KURL& url);
  /** Load an user toolbar from the disk. */
  void slotLoadToolbar();
  /** Load a global toolbar from the disk. */
  void slotLoadGlobalToolbar();
  /** Remove the toolbar named "name". */
  void slotRemoveToolbar(const QString& name);
  /** Rename the toolbar named "name". */
  void slotRenameToolbar(const QString& name);
  /** Rename the toolbar. */
  void slotRenameToolbar();
  /** Delete an action */
  void slotDeleteAction(KAction *action);
  /** Remove the action from toolbar*/
  void slotRemoveAction(const QString&, const QString& actionName);
  /** Edit the action */
  void slotEditAction(const QString&);

  /** No descriptions */
  void slotToolsChangeDTD();
  /** No descriptions */
  void slotShowCompletion();
  /** No descriptions */
  void slotShowCompletionHint();
  /** No descriptions */
  void slotParsingDTDChanged(const QString& newDTDName);
  /** Runs the plugin specified by id */
  void slotPluginRun(int);
  /** Builds the plugins menu dynamically */
  void slotBuildPluginMenu();
  /** Brings up the plugin editor menu */
  void slotPluginsEdit();
  /** Validates the currently loaded plugins */
  void slotPluginsValidate();
  /** No descriptions */
  void slotBuildPrjToolbarsMenu();

  void slotReparse();
  void slotForceReparse();

  void slotExpandAbbreviation();

protected slots:
  /** No descriptions */
  void slotMakeDonation();
  /** No descriptions */
  void slotHelpHomepage();
  /** Show or hide the DTD toolbar */
  void slotToggleDTDToolbar(bool show);
  /** No descriptions */
  void slotEmailDTD();
  /** Shows tip of the day */
  void slotHelpTip();
  /** Show the user mailing list sign up */
  void slotHelpUserList();
  void slotOpenFileUnderCursor();
  void slotUploadFile();
  void slotUploadOpenedFiles();

protected:
  WHTMLPart *htmlPart();
  KParts::BrowserExtension *browserExtension()
  {
      return static_cast<KParts::BrowserExtension *>(((KParts::ReadOnlyPart *)htmlPart())->child( 0L, "KParts::BrowserExtension" ));
  }
  void initToolBars();
  /** Ask for save all the modified user toolbars. */
  void removeToolbars();
  /** Returns true if all toolbars are hidden, false otherwise. */
  bool allToolbarsHidden() const;
  /** Reads the tag files and the description.rc from tagDir in order to build up the internal DTD and tag structures. */
  void readTagDir(QString &dirName);
  /** No descriptions */
  virtual void focusInEvent(QFocusEvent*);
  void saveOptions();
  void readOptions();

  void initActions();
  void initStatusBar();
  void initDocument();
  void initView();
  void initProject();
  void initTagDict();

  /** Reads the tags for the tag files. Returns the number of read tags. */
  uint readTagFile(const QString& fileName, DTDStruct* parentDTD, QTagList *tagList);
  /** Parse the dom document and retrieve the tag attributes */
  void setAttributes(QDomNode *dom, QTag *tag);
  virtual bool queryClose();
  void saveAsTemplate (bool projectTemplate, bool selectionOnly = false);
  /** Saves a toolbar as local or project specific. */
  void saveToolbar(bool localToolbar = true, const QString& toolbarToSave = QString::null);
  /** Saves the toolbar and the actions. Returns the name of the actions file*/
  KURL saveToolbarToFile(const QString& toolbarName,const KURL& destFile);
  /** Show the toolbar which is in url. If it was not loaded yet, it loads the
      toolbar from the file */
  void showToolbarFile(const KURL &url);
  /** Initialize the plugin architecture. */
  void initPlugins();
  /** Loads the toolbars for dtd named dtdName and unload the ones belonging to oldDtdName. */
  void loadToolbarForDTD(const QString& dtdName);
  void setTitle(const QString&);
  void connectDockSignals(QObject *obj);

private:
  /** Messaage output window */
  MessageOutput *messageOutput;

  QPopupMenu *m_pluginMenu;
  QPopupMenu *m_tagsMenu;

  /** project class */
  Project *m_project;
  // config
  KConfig *m_config;

  /** widget stack for left panel */
  QWidgetStack *rightWidgetStack;
  QWidgetStack *bottomWidgetStack;

  /** tabs for left panel */
  ProjectTreeView *pTab;
  DocTreeView *dTab;
  FilesTreeView *fTab;
  TemplatesTreeView *tTab;
  StructTreeView *sTab;
  TagAttributeTree *aTab;

  KDockWidget *leftdock;
  KDockWidget *maindock;
  KDockWidget *bottdock;
  KDockWidget *ptabdock;
  KDockWidget *ttabdock;
  KDockWidget *dtabdock;
  KDockWidget *ftabdock;
  KDockWidget *stabdock;
  KDockWidget *atabdock;

   /** HTML class for preview */
  WHTMLPart *htmlpart;
  WHTMLPart *htmlPartDoc;

  /** KafkaHTMLPart widget */
  WKafkaPart *kafkaPart;

  /** Grep window */
  GrepDialog *grepDialog;

  // DOC & VIEW
  QuantaDoc  *m_doc;
  QuantaView *m_view;

  bool exitingFlag;

  /** parsered tree of document  */
  QTimer *statusbarTimer;

  // debugger
  int phpDebugPort;
  PHP3Debugger* dbg3;
  PHP4Debugger* dbg4;
  QString debuggerStyle;

  // ACTIONS
  KRecentFilesAction *projectToolbarFiles;

  KToggleAction *showMessagesAction, *showTreeAction,
    *showFTabAction,*showPTabAction,*showTTabAction,*showSTabAction,*showATabAction,
    *showDTabAction, *showToolbarAction,*showStatusbarAction,*showPreviewAction,
    *showKafkaAction;
  KToggleAction *showDTDToolbar;

  KSelectAction *setEndOfLine;

  KAction *saveAction, *saveAllAction, *undoAction, *redoAction,
    *saveprjAction, *closeprjAction, *insertFileAction, *insertDirAction,
    *newPrjAction, *openPrjAction,  *uploadProjectAction,  *rescanPrjDirAction,
    *projectOptionAction, *saveAsLocalTemplateAction, *saveAsProjectTemplateAction,
    *saveSelectionAsLocalTemplateAction, *saveSelectionAsProjectTemplateAction,
    *openPrjViewAction, *savePrjViewAction, *saveAsPrjViewAction;

  KAction *editUndo, *editRedo, *bookmarkToggle, *bookmarkClear;
  KAction *editTagAction;

  //KToggleAction *viewFoldingMarkers;
  Kate::ActionMenu *setHighlight;

  QPopupMenu* pm_set;
  QPopupMenu* pm_bookmark;

  QDomDocument* m_actions;

  QPtrList<KTextEditor::Mark> markList;

  int currentPageIndex;
  uint userToolbarsCount;
  bool previewCopyMade;
  KTempFile *previewTmpFile;

protected: // Protected attributes
  /** Timer to refresh the structure tree. */
  QTimer *refreshTimer;
  QString scriptBeginRxStr;
  QString scriptEndRxStr;
  /** The toolbars for this DTD are currently shown to the user. */
  QString currentToolbarDTD;
  SpellChecker *m_spellChecker;
  KDockWidget *m_oldTreeViewWidget;
  /** The ids of the widgets visible before doing the preview/documentation browsing */
  QValueList<int> previousWidgetList;

  /* Store the old shortcuts from the local quantaui.rc */
  QMap<QString, QString> oldShortcuts;
  KURL urlUnderCursor;

public: //TODO: check if it's worth to make a read method for them
  QPtrList<KTempFile> tempFileList;
  QDict<ToolbarEntry> toolbarList;
  KToggleAction *viewBorder;
  KToggleAction *viewLineNumbers;
  KToggleAction *viewDynamicWordWrap;
  KRecentFilesAction *fileRecent;
  /** plugin classes */
  QuantaPluginInterface *m_pluginInterface;
  /** True when the whole quanta is initalized. */
  bool quantaStarted;
};

#endif // QUANTA_H
