/***************************************************************************
                          quanta.cpp  -  description
                             -------------------
    begin                : ?? ???  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev <pdima@users.sourceforge.net,yshurik@linuxfan.com>
                           (C) 2001-2003 by Andras Mantia <amantia@kde.org>
                           (C) 2000, 2003 by Eric Laffoon <sequitur@kde.org>
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
#include <qeventloop.h>
#include <qtimer.h>
#include <qdom.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextcodec.h>
#include <qpopupmenu.h>
#include <qdatetime.h>
#include <qradiobutton.h>

// include files for KDE
#include <kaccel.h>
#include <kapplication.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kglobal.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <khtmlview.h>
#include <kstandarddirs.h>
#include <kstatusbar.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kparts/componentfactory.h>
#include <kpopupmenu.h>
#include <kprocess.h>
#include <kprogress.h>
#include <kspell.h>
#include <ktip.h>
#include <kio/netaccess.h>
#if KDE_IS_VERSION(3,1,90)
#include <ktabwidget.h>
#endif

// application specific includes

#include "quanta.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "resource.h"
#include "document.h"
#include "qextfileinfo.h"
#include "dtds.h"
#include <kapplication.h>
#include <dcopclient.h>

#include "project/project.h"

#include "widgets/whtmlpart.h"
#include "messages/messageoutput.h"

#include "toolbar/tagaction.h"

#include "treeviews/filestreeview.h"
#include "treeviews/projecttreeview.h"
#include "treeviews/doctreeview.h"
#include "treeviews/structtreeview.h"
#include "treeviews/templatestreeview.h"
#include "treeviews/tagattributetree.h"
#include "treeviews/scripttreeview.h"

#include "plugins/quantaplugininterface.h"
#include "plugins/quantaplugin.h"
#include "plugins/quantaplugineditor.h"

#include "plugins/php3dbg/debugger.h"
#include "plugins/php4dbg/debugger.h"
#include "plugins/spellchecker.h"

#include "parser/parser.h"
#include "dialogs/filemasks.h"
#include "dialogs/dirtydlg.h"
#include "dialogs/dirtydialog.h"

QuantaApp::QuantaApp() : DCOPObject("WindowManagerIf"), KDockMainWindow(0L,"Quanta")
{
  quantaStarted = true;
  tempFileList.setAutoDelete(true);
  toolbarList.setAutoDelete(true);
  userToolbarsCount = 0;
  baseNode = 0L;
  currentToolbarDTD = QString::null;
  m_config=kapp->config();

  qConfig.globalDataDir = KGlobal::dirs()->findResourceDir("data",resourceDir + "toolbar/quantalogo.png");
  if (qConfig.globalDataDir.isEmpty())
  {
    quantaStarted = false;
    kdWarning() << "***************************************************************************" << endl
                << i18n("Quanta data files were not found.\nYou may forgot to run \"make install\","
                        "or your KDEDIR, KDEDIRS or PATH is not set correctly.!") << endl
                << "***************************************************************************" << endl;
    QTimer::singleShot(20, kapp, SLOT(quit()));
    return;
  }
  qConfig.enableDTDToolbar = true;

  setHighlight = 0;
  grepDialog  = 0L;
  exitingFlag = false;
  qConfig.spellConfig = new KSpellConfig();
  idleTimer = new QTimer(this);
  connect(idleTimer, SIGNAL(timeout()), SLOT(slotIdleTimerExpired()));

  // connect up signals from KXXsldbgPart
  connectDCOPSignal(0, 0, "debuggerPositionChangedQString,int)", "newDebuggerPosition(QString,int)", false );
  connectDCOPSignal(0, 0, "editorPositionChanged(QString,int,int)", "newCursorPosition(QString,int,int)", false );
  connectDCOPSignal(0, 0, "openFile(QString,int,int)", "openFile(QString,int,int)", false);

  m_execCommandPS = 0L;
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
 QStringList tmpDirs = KGlobal::dirs()->resourceDirs("tmp");
 tmpDir = tmpDirs[0];
 for (uint i = 0; i < tmpDirs.count(); i++)
 {
   if (tmpDirs[i].contains("kde-"))
      tmpDir = tmpDirs[i];
 }
 QDir dir;
 dir.rmdir(tmpDir + "quanta");

 delete m_execCommandPS;
 m_execCommandPS = 0L;
}


/** Delayed initialization. */
void QuantaApp::initQuanta()
{
  parser = new Parser();

  QStringList tmpDirs = KGlobal::dirs()->resourceDirs("tmp");
  QDir dir;
  tmpDir = tmpDirs[0];
  for (uint i = 0; i < tmpDirs.count(); i++)
  {
    if (tmpDirs[i].contains("kde-"))
       tmpDir = tmpDirs[i];
  }
  dir.mkdir(tmpDir + "quanta");
  tmpDir += "quanta/quanta";

  scriptBeginRx.setCaseSensitive(false);
  scriptBeginRx.setPattern("(<script)");
  scriptEndRx.setCaseSensitive(false);
  scriptEndRx.setPattern("(/script>)");
  
  initStatusBar();
  
  //defaultDocType must be read before the Project object is created!!
  m_config->setGroup("General Options");
  qConfig.defaultDocType = m_config->readEntry("Default DTD",DEFAULT_DTD);
  if (! DTDs::ref()->find(qConfig.defaultDocType))
     qConfig.defaultDocType = DEFAULT_DTD;

  initDocument ();
  initView     ();
  initProject  ();
  initActions();
  
  DTDs::ref();  // create the class, must be before readOptions() !
  readOptions();

  m_pluginInterface = new QuantaPluginInterface();
  connect(m_pluginInterface, SIGNAL(hideSplash()), SLOT(slotHideSplash()));
  m_pluginInterface->readConfig();

  createGUI( QString::null, false /* conserveMemory */ );

//Compatility code (read the action shortcuts from quantaui.rc)
//TODO: Remove after upgrade from 3.1 is not supported
  QDomDocument doc;
  doc.setContent(KXMLGUIFactory::readConfigFile(xmlFile(), instance()));
  QDomNodeList nodeList = doc.elementsByTagName("ActionProperties");
  QDomNode node = nodeList.item(0).firstChild();
  while (!node.isNull())
  {
    if (node.nodeName() == "Action")
    {
      QDomElement el = node.toElement();
      oldShortcuts.insert(el.attribute("name"), el.attribute("shortcut"));
      node = node.nextSibling();
      el.parentNode().removeChild(el);
    } else
    {
      node = node.nextSibling();
    }
  }

//  applyMainWindowSettings(m_config);
  initPlugins  ();

  m_tagsMenu = new QPopupMenu(this);
  editTagAction->plug(m_tagsMenu);
  m_tagsMenu->insertSeparator();
  menuBar()->insertItem(i18n("&Tags"),m_tagsMenu,-1,TAGS_MENU_PLACE);

  pm_set  = (QPopupMenu*)guiFactory()->container("settings", this);
  connect(pm_set, SIGNAL(aboutToShow()), this, SLOT(settingsMenuAboutToShow()));

  pm_bookmark  = (QPopupMenu*)guiFactory()->container("bookmarks", this);
  connect(pm_bookmark, SIGNAL(aboutToShow()),
          this, SLOT(bookmarkMenuAboutToShow()));

  QPopupMenu* pm_view = (QPopupMenu*)guiFactory()->container("view", this);
  connect(pm_view,SIGNAL(aboutToShow()), this, SLOT(viewMenuAboutToShow()));

  QPopupMenu *toolbarsMenu  = (QPopupMenu*)guiFactory()->container("toolbars_load", this);
  connect(toolbarsMenu, SIGNAL(aboutToShow()), this, SLOT(slotBuildPrjToolbarsMenu()));

  connect(guiFactory()->container("popup_editor", quantaApp), SIGNAL(aboutToShow()), this, SLOT(slotContextMenuAboutToShow()));

  connect(m_messageOutput, SIGNAL(clicked(const QString&,int)),
          this, SLOT(gotoFileAndLine(const QString&,int)));
  connect(m_problemOutput, SIGNAL(clicked(const QString&,int)),
          this, SLOT(gotoFileAndLine(const QString&,int)));

  refreshTimer = new QTimer( this );
  connect(refreshTimer, SIGNAL(timeout()), SLOT(slotReparse()));
  refreshTimer->start( qConfig.refreshFrequency*1000, false ); //update the structure tree every 5 seconds
  if (qConfig.instantUpdate || qConfig.refreshFrequency == 0)
  {
    refreshTimer->stop();
  }

  slotFileNew();
  initToolBars();
  KTipDialog::showTip(this);

  //get the PID of this running instance
  qConfig.quantaPID = QString::number(int(getpid()), 10);
  qConfig.backupDirPath = KGlobal::instance()->dirs()->saveLocation("data", resourceDir + "backups/");

  autosaveTimer = new QTimer( this );
  connect(autosaveTimer, SIGNAL(timeout()), SLOT(slotAutosaveTimer()));
  autosaveTimer->start(qConfig.autosaveInterval * 60000, false);

  connect(m_doc, SIGNAL(hideSplash()), SLOT(slotHideSplash()));
  
  connect(parser, SIGNAL(rebuildStructureTree()), this, SLOT(slotReloadStructTreeView()));
}


void QuantaApp::initToolBars()
{
 if (toolbarList.count() == 0)
     loadToolbarForDTD(Project::ref()->defaultDTD());
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
  m_doc = new QuantaDoc(this);
  connect(m_doc, SIGNAL(newStatus()),    this, SLOT(slotNewStatus()));
  connect(m_doc, SIGNAL(documentClosed()), ProjectTreeView::ref(), SLOT(slotDocumentClosed()));
}

void QuantaApp::initProject()
{
  Project *m_project = Project::ref();

  connect(m_project,  SIGNAL(openFile    (const KURL &, const QString&)),
          this,     SLOT  (slotFileOpen(const KURL &, const QString&)));
  connect(m_project,  SIGNAL(closeFile   (const KURL &)),
          this,     SLOT  (slotFileClose(const KURL &)));
  connect(m_project,  SIGNAL(reloadTree(const ProjectUrlList & ,bool)),
          pTab,     SLOT  (slotReloadTree(const ProjectUrlList &,bool)));
  connect(m_project,  SIGNAL(closeFiles()),
          m_doc,      SLOT  (closeAll()));
  connect(m_project,  SIGNAL(showTree()),
          this,     SLOT  (slotShowProjectTree()));

  connect(fTab,    SIGNAL(insertDirInProject(const KURL&)),
          m_project,  SLOT  (slotAddDirectory(const KURL&)));

  connect(fTab,    SIGNAL(insertFileInProject(const KURL&)),
          m_project,  SLOT  (slotInsertFile(const KURL&)));

  connect(TemplatesTreeView::ref(),    SIGNAL(insertDirInProject(const KURL&)),
          m_project,  SLOT  (slotAddDirectory(const KURL&)));

  connect(TemplatesTreeView::ref(),    SIGNAL(insertFileInProject(const KURL&)),
          m_project,  SLOT  (slotInsertFile(const KURL&)));

  // inform project if something was renamed
  connect(pTab,     SIGNAL(renamed(const KURL&, const KURL&)),
          m_project,  SLOT  (slotRenamed(const KURL&, const KURL&)));
  connect(fTab,     SIGNAL(renamed(const KURL&, const KURL&)),
          m_project,  SLOT  (slotRenamed(const KURL&, const KURL&)));
  connect(tTab,     SIGNAL(renamed(const KURL&, const KURL&)),
          m_project,  SLOT  (slotRenamed(const KURL&, const KURL&)));

  connect(pTab,     SIGNAL(removeFromProject(const KURL&)),
          m_project,  SLOT  (slotRemove(const KURL&)));
  connect(pTab,     SIGNAL(uploadSingleURL(const KURL&)),
          m_project,  SLOT  (slotUploadURL(const KURL&)));
  connect(pTab,     SIGNAL(rescanProjectDir()),
          m_project,  SLOT  (slotRescanPrjDir()));
  connect(pTab,     SIGNAL(showProjectOptions()),
          m_project,  SLOT  (slotOptions()));
  connect(pTab,     SIGNAL(uploadProject()),
          m_project,  SLOT  (slotUpload()));

  connect(m_project,  SIGNAL(enableMessageWidget(bool)),
          this,     SLOT  (slotEnableMessageWidget(bool)));

  connect(m_project,  SIGNAL(messages(const QString&)),
          m_messageOutput, SLOT(showMessage(const QString&)));

  connect(m_project,  SIGNAL(saveAllFiles()),
          this, SLOT(slotFileSaveAll()));
  connect(m_project,  SIGNAL(newStatus()),
          this, SLOT(slotNewStatus()));

  connect(m_project,  SIGNAL(newProjectLoaded(const QString &, const KURL &, const KURL &)),
          TemplatesTreeView::ref(), SLOT(slotNewProjectLoaded(const QString &, const KURL &, const KURL &)));
  connect(m_project,  SIGNAL(newProjectLoaded(const QString &, const KURL &, const KURL &)),
          pTab, SLOT(slotNewProjectLoaded(const QString &, const KURL &, const KURL &)));
  connect(m_project,  SIGNAL(newProjectLoaded(const QString &, const KURL &, const KURL &)),
          fTab, SLOT(slotNewProjectLoaded(const QString &, const KURL &, const KURL &)));
  connect(pTab,    SIGNAL(changeFileDescription(const KURL&, const QString&)),
          m_project, SLOT(slotFileDescChanged(const KURL&, const QString&)));

  connect(m_project, SIGNAL(hideSplash()), SLOT(slotHideSplash()));

}

void QuantaApp::initView()
{
  ////////////////////////////////////////////////////////////////////
  // create the main widget here that is managed by KTMainWindow's view-region and
  // connect the widget to your document to display document contents.

  maindock = createDockWidget( "Editor", UserIcon("textarea"  ), 0L, i18n("Editor"), 0L);
  bottdock = createDockWidget( "Output", UserIcon("output_win"), 0L, i18n("Message output"), 0L);
  bottdock->setToolTipString(i18n("Message output"));
  problemsdock = createDockWidget( "Problems", SmallIcon("stop"), 0L, i18n("Problem reporter"), 0L);
  problemsdock->setToolTipString(i18n("Problem reporter"));

  atabdock = createDockWidget("TagAttributes", UserIcon("tag_misc"), 0L, i18n("Attribute tree view"), "");
  atabdock->setToolTipString(i18n("Attribute tree view"));
  ftabdock = createDockWidget("Files", UserIcon("ftab"), 0L, i18n("Files tree view"), "");
  ftabdock->setToolTipString(i18n("Files tree view"));
  ptabdock = createDockWidget("Project", UserIcon("ptab"), 0L, i18n("Project tree view"), "");
  ptabdock->setToolTipString(i18n("Project tree view"));
  ttabdock = createDockWidget("Templates", UserIcon("ttab"), 0L, i18n("Templates tree view"), "");
  ttabdock->setToolTipString(i18n("Templates tree view"));
  scripttabdock = createDockWidget("Scripts", SmallIcon("run"), 0L, i18n("Scripts tree view"), "");
  scripttabdock->setToolTipString(i18n("Scripts tree view"));
  stabdock = createDockWidget("Struct", BarIcon ("view_sidetree"), 0L, i18n("Structure view (DOM tree)"), "");
  stabdock->setToolTipString(i18n("Structure view (DOM tree)"));
  dtabdock = createDockWidget("Docs", BarIcon ("contents"), 0L, i18n("Documentation"), "");
  dtabdock->setToolTipString(i18n("Documentation"));

  m_oldTreeViewWidget = ptabdock;
  QStringList topStrList;
  m_config->setGroup("General Options");
#if KDE_IS_VERSION(3,1,3)
  topStrList = m_config->readPathListEntry("Top folders");
#else
  topStrList = m_config->readListEntry("Top folders");
#endif
  KURL::List topList;
  KURL url;
  for (uint i = 0; i < topStrList.count(); i++)
  {
    url = KURL();
    QuantaCommon::setUrl(url, topStrList[i]);
    topList.append(url);
  }

  m_config->setGroup("General Options");
  if (m_config->readBoolEntry("Home-Root Folder On", true))
  {
    url = KURL();
    url.setPath("/");
    if (!topList.contains(url))
        topList.append(url);
    url = KURL();
    url.setPath(QDir::homeDirPath()+"/");
    if (!topList.contains(url))
        topList.append(url);
  }
  fTab = new FilesTreeView(topList, ftabdock);
  aTab = new EnhancedTagAttributeTree(atabdock);
  pTab = ProjectTreeView::ref(ptabdock );
  tTab = TemplatesTreeView::ref(ttabdock);  // creates the treeview
  dTab = new DocTreeView(dtabdock);
  StructTreeView *sTab = StructTreeView::ref(stabdock ,"struct");
  scriptTab = new ScriptTreeView(scripttabdock);

  rightWidgetStack = new QWidgetStack(maindock);
  bottomWidgetStack = new QWidgetStack(bottdock);
  m_view = new QuantaView(rightWidgetStack);
  connect(m_view, SIGNAL(dragInsert(QDropEvent *)), tTab,
                  SLOT(slotDragInsert(QDropEvent *)));

  maindock->setWidget(rightWidgetStack);
  bottdock->setWidget(bottomWidgetStack);
  atabdock->setWidget(aTab);
  ftabdock->setWidget(fTab);
  ptabdock->setWidget(pTab);
  ttabdock->setWidget(tTab);
  scripttabdock->setWidget(scriptTab);
  stabdock->setWidget(sTab);
  dtabdock->setWidget(dTab);

  maindock->setFocusPolicy(QWidget::StrongFocus);

  rightWidgetStack->setMinimumHeight(1);

  m_htmlPart = new WHTMLPart(rightWidgetStack,"rightHTML");
  m_htmlPartDoc = new WHTMLPart(rightWidgetStack, "docHTML");

  rightWidgetStack->addWidget(m_view, 0);
  rightWidgetStack->addWidget(m_htmlPart->view(), 1);
  rightWidgetStack->addWidget(m_htmlPartDoc->view(), 2);
  rightWidgetStack->raiseWidget(0);

  m_messageOutput = new MessageOutput(bottomWidgetStack);
  m_messageOutput->setFocusPolicy(QWidget::NoFocus);
  m_messageOutput->showMessage(i18n("Message Window...\n"));

  bottomWidgetStack->addWidget(m_messageOutput, 0);
//  bottomWidgetStack->addWidget( m_htmlPart   ->view(), 1 );
//  bottomWidgetStack->addWidget( htmlPartDoc->view(), 2 );

  m_problemOutput = new MessageOutput(problemsdock);
  m_problemOutput->setFocusPolicy(QWidget::NoFocus);
  problemsdock->setWidget(m_problemOutput);

  connect(fTab, SIGNAL(openFile  (const KURL &, const QString&)),
          this, SLOT(slotFileOpen(const KURL &, const QString&)));
  connect(fTab, SIGNAL(openImage(const KURL&)),
          this, SLOT  (slotImageOpen(const KURL&)));

  connect(pTab, SIGNAL(openFile  (const KURL &, const QString&)),
          this, SLOT(slotFileOpen(const KURL &, const QString&)));
  connect(pTab, SIGNAL(openImage  (const KURL&)),
          this, SLOT(slotImageOpen(const KURL&)));
  connect(pTab, SIGNAL(loadToolbarFile  (const KURL&)),
          this, SLOT(slotLoadToolbarFile(const KURL&)));

  connect(fTab, SIGNAL(closeFile   (const KURL &)),
          this, SLOT  (slotFileClose(const KURL &)));
  connect(pTab, SIGNAL(closeFile   (const KURL &)),
          this, SLOT  (slotFileClose(const KURL &)));
  connect(tTab, SIGNAL(closeFile   (const KURL &)),
          this, SLOT  (slotFileClose(const KURL &)));

  connect(tTab, SIGNAL(openImage  (const KURL&)),
          this, SLOT(slotImageOpen(const KURL&)));
  connect(tTab, SIGNAL(openFile  (const KURL &, const QString&)),
          this, SLOT(slotFileOpen(const KURL &, const QString&)));
  connect(tTab, SIGNAL(insertFile  (const KURL &)),
          this, SLOT(slotInsertFile(const KURL &)));
  connect(tTab, SIGNAL(insertTag(const KURL &, DirInfo)),
          this, SLOT(slotInsertTag(const KURL &, DirInfo)));

  connect(scriptTab, SIGNAL(openFile(const KURL &, const QString&)),
          this, SLOT(slotFileOpen(const KURL &, const QString&)));
  connect(scriptTab, SIGNAL(openFileInPreview(const KURL &)),
          this, SLOT(slotOpenFileInPreview(const KURL &)));
  connect(scriptTab, SIGNAL(showPreviewWidget(bool)),
          this, SLOT(slotShowPreviewWidget(bool)));
  connect(scriptTab, SIGNAL(assignActionToScript(const KURL &, const QString&)),
          this, SLOT(slotAssignActionToScript(const KURL &, const QString&)));


  connect(fTab, SIGNAL(insertTag(const KURL &, DirInfo)),
          this, SLOT(slotInsertTag(const KURL &, DirInfo)));
  connect(pTab, SIGNAL(insertTag(const KURL &, DirInfo)),
          this, SLOT(slotInsertTag(const KURL &, DirInfo)));

  connect(fTab, SIGNAL(showPreviewWidget(bool)),
          this, SLOT(slotShowPreviewWidget(bool)));
  connect(pTab, SIGNAL(showPreviewWidget(bool)),
          this, SLOT(slotShowPreviewWidget(bool)));

  connect(m_htmlPart, SIGNAL(onURL(const QString&)),
              this, SLOT(slotStatusMsg(const QString&)));
  connect(m_htmlPartDoc, SIGNAL(onURL(const QString&)),
                 this, SLOT(slotStatusMsg(const QString&)));

  connect(m_view, SIGNAL(newCurPos()), this, SLOT(slotNewLineColumn()));

  connect(sTab, SIGNAL(newCursorPosition(int,int)), SLOT(setCursorPosition(int,int)));
  connect(sTab, SIGNAL(selectArea(int,int,int,int)), SLOT( selectArea(int,int,int,int)));
  connect(sTab, SIGNAL(needReparse()), SLOT(slotForceReparse()));
  connect(sTab, SIGNAL(parsingDTDChanged(const QString&)), SLOT(slotParsingDTDChanged(const QString&)));
  connect(sTab, SIGNAL(openFile    (const KURL &, const QString&)),
          this, SLOT  (slotFileOpen(const KURL &, const QString&)));
  connect(sTab, SIGNAL(openImage  (const KURL&)),
          this, SLOT(slotImageOpen(const KURL&)));
  connect(sTab, SIGNAL(showPreviewWidget(bool)),
          this, SLOT(slotShowPreviewWidget(bool)));
  connect(parser, SIGNAL(nodeTreeChanged()), sTab, SLOT(slotNodeTreeChanged()));

  connect(dTab, SIGNAL(openURL(const QString&)), SLOT(openDoc(const QString&)));

  connect(m_view, SIGNAL(dragInsert(QDropEvent *)), tTab, SLOT(slotDragInsert(QDropEvent *)));

  setMainDockWidget( maindock );
  setView(maindock);

  qConfig.windowLayout = "Default";
  layoutDockWidgets(qConfig.windowLayout);
  KDockManager *mng = ftabdock->dockManager();
  connect(mng, SIGNAL(change()), this, SLOT(slotDockChanged()));

//signal docking ststus changes
  connectDockSignals(maindock);
  connectDockSignals(bottdock);
  connectDockSignals(problemsdock);
  connectDockSignals(ftabdock);
  connectDockSignals(ptabdock);
  connectDockSignals(ttabdock);
  connectDockSignals(stabdock);
  connectDockSignals(dtabdock);
  connectDockSignals(atabdock);
  connectDockSignals(scripttabdock);
}

void QuantaApp::connectDockSignals(QObject *obj)
{
  connect(obj, SIGNAL(docking(KDockWidget*, KDockWidget::DockPosition)), SLOT(slotDockStatusChanged()));
  connect(obj, SIGNAL( setDockDefaultPos()), SLOT(slotDockStatusChanged()));
  connect(obj, SIGNAL( headerCloseButtonClicked()), SLOT(slotDockStatusChanged()));
  connect(obj, SIGNAL( headerDockbackButtonClicked()), SLOT(slotDockStatusChanged()));
  connect(obj, SIGNAL( iMBeingClosed()), SLOT(slotDockStatusChanged()));
  connect(obj, SIGNAL( hasUndocked()), SLOT(slotDockStatusChanged()));
}

QWidgetStack *QuantaApp::widgetStackOfHtmlPart()
{
  QWidgetStack *s;
  if (qConfig.previewPosition == "Bottom")
  {
    s = bottomWidgetStack;
  } else
  {
    s = rightWidgetStack;
  }
//TODO: This should be done on startup and after the setting has changed
  if (m_htmlPart->view()->parentWidget() != s)
  {
    s->addWidget( m_htmlPart->view(), 1 );
    s->addWidget( m_htmlPartDoc->view(), 2 );
  }

  return s;
}

void QuantaApp::saveOptions()
{
  if (m_config)
  {
    m_config->setGroup  ("General Options");

    m_config->writeEntry("Geometry", size());

    m_config->writeEntry("Show Toolbar", toolBar("mainToolBar")->isVisible());
    m_config->writeEntry("Show DTD Toolbar", showDTDToolbar->isChecked());
    m_config->writeEntry("Show Statusbar", statusBar()->isVisible());

    m_config->writeEntry("Markup mimetypes", qConfig.markupMimeTypes  );
    m_config->writeEntry("Script mimetypes", qConfig.scriptMimeTypes   );
    m_config->writeEntry("Image mimetypes", qConfig.imageMimeTypes );
    m_config->writeEntry("Text mimetypes", qConfig.textMimeTypes  );

    m_config->writeEntry("Capitals for tags", qConfig.tagCase);
    m_config->writeEntry("Capitals for attr", qConfig.attrCase);
    m_config->writeEntry("Attribute quotation", qConfig.attrValueQuotation=='"' ? "double":"single");
    m_config->writeEntry("Close tag if optional", qConfig.closeOptionalTags);
    m_config->writeEntry("Close tags", qConfig.closeTags);
    m_config->writeEntry("Auto completion", qConfig.useAutoCompletion);
    m_config->writeEntry("Update Closing Tags", qConfig.updateClosingTags);

//    m_config->writeEntry("DynamicWordWrap", qConfig.dynamicWordWrap);

    m_config->writeEntry("Default encoding", qConfig.defaultEncoding);
    m_config->writeEntry("Default DTD", qConfig.defaultDocType);

    m_config->writeEntry("Preview position", qConfig.previewPosition);
    m_config->writeEntry("Window layout", qConfig.windowLayout);
    m_config->writeEntry("Follow Cursor", StructTreeView::ref()->followCursor() );
    m_config->writeEntry("PHP Debugger Port", phpDebugPort );
    //If user choose the timer interval, it needs to restart the timer too
    m_config->writeEntry("Autosave interval", qConfig.autosaveInterval);
#if KDE_IS_VERSION(3,1,3)
    m_config->writePathEntry("Top folders", fTab->topURLList.toStringList());
    m_config->writePathEntry("List of opened files", m_doc->openedFiles().toStringList());
#else
    m_config->writeEntry("Top folders", fTab->topURLList.toStringList());
    m_config->writeEntry("List of opened files", m_doc->openedFiles().toStringList());
#endif
    m_config->writeEntry("Version", VERSION); // version
    m_config->writeEntry ("Enable Debugger", debuggerStyle!="None");
    m_config->writeEntry ("PHP Debugger style", debuggerStyle);
    m_config->writeEntry("Show Close Buttons", qConfig.showCloseButtons);

    if (m_view->writeExists())
        m_view->write()->writeConfig(m_config);

    m_config->deleteGroup("RecentFiles");
    fileRecent->saveEntries(m_config);

    m_config->setGroup("Parser Options");
    m_config->writeEntry("Instant Update", qConfig.instantUpdate);
    m_config->writeEntry("Show Empty Nodes", qConfig.showEmptyNodes);
    m_config->writeEntry("Show Closing Tags", qConfig.showClosingTags);
    m_config->writeEntry("Refresh frequency", qConfig.refreshFrequency);
    m_config->writeEntry("Expand Level", qConfig.expandLevel);
    m_config->writeEntry("Show DTD Select Dialog", qConfig.showDTDSelectDialog);

    m_config->setGroup("Quanta View");
    m_config->writeEntry("LineNumbers", qConfig.lineNumbers);
    m_config->writeEntry("Iconbar", qConfig.iconBar);
    m_config->writeEntry("DynamicWordWrap", qConfig.dynamicWordWrap);
   // m_doc->writeConfig(m_config); // kwrites
    Project::ref()->writeConfig(m_config); // project
    manager()->writeConfig(m_config);
    //saveMainWindowSettings(m_config);
    writeDockConfig(m_config);
    SpellChecker::ref()->writeConfig(m_config);
    m_config->sync();
  }
}


void QuantaApp::readOptions()
{
  m_config->setGroup("General Options");
  //if (2>m_config->readNumEntry("Version",0)) m_config = new KConfig();

  qConfig.markupMimeTypes = m_config->readEntry("Markup mimetypes", qConfig.markupMimeTypes);
  qConfig.scriptMimeTypes = m_config->readEntry("Script mimetypes", qConfig.scriptMimeTypes);
  qConfig.imageMimeTypes = m_config->readEntry("Image mimetypes", qConfig.imageMimeTypes);
  qConfig.textMimeTypes = m_config->readEntry("Text mimetypes", qConfig.textMimeTypes);

  qConfig.tagCase = m_config->readNumEntry("Capitals for tags", 0);
  qConfig.attrCase = m_config->readNumEntry("Capitals for attr", 0);
  qConfig.attrValueQuotation = (m_config->readEntry("Attribute quotation", "double") == "double" )? '"':'\'';
  qConfig.closeOptionalTags = m_config->readBoolEntry("Close tag if optional", true);
  qConfig.closeTags = m_config->readBoolEntry("Close tags", true);
  qConfig.useAutoCompletion = m_config->readBoolEntry("Auto completion",true);
  qConfig.updateClosingTags = m_config->readBoolEntry("Update Closing Tags", true);

  qConfig.defaultEncoding = m_config->readEntry("Default encoding", QTextCodec::codecForLocale()->name());

  phpDebugPort = m_config->readNumEntry("PHP Debugger Port", 7869);

  StructTreeView::ref()->setFollowCursor( m_config->readBoolEntry("Follow Cursor", true));

  qConfig.previewPosition   = m_config->readEntry("Preview position","Right");

  QSize s(800,580);
  resize( m_config->readSizeEntry("Geometry", &s));
  qConfig.autosaveInterval = m_config->readNumEntry("Autosave interval", 1);

  KToggleAction *showToolbarAction = (KToggleAction *) actionCollection()->action( "view_toolbar" );
  if (!m_config->readBoolEntry("Show Toolbar",true))
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
  if (!m_config->readBoolEntry("Show Statusbar", true))
  {
     showStatusbarAction->setChecked(false);
  } else
  {
     showStatusbarAction->setChecked(true);
  }
  slotViewStatusBar();
  showToolbarAction  ->setChecked(m_config->readBoolEntry("Show Toolbar",   true));
  qConfig.enableDTDToolbar = m_config->readBoolEntry("Show DTD Toolbar",true);
  showDTDToolbar->setChecked(qConfig.enableDTDToolbar);
  qConfig.showCloseButtons = m_config->readBoolEntry("Show Close Buttons", true);
#if KDE_IS_VERSION(3,1,90)
  KTabWidget *tab = static_cast<KTabWidget*>(m_view->writeTab());
  if (qConfig.showCloseButtons)
  {
    tab->setHoverCloseButton(true);
    tab->setHoverCloseButtonDelayed(false);
  } else
  {
    tab->setHoverCloseButton(false);
  }
#else
  qConfig.showCloseButtons = false;
#endif

  fileRecent ->loadEntries(m_config);

  m_config->setGroup("Parser Options");
  qConfig.showEmptyNodes = m_config->readBoolEntry("Show Empty Nodes", false);
  qConfig.showClosingTags = m_config->readBoolEntry("Show Closing Tags", false);
  qConfig.instantUpdate = m_config->readBoolEntry("Instant Update", false);
  qConfig.refreshFrequency = m_config->readNumEntry("Refresh frequency",5);
  qConfig.expandLevel = m_config->readNumEntry("Expand Level", 4);
  qConfig.showDTDSelectDialog = m_config->readBoolEntry("Show DTD Select Dialog", true);

  m_config->setGroup("Quanta View");
  qConfig.lineNumbers = m_config->readBoolEntry("LineNumbers", false);
  qConfig.iconBar = m_config->readBoolEntry("Iconbar", false);
  qConfig.dynamicWordWrap = m_config->readBoolEntry("DynamicWordWrap",false);
  viewBorder->setChecked(qConfig.iconBar);
  viewLineNumbers->setChecked(qConfig.lineNumbers);
  viewDynamicWordWrap->setChecked(qConfig.dynamicWordWrap);
  manager()->readConfig(m_config);

#ifdef BUILD_KAFKAPART
  quantaApp->view()->readConfig(m_config);
#endif

  m_previewVisible = false;
  m_noFramesPreview = false;
#ifdef BUILD_KAFKAPART
  showKafkaAction->setChecked( false );
#endif
  showMessagesAction->setChecked( bottdock->parent() != 0L );

  m_doc->readConfig(m_config); // kwrites
  Project::ref()->readConfig(m_config); // project

  if (m_view->writeExists() && m_view->write()->isUntitled())
  {
    m_doc->closeDocument();
    m_doc->openDocument(KURL());
  }

  debuggerStyle = "None";
  m_config->setGroup  ("General Options");
  if (m_config->readBoolEntry("Enable Debugger", false))
    if (m_config->readEntry("PHP Debugger style","None") == "PHP4")
         enablePhp4Debug(true);
    else enablePhp3Debug(true);

  SpellChecker::ref()->readConfig(m_config);
  readDockConfig(m_config);

  m_config->setGroup  ("General Options");
  QString layout = m_config->readEntry("Window layout", "Custom");
  layoutDockWidgets(layout);
  qConfig.windowLayout = layout;

}

void QuantaApp::enablePhp3Debug(bool enable)
{
  if (enable) {
    dbg3 = new PHP3Debugger( phpDebugPort,0,0);

    connect( dbg3,          SIGNAL(newConnect()),
             m_messageOutput, SLOT(newPhpConnect()) );
    connect( dbg3,          SIGNAL(endConnect()),
             m_messageOutput, SLOT(endPhpConnect()) );
    connect( dbg3,          SIGNAL(data(const QString&)),
             m_messageOutput, SLOT(phpDebug(const QString&)) );

    if ( !dbg3->ok() )
        m_messageOutput->insertItem(i18n("Can't bind port %1, PHP3 debugger disabled").arg(phpDebugPort));
    else
        m_messageOutput->insertItem(i18n("PHP3 debugger listens at port %1").arg(phpDebugPort));
    debuggerStyle = "PHP3";
  } else delete dbg3;
}

void QuantaApp::enablePhp4Debug(bool enable)
{
  if (enable) {
    dbg4 = new PHP4Debugger(0L,0L);
    connect( dbg4,          SIGNAL(message(const QString&)),
             m_messageOutput, SLOT(php4Debug(const QString&)) );
    dbg4->init();
    debuggerStyle = "PHP4";
  } else delete dbg4;
}

void QuantaApp::openLastFiles()
{

  // Reload previously opened files only if setting allows
  m_config->setGroup("General Options");

  // we need to check config
  // because project now can be
  // in load stage ( remote prj )
  m_config->setGroup  ("Projects");
  QString pu = m_config->readPathEntry("Last Project");

  KURL u;
  QuantaCommon::setUrl(u, pu);
  bool isPrj = true;
  if (pu.isEmpty())
     isPrj = false;
  if (!u.isValid())
     isPrj = false;

  m_config->setGroup("General Options");

#if KDE_IS_VERSION(3,1,3)
  QStringList urls = m_config->readPathListEntry("List of opened files");
#else
  QStringList urls = m_config->readListEntry("List of opened files");
#endif
  m_doc->blockSignals(true);
  m_view->writeTab()->blockSignals(true);
  for ( QStringList::Iterator it = urls.begin(); it != urls.end(); ++it )
  {
    KURL fu;
    QuantaCommon::setUrl(fu, *it);

    if (!m_doc->isOpened(fu) && (!isPrj || fu.isLocalFile()))
      m_doc->openDocument(fu, QString::null, false);
 //   kapp->eventLoop()->processEvents( QEventLoop::ExcludeUserInput | QEventLoop::ExcludeSocketNotifiers);
  }
  m_config->sync();
  m_doc->blockSignals(false);
  m_view->writeTab()->blockSignals(false);
  Document *w = m_view->write();
  if (w) //w==0 might happen on quick close on startup
  {
    setCaption(w->url().prettyURL() );
    slotUpdateStatus(w);
  }
}

/** Loads the initial project */
void QuantaApp::loadInitialProject(const QString& url)
{
  if(url.isNull())
  {
    // Get config
    KConfig *config = quantaApp->config();
    config->setGroup("General Options");

    // Reload last project if setting is enabled
    Project::ref()->readLastConfig();
  }
  else
    Project::ref()->slotOpenProject(KURL( url ));
}


bool QuantaApp::queryClose()
{
  bool canExit = true;
  if (quantaStarted)
  {
    if (dtabdock->isVisible())
    {
      QWidgetStack *s = widgetStackOfHtmlPart();
      s->raiseWidget(0);
    }
    saveOptions();
    exitingFlag = true;
    canExit = m_doc->saveAll(false);
    if (canExit)
        canExit = removeToolbars();
    if (canExit)
    {
      disconnect( m_view->writeTab(),SIGNAL(currentChanged(QWidget*)), this,     SLOT(slotUpdateStatus(QWidget*)));
      //avoid double question about saving files, so set the "modified"
      //flags to "false". This is safe here.
      Document *w;
      for (int i = m_view->writeTab()->count() -1; i >=0; i--)
      {
        w = dynamic_cast<Document*>(m_view->writeTab()->page(i));
        if (w)
           w->setModified(false);
      }
      Project::ref()->slotCloseProject();
      do
      {
        if (m_view->writeExists())
        {
          w = m_view->write();
          w->closeTempFile();
        }
      }while (m_view->removeWrite());
    }
  }

  return canExit;
}



void QuantaApp::initActions()
{
    KActionCollection *ac = actionCollection();

    editTagAction = new KAction( i18n( "&Edit Current Tag..." ), CTRL+Key_E,
                        m_view, SLOT( slotEditCurrentTag() ),
                        ac, "edit_current_tag" );
    new KAction( i18n( "E&xpand Abbreviation" ), CTRL+Key_J,
                        this, SLOT( slotExpandAbbreviation() ),
                        ac, "expand_abbreviation" );

    //Kate actions

//Edit menu
    KStdAction::undo(m_view, SLOT(slotUndo()), ac);
    KStdAction::redo(m_view, SLOT(slotRedo()), ac);

    KStdAction::cut(m_view, SLOT(slotCut()), ac);
    KStdAction::copy(m_view, SLOT(slotCopy()), ac) ;
#if KDE_VERSION < KDE_MAKE_VERSION(3,1,92)
    (void) new KQPasteAction(i18n("Paste"), "editpaste", KStdAccel::shortcut(KStdAccel::Paste), m_view, SLOT(slotPaste()), ac, "edit_paste");
#else
    KStdAction::pasteText(m_view, SLOT(slotPaste()), ac);
#endif

    KStdAction::selectAll(this, SLOT(slotSelectAll()), ac);
    KStdAction::deselect(m_view, SLOT(slotDeselectAll()), ac);
    (void) new KAction( i18n( "Toggle &Block Selection" ), Key_F4, m_view,
                        SLOT( toggleVertical() ), ac, "set_verticalSelect" );
    new KAction(i18n("Toggle Insert"), Key_Insert, m_view, SLOT(toggleInsert()), ac, "set_insert" );


    KStdAction::find(this, SLOT(slotFind()), ac);
    new KAction(i18n("Find &Next"), "findnext", KStdAccel::shortcut(KStdAccel::FindNext), this, 
                 SLOT(slotFindAgain()), ac, "edit_find_next");
    KStdAction::findPrev(this, SLOT(slotFindAgainB()), ac, "edit_find_prev");
    KStdAction::replace(this, SLOT(slotReplace()), ac);

    new KAction(i18n("&Indent"), "indent", CTRL+Key_I, m_view,
                SLOT(slotIndent()), ac, "edit_indent");
    new KAction(i18n("Unindent"), "unindent", CTRL+SHIFT+Key_I, m_view,
                SLOT(slotUnIndent()), ac, "edit_unindent");
    new KAction(i18n("Cl&ean Indentation"), "cleanindent", 0, m_view,
                SLOT(slotCleanIndent()), ac, "edit_cleanindent");

    new KAction(i18n("Co&mment"),  CTRL+Qt::Key_NumberSign,
                m_view, SLOT(slotComment()), ac, "edit_comment");
    new KAction(i18n("Unc&omment"),
                CTRL+SHIFT+Qt::Key_NumberSign, m_view, SLOT(slotUnComment()),
                ac, "edit_uncomment");
    new KAction(i18n("Apply &Word Wrap"), "", 0, m_view, SLOT(slotApplyWordWrap()),
                ac, "edit_apply_wordwrap");



//Tools menu
    KStdAction::gotoLine(m_view, SLOT(slotGotoLine()), ac, "edit_goto_line");
    KStdAction::spelling(m_view, SLOT(slotSpellcheck()), ac);

//Bookmarks
    bookmarkToggle = new KAction(i18n("Toggle &Bookmark"), Qt::CTRL+Qt::Key_B,
          m_view, SLOT(toggleBookmark()), ac, "bookmarks_toggle");
    bookmarkClear = new KAction(i18n("&Clear Bookmarks"), 0, m_view,
          SLOT(clearBookmarks()), ac, "bookmarks_clear");

//Settings
  viewBorder =  new KToggleAction(i18n("Show &Icon Border"), Qt::SHIFT+Qt::Key_F9, m_view,
                    SLOT(toggleIconBorder()), ac, "view_border");
  viewLineNumbers =  new KToggleAction(i18n("Show &Line Numbers"), Key_F11, m_view,
                        SLOT(toggleLineNumbers()), ac, "view_line_numbers");

//help
   (void) new KAction(i18n("Ti&p of the Day"), "idea", "", this,
      SLOT(slotHelpTip()), ac, "help_tip");

//    viewFoldingMarkes =  new KToggleAction(i18n("Show Folding &Markers"), Key_F9, m_view,
//                              SLOT(toggleFoldingMarkers()), ac, "view_folding_markers");
  viewDynamicWordWrap = new KToggleAction(i18n("&Dynamic Word Wrap"), Key_F10, m_view,
                              SLOT(toggleDynamicWordWrap()), ac, "view_dynamic_word_wrap");

  (void) new KAction( i18n( "Configure &Editor..." ), SmallIcon("configure"), 0,
                      m_view, SLOT( slotEditorOptions() ), ac, "editor_options" );


    setEndOfLine = new KSelectAction(i18n("End of &Line"), 0, ac,"set_eol");
    connect(setEndOfLine, SIGNAL(activated(int)), m_view, SLOT(setEol(int)));
    connect(setEndOfLine->popupMenu(), SIGNAL(aboutToShow()), this, SLOT(setEOLMenuAboutToShow()));

    QStringList list;
    list.append(i18n("&Unix"));
    list.append(i18n("&Windows/DOS"));
    list.append(i18n("&Macintosh"));
    setEndOfLine->setItems(list);


    // File actions
    //
    KStdAction::openNew( this, SLOT( slotFileNew()  ), ac);
    KStdAction::open   ( this, SLOT( slotFileOpen() ), ac, "file_open");
    KStdAction::close  ( this, SLOT( slotFileClose()), ac);
    (void) new KAction(i18n("Close Other Tabs"), 0, m_view, SLOT(slotCloseOtherTabs()), ac, "close_other_tabs");

#if KDE_VERSION < KDE_MAKE_VERSION(3,1,92)
    fileRecent =  new KQRecentFilesAction(i18n("Open &Recent"), "fileopen", 0,
                      this, SLOT(slotFileOpenRecent(const KURL&)), ac,
                      "file_open_recent");
#else
    fileRecent =  KStdAction::openRecent(this, SLOT(slotFileOpenRecent(const KURL&)),
                                         ac, "file_open_recent");
#endif
    fileRecent->setMaxItems(32);
    fileRecent->setToolTip(i18n("Open / Open Recent"));
    connect(fileRecent, SIGNAL(activated()), this, SLOT(slotFileOpen()));

    (void) new KAction( i18n( "Close All" ), 0, this,
                        SLOT( slotFileCloseAll() ),
                        ac, "file_close_all" );

    saveAction = KStdAction::save(this, SLOT( slotFileSave() ), ac);

    KStdAction::saveAs( this, SLOT( slotFileSaveAs() ), ac );

    saveAllAction = new KAction( i18n( "Save All..." ), UserIconSet("save_all"), SHIFT+KStdAccel::shortcut(KStdAccel::Save).keyCodeQt(),
                        this, SLOT( slotFileSaveAll() ),
                        ac, "file_save_all" );
    (void)  new KAction(i18n("Reloa&d"), UserIconSet("reload"), CTRL+Key_F5, this,
                        SLOT(slotFileReload()), ac, "file_reload");
//    (void)  new KAction(i18n("Reload All "), 0, 0, this,
//                        SLOT(slotFileReloadAll()), ac, "file_reload_all");

    (void) new KAction( i18n( "Save as Local Template..." ), 0,
                        this, SLOT( slotFileSaveAsLocalTemplate() ),
                        ac, "save_local_template" );
    saveAsProjectTemplateAction = new KAction( i18n( "Save as Project Template..." ), 0,
                        this, SLOT( slotFileSaveAsProjectTemplate() ),
                        ac, "save_project_template" );

    (void) new KAction( i18n( "Save Selection to Local Template File..." ), 0,
                        this, SLOT( slotFileSaveSelectionAsLocalTemplate() ),
                        ac, "save_selection_local_template" );
    saveSelectionAsProjectTemplateAction = new KAction( i18n( "Save Selection to Project Template File..." ), 0,
                        this, SLOT( slotFileSaveSelectionAsProjectTemplate() ),
                        ac, "save_selection_project_template" );

    KStdAction::print( this, SLOT( slotFilePrint() ), ac );

    KStdAction::quit( this, SLOT( slotFileQuit() ), ac );

   (void) new KAction( i18n( "&List Opened Files..." ), ALT+Key_0, this,
                       SLOT( slotShowOpenFileList() ), ac, "file_list" );

    // Edit actions
 /*
    KAction *undoRedo
      = new KAction( i18n( "Undo/Redo &History..."), 0,
                     doc, SLOT( undoHistory()),
                     ac, "undo_history" );

    undoRedo->setGroup( "edit_undo_merge" ); */

    (void) new KAction( i18n( "Find in Files..." ),
                        UserIcon("find"), CTRL+ALT+Key_F,
                        this, SLOT( slotEditFindInFiles() ),
                        ac, "find_in_files" );

    // Tool actions

    (void) new KAction( i18n( "Context &Help..." ), CTRL+Key_H,
                        this, SLOT( slotContextHelp() ),
                        ac, "context_help" );

    (void) new KAction( i18n( "&Quanta Homepage" ), 0,
                        this, SLOT( slotHelpHomepage() ),
                        ac, "help_homepage" );

    (void) new KAction( i18n( "&User Mailing List" ), 0,
                        this, SLOT( slotHelpUserList() ),
                        ac, "help_userlist" );

    (void) new KAction( i18n( "Make &Donation" ), 0,
                        this, SLOT( slotMakeDonation() ),
                        ac, "help_donation" );

    (void) new KAction( i18n( "Tag &Attributes..." ), ALT+Key_Down,
                        m_doc, SLOT( slotAttribPopup() ),
                        ac, "tag_attributes" );

    (void) new KAction( i18n( "&Change the DTD..." ), 0,
                        this, SLOT( slotChangeDTD() ),
                        ac, "change_dtd" );

    (void) new KAction( i18n( "&Load && Convert DTD..." ), 0,
                        DTDs::ref(), SLOT( slotLoadDTD() ),
                        ac, "load_dtd" );

    (void) new KAction( i18n( "Load DTD &Package (DTEP)..." ), 0,
                        DTDs::ref(), SLOT( slotLoadDTEP() ),
                        ac, "load_dtep" );

    (void) new KAction( i18n( "Send DTD Package (DTEP) in E-&Mail" ), 0,
                        this, SLOT( slotEmailDTEP() ),
                        ac, "send_dtep" );

#ifdef BUILD_KAFKAPART
    (void) new KAction( i18n( "&Document Properties" ), 0,
                        this, SLOT( slotDocumentProperties() ),
                        ac, "tools_document_properties" );
#endif

    (void) new KAction( i18n( "&Convert Tag && Attribute Case..."), 0,
                        this, SLOT(slotConvertCase()),
                        ac, "tools_change_case");

    // View actions

    showFTabAction =
      new KToggleAction( i18n( "Show Files Tree" ), UserIcon("ftab"), 0,
                         this, SLOT( slotShowFTabDock() ),
                         ac, "show_ftab_tree" );
    showPTabAction =
      new KToggleAction( i18n( "Show Project Tree" ), UserIcon("ptab"), 0,
                         this, SLOT( slotShowPTabDock() ),
                         ac, "show_ptab_tree" );
    showTTabAction =
      new KToggleAction( i18n( "Show Templates Tree" ), UserIcon("ttab"), 0,
                         this, SLOT( slotShowTTabDock() ),
                         ac, "show_ttab_tree" );
    showScriptTabAction =
      new KToggleAction( i18n( "Show Scripts Tree" ), SmallIcon("run"), 0,
                         this, SLOT( slotShowScriptTabDock() ),
                         ac, "show_scripttab_tree" );
    showSTabAction =
      new KToggleAction( i18n( "Show Structure Tree" ), BarIcon ("view_sidetree"), 0,
                         this, SLOT( slotShowSTabDock() ),
                         ac, "show_stab_tree" );
    showATabAction =
      new KToggleAction( i18n( "Show Attribute Tree" ), UserIcon ("tag_misc"), 0,
                         this, SLOT( slotShowATabDock() ),
                         ac, "show_atab_tree" );
    showDTabAction =
      new KToggleAction( i18n( "Show Documentation Tree" ), BarIcon ("contents2"), 0,
                         this, SLOT( slotShowDTabDock() ),
                         ac, "show_dtab_tree" );

    showMessagesAction =
      new KToggleAction( i18n( "Show &Messages" ), "output_win", CTRL+Key_M,
                         this, SLOT( slotShowBottDock() ),
                         ac, "show_messages" );
    showProblemsAction =
      new KToggleAction( i18n( "Show &Problem Reporter" ), "stop", 0,
                         this, SLOT( slotShowProblemsDock() ),
                         ac, "show_problems" );

#ifdef BUILD_KAFKAPART
    KToggleAction *ta;
      ta =
      new KToggleAction( i18n( "&Source Editor"), UserIcon ("view_text"), ALT+Key_F9,
                         m_view, SLOT( slotShowQuantaEditor()),
                         ac, "show_quanta_editor");
      ta->setExclusiveGroup("view");

     showKafkaAction =
      new KToggleAction( i18n( "&VPL Editor"), UserIcon ("vpl"), CTRL+SHIFT+Key_F9,
      m_view, SLOT( slotShowKafkaPart() ),
                          ac, "show_kafka_view");
     showKafkaAction->setExclusiveGroup("view");

     ta =
      new KToggleAction( i18n("VPL && So&urce Editors"), UserIcon ("vpl_text"), Key_F9,
                         m_view, SLOT( slotShowKafkaAndQuanta() ),
                          ac, "show_kafka_and_quanta");
     ta->setExclusiveGroup("view");
    /**kafkaSelectAction = new KSelectAction(i18n("Main &View"), 0, ac,"show_kafka");
    QStringList list2;
    list2.append(i18n("&Source editor"));
    list2.append(i18n("&VPL editor (experimental)"));
    list2.append(i18n("&Both editors"));
    kafkaSelectAction->setItems(list2);
    connect(kafkaSelectAction, SIGNAL(activated(int)), this, SLOT(slotShowKafkaPartl(int)));*/
#endif

     showPreviewAction =
      new KToolBarPopupAction( i18n( "Pr&eview" ), "preview", Key_F6,
                         this, SLOT( slotShowPreview() ),
                         ac, "show_preview" );

     showPreviewAction->popupMenu()->insertItem(i18n("Preview Without Frames"), 0);
     connect(showPreviewAction->popupMenu(), SIGNAL(activated(int)),
             this, SLOT(slotShowNoFramesPreview()));

    (void) new KAction( i18n( "&Reload Preview" ), "reload",
                        KStdAccel::shortcut(KStdAccel::Reload).keyCodeQt(),
                        this, SLOT(slotRepaintPreview()),
                        ac, "reload" );

    (void) new KAction( i18n( "View with &Konqueror" ), "konqueror", CTRL+Key_F6,
                        m_view, SLOT( slotViewInKFM() ),
                        ac, "view_with_konqueror" );

    (void) new KAction( i18n( "View with L&ynx" ), "terminal", SHIFT+Key_F6,
                        m_view, SLOT( slotViewInLynx() ),
                        ac, "view_with_lynx" );

    (void) new KAction( i18n( "&Previous File" ), "1leftarrow", KStdAccel::back(),
                        this, SLOT( slotBack() ),
                        ac, "previous_file" );

    (void) new KAction( i18n( "&Next File" ), "1rightarrow", KStdAccel::forward(),
                        this, SLOT( slotForward() ),
                        ac, "next_file" );

    // Project actions
    //
    Project *m_project = Project::ref();
    (void) new KAction( i18n( "&New Project..." ), 0,
                        m_project, SLOT( slotNewProject() ),
                        ac, "project_new" );

    (void) new KAction( i18n( "&Open Project..." ), BarIcon("folder_new"), 0,
                        m_project, SLOT( slotOpenProject() ),
                        ac, "project_open" );
#if KDE_VERSION < KDE_MAKE_VERSION(3,1,92)
    m_project -> projectRecent = new KQRecentFilesAction(i18n("Op&en Recent Project"),
                                 "folder_new", 0,
                                  m_project, SLOT(slotOpenProject(const KURL&)),
                                  ac, "project_open_recent");
#else
    m_project -> projectRecent =
      KStdAction::openRecent(m_project, SLOT(slotOpenProject(const KURL&)),
                             ac, "project_open_recent");
    m_project->projectRecent->setText(i18n("Open Recent Project"));
    m_project->projectRecent->setIcon("folder_new");
#endif
    m_project->projectRecent->setMaxItems(32);
    m_project->projectRecent->setToolTip(i18n("Open / Open Recent Project"));
    connect(m_project->projectRecent, SIGNAL(activated()), m_project, SLOT(slotOpenProject()));


    closeprjAction =  new KAction( i18n( "&Close Project" ), SmallIcon("fileclose"), 0,
                         m_project, SLOT( slotCloseProject() ),
                         ac, "project_close" );


    openPrjViewAction = new KAction( i18n( "Open Project &View..." ), 0,
                        m_project, SLOT( slotOpenProjectView() ),
                        ac, "project_view_open" );

    savePrjViewAction = new KAction( i18n( "&Save Project View" ), 0,
                        m_project, SLOT( slotSaveProjectView() ),
                        ac, "project_view_save" );
    saveAsPrjViewAction = new KAction( i18n( "Save Project View &As..." ), 0,
                        m_project, SLOT( slotSaveAsProjectView() ),
                        ac, "project_view_save_as" );
    deletePrjViewAction = new KAction( i18n( "&Delete Project View..." ), 0,
                        m_project, SLOT( slotDeleteProjectView() ),
                        ac, "project_view_delete" );



    insertFileAction = new KAction( i18n( "&Insert Files..." ), 0,
                        m_project, SLOT( slotAddFiles() ),
                        ac, "project_insert_file" );

    insertDirAction = new KAction( i18n( "Insert &Directory..." ), 0,
                        m_project, SLOT( slotAddDirectory() ),
                        ac, "project_insert_directory" );

    rescanPrjDirAction = new KAction( i18n( "&Rescan Project Folder" ), SmallIcon("reload"), 0,
                        m_project, SLOT( slotRescanPrjDir() ),
                        ac, "project_rescan" );

    uploadProjectAction = new KAction( i18n( "&Upload Project" ), Key_F8,
                        m_project, SLOT( slotUpload() ),
                        ac, "project_upload" );

    projectOptionAction = new KAction( i18n( "&Project Options" ), Key_F7,
                        m_project, SLOT( slotOptions() ),
                        ac, "project_options" );

    // Options actions
    //
    KStdAction::showToolbar  ( this, SLOT( slotViewToolBar() ), ac, "view_toolbar" );
    showStatusbarAction = KStdAction::showStatusbar( this, SLOT( slotViewStatusBar() ), ac, "view_statusbar" );


    (void) new KAction( i18n( "Configure &Actions..." ), UserIcon("ball"),0,
                        this, SLOT( slotOptionsConfigureActions() ),
                        ac, "conf_actions" );

    KStdAction::keyBindings      ( this, SLOT( slotOptionsConfigureKeys() ), ac, "keys_bind" );
    KStdAction::configureToolbars( this, SLOT( slotOptionsConfigureToolbars() ), ac, "conf_toolbars" );
    KStdAction::preferences      ( this, SLOT( slotOptions() ), ac, "general_options" );

    // Toolbars actions
    projectToolbarFiles = new KRecentFilesAction(i18n("Load &Project Toolbar"),0,this, SLOT(slotLoadToolbarFile(const KURL&)),
                           ac, "toolbars_load_project");

    new KAction(i18n("Load &Global Toolbar..."), 0, this, SLOT(slotLoadGlobalToolbar()), ac, "toolbars_load_global");
    new KAction(i18n("Load &Local Toolbar..."), 0, this, SLOT(slotLoadToolbar()), ac, "toolbars_load_user");
    new KAction(i18n("Save as &Local Toolbar..."), 0, this, SLOT(slotSaveLocalToolbar()), ac, "toolbars_save_local");
    new KAction(i18n("Save as &Project Toolbar..."), 0, this, SLOT(slotSaveProjectToolbar()), ac, "toolbars_save_project");
    new KAction(i18n("&Add User Toolbar..."),  0, this, SLOT(slotAddToolbar()), ac, "toolbars_add");
    new KAction(i18n("&Remove User Toolbar..."), 0, this, SLOT(slotRemoveToolbar()), ac, "toolbars_remove");
    new KAction(i18n("Re&name User Toolbar..."), 0, this, SLOT(slotRenameToolbar()), ac, "toolbars_rename");
    new KAction(i18n("Send Toolbar in E-&Mail..."), 0, this, SLOT(slotSendToolbar()), ac, "toolbars_send");

    showDTDToolbar=new KToggleAction(i18n("Show DTD Toolbar"), 0, ac, "view_dtd_toolbar");
    connect(showDTDToolbar, SIGNAL(toggled(bool)), this, SLOT(slotToggleDTDToolbar(bool)));


    new KAction(i18n("Complete Text"), CTRL+Key_Space,
                this, SLOT(slotShowCompletion()), ac,"show_completion");
    new KAction(i18n("Completion Hints"), CTRL+SHIFT+Key_Space,
                this, SLOT(slotShowCompletionHint()), ac,"show_completion_hint");

    KStdAction::back(this, SLOT( slotBack() ), ac, "w_back");
    KStdAction::forward(this, SLOT( slotForward() ), ac, "w_forward");

    new KAction(i18n("Open File: none"), 0, this, SLOT(slotOpenFileUnderCursor()), ac, "open_file_under_cursor");
    new KAction(i18n("Upload..."), 0, this, SLOT(slotUploadFile()), ac, "upload_file");
    new KAction(i18n("Delete File"), 0, this, SLOT(slotDeleteFile()), ac, "delete_file");

    QString ss = i18n("Upload Opened Project Files...");
/*    new KAction(i18n("Upload Opened Project Files"), 0, this, SLOT(slotUploadOpenedFiles()), ac, "upload_opened_files"); */

    QString error;
    int el, ec;
    m_actions = new QDomDocument();
//load the global actions
    QFile f(qConfig.globalDataDir + resourceDir + "actions.rc");
    if ( f.open( IO_ReadOnly ))
    {
      if (m_actions->setContent(&f, &error, &el, &ec))
      {
        QDomElement docElem = m_actions->documentElement();

        QDomNode n = docElem.firstChild();
        while( !n.isNull() ) {
          QDomElement e = n.toElement(); // try to convert the node to an element.
          if( !e.isNull() ) { // the node was really an element.
              new TagAction( &e, ac);
          }
          n = n.nextSibling();
        }
      } else
        kdError(24000) << QString("Error %1 at (%2, %3) in %4").arg(error).arg(el).arg(ec).arg(f.name()) << endl;
      f.close();
    }
    m_actions->clear();
//read the user defined actions
    QString s = locateLocal("appdata","actions.rc");
    if (!s.isEmpty())
    {
      f.setName(s);
      if ( f.open( IO_ReadOnly ))
      {
        if (m_actions->setContent(&f, &error, &el, &ec))
        {
          QDomElement docElem = m_actions->documentElement();

          QDomNode n = docElem.firstChild();
          while( !n.isNull() ) {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            if( !e.isNull())
            { // the node was really an element.
                delete ac->action(e.attribute("name"));
                new TagAction( &e, ac);
            }
            n = n.nextSibling();
          }
        } else
        kdError(24000) << QString("Error %1 at (%2, %3) in %4").arg(error).arg(el).arg(ec).arg(f.name()) << endl;
        f.close();
      }
    } else
    {
      m_actions->setContent(s);
    }

}

/** Initialize the plugin architecture. */
void QuantaApp::initPlugins()
{
  // TODO : read option from plugins.rc to see if we should validate the plugins

  m_pluginMenu = new QPopupMenu(this);
  m_pluginMenu->setCheckable(TRUE);
  connect(m_pluginMenu, SIGNAL(aboutToShow()), this, SLOT(slotBuildPluginMenu()));

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

void QuantaApp::slotPluginsEdit()
{
  QuantaPluginEditor *editor = new QuantaPluginEditor(view(), "plugin_editor");
  editor->setSearchPaths(m_pluginInterface->searchPaths());
  editor->setPlugins(m_pluginInterface->plugins());

  editor->exec();
  m_pluginInterface->setSearchPaths(editor->searchPathList());
  m_pluginInterface->setPlugins(editor->plugins());
  m_pluginInterface->writeConfig();
}

void QuantaApp::slotPluginsValidate()
{
  QDict<QuantaPlugin> plugins = m_pluginInterface->plugins();

  QDictIterator<QuantaPlugin> it(plugins);
  for(;it.current();++it)
  {
    if(!QuantaPlugin::validatePlugin(it.current()))
    {
      int answer = KMessageBox::warningYesNo(m_view, i18n("You have plugins installed that aren't currently valid. Do you want to edit the plugins?"), i18n("Invalid Plugins"));
      if(answer == KMessageBox::Yes)
      {
        slotPluginsEdit();
      }
      return;
    }
  }
  statusBar()->message(i18n("All plugins validated successfully."));
}
void QuantaApp::recoverCrashed(QStringList& recoveredFileNameList)
{

  m_doc->blockSignals(true);
  m_view->writeTab()->blockSignals(true);

  execCommandPS("ps -C quanta -o pid --no-headers");

  m_config->setGroup  ("Projects");
  QString pu = m_config->readPathEntry("Last Project");

  KURL u;
  QuantaCommon::setUrl(u, pu);
  bool isPrj = true;
  if (pu.isEmpty())
     isPrj = false;
  if (!u.isValid())
     isPrj = false;

  m_config->reparseConfiguration();
  m_config->setGroup("General Options");

#if KDE_IS_VERSION(3,1,3)
  QStringList backedUpUrlsList = m_config->readPathListEntry("List of backedup files");
  QStringList autosavedUrlsList = m_config->readPathListEntry("List of autosaved files");
#else
  QStringList backedUpUrlsList = m_config->readListEntry("List of backedup files");
  QStringList autosavedUrlsList = m_config->readListEntry("List of autosaved files");
#endif

  for ( QStringList::Iterator backedUpUrlsIt = backedUpUrlsList.begin();
        backedUpUrlsIt != backedUpUrlsList.end();
      ++backedUpUrlsIt )
  {
   // when quanta crashes and file autoreloading option is on
   // then if user restarts quanta, the backup copies will reload
   QString backedUpFileName = retrieveBaseFileName((*backedUpUrlsIt));
   QString autosavedPath = searchPathListEntry( backedUpFileName, autosavedUrlsList.join(",") );
   if(!autosavedPath.isEmpty())
    {
     KURL originalVersion;
     QuantaCommon::setUrl(originalVersion, backedUpFileName );
     KURL autosavedVersion;
     QuantaCommon::setUrl(autosavedVersion,autosavedPath);
     bool isUntitledDocument = false;
     if (autosavedVersion.path().right(1) == "U")
      isUntitledDocument = true;
     if (!isPrj || originalVersion.isLocalFile())
     {
       KIO::UDSEntry entry;
       KIO::NetAccess::stat(originalVersion, entry);
       KFileItem* item= new KFileItem(entry, originalVersion, false, true);
       QString origTime = item->timeString();
       KIO::filesize_t origSize = item->size();
       delete item;
       KIO::NetAccess::stat(autosavedVersion, entry);
       item= new KFileItem(entry, autosavedVersion, false, true);
       QString backupTime = item->timeString();
       KIO::filesize_t backupSize = item->size();
       delete item;
       if (QFileInfo(autosavedVersion.path()).exists())
       {
          emit showSplash(false);
          DirtyDlg *dlg = new DirtyDlg(autosavedVersion.path(), originalVersion.path(), false, this);
          dlg->setCaption(i18n("Restore File"));
          DirtyDialog *w = static_cast<DirtyDialog*>(dlg->mainWidget());
          w->textLabel->setText(i18n("<qt>A backup copy of a file was found:<br><br>"
           "Original file: <b>%1</b><br>"
           "Original file size: <b>%2</b><br>"
           "Original file last modified on: <b>%3</b><br><br>"
           "Backup file size: <b>%4</b><br>"
           "Backup created on: <b>%5</b><br><br>"
           "</qt>")
           .arg(originalVersion.prettyURL(0, KURL::StripFileProtocol ))
           .arg(KIO::convertSize(origSize)).arg(origTime)
           .arg(KIO::convertSize(backupSize)).arg(backupTime));
          w->buttonLoad->setText(i18n("&Restore the file from backup"));
          w->buttonIgnore->setText(i18n("Do &not restore the file from backup"));
          delete w->warningLabel;
          w->warningLabel = 0L;
          w->setMinimumHeight(320);
          dlg->adjustSize();
          if (KStandardDirs::findExe("kompare").isEmpty() || isUntitledDocument)
          {
            w->buttonCompare->setEnabled(false);
            w->buttonLoad->setChecked(true);
          }
          if (dlg->exec())
          {
            KURL backupURL = originalVersion;
            backupURL.setPath(backupURL.path()+"."+QString::number(getpid(),10)+".backup");
            //TODO: Replace with KIO::NetAccess::file_move, when KDE 3.1 support
            //is dropped
            QExtFileInfo::copy(originalVersion, backupURL, -1, true, false, this);
            //TODO: Replace with KIO::NetAccess::file_copy, when KDE 3.1 support
            //is dropped
            QExtFileInfo::copy(autosavedVersion, originalVersion, -1, true, false, this);
            //we save a list of autosaved file names so "KQApplicationPrivate::init()"
            //can open them. If autosavedVersion.path().right(1) == "U" then we are recovering
            //an untitled document
            if(isUntitledDocument)
             slotFileOpen(autosavedVersion, defaultEncoding());  // load initial files
            else
             recoveredFileNameList += originalVersion.path();
          }
          delete dlg;
       }
     }
     //now we remove the autosaved copiy and clean the quantarc up
     if(QFile::exists(autosavedVersion.path()))
     {
      QFile::remove(autosavedVersion.path());
      m_config->setGroup("General Options");

#if KDE_IS_VERSION(3,1,3)
     QStringList backedupFilesEntryList = m_config->readPathListEntry("List of backedup files");
     QStringList autosavedFilesEntryList = m_config->readPathListEntry("List of autosaved files");
#else
     QStringList backedupFilesEntryList = m_config->readListEntry("List of backedup files");
     QStringList autosavedFilesEntryList = m_config->readListEntry("List of autosaved files");
#endif
      QStringList::Iterator entryIt;

      for ( entryIt = autosavedFilesEntryList.begin();
            entryIt != autosavedFilesEntryList.end(); ++entryIt )
      {
        if ((*entryIt) == autosavedVersion.path())
        {
          entryIt = autosavedFilesEntryList.remove(entryIt);
        }
      }
      m_config->writeEntry("List of autosaved files", autosavedFilesEntryList);

      autosavedUrlsList = autosavedFilesEntryList;

      for ( entryIt = backedupFilesEntryList.begin();
            entryIt != backedupFilesEntryList.end(); ++entryIt )
      {
        if ((*entryIt) == (*backedUpUrlsIt))
          entryIt = backedupFilesEntryList.remove(entryIt);
      }
      m_config->writeEntry("List of backedup files", backedupFilesEntryList);
     }
    }
  }

  m_view->writeTab()->blockSignals(false);
 }
 void QuantaApp::execCommandPS(const QString& cmd)
 {

  //We create a KProcess that executes the "ps" *nix command to get the PIDs of the
  //other instances of quanta actually running
  m_execCommandPS = new KProcess();
  *m_execCommandPS << QStringList::split(" ",cmd);

  connect( m_execCommandPS, SIGNAL(receivedStdout(KProcess*,char*,int)),
           this, SLOT(slotGetScriptOutput(KProcess*,char*,int)));
  connect( m_execCommandPS, SIGNAL(receivedStderr(KProcess*,char*,int)),
           this, SLOT(slotGetScriptError(KProcess*,char*,int)));
  connect( m_execCommandPS, SIGNAL(processExited(KProcess*)),
           this, SLOT(slotProcessExited(KProcess*)));

  //if KProcess fails I think a message box is needed... I will fix it
  if (!m_execCommandPS->start(KProcess::NotifyOnExit,KProcess::All))
    kdError() << "Failed to query for running Quanta instances!" << endl;
    //TODO: Replace the above error with a real messagebox after the message freeze is over
  else
  {
    //To avoid lock-ups, start a timer.
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(slotProcessTimeout()));
    timer->start(180*1000, true);
    QExtFileInfo internalFileInfo;
    m_loopStarted = true;
    internalFileInfo.enter_loop();
    delete timer;
  }
 }



