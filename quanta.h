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

// include files for KDE 
#include <kapp.h>
#include <kdockwidget.h>
#include "kqapp.h"
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
class TemplatesTreeView;
class Project;
class ToolBars;
class GrepDialog;
class MessageOutput;
class QDomDocument;
class ActionEditDlg;

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
    
    void initActions();
    void initStatusBar();
    void initDocument();
    void initView();
    void initProject();
    void initTagDict();
    
    virtual bool queryExit();
    void saveAsTemplate (bool projectTemplate);

  public slots:
    
    void slotFileNew ();
    void slotFileOpen();
    void slotFileOpen( const KURL & );
    void slotFileSave    ();
    void slotFileSaveAs  ();
    void slotFileSaveAsLocalTemplate ();
    void slotFileSaveAsProjectTemplate ();
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

    void slotFtpClient();

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
//    void setEOLMenuAboutToShow();

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
    void gotoFileAndLine  ( const QString &filename, int line );

	  void selectArea(int col1, int row1, int col2, int row2);
	
	  void setTitle(QString);
	  
	  void autoComplete();
	
	  void slotShowOpenFileList();
  /** No descriptions */
  void slotNewProjectLoaded();
  /** No descriptions */
  void slotInsertFile(QString);
	  
  public:

	  	
	  	QDomDocument *actions() { return m_actions; }
	  	  
	protected:
	  KParts::BrowserExtension *browserExtension() {
      return static_cast<KParts::BrowserExtension *>(((KParts::ReadOnlyPart *)htmlPart())->child( 0L, "KParts::BrowserExtension" ));
    }
	
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
    Node   *node;
    Parser *parser;

    ToolBars *toolbars;
    
    QTimer *statusbarTimer;

    // debugger
    int phpDebugPort;
    PHP3Debugger* dbg3;
    PHP4Debugger* dbg4;
    QString debuggerStyle;
    
    // ACTIONS
    KRecentFilesAction *fileRecent;
    
    KToggleAction *showMessagesAction, *showTreeAction,
      *showFTabAction,*showPTabAction,*showTTabAction,*showSTabAction,*showDTabAction,
      *showToolbarAction,*showStatusbarAction,*showPreviewAction;
    
    KSelectAction *setEndOfLine;

    KAction *saveAction, *saveAllAction, *undoAction, *redoAction,
      *backAction, *forwardAction,
      *saveprjAction, *closeprjAction, *insertFileAction, *insertDirAction,
      *newPrjAction, *openPrjAction, *rescanPrjDirAction, *uploadProjectAction,
      *projectOptionAction, *saveAsLocalTemplateAction, *saveAsProjectTemplateAction;
      
    KAction *editUndo, *editRedo, *bookmarkToggle, *bookmarkClear;

    QDomDocument *m_actions;

};
 
#endif // QUANTA_H
