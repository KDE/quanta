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
#include "widgets/wsplitter.h"
#include "messages/messageoutput.h"

#include "treeviews/fileslistview.h"
#include "treeviews/filestreeview.h"
#include "treeviews/projecttreeview.h"
#include "treeviews/doctreeview.h"
#include "treeviews/structtreeview.h"

#include "phpdebug/phpdebugserversocket.h"

#include "parser/parser.h"

#include "toolbar/toolbars.h"

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

QuantaApp::QuantaApp()
{
  grepDialog	= 0;

  config=kapp->config();

  parser = new Parser();

  initTagDict  ();
  initStatusBar();
  initDocument ();
  initView     ();
  initProject  ();
  
  doc->newDocument();

  initActions();
  createGUI( QString::null, false );
  
  initContextMenu();
  
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
  statusBar()->insertItem(i18n(IDS_STATUS_DEFAULT), ID_STATUS_MSG, 1);
  statusBar()->setItemAlignment( ID_STATUS_MSG, AlignLeft );
  
  statusBar()->insertFixedItem(i18n("Line: 00000 Col: 000"), ID_STATUS_CLM, true);
  statusBar()->changeItem("", ID_STATUS_CLM);
}

void QuantaApp::initContextMenu()
{
//  kwritePopupMenu = (QPopupMenu *)factory()->container("documentContextPopup", this);
//  doc->setRBMenu(kwritePopupMenu);
}

void QuantaApp::initDocument()
{
  doc = new QuantaDoc(this,this);
}

void QuantaApp::initProject()
{
	project = new Project(this);
	
	connect(project, 	SIGNAL(openFile(QString)),
					this, 		SLOT  (slotFileOpen(QString)));
	connect(project, 	SIGNAL(reloadTree(QStringList,bool,bool)),
					pTab, 		SLOT  (slotReloadTree(QStringList,bool,bool)));
	connect(project, 	SIGNAL(setBasePath(QString)),
					pTab, 		SLOT  (slotSetBasePath(QString)));
	connect(project, 	SIGNAL(setProjectName(QString)),
					pTab, 		SLOT  (slotSetProjectName(QString)));
	connect(project,	SIGNAL(closeFiles()),
					doc,			SLOT	(closeAll()));
	connect(project,	SIGNAL(requestOpenedFiles()),
					doc,			SLOT	(slotRequestOpenedFiles()));
	connect(doc,			SIGNAL(openedFiles(QStringList)),
					project,	SLOT  (slotOpenedFiles(QStringList)));
	connect(project,  SIGNAL(addRecentProject(const QString &)),
					this,			SLOT	(addRecentProject(const QString &)));
					
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
}

void QuantaApp::initView()
{ 
  ////////////////////////////////////////////////////////////////////
  // create the main widget here that is managed by KTMainWindow's view-region and
  // connect the widget to your document to display document contents.

	
  topWidgetStack = new QWidgetStack( this );

  hSplit = new WSplitter( topWidgetStack );
  hSplit->setOrientation( Vertical);
  vSplit = new WSplitter( hSplit);
  vSplit->setOrientation( Horizontal);

  leftPanel = new QTabWidget( vSplit);
  leftPanel ->setFocusPolicy(QWidget::NoFocus);

  fTab = new QWidgetStack( leftPanel );

  QStrList topList;
  config->setGroup("General Options");
  config->readListEntry("Top folders", topList);

  fTTab = new FilesTreeView( QDir::homeDirPath()+"/" , topList, fTab );
  fLTab = new FilesListView( QDir::homeDirPath()+"/" , 0L, fTab );

  fTab  ->setFocusPolicy(QWidget::NoFocus);
  fTTab ->setFocusPolicy(QWidget::ClickFocus);
  fLTab ->setFocusPolicy(QWidget::ClickFocus);

  fTab -> addWidget( fTTab, 0 );
  fTab -> addWidget( fLTab, 1 );
  fTab -> raiseWidget( 0 );

  pTab	= new ProjectTreeView( leftPanel);
  dTab	= new DocTreeView( leftPanel);

  pTab -> setFocusPolicy(QWidget::NoFocus);
  dTab -> setFocusPolicy(QWidget::NoFocus);


  sTab = new StructTreeView( parser, config, leftPanel ,"struct");
	sTab ->setFocusPolicy(QWidget::NoFocus);

  leftPanel->addTab( fTab,	"Files"  );
  leftPanel->addTab( pTab,	"Project");
  leftPanel->addTab( sTab,	"Struct" );
  leftPanel->addTab( dTab,	"Docs"   );

  rightWidgetStack = new QWidgetStack( vSplit );

  view = new QuantaView( this, rightWidgetStack );
  view->app = this;
  view->doc = doc;

  bottomWidgetStack = new QWidgetStack( hSplit );

  messageOutput = new MessageOutput( bottomWidgetStack );
  messageOutput ->setFocusPolicy(QWidget::NoFocus);


  vSplit->activate( leftPanel, rightWidgetStack );
  hSplit->activate( vSplit, bottomWidgetStack );

  leftPanel->setMinimumWidth( 1 );

  bottomWidgetStack ->setMinimumHeight( 1 );
	leftPanel					->setMinimumHeight( 1 );
  rightWidgetStack  ->setMinimumHeight( 1 );
	hSplit	 					->setMinimumHeight( 1 );

  vSplit->setPos( 25);
  hSplit->setPos( 100);

  htmlPartRight	 = new WHTMLPart( rightWidgetStack,  "rightHTML");
  htmlPartBottom = new WHTMLPart( bottomWidgetStack, "bottomHTML");
  htmlPartTop    = new WHTMLPart( topWidgetStack, 	 "topHTML");
  htmlPartDoc 	 = new WHTMLPart( rightWidgetStack,  "docHTML");

  bottomWidgetStack->addWidget( messageOutput, 0 );
  bottomWidgetStack->addWidget( htmlPartBottom->view(), 1 );
  bottomWidgetStack->raiseWidget(0);

  rightWidgetStack->addWidget( view, 0 );
  rightWidgetStack->addWidget( htmlPartRight->view(), 1 );
  rightWidgetStack->addWidget( htmlPartDoc  ->view(), 2 );
  rightWidgetStack->raiseWidget(0);

  topWidgetStack->addWidget( hSplit, 0);
  topWidgetStack->addWidget( htmlPartTop->view(), 1 );
  topWidgetStack->raiseWidget(0);

  doc->addView    ( view );
  setCentralWidget( topWidgetStack );
  setCaption      ( doc->getTitle());

  connect( 	fTTab,SIGNAL(openFile(QString)),
  					this, SLOT(slotFileOpen(QString)));
  connect( 	fLTab,SIGNAL(openFile(QString)),
  					this, SLOT(slotFileOpen(QString)));
  					
  connect( 	fTTab,SIGNAL(openImage(QString)),
  					this, SLOT(slotImageOpen(QString)));
  connect( 	fLTab,SIGNAL(openImage(QString)),
  					this, SLOT(slotImageOpen(QString)));
  					
  connect(	fLTab,SIGNAL(changeMode()),
  					this, SLOT(slotSwapLeftPanelMode()));
  connect(	fTTab,SIGNAL(changeMode()),
  					this, SLOT(slotSwapLeftPanelMode()));
  					
  connect( 	pTab, SIGNAL(openFile(QString)),
  					this, SLOT(slotFileOpen(QString)));
  connect( 	pTab, SIGNAL(openImage(QString)),
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
  					
  connect(  htmlPartRight,  SIGNAL(onURL(const QString&)), this, SLOT(slotStatusMsg(const QString&)));
  connect(  htmlPartBottom, SIGNAL(onURL(const QString&)), this, SLOT(slotStatusMsg(const QString&)));
  connect(  htmlPartTop,    SIGNAL(onURL(const QString&)), this, SLOT(slotStatusMsg(const QString&)));
  connect(  htmlPartDoc,    SIGNAL(onURL(const QString&)), this, SLOT(slotStatusMsg(const QString&)));

  connect(  htmlPartDoc,    SIGNAL(updateStatus(bool, bool)), SLOT(updateNavButtons( bool, bool)));

  connect( view, SIGNAL(newCurPos()), this, SLOT(slotNewLineColumn()));

  connect( sTab, SIGNAL(newCursorPosition(int,int)), SLOT(setCursorPosition(int,int)));
  connect( sTab, SIGNAL(onTag(const QString &)), SLOT( slotStatusHelpMsg(const QString &)));
  connect( sTab, SIGNAL(selectArea(int,int,int,int)), SLOT( selectArea(int,int,int,int)));
  connect( sTab, SIGNAL(needReparse()), SLOT(reparse()));

  connect( dTab, SIGNAL(openURL(QString)), SLOT(openDoc(QString)));

  connect( leftPanel, SIGNAL(currentChanged(QWidget*)), this, SLOT( slotLeftTabChanged(QWidget*)));
}


WHTMLPart * QuantaApp::htmlPart()
{
  WHTMLPart *res = 0L;

  if ( previewPosition == "Bottom" )
     res = htmlPartBottom;

  if ( previewPosition == "Right" )
     res = htmlPartRight;

  if ( previewPosition == "FWSpace" )
     res = htmlPartTop;

  return res;

}


QWidgetStack *QuantaApp::widgetStackOfHtmlPart()
{
  QWidgetStack *res = 0L;

  if ( previewPosition == "Bottom" )
     res = bottomWidgetStack;

  if ( previewPosition == "Right" )
     res = rightWidgetStack;

  if ( previewPosition == "FWSpace" )
     res = topWidgetStack;

  return res;

}

void QuantaApp::addRecentFile(const QString &file)
{
  if(recentFiles.find(file) == -1)
  {
    if( recentFiles.count() < 5)
    {
      recentFiles.insert(0, file);
    }
    else
    {
      recentFiles.remove(4);
      recentFiles.insert(0, file);
    }
/*    
    recentFilesMenu->clear();
    for ( int i=0 ; i < (int) recentFiles.count(); i++)
    {
      recentFilesMenu->insertItem(recentFiles.at(i));
    }
*/    
  }
}

void QuantaApp::addRecentProject(const QString &file)
{
  if(recentProjects.find(file) == -1)
  {
    if( recentProjects.count() < 5)
    {
      recentProjects.insert(0, file);
    }
    else
    {
      recentProjects.remove(4);
      recentProjects.insert(0, file);
    }
/*    
    recentProjectsMenu->clear();
    for ( int i=0 ; i < (int) recentProjects.count(); i++)
    {
      recentProjectsMenu->insertItem(recentProjects.at(i));
    }
*/    
  }
}

void QuantaApp::openDocumentFile(const char* _cmdl)
{
  slotStatusMsg(i18n("Opening file..."));

  doc->openDocument(_cmdl);

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}


QuantaDoc *QuantaApp::getDocument() const
{
  return doc;
}

void QuantaApp::saveOptions()
{	
  config->setGroup("General Options");
  doc->write()->writeConfig(config);
  config->writeEntry("Geometry", size());

  config->writeEntry("Show Statusbar",statusBar()->isVisible());
  config->writeEntry("Recent Files", recentFiles);
  config->writeEntry("Recent Projects", recentProjects);

  config->writeEntry("Html mask", fileMaskHtml );
  config->writeEntry("Images mask", fileMaskImage );
  config->writeEntry("Php mask", fileMaskPhp );
  config->writeEntry("Java mask", fileMaskJava );
  config->writeEntry("Text mask", fileMaskText );

  config->writeEntry("Capitals for tags", tagsCapital);
  config->writeEntry("Capitals for attr", attrCapital);
  config->writeEntry("Use close tag if optional", useCloseTag);

  config->writeEntry("Preview position", previewPosition);

  config->writeEntry("Left panel mode", fTab->id( fTab->visibleWidget()));

  config->writeEntry("HSplit position", hSplit->getPos() );
  config->writeEntry("VSplit position", vSplit->getPos() );

  config->writeEntry("Follow Cursor", sTab->followCursor() );

  config->writeEntry("PHP Debugger Port", phpDebugPort );
  
  // save list of open files

  QStrList fileList;
  QDictIterator<Document> it( *(doc->docList) ); // iterator for dict

  while ( Document *twrite = it.current() ) {
    if ( twrite->hasFileName() )
    	fileList.append( twrite->fileName() );
    ++it;
  }

  config->writeEntry("List of opened files", fileList);
  config->writeEntry("Top folders",          fTTab->dirList);

  if ( project->hasProject() )
  {
    project->saveProject();
    config->writeEntry("Last Project", project->projectFileName);
  }
  else config->writeEntry("Last Project", "");
}


void QuantaApp::readOptions()
{
	
  config->setGroup("General Options");

  fileMaskHtml  = config->readEntry("Html mask", 	fileMaskHtml)	+" ";
  fileMaskImage = config->readEntry("Images mask",fileMaskImage)+" ";
  fileMaskPhp   = config->readEntry("Php mask", 	fileMaskPhp)	+" ";
  fileMaskJava  = config->readEntry("Java mask", 	fileMaskJava)	+" ";
  fileMaskText  = config->readEntry("Text mask", 	fileMaskText)	+" ";

  tagsCapital = config->readBoolEntry("Capitals for tags", false);
  attrCapital = config->readBoolEntry("Capitals for attr", false);
  useCloseTag = config->readBoolEntry("Use close tag if optional", true);

  previewPosition   = config->readEntry("Preview position","Right");

  int mode = config->readNumEntry("Left panel mode", 0);
  if ( mode == 0 || mode == 1 ) fTab->raiseWidget(mode);

  hSplit->setPos( config->readNumEntry("HSplit position", 850) );
  vSplit->setPos( config->readNumEntry("VSplit position", 300 ) );
  
  if ( hSplit->getPos() < 1000 )
  {
#warning Check action for message output    
    int pos = hSplit->getPos();
    hSplit->setPos( (pos > 850) ? 850 : pos);
  }

  phpDebugPort = config->readNumEntry("PHP Debugger Port", 7869 );
/*  
  if ( vSplit->getPos() == 0 )
        checkCommand( ID_VIEW_TREE, false );
  else  checkCommand( ID_VIEW_TREE, true  );
*/
  sTab->setFollowCursor( config->readBoolEntry("Follow Cursor", true ) );

  bool bViewStatusbar = config->readBoolEntry("Show Statusbar", true);

#warning Check for statusbar  
  
  if(!bViewStatusbar)
  {
//    enableStatusBar(KStatusBar::Hide);
  }

  // initialize the recent file list
  recentFiles.setAutoDelete(true);
  config->readListEntry("Recent Files", recentFiles);
	
  recentProjects.setAutoDelete(true);
  config->readListEntry("Recent Projects", recentProjects);

#warning TODO reading recent files
/*
  for (int i=0; i < (int) recentFiles.count(); i++)
    recentFilesMenu->insertItem(recentFiles.at(i));

  for (int i=0; i < (int) recentProjects.count(); i++)
    recentProjectsMenu->insertItem(recentProjects.at(i));
*/

  QSize size=config->readSizeEntry("Geometry");
  if(!size.isEmpty())
  {
    resize(size);
  } else
  	resize( QSize(800,580) );
}

void QuantaApp::openLastFiles()
{
  config->setGroup("General Options");

  QString projectFileName = config->readEntry("Last Project");
  QFileInfo fi( projectFileName );

  if ( projectFileName.isEmpty() || !fi.exists() )
  {	  	
	  QStrList fileList;
  	config->readListEntry("List of opened files",fileList);

	  for ( fileList.last();fileList.current();fileList.prev() )
      doc->openDocument( fileList.current() );
  }
  else {
  	project->loadProject( projectFileName );
  	leftPanel-> showPage( (QWidget *)pTab );
  }

  if ( doc->docList->count() == 0 ) // no documents opened
  {
  	
  }
}

void QuantaApp::saveProperties(KConfig *)
{
    doc->saveAll();
}


void QuantaApp::readProperties(KConfig *)
{
}		

bool QuantaApp::queryClose()
{
  return true;
}

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

  config->readListEntry("TagsCore", *tagsCore);
  config->readListEntry("TagsI18n", *tagsI18n);
  config->readListEntry("TagsScript", *tagsScript);
  config->readListEntry("Quoted Attribs", *quotedAttribs);

  config->readListEntry("Core", *lCore);
  config->readListEntry("I18n", *lI18n);
  config->readListEntry("Scripts", *lScript);

  config->readListEntry("TagsList", *tagsList);

  config->readListEntry("Single tags", *singleTags);
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
                        
#warning TODO recent files
    KStdAction::close  ( this, SLOT( slotFileClose()), actionCollection() );

    (void) new KAction( i18n( "Close All" ), 0, this, 
                        SLOT( slotFileCloseAll() ),
                        actionCollection(), "close_all" );
                        
    (void) new KAction( i18n( "&Save" ), UserIcon("save"), KStdAccel::key(KStdAccel::Save),
                        this, SLOT( slotFileSave() ),
                        actionCollection(), "file_save" );
    
    KStdAction::saveAs( this, SLOT( slotFileSaveAs() ), actionCollection() );

    (void) new KAction( i18n( "Save All..." ), UserIcon("save_all"), 0,
                        this, SLOT( slotFileSaveAll() ),
                        actionCollection(), "save_all" );

    KStdAction::quit( this, SLOT( slotFileQuit() ), actionCollection() );

    // Edit actions
    //
    (void) new KAction( i18n( "&Undo" ), UserIcon("undo"), KStdAccel::key(KStdAccel::Undo),
                        this, SLOT( slotEditUndo() ),
                        actionCollection(), "edit_undo" );
                        
    (void) new KAction( i18n( "&Redo" ), UserIcon("redo"), KStdAccel::key(KStdAccel::Redo),
                        this, SLOT( slotEditRedo() ),
                        actionCollection(), "edit_redo" );

    KAction *undoRedo 
      = new KAction( i18n( "Undo/Redo &History..."), 0, 
                     this, SLOT( slotURedoHistory()),
                     actionCollection(), "undo_history" );
                                     
    undoRedo->setGroup( "edit_undo_merge" );
    
    (void) new KAction( i18n( "Cu&t" ), UserIcon("cut"), KStdAccel::key(KStdAccel::Cut),
                        this, SLOT( slotEditCut() ),
                        actionCollection(), "edit_cut" );
                        
    (void) new KAction( i18n( "&Copy" ), UserIcon("copy"), KStdAccel::key(KStdAccel::Copy),
                        this, SLOT( slotEditCopy() ),
                        actionCollection(), "edit_copy" );   
                        
    (void) new KAction( i18n( "&Paste" ), UserIcon("paste"), KStdAccel::key(KStdAccel::Paste),
                        this, SLOT( slotEditPaste() ),
                        actionCollection(), "edit_paste" );                    
                        
    KStdAction::selectAll( this, SLOT( slotEditSelectAll()), actionCollection());

    (void) new KAction(  i18n( "&Unselect All"),       0, this, 
                         SLOT( slotEditDeselectAll()),
                         actionCollection(), "unselect_all" );
                         
    (void) new KAction(  i18n( "&Invert Selection"),   0, this, 
                         SLOT( slotEditInvertSelect()),
                         actionCollection(), "invert_selection" );
                         
    (void) new KAction(  i18n( "&Vertical Selection"), 0, this, 
                         SLOT( slotEditVerticalSelect()),
                         actionCollection(), "vertical_selection" );

    (void) new KAction( i18n( "&Find" ), UserIcon("find"), KStdAccel::key(KStdAccel::Find),
                        this, SLOT( slotEditSearch() ),
                        actionCollection(), "edit_find" );
                        
    KStdAction::findNext( this, SLOT( slotEditSearchAgain()), actionCollection());
    KStdAction::replace ( this, SLOT( slotEditReplace()),     actionCollection());

    (void) new KAction( i18n( "Find In Files" ), 
                        UserIcon("find"), CTRL+ALT+Key_F,
                        this, SLOT( slotEditFindInFiles() ),
                        actionCollection(), "find_in_files" );

    // Tool actions
    //
    (void) new KAction( i18n( "&Go To Line..." ), CTRL+Key_G, 
                        this, SLOT( slotEditGotoLine() ),
                        actionCollection(), "goto_line" );

    (void) new KAction( i18n( "&Indent" ), ALT+SHIFT+Key_Right, 
                        this, SLOT( slotEditIndent() ),
                        actionCollection(), "indent" );
                        
    (void) new KAction( i18n( "&Unindent" ), ALT+SHIFT+Key_Left, 
                        this, SLOT( slotEditUnindent() ),
                        actionCollection(), "unindent" );
                        
    (void) new KAction( i18n( "&Clean Indentation" ), 0, 
                        this, SLOT( slotEditCleanIndent() ),
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
    KToggleAction *showTree = 
      new KToggleAction( i18n( "Show &Tree" ), "tree_win", CTRL+Key_T,
                         this, SLOT( slotShowLeftPanel() ),
                         actionCollection(), "show_tree" );
                         
    KToggleAction *showMessages 
      = new KToggleAction( i18n( "Show &Messages" ), "output_win", CTRL+Key_M,
                           this, SLOT( slotViewMessages() ),
                           actionCollection(), "show_messages" );
                           
    KToggleAction *showPreview 
      = new KToggleAction( i18n( "Pr&eview" ), "preview", Key_F6,
                           this, SLOT( slotShowPreview() ),
                           actionCollection(), "show_preview" );
    
    showTree    ->setChecked( true );
    showMessages->setChecked( true );
    showPreview ->setChecked( false );
                           
    KStdAction::back   ( htmlPartDoc, SLOT( back() ),    actionCollection(), "doc_back" );
    KStdAction::forward( htmlPartDoc, SLOT( forward() ), actionCollection(), "doc_forward" );

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

#warning TODO: Recent projects
    
    (void) new KAction( i18n( "&Close Project" ), SmallIcon("fileclose"), 0, 
                        project, SLOT( closeProject() ),
                        actionCollection(), "close_project" );

    (void) new KAction( i18n( "&Insert File(s)..." ), 0, 
                        project, SLOT( addFiles() ),
                        actionCollection(), "insert_file" );
                        
    (void) new KAction( i18n( "&Insert Directory..." ), 0, 
                        project, SLOT( addDirectory() ),
                        actionCollection(), "insert_directory" );
                        
    (void) new KAction( i18n( "&Rescan project directory" ), SmallIcon("reload"), 0, 
                        project, SLOT( slotRescanPrjDir() ),
                        actionCollection(), "rescan_prjdir" );

    (void) new KAction( i18n( "&Upload Project..." ), Key_F8, 
                        project, SLOT( upload() ),
                        actionCollection(), "upload_project" );
                        
    (void) new KAction( i18n( "&Project Options..." ), Key_F7, 
                        project, SLOT( options() ),
                        actionCollection(), "project_options" );
    
    // Options actions
    //
    KStdAction::showToolbar  ( this, SLOT( slotViewToolBar() ),   actionCollection(), "view_toolbar" );
    KStdAction::showStatusbar( this, SLOT( slotViewStatusBar() ), actionCollection(), "view_statusbar" );
    
    (void) new KAction( i18n( "&Highlightings and Fonts" ), 0, 
                        doc->write(), SLOT( hlDlg() ),
                        actionCollection(), "highlight_options" );
                        
    KSelectAction *setHighlight =
      new KSelectAction(i18n("Highlight &Mode"), 0, actionCollection(), "set_highlight");
      
    QStringList list;
    for (int z = 0; z < HlManager::self()->highlights(); z++)
        list.append(i18n(HlManager::self()->hlName(z)));
        
    setHighlight->setItems(list);
      
    KSelectAction *setEndOfLine = 
      new KSelectAction(i18n("&End Of Line"),    0, actionCollection(), "set_eol");
    
    list.clear();
    list.append("&Unix");
    list.append("&Macintosh");
    list.append("&Windows/Dos");
    setEndOfLine->setItems(list);
        
    connect(setEndOfLine, SIGNAL(activated(int)), this, SLOT(slotSetEol(int)));  
    connect(setHighlight, SIGNAL(activated(int)), this, SLOT(slotSetHl (int)));
    
    (void) new KAction( i18n( "Configure &Editor..." ), SmallIcon("configure"), 0, 
                        doc, SLOT( editorOptions() ),
                        actionCollection(), "editor_options" );

    KStdAction::keyBindings      ( this, SLOT( slotOptionsConfigureKeys() ),     actionCollection(), "keys_bind" );
    KStdAction::configureToolbars( this, SLOT( slotOptionsConfigureToolbars() ), actionCollection(), "conf_toolbars" );
    KStdAction::preferences      ( this, SLOT( slotOptions() ),                  actionCollection(), "general_options" );
}
