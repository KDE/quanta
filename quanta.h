/***************************************************************************
                          quanta.h  -  description
                             -------------------
    begin                : ��� ���  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
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
 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// include files for Qt
#include <qstrlist.h>

// include files for KDE 
#include <kapp.h>
#include <ktmainwindow.h>
#include <kaccel.h>
#include "widgets/whtmlpart.h"
#include <kparts/browserextension.h>

// forward declaration of the Quanta classes
class QuantaDoc;
class QuantaView;

class WSplitter;
class QTabWidget;
class QWidgetStack;
class WHTMLPart;
class KHTMLView;
class QListViewItem;
class QListBox;

class Parser;
class Node;
class DocTreeView;
class FilesListView;
class FilesTreeView;
class StructTreeView;
class ProjectTreeView;
class Project;
class ToolBars;
class GrepDialog;
class FtpClient;
class MessageOutput;

/**
  * The base class for Quanta application windows. It sets up the main
  * window and reads the config file as well as providing a menubar, toolbar
  * and statusbar. An instance of QuantaView creates your center view, which is connected
  * to the window's Doc object.
  * QuantaApp reimplements the methods that KTMainWindow provides for main window handling and supports
  * full session management as well as keyboard accelerator configuration by using KAccel.
  * @see KTMainWindow
  * @see KApplication
  * @see KConfig
  * @see KAccel
  *
  * @author Source Framework Automatically Generated by KDevelop, (c) The KDevelop Team.
  * @version KDevelop version 0.4 code generation
  */
class QuantaApp : public KTMainWindow
{
  Q_OBJECT

  friend class QuantaDoc;
  friend class QuantaView;

  public:
    /** construtor of QuantaApp, calls all init functions to create the application.
     * @see initMenuBar initToolBar
     */
    QuantaApp();
    ~QuantaApp();
    /** enables menuentries/toolbar items
     */
    void enableCommand(int id_);
    /** disables menuentries/toolbar items
     */
    void disableCommand(int id_);
    /** check button/item in stat
     */
		void checkCommand(int id_, bool stat);
    /** add a opened file to the recent file list and update recent_file_menu
     */
    void addRecentFile(const QString &file);
    /** opens a file specified by commandline option
     */
    void openDocumentFile(const char *_cmdl=0);
    /** returns a pointer to the current document connected to the KTMainWindow instance and is used by
     * the View class to access the document object's methods
     */	
    QuantaDoc *getDocument() const;
    /** members of app */
    QuantaDoc *getDoc() { return doc;};
    QuantaView *getView() { return view;};

    MessageOutput *getMessages() { return messageOutput; }

    void openLastFiles();
     	

  protected:
    /** save general Options like all bar positions and status as well as the geometry and the recent file list to the configuration
     * file
     */ 	
    void saveOptions();
    /** read general Options again and initialize all variables like the recent file list
     */
    void readOptions();
    /** initKeyAccel creates the keyboard accelerator items for the available slots and changes the menu accelerators.
     * @see KAccel
     */
    void initKeyAccel();
    /** initMenuBar creates the menubar and inserts the menupopups as well as creating the helpMenu.
     * @see KApplication#getHelpMenu
     */
    void initMenuBar();

    void initContextMenu();

    /**
     * Creates the KActions.
     */
    void initActions();

    /** this creates the toolbars.
     */
    void initToolBar();
    /** sets up the statusbar for the main window by initialzing a statuslabel.
     */
    void initStatusBar();
    /** initializes the document object of the main window that is connected to the view in initView().
     * @see initView();
     */
    void initDocument();
    /** creates the centerwidget of the KTMainWindow instance and sets it as the view
     */
    void initView();
    /** init project class */
    void initProject();
    /** initialize dicts with tag data */
    void initTagDict();
    /** queryClose is called by KTMainWindow on each closeEvent of a window. Against the
     * default implementation (only returns true), this calles saveModified() on the document object to ask if the document shall
     * be saved if Modified; on cancel the closeEvent is rejected.
     * @see KTMainWindow#queryClose
     * @see KTMainWindow#closeEvent
     */
    virtual bool queryClose();
    /** queryExit is called by KTMainWindow when the last window of the application is going to be closed during the closeEvent().
     * Against the default implementation that just returns true, this calls saveOptions() to save the settings of the last window's	
     * properties.
     * @see KTMainWindow#queryExit
     * @see KTMainWindow#closeEvent
     */
    virtual bool queryExit();
    /** saves the window properties for each open window during session end to the session config file, including saving the currently
     * opened file by a temporary filename provided by KApplication.
     * @see KTMainWindow#saveProperties
     */
    virtual void saveProperties(KConfig *_cfg);
    /** reads the session config file and restores the application's state including the last opened files and documents by reading the
     * temporary files saved by saveProperties()
     * @see KTMainWindow#readProperties
     */
    virtual void readProperties(KConfig *_cfg);

  public slots:
    /** switch argument for slot selection by menu or toolbar ID */
    void commandCallback(int id_);
    /** switch argument for Statusbar help entries on slot selection. Add your ID's help here for toolbars and menubar entries. */
    void statusCallback(int id_);
    /** open a new application window by creating a new instance of QuantaApp */
    void slotFileNewWindow();
    //
    void addRecentProject(const QString &file);
    ////////////////////////
	  // editor commands
  	///////////////////////

  	void doCursorCommand(int cmdNum);
  	void doEditCommand(int cmdNum);
  	void doStateCommand(int cmdNum);


    void slotFileNew();
    void slotFileOpen();
    void slotFileOpen( QString fileToOpen );
    void slotImageOpen( QString fileToOpen );
    void slotFileOpenRecent(int id_);
    void slotProjectOpenRecent(int id_);
    void slotFileSave();
    void slotFileSaveAs();
    void slotFileSaveAll();
    void slotFileClose();
    void slotFileCloseAll();
    void slotFilePrint();
    void slotFileNext();
		void slotFilePrev();

    /** closes all open windows by calling close() on each memberList item until the list is empty, then quits the application.
     * If queryClose() returns false because the user canceled the saveModified() dialog, the closing breaks.
     */
    void slotFileQuit();

    void slotEditCut();
    void slotEditCopy();
    void slotEditPaste();
    void slotEditUndo();
    void slotEditRedo();
    void slotURedoHistory();
    void slotEditSelectAll();
    void slotEditDeselectAll();
    void slotEditInvertSelect();
    void slotEditVerticalSelect();
    void slotEditSearch();
    void slotEditSearchAgain();
    void slotEditReplace();
    void slotEditFindInFiles();
    void slotEditIndent();
    void slotEditUnindent();
    void slotEditCleanIndent();
    void slotEditGotoLine();
    
    void slotToolSyntaxCheck();

    /** open url in documentation window
     */
    void openDoc( QString url );

    void updateNavButtons( bool back, bool forward );

    void contextHelp();

    /** toggles the toolbar
     */
    void slotViewToolBar();
    /** toggles the statusbar
     */
    void slotViewStatusBar();
    /** changes the statusbar contents for the standard label permanently, used to indicate current actions.
     * @param text the text that is displayed in the statusbar
     */
    void slotStatusMsg(const QString &text);
    /** changes the status message of the whole statusbar for two seconds, then restores the last status. This is used to display
     * statusbar messages that give information about actions for toolbar icons and menuentries.
     * @param text the text that is displayed in the statusbar
     */
    void slotStatusHelpMsg(const QString &text);
    /** statuses */
    void slotNewStatus();
    void slotNewUndo();
    void slotNewMarkStatus();
    void slotNewLineColumn();
    /** update all statuses */
    void slotUpdateStatus(const QString &);

    void slotSwapLeftPanelMode();

    void slotLeftTabChanged(QWidget *);

    void slotActivatePreview();
    /** show preview ( F6 )*/
    void slotShowPreview();
    void slotShowLeftPanel();

    void slotFtpClient();
    
    void slotInsertTag(QString);

    /** Repaint preview ( slot ) */
    void slotViewRepaint();

    void slotViewMessages();
    
    /** repaint preview */
    void repaintPreview( bool clear = false);

    /** options slots */
    void slotOptions();

    /** configure keybindings */
    void slotOptionsConfigureKeys();

    void slotOptionsConfigureToolbars();

    /** set highlight of document */
    void slotSetHl(int);
    void slotSetEol(int);

    /** reparse current document and initialize node. */
	  void reparse();
	
	  void setCursorPosition( int row, int col );
      void gotoFileAndLine( const QString &filename, int line );

	  void selectArea(int col1, int row1, int col2, int row2);
	
	  void slotSelectMessageWidget();
	  void slotDisableMessageWidget();
	  
	protected:
	  KParts::BrowserExtension *browserExtension() {
      return static_cast<KParts::BrowserExtension *>(((KParts::ReadOnlyPart *)htmlPart())->child( 0L, "KParts::BrowserExtension" ));
    }
	
  private:
    /** project class */
    Project *project;

    /** return htmlPart or 0L if use previewPosition=="Disabled" */
    WHTMLPart *htmlPart();
    QWidgetStack *widgetStackOfHtmlPart();

    /** contains the recently used filenames */
    QStrList recentFiles;
    QStrList recentProjects;

    /** tool bars */
    KToolBar *toolbar1;
    KToolBar *toolbar2;
    
    /** the configuration object of the application */
    KConfig *config;
    /** the key accelerator container */
    KAccel *keyAccel;
    /** menus contains all items of the menubar entry "File" */
    QPopupMenu *fileMenu;
    QPopupMenu *recentFilesMenu;
    QPopupMenu *recentProjectsMenu;
    QPopupMenu *editMenu;
    QPopupMenu *toolMenu;
    QPopupMenu *viewMenu;
    QPopupMenu *insertMenu;
    QPopupMenu *projectMenu;
    QPopupMenu *bookmarksMenu;
    QPopupMenu *optionsMenu;
    QPopupMenu *helpMenu_;

    QPopupMenu *highlightMenu;

    /** child menus(insert) */
    QPopupMenu *insertStandardMenu;
    QPopupMenu *insertFontsMenu;
    QPopupMenu *insertTablesMenu;
    QPopupMenu *insertFormsMenu;
    QPopupMenu *insertListsMenu;

    /** RBM popup menu for kwrite */

    QPopupMenu *kwritePopupMenu;


    /** widget stack for left panel */
    QWidgetStack *fTab;
    /** tabs for left panel */
    ProjectTreeView *pTab;
    DocTreeView 		*dTab;
    FilesTreeView		*fTTab;
    FilesListView		*fLTab;

    StructTreeView *sTab;

    /** panners for a tree, a preview and a write*/

    QString previewPosition;

    QWidgetStack *topWidgetStack;
    QWidgetStack *rightWidgetStack;
    QWidgetStack *bottomWidgetStack;

    WSplitter *hSplit;
    WSplitter *vSplit;

   	/** HTML class for preview */
    WHTMLPart *htmlPartTop;
    WHTMLPart *htmlPartBottom;
    WHTMLPart *htmlPartRight;
    WHTMLPart *htmlPartDoc;

    /** Messaage output window */
    MessageOutput *messageOutput;

    /** Grep window */
    GrepDialog *grepDialog;

    /** qtabwidget for left panel */
    QTabWidget *leftPanel;
    /** view is the main widget */
    QuantaView *view;
    /** doc represents your actual document and */
    QuantaDoc *doc;

        /** parsered tree of document  */
    Node *node;
    Parser *parser;

    ToolBars *toolbars;

    int phpDebugPort;
};
 
#endif // QUANTA_H
