/***************************************************************************
                          quanta.h  -  description
                             -------------------
    begin                : ?? ???  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2004 by Andras Mantia <amantia@kde.org>
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

#define QUANTA_PACKAGE "quanta"
#define QUANTA_VERSION "3.2.90 (Beta1, 2004-06-23)"

#define IDS_STATUS      1
#define IDS_INS_OVR     2
#define IDS_MODIFIED    3
#define IDS_STATUS_CLM  4
#define IDS_DEFAULT     "Ready."

// include files for Qt
#include <qmap.h>
#include <qdict.h>
#include <qvaluelist.h>
#include <qstrlist.h>
#include <qptrlist.h>

// include files for KDE
#include <kdeversion.h>
#include <kdockwidget.h>
#include <kparts/browserextension.h>
#include <kparts/dockmainwindow.h>

#include <kmdimainfrm.h>

//app includes
#include "dcopwindowmanagerif.h"

// forward declaration
class DCOPSettings;
class DCOPQuanta;

class QuantaPluginInterface;

class QuantaDoc;
class QuantaView;
class QNewDTEPStuff;

class QTabWidget;
class QWidgetStack;
class QListViewItem;

class QPopupMenu;

class WHTMLPart;
class KHTMLView;

class KAction;
class KToggleAction;
class KProcess;
class KSelectAction;
class KRecentFilesAction;
class KToolBarPoupAction;

class Node;
class Parser;
class DocTreeView;
class FilesTreeView;
class ScriptTreeView;
class EnhancedTagAttributeTree;
class Project;
class MessageOutput;
class QDomDocument;
class Document;
class DebuggerManager;
class QuantaInit;
class KToolBarPopupAction;
class KTempFile;
class KParts::Part;
class KParts::PartManager;
namespace KTextEditor
{
  class Mark;
  class View;
}

class SpellChecker;
struct DirInfo;

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
class QuantaApp : public KMdiMainFrm, public DCOPWindowManagerIf
{
  Q_OBJECT

friend class QuantaInit;

public:
  QuantaApp(int mdiMode);
  ~QuantaApp();

  QuantaDoc  *doc() const {return m_doc; }
  QPopupMenu *tagsMenu() const {return m_tagsMenu;}
  KConfig *config() const {return m_config;}

//TODO: check if we really need these "get" methods (and get rid o get)
  MessageOutput *messageOutput() const {return m_messageOutput;}
  MessageOutput *problemOutput() const {return m_problemOutput;}

  DebuggerManager *debugger() const {return m_debugger;}
  KParts::PartManager *partManager() {return m_partManager;}

  QWidget* createContainer(QWidget *parent, int index, const QDomElement &element, int &id );
  void removeContainer(QWidget *container, QWidget *parent, QDomElement &element, int id );
  /** Reads the DTD info from the file, tries to find the correct DTD and builds the tag/attribute list from the DTD file. */
  void processDTD(const QString& documentType = QString::null);
/** Returns the project's base URL if it exists, the HOME dir if there is no project and no opened document (or the current opened document was not saved yet), and the base URL of the opened document, if it is saved somewhere.

  maps to the same function in Project*/
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
  QString saveCurrentFile();
  QStringList tagAreas(const QString& name, bool includeCoordinates, bool skipFoundContent) const;
  QString documentFolderForURL(QString url);
  QString urlWithPreviewPrefix(QString url);
  /** Capture DCOP signals from KXsldbgPart or similar plugin */
  void newCursorPosition(QString file, int lineNumber, int columnNumber);
  void newDebuggerPosition(QString file, int lineNumber);
  void openFile(QString file, int lineNumber, int columnNumber);

  /** reparse current document and initialize node. */
  void reparse(bool force);

  bool structTreeVisible() const;

  //return the old Cursor position
  void oldCursorPos(uint &line, uint &col) {line = oldCursorLine; col = oldCursorCol;}
  /** Loads the toolbars for dtd named dtdName and unload the ones belonging to oldDtdName. */
  void loadToolbarForDTD(const QString& dtdName);

  QStringList selectors(const QString& tag);
  QStringList idSelectors();
  WHTMLPart *documentationPart() {return m_htmlPartDoc;}
  /** Show the toolbar which is in url. If it was not loaded yet, it loads the
      toolbar from the file */
  void showToolbarFile(const KURL &url);
  /** Restores the original document from the temporary backup */
  void restoreFromTempfile(Document *w);

    /** tabs for left panel */
  DocTreeView *dTab;
  EnhancedTagAttributeTree *aTab;

/**
   * Show a TagDialog of Node tag, with attrs attr.
   * @param tag The name of the new Node to create.
   * @param attr The string containing the attrs of the new Node to create.
   * @return Returns a new Node created according to the contents of the TagDialog.
   */
  Node *showTagDialogAndReturnNode(const QString &tag, const QString &attr = QString::null);
  /** Returns the baseURL of the document. */
  KURL baseURL();

  ScriptTreeView *scriptToolView() {return scriptTab;}
  FilesTreeView *filesToolView() {return fTab;}
  bool enableIdleTimer(bool enable);

  /** Called when a document was closed. Resets some variables. */
  void slotFileClosed();

signals: // Signals
  /** signal used to hide the splash screen */
  void showSplash(bool);
  void debuggerAddWatch(const QString &);

public slots:
  void slotFileNew();
  void slotFileOpen();
  void slotFileOpen(const KURL &);
  void slotFileOpen( const KURL &, const QString& );
  void slotFileSave();
  bool slotFileSaveAs();
  void slotFileSaveAsLocalTemplate();
  void slotFileSaveAsProjectTemplate();
  void slotFileSaveSelectionAsLocalTemplate();
  void slotFileSaveSelectionAsProjectTemplate();
  void slotFileSaveAll();
  void slotFileReload(QuantaView *view = 0L);
  void slotFileReloadAll();
  void slotFileClose();
  /** Close the document specified in the parameter if it's opened */
  void slotFileClose(const KURL &url);
  void slotFileCloseAll();
  void slotFileQuit();

  void slotOpenFileInPreview(const KURL&);
  void slotImageOpen(const KURL&);
  void slotFileOpenRecent(const KURL&);
  void slotInsertTag(const KURL&, DirInfo);

  void slotEditFindInFiles();
  /// open url in documentation window
  void openDoc(const QString& url);

  void slotContextHelp();

  void slotBack();
  void slotForward();

  void slotViewToolBar();
  void slotViewStatusBar();

  void statusBarTimeout();
  /** Shows the message in the status bar.
       WARNING: Don't use in place where nothing should happen until the function
       exits (like in startup code, DTD reading, etc.) as it calls processEvents() and
       unexpected things may happen. */
  void slotStatusMsg(const QString &text);

  void slotNewStatus();
  void slotNewLineColumn();
//  void slotUpdateStatus(QWidget*);FIXME:

  /** repaint preview */
  void slotRepaintPreview();
  /** toggles showing the preview */
  void slotToggleShowPreview();
  /** Shows the preview widget and repaints the preview or
        hides the preview widget and restores the original document
  */
  void slotShowPreviewWidget(bool show);
  /** Called from the view manager when the status of the preview must change:
        - hide if the preview was in the editor area
        - update if the preview is in a toolview
   */
  void slotChangePreviewStatus();
  /** Called when the preview widget got or lost the focus */
  void slotPreviewHasFocus(bool focus);

  void slotShowMessagesView();
  void slotShowProblemsView();

  void slotContextMenuAboutToShow();

  /** options slots */
  void slotPreviewOptions();
  void slotOptions();
  void slotOptionsConfigureKeys();
  void slotOptionsConfigureToolbars();
  void slotNewToolbarConfig();
  /** Configure toolbars, show defaultToolbar by default */
  void slotConfigureToolbars(const QString& defaultToolbar = QString::null);
  void slotOptionsConfigureActions();

  void setCursorPosition(int row, int col );
  void gotoFileAndLine(const QString& filename, int line, int column);

  void selectArea(int line1, int col1, int line2, int col2);

  /** No descriptions */
  void slotInsertFile(const KURL&);
  /** No descriptions */
  void slotSyntaxCheckDone();
  /** Sends a toolbar in mail. */
  void slotSendToolbar();
  /** Removes a user toolbar from the toolbars. */
  bool slotRemoveToolbar();
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
  bool slotRemoveToolbar(const QString& name);
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
  /** Creates a script action for a_scriptURL using the a_interpreter as the script
  interpreter application */
  void slotAssignActionToScript(const KURL&a_scriptURL, const QString& a_interpreter);

  /** No descriptions */
  void slotChangeDTD();
  /** No descriptions */
  void slotShowCompletion();
  /** No descriptions */
  void slotShowCompletionHint();
  /** No descriptions */
  void slotParsingDTDChanged(const QString& newDTDName);
  /** No descriptions */
  void slotBuildPrjToolbarsMenu();

  void slotReparse();
  void slotForceReparse();

  void slotExpandAbbreviation();

  /** Show the Document Properties Dialog */
  void slotDocumentProperties();
  /** No descriptions */
  void slotAutosaveTimer();

  void slotHideSplash() {emit showSplash(false);}

  void slotConvertCase();

  /** Reload the tree of the StructTreeView */
  void slotReloadStructTreeView(bool groupOnly = false);

  void slotReportBug();
  /** registers a new part in the partmanager */
  void slotNewPart(KParts::Part *newPart, bool setActiv);

  void slotUploadFile(QuantaView *view=0L);
  void slotDeleteFile(QuantaView *view=0L);

  // Debugger
  void slotDebuggerAddWatch();

//Overridden KMdiMainFrm slots
  virtual void closeAllViews();
  virtual void closeActiveView();
  virtual void switchToToplevelMode();
  virtual void switchToChildframeMode();
  virtual void switchToIDEAlMode();
  virtual void switchToTabPageMode();

protected slots:
  void slotDockWidgetHasUndocked(KDockWidget *widget);
  void slotPreviewBeingClosed();
  /** No descriptions */
  void slotMakeDonation();
  /** No descriptions */
  void slotHelpHomepage();
  /** Show or hide the DTD toolbar */
  void slotToggleDTDToolbar(bool show);
  /** Loads a DTEP*/
  void slotLoadDTEP();
  /** Sends a DTEP in email */
  void slotEmailDTEP();
  /** Downloads a DTEP from the main server */
  void slotDownloadDTEP();
  /** Uploads a DTEP to the main server */
  void slotUploadDTEP();
  /** Shows tip of the day */
  void slotHelpTip();
  /** Show the user mailing list sign up */
  void slotHelpUserList();
  void slotOpenFileUnderCursor();
  void slotUploadOpenedFiles();
  /** Called after there was no user activity - cursor movement - for xx ms*/
  void slotIdleTimerExpired();
  void slotShowNoFramesPreview();
  /** Get script output */
  void slotGetScriptOutput(KProcess*, char*, int);
  /** Get script error */
  void slotGetScriptError(KProcess*, char*, int);
  /** Notify when process exits */
  void slotProcessExited(KProcess*);
  /** External app execution timeout handling */
  void slotProcessTimeout();

  /** connected to the part manager, activates a new part */
  void slotActivePartChanged(KParts::Part * );

  void slotTagMail();
  void slotTagQuickList();
  void slotTagEditTable();
  void slotTagColor();
  void slotTagDate();
  void slotTagSelect();
  /** Add the starting and closing text for a
  user specified tag. */
  void slotTagMisc();
  void slotEditCurrentTag();
  void slotSelectTagArea();
  void slotSelectTagArea(Node *node);

  void slotInsertCSS();
  void slotFrameWizard();
  void slotViewInKFM();
  void slotViewInLynx();

  void slotPasteHTMLQuoted();
  void slotPasteURLEncoded();
  void slotInsertChar();

//Edit
  void slotUndo ();
  void slotRedo ();
  void slotCut();
  void slotCopy();
  void slotPaste();

//Tools
  void slotSpellcheck ();

  void slotShowSourceEditor();
  void slotShowVPLAndSourceEditor();
  void slotShowVPLOnly();
  void slotTabDragged(QWidget *widget);

protected:
  /** Ask for save all the modified user toolbars. */
  bool removeToolbars();
  /** Returns true if all toolbars are hidden, false otherwise. */
  bool allToolbarsHidden() const;
  /** No descriptions */
  virtual void focusInEvent(QFocusEvent*);
  void saveOptions();

  virtual bool queryClose();
  void saveAsTemplate (bool projectTemplate, bool selectionOnly = false);
  /** Saves a toolbar as local or project specific. */
  bool saveToolbar(bool localToolbar = true, const QString& toolbarToSave = QString::null, const KURL& destURL = KURL());
  /** Saves the toolbar and the actions. Returns the name of the actions file*/
  KURL saveToolbarToFile(const QString& toolbarName,const KURL& destFile);
  void setTitle(const QString&);
  /** Updates the structure and attribute treeview. */
  void updateTreeViews();
  /** Makes the tabwidget look and behave like we want. If closeButtonsOnly is true,
  only the close button behavior is changed. */
  void initTabWidget(bool closeButtonsOnly = false);

  ScriptTreeView *scriptTab;

private:
  FilesTreeView *fTab;
  ToolbarEntry *toolbarByURL(const KURL& url);
  /** Message output window */
  MessageOutput *m_messageOutput;
  MessageOutput *m_problemOutput;
  KMdiToolViewAccessor* m_messageOutputView;
  KMdiToolViewAccessor* m_problemsOutputView;
  KMdiToolViewAccessor* m_previewToolView;
  KMdiToolViewAccessor* m_documentationToolView;
  Document *m_previewedDocument;

  // Debugger
  DebuggerManager *m_debugger;
  QString popupWord;

  QuantaPluginInterface *m_pluginInterface;

  QPopupMenu *m_tagsMenu;

  // config
  KConfig *m_config;

   /** HTML class for preview */
  WHTMLPart *m_htmlPart;
  WHTMLPart *m_htmlPartDoc;

  // DOC & VIEW
  QuantaDoc  *m_doc;

  bool exitingFlag;

  /** parsered tree of document  */
  QTimer *statusbarTimer;

  // ACTIONS
  KRecentFilesAction *projectToolbarFiles;

  KToggleAction *showStatusbarAction, *showKafkaAction, *showDTDToolbar;
  KToolBarPopupAction *showPreviewAction;

  KAction *saveAction, *saveAllAction;

  KAction *editTagAction, *selectTagAreaAction;

  QDomDocument* m_actions;

  QPtrList<KTextEditor::Mark> markList;

  int currentPageIndex;
  uint userToolbarsCount;
  bool previewCopyMade;
  KTempFile *previewTmpFile;

  uint cursorLine;
  uint cursorCol;
  uint oldCursorLine;
  uint oldCursorCol;
  bool m_previewVisible;
  bool m_noFramesPreview;

  KProcess* m_execCommandPS;
  QString m_scriptOutput;
  // remember the old actions for the context menu
  KAction *m_oldContextCut, *m_oldContextCopy, *m_oldContextPaste;

protected: // Protected attributes
  /** Timer to refresh the structure tree. */
  QTimer *refreshTimer;
  /** Timer to detect idle periods. Every time the cursor moves the timer is
  restarted.*/
  QTimer *idleTimer;
  /** The toolbars for this DTD are currently shown to the user. */
  QString currentToolbarDTD;
  KDockWidget *m_oldTreeViewWidget;
  /** The ids of the widgets visible before doing the preview/documentation browsing */
  QValueList<int> previousWidgetList;

  /* Store the old shortcuts from the local quantaui.rc */
  QMap<QString, QString> oldShortcuts;
  KURL urlUnderCursor;
  QTimer *autosaveTimer;
  DCOPSettings *dcopSettings;
  DCOPQuanta *dcopQuanta;
  KParts::PartManager *m_partManager;  ///< the pointer to the part manager
  QGuardedPtr<KTextEditor::View> m_oldKTextEditor;  ///< remembers the last activated GUI
  int m_cvsMenuId;
  QNewDTEPStuff *m_newDTEPStuff;

public: //TODO: check if it's worth to make a read method for them
  QDict<ToolbarEntry> toolbarList;
  KRecentFilesAction *fileRecent;
  /** True when the whole quanta is initialized. */
  bool quantaStarted;
  bool m_loopStarted; //true if an internal event loop has been started
  bool m_idleTimerEnabled;
  QuantaInit *m_quantaInit;  ///< the pointer to all the init stuff
};

#endif // QUANTA_H
