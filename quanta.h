/***************************************************************************
                          quanta.h  -  description
                             -------------------
    begin                : ?? ???  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    								  (C) 2001 by Andras Mantia
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
#include <qstrlist.h>
#include <qptrlist.h>

// include files for KDE 
#include <kapp.h>
#include <kdockwidget.h>
#include "kqapp.h"
#include "widgets/whtmlpart.h"
#include <kparts/browserextension.h>
#include <ktoolbar.h>
#include <kate/document.h>
#include <ktempfile.h>

//app includes
#include "parser/qtag.h"
#include "treeviews/filestreeview.h"
#include "quantacommon.h"
#include "qextfileinfo.h"
#include "resource.h"


// forward declaration
class QuantaDoc;
class QuantaView;

class QTabWidget;
class QWidgetStack;
class QListViewItem;

class WHTMLPart;
class KHTMLView;

class KAction;
class KToggleAction;
class KSelectAction;
class KRecentFilesAction;

class Node;
class Parser;
class DocTreeView;
class FilesListView;
class FilesTreeView;
class StructTreeView;
class ProjectTreeView;
class TemplatesTreeView;
class Project;
class GrepDialog;
class MessageOutput;
class QDomDocument;
class ActionEditDlg;
class Document;

class PHP3Debugger;
class PHP4Debugger;

/**
  * The base class for Quanta application windows. 
  */
class QuantaApp : public KDockMainWindow
{
  Q_OBJECT

friend class QuantaDoc;
friend class QuantaView;
friend class ActionEditDlg;

public:
  QuantaApp();
  ~QuantaApp();

  QuantaDoc  *getDoc()   { return doc; };
  QuantaView *getView()  { return view;};
  Project *getProject()  { return project; };
  QDomDocument *actions() { return m_actions; }

  MessageOutput *getMessages() { return messageOutput; }

  /** Loads the initial project */
  void loadInitialProject(QString url="");


  /** from @see KXMLGUIBuilder, for toolbars in tabbar ( tags,... )
   *
   */
  QWidget* createContainer( QWidget *parent, int index, const QDomElement &element, int &id );
  void removeContainer( QWidget *container, QWidget *parent, QDomElement &element, int id );
  /** Reads the DTD info from the file, tries to find the correct DTD and builds the tag/attribute list from the DTD file. */
  void processDTD(QString documentType = QString::null);

public slots:

  void slotFileNew ();
  void slotFileOpen();
  void slotFileOpen( const KURL & );
  void slotFileSave    ();
  void slotFileSaveAs  ();
  void slotFileSaveAsLocalTemplate ();
  void slotFileSaveAsProjectTemplate ();
  void slotFileSaveSelectionAsLocalTemplate ();
  void slotFileSaveSelectionAsProjectTemplate ();
  void slotFileSaveAll ();
  void slotFileClose   ();
  void slotFileCloseAll();
  void slotFilePrint   ();
  void slotFileNext    ();
	void slotFilePrev    ();
  void slotFileQuit    ();

  void slotImageOpen        (QString);
  void slotFileOpenRecent   (const KURL&);
  void slotInsertTag        (QString, DirInfo);

  void slotEditFindInFiles   ();

  void slotToolSyntaxCheck();

  void openLastFiles();
  /// open url in documentation window
  void openDoc( QString url );

  void updateNavButtons( bool back, bool forward );

  void contextHelp();

  void slotBack();
  void slotForward();

  void slotViewToolBar  ();
  void slotViewStatusBar();

  void statusBarTimeout ();
  void slotStatusMsg    (const QString &text);

  void slotNewUndo      ();
  void slotNewStatus    ();
  void slotNewLineColumn();
  //void slotUpdateStatus (const QString &);
  void slotUpdateStatus (QWidget*);

  void slotDockChanged();
  void slotSwapLeftPanelMode();

  void slotActivatePreview();

  /** show preview ( F6 )*/
  void slotShowPreview();

  void slotShowProjectTree();
  void slotShowTemplatesTree();

  /** Repaint preview ( slot ) */
  void slotViewRepaint();

  void slotShowLeftDock();
  void slotShowBottDock();
  void slotShowFTabDock();
  void slotShowPTabDock();
  void slotShowTTabDock();
  void slotShowSTabDock();
  void slotShowDTabDock();

  void viewMenuAboutToShow();
  void settingsMenuAboutToShow();
  void setEOLMenuAboutToShow();
  void bookmarkMenuAboutToShow();
  void gotoBookmark (int n);

  void slotMessageWidgetEnable();
  void slotMessageWidgetDisable();

  /** repaint preview */
  void repaintPreview( bool clear = false);

  /** options slots */
  void slotOptions();
  void slotOptionsConfigureKeys();
  void slotOptionsConfigureToolbars();
  void slotOptionsConfigureActions();

  void enablePhp3Debug(bool);
  void enablePhp4Debug(bool);

  /** reparse current document and initialize node. */
  void reparse();
	
  void setCursorPosition( int row, int col );
  void gotoFileAndLine  ( QString filename, int line );

  void selectArea(int line1, int col1, int line2, int col2);
	
  void setTitle(QString);
	
  void autoComplete();
	
  void slotShowOpenFileList();
  /** No descriptions */
  void slotNewProjectLoaded();
  /** No descriptions */
  void slotInsertFile(QString);
  /** No descriptions */
  void slotSyntaxCheckDone();
  /** Sends a toolbar in mail. */
  void slotSendToolbar();
  /** Removes a user toolbar from the toolbars. */
  void slotRemoveToolbar();
  /** Adds a new, empty toolbar. */
  void slotAddToolbar();
  /** Saves a toolbar as local or project specific. */
  void slotSaveToolbar(bool localToolbar = true, QString toolbarToSave = QString::null);
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
  /** No descriptions */
  void slotToolsChangeDTD();
  /** No descriptions */
  void slotShowCompletion();
  /** No descriptions */
  void slotShowCompletionHint();

protected:
  KParts::BrowserExtension *browserExtension()
  {
      return static_cast<KParts::BrowserExtension *>(((KParts::ReadOnlyPart *)htmlPart())->child( 0L, "KParts::BrowserExtension" ));
  }
  /** Ask for save all the modified user toolbars. */
  void saveModifiedToolbars();
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
  void initToolBars();

  /** Parse the dom document and retrieve the tag attributes */
  void setAttributes(QDomDocument *dom, QTag *tag);
  virtual bool queryExit();
  void saveAsTemplate (bool projectTemplate, bool selectionOnly = false);
  /* Saves the toolbar and the actions. Returns the name of the actions file*/
  QString saveToolBar(QString& toolbarName, QString destFile);

private:
  /** project class */
  Project *project;

  WHTMLPart    *htmlPart();
  QWidgetStack *widgetStackOfHtmlPart();

  // config
  KConfig *config;

  /** widget stack for left panel */
  QWidgetStack *fTab;

  /** tabs for left panel */
  ProjectTreeView  	*pTab;
  DocTreeView 	*dTab;
  FilesTreeView	*fTTab;
  FilesListView		*fLTab;
  StructTreeView *sTab;
  TemplatesTreeView	 *tTab;

  QString previewPosition;

  QWidgetStack *rightWidgetStack;

  KDockWidget *leftdock;
  KDockWidget *maindock;
  KDockWidget *bottdock;
  KDockWidget *ptabdock;
  KDockWidget *ttabdock;
  KDockWidget *dtabdock;
  KDockWidget *ftabdock;
  KDockWidget *stabdock;

 	/** HTML class for preview */
  WHTMLPart *htmlpart;
  WHTMLPart *htmlPartDoc;

  /** Messaage output window */
  MessageOutput *messageOutput;

  /** Grep window */
  GrepDialog *grepDialog;

  // DOC & VIEW
  QuantaDoc  *doc;
  QuantaView *view;

  bool exitingFlag;

  /** parsered tree of document  */

  QTimer *statusbarTimer;

  // debugger
  int phpDebugPort;
  PHP3Debugger* dbg3;
  PHP4Debugger* dbg4;
  QString debuggerStyle;

  // ACTIONS
  KRecentFilesAction *fileRecent;
  KRecentFilesAction *projectToolbarFiles;

  KToggleAction *showMessagesAction, *showTreeAction,
    *showFTabAction,*showPTabAction,*showTTabAction,*showSTabAction,*showDTabAction,
    *showToolbarAction,*showStatusbarAction,*showPreviewAction;

  KSelectAction *setEndOfLine;

  KAction *saveAction, *saveAllAction, *undoAction, *redoAction,
    *backAction, *forwardAction,
    *saveprjAction, *closeprjAction, *insertFileAction, *insertDirAction,
    *newPrjAction, *openPrjAction, *rescanPrjDirAction, *uploadProjectAction,
    *projectOptionAction, *saveAsLocalTemplateAction, *saveAsProjectTemplateAction,
    *saveSelectionAsLocalTemplateAction, *saveSelectionAsProjectTemplateAction;

  KAction *editUndo, *editRedo, *bookmarkToggle, *bookmarkClear;

  KToggleAction *viewBorder;
  KToggleAction *viewLineNumbers;
  Kate::ActionMenu *setHighlight;

  QPopupMenu* pm_set;
  QPopupMenu* pm_bookmark;

  QDomDocument *m_actions;

  QPtrList<KTextEditor::Mark> markList;
  QPtrList<KTempFile> tempFileList;
  QDict<KXMLGUIClient> toolbarGUIClientList;
  QDict<QDomDocument> toolbarDomList;

  uint userToolbarsCount;
  /**  */
  bool previewCopyMade;
  /**  */
  KTempFile *previewTmpFile;

signals: // Signals
  /** The tree views should be updated due to some changes on the disk. */
  void reloadTreeviews();
};
 
#endif // QUANTA_H
