/***************************************************************************
                          quanta.cpp  -  description
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

// include files for QT
#include <qdir.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qtabwidget.h>
#include <qwidgetstack.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qtimer.h>

// include files for KDE
#include <kaccel.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <khtmlview.h>
#include <kstddirs.h>
#include <kstatusbar.h>

// application specific includes

#include "quanta.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "resource.h"
#include "document.h"


#include "project/project.h"

#include "widgets/wtoolbar.h"
#include "widgets/whtmlpart.h"
#include "messages/messageoutput.h"

#include "treeviews/fileslistview.h"
#include "treeviews/filestreeview.h"
#include "treeviews/projecttreeview.h"
#include "treeviews/doctreeview.h"
#include "treeviews/structtreeview.h"
#include "treeviews/templatestreeview.h"

#include "plugins/php3dbg/debugger.h"
#include "plugins/php4dbg/debugger.h"

#include "parser/parser.h"


QString fileMaskHtml   = "*.*html *.*htm *.php* *.asp *.cfm *.css *.inc* *.*HTML *.*HTM *.PHP* *.ASP *.CFM *.CSS *.INC* *.xml *.XML";
QString fileMaskPhp   = "*.*PHP* *.*php* ";
QString fileMaskJava  = "*.jss *.js *.JSS *.JS ";
QString fileMaskText  = "*.txt; *.TXT";
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
  grepDialog  = 0L;
  exitingFlag = false;

  config=kapp->config();

  parser = new Parser();
  
  initTagDict  ();
  initStatusBar();
  initDocument ();
  initView     ();
  initProject  ();
  
  initActions();
  createGUI( QString::null, false );

  pm_set  = (QPopupMenu*)guiFactory()->container("settings", this);
  connect(pm_set, SIGNAL(aboutToShow()), this, SLOT(settingsMenuAboutToShow()));

  pm_bookmark  = (QPopupMenu*)guiFactory()->container("bookmarks", this);
  connect(pm_bookmark, SIGNAL(aboutToShow()), this, SLOT(bookmarkMenuAboutToShow()));

  QPopupMenu* pm_view = (QPopupMenu*)guiFactory()->container("qview", this);
  connect(pm_view,SIGNAL(aboutToShow()), this, SLOT(viewMenuAboutToShow()));

  readOptions();

  connect( messageOutput, SIGNAL(clicked(const QString&,int)),
           this,          SLOT(gotoFileAndLine(const QString&,int)));

  QTimer *t = new QTimer( this );
  connect( t, SIGNAL(timeout()), SLOT(reparse()) );
  t->start( 2000, false );
  slotFileNew();

  setHighlight = view->write()->kate_doc->hlActionMenu (i18n("&Highlight Mode"), actionCollection(), "set_highlight");

}

QuantaApp::~QuantaApp()
{
}

void QuantaApp::initStatusBar()
{
  statusbarTimer = new QTimer(this);
  connect(statusbarTimer,SIGNAL(timeout()),this,SLOT(statusBarTimeout()));
  
  statusBar()->insertItem(i18n(IDS_DEFAULT),IDS_STATUS, 1);
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

void QuantaApp::initDocument()
{
  doc = new QuantaDoc(this,this);  
  connect(doc, SIGNAL(newStatus()),    this, SLOT(slotNewStatus()));
}

void QuantaApp::initProject()
{
  project = new Project(this);
  
  connect(project,  SIGNAL(openFile    (const KURL &)),
          this,     SLOT  (slotFileOpen(const KURL &)));
  connect(project,  SIGNAL(reloadTree(QStringList,bool,bool)),
          pTab,     SLOT  (slotReloadTree(QStringList,bool,bool)));
  connect(project,  SIGNAL(setBasePath(QString)),
          pTab,     SLOT  (slotSetBasePath(QString)));
  connect(project,  SIGNAL(setProjectName(QString)),
          pTab,     SLOT  (slotSetProjectName(QString)));
  connect(project,  SIGNAL(closeFiles()),
          doc,      SLOT  (closeAll()));
  connect(project,  SIGNAL(showTree()),
          this,     SLOT  (slotShowProjectTree()));
  connect(project,  SIGNAL(removeFromProject(int)),
          pTab,     SLOT  (slotRemoveFromProject(int)));

  connect(fLTab,    SIGNAL(insertDirInProject(QString)),
          project,  SLOT  (addDirectory(QString)));
  connect(fTTab,    SIGNAL(insertDirInProject(QString)),
          project,  SLOT  (addDirectory(QString)));
          
  connect(fLTab,    SIGNAL(insertFileInProject(QString)),
          project,  SLOT  (insertFile(QString)));
  connect(fTTab,    SIGNAL(insertFileInProject(QString)),
          project,  SLOT  (insertFile(QString)));
          
  connect(pTab,     SIGNAL(renameFileInProject(QString)),
          project,  SLOT  (slotRenameFile(QString)));
  connect(pTab,     SIGNAL(renameFolderInProject(QString)),
          project,  SLOT  (slotRenameFolder(QString)));
  connect(pTab,     SIGNAL(removeFileFromProject(QString)),
          project,  SLOT  (slotRemoveFile(QString)));
  connect(pTab,     SIGNAL(removeFolderFromProject(QString)),
          project,  SLOT  (slotRemoveFolder(QString)));
  connect(pTab,     SIGNAL(uploadSingleFile(QString)),
          project,  SLOT  (uploadFile(QString)));
  connect(pTab,     SIGNAL(uploadSingleFolder(QString)),
          project,  SLOT  (uploadFile(QString)));
          
  connect(project,  SIGNAL(selectMessageWidget()),
          this,     SLOT  (slotMessageWidgetEnable()));
  connect(project,  SIGNAL(disableMessageWidget()),
          this,     SLOT  (slotMessageWidgetDisable()));
          
  connect(project,  SIGNAL(messages(QString)),
          messageOutput, SLOT(showMessage(QString)));

  connect(project,  SIGNAL(saveAllFiles()),
          this, SLOT(slotFileSaveAll()));
  connect(project,  SIGNAL(newStatus()),
          this, SLOT(slotNewStatus()));

  connect(project,  SIGNAL(newProjectLoaded()),
          this,     SLOT  (slotNewProjectLoaded()));

}

void QuantaApp::initView()
{ 
  ////////////////////////////////////////////////////////////////////
  // create the main widget here that is managed by KTMainWindow's view-region and
  // connect the widget to your document to display document contents.

  maindock = createDockWidget( "Editor", UserIcon("textarea"  ), 0L, i18n("Editor"));
  bottdock = createDockWidget( "Output", UserIcon("output_win"), 0L, i18n("Output"));
  
  ftabdock = createDockWidget( "Files",  UserIcon("ftab"),     0L, "");
  ftabdock->setToolTipString(i18n("Files tree view"));
  ptabdock = createDockWidget( "Project",UserIcon("ptab"),     0L, "");
  ptabdock->setToolTipString(i18n("Project tree view"));
  ttabdock = createDockWidget( "Templates",UserIcon("ttab"),     0L, "");
  ttabdock->setToolTipString(i18n("Templates tree view"));
  stabdock = createDockWidget( "Struct", BarIcon ("view_sidetree"),0L, "");
  stabdock->setToolTipString(i18n("Structure view (DOM tree)"));
  dtabdock = createDockWidget( "Docs",   BarIcon ("contents2"),    0L, "");
  dtabdock->setToolTipString(i18n("Documentation"));

  QStrList topList;
  config->setGroup("General Options");
  config->readListEntry("Top folders", topList);

  fTab = new QWidgetStack( ftabdock );
  
  fTTab = new FilesTreeView( QDir::homeDirPath()+"/" , topList, fTab );
  fLTab = new FilesListView( QDir::homeDirPath()+"/" , 0L, fTab );

  fTab -> addWidget( fTTab, 0 );
  fTab -> addWidget( fLTab, 1 );
  fTab -> raiseWidget( 0 );

  pTab  = new ProjectTreeView( ptabdock );

  tTab = new TemplatesTreeView( "" , ttabdock );


  dTab  = new DocTreeView    ( dtabdock );

  sTab = new StructTreeView( parser, config, stabdock ,"struct");

  rightWidgetStack = new QWidgetStack( maindock );

  view = new QuantaView( this, rightWidgetStack );   
  view->app = this;
  view->doc = doc;
  
  messageOutput = new MessageOutput( bottdock );

  maindock ->setWidget( rightWidgetStack );
  bottdock ->setWidget( messageOutput );
  ftabdock ->setWidget( fTab );
  ptabdock ->setWidget( pTab );
  ttabdock ->setWidget( tTab );
  stabdock ->setWidget( sTab );
  dtabdock ->setWidget( dTab );
  
  fTab  ->setFocusPolicy(QWidget::NoFocus);
  fTTab ->setFocusPolicy(QWidget::ClickFocus);
  fLTab ->setFocusPolicy(QWidget::ClickFocus);
  pTab  ->setFocusPolicy(QWidget::NoFocus);
  tTab  ->setFocusPolicy(QWidget::NoFocus);
  sTab  ->setFocusPolicy(QWidget::NoFocus);
  dTab  ->setFocusPolicy(QWidget::NoFocus);
  
  maindock      ->setFocusPolicy(QWidget::StrongFocus);
  messageOutput ->setFocusPolicy(QWidget::NoFocus);
  
  rightWidgetStack  ->setMinimumHeight( 1 );

  htmlpart       = new WHTMLPart( rightWidgetStack,  "rightHTML");
  htmlPartDoc    = new WHTMLPart( rightWidgetStack,  "docHTML");

  rightWidgetStack->addWidget( view, 0 );
  rightWidgetStack->addWidget( htmlpart   ->view(), 1 );
  rightWidgetStack->addWidget( htmlPartDoc->view(), 2 );
  rightWidgetStack->raiseWidget(0);

  connect(   fTTab,SIGNAL(openFile  (const KURL &)),
            this, SLOT(slotFileOpen(const KURL &)));
  connect(   fLTab,SIGNAL(openFile  (const KURL &)),
            this, SLOT(slotFileOpen(const KURL &)));
            
  connect(   fTTab,SIGNAL(openImage(QString)),
            this, SLOT  (slotImageOpen(QString)));
  connect(   fLTab,SIGNAL(openImage(QString)),
            this, SLOT  (slotImageOpen(QString)));
            
  connect(  fLTab,SIGNAL(changeMode()),
            this, SLOT(slotSwapLeftPanelMode()));
  connect(  fTTab,SIGNAL(changeMode()),
            this, SLOT(slotSwapLeftPanelMode()));
            
  connect(   pTab, SIGNAL(openFile  (const KURL &)),
            this, SLOT(slotFileOpen(const KURL &)));
  connect(   pTab, SIGNAL(openImage  (QString)),
            this, SLOT(slotImageOpen(QString)));

  connect(   tTab, SIGNAL(openFile  (const KURL &)),
            this, SLOT(slotFileOpen(const KURL &)));
  connect(   tTab, SIGNAL(insertFile  (QString)),
            this, SLOT(slotInsertFile(QString)));
  connect(   tTab,SIGNAL(insertTag(QString)),
            this, SLOT(slotInsertTag(QString)));

  connect(   fTTab,SIGNAL(insertTag(QString)),
            this, SLOT(slotInsertTag(QString)));
  connect(   fLTab,SIGNAL(insertTag(QString)),
            this, SLOT(slotInsertTag(QString)));
  connect(   pTab,SIGNAL(insertTag(QString)),
            this, SLOT(slotInsertTag(QString)));
            
  connect(   fLTab,SIGNAL(activatePreview()),
            this, SLOT(slotActivatePreview()));
  connect(   fTTab,SIGNAL(activatePreview()),
            this, SLOT(slotActivatePreview()));
  connect(   pTab, SIGNAL(activatePreview()),
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
  ttabdock ->manualDock(ftabdock, KDockWidget::DockCenter);
  stabdock ->manualDock(ftabdock, KDockWidget::DockCenter);
  dtabdock ->manualDock(ftabdock, KDockWidget::DockCenter);
  
  KDockManager *mng = stabdock->dockManager();
  connect(mng, SIGNAL(change()),this,SLOT(slotDockChanged()));
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
  
  config->writeEntry("Show Toolbar",   toolBar("mainToolBar")->isVisible());
  config->writeEntry("Show Statusbar", statusBar()->isVisible());

  config->writeEntry("Html mask",   fileMaskHtml  );
  config->writeEntry("Images mask", fileMaskImage );
  config->writeEntry("Php mask",    fileMaskPhp   );
  config->writeEntry("Java mask",   fileMaskJava  );
  config->writeEntry("Text mask",   fileMaskText  );

  config->writeEntry("Capitals for tags",     tagsCapital);
  config->writeEntry("Capitals for attr",     attrCapital);
  config->writeEntry("Close tag if optional", useCloseTag);

  config->writeEntry("Left panel mode", fTab->id( fTab->visibleWidget()));

  config->writeEntry("Follow Cursor", sTab->followCursor() );

  config->writeEntry("PHP Debugger Port", phpDebugPort );
  
  config->writeEntry("Top folders",          fTTab->dirList);
  config->writeEntry("List of opened files", doc->openedFiles());
  
  config->writeEntry ("Version", VERSION); // version
  
  doc    ->writeConfig(config); // kwrites
  project->writeConfig(config); // project
  
  config->setGroup  ("General Options");
  
  fileRecent->saveEntries(config);
  config->writeEntry ("Enable Debugger", debuggerStyle!="None");
  config->writeEntry ("PHP Debugger style", debuggerStyle);
  
  writeDockConfig();
}


void QuantaApp::readOptions()
{
  config->setGroup("General Options");
  //if (2>config->readNumEntry("Version",0)) config = new KConfig();

  fileMaskHtml  = config->readEntry("Html mask",   fileMaskHtml)  +" ";
  fileMaskImage = config->readEntry("Images mask",fileMaskImage)+" ";
  fileMaskPhp   = config->readEntry("Php mask",   fileMaskPhp)  +" ";
  fileMaskJava  = config->readEntry("Java mask",   fileMaskJava)  +" ";
  fileMaskText  = config->readEntry("Text mask",   fileMaskText)  +" ";

  tagsCapital = config->readBoolEntry("Capitals for tags",     false);
  attrCapital = config->readBoolEntry("Capitals for attr",     false);
  useCloseTag = config->readBoolEntry("Close tag if optional", true);

  previewPosition   = config->readEntry("Preview position","Right");

  phpDebugPort = config->readNumEntry("PHP Debugger Port", 7869);
  
  sTab->setFollowCursor( config->readBoolEntry("Follow Cursor", true));
  
  int mode = config->readNumEntry("Left panel mode", 0);
  if ( mode == 0 || mode == 1 ) fTab->raiseWidget(mode);

  fileRecent ->loadEntries(config);

  QSize s(800,580);
  config->setGroup("General Options");
  resize( config->readSizeEntry("Geometry", &s));

  if (!config->readBoolEntry("Show Toolbar",   true)) {
    toolBar("mainToolBar")    ->hide();
    toolBar("mainEditToolBar")    ->hide();
    toolBar("mainNaviToolBar")->hide();
   showToolbarAction  ->setChecked(false);
 } else
 {
    toolBar("mainToolBar")    ->show();
    toolBar("mainEditToolBar")    ->show();
    toolBar("mainNaviToolBar")->show();
 }
  if (!config->readBoolEntry("Show Statusbar", true))
  {
     statusBar()->hide();
  } else
  {
      statusBar()->show();
     showStatusbarAction->setChecked(true);
 }
 showToolbarAction  ->setChecked(config->readBoolEntry("Show Toolbar",   true));
 showStatusbarAction->setChecked(config->readBoolEntry("Show Statusbar", true));

  readDockConfig();

  showPreviewAction  ->setChecked( false );
  showMessagesAction ->setChecked( bottdock->parent() != 0L );

  doc    ->readConfig(config); // kwrites
  project->readConfig(config); // project
  if (doc->write() !=0 )
  {
    if (doc->write()->isUntitled())
    {
       doc->closeDocument();
      doc->newDocument(KURL());
    }
  }

  debuggerStyle = "None";
  config->setGroup  ("General Options");
  if (config->readBoolEntry("Enable Debugger", true))
    if (config->readEntry("PHP Debugger style","PHP4") == "PHP4") 
         enablePhp4Debug(true);
    else enablePhp3Debug(true);
}

void QuantaApp::enablePhp3Debug(bool enable)
{
  if (enable) {
    dbg3 = new PHP3Debugger( phpDebugPort,0,0);
          
    connect( dbg3,          SIGNAL(newConnect()),
             messageOutput, SLOT(newPhpConnect()) );
    connect( dbg3,          SIGNAL(endConnect()),
             messageOutput, SLOT(endPhpConnect()) );
    connect( dbg3,          SIGNAL(data(QString)),
             messageOutput, SLOT(phpDebug(QString)) );
                 
    if ( !dbg3->ok() ) {
      QString s;
      messageOutput->insertItem("Can't bind port "+
        s.sprintf("%i",phpDebugPort)+" , PHP3 debugger disabled" );
    }
    else {
      QString s;
      messageOutput->insertItem("PHP3 Debugger listens port "+
        s.sprintf("%i",phpDebugPort)+"" );
    }
    debuggerStyle = "PHP3";
  } else delete dbg3;
}

void QuantaApp::enablePhp4Debug(bool enable)
{
  if (enable) {
    dbg4 = new PHP4Debugger(0L,0L);
    connect( dbg4,          SIGNAL(message(QString)),
             messageOutput, SLOT(php4Debug(QString)) );
    dbg4->init();
    debuggerStyle = "PHP4";
  } else delete dbg4;
}

void QuantaApp::openLastFiles()
{
  // we need to check config
  // because project now can be
  // in load stage ( remote prj )
  config->setGroup  ("Projects");
  QString pu = config->readEntry("Last Project");
  
  KURL u(pu);
  bool isPrj = true;
  if ( pu.isEmpty())    isPrj = false;
  if ( u.isMalformed()) isPrj = false;
  
  config->setGroup("General Options");

  QStrList urls;
   config->readListEntry("List of opened files", urls);

  for ( urls.last();urls.current();urls.prev() )
  {
    KURL fu(urls.current());
    
    if ( !isPrj || fu.isLocalFile() ) 
      doc->openDocument( fu );
  }
}

bool QuantaApp::queryExit()
{
  exitingFlag = true;
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

    //Kate actions

//Edit menu
    editUndo = KStdAction::undo(view, SLOT(slotUndo()),actionCollection());
    editRedo = KStdAction::redo(view, SLOT(slotRedo()), actionCollection());

    KStdAction::cut(view, SLOT(slotCut()), actionCollection());
    KStdAction::copy(view, SLOT(slotCopy()), actionCollection()) ;
    KStdAction::paste(view, SLOT(slotPaste()), actionCollection());

    KStdAction::selectAll(view, SLOT(slotSelectAll()), actionCollection());
    KStdAction::deselect(view, SLOT(slotDeselectAll()),actionCollection());
    (void) new KAction( i18n( "&Toggle block selection" ), Key_F4, view,
                        SLOT( toggleVertical() ), actionCollection(), "set_verticalSelect" );


    KStdAction::find(view, SLOT(slotFind()), actionCollection());
    KStdAction::findNext(view, SLOT(slotFindAgain()), actionCollection());
    KStdAction::findPrev(view, SLOT(slotFindAgainB()), actionCollection(), "edit_find_prev");
    KStdAction::replace(view, SLOT(slotReplace()), actionCollection());

    new KAction(i18n("&Indent"), "indent", CTRL+Key_I, view,
                SLOT(slotIndent()), actionCollection(), "edit_indent");
    new KAction(i18n("&Unindent"), "unindent", CTRL+SHIFT+Key_I, view,
                SLOT(slotUnIndent()), actionCollection(), "edit_unindent");

    new KAction(i18n("Co&mment"),  CTRL+Qt::Key_NumberSign,
                view, SLOT(slotComment()), actionCollection(), "edit_comment");
    new KAction(i18n("Unc&omment"),
                CTRL+SHIFT+Qt::Key_NumberSign, view, SLOT(slotUnComment()),
                actionCollection(), "edit_uncomment");
    new KAction(i18n("Apply Word Wrap"), "", 0, view, SLOT(slotApplyWordWrap()),
                actionCollection(), "edit_apply_wordwrap");



//Tools menu
    KStdAction::gotoLine(view, SLOT(slotGotoLine()), actionCollection());
    KStdAction::spelling(view, SLOT(slotSpellcheck()), actionCollection());

//Bookmarks
    bookmarkToggle = new KAction(i18n("Toggle &Bookmark"), Qt::CTRL+Qt::Key_B,
          view, SLOT(toggleBookmark()), actionCollection(), "edit_bookmarkToggle");
    bookmarkClear = new KAction(i18n("Clear Bookmarks"), 0, view,
          SLOT(clearBookmarks()), actionCollection(), "edit_bookmarksClear");

//Settings
  viewBorder =  new KToggleAction(i18n("Show &Icon Border"), Qt::SHIFT+Qt::Key_F9, view,
                    SLOT(toggleIconBorder()), actionCollection(), "view_border");
  viewLineNumbers =  new KToggleAction(i18n("Show &Line Numbers"), Key_F9, view,
                        SLOT(toggleLineNumbers()), actionCollection(), "view_line_numbers");

  (void) new KAction( i18n( "Configure &Editor..." ), SmallIcon("configure"), 0,
                      view, SLOT( editorOptions() ), actionCollection(), "editor_options" );


    setEndOfLine = new KSelectAction(i18n("&End of Line"), 0, actionCollection(),"set_eol");
    connect(setEndOfLine, SIGNAL(activated(int)), view, SLOT(setEol(int)));
    connect(setEndOfLine->popupMenu(), SIGNAL(aboutToShow()), this, SLOT(setEOLMenuAboutToShow()));

    QStringList list;
    list.append("&Unix");
    list.append("&Windows/Dos");
    list.append("&Macintosh");
    setEndOfLine->setItems(list);


    // File actions
    //
    KStdAction::openNew( this, SLOT( slotFileNew()  ), actionCollection());
    KStdAction::open   ( this, SLOT( slotFileOpen() ), actionCollection());
    KStdAction::close  ( this, SLOT( slotFileClose()), actionCollection());

    fileRecent =
      KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)),
                             actionCollection());

    (void) new KAction( i18n( "Close All" ), 0, this, 
                        SLOT( slotFileCloseAll() ),
                        actionCollection(), "close_all" );

    saveAction = KStdAction::save(this, SLOT( slotFileSave() ),actionCollection());
    
    KStdAction::saveAs( this, SLOT( slotFileSaveAs() ), actionCollection() );

    saveAllAction = new KAction( i18n( "Save All..." ), UserIcon("save_all"), SHIFT+KStdAccel::key(KStdAccel::Save),
                        this, SLOT( slotFileSaveAll() ),
                        actionCollection(), "save_all" );

    saveAsLocalTemplateAction = new KAction( i18n( "Save As Local Template" ), 0,
                        this, SLOT( slotFileSaveAsLocalTemplate() ),
                        actionCollection(), "save_local_template" );
    saveAsProjectTemplateAction = new KAction( i18n( "Save As Project Template..." ), 0,
                        this, SLOT( slotFileSaveAsProjectTemplate() ),
                        actionCollection(), "save_project_template" );

    KStdAction::quit( this, SLOT( slotFileQuit() ), actionCollection() );

   (void) new KAction( i18n( "&File list" ), 0,this, SLOT( slotShowOpenFileList() ),
                        actionCollection(), "file_list" );

    // Edit actions
 /*
    KAction *undoRedo 
      = new KAction( i18n( "Undo/Redo &History..."), 0, 
                     doc, SLOT( undoHistory()),
                     actionCollection(), "undo_history" );

    undoRedo->setGroup( "edit_undo_merge" ); */

    (void) new KAction( i18n( "Find In Files" ),
                        UserIcon("find"), CTRL+ALT+Key_F,
                        this, SLOT( slotEditFindInFiles() ),
                        actionCollection(), "find_in_files" );

    // Tool actions

    (void) new KAction( i18n( "Context &Help..." ), CTRL+Key_H, 
                        this, SLOT( contextHelp() ),
                        actionCollection(), "context_help" );
  /*
    (void) new KAction( i18n( "Tag &Attributes" ), ALT+Key_Down,
                        doc, SLOT( slotAttribPopup() ),
                        actionCollection(), "tag_attributes" );
     */
    (void) new KAction( i18n( "&Edit Current Tag..." ), CTRL+Key_E,
                        view, SLOT( slotEditCurrentTag() ),
                        actionCollection(), "edit_current_tag" );

    (void) new KAction( i18n( "&Syntax check" ), 0, 
                        this, SLOT( slotToolSyntaxCheck() ),
                        actionCollection(), "syntax_check" );
    
    // View actions

    showFTabAction =
      new KToggleAction( i18n( "Show Files Tree" ), 0,
                         this, SLOT( slotShowFTabDock() ),
                         actionCollection(), "show_ftab_tree" );
    showPTabAction =
      new KToggleAction( i18n( "Show Project Tree" ), 0,
                         this, SLOT( slotShowPTabDock() ),
                         actionCollection(), "show_ptab_tree" );
    showTTabAction =
      new KToggleAction( i18n( "Show Templates Tree" ), 0,
                         this, SLOT( slotShowTTabDock() ),
                         actionCollection(), "show_ttab_tree" );
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
                           
    showPreviewAction = 
      new KToggleAction( i18n( "Pr&eview" ), "preview", Key_F6,
                         this, SLOT( slotShowPreview() ),
                         actionCollection(), "show_preview" );
    
//       backAction = KStdAction::back   ( this, SLOT( slotBack() ),    actionCollection(), "w_back" );
//    forwardAction = KStdAction::forward( this, SLOT( slotForward() ), actionCollection(), "w_forward" );

    backAction = new KAction ( i18n("Back"), "back", ALT+Key_Left, this, SLOT( slotBack() ),    actionCollection(), "w_back" );
    forwardAction = new KAction ( i18n("Forward"), "forward", ALT+Key_Right, this, SLOT( slotForward() ),    actionCollection(), "w_forward" );

    (void) new KAction( i18n( "&Reload preview" ), "reload",
                        KStdAccel::key(KStdAccel::Reload),
                        this, SLOT( slotViewRepaint() ),
                        actionCollection(), "reload" );
                        
    (void) new KAction( i18n( "View With Net&scape" ), "netscape", ALT+Key_F6,
                        view, SLOT( slotViewInNetscape() ),
                        actionCollection(), "view_with_netscape" );
                        
    (void) new KAction( i18n( "View With &Konqueror" ), "konqueror", CTRL+Key_F6,
                        view, SLOT( slotViewInKFM() ),
                        actionCollection(), "view_with_konqueror" );
                        
    (void) new KAction( i18n( "View With L&ynx" ), "terminal", SHIFT+Key_F6,
                        view, SLOT( slotViewInLynx() ),
                        actionCollection(), "view_with_lynx" );

    (void) new KAction( i18n( "&Previous File" ), "1leftarrow", KStdAccel::back(),
                        this, SLOT( slotFilePrev() ),
                        actionCollection(), "previous_file" );
                        
    (void) new KAction( i18n( "&Next File" ), "1rightarrow", KStdAccel::forward(),
                        this, SLOT( slotFileNext() ),
                        actionCollection(), "next_file" );

    // Project actions
    //
    newPrjAction = new KAction( i18n( "&New Project..." ), 0, 
                        project, SLOT( newProject() ),
                        actionCollection(), "new_project" );
                        
    openPrjAction = new KAction( i18n( "&Open Project..." ), BarIcon("folder_new"), 0,
                        project, SLOT( openProject() ),
                        actionCollection(), "open_project" );
                        
    project -> projectRecent =
      KStdAction::openRecent(project, SLOT(openProject(const KURL&)),
                             actionCollection(), "project_open_recent");
                             
    connect(project,                SIGNAL(checkOpenAction(bool)),
            newPrjAction,           SLOT(setEnabled(bool)));
    connect(project,                SIGNAL(checkOpenAction(bool)),
            openPrjAction,          SLOT(setEnabled(bool)));
    connect(project,                SIGNAL(checkOpenAction(bool)),
            project->projectRecent, SLOT(setEnabled(bool)));
    
    saveprjAction =  new KAction( i18n( "&Save Project" ), SmallIcon("save"), 0,
                         project, SLOT( saveProject() ),
                         actionCollection(), "save_project" );

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
    showToolbarAction   = KStdAction::showToolbar  ( this, SLOT( slotViewToolBar() ),   actionCollection(), "view_toolbar" );
    showStatusbarAction = KStdAction::showStatusbar( this, SLOT( slotViewStatusBar() ), actionCollection(), "view_statusbar" );


    (void) new KAction( i18n( "Configure &Actions..." ), UserIcon("ball"),0,
                        this, SLOT( slotOptionsConfigureActions() ),
                        actionCollection(), "conf_actions" );

    KStdAction::keyBindings      ( this, SLOT( slotOptionsConfigureKeys() ),     actionCollection(), "keys_bind" );
    KStdAction::configureToolbars( this, SLOT( slotOptionsConfigureToolbars() ), actionCollection(), "conf_toolbars" );
    KStdAction::preferences      ( this, SLOT( slotOptions() ),                  actionCollection(), "general_options" );

}
