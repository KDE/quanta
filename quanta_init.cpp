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

#include <stdlib.h>

// include files for QT
#include <qdir.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qtabwidget.h>
#include <qwidgetstack.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qtimer.h>
#include <qdom.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextcodec.h>
#include <qpopupmenu.h>

// include files for KDE
#include <kaccel.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <khtmlview.h>
#include <kstandarddirs.h>
#include <kstatusbar.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kparts/componentfactory.h>
#include <kprogress.h>
#include <kspell.h>

// application specific includes

#include "quanta.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "resource.h"
#include "document.h"


#include "project/project.h"

#include "widgets/whtmlpart.h"
#include "messages/messageoutput.h"

#include "treeviews/fileslistview.h"
#include "treeviews/filestreeview.h"
#include "treeviews/projecttreeview.h"
#include "treeviews/doctreeview.h"
#include "treeviews/structtreeview.h"
#include "treeviews/templatestreeview.h"

#include "plugins/quantaplugininterface.h"
#include "plugins/quantaplugin.h"
#include "plugins/quantakpartplugin.h"
#include "plugins/quantacmdplugin.h"
#include "plugins/quantaplugineditor.h"

#include "plugins/php3dbg/debugger.h"
#include "plugins/php4dbg/debugger.h"        
#include "plugins/spellchecker.h"

#include "parser/parser.h"
#include "dialogs/filemasks.h"

QuantaApp::QuantaApp() : KDockMainWindow(0L,"Quanta")
{
  quantaStarted = true;
  tempFileList.setAutoDelete(true);
  toolbarList.setAutoDelete(true);
  userToolbarsCount = 0;
  baseNode = 0L;
  currentToolbarDTD = QString::null;
  config=kapp->config();

  qConfig.globalDataDir = KGlobal::dirs()->findResourceDir("data","quanta/toolbar/quantalogo.png");
  if (qConfig.globalDataDir.isEmpty())
  {
    quantaStarted = false;
    fprintf(stderr,"***************************************************************************\n");
    fprintf(stderr, i18n("\tQuanta data files were not found.\nYou may forgot to run \"make install\",\nor your KDEDIR, KDEDIRS or PATH is not set correctly.!\n"));
    fprintf(stderr,"***************************************************************************\n");
    QTimer::singleShot(20,kapp, SLOT(quit()));
    return;
  }
  qConfig.enableDTDToolbar = true;

  setHighlight = 0;
  grepDialog  = 0L;
  exitingFlag = false;
  qConfig.spellConfig = new KSpellConfig();
  spellChecker = new SpellChecker();
}

QuantaApp::~QuantaApp()
{
 tempFileList.clear();
 QDictIterator<ToolbarEntry> iter(toolbarList);
 ToolbarEntry *p_toolbar;
 for( ; iter.current(); ++iter )
 {
   p_toolbar = iter.current();
   if (p_toolbar->dom) delete p_toolbar->dom;
   if (p_toolbar->menu)
     delete p_toolbar->menu;
   if (p_toolbar->guiClient) delete p_toolbar->guiClient;
 }
 
 toolbarList.clear();
 delete spellChecker;
}


/** Delayed initialization. */
void QuantaApp::initQuanta()
{
  parser = new Parser();

  initTagDict  ();
  initStatusBar();
  initDocument ();
  initView     ();
  initProject  ();

  initActions();

  readOptions();

//Check for an existing quanatui.rc
  QString uiFileName = locateLocal("appdata","quantaui.rc");
  if (QFileInfo(uiFileName).exists())
  {
    QDomDocument doc;
     QFile f( uiFileName );
		 f.open( IO_ReadOnly );
	   if ( doc.setContent( &f ) )
     {
       f.close();
       QDomElement el = doc.firstChild().toElement();
       if (el.attribute("version","old") != QString(VERSION))
       {
         QString command = "mv "+uiFileName+" "+uiFileName+".old";
         system(QFile::encodeName(command));
       }
     } else
     {
       f.close();
     }
  }


  m_pluginInterface = new QuantaPluginInterface();

  createGUI( QString::null, false /* conserveMemory */ );

  initPlugins  ();
  
  m_tagsMenu = new QPopupMenu(this);
  KAction *editTagAction =
           new KAction( i18n( "&Edit Current Tag..." ), CTRL+Key_E,
                        view, SLOT( slotEditCurrentTag() ),
                        actionCollection(), "edit_current_tag" );
  editTagAction->plug(m_tagsMenu);
  m_tagsMenu->insertSeparator();
  menuBar()->insertItem(i18n("&Tags"),m_tagsMenu,-1,TAGS_MENU_PLACE);                                                                     

  pm_set  = (QPopupMenu*)guiFactory()->container("settings", this);
  connect(pm_set, SIGNAL(aboutToShow()), this, SLOT(settingsMenuAboutToShow()));

  pm_bookmark  = (QPopupMenu*)guiFactory()->container("bookmarks", this);
  connect(pm_bookmark, SIGNAL(
  aboutToShow()), this, SLOT(bookmarkMenuAboutToShow()));

  QPopupMenu* pm_view = (QPopupMenu*)guiFactory()->container("view", this);
  connect(pm_view,SIGNAL(aboutToShow()), this, SLOT(viewMenuAboutToShow()));

  QPopupMenu *toolbarsMenu  = (QPopupMenu*)guiFactory()->container("toolbars_load", this);
  connect(toolbarsMenu, SIGNAL(aboutToShow()), this, SLOT(slotBuildPrjToolbarsMenu()));

  connect( messageOutput, SIGNAL(clicked(QString,int)),
           this,          SLOT(gotoFileAndLine(QString,int)));

  refreshTimer = new QTimer( this );
  connect( refreshTimer, SIGNAL(timeout()), SLOT(reparse()) );
  refreshTimer->start( qConfig.refreshFrequency*1000, false ); //update the structure tree every 5 seconds

//Delay the calls as they contain dialog popups. That may crash Quanta!
  slotFileNew();
  initToolBars();

}


void QuantaApp::initToolBars()
{
 if (toolbarList.count() == 0)
     loadToolbarForDTD(project->defaultDTD());
}

void QuantaApp::initStatusBar()
{
  statusbarTimer = new QTimer(this);
  connect(statusbarTimer,SIGNAL(timeout()),this,SLOT(statusBarTimeout()));

  progressBar = new KProgress(statusBar());
  progressBar->show();
  statusBar()->insertItem(i18n(IDS_DEFAULT),IDS_STATUS, 1);
  statusBar()->addWidget(progressBar);
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
  doc = new QuantaDoc(this);
  connect(doc, SIGNAL(newStatus()),    this, SLOT(slotNewStatus()));
}

void QuantaApp::initProject()
{
  project = new Project(this);

  connect(project,  SIGNAL(openFile    (const KURL &, const QString&)),
          this,     SLOT  (slotFileOpen(const KURL &, const QString&)));
  connect(project,  SIGNAL(reloadTree(const KURL::List & ,bool)),
          pTab,     SLOT  (slotReloadTree(const KURL::List &,bool)));
  connect(project,  SIGNAL(setBaseURL(const KURL&)),
          pTab,     SLOT  (slotSetBaseURL(const KURL&)));
  connect(project,  SIGNAL(setProjectName(QString)),
          pTab,     SLOT  (slotSetProjectName(QString)));
  connect(project,  SIGNAL(closeFiles()),
          doc,      SLOT  (closeAll()));
  connect(project,  SIGNAL(showTree()),
          this,     SLOT  (slotShowProjectTree()));
  connect(project,  SIGNAL(removeFromProject(int)),
          pTab,     SLOT  (slotRemoveFromProject(int)));
  connect(project,  SIGNAL(templateURLChanged(const KURL &)),
          tTab,     SLOT  (slotSetTemplateURL(const KURL &)));
          
  connect(fLTab,    SIGNAL(insertDirInProject(const KURL&)),
          project,  SLOT  (slotAddDirectory(const KURL&)));
  connect(fTTab,    SIGNAL(insertDirInProject(const KURL&)),
          project,  SLOT  (slotAddDirectory(const KURL&)));
          
  connect(fLTab,    SIGNAL(insertFileInProject(const KURL&)),
          project,  SLOT  (slotInsertFile(const KURL&)));
  connect(fTTab,    SIGNAL(insertFileInProject(const KURL&)),
          project,  SLOT  (slotInsertFile(const KURL&)));
          
  connect(pTab,     SIGNAL(renameInProject(const KURL&)),
          project,  SLOT  (slotRename(const KURL&)));
  connect(pTab,     SIGNAL(removeFromProject(const KURL&)),
          project,  SLOT  (slotRemove(const KURL&)));
  connect(pTab,     SIGNAL(uploadSingleURL(const KURL&)),
          project,  SLOT  (slotUploadURL(const KURL&)));
  connect(pTab,     SIGNAL(rescanProjectDir()),
          project,  SLOT  (slotRescanPrjDir()));
  connect(pTab,     SIGNAL(showProjectOptions()),
          project,  SLOT  (slotOptions()));
  connect(pTab,     SIGNAL(uploadProject()),
          project,  SLOT  (slotUpload()));

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

  maindock = createDockWidget( "Editor", UserIcon("textarea"  ), 0L, i18n("Editor"), 0L);
  bottdock = createDockWidget( "Output", UserIcon("output_win"), 0L, i18n("Output"), 0L);

  ftabdock = createDockWidget("Files", UserIcon("ftab"), 0L, ""/*i18n("Files")*/);
  ftabdock->setToolTipString(i18n("Files tree view"));
  ptabdock = createDockWidget("Project", UserIcon("ptab"), 0L, ""/*i18n("Project")*/);
  ptabdock->setToolTipString(i18n("Project tree view"));
  ttabdock = createDockWidget("Templates", UserIcon("ttab"),     0L,"" /*i18n("Templates")*/);
  ttabdock->setToolTipString(i18n("Templates tree view"));
  stabdock = createDockWidget("Struct", BarIcon ("view_sidetree"),0L,"" /*i18n("Struct")*/);
  stabdock->setToolTipString(i18n("Structure view (DOM tree)"));
  dtabdock = createDockWidget("Docs", BarIcon ("contents2"),    0L,"" /*i18n("Docs")*/);
  dtabdock->setToolTipString(i18n("Documentation"));

  QStringList topStrList;
  config->setGroup("General Options");
  topStrList = config->readListEntry("Top folders");
  KURL::List topList;
  KURL url;
  for (uint i = 0; i < topStrList.count(); i++)
  {
    url = KURL();
    QuantaCommon::setUrl(url, topStrList[i]);
    topList.append(url);
  }
  url = KURL();
  url.setPath("/");
  if (!topList.contains(url)) topList.append(url);
  url = KURL();
  url.setPath(QDir::homeDirPath()+"/");
  if (!topList.contains(url)) topList.append(url);

  fTab = new QWidgetStack( ftabdock );
  fTTab = new FilesTreeView(topList, fTab );
  fLTab = new FilesListView( QDir::homeDirPath()+"/" , 0L, fTab );

  fTab -> addWidget( fTTab, 0 );
  fTab -> addWidget( fLTab, 1 );
  fTab -> raiseWidget( 0 );

  pTab  = new ProjectTreeView( ptabdock );

  tTab = new TemplatesTreeView( "" , ttabdock );


  dTab  = new DocTreeView    ( dtabdock );

  sTab = new StructTreeView( parser, config, stabdock ,"struct");

  rightWidgetStack = new QWidgetStack( maindock );

  view = new QuantaView( rightWidgetStack );

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

  connect(   fTTab,SIGNAL(openFile  (const KURL &, const QString&)),
            this, SLOT(slotFileOpen(const KURL &, const QString&)));
  connect(   fLTab,SIGNAL(openFile  (const KURL &, const QString&)),
            this, SLOT(slotFileOpen(const KURL &, const QString&)));
            
  connect(   fTTab,SIGNAL(openImage(const KURL&)),
            this, SLOT  (slotImageOpen(const KURL&)));
  connect(   fLTab,SIGNAL(openImage(const KURL&)),
            this, SLOT  (slotImageOpen(const KURL&)));
            
  connect(  fLTab,SIGNAL(changeMode()),
            this, SLOT(slotSwapLeftPanelMode()));
  connect(  fTTab,SIGNAL(changeMode()),
            this, SLOT(slotSwapLeftPanelMode()));

  connect(   pTab, SIGNAL(openFile  (const KURL &, const QString&)),
            this, SLOT(slotFileOpen(const KURL &, const QString&)));
  connect(   pTab, SIGNAL(openImage  (const KURL&)),
            this, SLOT(slotImageOpen(const KURL&)));
  connect(   pTab, SIGNAL(loadToolbarFile  (const KURL&)),
            this, SLOT(slotLoadToolbarFile(const KURL&)));

  connect(   tTab, SIGNAL(openImage  (const KURL&)),
            this, SLOT(slotImageOpen(const KURL&)));
  connect(   tTab, SIGNAL(openFile  (const KURL &, const QString&)),
            this, SLOT(slotFileOpen(const KURL &, const QString&)));
  connect(   tTab, SIGNAL(insertFile  (const KURL &)),
            this, SLOT(slotInsertFile(const KURL &)));
  connect(   tTab,SIGNAL(insertTag(const KURL &, DirInfo)),
            this, SLOT(slotInsertTag(const KURL &, DirInfo)));

  connect(   fTTab,SIGNAL(insertTag(const KURL &, DirInfo)),
            this, SLOT(slotInsertTag(const KURL &, DirInfo)));
  connect(   fLTab,SIGNAL(insertTag(const KURL &, DirInfo)),
            this, SLOT(slotInsertTag(const KURL &, DirInfo)));
  connect(   pTab,SIGNAL(insertTag(const KURL &, DirInfo)),
            this, SLOT(slotInsertTag(const KURL &, DirInfo)));
            
  connect(   fLTab,SIGNAL(activatePreview()),
            this, SLOT(slotActivatePreview()));
  connect(   fTTab,SIGNAL(activatePreview()),
            this, SLOT(slotActivatePreview()));
  connect(   pTab, SIGNAL(activatePreview()),
            this, SLOT(slotActivatePreview()));

  connect(  htmlpart,       SIGNAL(onURL(const QString&)), this, SLOT(slotStatusMsg(const QString&)));
  connect(  htmlPartDoc,    SIGNAL(onURL(const QString&)), this, SLOT(slotStatusMsg(const QString&)));
//  connect(  htmlPartDoc,    SIGNAL(updateStatus(bool, bool)), SLOT(updateNavButtons( bool, bool)));

  connect( view, SIGNAL(newCurPos()), this, SLOT(slotNewLineColumn()));

  connect( sTab, SIGNAL(newCursorPosition(int,int)), SLOT(setCursorPosition(int,int)));
//  connect( sTab, SIGNAL(onTag(const QString &)), SLOT( slotStatusHelpMsg(const QString &)));
  connect( sTab, SIGNAL(selectArea(int,int,int,int)), SLOT( selectArea(int,int,int,int)));
  connect( sTab, SIGNAL(needReparse()),    SLOT(reparse()));
  connect( sTab, SIGNAL(parsingDTDChanged(QString)), SLOT(slotParsingDTDChanged(QString)));
  connect( dTab, SIGNAL(openURL(QString)), SLOT(openDoc(QString)));

  connect(getView(), SIGNAL(dragInsert(QDropEvent *)), tTab, SLOT(slotDragInsert(QDropEvent *)));

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
  if (config)
  {
    config->setGroup  ("General Options");

    config->writeEntry("Geometry", size());

    config->writeEntry("Show Toolbar", toolBar("mainToolBar")->isVisible());
    config->writeEntry("Show DTD Toolbar", qConfig.enableDTDToolbar);
    config->writeEntry("Show Statusbar", statusBar()->isVisible());

    config->writeEntry("Markup mimetypes", qConfig.markupMimeTypes  );
    config->writeEntry("Script mimetypes", qConfig.scriptMimeTypes   );
    config->writeEntry("Image mimetypes", qConfig.imageMimeTypes );
    config->writeEntry("Text mimetypes", qConfig.textMimeTypes  );

    config->writeEntry("Capitals for tags", qConfig.tagCase);
    config->writeEntry("Capitals for attr", qConfig.attrCase);
    config->writeEntry("Attribute quotation", qConfig.attrValueQuotation);
    config->writeEntry("Close tag if optional", qConfig.closeOptionalTags);
    config->writeEntry("Close tags", qConfig.closeTags);
    config->writeEntry("Auto completion", qConfig.useAutoCompletion);

    config->writeEntry("Default encoding", qConfig.defaultEncoding);
    config->writeEntry("Default DTD", qConfig.defaultDocType);
    config->writeEntry("New File Type", qConfig.newFileType);
    config->writeEntry("Use MimeTypes", qConfig.useMimeTypes);

    config->writeEntry("Refresh frequency", qConfig.refreshFrequency);

    config->writeEntry("Left panel mode", fTab->id( fTab->visibleWidget()));
    config->writeEntry("Follow Cursor", sTab->followCursor() );
    config->writeEntry("PHP Debugger Port", phpDebugPort );
    config->writeEntry("Top folders", fTTab->topURLList.toStringList());
    config->writeEntry("List of opened files", doc->openedFiles().toStringList());
    config->writeEntry("Version", VERSION); // version
    doc    ->writeConfig(config); // kwrites
    project->writeConfig(config); // project
    config->setGroup  ("General Options");
    config->deleteGroup("RecentFiles");
    fileRecent->saveEntries(config);
    config->writeEntry ("Enable Debugger", debuggerStyle!="None");
    config->writeEntry ("PHP Debugger style", debuggerStyle);
    writeDockConfig(config);
    saveMainWindowSettings(config);
    spellChecker->writeConfig(config);
    config->sync(); 
  }
}


void QuantaApp::readOptions()
{
  config->setGroup("General Options");
  //if (2>config->readNumEntry("Version",0)) config = new KConfig();

  FileMasks dlg(this);
  dlg.setToDefault();
  qConfig.markupMimeTypes = config->readEntry("Markup mimetypes", qConfig.markupMimeTypes);
  qConfig.scriptMimeTypes = config->readEntry("Script mimetypes", qConfig.scriptMimeTypes);
  qConfig.imageMimeTypes = config->readEntry("Image mimetypes", qConfig.imageMimeTypes);
  qConfig.textMimeTypes = config->readEntry("Text mimetypes", qConfig.textMimeTypes);

  qConfig.tagCase = config->readNumEntry("Capitals for tags", 0);
  qConfig.attrCase = config->readNumEntry("Capitals for attr", 0);
  qConfig.attrValueQuotation = config->readEntry("Attribute quotation", "double");
  qConfig.closeOptionalTags = config->readBoolEntry("Close tag if optional", true);
  qConfig.closeTags = config->readBoolEntry("Close tags", true);
  qConfig.useAutoCompletion = config->readBoolEntry("Auto completion",true);
  qConfig.defaultDocType = config->readEntry("Default DTD",DEFAULT_DTD);
  qConfig.newFileType = config->readEntry("New File Type", qConfig.defaultDocType);
  qConfig.defaultEncoding = config->readEntry("Default encoding", QTextCodec::codecForLocale()->name());
  previewPosition   = config->readEntry("Preview position","Right");
  qConfig.useMimeTypes = config->readBoolEntry("Use MimeTypes", false);

  qConfig.refreshFrequency = config->readNumEntry("Refresh frequency",5);

  phpDebugPort = config->readNumEntry("PHP Debugger Port", 7869);

  sTab->setFollowCursor( config->readBoolEntry("Follow Cursor", true));

  int mode = config->readNumEntry("Left panel mode", 0);
/* List Mode disabled!!
  if ( mode == 0 || mode == 1 ) fTab->raiseWidget(mode);
*/
  fTab->raiseWidget(0);  

  fileRecent ->loadEntries(config);

  QSize s(800,580);
  config->setGroup("General Options");
  resize( config->readSizeEntry("Geometry", &s));

  if (!config->readBoolEntry("Show Toolbar",true))
  {
    toolBar("mainToolBar")->hide();
    toolBar("mainEditToolBar")->hide();
    toolBar("mainNaviToolBar")->hide();
    showToolbarAction->setChecked(false);
  } else
  {
    toolBar("mainToolBar")->show();
    toolBar("mainEditToolBar")->show();
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
  qConfig.enableDTDToolbar = config->readBoolEntry("Show DTD Toolbar",true);

  slotToggleDTDToolbar(qConfig.enableDTDToolbar);
  showDTDToolbar->setEnabled(qConfig.enableDTDToolbar);

  qConfig.lineNumbers = config->readBoolEntry("LineNumbers", false);
  qConfig.iconBar = config->readBoolEntry("Iconbar", false);
  viewBorder->setChecked(qConfig.iconBar);
  viewLineNumbers->setChecked(qConfig.lineNumbers);
  viewBorder->setChecked(qConfig.iconBar);
  viewLineNumbers->setChecked(qConfig.lineNumbers);

  readDockConfig(config);

  showPreviewAction  ->setChecked( false );
  showMessagesAction ->setChecked( bottdock->parent() != 0L );

  doc    ->readConfig(config); // kwrites
  project->readConfig(config); // project

  if (view->writeExists() && view->write()->isUntitled())
  {
    doc->closeDocument();
    doc->newDocument(KURL());
  }

  debuggerStyle = "None";
  config->setGroup  ("General Options");
  if (config->readBoolEntry("Enable Debugger", true))
    if (config->readEntry("PHP Debugger style","PHP4") == "PHP4")
         enablePhp4Debug(true);
    else enablePhp3Debug(true);

  applyMainWindowSettings(config);

  spellChecker->readConfig(config);
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

    if ( !dbg3->ok() )
        messageOutput->insertItem(i18n("Can't bind port %1, PHP3 debugger disabled").arg(phpDebugPort));
    else
        messageOutput->insertItem(i18n("PHP3 Debugger listens at port %1").arg(phpDebugPort));
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
    KURL fu;
    QuantaCommon::setUrl(fu, QString(urls.current()));

    if ( !isPrj || fu.isLocalFile() )
      doc->openDocument( fu );
  }
}

/** Loads the initial project */
void QuantaApp::loadInitialProject(QString url)
{
  if(url.isNull())
    project->readLastConfig();
  else
    project->slotOpenProject(url);
}


bool QuantaApp::queryExit()
{
  bool canExit = true;
  if (quantaStarted)
  {
    exitingFlag = true;
    saveOptions();
    saveModifiedToolbars();
    canExit = doc->saveAll(false);
    if (canExit)
    {
      project->slotCloseProject();
      Document *w;
      do
      {
        w = view->write();
        w->closeTempFile();
  	  }while (view->removeWrite());
    }
  }
  return canExit;
}

/**
 Parse the dom document and retrieve the tag attributes
*/
void QuantaApp::setAttributes(QDomNode *dom, QTag* tag)
{
 Attribute *attr;

 QDomElement el = dom->toElement();

 QDictIterator<AttributeList> it(*(tag->parentDTD->commonAttrs));
 for( ; it.current(); ++it )
 {
   QString lookForAttr = "has"+QString(it.currentKey()).stripWhiteSpace();
   if (el.attribute(lookForAttr) == "1")
   {
    tag->commonGroups += QString(it.currentKey()).stripWhiteSpace();
   }
 }

 if (el.attribute("single") == "1")
 {
  tag->setSingle(true);
 }

 if (el.attribute("optional") == "1")
 {
  tag->setOptional(true);
 }

 tag->type = el.attribute("type","xmltag");
 tag->returnType = el.attribute("returnType","");

 for ( QDomNode n = dom->firstChild(); !n.isNull(); n = n.nextSibling() )
 {
   if (n.nodeName() == "stoppingtags") //read what tag can act as closing tag
   {
     QDomElement el = n.toElement();
     QDomElement item = el.firstChild().toElement();
     while ( !item.isNull() )
     {
       QString stopTag = item.text();
       if (!tag->parentDTD->caseSensitive) stopTag = stopTag.upper();
       tag->stoppingTags.append(stopTag);
       item = item.nextSibling().toElement();
     }
   }
   if ( n.nodeName() == "attr" ) //an attribute
   {
     attr = new Attribute;
     attr->name = n.toElement().attribute("name");
     attr->type = n.toElement().attribute("type",tag->parentDTD->defaultAttrType);
     attr->defaultValue = n.toElement().attribute("defaultValue");
     attr->status = n.toElement().attribute("status");

     if ( attr->type == "list" ) {
       QDomElement el = n.toElement();
       for ( QDomElement attrEl = el.firstChild().toElement(); !attrEl.isNull(); attrEl = attrEl.nextSibling().toElement() ) {
         if ( attrEl.tagName() == "items" ) {
           QDomElement item = attrEl.firstChild().toElement();
           while ( !item.isNull() ) {
             attr->values.append( item.text() );
             item = item.nextSibling().toElement();
           }
         }
       }
     } else if ( attr->type == "check" ) {
       attr->values.append("true");
       attr->values.append("false");
     } else if ( attr->type == "color" ) {
       attr->values.append("Black");
       attr->values.append("Silver");
       attr->values.append("Gray");
       attr->values.append("White");
       attr->values.append("Maroon");
       attr->values.append("Red");
       attr->values.append("Purple");
       attr->values.append("Fuchsia");
       attr->values.append("Green");
       attr->values.append("Lime");
       attr->values.append("Olive");
       attr->values.append("Yellow");
       attr->values.append("Navy");
       attr->values.append("Blue");
       attr->values.append("Teal");
       attr->values.append("Aqua");
     } else if ( attr->type == "url" ) {
     } else if ( attr->type == "input" ) {
     } else {
     }

     if (!attr->name.isEmpty())
     {
       tag->addAttribute(attr);
     }

     delete attr;
   }
 }

// return attrs;
}

/** Reads the tags for the tag files. Returns the number of read tags. */
uint QuantaApp::readTagFile(QString fileName, DTDStruct* parentDTD, QTagList *tagList)
{
 int numOfTags = 0;

 QFile f( fileName );
 f.open( IO_ReadOnly );
 QDomDocument *doc = new QDomDocument();
 doc->setContent( &f );
 f.close();

 for ( QDomNode n = doc->firstChild().firstChild(); !n.isNull(); n = n.nextSibling() )
 {
   if (n.nodeName() == "tag") //read a tag
   {
     QTag *tag = new QTag();
     tag->setName(n.toElement().attribute("name"));
     tag->setFileName(fileName);
     tag->parentDTD = parentDTD;
     setAttributes(&n, tag);
     if (parentDTD->caseSensitive)
     {
       tagList->insert(tag->name(),tag);  //append the tag to the list for this DTD
     } else
     {
       tagList->insert(tag->name().upper(),tag);
     }
     numOfTags++;
   }
 }

 delete doc;
 return numOfTags;
}

/** Reads the tag files and the description.rc from tagDir in order to
    build up the internal DTD and tag structures. */
void QuantaApp::readTagDir(QString &dirName)
{
 //read the general DTD info
 DTDStruct *dtd = new DTDStruct;
 dtd->fileName = dirName + "description.rc";
 dtd->commonAttrs = new AttributeListDict();
 dtd->commonAttrs->setAutoDelete(true);

 KConfig *dtdConfig = new KConfig(dtd->fileName);
 dtdConfig->setGroup("General");
 QString dtdName = dtdConfig->readEntry("Name", "Unknown");
 bool caseSensitive = dtdConfig->readBoolEntry("CaseSensitive");
 dtd->name = dtdName.lower();
 dtd->nickName = dtdConfig->readEntry("NickName", dtdName);
 dtd->url = dtdConfig->readEntry("URL");
 dtd->inheritsTagsFrom = dtdConfig->readEntry("Inherits");

 dtd->defaultExtension = dtdConfig->readEntry("DefaultExtension", "html");
 dtd->caseSensitive = caseSensitive;
 dtd->family = dtdConfig->readNumEntry("Family", Xml);
 int numOfTags = 0;

 //read the attributes for each common group
 QStrList * groupList = new QStrList();
 dtdConfig->readListEntry("Groups", *groupList); //read the common groups
 for (uint i = 0; i < groupList->count(); i++)
 {
   AttributeList *commonAttrList = new AttributeList;      //no need to delete it
   commonAttrList->setAutoDelete(true);
   QString groupName = QString(groupList->at(i)).stripWhiteSpace();

   dtdConfig->setGroup(groupName);
   QStrList *attrList = new QStrList();
   dtdConfig->readListEntry("Attributes", * attrList);
   for (uint j = 0; j < attrList->count(); j++)
   {
     Attribute *attr = new Attribute;                                  //no need to delete it
     attr->name = QString(attrList->at(j)).stripWhiteSpace();
     attr->type = "input";
     attr->defaultValue = "";
     attr->status = "optional";
     commonAttrList->append(attr);
   }
   delete attrList;

   dtd->commonAttrs->insert(groupName, commonAttrList);
 }
 delete groupList;

 QTagList *tagList = new QTagList(119, false); //max 119 tag in a DTD
 tagList->setAutoDelete(true);
 //read all the tag files
 KURL dirURL;
 QuantaCommon::setUrl(dirURL, dirName);
 dirURL.adjustPath(1);
 KURL::List files = QExtFileInfo::allFilesRelative(dirURL, "*.tag");
 for ( KURL::List::Iterator it_f = files.begin(); it_f != files.end(); ++it_f )
 {
   QString name = (*it_f).fileName();
   if (! name.isEmpty())
   {
     QString fname = dirName + (*it_f).fileName() ;
     numOfTags += readTagFile(fname,dtd, tagList);
   }
 }

 //read the toolbars
 dtdConfig->setGroup("Toolbars");
 QString toolbarsLocation = dtdConfig->readEntry("Location");
 if (!toolbarsLocation.endsWith("/") && !toolbarsLocation.isEmpty())
 {
   toolbarsLocation.append("/");
 }
 dtd->toolbars = dtdConfig->readListEntry("Names");
 for (uint i = 0; i < dtd->toolbars.count(); i++)
 {
   dtd->toolbars[i] = toolbarsLocation + dtd->toolbars[i].stripWhiteSpace() + toolbarExtension;
 }

 //read the extra tags and their attributes
 dtdConfig->setGroup("Extra tags");
 dtd->defaultAttrType = dtdConfig->readEntry("DefaultAttrType","input");
 QStrList extraTagsList;
 dtdConfig->readListEntry("List",extraTagsList);
 for (uint i = 0 ; i < extraTagsList.count(); i++)
 {
   QTag *tag = new QTag();
   tag->setName(QString(extraTagsList.at(i)).stripWhiteSpace());

   QString searchForTag = (dtd->caseSensitive) ? tag->name() : tag->name().upper();
   if (tagList->find(searchForTag)) //the tag is already defined in a .tag file
   {
     delete tag;
     continue; //skip this tag
   }
   tag->parentDTD = dtd;
   //read the possible stopping tags
   QStrList stoppingTags;
   dtdConfig->readListEntry(tag->name() + "_stoppingtags",stoppingTags);
   for (uint j = 0; j < stoppingTags.count(); j++)
   {
     QString stopTag = QString(stoppingTags.at(j)).stripWhiteSpace();
     if (!dtd->caseSensitive) stopTag = stopTag.upper();
     tag->stoppingTags.append(stopTag);
   }
   QStrList optionsList;
   //read the possible tag options
   dtdConfig->readListEntry(tag->name() + "_options",optionsList);
   for (uint j = 0; j < optionsList.count(); j++)
   {
     QString option = QString(optionsList.at(j)).stripWhiteSpace();
     QDictIterator<AttributeList> it(*(dtd->commonAttrs));
     for( ; it.current(); ++it )
     {
       QString lookForAttr = "has"+QString(it.currentKey()).stripWhiteSpace();
       if (option == lookForAttr)
       {
         tag->commonGroups += QString(it.currentKey()).stripWhiteSpace();
       }
     }
     if (option == "single")
     {
       tag->setSingle(true);
     }
     if (option == "optional")
     {
        tag->setOptional(true);
     }
   }
   QStrList attrList;
   dtdConfig->readListEntry(tag->name(), attrList);
   for (uint j = 0; j < attrList.count(); j++)
   {
     Attribute* attr = new Attribute;
     attr->name = QString(attrList.at(j)).stripWhiteSpace();
     attr->type = dtd->defaultAttrType;
     tag->addAttribute(attr);
     delete attr;
   }
   if (caseSensitive)
   {
     tagList->insert(tag->name(),tag);  //append the tag to the list for this DTD
   } else
   {
     tagList->insert(tag->name().upper(),tag);
   }
 }
 dtd->tagsList = tagList;

 dtdConfig->setGroup("Extra rules");
 dtd->scriptName = (dtdConfig->readEntry("ScriptName")).lower();
 if (!dtd->scriptName.isEmpty())
 {
   dtd->scriptTagStart.append("<script");
   dtd->scriptTagEnd.append("/script>");
 }
 dtd->scriptRegExpStr = dtdConfig->readEntry("ScriptRegExp");
 QStringList tagBorders = dtdConfig->readListEntry("ScriptTagBorders");

 for (uint i = 0; i < tagBorders.count(); i++)
 {
   QString s;
   QStringList slist = QStringList::split(" ",tagBorders[i].stripWhiteSpace());
   s = slist[0].stripWhiteSpace();
   dtd->scriptTagStart.append(s);
   s.replace(QRegExp("\\?"),"\\?");
   s.replace(QRegExp("\\*"),"\\*");
   s.replace(QRegExp("\\."),"\\.");
   s.replace(QRegExp("\\^"),"\\^");
   s.replace(QRegExp("\\$"),"\\$");
   scriptBeginRxStr.append("|(?:"+s+")");
   s = slist[1].stripWhiteSpace();
   dtd->scriptTagEnd.append(s);
   s.replace(QRegExp("\\?"),"\\?");
   s.replace(QRegExp("\\*"),"\\*");
   s.replace(QRegExp("\\."),"\\.");
   s.replace(QRegExp("\\^"),"\\^");
   s.replace(QRegExp("\\$"),"\\$");
   scriptEndRxStr.append("|("+s+")");
 }

 dtd->booleanAttributes = dtdConfig->readEntry("BooleanAttributes","extended");
 dtd->booleanTrue = dtdConfig->readEntry("BooleanTrue","true");
 dtd->booleanFalse = dtdConfig->readEntry("BooleanFalse","false");
 dtd->singleTagStyle = dtdConfig->readEntry("Single Tag Style", "html").lower();

 dtds->insert(dtdName.lower(), dtd);//insert the taglist into the full list

 delete dtdConfig;
}


/**
  read dictionary of known tags and attributes from tags.rc file.
*/

void QuantaApp::initTagDict()
{
  dtds = new QDict<DTDStruct>(119, false); //optimized for max 119 DTD. This should be enough.
  dtds->setAutoDelete(true);

  QStringList tagsResourceDirs = KGlobal::instance()->dirs()->findDirs("appdata", "tags");
  QStringList tagsDirs;
  for ( QStringList::Iterator it = tagsResourceDirs.begin(); it != tagsResourceDirs.end(); ++it )
  {
    QDir dir(*it);
    dir.setFilter(QDir::Dirs);
    QStringList subDirs = dir.entryList();
    for ( QStringList::Iterator subit = subDirs.begin(); subit != subDirs.end(); ++subit )
    {
      if ((*subit != ".") && (*subit != ".."))
         tagsDirs += *it + *subit+"/";
    }
  }
  scriptBeginRxStr = "(<script)";
  scriptEndRxStr = "(/script>)";
  for ( QStringList::Iterator it = tagsDirs.begin(); it != tagsDirs.end(); ++it )
  {
    readTagDir(*it);
  }

//Now resolve the inheritence
  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    DTDStruct* dtd = it.current();
    if (!dtd->inheritsTagsFrom.isEmpty())
    {
      DTDStruct *parent = dtds->find(dtd->inheritsTagsFrom);
      QDictIterator<QTag> tag_it(*(parent->tagsList));
      for ( ; tag_it.current(); ++tag_it)
      {
        QTag *tag = tag_it.current();
        QString searchForTag = (dtd->caseSensitive) ? tag->name() : tag->name().upper();
        if (!dtd->tagsList->find(searchForTag))
        {
          QTag *newTag = new QTag(*tag);
          dtd->tagsList->insert(searchForTag, newTag);
        }
      }
    }
  }
    
  
  scriptBeginRx.setCaseSensitive(false);
  scriptBeginRx.setPattern(scriptBeginRxStr);
  scriptEndRx.setCaseSensitive(false);
  scriptEndRx.setPattern(scriptEndRxStr);
  if (!dtds->find(qConfig.defaultDocType)) qConfig.defaultDocType = DEFAULT_DTD;
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
    (void) new KAction( i18n( "&Toggle Block Selection" ), Key_F4, view,
                        SLOT( toggleVertical() ), actionCollection(), "set_verticalSelect" );


    KStdAction::find(view, SLOT(slotFind()), actionCollection());
    KStdAction::findNext(view, SLOT(slotFindAgain()), actionCollection());
    KStdAction::findPrev(view, SLOT(slotFindAgainB()), actionCollection(), "edit_find_prev");
    KStdAction::replace(view, SLOT(slotReplace()), actionCollection());

    new KAction(i18n("&Indent"), "indent", CTRL+Key_I, view,
                SLOT(slotIndent()), actionCollection(), "edit_indent");
    new KAction(i18n("&Unindent"), "unindent", CTRL+SHIFT+Key_I, view,
                SLOT(slotUnIndent()), actionCollection(), "edit_unindent");
    new KAction(i18n("&Clean Indentation"), "cleanindent", 0, view,
                SLOT(slotCleanIndent()), actionCollection(), "edit_cleanindent");

    new KAction(i18n("Co&mment"),  CTRL+Qt::Key_NumberSign,
                view, SLOT(slotComment()), actionCollection(), "edit_comment");
    new KAction(i18n("Unc&omment"),
                CTRL+SHIFT+Qt::Key_NumberSign, view, SLOT(slotUnComment()),
                actionCollection(), "edit_uncomment");
    new KAction(i18n("Apply Word Wrap"), "", 0, view, SLOT(slotApplyWordWrap()),
                actionCollection(), "edit_apply_wordwrap");



//Tools menu
    KStdAction::gotoLine(view, SLOT(slotGotoLine()), actionCollection(), "edit_goto_line");
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
                      view, SLOT( slotEditorOptions() ), actionCollection(), "editor_options" );


    setEndOfLine = new KSelectAction(i18n("&End of Line"), 0, actionCollection(),"set_eol");
    connect(setEndOfLine, SIGNAL(activated(int)), view, SLOT(setEol(int)));
    connect(setEndOfLine->popupMenu(), SIGNAL(aboutToShow()), this, SLOT(setEOLMenuAboutToShow()));

    QStringList list;
    list.append(i18n("&Unix"));
    list.append(i18n("&Windows/DOS"));
    list.append(i18n("&Macintosh"));
    setEndOfLine->setItems(list);


    // File actions
    //
    KStdAction::openNew( this, SLOT( slotFileNew()  ), actionCollection());
    KStdAction::open   ( this, SLOT( slotFileOpen() ), actionCollection(), "qfile_open");
    KStdAction::close  ( this, SLOT( slotFileClose()), actionCollection());

    fileRecent =  KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)),
                                         actionCollection(), "qfile_open_recent");

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
    saveAsProjectTemplateAction = new KAction( i18n( "Save As Project Template" ), 0,
                        this, SLOT( slotFileSaveAsProjectTemplate() ),
                        actionCollection(), "save_project_template" );

    saveSelectionAsLocalTemplateAction = new KAction( i18n( "Save Selection To Local Template File" ), 0,
                        this, SLOT( slotFileSaveSelectionAsLocalTemplate() ),
                        actionCollection(), "save_selection_local_template" );
    saveSelectionAsProjectTemplateAction = new KAction( i18n( "Save Selection To Project Template File" ), 0,
                        this, SLOT( slotFileSaveSelectionAsProjectTemplate() ),
                        actionCollection(), "save_selection_project_template" );

    KStdAction::print( this, SLOT( slotFilePrint() ), actionCollection() );

    KStdAction::quit( this, SLOT( slotFileQuit() ), actionCollection() );

   (void) new KAction( i18n( "&File List" ), ALT+Key_0,this, SLOT( slotShowOpenFileList() ),
                        actionCollection(), "file_list" );

    // Edit actions
 /*
    KAction *undoRedo
      = new KAction( i18n( "Undo/Redo &History..."), 0,
                     doc, SLOT( undoHistory()),
                     actionCollection(), "undo_history" );

    undoRedo->setGroup( "edit_undo_merge" ); */

    (void) new KAction( i18n( "Find in Files..." ),
                        UserIcon("find"), CTRL+ALT+Key_F,
                        this, SLOT( slotEditFindInFiles() ),
                        actionCollection(), "find_in_files" );

    // Tool actions

    (void) new KAction( i18n( "&Context Help..." ), CTRL+Key_H,
                        this, SLOT( slotContextHelp() ),
                        actionCollection(), "context_help" );

    (void) new KAction( i18n( "&Quanta Homepage" ), 0,
                        this, SLOT( slotHelpHomepage() ),
                        actionCollection(), "help_homepage" );

    (void) new KAction( i18n( "Make a &Donation" ), 0,
                        this, SLOT( slotMakeDonation() ),
                        actionCollection(), "help_donation" );

    (void) new KAction( i18n( "Tag &Attributes" ), ALT+Key_Down,
                        doc, SLOT( slotAttribPopup() ),
                        actionCollection(), "tag_attributes" );

    (void) new KAction( i18n( "&Change DTD Type" ), 0,
                        this, SLOT( slotToolsChangeDTD() ),
                        actionCollection(), "tools_change_dtd" );

    (void) new KAction( i18n( "Send DTD in E-&Mail" ), 0,
                        this, SLOT( slotEmailDTD() ),
                        actionCollection(), "tools_send_dtd" );

    (void) new KAction( i18n( "&Syntax Check" ), 0,
                        this, SLOT( slotToolSyntaxCheck() ),
                        actionCollection(), "syntax_check" );

    // View actions

    showFTabAction =
      new KToggleAction( i18n( "Show Files Tree" ), UserIcon("ftab"), 0,
                         this, SLOT( slotShowFTabDock() ),
                         actionCollection(), "show_ftab_tree" );
    showPTabAction =
      new KToggleAction( i18n( "Show Project Tree" ), UserIcon("ptab"), 0,
                         this, SLOT( slotShowPTabDock() ),
                         actionCollection(), "show_ptab_tree" );
    showTTabAction =
      new KToggleAction( i18n( "Show Templates Tree" ), UserIcon("ttab"), 0,
                         this, SLOT( slotShowTTabDock() ),
                         actionCollection(), "show_ttab_tree" );
    showSTabAction =
      new KToggleAction( i18n( "Show Structure Tree" ), BarIcon ("view_sidetree"), 0,
                         this, SLOT( slotShowSTabDock() ),
                         actionCollection(), "show_stab_tree" );
    showDTabAction =
      new KToggleAction( i18n( "Show Documentation Tree" ), BarIcon ("contents2"), 0,
                         this, SLOT( slotShowDTabDock() ),
                         actionCollection(), "show_dtab_tree" );

    showMessagesAction =
      new KToggleAction( i18n( "Show &Messages" ), "output_win", CTRL+Key_M,
                         this, SLOT( slotShowBottDock() ),
                         actionCollection(), "show_messages" );

    showPreviewAction =
      new KToggleAction( i18n( "Pr&eview" ), "preview", Key_F6,
                         this, SLOT( slotShowPreview() ),
                         actionCollection(), "show_preview" );

    (void) new KAction( i18n( "&Reload Preview" ), "reload",
                        KStdAccel::key(KStdAccel::Reload),
                        this, SLOT( slotViewRepaint() ),
                        actionCollection(), "reload" );

    (void) new KAction( i18n( "View with Net&scape" ), "netscape", ALT+Key_F6,
                        view, SLOT( slotViewInNetscape() ),
                        actionCollection(), "view_with_netscape" );

    (void) new KAction( i18n( "View with &Konqueror" ), "konqueror", CTRL+Key_F6,
                        view, SLOT( slotViewInKFM() ),
                        actionCollection(), "view_with_konqueror" );

    (void) new KAction( i18n( "View with L&ynx" ), "terminal", SHIFT+Key_F6,
                        view, SLOT( slotViewInLynx() ),
                        actionCollection(), "view_with_lynx" );

    (void) new KAction( i18n( "&Previous File" ), "1leftarrow", KStdAccel::back(),
                        this, SLOT( slotBack() ),
                        actionCollection(), "previous_file" );

    (void) new KAction( i18n( "&Next File" ), "1rightarrow", KStdAccel::forward(),
                        this, SLOT( slotForward() ),
                        actionCollection(), "next_file" );

    // Project actions
    //
    newPrjAction = new KAction( i18n( "&New Project..." ), 0,
                        project, SLOT( slotNewProject() ),
                        actionCollection(), "project_new" );

    openPrjAction = new KAction( i18n( "&Open Project..." ), BarIcon("folder_new"), 0,
                        project, SLOT( slotOpenProject() ),
                        actionCollection(), "project_open" );

    project -> projectRecent =
      KStdAction::openRecent(project, SLOT(slotOpenProject(const KURL&)),
                             actionCollection(), "project_open_recent");
    project->projectRecent->setText(i18n("Open Recent Project..."));

    saveprjAction =  new KAction( i18n( "&Save Project" ), SmallIcon("save"), 0,
                         project, SLOT( slotSaveProject() ),
                         actionCollection(), "project_save" );

    closeprjAction =  new KAction( i18n( "&Close Project" ), SmallIcon("fileclose"), 0,
                         project, SLOT( slotCloseProject() ),
                         actionCollection(), "project_close" );


    openPrjViewAction = new KAction( i18n( "Open Project View..." ), 0,
                        project, SLOT( slotOpenProjectView() ),
                        actionCollection(), "project_view_open" );

    savePrjViewAction = new KAction( i18n( "Save Project View..." ), 0,
                        project, SLOT( slotSaveProjectView() ),
                        actionCollection(), "project_view_save" );
    saveAsPrjViewAction = new KAction( i18n( "Save Project View As..." ), 0,
                        project, SLOT( slotSaveAsProjectView() ),
                        actionCollection(), "project_view_save_as" );
                         


    insertFileAction = new KAction( i18n( "&Insert Files..." ), 0,
                        project, SLOT( slotAddFiles() ),
                        actionCollection(), "project_insert_file" );

    insertDirAction = new KAction( i18n( "&Insert Directory..." ), 0,
                        project, SLOT( slotAddDirectory() ),
                        actionCollection(), "project_insert_directory" );

    rescanPrjDirAction = new KAction( i18n( "&Rescan Project Directory" ), SmallIcon("reload"), 0,
                        project, SLOT( slotRescanPrjDir() ),
                        actionCollection(), "project_rescan" );

    uploadProjectAction = new KAction( i18n( "&Upload Project..." ), Key_F8,
                        project, SLOT( slotUpload() ),
                        actionCollection(), "project_upload" );

    projectOptionAction = new KAction( i18n( "&Project Options..." ), Key_F7,
                        project, SLOT( slotOptions() ),
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

    // Toolbars actions
    projectToolbarFiles = new KRecentFilesAction(i18n("Load &Project Toolbar"),0,this, SLOT(slotLoadToolbarFile(const KURL&)),
                           actionCollection(), "toolbars_load_project");

    new KAction(i18n("Load &Global Toolbar..."), 0, this, SLOT(slotLoadGlobalToolbar()), actionCollection(), "toolbars_load_global");
    new KAction(i18n("Load &Local Toolbar..."), 0, this, SLOT(slotLoadToolbar()), actionCollection(), "toolbars_load_user");
    new KAction(i18n("Save as &Local Toolbar..."),  0, this, SLOT(slotSaveLocalToolbar()), actionCollection(), "toolbars_save_local");
    new KAction(i18n("Save as &Project Toolbar..."),  0, this, SLOT(slotSaveProjectToolbar()), actionCollection(), "toolbars_save_project");
    new KAction(i18n("&Add User Toolbar..."),  0, this, SLOT(slotAddToolbar()), actionCollection(), "toolbars_add");
    new KAction(i18n("&Remove User Toolbar..."),  0, this, SLOT(slotRemoveToolbar()), actionCollection(), "toolbars_remove");
    new KAction(i18n("Send Toolbar in E-&Mail..."),  0, this, SLOT(slotSendToolbar()), actionCollection(), "toolbars_send");

    showDTDToolbar=new KToggleAction(i18n("Show DTD Toolbar"),0, actionCollection(), "view_dtd_toolbar");//,view,SLOT(slotHideToolbar()),actionCollection(),"view_dtd_toolbar");
    connect(showDTDToolbar, SIGNAL(toggled(bool)), this, SLOT(slotToggleDTDToolbar(bool)));
    

    new KAction(i18n("Complete Text"),CTRL+Key_Space,this,SLOT(slotShowCompletion()),actionCollection(),"show_completion");
    new KAction(i18n("Completion Hints"),CTRL+SHIFT+Key_Space,this,SLOT(slotShowCompletionHint()),actionCollection(),"show_completion_hint");

    KStdAction::back   ( this, SLOT( slotBack() ),    actionCollection(), "w_back" );
    KStdAction::forward( this, SLOT( slotForward() ), actionCollection(), "w_forward" );
}

/** Initialize the plugin architecture. */
void QuantaApp::initPlugins()
{
  // TODO : read option from plugins.rc to see if we should validate the plugins

  m_pluginMenu = new QPopupMenu(this);
  m_pluginMenu->setCheckable(TRUE);
  connect(m_pluginMenu, SIGNAL(aboutToShow()), this, SLOT(slotBuildPluginMenu()));
  connect(m_pluginMenu, SIGNAL(activated(int)), this, SLOT(slotPluginRun(int)));

  menuBar()->insertItem(i18n("Plu&gins"), m_pluginMenu, -1, PLUGINS_MENU_PLACE);
}

/** Builds the plugins menu dynamically */
void QuantaApp::slotBuildPluginMenu()
{
  m_pluginMenu->clear();
  m_pluginMenu->insertItem(i18n("&Edit"), this, SLOT(slotPluginsEdit()), 0);
  m_pluginMenu->insertItem(i18n("&Validate"), this, SLOT(slotPluginsValidate()), 0);
  m_pluginMenu->insertSeparator();

  // TODO : We should have a QuantaPluginInterface::isModified function
  // so that we can skip all of this work if nothing has changed

  QDict<QuantaPlugin> plugins = m_pluginInterface->plugins();

  QDictIterator<QuantaPlugin> it(plugins);
  for(;it.current() != 0;++it)
  {
       QuantaPlugin *curPlugin = it.current();

       if(curPlugin)
       {
//         int id = m_pluginMenu->insertItem(curPlugin->pluginName());
//         if(curPlugin->isRunning())
//           m_pluginMenu->setItemChecked(id, TRUE);
         curPlugin->m_action->plug(m_pluginMenu);
       }
  }
}

/** Runs the plugin specified by id */
void QuantaApp::slotPluginRun(int a_id)
{ 
  QString pluginName = m_pluginMenu->text(a_id);
  if(pluginName.isEmpty() || pluginName == "&Edit" || pluginName == "&Validate")
    return;
    
  if(pluginName != QString::null)
  {
    QuantaPlugin *plugin = m_pluginInterface->plugin(pluginName);
    if(plugin)
    {
      if(plugin->type() == i18n("KPart")) // special case
      {
        /*
         Currently there's no easy way to determine when a KPart has been closed
         by the user, so they have to manually stop it by unchecking it in the
         plugins menu
        */
        if(m_pluginMenu->isItemChecked(a_id))
        {
          plugin->unload();
        }
        else
        {
          plugin->load();
          plugin->run();
        }
        m_pluginMenu->setItemChecked(a_id, TRUE);
        return;
      }
      plugin->load();
      plugin->run();
    }    
  }
}



void QuantaApp::slotPluginsEdit()
{
  QuantaPluginEditor *editor = new QuantaPluginEditor(getView(), "plugin_editor");
  editor->setSearchPaths(m_pluginInterface->searchPaths());
  editor->setPlugins(m_pluginInterface->plugins());

  if(editor->exec())
  {
    m_pluginInterface->setSearchPaths(editor->searchPathList());
    m_pluginInterface->setPlugins(editor->plugins());
    m_pluginInterface->writeConfig();
  }
}

void QuantaApp::slotPluginsValidate()
{
  QDict<QuantaPlugin> plugins = m_pluginInterface->plugins();

  QDictIterator<QuantaPlugin> it(plugins);
  for(;it.current();++it)
  {
    if(!QuantaPlugin::validatePlugin(it.current()))
    {
      int answer = KMessageBox::warningYesNo(getView(), i18n("You have plugins installed that aren't currently valid. Do you want to edit the plugins?"), i18n("Invalid Plugins"));
      if(answer == KMessageBox::Yes)
      {
        slotPluginsEdit();
      }
      return;      
    }
  }
  statusBar()->message(i18n("All plugins validated successfully."));
}


