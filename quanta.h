/***************************************************************************
                          quanta.h  -  description
                             -------------------
    begin                : ?? ???  9 13:29:57 EEST 2000
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
 
#ifndef VERSION
  #define VERSION "2"
#endif

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

// include files for KDE 
#include <kapp.h>
#include <kdockwidget.h>
#include "widgets/whtmlpart.h"
#include <kparts/browserextension.h>

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
class Project;
class ToolBars;
class GrepDialog;
class MessageOutput;
class QDomDocument;

/**
  * The base class for Quanta application windows. 
  */
class QuantaApp : public KDockMainWindow
{
  Q_OBJECT

  friend class QuantaDoc;
  friend class QuantaView;

  public:
    QuantaApp();
    ~QuantaApp();
    
    QuantaDoc  *getDoc()   { return doc; };
    QuantaView *getView()  { return view;};

    MessageOutput *getMessages() { return messageOutput; }

    void openLastFiles();
    
    /** from @see KXMLGUIBuilder, for tollbars in tabbar ( tags,... )
     *
     */
    QWidget* createContainer( QWidget *parent, int index, const QDomElement &element, int &id );  
    void removeContainer( QWidget *container, QWidget *parent, QDomElement &element, int id );

  protected:
  
    void saveOptions();
    void readOptions();
    
    void initContextMenu();
    void initActions();
    void initStatusBar();
    void initDocument();
    void initView();
    void initProject();
    void initTagDict();
    
    virtual bool queryExit();
    
  public slots:
    
    void slotFileNew ();
    void slotFileOpen();
    void slotFileOpen( KURL & );
    void slotFileSave    ();
    void slotFileSaveAs  ();
    void slotFileSaveAll ();
    void slotFileClose   ();
    void slotFileCloseAll();
    void slotFilePrint   ();
    void slotFileNext    ();
		void slotFilePrev    ();
    void slotFileQuit    ();
    
    void slotImageOpen        (QString);
    void slotFileOpenRecent   (const KURL&);
    void slotInsertTag        (QString);

    void slotEditFindInFiles   ();
    
    void slotToolSyntaxCheck();

    /// open url in documentation window
    void openDoc( QString url );

    void updateNavButtons( bool back, bool forward );

    void contextHelp();

    void slotViewToolBar  ();
    void slotViewStatusBar();
    
    void statusBarTimeout ();
    void slotStatusMsg    (const QString &text);
    
    void slotNewUndo      ();
    void slotNewStatus    ();
    void slotNewMarkStatus();
    void slotNewLineColumn();
    void slotUpdateStatus (const QString &);

    void slotSwapLeftPanelMode();

    void slotLeftTabChanged(QWidget *);

    void slotActivatePreview();
    
    /** show preview ( F6 )*/
    void slotShowPreview();
    
    
    
    void slotShowProjectTree();

    void slotFtpClient();

    /** Repaint preview ( slot ) */
    void slotViewRepaint();

    void slotShowLeftDock();
    void slotShowBottDock();
    void slotShowFTabDock();
    void slotShowPTabDock();
    void slotShowSTabDock();
    void slotShowDTabDock();
    
    void viewMenuAboutToShow();
    void settingsMenuAboutToShow();
    
    /** repaint preview */
    void repaintPreview( bool clear = false);

    /** options slots */
    void slotOptions();
    void slotOptionsConfigureKeys();
    void slotOptionsConfigureToolbars();
    void slotOptionsConfigureActions();

    /** reparse current document and initialize node. */
	  void reparse();
	
	  void setCursorPosition( int row, int col );
    void gotoFileAndLine  ( const QString &filename, int line );

	  void selectArea(int col1, int row1, int col2, int row2);
	
	  void setTitle(QString);

	  	  
	protected:
	  KParts::BrowserExtension *browserExtension() {
      return static_cast<KParts::BrowserExtension *>(((KParts::ReadOnlyPart *)htmlPart())->child( 0L, "KParts::BrowserExtension" ));
    }
	
  private:
    /** project class */
    Project *project;

    /** return htmlPart or 0L if use previewPosition=="Disabled" */
    WHTMLPart    *htmlPart();
    QWidgetStack *widgetStackOfHtmlPart();
    
    // config
    KConfig *config;

    /** widget stack for left panel */
    QWidgetStack *fTab;
    
    /** tabs for left panel */
    ProjectTreeView  *pTab;
    DocTreeView 		 *dTab;
    FilesTreeView		*fTTab;
    FilesListView		*fLTab;
    StructTreeView   *sTab;

    QString previewPosition;

    QWidgetStack *rightWidgetStack;
    
    KDockWidget *leftdock;
    KDockWidget *maindock;
    KDockWidget *bottdock;
    KDockWidget *ptabdock;
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

    /** parsered tree of document  */
    Node   *node;
    Parser *parser;

    ToolBars *toolbars;
    
    QTimer *statusbarTimer;

    int phpDebugPort;
    
    // ACTIONS
    KRecentFilesAction *fileRecent;
    
    KToggleAction *verticalSelectAction, *showMessagesAction, *showTreeAction,
      *showFTabAction,*showPTabAction,*showSTabAction,*showDTabAction;
    
    KSelectAction *eolSelectAction, *hlSelectAction;
    
    KAction *saveAction, *saveAllAction, *undoAction, *redoAction,
      *cutAction, *copyAction, *findNextAction, *backAction, *forwardAction,
      *closeprjAction, *insertFileAction, *insertDirAction, 
      *rescanPrjDirAction, *uploadProjectAction, *projectOptionAction;
      
    QDomDocument *actions;
};
 
#endif // QUANTA_H
