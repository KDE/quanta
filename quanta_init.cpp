/***************************************************************************
                          quanta.cpp  -  description
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

// include files for QT
#include <qdir.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qtabwidget.h>
#include <qwidgetstack.h>
#include <qlayout.h>
#include <qtoolbutton.h>

#include <qprogressbar.h>

// include files for KDE
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <khtmlview.h>
#include <kstddirs.h>

// application specific includes
#include "quanta.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "resource.h"
#include "document.h"

#include "kwrite/kwdoc.h"
#include "kwrite/kwview.h"

#include "project/project.h"

#include "widgets/wtoolbar.h"
#include "widgets/whtmlpart.h"
#include "messages/messageoutput.h"

#include "treeviews/fileslistview.h"
#include "treeviews/filestreeview.h"
#include "treeviews/projecttreeview.h"
#include "treeviews/doctreeview.h"
#include "treeviews/structtreeview.h"

#include "phpdebug/phpdebugserversocket.h"

#include "parser/parser.h"


QString fileMaskHtml 	= "*.*html *.*htm *.php* *.asp *.cfm *.css *.inc* *.*HTML *.*HTM *.PHP* *.ASP *.CFM *.CSS *.INC* *.xml *.XML";
QString fileMaskPhp 	= "*.*PHP* *.*php* ";
QString fileMaskJava  = "*.jss *.js *.JSS *.JS ";
QString fileMaskText  = "*.txt *.TXT ";
QString fileMaskImage = "*.gif *.jpg *.png *.jpeg *.bmp *.xpm *.GIF *.JPG *.PNG *.JPEG *.BMP ";

QStrList *tagsList; // list of known tags
QStrList *tagsCore; // list of tags, allowed core attributes ( id, class, id, title )
QStrList *tagsI18n; // list of tags, allowed i18 attribs.
QStrList *tagsScript; // list of tags, allowed script attribs ( onClicl, omMouseOver... )
QStrList *quotedAttribs; // list of attribs, that have quoted values ( alt, src ... )
QStrList *lCore;          // list of core attributes ( id, class, style ... )
QStrList *lI18n;
QStrList *lScript;
QStrList *singleTags; // tags without end  part </ >
QStrList *optionalTags; // tags with optional end part

bool tagsCapital;
bool attrCapital;
bool useCloseTag;

QDict <QStrList> *tagsDict;

#include <kaction.h>
#include <kstdaction.h>

QuantaApp::QuantaApp() : KDockMainWindow(0L,"Quanta")
{
  grepDialog	= 0;

  config=kapp->config();

  parser = new Parser();
  
  initTagDict  ();
  initStatusBar();
  initDocument ();
  initView     ();
  initProject  ();
  
  initActions();
  createGUI( QString::null, false );
  
  initContextMenu();
  
  QPopupMenu* pm_set  = (QPopupMenu*)guiFactory()->container("settings", this);
  connect(pm_set, SIGNAL(aboutToShow()), this, SLOT(settingsMenuAboutToShow())); 
  
  QPopupMenu* pm_view = (QPopupMenu*)guiFactory()->container("qview", this);
  connect(pm_view,SIGNAL(aboutToShow()), this, SLOT(viewMenuAboutToShow())); 
  
  doc->newDocument( KURL() );
  
  readOptions();
  
  PhpDebugServerSocket *debugger = 
    new PhpDebugServerSocket( phpDebugPort,0,0);
    
  connect( debugger,      SIGNAL(newConnect()),
           messageOutput, SLOT(newPhpConnect()) );
  connect( debugger,      SIGNAL(endConnect()),
           messageOutput, SLOT(endPhpConnect()) );
  connect( debugger,      SIGNAL(data(QString)),
           messageOutput, SLOT(phpDebug(QString)) );
  connect( messageOutput, SIGNAL(clicked(const QString&,int)),
           this,          SLOT(gotoFileAndLine(const QString&,int)));
           
  if ( !debugger->ok() ) {
    QString s;
	  messageOutput->insertItem("Can't bind port "+
	    s.sprintf("%i",phpDebugPort)+" , PHP debugger disabled" );
	}
	else {
	  QString s;
	  messageOutput->insertItem("Php Debugger listens port "+
	    s.sprintf("%i",phpDebugPort)+"" );
	}
}

QuantaApp::~QuantaApp()
{
}

void QuantaApp::initStatusBar()
{
  statusbarTimer = new QTimer(this);
  connect(statusbarTimer,SIGNAL(timeout()),this,SLOT(statusBarTimeout()));
  
//  int h = statusBar()->fontMetrics().height();
  
//  QProgressBar *progressBar = new QProgressBar();
  
//  progressBar->setProgress(100);
//  progressBar->setMinimumSize(QSize(200,h));
//  progressBar->setMaximumSize(QSize(200,h));
  
  statusBar()->insertItem(i18n(IDS_DEFAULT),IDS_STATUS, 1);
//  statusBar()->addWidget(progressBar);
  statusBar()->insertFixedItem(" XXX ",     IDS_INS_OVR  );
  statusBar()->insertFixedItem(" * ",       IDS_MODIFIED );
  statusBar()->insertFixedItem(i18n("Line: 00000 Col: 000"), IDS_STATUS_CLM, true);
  
  statusBar()->changeItem("", IDS_INS_OVR);
  statusBar()->changeItem("", IDS_MODIFIED);
  statusBar()->changeItem("", IDS_STATUS_CLM);
  statusBar()->setItemAlignment(IDS_STATUS, AlignLeft);
}

void QuantaApp::statusBarTimeout() 
{
  statusBar()->changeItem("", IDS_STATUS);
}

void QuantaApp::initContextMenu()
{
//  kwritePopupMenu = (QPopupMenu *)factory()->container("documentContextPopup", this);
//  doc->setRBMenu(kwritePopupMenu);
}

void QuantaApp::initDocument()
{
  doc = new QuantaDoc(this,this);
  connect(doc, SIGNAL(newStatus()),    this, SLOT(slotNewStatus()));
}

void QuantaApp::initProject()
{
	project = new Project(this);
	
	connect(project, 	SIGNAL(openFile    (KURL &)),
					this, 		SLOT  (slotFileOpen(KURL &)));
	connect(project, 	SIGNAL(reloadTree(QStringList,bool,bool)),
					pTab, 		SLOT  (slotReloadTree(QStringList,bool,bool)));
	connect(project, 	SIGNAL(setBasePath(QString)),
					pTab, 		SLOT  (slotSetBasePath(QString)));
	connect(project, 	SIGNAL(setProjectName(QString)),
					pTab, 		SLOT  (slotSetProjectName(QString)));
	connect(project,	SIGNAL(closeFiles()),
					doc,			SLOT	(closeAll()));
	connect(project,  SIGNAL(showTree()),
	        this,     SLOT  (slotShowProjectTree()));
					
	connect(fLTab,		SIGNAL(insertDirInProject(QString)),
					project,	SLOT	(addDirectory(QString)));
	connect(fTTab,		SIGNAL(insertDirInProject(QString)),
					project,	SLOT	(addDirectory(QString)));
					
	connect(fLTab,		SIGNAL(insertFileInProject(QString)),
					project,	SLOT	(insertFile(QString)));
	connect(fTTab,		SIGNAL(insertFileInProject(QString)),
					project,	SLOT	(insertFile(QString)));
					
	connect(pTab,			SIGNAL(removeFileFromProject(QString)),
					project,	SLOT	(slotRemoveFile(QString)));
	connect(pTab,			SIGNAL(removeFolderFromProject(QString)),
					project,	SLOT	(slotRemoveFolder(QString)));
					
	connect(project,	SIGNAL(selectMessageWidget()),
					this,			SLOT  (slotSelectMessageWidget()));
	connect(project,	SIGNAL(disableMessageWidget()),
					this,			SLOT  (slotDisableMessageWidget()));
					
	connect(project,	SIGNAL(messages(QString)),
					messageOutput, SLOT(showMessage(QString)));

	connect(project,	SIGNAL(saveAllFiles()),
					this, SLOT(slotFileSaveAll()));
	connect(project,	SIGNAL(newStatus()),
					this, SLOT(slotNewStatus()));
}

void QuantaApp::initView()
{ 
  ////////////////////////////////////////////////////////////////////
  // create the main widget here that is managed by KTMainWindow's view-region and
  // connect the widget to your document to display document contents.

  maindock = createDockWidget( "Editor", UserIcon("textarea"  ), 0L, i18n("Editor"));
  bottdock = createDockWidget( "Output", UserIcon("output_win"), 0L, i18n("Output"));
  
  ftabdock = createDockWidget( "Files",  UserIcon("ftab"),     0L, " ");
  ptabdock = createDockWidget( "Project",UserIcon("ptab"),     0L, " ");
  stabdock = createDockWidget( "Struct", BarIcon ("view_sidetree"),0L, " ");
  dtabdock = createDockWidget( "Docs",   BarIcon ("contents2"),    0L, " ");

  QStrList topList;
  config->setGroup("General Options");
  config->readListEntry("Top folders", topList);

  fTab = new QWidgetStack( ftabdock );
  
  fTTab = new FilesTreeView( QDir::homeDirPath()+"/" , topList, fTab );
  fLTab = new FilesListView( QDir::homeDirPath()+"/" , 0L, fTab );

  fTab  ->setFocusPolicy(QWidget::NoFocus);
  fTTab ->setFocusPolicy(QWidget::ClickFocus);
  fLTab ->setFocusPolicy(QWidget::ClickFocus);

  fTab -> addWidget( fTTab, 0 );
  fTab -> addWidget( fLTab, 1 );
  fTab -> raiseWidget( 0 );

  pTab	= new ProjectTreeView( ptabdock );
  dTab	= new DocTreeView    ( dtabdock );

  pTab -> setFocusPolicy(QWidget::NoFocus);
  dTab -> setFocusPolicy(QWidget::NoFocus);

  sTab = new StructTreeView( parser, config, stabdock ,"struct");
	sTab ->setFocusPolicy(QWidget::NoFocus);

  rightWidgetStack = new QWidgetStack( maindock );

  view = new QuantaView( this, rightWidgetStack );
  view->app = this;
  view->doc = doc;
  
  messageOutput = new MessageOutput( bottdock );
  messageOutput ->setFocusPolicy(QWidget::NoFocus);

  maindock ->setWidget( rightWidgetStack );
  bottdock ->setWidget( messageOutput );
  ftabdock ->setWidget( fTab );
  ptabdock ->setWidget( pTab );
  stabdock ->setWidget( sTab );
  dtabdock ->setWidget( dTab );
  
  rightWidgetStack  ->setMinimumHeight( 1 );

  htmlpart    	 = new WHTMLPart( rightWidgetStack,  "rightHTML");
  htmlPartDoc 	 = new WHTMLPart( rightWidgetStack,  "docHTML");

  rightWidgetStack->addWidget( view, 0 );
  rightWidgetStack->addWidget( htmlpart   ->view(), 1 );
  rightWidgetStack->addWidget( htmlPartDoc->view(), 2 );
  rightWidgetStack->raiseWidget(0);

  connect( 	fTTab,SIGNAL(openFile  (KURL &)),
  					this, SLOT(slotFileOpen(KURL &)));
  connect( 	fLTab,SIGNAL(openFile  (KURL &)),
  					this, SLOT(slotFileOpen(KURL &)));
  					
  connect( 	fTTab,SIGNAL(openImage(QString)),
  					this, SLOT  (slotImageOpen(QString)));
  connect( 	fLTab,SIGNAL(openImage(QString)),
  					this, SLOT  (slotImageOpen(QString)));
  					
  connect(	fLTab,SIGNAL(changeMode()),
  					this, SLOT(slotSwapLeftPanelMode()));
  connect(	fTTab,SIGNAL(changeMode()),
  					this, SLOT(slotSwapLeftPanelMode()));
  					
  connect( 	pTab, SIGNAL(openFile  (KURL &)),
  					this, SLOT(slotFileOpen(KURL &)));
  connect( 	pTab, SIGNAL(openImage  (QString)),
  					this, SLOT(slotImageOpen(QString)));
  					
  connect( 	fTTab,SIGNAL(insertTag(QString)),
  					this, SLOT(slotInsertTag(QString)));
  connect( 	fLTab,SIGNAL(insertTag(QString)),
  					this, SLOT(slotInsertTag(QString)));
  connect( 	pTab,SIGNAL(insertTag(QString)),
  					this, SLOT(slotInsertTag(QString)));
  					
  connect( 	fLTab,SIGNAL(activatePreview()),
  					this, SLOT(slotActivatePreview()));
  connect( 	fTTab,SIGNAL(activatePreview()),
  					this, SLOT(slotActivatePreview()));
  connect( 	pTab, SIGNAL(activatePreview()),
  					this, SLOT(slotActivatePreview()));
  					
  connect(  htmlpart,       SIGNAL(onURL(const QString&)), this, SLOT(slotStatusMsg(const QString&)));
  connect(  htmlPartDoc,    SIGNAL(onURL(const QString&)), this, SLOT(slotStatusMsg(const QString&)));
  connect(  htmlPartDoc,    SIGNAL(updateStatus(bool, bool)), SLOT(updateNavButtons( bool, bool)));

  connect( view, SIGNAL(newCurPos()), this, SLOT(slotNewLineColumn()));

  connect( sTab, SIGNAL(newCursorPosition(int,int)), SLOT(setCursorPosition(int,int)));
//  connect( sTab, SIGNAL(onTag(const QString &)), SLOT( slotStatusHelpMsg(const QString &)));
  connect( sTab, SIGNAL(selectArea(int,int,int,int)), SLOT( selectArea(int,int,int,int)));
  connect( sTab, SIGNAL(needReparse()),    SLOT(reparse()));
  connect( dTab, SIGNAL(openURL(QString)), SLOT(openDoc(QString)));
  
  setMainDockWidget( maindock );
  setView(maindock);
  
  maindock ->setEnableDocking( KDockWidget::DockNone );
  ftabdock ->manualDock(maindock, KDockWidget::DockLeft,   30);
  bottdock ->manualDock(maindock, KDockWidget::DockBottom, 80);
  
  ptabdock ->manualDock(ftabdock, KDockWidget::DockCenter);
  stabdock ->manualDock(ftabdock, KDockWidget::DockCenter);
  dtabdock ->manualDock(ftabdock, KDockWidget::DockCenter);
}


WHTMLPart * QuantaApp::htmlPart()
{
  return htmlpart;
}


QWidgetStack *QuantaApp::widgetStackOfHtmlPart()
{
  return rightWidgetStack;
}

void QuantaApp::saveOptions()
{	
  config->setGroup  ("General Options");
  
  config->writeEntry("Geometry", size());
  config->writeEntry("Show Statusbar",statusBar()->isVisible());

  config->writeEntry("Html mask",   fileMaskHtml  );
  config->writeEntry("Images mask", fileMaskImage );
  config->writeEntry("Php mask",    fileMaskPhp   );
  config->writeEntry("Java mask",   fileMaskJava  );
  config->writeEntry("Text mask",   fileMaskText  );

  config->writeEntry("Capitals for tags",     tagsCapital);
  config->writeEntry("Capitals for attr",     attrCapital);
  config->writeEntry("Close tag if optional", useCloseTag);

  config->writeEntry("Preview position", previewPosition);

  config->writeEntry("Left panel mode", fTab->id( fTab->visibleWidget()));

  config->writeEntry("Follow Cursor", sTab->followCursor() );

  config->writeEntry("PHP Debugger Port", phpDebugPort );
  
  config->writeEntry("Top folders",          fTTab->dirList);
  config->writeEntry("List of opened files", doc->openedFiles());
  
  config->writeEntry ("Version", VERSION); // version
  
  doc    ->writeConfig(config); // kwrites
  project->writeConfig(config); // project
  
  fileRecent            ->saveEntries(config);
  
  writeDockConfig();
}


void QuantaApp::readOptions()
{
  config->setGroup("General Options");
  //if (2>config->readNumEntry("Version",0)) config = new KConfig();

  fileMaskHtml  = config->readEntry("Html mask", 	fileMaskHtml)	+" ";
  fileMaskImage = config->readEntry("Images mask",fileMaskImage)+" ";
  fileMaskPhp   = config->readEntry("Php mask", 	fileMaskPhp)	+" ";
  fileMaskJava  = config->readEntry("Java mask", 	fileMaskJava)	+" ";
  fileMaskText  = config->readEntry("Text mask", 	fileMaskText)	+" ";

  tagsCapital = config->readBoolEntry("Capitals for tags",     false);
  attrCapital = config->readBoolEntry("Capitals for attr",     false);
  useCloseTag = config->readBoolEntry("Close tag if optional", true);

  previewPosition   = config->readEntry("Preview position","Right");

  phpDebugPort = config->readNumEntry("PHP Debugger Port", 7869);
  
  sTab->setFollowCursor( config->readBoolEntry("Follow Cursor", true));
  
  int mode = config->readNumEntry("Left panel mode", 0);
  if ( mode == 0 || mode == 1 ) fTab->raiseWidget(mode);
  
#warning Check for statusbar  

  fileRecent ->loadEntries(config);
  
  doc    ->readConfig(config); // kwrites
  project->readConfig(config); // project

  config->setGroup("General Options");
  resize( config->readSizeEntry("Geometry", &QSize(800,580)));
  
  readDockConfig();
}

void QuantaApp::openLastFiles()
{
  config->setGroup("General Options");

  QStrList urls;
 	config->readListEntry("List of opened files", urls);

  for ( urls.last();urls.current();urls.prev() )
  {
    doc->openDocument( KURL(urls.current()) );
  }
}

#warning Check action for message output and tree

bool QuantaApp::queryExit()
{
  saveOptions();
  return doc->saveAll(false);
}

/**
  read dictionary of known tags and attributes from tags.rc file.
*/

void QuantaApp::initTagDict()
{
  tagsList      = new QStrList();
  tagsCore      = new QStrList();
  tagsI18n      = new QStrList();
  tagsScript    = new QStrList();
  quotedAttribs = new QStrList();
  lCore         = new QStrList();
  lI18n         = new QStrList();
  lScript       = new QStrList();
  singleTags    = new QStrList();
  optionalTags  = new QStrList();

  tagsDict = new QDict <QStrList>(233,false);

  KConfig *config = new KConfig( locate("appdata","tagdata.rc") );

  config->setGroup("Tags");

  config->readListEntry("TagsCore",       *tagsCore);
  config->readListEntry("TagsI18n",       *tagsI18n);
  config->readListEntry("TagsScript",     *tagsScript);
  config->readListEntry("Quoted Attribs", *quotedAttribs);

  config->readListEntry("Core",    *lCore);
  config->readListEntry("I18n",    *lI18n);
  config->readListEntry("Scripts", *lScript);

  config->readListEntry("TagsList", *tagsList);

  config->readListEntry("Single tags",   *singleTags);
  config->readListEntry("Optional tags", *optionalTags);

  char *tag, *t;
  for ( tag = tagsList->first(); tag; tag = tagsList->next() )
	{
    QStrList *attrList = new QStrList();
    config->readListEntry(tag, *attrList);

    if ( tagsCore->find(tag) != -1 )
      for ( t = lCore->first(); t; t = lCore->next() )
        attrList->append(t);

    if ( tagsI18n->find(tag) != -1 )
      for ( t = lI18n->first(); t; t = lI18n->next() )
        attrList->append(t);

    if ( tagsScript->find(tag) != -1 )
      for ( t = lScript->first(); t; t = lScript->next() )
        attrList->append(t);

    tagsDict->insert(tag, attrList);
  }
}

void QuantaApp::initActions()
{
    // File actions
    //
    (void) new KAction( i18n( "&New" ), UserIcon("new"), KStdAccel::key(KStdAccel::New),
                        this, SLOT( slotFileNew() ),
                        actionCollection(), "file_new" );
    
    (void) new KAction( i18n( "&Open" ), UserIcon("open"), KStdAccel::key(KStdAccel::Open),
                        this, SLOT( slotFileOpen() ),
                        actionCollection(), "file_open" );
                        
    fileRecent =
      KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)),
                             actionCollection(), "file_open_recent");
                        
    KStdAction::close  ( this, SLOT( slotFileClose()), actionCollection() );

    (void) new KAction( i18n( "Close All" ), 0, this, 
                        SLOT( slotFileCloseAll() ),
                        actionCollection(), "close_all" );
                        
    saveAction = new KAction( i18n( "&Save" ), UserIcon("save"), KStdAccel::key(KStdAccel::Save),
                        this, SLOT( slotFileSave() ),
                        actionCollection(), "file_save" );
    
    KStdAction::saveAs( this, SLOT( slotFileSaveAs() ), actionCollection() );

    saveAllAction = new KAction( i18n( "Save All..." ), UserIcon("save_all"), 0,
                        this, SLOT( slotFileSaveAll() ),
                        actionCollection(), "save_all" );

    KStdAction::quit( this, SLOT( slotFileQuit() ), actionCollection() );

    // Edit actions
    //
    undoAction = new KAction( i18n( "&Undo" ), UserIcon("undo"), KStdAccel::key(KStdAccel::Undo),
                        doc, SLOT( undo() ),
                        actionCollection(), "edit_undo" );
                        
    redoAction = new KAction( i18n( "&Redo" ), UserIcon("redo"), KStdAccel::key(KStdAccel::Redo),
                        doc, SLOT( redo() ),
                        actionCollection(), "edit_redo" );

    KAction *undoRedo 
      = new KAction( i18n( "Undo/Redo &History..."), 0, 
                     doc, SLOT( undoHistory()),
                     actionCollection(), "undo_history" );
                                     
    undoRedo->setGroup( "edit_undo_merge" );
    
    cutAction = new KAction( i18n( "Cu&t" ), UserIcon("cut"), KStdAccel::key(KStdAccel::Cut),
                        doc, SLOT( cut() ),
                        actionCollection(), "edit_cut" );
                        
    copyAction = new KAction( i18n( "&Copy" ), UserIcon("copy"), KStdAccel::key(KStdAccel::Copy),
                        doc, SLOT( copy() ),
                        actionCollection(), "edit_copy" );   
                        
    (void) new KAction( i18n( "&Paste" ), UserIcon("paste"), KStdAccel::key(KStdAccel::Paste),
                        doc, SLOT( paste() ),
                        actionCollection(), "edit_paste" );                    
                        
    KStdAction::selectAll( doc, SLOT( selectAll()), actionCollection());

    (void) new KAction(  i18n( "&Unselect All"),       0, 
                         doc, SLOT( deselectAll()),
                         actionCollection(), "unselect_all" );
                         
    (void) new KAction(  i18n( "&Invert Selection"),   0, 
                         doc, SLOT( invertSelect()),
                         actionCollection(), "invert_selection" );
                         
    verticalSelectAction =
           new KToggleAction(  i18n( "&Vertical Selection"), 0, 
                               doc, SLOT( verticalSelect()),
                               actionCollection(), "vertical_selection" );
                               
    (void) new KAction( i18n( "&Find" ), UserIcon("find"), KStdAccel::key(KStdAccel::Find),
                        doc, SLOT( find() ),
                        actionCollection(), "edit_find" );
                        
    findNextAction = KStdAction::findNext( doc, SLOT( findAgain()), actionCollection());
    KStdAction::replace ( doc, SLOT( replace()),   actionCollection());

    (void) new KAction( i18n( "Find In Files" ), 
                        UserIcon("find"), CTRL+ALT+Key_F,
                        this, SLOT( slotEditFindInFiles() ),
                        actionCollection(), "find_in_files" );

    // Tool actions
    //
    (void) new KAction( i18n( "&Go To Line..." ), CTRL+Key_G, 
                        doc, SLOT( gotoLine() ),
                        actionCollection(), "goto_line" );

    (void) new KAction( i18n( "&Indent" ), ALT+SHIFT+Key_Right, 
                        doc, SLOT( indent() ),
                        actionCollection(), "indent" );
                        
    (void) new KAction( i18n( "&Unindent" ), ALT+SHIFT+Key_Left, 
                        doc, SLOT( unindent() ),
                        actionCollection(), "unindent" );
                        
    (void) new KAction( i18n( "&Clean Indentation" ), 0, 
                        doc, SLOT( cleanIndent() ),
                        actionCollection(), "clean_indentation" );

    (void) new KAction( i18n( "Context &Help..." ), CTRL+Key_H, 
                        this, SLOT( contextHelp() ),
                        actionCollection(), "context_help" );
                        
    KAction *tagAttr 
      = new KAction( i18n( "Tag &Attributes" ), ALT+Key_Down,
                     doc, SLOT( slotAttribPopup() ),
                     actionCollection(), "tag_attributes" );
                     
    QAccel *menuKey = new QAccel( this );
//    menuKey->connectItem( menuKey->insertItem( Key_Menu ),
//                          tagAttr, SLOT( activate() ) );

    (void) new KAction( i18n( "&Edit Current Tag..." ), Key_F4, 
                        view, SLOT( slotEditCurrentTag() ),
                        actionCollection(), "edit_current_tag" );

    KStdAction::spelling( view->write(), SLOT( slotSpellCheck() ), 
                          actionCollection(), "text_spelling" )->setAccel( Key_F9 );
    
    (void) new KAction( i18n( "&Syntax check" ), 0, 
                        this, SLOT( slotToolSyntaxCheck() ),
                        actionCollection(), "syntax_check" );
    
    // View actions
    //
    showFTabAction = 
      new KToggleAction( i18n( "Show Files Tree" ), 0,
                         this, SLOT( slotShowFTabDock() ),
                         actionCollection(), "show_ftab_tree" );
    showPTabAction = 
      new KToggleAction( i18n( "Show Project Tree" ), 0,
                         this, SLOT( slotShowPTabDock() ),
                         actionCollection(), "show_ptab_tree" );
    showSTabAction = 
      new KToggleAction( i18n( "Show Structure Tree" ), 0,
                         this, SLOT( slotShowSTabDock() ),
                         actionCollection(), "show_stab_tree" );
    showDTabAction = 
      new KToggleAction( i18n( "Show Documentation Tree" ), 0,
                         this, SLOT( slotShowDTabDock() ),
                         actionCollection(), "show_dtab_tree" );
                         
//    showTreeAction = 
//      new KToggleAction( i18n( "Show &Tree" ), "tree_win", CTRL+Key_T,
//                         this, SLOT( slotShowLeftDock() ),
//                         actionCollection(), "show_tree" );
                         
    showMessagesAction = 
      new KToggleAction( i18n( "Show &Messages" ), "output_win", CTRL+Key_M,
                         this, SLOT( slotShowBottDock() ),
                         actionCollection(), "show_messages" );
                           
    KToggleAction *showPreviewAction = 
      new KToggleAction( i18n( "Pr&eview" ), "preview", Key_F6,
                         this, SLOT( slotShowPreview() ),
                         actionCollection(), "show_preview" );
    
//    showTreeAction    ->setChecked( true );
    showMessagesAction->setChecked( true );
    showPreviewAction ->setChecked( false );
                           
       backAction = KStdAction::back   ( htmlPartDoc, SLOT( back() ),    actionCollection(), "doc_back" );
    forwardAction = KStdAction::forward( htmlPartDoc, SLOT( forward() ), actionCollection(), "doc_forward" );

    (void) new KAction( i18n( "&Reload preview" ), "reload",
                        KStdAccel::key(KStdAccel::Reload),
                        this, SLOT( slotViewRepaint() ),
                        actionCollection(), "reload" );
                        
    (void) new KAction( i18n( "View With Net&scape" ), "netscape", 0,
                        view, SLOT( slotViewInNetscape() ),
                        actionCollection(), "view_with_netscape" );
                        
    (void) new KAction( i18n( "View With &Konqueror" ), "konqueror", 0,
                        view, SLOT( slotViewInKFM() ),
                        actionCollection(), "view_with_konqueror" );
                        
    (void) new KAction( i18n( "View With L&ynx" ), "terminal", 0,
                        view, SLOT( slotViewInLynx() ),
                        actionCollection(), "view_with_lynx" );

    (void) new KAction( i18n( "&Previous File" ), "1leftarrow", CTRL+ALT+Key_Right,
                        this, SLOT( slotFilePrev() ),
                        actionCollection(), "previous_file" );
                        
    (void) new KAction( i18n( "&Next File" ), "1rightarrow", CTRL+ALT+Key_Left,
                        this, SLOT( slotFileNext() ),
                        actionCollection(), "next_file" );

    // Project actions
    //
    (void) new KAction( i18n( "&New Project..." ), SmallIcon("idea"), 0, 
                        project, SLOT( newProject() ),
                        actionCollection(), "new_project" );
                        
    (void) new KAction( i18n( "&Open Project..." ), UserIcon("openprj"), 0,
                        project, SLOT( openProject() ),
                        actionCollection(), "open_project" );
                        
    project -> projectRecent =
      KStdAction::openRecent(project, SLOT(openProject(const KURL&)),
                             actionCollection(), "project_open_recent");
    
    closeprjAction =  new KAction( i18n( "&Close Project" ), SmallIcon("fileclose"), 0, 
                         project, SLOT( closeProject() ),
                         actionCollection(), "close_project" );

    insertFileAction = new KAction( i18n( "&Insert File(s)..." ), 0, 
                        project, SLOT( addFiles() ),
                        actionCollection(), "insert_file" );
                        
    insertDirAction = new KAction( i18n( "&Insert Directory..." ), 0, 
                        project, SLOT( addDirectory() ),
                        actionCollection(), "insert_directory" );
                        
    rescanPrjDirAction = new KAction( i18n( "&Rescan project directory" ), SmallIcon("reload"), 0, 
                        project, SLOT( slotRescanPrjDir() ),
                        actionCollection(), "rescan_prjdir" );

    uploadProjectAction = new KAction( i18n( "&Upload Project..." ), Key_F8, 
                        project, SLOT( upload() ),
                        actionCollection(), "upload_project" );
                        
    projectOptionAction = new KAction( i18n( "&Project Options..." ), Key_F7, 
                        project, SLOT( options() ),
                        actionCollection(), "project_options" );
    
    // Options actions
    //
    KStdAction::showToolbar  ( this, SLOT( slotViewToolBar() ),   actionCollection(), "view_toolbar" );
    KStdAction::showStatusbar( this, SLOT( slotViewStatusBar() ), actionCollection(), "view_statusbar" );
    
    (void) new KAction( i18n( "&Highlightings and Fonts" ), 0, 
                        doc, SLOT( highlightings() ),
                        actionCollection(), "highlight_options" );
                        
    hlSelectAction =
      new KSelectAction(i18n("Highlight &Mode"), 0, actionCollection(), "set_highlight");
      
    QStringList list;
    for (int z = 0; z < HlManager::self()->highlights(); z++)
        list.append(i18n(HlManager::self()->hlName(z)));
        
    hlSelectAction->setItems(list);
      
    eolSelectAction = 
      new KSelectAction(i18n("&End Of Line"),    0, actionCollection(), "set_eol");
    
    list.clear();
    list.append("&Unix");
    list.append("&Macintosh");
    list.append("&Windows/Dos");
    
    eolSelectAction->setItems(list);
        
    connect(eolSelectAction, SIGNAL(activated(int)), doc, SLOT(setEol(int)));  
    connect(hlSelectAction,  SIGNAL(activated(int)), doc, SLOT(setHl (int)));
    
    (void) new KAction( i18n( "Configure &Editor..." ), SmallIcon("configure"), 0, 
                        doc, SLOT( editorOptions() ),
                        actionCollection(), "editor_options" );
                        
    (void) new KAction( i18n( "Configure &Actions..." ), UserIcon("ball"),0, 
                        this, SLOT( slotOptionsConfigureActions() ),
                        actionCollection(), "conf_actions" );

    KStdAction::keyBindings      ( this, SLOT( slotOptionsConfigureKeys() ),     actionCollection(), "keys_bind" );
    KStdAction::configureToolbars( this, SLOT( slotOptionsConfigureToolbars() ), actionCollection(), "conf_toolbars" );
    KStdAction::preferences      ( this, SLOT( slotOptions() ),                  actionCollection(), "general_options" );
}
