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
#define QUANTA_VERSION "3.5.10"

#define IDS_STATUS      1
#define IDS_INS_OVR     2
#define IDS_MODIFIED    3
#define IDS_STATUS_CLM  4
// Number 5 is used by the debugger, but not added until debugger is activated
#define IDS_STATUS_DEBUGGER  5

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
class QNewToolbarStuff;
class QNewTemplateStuff;
class QNewScriptStuff;
class QNewDocStuff;

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
class KTextEdit;

class TagAction;
class Node;
class Parser;
class DocTreeView;
class FilesTreeView;
class ScriptTreeView;
class EnhancedTagAttributeTree;
class Project;
class AnnotationOutput;
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

struct DirInfo;
struct EventAction;
struct ToolbarEntry;


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
  AnnotationOutput *annotationOutput() const {return m_annotationOutput;}

  DebuggerManager *debugger() const {return m_debugger;}
  KParts::PartManager *partManager() {return m_partManager;}

  QWidget* createContainer(QWidget *parent, int index, const QDomElement &element, int &id );
  void removeContainer(QWidget *container, QWidget *parent, QDomElement &element, int id );
/** Returns the project's base URL if it exists, the HOME dir if there is no project and no opened document (or the current opened document was not saved yet), and the base URL of the opened document, if it is saved somewhere.

  maps to the same function in Project*/
  KURL projectBaseURL() const;

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
  /**
   * Sets the DTEP for the current document.
   * @param dtepName the name (nickname or full name) of the DTEP
   * @param convert  if true, converts the !DOCTYPE line to the new DTEP
   */
  void setDtep(const QString& dtepName, bool convert);
  QStringList tagAreas(const QString& name, bool includeCoordinates, bool skipFoundContent) const;
  QString documentFolderForURL(const QString &url);
  QString urlWithPreviewPrefix(const QString &url);
  void addFileToProject(const QString &url);
  void addFolderToProject(const QString &url);
  void uploadURL(const QString &url, const QString& profile, bool markOnly);  /** Capture DCOP signals from KXsldbgPart or similar plugin */
  void newCursorPosition(const QString &file, int lineNumber, int columnNumber);
  void newDebuggerPosition(const QString &file, int lineNumber);
  void openFile(const QString &file, int lineNumber, int columnNumber);

  /** reparse current document and initialize node. */
  void reparse(bool force);

  bool structTreeVisible() const;
  bool previewVisible() const {return m_previewVisible;}

  //return the old Cursor position
  void oldCursorPos(uint &line, uint &col) {line = oldCursorLine; col = oldCursorCol;}

  QStringList selectors(const QString& tag);
  QStringList idSelectors();
  WHTMLPart *documentationPart() {return m_htmlPartDoc;}
  /** Show the toolbar which is in url. If it was not loaded yet, it loads the
      toolbar from the file */
  void showToolbarFile(const KURL &url);

  void setParserEnabled(bool enabled) {m_parserEnabled = enabled;}

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

  void startIdleTimer();

  /** Called when a document was closed. Resets some variables. */
  void slotFileClosed(Document *w);
  void setTabToolTip(QWidget *w, const QString& toolTipStr);

  void createPreviewPart();
  void createDocPart();

  /**
   * This is a pool for actions waiting for being applied (VPL only yet).
   * For example, whee a user presses the bold action, it isn't immediately applied on the document.
   * If the user presses a key right away the character is inserted inside the tags for the queued actions.
   * If the user changes the place of the cursor, the actions waiting for being inserted are removed.
   */
  QStringList const& tagActionPool() const {return m_tagActionPool;}
  void insertTagActionPoolItem(QString const& action_item);
  void removeTagActionPoolItem(QString const& action_item);
  void removeAllTagActionPoolItems() {m_tagActionPool.clear();}
  /** Updates the structure and attribute treeview. */
  void updateTreeViews();
  void setTitle(const QString&);

  QPtrList<TagAction> const& tagActions() const {return m_tagActions;}

  /** Clicked word or selected text for context sensitive menu in editor */
  QString popupWord;

signals: // Signals
  /** signal used to hide the splash screen */
  void showSplash(bool);
  // trees need reload because hidden files option changed
  void reloadAllTrees();

  /** Emitted when some kind of event that can have associated actions has happened. */
  void eventHappened(const QString&, const QString&, const QString& );

  void toolbarRemoved(const QString&);

  void showMessage(const QString&, bool);
  void clearMessages();

public slots:
  void slotFileNew();
  void slotFileOpen();
  void slotFileOpen(const KURL &url);
  void slotFileOpen(const KURL &url, const QString &encoding);
  void slotFileOpen(const KURL &url, const QString &encoding, bool readOnly);
  void slotFileOpen(const KURL::List &urls, const QString& encoding);
  void slotFileSave();
  bool slotFileSaveAs(QuantaView *viewToSave = 0L);
  void slotFileSaveAsLocalTemplate();
  void slotFileSaveAsProjectTemplate();
  void slotFileSaveSelectionAsLocalTemplate();
  void slotFileSaveSelectionAsProjectTemplate();
  void slotFileSaveAll();
  void slotFileReload(QuantaView *view = 0L);
  void slotFileReloadAll();
  /** Close the document specified in the parameter if it's opened */
  void slotFileClose(const KURL &url);
  void slotFileCloseAll();
  void slotFileQuit();

  void slotOpenFileInPreview(const KURL &url);
  void slotImageOpen(const KURL &url);
  void slotFileOpenRecent(const KURL &url);
  void slotInsertTag(const KURL&, DirInfo);

  void slotEditFindInFiles();
  /// open url in documentation window
  void openDoc(const QString& url);

  void slotContextHelp();

  void slotBack();
  void slotForward();

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
  void slotShowAnnotationView();

  void slotContextMenuAboutToShow();

  /** options slots */
  void slotShowMenuBar();
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

  /** Insert the content of the url into the current document. */
  void slotInsertFile(const KURL& url);
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
  /** Loads the toolbars for dtd named dtdName and unload the ones belonging to oldDtdName. */
  void slotLoadToolbarForDTD(const QString& dtdName);
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
  /** Creates a new, empty action */
  void slotNewAction();
  /** Creates a script action for a_scriptURL using the a_interpreter as the script
  interpreter application */
  void slotAssignActionToScript(const KURL&a_scriptURL, const QString& a_interpreter);

  /** Change the DTD/DTEP of the current document. */
  void slotChangeDTD();
  /** Edit a DTD/DTEP description. */
  void slotEditDTD();
  /** Called when the user explicetly requested the completion box.*/
  void slotShowCompletion();
  /** Called when the user explicetly requested the completion hint.*/
  void slotShowCompletionHint();
  /** Show or hide the groups for dtepName in the structure tree.
    The special value of "clear" for dtepName means show groups
    for all DTEPs found in the document.*/
  void slotShowGroupsForDTEP(const QString& dtepName, bool show);
  /** Build the project specific toolbar menu. */
  void slotBuildPrjToolbarsMenu();

  void slotReparse();
  void slotForceReparse();

  void slotExpandAbbreviation();

  /** Format the code of the document */
  void slotCodeFormatting();
  /** Show the Document Properties Dialog */
  void slotDocumentProperties();
  /** Show the Document Properties Dialog
   * If forceInsertionOfMinimalTree and the user haven't modified something in the dialog,
   * it will anyway insert a Minimal tree (HTML, BODY, ...)
   */
  void documentProperties(bool forceInsertionOfMinimalTree = false);
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

  /** Called when the CVS command working on files was executed successfully. */
  void slotCVSCommandExecuted(const QString &command, const QStringList &files);

  /** Called when the preview or documentation part is deleted. */
  void slotHTMLPartDeleted(QObject *object);

  void slotRefreshActiveWindow();

  bool slotEnableIdleTimer(bool enable);

//Overridden KMdiMainFrm slots
  virtual void closeAllViews();
  virtual void closeActiveView();
  virtual void switchToToplevelMode();
  virtual void switchToChildframeMode();
  virtual void switchToIDEAlMode();
  virtual void switchToTabPageMode();
  /** appends all visible user toolbar urls to the list */
  void slotGetUserToolbarFiles(KURL::List *list);

  // Get saved position of dock
  KDockWidget::DockPosition prevDockPosition(QWidget* dock, KDockWidget::DockPosition def);

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
  /** Downloads a toolbar from the main server */
  void slotDownloadToolbar();
  /** Uploads a toolbar to the main server */
  void slotUploadToolbar();
  /** Enable/Disable Smart Tag Insertion */
  void slotSmartTagInsertion();
  /** Downloads a template from the main server */
  void slotDownloadTemplate();
  /** Uploads a template to the main server */
  void slotUploadTemplate(const QString &fileName);
  /** Downloads a script from the main server */
  void slotDownloadScript();
  /** Uploads a script to the main server */
  void slotUploadScript(const QString &fileName);
  /** Downloads a documentation from the main server */
  void slotDownloadDoc();
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

  void slotShowSourceEditor();
  void slotShowVPLAndSourceEditor();
  void slotShowVPLOnly();
  void slotTabDragged(QWidget *widget);
  void slotTabMoved(int from, int to);
  void slotTabAboutToMove(int from, int to);
  void slotAnnotate();

protected:
  /** Create a DTEP tarball which can be uploaded or sent in email. Returns
  *   the name of the created file or QString::null if creation has failed.
  */
  QString createDTEPTarball();

  /** Create a toolbar tarball which can be uploaded or sent in email. Returns
  *   the name of the created file or QString::null if creation has failed.
  */
  QString createToolbarTarball();

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
  /** Makes the tabwidget look and behave like we want. If closeButtonsOnly is true,
  only the close button behavior is changed. */
  void initTabWidget(bool closeButtonsOnly = false);

  void dropEvent(QDropEvent *ev);
  void dragEnterEvent ( QDragEnterEvent * );

  void resetDockLayout();

  ScriptTreeView *scriptTab;

private:
  FilesTreeView *fTab;
  ToolbarEntry *toolbarByURL(const KURL& url);
  /** Message output window */
  MessageOutput *m_messageOutput;
  MessageOutput *m_problemOutput;
  AnnotationOutput *m_annotationOutput;
  KMdiToolViewAccessor* m_messageOutputView;
  KMdiToolViewAccessor* m_problemsOutputView;
  KMdiToolViewAccessor* m_annotationOutputView;
  KMdiToolViewAccessor* m_previewToolView;
  KMdiToolViewAccessor* m_documentationToolView;
  Document *m_previewedDocument;

  // Debugger
  DebuggerManager *m_debugger;

  QuantaPluginInterface *m_pluginInterface;

  QPopupMenu *m_tagsMenu;

  // config
  KConfig *m_config;

   /** HTML class for preview */
  WHTMLPart *m_htmlPart;
  WHTMLPart *m_htmlPartDoc;

  // DOC & VIEW
  QuantaDoc  *m_doc;

  /** parsered tree of document  */
  QTimer *statusbarTimer;

  // ACTIONS
  KRecentFilesAction *projectToolbarFiles;

  KToggleAction *showVPLAction, *showSourceAction, *showVPLSourceAction, *showDTDToolbar;
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
  bool m_parserEnabled; ///< enables/disables reparsing. If false, even a forced reparse is ignored (used when opening multiple files)

  QString m_scriptOutput;

  QStringList m_tagActionPool;
  QPtrList<TagAction> m_tagActions;

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
  QNewDTEPStuff *m_newDTEPStuff;
  QNewToolbarStuff *m_newToolbarStuff;
  QNewTemplateStuff *m_newTemplateStuff;
  QNewScriptStuff *m_newScriptStuff;
  QNewDocStuff *m_newDocStuff;
  QDict<ToolbarEntry> m_toolbarList;

public: //TODO: check if it's worth to make a read method for them
  KRecentFilesAction *fileRecent;
  /** True when the whole quanta is initialized. */
  bool quantaStarted;
  bool m_loopStarted; //true if an internal event loop has been started
  bool m_idleTimerEnabled;
  QuantaInit *m_quantaInit;  ///< the pointer to all the init stuff
};

#endif // QUANTA_H
