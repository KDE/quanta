/***************************************************************************
                          quanta.cpp  -  description
                             -------------------
    begin                : ?? ???  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev <pdima@users.sourceforge.net,yshurik@linuxfan.com>
                           (C) 2001-2004 by Andras Mantia <amantia@kde.org>
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
#include <dcopclient.h>
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
#include <kpopupmenu.h>
#include <kprocess.h>
#include <kprogress.h>
#include <kspell.h>
#include <ktip.h>
#include <kio/netaccess.h>
#include <ktabwidget.h>
#include <kmultitabbar.h>

#ifdef BUILD_KAFKAPART
#include "wkafkapart.h"
#include "kafkacommon.h"
#include "undoredo.h"
#include <dom/dom_string.h>
#endif

// application specific includes

#include "quanta_init.h"
#include "quanta.h"
#include "quantaview.h"
#include "viewmanager.h"
#include "quantadoc.h"
#include "resource.h"
#include "document.h"
#include "qextfileinfo.h"

#include "project.h"
#include "debuggermanager.h"

#include "wkafkapart.h"
#include "whtmlpart.h"
#include "messageoutput.h"

#include "tagaction.h"

#include "filestreeview.h"
#include "projecttreeview.h"
#include "doctreeview.h"
#include "structtreeview.h"
#include "templatestreeview.h"
#include "tagattributetree.h"
#include "scripttreeview.h"
#include "toolbartabwidget.h"

#ifdef ENABLE_CVSSERVICE
#include "cvsservice.h"
#endif

#include "quantaplugin.h"
#include "quantaplugininterface.h"


#include "parser.h"
#include "filemasks.h"
#include "dirtydlg.h"
#include "dirtydialog.h"

#include "dtds.h"
#include "spellchecker.h"

QuantaInit::QuantaInit(QuantaApp * quantaApp)
        : QObject()
{
  m_quanta = quantaApp;
  connect(this, SIGNAL(hideSplash()), m_quanta, SLOT(slotHideSplash()));
}

QuantaInit::~QuantaInit()
{
}


/** Delayed initialization. */
void QuantaInit::initQuanta()
{
  m_config = quantaApp->m_config;
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

  initView();
  initDocument();  // after initView because of init of treeViews
  initProject();
  initActions();


  DTDs::ref();  // create the class, must be before readOptions() !
  readOptions();
  initPlugins();  // needs to be before createGUI because some actions are created inside
  readAbbreviations();

  // Initialize debugger
  m_quanta->m_debugger = new DebuggerManager(m_quanta);
  connect(Project::ref(), SIGNAL(newProjectLoaded(const QString &, const KURL &, const KURL &)),
          m_quanta->m_debugger, SLOT(slotNewProjectLoaded(const QString &, const KURL &, const KURL &)));
  connect(m_quanta->m_debugger, SIGNAL(hideSplash()), m_quanta, SLOT(slotHideSplash()));

  m_quanta->m_pluginInterface->readConfig(); //call here, so the plugin actions are created before the GUI

  //m_quanta->KDockMainWindow::createGUI( QString::null, false /* conserveMemory */ );
  m_quanta->createShellGUI(true);

  addToolTreeView(m_quanta->fTab, i18n("Files"), UserIcon("ftab"), KDockWidget::DockLeft);
  addToolTreeView(m_quanta->dTab, i18n("Documentation"), BarIcon("contents"), KDockWidget::DockRight);
  addToolTreeView(m_quanta->aTab, i18n("Attribute Editor"), UserIcon("tag_misc"), KDockWidget::DockRight);
  addToolTreeView(ProjectTreeView::ref(), i18n("Project"), UserIcon("ptab"), KDockWidget::DockLeft);
  addToolTreeView(TemplatesTreeView::ref(), i18n("Templates"), UserIcon("ttab"), KDockWidget::DockLeft);
  addToolTreeView(StructTreeView::ref(), i18n("Document Structure"), BarIcon("view_sidetree"), KDockWidget::DockLeft);
  addToolTreeView(m_quanta->scriptTab, i18n("Scripts"), BarIcon("run"), KDockWidget::DockLeft);
  m_quanta->m_messageOutputView = addToolTreeView(m_quanta->m_messageOutput, i18n("Messages"), SmallIcon("openterm"), KDockWidget::DockBottom);
  m_quanta->m_problemsOutputView = addToolTreeView(m_quanta->m_problemOutput, i18n("Problems"), SmallIcon("info"), KDockWidget::DockBottom);

  m_config->setGroup  ("General Options");
  QString layout = m_config->readEntry("Window layout", "Default");
  int mdiMode = m_config->readNumEntry("MDI mode", -1);
  if (mdiMode != -1 && layout != "Default")
  {
      m_quanta->readDockConfig(m_config); //FIXME: This causes the visible widget construction on startup, but is needed to restore the window layout...
      if (mdiMode != KMdi::IDEAlMode)
        m_quanta->setToolviewStyle(qConfig.toolviewTabs);
  }
  m_quanta->initTabWidget(true);
  qConfig.windowLayout = "Custom";
  //FIXME: This is a hack to workaround the starting problem when we are in Toplevel mode.
  //Without this, the editor becomes the child of the widget holding the menus and toolbars...
  if (mdiMode == KMdi::ToplevelMode)
  {
      m_quanta->switchToChildframeMode();
      QTimer::singleShot(0, m_quanta, SLOT(switchToToplevelMode()));
  }

  //restore shown/hidden state of the toolbars
  m_config->setGroup  ("General Options");
  KToggleAction *showToolbarAction = (KToggleAction *) m_quanta->actionCollection()->action( "view_toolbar" );
  if (!m_config->readBoolEntry("Show Toolbar",true))
  {
    m_quanta->toolBar("mainToolBar")->hide();
    m_quanta->toolBar("mainEditToolBar")->hide();
    m_quanta->toolBar("mainNaviToolBar")->hide();
    m_quanta->toolBar("mainPluginsToolBar")->hide();
    showToolbarAction->setChecked(false);
  } else
  {
    m_quanta->toolBar("mainToolBar")->show();
    m_quanta->toolBar("mainEditToolBar")->show();
    m_quanta->toolBar("mainNaviToolBar")->show();
    m_quanta->toolBar("mainPluginsToolBar")->show();
    showToolbarAction->setChecked(true);
  }

  m_quanta->m_pluginInterface->setPluginMenu(static_cast<QPopupMenu*>(m_quanta->factory()->container("plugins", m_quanta)));
 m_quanta->m_pluginInterface->buildPluginMenu();
//TODO: Remove after upgrade from 3.1 is not supported
  QDomDocument doc;
  doc.setContent(KXMLGUIFactory::readConfigFile(m_quanta->xmlFile(), m_quanta->instance()));
  QDomNodeList nodeList = doc.elementsByTagName("ActionProperties");
  QDomNode node = nodeList.item(0).firstChild();
  while (!node.isNull())
  {
    if (node.nodeName() == "Action")
    {
      QDomElement el = node.toElement();
      m_quanta->oldShortcuts.insert(el.attribute("name"), el.attribute("shortcut"));
      node = node.nextSibling();
      el.parentNode().removeChild(el);
    } else
    {
      node = node.nextSibling();
    }
  }

  //m_quanta->applyMainWindowSettings(m_config);

  m_quanta->m_tagsMenu =   static_cast<QPopupMenu*>(m_quanta->factory()->container("tags", m_quanta));
  KMenuBar *mb = m_quanta->menuBar();
  for (uint i = 0 ; i < mb->count(); i++)
  {
       if (mb->text(mb->idAt(i)) == i18n("&Settings"))
       {
          mb->insertItem(i18n("&Window"), m_quanta->windowMenu(), -1, i);
          break;
       }
  }
  KActionMenu *toolviewMenu = (KActionMenu*)(m_quanta->actionCollection()->action("kmdi_toolview_menu"));
  if (toolviewMenu)
  toolviewMenu->plug(m_quanta->windowMenu());

  QPopupMenu *toolbarsMenu  = (QPopupMenu*)(m_quanta->guiFactory())->container("toolbars_load", m_quanta);
  connect(toolbarsMenu, SIGNAL(aboutToShow()), m_quanta, SLOT(slotBuildPrjToolbarsMenu()));

  QPopupMenu *contextMenu = (QPopupMenu*)(m_quanta->guiFactory())->container("popup_editor", m_quanta);
  connect(contextMenu, SIGNAL(aboutToShow()), m_quanta, SLOT(slotContextMenuAboutToShow()));
  contextMenu->insertSeparator(0);  // XML gui does not support a separator at the beginning

  connect(m_quanta->m_messageOutput, SIGNAL(clicked(const QString&, int, int)),
          m_quanta, SLOT(gotoFileAndLine(const QString&, int, int)));
  connect(m_quanta->m_problemOutput, SIGNAL(clicked(const QString&, int, int)),
          m_quanta, SLOT(gotoFileAndLine(const QString&, int, int)));

  m_quanta->refreshTimer = new QTimer(m_quanta);
  connect(m_quanta->refreshTimer, SIGNAL(timeout()), m_quanta, SLOT(slotReparse()));
  m_quanta->refreshTimer->start( qConfig.refreshFrequency*1000, false ); //update the structure tree every 5 seconds
  if (qConfig.instantUpdate || qConfig.refreshFrequency == 0)
  {
    m_quanta->refreshTimer->stop();
  }

  m_quanta->slotFileNew();
  m_quanta->slotNewStatus();
  initToolBars();
  KTipDialog::showTip(m_quanta);

  //get the PID of this running instance
  qConfig.quantaPID = QString::number(int(getpid()), 10);
  qConfig.backupDirPath = KGlobal::instance()->dirs()->saveLocation("data", resourceDir + "backups/");

  m_quanta->autosaveTimer = new QTimer(m_quanta);
  connect(m_quanta->autosaveTimer, SIGNAL(timeout()), m_quanta, SLOT(slotAutosaveTimer()));
  m_quanta->autosaveTimer->start(qConfig.autosaveInterval * 60000, false);

  connect(m_quanta->m_doc, SIGNAL(hideSplash()), m_quanta, SLOT(slotHideSplash()));
  connect(parser, SIGNAL(rebuildStructureTree()),
          m_quanta, SLOT(slotReloadStructTreeView()));

  // Read list of characters
  QFile file(locate("appdata","chars"));
  if ( file.open(IO_ReadOnly) ) {    // file opened successfully
    QTextStream t( &file );        // use a text stream
    t.setEncoding(QTextStream::UnicodeUTF8);
    QString s;
    while (!t.eof())
      charList << i18n(t.readLine().utf8()); // line excluding '\n'
    file.close();
  }

  QString infoCss = tmpDir;
  infoCss.replace(QRegExp("/quanta$"),"");
  infoCss += "/info.css";
  QExtFileInfo::copy(KURL().fromPathOrURL(qConfig.globalDataDir + resourceDir + "scripts/info.css"), KURL().fromPathOrURL(infoCss));

  checkRuntimeDependencies();
}

void QuantaInit::initToolBars()
{
 if (m_quanta->toolbarList.count() == 0)
     m_quanta->loadToolbarForDTD(Project::ref()->defaultDTD());
}

void QuantaInit::initStatusBar()
{
  m_quanta->statusbarTimer = new QTimer(m_quanta);
  connect(m_quanta->statusbarTimer,SIGNAL(timeout()),
          m_quanta, SLOT(statusBarTimeout()));

  progressBar = new KProgress(m_quanta->statusBar());
  progressBar->show();
  m_quanta->statusBar()->insertItem(i18n(IDS_DEFAULT),IDS_STATUS, 1);
  m_quanta->statusBar()->addWidget(progressBar);
  m_quanta->statusBar()->insertFixedItem(" XXX ",     IDS_INS_OVR  );
  m_quanta->statusBar()->insertFixedItem(" * ",       IDS_MODIFIED );
  m_quanta->statusBar()->insertFixedItem(i18n("Line: 00000 Col: 000"), IDS_STATUS_CLM, true);

  m_quanta->statusBar()->changeItem("", IDS_INS_OVR);
  m_quanta->statusBar()->changeItem("", IDS_MODIFIED);
  m_quanta->statusBar()->changeItem("", IDS_STATUS_CLM);
  m_quanta->statusBar()->setItemAlignment(IDS_STATUS, AlignLeft);
}


void QuantaInit::initDocument()
{
  m_quanta->m_doc = new QuantaDoc(0L);
  connect(m_quanta->m_doc, SIGNAL(newStatus()),
          m_quanta, SLOT(slotNewStatus()));
}

void QuantaInit::initProject()
{
  Project *m_project = Project::ref(m_quanta);

  connect(m_project, SIGNAL(openFile(const KURL &, const QString&)),
          m_quanta, SLOT(slotFileOpen(const KURL &, const QString&)));
  connect(m_project, SIGNAL(closeFile(const KURL &)),
          m_quanta, SLOT(slotFileClose(const KURL &)));
  connect(m_project, SIGNAL(reloadTree(const ProjectUrlList & ,bool)),
          pTab, SLOT(slotReloadTree(const ProjectUrlList &,bool)));
  connect(m_project, SIGNAL(closeFiles()), ViewManager::ref(), SLOT(closeAll()));

  connect(m_quanta->fTab, SIGNAL(insertDirInProject(const KURL&)),
          m_project, SLOT(slotAddDirectory(const KURL&)));

  connect(m_quanta->fTab, SIGNAL(insertFileInProject(const KURL&)),
          m_project, SLOT(slotInsertFile(const KURL&)));

  connect(TemplatesTreeView::ref(), SIGNAL(insertDirInProject(const KURL&)),
          m_project, SLOT(slotAddDirectory(const KURL&)));

  connect(TemplatesTreeView::ref(), SIGNAL(insertFileInProject(const KURL&)),
          m_project, SLOT(slotInsertFile(const KURL&)));

  // inform project if something was renamed
  connect(pTab, SIGNAL(renamed(const KURL&, const KURL&)),
          m_project, SLOT(slotRenamed(const KURL&, const KURL&)));
  connect(m_quanta->fTab, SIGNAL(renamed(const KURL&, const KURL&)),
          m_project, SLOT(slotRenamed(const KURL&, const KURL&)));
  connect(tTab, SIGNAL(renamed(const KURL&, const KURL&)),
          m_project, SLOT(slotRenamed(const KURL&, const KURL&)));

  connect(pTab, SIGNAL(removeFromProject(const KURL&)),
          m_project, SLOT(slotRemove(const KURL&)));
  connect(pTab, SIGNAL(uploadSingleURL(const KURL&)),
          m_project, SLOT(slotUploadURL(const KURL&)));
  connect(pTab, SIGNAL(rescanProjectDir()), m_project, SLOT(slotRescanPrjDir()));
  connect(pTab, SIGNAL(showProjectOptions()), m_project, SLOT(slotOptions()));
  connect(pTab, SIGNAL(uploadProject()), m_project, SLOT(slotUpload()));

  connect(m_project, SIGNAL(enableMessageWidget()),
          m_quanta, SLOT(slotShowMessagesView()));

  connect(m_project, SIGNAL(messages(const QString&)),
          m_quanta->m_messageOutput, SLOT(showMessage(const QString&)));

  connect(m_project, SIGNAL(saveAllFiles()),
          m_quanta, SLOT(slotFileSaveAll()));
  connect(m_project, SIGNAL(newStatus()),
          m_quanta, SLOT(slotNewStatus()));

  connect(m_project, SIGNAL(newProjectLoaded(const QString &, const KURL &, const KURL &)),
          TemplatesTreeView::ref(), SLOT(slotNewProjectLoaded(const QString &, const KURL &, const KURL &)));
  connect(m_project, SIGNAL(newProjectLoaded(const QString &, const KURL &, const KURL &)),
          pTab, SLOT(slotNewProjectLoaded(const QString &, const KURL &, const KURL &)));
  connect(m_project, SIGNAL(newProjectLoaded(const QString &, const KURL &, const KURL &)),
          m_quanta->fTab, SLOT(slotNewProjectLoaded(const QString &, const KURL &, const KURL &)));
  connect(pTab, SIGNAL(changeFileDescription(const KURL&, const QString&)),
          m_project, SLOT(slotFileDescChanged(const KURL&, const QString&)));

  connect(m_project, SIGNAL(hideSplash()), m_quanta, SLOT(slotHideSplash()));

  connect(m_project, SIGNAL(statusMsg(const QString &)),
          m_quanta, SLOT(slotStatusMsg(const QString & )));
}

void QuantaInit::initView()
{
  ViewManager *m_viewManager = ViewManager::ref(m_quanta);
  connect(m_quanta, SIGNAL(viewActivated (KMdiChildView *)), m_viewManager, SLOT(slotViewActivated(KMdiChildView*)));
  connect(m_quanta, SIGNAL(lastChildViewClosed()), m_viewManager, SLOT(slotLastViewClosed()));
//   connect(m_quanta, SIGNAL(viewDeactivated(KMdiChildView *)), m_viewManager, SLOT(slotViewDeactivated(KMdiChildView*)));
#ifdef BUILD_KAFKAPART
   KafkaDocument *m_kafkaDocument = KafkaDocument::ref(0, 0, "KafkaPart");
   m_kafkaDocument->getKafkaWidget()->view()->setMinimumHeight(50);
   loadVPLConfig();
#endif
  (void) ToolbarTabWidget::ref(quantaApp);
  //set the toolview and close button style before the GUI is created
  m_config->setGroup  ("General Options");
  qConfig.toolviewTabs = m_config->readNumEntry("MDI style", KMdi::IconOnly);
  m_quanta->initTabWidget();

  m_quanta->setStandardMDIMenuEnabled(false);
  m_quanta->setManagedDockPositionModeEnabled(true);

  m_quanta->fTab = new FilesTreeView(m_config,  m_quanta, "filestree");
  m_quanta->aTab = new EnhancedTagAttributeTree(m_quanta, "TagAttributes");
  pTab = ProjectTreeView::ref(m_quanta, "Project");
  tTab = TemplatesTreeView::ref(m_quanta, "Templates");  // creates the treeview
  m_quanta->dTab = new DocTreeView(m_quanta, "Docs");
  StructTreeView *sTab = StructTreeView::ref(m_quanta ,"Struct");
  m_quanta->scriptTab = new ScriptTreeView(m_quanta, "Scripts");

  m_quanta->m_messageOutput = new MessageOutput(m_quanta, "Messages");
  m_quanta->m_messageOutput->setFocusPolicy(QWidget::NoFocus);
  m_quanta->m_messageOutput->showMessage(i18n("Message Window..."));

  m_quanta->m_problemOutput = new MessageOutput(m_quanta, "Problems");
  m_quanta->m_problemOutput->setFocusPolicy(QWidget::NoFocus);

  m_quanta->m_htmlPart = new WHTMLPart(m_quanta, "rightHTML");
  m_quanta->m_htmlPart->view()->resize(0, 0);
  m_quanta->m_htmlPart->view()->setIcon(UserIcon("preview"));
  m_quanta->m_htmlPart->view()->setCaption(i18n("Preview"));
  m_quanta->slotNewPart(m_quanta->m_htmlPart, false);
  connect(m_quanta->m_htmlPart, SIGNAL(previewHasFocus(bool)), m_quanta, SLOT(slotPreviewHasFocus(bool)));


  m_quanta->m_htmlPartDoc = new WHTMLPart(m_quanta, "docHTML");
  m_quanta->m_htmlPartDoc->view()->resize(0, 0);
  m_quanta->m_htmlPartDoc->view()->setIcon(SmallIcon("contents"));
  m_quanta->m_htmlPartDoc->view()->setCaption(i18n("Documentation"));
  m_quanta->slotNewPart(m_quanta->m_htmlPartDoc, false);

  connect(m_quanta->fTab, SIGNAL(openFile(const KURL &)),
          m_quanta, SLOT(slotFileOpen(const KURL &)));
  connect(m_quanta->fTab, SIGNAL(openImage(const KURL&)),
          m_quanta, SLOT  (slotImageOpen(const KURL&)));

  connect(pTab, SIGNAL(openFile(const KURL &)),
          m_quanta, SLOT(slotFileOpen(const KURL &)));
  connect(pTab, SIGNAL(openImage  (const KURL&)),
          m_quanta, SLOT(slotImageOpen(const KURL&)));
  connect(pTab, SIGNAL(loadToolbarFile  (const KURL&)),
          m_quanta, SLOT(slotLoadToolbarFile(const KURL&)));

  connect(m_quanta->fTab, SIGNAL(closeFile   (const KURL &)),
          m_quanta, SLOT  (slotFileClose(const KURL &)));
  connect(pTab, SIGNAL(closeFile   (const KURL &)),
          m_quanta, SLOT  (slotFileClose(const KURL &)));
  connect(tTab, SIGNAL(closeFile   (const KURL &)),
          m_quanta, SLOT  (slotFileClose(const KURL &)));

  connect(m_quanta->fTab, SIGNAL(closeFile   (const KURL &)),
          m_quanta, SLOT  (slotFileClose(const KURL &)));
  connect(pTab, SIGNAL(closeFile   (const KURL &)),
          m_quanta, SLOT  (slotFileClose(const KURL &)));
  connect(tTab, SIGNAL(closeFile   (const KURL &)),
          m_quanta, SLOT  (slotFileClose(const KURL &)));

  connect(tTab, SIGNAL(openImage  (const KURL&)),
          m_quanta, SLOT(slotImageOpen(const KURL&)));
  connect(tTab, SIGNAL(openFile(const KURL &)),
          m_quanta, SLOT(slotFileOpen(const KURL &)));
  connect(tTab, SIGNAL(insertFile  (const KURL &)),
          m_quanta, SLOT(slotInsertFile(const KURL &)));
  connect(tTab, SIGNAL(insertTag(const KURL &, DirInfo)),
          m_quanta, SLOT(slotInsertTag(const KURL &, DirInfo)));

  connect(m_quanta->scriptTab, SIGNAL(openFile(const KURL &)),
          m_quanta, SLOT(slotFileOpen(const KURL &)));
  connect(m_quanta->scriptTab, SIGNAL(openFileInPreview(const KURL &)),
          m_quanta, SLOT(slotOpenFileInPreview(const KURL &)));
  connect(m_quanta->scriptTab, SIGNAL(showPreviewWidget(bool)),
          m_quanta, SLOT(slotShowPreviewWidget(bool)));
  connect(m_quanta->scriptTab, SIGNAL(assignActionToScript(const KURL &, const QString&)),
          m_quanta, SLOT(slotAssignActionToScript(const KURL &, const QString&)));


  connect(m_quanta->fTab, SIGNAL(insertTag(const KURL &, DirInfo)),
          m_quanta, SLOT(slotInsertTag(const KURL &, DirInfo)));
  connect(pTab, SIGNAL(insertTag(const KURL &, DirInfo)),
          m_quanta, SLOT(slotInsertTag(const KURL &, DirInfo)));

  connect(m_quanta->fTab, SIGNAL(showPreviewWidget(bool)),
          m_quanta, SLOT(slotShowPreviewWidget(bool)));
  connect(pTab, SIGNAL(showPreviewWidget(bool)),
          m_quanta, SLOT(slotShowPreviewWidget(bool)));

  connect(m_quanta->m_htmlPart, SIGNAL(onURL(const QString&)),
              m_quanta, SLOT(slotStatusMsg(const QString&)));
  connect(m_quanta->m_htmlPartDoc, SIGNAL(onURL(const QString&)),
                 m_quanta, SLOT(slotStatusMsg(const QString&)));

  connect(sTab, SIGNAL(newCursorPosition(int,int)), m_quanta, SLOT(setCursorPosition(int,int)));
  connect(sTab, SIGNAL(selectArea(int,int,int,int)), m_quanta, SLOT( selectArea(int,int,int,int)));
  connect(sTab, SIGNAL(selectTagArea(Node*)), m_quanta, SLOT(slotSelectTagArea(Node*)));
  connect(sTab, SIGNAL(needReparse()), m_quanta, SLOT(slotForceReparse()));
  connect(sTab, SIGNAL(parsingDTDChanged(const QString&)), m_quanta, SLOT(slotParsingDTDChanged(const QString&)));
  connect(sTab, SIGNAL(openFile(const KURL &)),
          m_quanta, SLOT  (slotFileOpen(const KURL &)));
  connect(sTab, SIGNAL(openImage  (const KURL&)),
          m_quanta, SLOT(slotImageOpen(const KURL&)));
  connect(sTab, SIGNAL(showPreviewWidget(bool)),
          m_quanta, SLOT(slotShowPreviewWidget(bool)));
  connect(parser, SIGNAL(nodeTreeChanged()), sTab, SLOT(slotNodeTreeChanged()));

  connect(m_quanta->dTab, SIGNAL(openURL(const QString&)), m_quanta, SLOT(openDoc(const QString&)));

  connect(m_viewManager, SIGNAL(dragInsert(QDropEvent *)), tTab, SLOT(slotDragInsert(QDropEvent *)));

  qConfig.windowLayout = "Default";
}

KMdiToolViewAccessor* QuantaInit::addToolTreeView(QWidget *widget, const QString &name, const QPixmap &icon, KDockWidget::DockPosition position)
{
   widget->setIcon(icon);
   widget->setCaption(name);
   return m_quanta->addToolWindow(widget, position, m_quanta->getMainDockWidget());
}

void QuantaInit::readOptions()
{
  m_config->setGroup("General Options");

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

  StructTreeView::ref()->setFollowCursor( m_config->readBoolEntry("Follow Cursor", true));

  qConfig.previewPosition   = m_config->readEntry("Preview area","Editor");
  qConfig.docPosition   = m_config->readEntry("Documentation area","Tab");

  QSize s(800,580);
  m_quanta->resize( m_config->readSizeEntry("Geometry", &s));
  qConfig.autosaveInterval = m_config->readNumEntry("Autosave interval", 1);

  if (!m_config->readBoolEntry("Show Statusbar", true))
  {
     m_quanta->showStatusbarAction->setChecked(false);
  } else
  {
     m_quanta->showStatusbarAction->setChecked(true);
  }
  m_quanta->slotViewStatusBar();
  qConfig.enableDTDToolbar = m_config->readBoolEntry("Show DTD Toolbar",true);
  m_quanta->showDTDToolbar->setChecked(qConfig.enableDTDToolbar);
  qConfig.showCloseButtons = m_config->readEntry("Close Buttons", "ShowDelayed");
//  m_quanta->initTabWidget(true);
  m_quanta->fileRecent ->loadEntries(m_config);
  qConfig.showHiddenFiles = m_config->readBoolEntry("Show Hidden Files", true);

  m_config->setGroup("Parser options");
  qConfig.showEmptyNodes = m_config->readBoolEntry("Show Empty Nodes", false);
  qConfig.showClosingTags = m_config->readBoolEntry("Show Closing Tags", false);
  qConfig.instantUpdate = m_config->readBoolEntry("Instant Update", false);
  qConfig.refreshFrequency = m_config->readNumEntry("Refresh frequency",5);
  qConfig.expandLevel = m_config->readNumEntry("Expand Level", 4);
  qConfig.showDTDSelectDialog = m_config->readBoolEntry("Show DTD Select Dialog", true);

  m_quanta->m_previewVisible = false;
  m_quanta->m_noFramesPreview = false;
#ifdef BUILD_KAFKAPART
  m_quanta->showKafkaAction->setChecked( false );
#endif

  Project::ref()->readConfig(m_config); // project
  SpellChecker::ref()->readConfig(m_config);
}

void QuantaInit::openLastFiles()
{

  // Reload previously opened files only if setting allows
  m_config->setGroup("General Options");
  if (!m_config->readBoolEntry("Reload Files", true))
    return;

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

  QStringList urls = m_config->readPathListEntry("List of opened files");
  m_quanta->m_doc->blockSignals(true);
  for ( QStringList::Iterator it = urls.begin(); it != urls.end(); ++it )
  {
    KURL fu;
    QuantaCommon::setUrl(fu, *it);

    if (!ViewManager::ref()->isOpened(fu) && (!isPrj || fu.isLocalFile()))
      m_quanta->m_doc->openDocument(fu, QString::null, false);
 //   kapp->eventLoop()->processEvents( QEventLoop::ExcludeUserInput | QEventLoop::ExcludeSocketNotifiers);
  }
  m_config->sync();
  m_quanta->m_doc->blockSignals(false);
  Document *w = ViewManager::ref()->activeDocument();
  if (w) //w==0 might happen on quick close on startup
  {
    m_quanta->setCaption(w->url().prettyURL() );
//    m_quanta->slotUpdateStatus(w);//FIXME:
  }
}

/** Loads the initial project */
void QuantaInit::loadInitialProject(const QString& url)
{
  if(url.isNull())
  {
    // Get config
    KConfig *config = m_quanta->config();
    config->setGroup("General Options");

    // Reload last project if setting is enabled
    if (config->readBoolEntry("Reload Project", true))
      Project::ref()->readLastConfig();
  }
  else
    Project::ref()->slotOpenProject(KURL( url ));
}


void QuantaInit::initActions()
{
    KActionCollection *ac = m_quanta->actionCollection();
    m_quanta->editTagAction = new KAction( i18n( "&Edit Current Tag..." ), CTRL+Key_E,
                        m_quanta, SLOT( slotEditCurrentTag() ),
                        ac, "edit_current_tag" );
    m_quanta->selectTagAreaAction = new KAction( i18n( "&Select Current Tag Area" ), 0,
                        m_quanta, SLOT( slotSelectTagArea() ),
                        ac, "select_tag_area" );
    new KAction( i18n( "E&xpand Abbreviation" ), CTRL+SHIFT+Key_J,
                        m_quanta, SLOT( slotExpandAbbreviation() ),
                        ac, "expand_abbreviation" );

    new KAction(i18n("&Report Bug..."), 0, m_quanta, SLOT(slotReportBug()), ac, "help_reportbug"); //needed, because quanta_be bugs should be reported for quanta

    //Kate actions

//Edit menu
    KStdAction::undo(m_quanta, SLOT(slotUndo()), ac);
    KStdAction::redo(m_quanta, SLOT(slotRedo()), ac);

//Tools menu
    KStdAction::spelling(m_quanta, SLOT(slotSpellcheck()), ac);

//help
   (void) new KAction(i18n("Ti&p of the Day"), "idea", "", m_quanta,
      SLOT(slotHelpTip()), ac, "help_tip");

    // File actions
    //
    KStdAction::openNew( m_quanta, SLOT( slotFileNew()  ), ac);
    KStdAction::open   ( m_quanta, SLOT( slotFileOpen() ), ac, "file_open");
    KStdAction::close  ( m_quanta, SLOT( slotFileClose()), ac);
    (void) new KAction(i18n("Close Other Tabs"), 0, ViewManager::ref(), SLOT(slotCloseOtherTabs()), ac, "close_other_tabs");

    m_quanta->fileRecent =  KStdAction::openRecent(m_quanta, SLOT(slotFileOpenRecent(const KURL&)),
                                         ac, "file_open_recent");
    m_quanta->fileRecent->setMaxItems(32);
    m_quanta->fileRecent->setToolTip(i18n("Open / Open Recent"));
    connect(m_quanta->fileRecent, SIGNAL(activated()), m_quanta, SLOT(slotFileOpen()));

    (void) new KAction( i18n( "Close All" ), 0, m_quanta,
                        SLOT( slotFileCloseAll() ),
                        ac, "file_close_all" );

    m_quanta->saveAction = KStdAction::save(m_quanta, SLOT( slotFileSave() ), ac);

    KStdAction::saveAs( m_quanta, SLOT( slotFileSaveAs() ), ac );

    m_quanta->saveAllAction = new KAction( i18n( "Save All..." ), "save_all", SHIFT+KStdAccel::shortcut(KStdAccel::Save).keyCodeQt(),
                        m_quanta, SLOT( slotFileSaveAll() ),
                        ac, "file_save_all" );
    (void)  new KAction(i18n("Reloa&d"), "reload", CTRL+Key_F5, m_quanta,
                        SLOT(slotFileReload()), ac, "file_reload");
//    (void)  new KAction(i18n("Reload All "), 0, 0, m_quanta,
//                        SLOT(slotFileReloadAll()), ac, "file_reload_all");

    (void) new KAction( i18n( "Save as Local Template..." ), 0,
                        m_quanta, SLOT( slotFileSaveAsLocalTemplate() ),
                        ac, "save_local_template" );

    (void) new KAction( i18n( "Save Selection to Local Template File..." ), 0,
                        m_quanta, SLOT( slotFileSaveSelectionAsLocalTemplate() ),
                        ac, "save_selection_local_template" );

    KStdAction::quit( m_quanta, SLOT( slotFileQuit() ), ac );

    // Edit actions

    (void) new KAction( i18n( "Find in Files..." ),
                        UserIcon("find"), CTRL+ALT+Key_F,
                        m_quanta, SLOT( slotEditFindInFiles() ),
                        ac, "find_in_files" );

    // Tool actions

    (void) new KAction( i18n( "&Context Help..." ), CTRL+Key_H,
                        m_quanta, SLOT( slotContextHelp() ),
                        ac, "context_help" );

    (void) new KAction( i18n( "&Quanta Homepage" ), 0,
                        m_quanta, SLOT( slotHelpHomepage() ),
                        ac, "help_homepage" );

    (void) new KAction( i18n( "&User Mailing List" ), 0,
                        m_quanta, SLOT( slotHelpUserList() ),
                        ac, "help_userlist" );

    (void) new KAction( i18n( "Make &Donation" ), 0,
                        m_quanta, SLOT( slotMakeDonation() ),
                        ac, "help_donation" );

    (void) new KAction( i18n( "Tag &Attributes..." ), ALT+Key_Down,
                        m_quanta->m_doc, SLOT( slotAttribPopup() ),
                        ac, "tag_attributes" );

    (void) new KAction( i18n( "&Change the DTD..." ), 0,
                        m_quanta, SLOT( slotChangeDTD() ),
                        ac, "change_dtd" );

    (void) new KAction( i18n( "&Load && Convert DTD..." ), 0,
                        DTDs::ref(), SLOT( slotLoadDTD() ),
                        ac, "load_dtd" );

    (void) new KAction( i18n( "Load DTD &Package (DTEP)..." ), 0,
                        DTDs::ref(), SLOT( slotLoadDTEP() ),
                        ac, "load_dtep" );

    (void) new KAction( i18n( "Send DTD Package (DTEP) in E&mail..." ), 0,
                        m_quanta, SLOT( slotEmailDTEP() ),
                        ac, "send_dtep" );

#ifdef BUILD_KAFKAPART
    (void) new KAction( i18n( "&Document Properties" ), 0,
                        m_quanta, SLOT( slotDocumentProperties() ),
                        ac, "tools_document_properties" );
#endif

    (void) new KAction( i18n( "&Convert Tag && Attribute Case..."), 0,
                        m_quanta, SLOT(slotConvertCase()),
                        ac, "tools_change_case");

    // View actions

#ifdef BUILD_KAFKAPART
    KToggleAction *ta;
      ta =
      new KToggleAction( i18n( "&Source Editor"), UserIcon ("view_text"), ALT+Key_F9,
                         m_quanta, SLOT( slotShowSourceEditor()),
                         ac, "show_quanta_editor");
      ta->setExclusiveGroup("view");

     m_quanta->showKafkaAction =
      new KToggleAction( i18n( "&VPL Editor"), UserIcon ("vpl"), CTRL+SHIFT+Key_F9,
      m_quanta, SLOT( slotShowVPLOnly() ),
                          ac, "show_kafka_view");
     m_quanta->showKafkaAction->setExclusiveGroup("view");

     ta =
      new KToggleAction( i18n("VPL && So&urce Editors"), UserIcon ("vpl_text"), Key_F9,
                         m_quanta, SLOT( slotShowVPLAndSourceEditor() ),
                          ac, "show_kafka_and_quanta");
     ta->setExclusiveGroup("view");
    /**kafkaSelectAction = new KSelectAction(i18n("Main &View"), 0, ac,"show_kafka");
    QStringList list2;
    list2.append(i18n("&Source editor"));
    list2.append(i18n("&VPL editor (experimental)"));
    list2.append(i18n("&Both editors"));
    kafkaSelectAction->setItems(list2);
    connect(kafkaSelectAction, SIGNAL(activated(int)), m_quanta, SLOT(slotShowKafkaPartl(int)));*/
#endif

     m_quanta->showPreviewAction =
      new KToolBarPopupAction( i18n( "&Preview" ), "preview", Key_F6,
                         m_quanta, SLOT( slotToggleShowPreview() ),
                         ac, "show_preview" );

     m_quanta->showPreviewAction->popupMenu()->insertItem(i18n("Preview Without Frames"), 0);
     connect(m_quanta->showPreviewAction->popupMenu(), SIGNAL(activated(int)),
             m_quanta, SLOT(slotShowNoFramesPreview()));

    (void) new KAction( i18n( "&Reload Preview" ), "reload",
                        KStdAccel::shortcut(KStdAccel::Reload).keyCodeQt(),
                        m_quanta, SLOT(slotRepaintPreview()),
                        ac, "reload" );

    (void) new KAction( i18n( "View with &Konqueror" ), "konqueror", CTRL+Key_F6,
                        m_quanta, SLOT( slotViewInKFM() ),
                        ac, "view_with_konqueror" );

    (void) new KAction( i18n( "View with L&ynx" ), "terminal", SHIFT+Key_F6,
                        m_quanta, SLOT( slotViewInLynx() ),
                        ac, "view_with_lynx" );

    (void) new KAction( i18n( "&Previous File" ), "1leftarrow", KStdAccel::back(),
                        m_quanta, SLOT( slotBack() ),
                        ac, "previous_file" );

    (void) new KAction( i18n( "&Next File" ), "1rightarrow", KStdAccel::forward(),
                        m_quanta, SLOT( slotForward() ),
                        ac, "next_file" );

    // Options actions
    //
    KStdAction::showToolbar(m_quanta, SLOT( slotViewToolBar() ), ac, "view_toolbar");
    m_quanta->showStatusbarAction = KStdAction::showStatusbar( m_quanta, SLOT( slotViewStatusBar() ), ac, "view_statusbar" );


    (void) new KAction( i18n( "Configure &Actions..." ), UserIcon("ball"),0,
                        m_quanta, SLOT( slotOptionsConfigureActions() ),
                        ac, "configure_actions" );

    KStdAction::keyBindings(m_quanta, SLOT( slotOptionsConfigureKeys() ), ac, "configure_shortcuts");
    KStdAction::configureToolbars( m_quanta, SLOT( slotOptionsConfigureToolbars() ), ac, "options_configure_toolbars");
    KStdAction::preferences(m_quanta, SLOT( slotOptions() ), ac, "general_options");
    new KAction(i18n("Configure &Browser Part..."), SmallIcon("konqueror"), 0, m_quanta, SLOT(slotPreviewOptions()), ac, "preview_options");

    // Toolbars actions
    m_quanta->projectToolbarFiles = new KRecentFilesAction(i18n("Load &Project Toolbar"),0,
                                                           m_quanta, SLOT(slotLoadToolbarFile(const KURL&)),
                                                           ac, "toolbars_load_project");

    new KAction(i18n("Load &Global Toolbar..."), 0, m_quanta, SLOT(slotLoadGlobalToolbar()), ac, "toolbars_load_global");
    new KAction(i18n("Load &Local Toolbar..."), 0, m_quanta, SLOT(slotLoadToolbar()), ac, "toolbars_load_user");
    new KAction(i18n("Save as &Local Toolbar..."), 0, m_quanta, SLOT(slotSaveLocalToolbar()), ac, "toolbars_save_local");
    new KAction(i18n("Save as &Project Toolbar..."), 0, m_quanta, SLOT(slotSaveProjectToolbar()), ac, "toolbars_save_project");
    new KAction(i18n("&Add User Toolbar..."),  0, m_quanta, SLOT(slotAddToolbar()), ac, "toolbars_add");
    new KAction(i18n("&Remove User Toolbar..."), 0, m_quanta, SLOT(slotRemoveToolbar()), ac, "toolbars_remove");
    new KAction(i18n("Re&name User Toolbar..."), 0, m_quanta, SLOT(slotRenameToolbar()), ac, "toolbars_rename");
    new KAction(i18n("Send Toolbar in E-&Mail..."), 0, m_quanta, SLOT(slotSendToolbar()), ac, "toolbars_send");

    m_quanta->showDTDToolbar=new KToggleAction(i18n("Show DTD Toolbar"), 0, ac, "view_dtd_toolbar");

    connect(m_quanta->showDTDToolbar, SIGNAL(toggled(bool)), m_quanta, SLOT(slotToggleDTDToolbar(bool)));
#if KDE_IS_VERSION(3,2,90)
    m_quanta->showDTDToolbar->setCheckedState(i18n("Hide DTD Toolbar"));
#endif

    new KAction(i18n("Complete Text"), CTRL+Key_Space,
                m_quanta, SLOT(slotShowCompletion()), ac,"show_completion");
    new KAction(i18n("Completion Hints"), CTRL+SHIFT+Key_Space,
                m_quanta, SLOT(slotShowCompletionHint()), ac,"show_completion_hint");

    KStdAction::back(m_quanta, SLOT( slotBack() ), ac, "w_back");
    KStdAction::forward(m_quanta, SLOT( slotForward() ), ac, "w_forward");

    new KAction(i18n("Open File: none"), 0, m_quanta, SLOT(slotOpenFileUnderCursor()), ac, "open_file_under_cursor");
    new KAction(i18n("Upload..."), 0, m_quanta, SLOT(slotUploadFile()), ac, "upload_file");
    new KAction(i18n("Delete File"), 0, m_quanta, SLOT(slotDeleteFile()), ac, "delete_file");
    new KAction(i18n("Add Watch"), SmallIcon("math_brace"), 0, m_quanta, SLOT(slotDebuggerAddWatch()), ac, "debug_addwatch");

    QString ss = i18n("Upload Opened Project Files...");
/*    new KAction(i18n("Upload Opened Project Files"), 0, m_quanta, SLOT(slotUploadOpenedFiles()), ac, "upload_opened_files"); */

    QString error;
    int el, ec;
    m_quanta->m_actions = new QDomDocument();
//load the global actions
    QFile f(qConfig.globalDataDir + resourceDir + "actions.rc");
    if ( f.open( IO_ReadOnly ))
    {
      if (m_quanta->m_actions->setContent(&f, &error, &el, &ec))
      {
        QDomElement docElem = m_quanta->m_actions->documentElement();

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
    m_quanta->m_actions->clear();
//read the user defined actions
    QString s = locateLocal("appdata","actions.rc");
    if (!s.isEmpty())
    {
      f.setName(s);
      if ( f.open( IO_ReadOnly ))
      {
        if (m_quanta->m_actions->setContent(&f, &error, &el, &ec))
        {
          QDomElement docElem = m_quanta->m_actions->documentElement();

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
      m_quanta->m_actions->setContent(s);
    }

    (void) new KAction( i18n( "Table Editor..." ), "quick_table", 0,
                        m_quanta, SLOT( slotTagEditTable() ),
                        ac, "tag_edit_table" );

    (void) new KAction( i18n( "Quick List..." ), "quick_list", 0,
                        m_quanta, SLOT( slotTagQuickList() ),
                        ac, "tag_quick_list" );

    (void) new KAction( i18n( "Color..." ), "color", CTRL+SHIFT+Key_C,
                        m_quanta, SLOT( slotTagColor() ),
                        ac, "tag_color" );


    (void) new KAction( i18n( "Email..." ), "tag_mail", 0,
                        m_quanta, SLOT( slotTagMail() ),
                        ac, "tag_mail" );

    (void) new KAction( i18n( "Misc. Tag..." ), "tag_misc", CTRL+SHIFT+Key_T,
                        m_quanta, SLOT( slotTagMisc() ),
                        ac, "tag_misc" );

    (void) new KAction( i18n( "Frame Wizard..." ), "frame", 0,
                        m_quanta, SLOT( slotFrameWizard() ),
                        ac, "tag_frame_wizard" );

    (void) new KAction( i18n( "Paste &HTML Quoted" ), "editpaste", 0,
                        m_quanta, SLOT( slotPasteHTMLQuoted() ),
                        ac, "edit_paste_html_quoted" );

    (void) new KAction( i18n( "Paste &URL Encoded" ), "editpaste", 0,
                        m_quanta, SLOT( slotPasteURLEncoded() ),
                        ac, "edit_paste_url_encoded" );

    (void) new KAction( i18n( "Insert CSS..." ),"css", 0,
                        m_quanta, SLOT( slotInsertCSS() ),
                        ac, "insert_css" );

    // special-character combo
    KAction* char_action = new KAction(
                        i18n( "Insert Special Character" ), "charset", 0,
                        ac, "insert_char" );
    connect( char_action, SIGNAL(activated()),
             m_quanta, SLOT(slotInsertChar()) );
}

/** Initialize the plugin architecture. */
void QuantaInit::initPlugins()
{
  // TODO : read option from plugins.rc to see if we should validate the plugins

  m_quanta->m_pluginInterface = QuantaPluginInterface::ref(m_quanta);

  connect(m_quanta->m_pluginInterface, SIGNAL(hideSplash()),
          m_quanta, SLOT(slotHideSplash()));
  connect(m_quanta->m_pluginInterface, SIGNAL(statusMsg(const QString &)),
          m_quanta, SLOT(slotStatusMsg(const QString & )));
}


void QuantaInit::recoverCrashed(QStringList& recoveredFileNameList)
{
  m_quanta->m_doc->blockSignals(true);

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

  QStringList backedUpUrlsList = m_config->readPathListEntry("List of backedup files");
  QStringList autosavedUrlsList = m_config->readPathListEntry("List of autosaved files");

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
       KIO::NetAccess::stat(originalVersion, entry, m_quanta);
       KFileItem* item= new KFileItem(entry, originalVersion, false, true);
       QString origTime = item->timeString();
       KIO::filesize_t origSize = item->size();
       delete item;
       KIO::NetAccess::stat(autosavedVersion, entry, m_quanta);
       item= new KFileItem(entry, autosavedVersion, false, true);
       QString backupTime = item->timeString();
       KIO::filesize_t backupSize = item->size();
       delete item;
       if (QFileInfo(autosavedVersion.path()).exists())
       {
          emit hideSplash();
          DirtyDlg *dlg = new DirtyDlg(autosavedVersion.path(), originalVersion.path(), false, m_quanta);
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
            QExtFileInfo::copy(originalVersion, backupURL, -1, true, false, m_quanta);
            QExtFileInfo::copy(autosavedVersion, originalVersion, -1, true, false, m_quanta);
            //we save a list of autosaved file names so "KQApplicationPrivate::init()"
            //can open them. If autosavedVersion.path().right(1) == "U" then we are recovering
            //an untitled document
            if(isUntitledDocument)
             m_quanta->slotFileOpen(autosavedVersion,
                                    m_quanta->defaultEncoding());  // load initial files
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

     QStringList backedupFilesEntryList = m_config->readPathListEntry("List of backedup files");
     QStringList autosavedFilesEntryList = m_config->readPathListEntry("List of autosaved files");
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

 }
 void QuantaInit::execCommandPS(const QString& cmd)
 {

  //We create a KProcess that executes the "ps" *nix command to get the PIDs of the
  //other instances of quanta actually running
  m_quanta->m_execCommandPS = new KProcess();
  *(m_quanta->m_execCommandPS) << QStringList::split(" ",cmd);

  connect( m_quanta->m_execCommandPS, SIGNAL(receivedStdout(KProcess*,char*,int)),
           m_quanta, SLOT(slotGetScriptOutput(KProcess*,char*,int)));
  connect( m_quanta->m_execCommandPS, SIGNAL(receivedStderr(KProcess*,char*,int)),
           m_quanta, SLOT(slotGetScriptError(KProcess*,char*,int)));
  connect( m_quanta->m_execCommandPS, SIGNAL(processExited(KProcess*)),
           m_quanta, SLOT(slotProcessExited(KProcess*)));

  //if KProcess fails I think a message box is needed... I will fix it
  if (!m_quanta->m_execCommandPS->start(KProcess::NotifyOnExit,KProcess::All))
    kdError() << "Failed to query for running Quanta instances!" << endl;
    //TODO: Replace the above error with a real messagebox after the message freeze is over
  else
  {
    //To avoid lock-ups, start a timer.
    QTimer *timer = new QTimer(m_quanta);
    connect(timer, SIGNAL(timeout()),
            m_quanta, SLOT(slotProcessTimeout()));
    timer->start(180*1000, true);
    QExtFileInfo internalFileInfo;
    m_quanta->m_loopStarted = true;
    internalFileInfo.enter_loop();
    delete timer;
  }
 }


QString QuantaInit::searchPathListEntry(const QString& backedUpUrl,const QString& autosavedUrls)
{
  KURL k(backedUpUrl);
  QStringList autosavedUrlsList = QStringList::split(",", autosavedUrls);
  QStringList::Iterator autosavedUrlsIt;
  for (autosavedUrlsIt = autosavedUrlsList.begin();
       autosavedUrlsIt != autosavedUrlsList.end();
       ++autosavedUrlsIt)
  {
   QString quPID = retrievePID((*autosavedUrlsIt));

   QStringList PIDlist = QStringList::split("\n", m_quanta->m_scriptOutput);

   bool isOrphan = true;
   QStringList::Iterator PIDIt;
   for ( PIDIt = PIDlist.begin(); PIDIt != PIDlist.end(); ++PIDIt )
   {
    if ((*PIDIt) == quPID && qConfig.quantaPID != quPID)
    {
     isOrphan = false;
     break;
    }
   }
   if (isOrphan || ((*autosavedUrlsIt).right(1) == "U"))
   {
    if(retrieveHashedPath(Document::hashFilePath(k.path())) == retrieveHashedPath((*autosavedUrlsIt)))
      return (*autosavedUrlsIt);
   }
  }
  return QString::null;
}

/** Retrieves hashed path from the name of a backup file */
QString QuantaInit::retrieveHashedPath(const QString& filename)
{
 return filename.mid(filename.findRev(".") + 1,
                      filename.findRev("P") - 1 - filename.findRev("."));
}


/** Retrieves PID from the name of a backup file */
QString QuantaInit::retrievePID(const QString& filename)
{
 QString strPID = QString::null;
 strPID = filename.right(filename.length() - filename.findRev("P") - 1);

 if (strPID.isEmpty())
  strPID = filename.right(filename.length() - filename.findRev("N") - 1);

 return strPID;
}

/** Retrieves the non hashed part of the name of a backup file */
QString QuantaInit::retrieveBaseFileName(const QString& filename)
{
 return filename.left(filename.findRev("."));
}

void QuantaInit::loadVPLConfig()
{
#ifdef BUILD_KAFKAPART
  //load the VPL options
  m_config->setGroup("Kafka Synchronization options");
  qConfig.quantaRefreshOnFocus = (m_config->readEntry("Source refresh", "delay") == "focus");
  qConfig.quantaRefreshDelay = m_config->readNumEntry("Source refresh delay", 500);
  qConfig.kafkaRefreshOnFocus = (m_config->readEntry("Kafka refresh", "focus") == "focus");
  qConfig.kafkaRefreshDelay = m_config->readNumEntry("Kafka refresh delay", 4000);
  /**reloadUpdateTimers();*/
#endif
}

void QuantaInit::checkRuntimeDependencies()
{
  QString stdErrorMsg = i18n("<br><b>-    %1</b> [<i>%2</i>] - %3 will not be available;");
  QString errorStr;
  if (KStandardDirs::findExe("kmdr-executor").isNull())
    errorStr += QString(stdErrorMsg).arg(i18n("Kommander")).arg("http://kommander.kdewebdev.org").arg(i18n("various script based dialogs including the Quick Start dialog"));
  if (KStandardDirs::findExe("tidy").isNull())
    errorStr += QString(stdErrorMsg).arg("Tidy").arg("http://tidy.sourceforge.net").arg(i18n("HTML syntax checking"));
  if (KStandardDirs::findExe("kcmshell").isNull())
    errorStr += QString(stdErrorMsg).arg("KControl (kdebase)").arg("http://www.kde.org").arg(i18n("preview browser configuration"));
  if (!QuantaPlugin::validatePlugin(m_quanta->m_pluginInterface->plugin("KFileReplace")))
    errorStr += QString(stdErrorMsg).arg("KFileReplace").arg("http://kfilereplace.kdewebdev.org").arg(i18n("search and replace in files"));
  if (!QuantaPlugin::validatePlugin(m_quanta->m_pluginInterface->plugin("CVS Management (Cervisia)")))
    errorStr += QString(stdErrorMsg).arg("Cervisia").arg("http://www.kde.org/apps/cervisia").arg(i18n("CVS management plugin"));

#ifdef ENABLE_CVSSERVICE
  QString error;
  QCString appId;

  KApplication::startServiceByDesktopName("cvsservice", QStringList(), &error,
                                            &appId);
  if (appId.isEmpty())
  {
     errorStr += QString(stdErrorMsg).arg("Cervisia (cvsservice)").arg("http://www.kde.org/apps/cervisia").arg(i18n("integrated CVS management"));
  } else
  {
     CVSService::ref(m_quanta->actionCollection())->setAppId(appId);
     connect(CVSService::ref(), SIGNAL(clearMessages()), m_quanta->m_messageOutput, SLOT(clear()));
     connect(CVSService::ref(), SIGNAL(showMessage(const QString&, bool)), m_quanta->m_messageOutput, SLOT(showMessage(const QString&, bool)));
     m_quanta->fTab->plugCVSMenu();
     pTab->plugCVSMenu();
  }
#endif

  if (!errorStr.isEmpty())
  {
      m_quanta->slotHideSplash();
      errorStr[errorStr.length() - 1] = '.';
      KMessageBox::information(m_quanta, "<qt>" + i18n("Some applications required for full functionality are missing:<br>") + errorStr + "<br><br>You may download the applications from the specified locations.</qt>", i18n("Missing applications"), "RuntimeDependencyCheck");
  }
}

void QuantaInit::readAbbreviations()
{
  QDomDocument doc;
  QString groupName;
  bool mainAbbrevFileFound = false;
  QStringList mainFileList;
  mainFileList << qConfig.globalDataDir + resourceDir + "abbreviations.xml";
  mainFileList << KGlobal::dirs()->saveLocation("data") + resourceDir + "abbreviations.xml";
  for (uint i = 0; i < mainFileList.count(); i++)
  {
     if (!QFile::exists(mainFileList[i]))
       continue;
     QFile file(mainFileList[i]);
     if (file.open(IO_ReadOnly))
     {
        if (doc.setContent(&file))
        {
          QDomNodeList groupList = doc.elementsByTagName("Group");
          for (uint groupIdx = 0; groupIdx < groupList.count(); groupIdx++)
          {
             Abbreviation abbrev;
             QDomElement el = groupList.item(groupIdx).toElement();
             groupName = el.attribute("name");
             QDomNodeList dtepList = el.elementsByTagName("DTEP");
             for (uint dtepListIdx = 0; dtepListIdx < dtepList.count(); dtepListIdx++)
             {
               abbrev.dteps.append(dtepList.item(dtepListIdx).toElement().attribute("name"));
             }
             QDomNodeList nodeList = el.elementsByTagName("Template");
             for (uint nodeIdx = 0; nodeIdx < nodeList.count(); nodeIdx++)
             {
               QDomElement e = nodeList.item(nodeIdx).toElement();
               abbrev.abbreviations.insert(e.attribute("name")+" "+e.attribute("description"), e.attribute("code"));
             }
             qConfig.abbreviations.insert(groupName, abbrev);
          }
        }
        mainAbbrevFileFound = true;
        file.close();
     }
  }
  if (mainAbbrevFileFound) return;
//Compatibility code: read the abbreviations files from the DTEP directories
//TODO: Remove when upgrade from 3.2 is not supported.
 QStringList filenameList = DTDs::ref()->fileNameList(false);
 QStringList::Iterator it;
 for (it = filenameList.begin(); it != filenameList.end(); ++it)
 {
    int pos =(*it).find('|');
    QString dirName = (*it).mid(pos + 1);
    QString dtepName = (*it).left(pos);
    KURL dirURL(dirName);
    dirURL.setFileName("");
    dirName = dirURL.path(1);
    QString abbrevFile = dirName;
    QString tmpStr = dirName;
    QStringList resourceDirs = KGlobal::dirs()->resourceDirs("data");
    bool dirFound = false;
    for (uint i = 0; i < resourceDirs.count(); i++)
    {
      if (tmpStr.startsWith(resourceDirs[i]))
      {
        dirFound = true;
        tmpStr = tmpStr.right(tmpStr.length() - resourceDirs[i].length());
        break;
      }
    }
    if (dirFound)
    {
      abbrevFile = KGlobal::dirs()->saveLocation("data", tmpStr) +"/";
    }
    abbrevFile.append("abbreviations");
    if (!QFile::exists(abbrevFile))
        abbrevFile = dirName + "abbreviations";
    QFile f(abbrevFile);
    if (f.open(IO_ReadOnly))
    {
      if (doc.setContent(&f))
      {
        Abbreviation abbrev;
        QDomNodeList nodeList = doc.elementsByTagName("Template");
        for (uint i = 0; i < nodeList.count(); i++)
        {
          QDomElement e = nodeList.item(i).toElement();
          abbrev.abbreviations.insert(e.attribute("name")+" "+e.attribute("description"), e.attribute("code"));
        }
        abbrev.dteps.append(dtepName);
        qConfig.abbreviations.insert(DTDs::ref()->getDTDNickNameFromName(dtepName), abbrev);
      }
      f.close();
    }
 }
}

#include "quanta_init.moc"
