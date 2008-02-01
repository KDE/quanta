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
#include <ktip.h>
#include <kio/netaccess.h>
#include <ktabwidget.h>
#include <kmultitabbar.h>
#include <ktexteditor/view.h>

#include "wkafkapart.h"
#include "kafkacommon.h"
#include "undoredo.h"
#include <dom/dom_string.h>

// application specific includes

#include "quanta_init.h"
#include "quanta.h"
#include "viewmanager.h"
#include "quantadoc.h"
#include "resource.h"
#include "document.h"
#include "qextfileinfo.h"

#include "project.h"
#include "debuggermanager.h"

#include "wkafkapart.h"
#include "whtmlpart.h"
#include "annotationoutput.h"
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
#include "qpevents.h"
#include "quantabookmarks.h"

#include "tagactionmanager.h"
#include "tagactionset.h"

extern QMap<int, QString> replacementMap;

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
  DTDs::ref()->find("dtd"); //load on startup

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
  connect(Project::ref(), SIGNAL(eventHappened(const QString &, const QString &, const QString &)),
          m_quanta->m_debugger, SLOT(slotHandleEvent(const QString &, const QString &, const QString &)));
  connect(m_quanta->m_debugger, SIGNAL(hideSplash()), m_quanta, SLOT(slotHideSplash()));

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
  m_quanta->m_annotationOutputView = addToolTreeView(m_quanta->m_annotationOutput, i18n("Annotations"), SmallIcon("stamp"), KDockWidget::DockBottom);

  // Restore the dock layout
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

  // Always hide debugger toolbar at this point
  m_quanta->toolBar("debugger_toolbar")->hide();

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

  m_quanta->applyMainWindowSettings(m_config);

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

  connect(m_quanta->m_messageOutput, SIGNAL(clicked(const QString&, int, int)),
          m_quanta, SLOT(gotoFileAndLine(const QString&, int, int)));
  connect(m_quanta->m_problemOutput, SIGNAL(clicked(const QString&, int, int)),
          m_quanta, SLOT(gotoFileAndLine(const QString&, int, int)));
  connect(m_quanta->m_annotationOutput->currentFileAnnotations(), SIGNAL(clicked(const QString&, int, int)),
          m_quanta, SLOT(gotoFileAndLine(const QString&, int, int)));
  connect(m_quanta->m_annotationOutput, SIGNAL(clicked(const QString&, int, int)),
          m_quanta, SLOT(gotoFileAndLine(const QString&, int, int)));

  m_quanta->slotFileNew();
  m_quanta->slotNewStatus();
  initToolBars();
  Project::ref()->setProjectToolbarVisible(m_quanta->factory()->container("project_toolbar", m_quanta)->isShown());
  Project::ref()->slotShowProjectToolbar(Project::ref()->hasProject());
  KTipDialog::showTip(m_quanta);

  //get the PID of this running instance
  qConfig.quantaPID = QString::number(int(getpid()), 10);
  qConfig.backupDirPath = KGlobal::instance()->dirs()->saveLocation("data", resourceDir + "backups/");

  m_quanta->autosaveTimer = new QTimer(m_quanta);
  connect(m_quanta->autosaveTimer, SIGNAL(timeout()), m_quanta, SLOT(slotAutosaveTimer()));
  m_quanta->autosaveTimer->start(qConfig.autosaveInterval * 60000, false);

  connect(m_quanta->m_doc, SIGNAL(hideSplash()), m_quanta, SLOT(slotHideSplash()));
  connect(parser, SIGNAL(rebuildStructureTree(bool)),
          m_quanta, SLOT(slotReloadStructTreeView(bool)));

  // Read list of characters
  QFile file(locate("appdata","chars"));
  if ( file.open(IO_ReadOnly) ) {    // file opened successfully
    QTextStream t( &file );        // use a text stream
    t.setEncoding(QTextStream::UnicodeUTF8);
    QString s;
    while (!t.eof())
    {
      s = t.readLine();
      charList << i18n(s.utf8()); // line excluding '\n'
      int begin = s.find("(&") + 1;
      if (begin == 1)
          continue;
      int length = s.find(";)") - begin + 1;
      QString s2 = s.mid(begin, length - 1);
      replacementMap[s[0].unicode()] = s2;
    }
    file.close();
  }

  QString infoCss = tmpDir;
  infoCss.replace(QRegExp("/quanta$"),"");
  infoCss += "/info.css";
  QExtFileInfo::copy(KURL().fromPathOrURL(qConfig.globalDataDir + resourceDir + "scripts/info.css"), KURL().fromPathOrURL(infoCss));

  checkRuntimeDependencies();
  ViewManager::ref()->activeDocument()->view()->setFocus();

  m_quanta->refreshTimer = new QTimer(m_quanta);
  connect(m_quanta->refreshTimer, SIGNAL(timeout()), m_quanta, SLOT(slotReparse()));
  m_quanta->refreshTimer->start( qConfig.refreshFrequency*1000, false ); //update the structure tree every 5 seconds
  if (qConfig.instantUpdate || qConfig.refreshFrequency == 0)
  {
    m_quanta->refreshTimer->stop();
  }

}

void QuantaInit::initToolBars()
{
 if (m_quanta->m_toolbarList.count() == 0)
   m_quanta->slotLoadToolbarForDTD(Project::ref()->defaultDTD());
}

void QuantaInit::initStatusBar()
{
  m_quanta->statusbarTimer = new QTimer(m_quanta);
  connect(m_quanta->statusbarTimer,SIGNAL(timeout()),
          m_quanta, SLOT(statusBarTimeout()));

  progressBar = new KProgress(m_quanta->statusBar());
  progressBar->setTextEnabled(false);
  progressBar->setMaximumHeight(progressBar->fontMetrics().height());
  progressBar->show();
  m_quanta->statusBar()->insertItem(i18n(IDS_DEFAULT),IDS_STATUS, 1);
  m_quanta->statusBar()->addWidget(progressBar);
  m_quanta->statusBar()->insertItem("",               IDS_INS_OVR  );
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

  connect(m_project, SIGNAL(getTreeStatus(QStringList *)),
          pTab, SLOT(slotGetTreeStatus(QStringList *)));
  connect(m_project, SIGNAL(loadToolbarFile(const KURL &)),
          m_quanta, SLOT(slotLoadToolbarFile(const KURL &)));
  connect(m_project, SIGNAL(getUserToolbarFiles(KURL::List *)),
          m_quanta, SLOT(slotGetUserToolbarFiles(KURL::List *)));
  connect(m_project, SIGNAL(openFiles(const KURL::List &, const QString&)),
          m_quanta, SLOT(slotFileOpen(const KURL::List &, const QString&)));
  connect(m_project, SIGNAL(openFile(const KURL &, const QString&)),
          m_quanta, SLOT(slotFileOpen(const KURL &, const QString&)));
  connect(m_project, SIGNAL(closeFile(const KURL &)),
          m_quanta, SLOT(slotFileClose(const KURL &)));
  connect(m_project, SIGNAL(reloadTree(ProjectList *, bool, const QStringList &)),
          pTab, SLOT(slotReloadTree(ProjectList *, bool, const QStringList &)));
  connect(m_project, SIGNAL(closeFiles()), ViewManager::ref(), SLOT(closeAll()));
  connect(m_project, SIGNAL(eventHappened(const QString&, const QString&, const QString& )), QPEvents::ref(m_quanta), SLOT(slotEventHappened(const QString&, const QString&, const QString& )));

  connect(m_quanta->fTab, SIGNAL(insertDirInProject(const KURL&)),
          m_project, SLOT(slotAddDirectory(const KURL&)));

  connect(m_quanta->fTab, SIGNAL(insertFileInProject(const KURL&)),
          m_project, SLOT(slotInsertFile(const KURL&)));

  connect(TemplatesTreeView::ref(), SIGNAL(insertDirInProject(const KURL&)),
          m_project, SLOT(slotAddDirectory(const KURL&)));

  connect(TemplatesTreeView::ref(), SIGNAL(insertFileInProject(const KURL&)),
          m_project, SLOT(slotInsertFile(const KURL&)));
  connect(TemplatesTreeView::ref(), SIGNAL(downloadTemplate()),
          m_quanta, SLOT(slotDownloadTemplate()));
  connect(TemplatesTreeView::ref(), SIGNAL(uploadTemplate(const QString&)), m_quanta, SLOT(slotUploadTemplate(const QString&)));

  // inform project if something was renamed
  connect(pTab, SIGNAL(renamed(const KURL&, const KURL&)),
          m_project, SLOT(slotRenamed(const KURL&, const KURL&)));
  connect(m_quanta->fTab, SIGNAL(renamed(const KURL&, const KURL&)),
          m_project, SLOT(slotRenamed(const KURL&, const KURL&)));
  connect(tTab, SIGNAL(renamed(const KURL&, const KURL&)),
          m_project, SLOT(slotRenamed(const KURL&, const KURL&)));

  connect(pTab, SIGNAL(insertToProject(const KURL&)),
          m_project, SLOT(slotInsertFile(const KURL&)));
  connect(pTab, SIGNAL(removeFromProject(const KURL&)),
          m_project, SLOT(slotRemove(const KURL&)));
  connect(pTab, SIGNAL(uploadSingleURL(const KURL&, const QString&, bool, bool)),
          m_project, SLOT(slotUploadURL(const KURL&, const QString&, bool, bool)));
  connect(pTab, SIGNAL(rescanProjectDir()), m_project, SLOT(slotRescanPrjDir()));
  connect(pTab, SIGNAL(showProjectOptions()), m_project, SLOT(slotOptions()));
  connect(pTab, SIGNAL(uploadProject()), m_project, SLOT(slotUpload()));

  connect(m_quanta->dTab, SIGNAL(reloadProjectDocs()), m_project, SLOT(slotReloadProjectDocs()));
  connect(m_project, SIGNAL(reloadProjectDocs()), m_quanta->dTab, SLOT(slotReloadProjectDocs()));
  connect(m_project, SIGNAL(addProjectDoc(const KURL&)), m_quanta->dTab, SLOT(slotAddProjectDoc(const KURL&)));

  connect(m_project, SIGNAL(enableMessageWidget()),
          m_quanta, SLOT(slotShowMessagesView()));

  connect(m_project, SIGNAL(messages(const QString&)),
          m_quanta->m_messageOutput, SLOT(showMessage(const QString&)));

  connect(m_project, SIGNAL(newStatus()),
          m_quanta, SLOT(slotNewStatus()));

  connect(m_project, SIGNAL(newProjectLoaded(const QString &, const KURL &, const KURL &)),
          TemplatesTreeView::ref(), SLOT(slotNewProjectLoaded(const QString &, const KURL &, const KURL &)));
  connect(m_project, SIGNAL(newProjectLoaded(const QString &, const KURL &, const KURL &)),
          pTab, SLOT(slotNewProjectLoaded(const QString &, const KURL &, const KURL &)));
  connect(m_project, SIGNAL(newProjectLoaded(const QString &, const KURL &, const KURL &)),
          m_quanta->fTab, SLOT(slotNewProjectLoaded(const QString &, const KURL &, const KURL &)));
  connect(m_project, SIGNAL(newProjectLoaded(const QString &, const KURL &, const KURL &)),
          m_quanta->annotationOutput(), SLOT(updateAnnotations()));

  connect(pTab, SIGNAL(changeFileDescription(const KURL&, const QString&)),
          m_project, SLOT(slotFileDescChanged(const KURL&, const QString&)));
  connect(pTab, SIGNAL(changeUploadStatus(const KURL&, int)),
          m_project, SLOT(slotUploadStatusChanged(const KURL&, int)));
  connect(pTab, SIGNAL(changeDocumentFolderStatus(const KURL&, bool)),
          m_project, SLOT(slotChangeDocumentFolderStatus(const KURL&, bool)));

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
  KafkaDocument *m_kafkaDocument = KafkaDocument::ref(0, 0, "KafkaPart");
  m_kafkaDocument->getKafkaWidget()->view()->setMinimumHeight(50);
  m_kafkaDocument->readConfig(quantaApp->config());
  loadVPLConfig();
  ToolbarTabWidget *toolBarTab = ToolbarTabWidget::ref(quantaApp);
  connect(toolBarTab, SIGNAL(iconTextModeChanged()), quantaApp, SLOT(slotRefreshActiveWindow()));

  //set the toolview and close button style before the GUI is created
  m_config->setGroup("General Options");
  int iconTextMode = m_config->readNumEntry("IconTextMode", KToolBar::IconOnly);
  toolBarTab->setIconText(KToolBar::IconText(iconTextMode));
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
  connect(m_quanta, SIGNAL(showMessage(const QString&, bool)), m_quanta->m_messageOutput, SLOT(showMessage(const QString&, bool)));
  connect(m_quanta, SIGNAL(clearMessages()), m_quanta->m_messageOutput, SLOT(clear()));

  m_quanta->m_problemOutput = new MessageOutput(m_quanta, "Problems");
  m_quanta->m_problemOutput->setFocusPolicy(QWidget::NoFocus);
  m_quanta->m_annotationOutput = new AnnotationOutput(m_quanta, "Annotations");
  m_quanta->m_annotationOutput->setFocusPolicy(QWidget::NoFocus);

  m_quanta->createPreviewPart();
  m_quanta->createDocPart();

  connect(m_quanta, SIGNAL(reloadAllTrees()),
          m_quanta->fTab, SLOT(slotReloadAllTrees()));

  connect(pTab, SIGNAL(loadToolbarFile  (const KURL&)),
          m_quanta, SLOT(slotLoadToolbarFile(const KURL&)));
  connect(m_viewManager, SIGNAL(viewActivated(const KURL&)),
          pTab, SLOT(slotViewActivated(const KURL&)));

  connect(m_viewManager, SIGNAL(documentClosed(const KURL&)),
          pTab, SLOT(slotDocumentClosed(const KURL&)));
  connect(m_viewManager, SIGNAL(documentClosed(const KURL&)),
          tTab, SLOT(slotDocumentClosed(const KURL&)));
  connect(m_viewManager, SIGNAL(documentClosed(const KURL&)),
          m_quanta->scriptTab, SLOT(slotDocumentClosed(const KURL&)));
  connect(m_viewManager, SIGNAL(documentClosed(const KURL&)),
          m_quanta->fTab, SLOT(slotDocumentClosed(const KURL&)));

  connect(tTab, SIGNAL(insertFile  (const KURL &)),
          m_quanta, SLOT(slotInsertFile(const KURL &)));

  connect(m_quanta->scriptTab, SIGNAL(openFileInPreview(const KURL &)),
          m_quanta, SLOT(slotOpenFileInPreview(const KURL &)));
  connect(m_quanta->scriptTab, SIGNAL(showPreviewWidget(bool)),
          m_quanta, SLOT(slotShowPreviewWidget(bool)));
  connect(m_quanta->scriptTab, SIGNAL(assignActionToScript(const KURL &, const QString&)),
          m_quanta, SLOT(slotAssignActionToScript(const KURL &, const QString&)));
  connect(m_quanta->scriptTab, SIGNAL(downloadScript()), m_quanta, SLOT(slotDownloadScript()));
  connect(m_quanta->scriptTab, SIGNAL(uploadScript(const QString&)), m_quanta, SLOT(slotUploadScript(const QString&)));
  connect(m_quanta->dTab, SIGNAL(downloadDoc()), m_quanta, SLOT(slotDownloadDoc()));

  connect(m_quanta->m_htmlPart, SIGNAL(onURL(const QString&)),
              m_quanta, SLOT(slotStatusMsg(const QString&)));
  connect(m_quanta->m_htmlPartDoc, SIGNAL(onURL(const QString&)),
                 m_quanta, SLOT(slotStatusMsg(const QString&)));

  connect(sTab, SIGNAL(newCursorPosition(int,int)), m_quanta, SLOT(setCursorPosition(int,int)));
  connect(sTab, SIGNAL(selectArea(int,int,int,int)), m_quanta, SLOT( selectArea(int,int,int,int)));
  connect(sTab, SIGNAL(selectTagArea(Node*)), m_quanta, SLOT(slotSelectTagArea(Node*)));
  connect(sTab, SIGNAL(needReparse()), m_quanta, SLOT(slotForceReparse()));
  connect(sTab, SIGNAL(showGroupsForDTEP(const QString&, bool)), m_quanta, SLOT(slotShowGroupsForDTEP(const QString&, bool)));
  connect(sTab, SIGNAL(openFile(const KURL &)),
          m_quanta, SLOT  (slotFileOpen(const KURL &)));
  connect(sTab, SIGNAL(openImage  (const KURL&)),
          m_quanta, SLOT(slotImageOpen(const KURL&)));
  connect(sTab, SIGNAL(showProblemMessage(const QString&)),
          m_quanta->m_problemOutput, SLOT(showMessage(const QString&)));
  connect(sTab, SIGNAL(clearProblemOutput()),
          m_quanta->m_problemOutput, SLOT(clear()));
  connect(parser, SIGNAL(nodeTreeChanged()), sTab, SLOT(slotNodeTreeChanged()));

  connect(m_quanta->dTab, SIGNAL(openURL(const QString&)), m_quanta, SLOT(openDoc(const QString&)));

  connect(m_viewManager, SIGNAL(dragInsert(QDropEvent *)), tTab, SLOT(slotDragInsert(QDropEvent *)));

  qConfig.windowLayout = "Default";
}

KMdiToolViewAccessor* QuantaInit::addToolTreeView(QWidget *widget, const QString &name, const QPixmap &icon, KDockWidget::DockPosition position)
{
  widget->setIcon(icon);
  widget->setCaption(name);
  return m_quanta->addToolWindow(widget, m_quanta->prevDockPosition(widget, position), m_quanta->getMainDockWidget());
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
  qConfig.replaceAccented = m_config->readBoolEntry("Replace Accented Chars", false);
  qConfig.replaceNotInEncoding = m_config->readBoolEntry("Replace Chars Not In Current Encoding", true);


  qConfig.defaultEncoding = m_config->readEntry("Default encoding", "UTF8");

  StructTreeView::ref()->setFollowCursor( m_config->readBoolEntry("Follow Cursor", true));

  qConfig.previewPosition   = m_config->readEntry("Preview area","Editor");
  qConfig.docPosition   = m_config->readEntry("Documentation area","Tab");

  qConfig.smartTagInsertion = m_config->readBoolEntry("Smart Tag Insertion", false);
  KAction *action = quantaApp->actionCollection()->action("smart_tag_insertion");
  (static_cast<KToggleAction* >(action))->setChecked(qConfig.smartTagInsertion);

  QSize s(800,580);
  m_quanta->resize( m_config->readSizeEntry("Geometry", &s));
  qConfig.autosaveInterval = m_config->readNumEntry("Autosave interval", 1);

  qConfig.enableDTDToolbar = m_config->readBoolEntry("Show DTD Toolbar",true);
  m_quanta->showDTDToolbar->setChecked(qConfig.enableDTDToolbar);
  qConfig.showCloseButtons = m_config->readEntry("Close Buttons", "ShowDelayed");
//  m_quanta->initTabWidget(true);
  m_quanta->fileRecent ->loadEntries(m_config);
  qConfig.showHiddenFiles = m_config->readBoolEntry("Show Hidden Files", true);
  qConfig.saveTrees = m_config->readBoolEntry("Save Local Trees", true);
  int maxRecentItems = m_config->readNumEntry("Recent Files Limit", 32);
  m_quanta->fileRecent->setMaxItems(maxRecentItems);

  m_config->setGroup("Parser options");
  qConfig.showEmptyNodes = m_config->readBoolEntry("Show Empty Nodes", false);
  qConfig.showClosingTags = m_config->readBoolEntry("Show Closing Tags", false);
  qConfig.instantUpdate = m_config->readBoolEntry("Instant Update", false);
  qConfig.refreshFrequency = m_config->readNumEntry("Refresh frequency",5);
  qConfig.expandLevel = m_config->readNumEntry("Expand Level", 4);
  qConfig.showDTDSelectDialog = m_config->readBoolEntry("Show DTD Select Dialog", true);

  m_quanta->m_previewVisible = false;
  m_quanta->m_noFramesPreview = false;

  m_quanta->showVPLAction->setChecked( false );


//KNewStuff config
   m_config->setGroup("KNewStuff");
   QString str = m_config->readEntry("ProvidersUrl");
   if (str.isEmpty())
   {
      m_config->writeEntry( "ProvidersUrl", "http://quanta.kdewebdev.org/newstuff/providers.xml" );
      m_config->sync();
   }


  Project::ref()->readConfig(m_config); // project
}

void QuantaInit::openLastFiles()
{

  // Reload previously opened files only if setting allows
  m_config->setGroup("General Options");
  if (!m_config->readBoolEntry("Reload Files", true))
  {
    m_quanta->setParserEnabled(true);
    m_quanta->reparse(true);
    return;
  }
  // we need to check config
  // because project now can be
  // in load stage ( remote prj )
  m_config->setGroup("Projects");
  QString pu = QuantaCommon::readPathEntry(m_config, "Last Project");

  KURL u;
  QuantaCommon::setUrl(u, pu);
  bool isPrj = true;
  if (pu.isEmpty())
     isPrj = false;
  if (!u.isValid())
     isPrj = false;

  m_config->setGroup("General Options");

  QStringList urls = QuantaCommon::readPathListEntry(m_config, "List of opened files");
  QStringList encodings = QuantaCommon::readPathListEntry(m_config, "Encoding of opened files");
  m_quanta->m_doc->blockSignals(true);
  m_quanta->setParserEnabled(false);
  uint i = 0;
  for ( QStringList::Iterator it = urls.begin(); it != urls.end(); ++it )
  {
    KURL fu;
    QuantaCommon::setUrl(fu, *it);

    if (!ViewManager::ref()->isOpened(fu) && (!isPrj || fu.isLocalFile()))
      m_quanta->slotFileOpen(fu, encodings[i]);
    i++;
  }
  m_config->sync();
  m_quanta->m_doc->blockSignals(false);
  m_quanta->setParserEnabled(true);
  m_quanta->reparse(true);
  Document *w = ViewManager::ref()->activeDocument();
  if (w) //w==0 might happen on quick close on startup
  {
    m_quanta->setTitle(w->url().prettyURL(0, KURL::StripFileProtocol) );
//    m_quanta->slotUpdateStatus(w);//FIXME:
  }
}

/** Loads the initial project */
void QuantaInit::loadInitialProject(const QString& url)
{
  if(url.isNull())
  {
    if(runningQuantas() == 1)
    {
      // Get config
      KConfig *config = m_quanta->config();
      config->setGroup("General Options");

      // Reload last project if setting is enabled
      Project::ref()->loadLastProject(config->readBoolEntry("Reload Project", true));
    }
  }
  else
    Project::ref()->slotOpenProject(KURL( url ));
}


void QuantaInit::initActions()
{
    KActionCollection *ac = m_quanta->actionCollection();
    new KAction(i18n("Annotate..."), 0, m_quanta, SLOT(slotAnnotate()),ac, "annotate");
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
    KStdAction::cut(m_quanta, SLOT(slotCut()), ac);
    KStdAction::copy(m_quanta, SLOT(slotCopy()), ac) ;
    KStdAction::pasteText(m_quanta, SLOT(slotPaste()), ac);

//help
   (void) new KAction(i18n("Ti&p of the Day"), "idea", "", m_quanta,
      SLOT(slotHelpTip()), ac, "help_tip");

    // File actions
    //
    KStdAction::openNew( m_quanta, SLOT( slotFileNew()  ), ac);
    KStdAction::open   ( m_quanta, SLOT( slotFileOpen() ), ac, "file_open");
    (void) new KAction(i18n("Close Other Tabs"), 0, ViewManager::ref(), SLOT(slotCloseOtherTabs()), ac, "close_other_tabs");

    m_quanta->fileRecent =  KStdAction::openRecent(m_quanta, SLOT(slotFileOpenRecent(const KURL&)),
                                         ac, "file_open_recent");
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
    (void)  new KAction(i18n("Reloa&d"), "revert", SHIFT+Key_F5, m_quanta,
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
                        SmallIcon("filefind"), CTRL+ALT+Key_F,
                        m_quanta, SLOT( slotEditFindInFiles() ),
                        ac, "find_in_files" );

    KAction* aux = TagActionManager::self()->actionCollection()->action("apply_source_indentation");
    aux->setEnabled(false);
    ac->insert(aux);

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

    (void) new KAction( i18n( "&Edit DTD Settings..." ), 0,
    m_quanta, SLOT( slotEditDTD() ),
    ac, "edit_dtd" );

    (void) new KAction( i18n( "&Load && Convert DTD..." ), 0,
                        DTDs::ref(), SLOT( slotLoadDTD() ),
                        ac, "load_dtd" );
    (void) new KAction( i18n( "Load DTD E&ntities..." ), 0,
                        DTDs::ref(), SLOT( slotLoadEntities() ),
                        ac, "load_entities" );

    (void) new KAction( i18n( "Load DTD &Package (DTEP)..." ), 0,
                        m_quanta, SLOT( slotLoadDTEP() ),
                        ac, "load_dtep" );

    (void) new KAction( i18n( "Send DTD Package (DTEP) in E&mail..." ), "mail_send", 0,
                        m_quanta, SLOT( slotEmailDTEP() ),
                        ac, "send_dtep" );

    (void) new KAction( i18n( "&Download DTD Package (DTEP)..." ), "network", 0,
                        m_quanta, SLOT( slotDownloadDTEP() ),
                        ac, "download_dtep" );
    (void) new KAction( i18n( "&Upload DTD Package (DTEP)..." ), "network", 0,
                        m_quanta, SLOT( slotUploadDTEP() ),
                        ac, "upload_dtep" );
/*
    (void) new KAction( i18n( "&Upload DTD Package (DTEP)..." ), 0,
                        m_quanta, SLOT( slotUploadDTEP() ),
                        ac, "send_dtep" );
*/
    (void) new KAction( i18n( "&Document Properties" ), 0,
                        m_quanta, SLOT( slotDocumentProperties() ),
                        ac, "tools_document_properties" );

    (void) new KAction ( i18n ("F&ormat XML Code"), 0,
                        m_quanta, SLOT( slotCodeFormatting() ),
                        ac, "tools_code_formatting");

    (void) new KAction( i18n( "&Convert Tag && Attribute Case..."), 0,
                        m_quanta, SLOT(slotConvertCase()),
                        ac, "tools_change_case");

    // View actions


    m_quanta->showSourceAction =
      new KToggleAction( i18n( "&Source Editor"), UserIcon ("view_text"), ALT+Key_F9,
                         m_quanta, SLOT( slotShowSourceEditor()),
                         ac, "show_quanta_editor");
    m_quanta->showSourceAction->setExclusiveGroup("view");

    m_quanta->showVPLAction =
      new KToggleAction( i18n( "&VPL Editor"), UserIcon ("vpl"), CTRL+SHIFT+Key_F9,
      m_quanta, SLOT( slotShowVPLOnly() ),
                          ac, "show_kafka_view");
    m_quanta->showVPLAction->setExclusiveGroup("view");

    m_quanta->showVPLSourceAction =
      new KToggleAction( i18n("VPL && So&urce Editors"), UserIcon ("vpl_text"), Key_F9,
                         m_quanta, SLOT( slotShowVPLAndSourceEditor() ),
                          ac, "show_kafka_and_quanta");
    m_quanta->showVPLSourceAction->setExclusiveGroup("view");
    /**kafkaSelectAction = new KSelectAction(i18n("Main &View"), 0, ac,"show_kafka");
    QStringList list2;
    list2.append(i18n("&Source Editor"));
    list2.append(i18n("&VPL Editor (experimental)"));
    list2.append(i18n("&Both Editors"));
    kafkaSelectAction->setItems(list2);
    connect(kafkaSelectAction, SIGNAL(activated(int)), m_quanta, SLOT(slotShowKafkaPartl(int)));*/


    (void) new KAction( i18n( "&Reload Preview" ), "reload",
                        KStdAccel::shortcut(KStdAccel::Reload).keyCodeQt(),
                        m_quanta, SLOT(slotRepaintPreview()),
                        ac, "reload" );

    (void) new KAction( i18n( "&Previous File" ), "1leftarrow", KStdAccel::back(),
                        m_quanta, SLOT( slotBack() ),
                        ac, "previous_file" );

    (void) new KAction( i18n( "&Next File" ), "1rightarrow", KStdAccel::forward(),
                        m_quanta, SLOT( slotForward() ),
                        ac, "next_file" );

    // Options actions
    //

    (void) new KAction( i18n( "Configure &Actions..." ), UserIcon("ball"),0,
                        m_quanta, SLOT( slotOptionsConfigureActions() ),
                        ac, "configure_actions" );

    KStdAction::showMenubar(m_quanta, SLOT(slotShowMenuBar()), ac, "options_show_menubar");
    KStdAction::keyBindings(m_quanta, SLOT( slotOptionsConfigureKeys() ), ac, "configure_shortcuts");
    KStdAction::configureToolbars( m_quanta, SLOT( slotOptionsConfigureToolbars() ), ac, "options_configure_toolbars");
    KStdAction::preferences(m_quanta, SLOT( slotOptions() ), ac, "general_options");
    new KAction(i18n("Configure Pre&view..."), SmallIcon("konqueror"), 0, m_quanta, SLOT(slotPreviewOptions()), ac, "preview_options");

    // Toolbars actions
    m_quanta->projectToolbarFiles = new KRecentFilesAction(i18n("Load &Project Toolbar"),0,
                                                           m_quanta, SLOT(slotLoadToolbarFile(const KURL&)),
                                                           ac, "toolbars_load_project");

    new KAction(i18n("Load &Global Toolbar..."), 0, m_quanta, SLOT(slotLoadGlobalToolbar()), ac, "toolbars_load_global");
    new KAction(i18n("Load &Local Toolbar..."), 0, m_quanta, SLOT(slotLoadToolbar()), ac, "toolbars_load_user");
    new KAction(i18n("Save as &Local Toolbar..."), 0, m_quanta, SLOT(slotSaveLocalToolbar()), ac, "toolbars_save_local");
    new KAction(i18n("Save as &Project Toolbar..."), 0, m_quanta, SLOT(slotSaveProjectToolbar()), ac, "toolbars_save_project");
    new KAction(i18n("&New User Toolbar..."),  0, m_quanta, SLOT(slotAddToolbar()), ac, "toolbars_add");
    new KAction(i18n("&Remove User Toolbar..."), 0, m_quanta, SLOT(slotRemoveToolbar()), ac, "toolbars_remove");
    new KAction(i18n("Re&name User Toolbar..."), 0, m_quanta, SLOT(slotRenameToolbar()), ac, "toolbars_rename");
    new KAction(i18n("Send Toolbar in E&mail..."), "mail_send", 0, m_quanta, SLOT(slotSendToolbar()), ac, "toolbars_send");
    new KAction(i18n("&Upload Toolbar..." ), "network", 0, m_quanta, SLOT(slotUploadToolbar()), ac, "toolbars_upload" );
    new KAction(i18n("&Download Toolbar..." ), "network", 0, m_quanta, SLOT(slotDownloadToolbar()), ac, "toolbars_download" );

    KToggleAction *toggle = new KToggleAction( i18n("Smart Tag Insertion"), 0, ac, "smart_tag_insertion");
    connect(toggle, SIGNAL(toggled(bool)), m_quanta, SLOT(slotSmartTagInsertion()));

    m_quanta->showDTDToolbar=new KToggleAction(i18n("Show DTD Toolbar"), 0, ac, "view_dtd_toolbar");

    connect(m_quanta->showDTDToolbar, SIGNAL(toggled(bool)), m_quanta, SLOT(slotToggleDTDToolbar(bool)));
    m_quanta->showDTDToolbar->setCheckedState(i18n("Hide DTD Toolbar"));

    new KAction(i18n("Complete Text"), CTRL+Key_Space,
                m_quanta, SLOT(slotShowCompletion()), ac,"show_completion");
    new KAction(i18n("Completion Hints"), CTRL+SHIFT+Key_Space,
                m_quanta, SLOT(slotShowCompletionHint()), ac,"show_completion_hint");

    KStdAction::back(m_quanta, SLOT( slotBack() ), ac, "w_back");
    KStdAction::forward(m_quanta, SLOT( slotForward() ), ac, "w_forward");

    new KAction(i18n("Open File: none"), 0, m_quanta, SLOT(slotOpenFileUnderCursor()), ac, "open_file_under_cursor");
    new KAction(i18n("Upload..."), 0, m_quanta, SLOT(slotUploadFile()), ac, "upload_file");
    new KAction(i18n("Delete File"), 0, m_quanta, SLOT(slotDeleteFile()), ac, "delete_file");

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
              bool toggable = (e.attribute("toggable", "") == "true");
              new TagAction(&e, m_quanta, toggable);
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
                bool toggable = (e.attribute("toggable", "") == "true");
                new TagAction(&e, m_quanta, toggable);
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

    // create the preview action
    m_quanta->showPreviewAction =
      new KToolBarPopupAction( i18n( "&Preview" ), "preview", Key_F6,
                         m_quanta, SLOT( slotToggleShowPreview() ),
                         ac, "show_preview" );

    KAction *act = new KAction( i18n( "Preview Without Frames" ), "", 0,
                                  m_quanta, SLOT(slotShowNoFramesPreview()),
                                  ac, "show_preview_no_frames" );
    act->plug(m_quanta->showPreviewAction->popupMenu());

    act = new KAction( i18n( "View with &Konqueror" ), "konqueror", Key_F12,
                        m_quanta, SLOT( slotViewInKFM() ),
                        ac, "view_with_konqueror" );
    act->plug(m_quanta->showPreviewAction->popupMenu());


    act = ac->action("view_with_firefox");
    if (act)
      act->plug(m_quanta->showPreviewAction->popupMenu());

    act = ac->action("view_with_mozilla");
    if (act)
      act->plug(m_quanta->showPreviewAction->popupMenu());

    act = ac->action("view_with_netscape");
    if (act)
      act->plug(m_quanta->showPreviewAction->popupMenu());

    act = ac->action("view_with_opera");
    if (act)
      act->plug(m_quanta->showPreviewAction->popupMenu());

    act = new KAction( i18n( "View with L&ynx" ), "terminal", 0,
                        m_quanta, SLOT( slotViewInLynx() ),
                        ac, "view_with_lynx" );
    act->plug(m_quanta->showPreviewAction->popupMenu());


    (void) new KAction( i18n( "Table Editor..." ), "quick_table", 0,
                        m_quanta, SLOT( slotTagEditTable() ),
                        ac, "tag_edit_table" );

    (void) new KAction( i18n( "Quick List..." ), "quick_list", 0,
                        m_quanta, SLOT( slotTagQuickList() ),
                        ac, "tag_quick_list" );

    (void) new KAction( i18n( "Color..." ), "colorize", CTRL+SHIFT+Key_C,
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

    connect(m_quanta, SIGNAL(eventHappened(const QString&, const QString&, const QString& )), QPEvents::ref(m_quanta), SLOT(slotEventHappened(const QString&, const QString&, const QString& )));
    connect(m_quanta->doc(), SIGNAL(eventHappened(const QString&, const QString&, const QString& )), QPEvents::ref(m_quanta), SLOT(slotEventHappened(const QString&, const QString&, const QString& )));
    connect(ViewManager::ref(), SIGNAL(eventHappened(const QString&, const QString&, const QString& )), QPEvents::ref(m_quanta), SLOT(slotEventHappened(const QString&, const QString&, const QString& )));

    QuantaBookmarks *m_bookmarks = new QuantaBookmarks(ViewManager::ref(m_quanta));
    m_bookmarks->createActions(ac);
    connect(m_bookmarks, SIGNAL(gotoFileAndLine(const QString&, int, int)), m_quanta, SLOT(gotoFileAndLine(const QString&, int, int)));
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

  m_quanta->m_pluginInterface->readConfig();
  if (!m_quanta->m_pluginInterface->pluginAvailable("KFileReplace"))
  {
    delete m_quanta->actionCollection()->action("find_in_files");
  }
}


void QuantaInit::recoverCrashed(QStringList& recoveredFileNameList)
{
  m_quanta->m_doc->blockSignals(true);

  execCommandPS("ps -C quanta -C quanta_be -o pid --no-headers");
  m_PIDlist = QStringList::split("\n", m_quanta->m_scriptOutput);

  m_config->setGroup("Projects");
  QString pu = QuantaCommon::readPathEntry(m_config, "Last Project");

  KURL u;
  QuantaCommon::setUrl(u, pu);
  bool isPrj = true;
  if (pu.isEmpty())
     isPrj = false;
  if (!u.isValid())
     isPrj = false;

  m_config->reparseConfiguration();
  m_config->setGroup("General Options");

  QStringList backedUpUrlsList = QuantaCommon::readPathListEntry(m_config, "List of backedup files");
  QStringList autosavedUrlsList = QuantaCommon::readPathListEntry(m_config, "List of autosaved files");

  QStringList::ConstIterator backedUpUrlsEndIt = backedUpUrlsList.constEnd();
  for (QStringList::ConstIterator backedUpUrlsIt = backedUpUrlsList.constBegin();
       backedUpUrlsIt != backedUpUrlsEndIt; ++backedUpUrlsIt )
  {
    // when quanta crashes and file autoreloading option is on
    // then if user restarts quanta, the backup copies will reload
    QString backedUpFileName = (*backedUpUrlsIt).left((*backedUpUrlsIt).findRev(".")); //the filename without the PID
    bool notFound;
    QString autosavedPath = searchPathListEntry(backedUpFileName, autosavedUrlsList, notFound);
    if (!autosavedPath.isEmpty()) //the current item was autosaved and is not in use by another Quanta
    {
      KURL originalVersion;
      KURL autosavedVersion;
      QuantaCommon::setUrl(originalVersion, backedUpFileName);
      QuantaCommon::setUrl(autosavedVersion, autosavedPath);
      bool isUntitledDocument = false;
      if (autosavedVersion.path().right(1) == "U")
        isUntitledDocument = true;
      if (!isPrj || originalVersion.isLocalFile())
      {
        //find some information about local files
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

        if (QFileInfo(autosavedPath).exists()) //if the backup file exists
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
            //backup the current version and restore it from the autosaved backup
            KURL backupURL = originalVersion;
            backupURL.setPath(backupURL.path() + "." + QString::number(getpid(),10) + ".backup");
            QExtFileInfo::copy(originalVersion, backupURL, -1, true, false, m_quanta);
            QExtFileInfo::copy(autosavedVersion, originalVersion, -1, true, false, m_quanta);
            //we save a list of autosaved file names so "KQApplicationPrivate::init()"
            //can open them. If autosavedVersion.path().right(1) == "U" then we are recovering
            //an untitled document
            if(isUntitledDocument)
              m_quanta->slotFileOpen(autosavedVersion,
                                    m_quanta->defaultEncoding());  // load initial files
            else
              recoveredFileNameList += backedUpFileName;
          }
          delete dlg;
          QFile::remove(autosavedPath); //we don't need the backup anymore
       }
      }
      //remove the auto-backup file from the list
      m_config->setGroup("General Options");
      QStringList autosavedFilesEntryList = QuantaCommon::readPathListEntry(m_config, "List of autosaved files");
      QStringList::Iterator entryIt = autosavedFilesEntryList.begin();
      while(entryIt != autosavedFilesEntryList.end())
      {
        if ((*entryIt) == KURL::fromPathOrURL(autosavedPath).url())
          entryIt = autosavedFilesEntryList.remove(entryIt);
        else
          ++entryIt;
      }
      m_config->writePathEntry("List of autosaved files", autosavedFilesEntryList);

      autosavedUrlsList = autosavedFilesEntryList;
    }

    if (notFound)
    {
      //remove processed items
      m_config->setGroup("General Options");

      QStringList backedupFilesEntryList = QuantaCommon::readPathListEntry(m_config, "List of backedup files");
      QStringList::Iterator entryIt = backedupFilesEntryList.begin();
      while (entryIt != backedupFilesEntryList.end())
      {
        if ((*entryIt) == (*backedUpUrlsIt))
          entryIt = backedupFilesEntryList.remove(entryIt);
        else
          ++entryIt;
      }
      m_config->writePathEntry("List of backedup files", backedupFilesEntryList);
    }
  }

  //clean up auto-backup list, just in case of an old Quanta was used before
  QStringList::Iterator entryIt = autosavedUrlsList.begin();
  while (entryIt != autosavedUrlsList.end())
  {
    QString quPID = retrievePID((*entryIt));

    //check if the file is opened by another running Quanta or not
    bool isOrphan = true;
    QStringList::ConstIterator PIDEndIt = m_PIDlist.constEnd();
    for (QStringList::ConstIterator PIDIt = m_PIDlist.constBegin(); PIDIt != PIDEndIt; ++PIDIt )
    {
      if ((*PIDIt) == quPID && qConfig.quantaPID != quPID)
      {
        isOrphan = false; //the file is opened
        break;
      }
    }
    if (isOrphan)
      entryIt = autosavedUrlsList.remove(entryIt);
    else
      ++entryIt;
  }
  m_config->writePathEntry("List of autosaved files", autosavedUrlsList);



}

 void QuantaInit::execCommandPS(const QString& cmd)
 {

  //We create a KProcess that executes the "ps" *nix command to get the PIDs of the
  //other instances of quanta actually running
  KProcess *execCommand = new KProcess();
  *(execCommand) << QStringList::split(" ",cmd);

  connect(execCommand, SIGNAL(receivedStdout(KProcess*,char*,int)),
          m_quanta, SLOT(slotGetScriptOutput(KProcess*,char*,int)));
  connect(execCommand, SIGNAL(receivedStderr(KProcess*,char*,int)),
          m_quanta, SLOT(slotGetScriptError(KProcess*,char*,int)));
  connect(execCommand, SIGNAL(processExited(KProcess*)),
          m_quanta, SLOT(slotProcessExited(KProcess*)));

  if (!execCommand->start(KProcess::NotifyOnExit,KProcess::All))
  {
    KMessageBox::error(m_quanta, i18n("Failed to query for running Quanta instances."));
  }
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


 QString QuantaInit::searchPathListEntry(const QString& url, const QStringList& autosavedUrlsList, bool &notFound)
{
  QString backedUpUrlHashedPath = retrieveHashedPath('.' + Document::hashFilePath(url));
  notFound = true;
  QStringList::ConstIterator autosavedUrlsEndIt = autosavedUrlsList.constEnd();
  for (QStringList::ConstIterator autosavedUrlsIt = autosavedUrlsList.constBegin();
       autosavedUrlsIt != autosavedUrlsEndIt;
       ++autosavedUrlsIt)
  {
    QString quPID = retrievePID((*autosavedUrlsIt));

    //check if the file is opened by another running Quanta or not
    bool isOrphan = true;
    QStringList::ConstIterator PIDEndIt = m_PIDlist.constEnd();
    for (QStringList::ConstIterator PIDIt = m_PIDlist.constBegin(); PIDIt != PIDEndIt; ++PIDIt )
    {
      if ((*PIDIt) == quPID && qConfig.quantaPID != quPID)
      {
        isOrphan = false; //the file is opened
        break;
      }
    }

    if (backedUpUrlHashedPath ==  retrieveHashedPath((*autosavedUrlsIt)))
    {
      notFound = false;
      if (isOrphan)
        return KURL::fromPathOrURL(*autosavedUrlsIt).path(); //the url was autosaved to this file
    }

  }
  return QString::null;
}

/** Retrieves hashed path from the name of a backup file */
QString QuantaInit::retrieveHashedPath(const QString& filename)
{
  int lastPoint = filename.findRev(".");
  int Ppos = filename.find("P", lastPoint);
  return filename.mid(lastPoint + 1,
                      Ppos - lastPoint);
}


/** Retrieves PID from the name of a backup file */
QString QuantaInit::retrievePID(const QString& filename)
{
 QString strPID = QString::null;
 strPID = filename.mid(filename.findRev("P") + 1);

 if (strPID.isEmpty())
  strPID = filename.mid(filename.findRev("N") + 1);

 if (strPID.endsWith("U"))
   strPID = strPID.left(strPID.length() - 1);

 return strPID;
}


void QuantaInit::loadVPLConfig()
{
  //load the VPL options
  m_config->setGroup("Kafka Synchronization options");
  qConfig.quantaRefreshOnFocus = (m_config->readEntry("Source refresh", "delay") == "focus");
  qConfig.quantaRefreshDelay = m_config->readNumEntry("Source refresh delay", 500);
  qConfig.kafkaRefreshOnFocus = (m_config->readEntry("Kafka refresh", "focus") == "focus");
  qConfig.kafkaRefreshDelay = m_config->readNumEntry("Kafka refresh delay", 4000);
  /**reloadUpdateTimers();*/

  m_config->setGroup("Kafka Indentation options");
  qConfig.inlineNodeIndentation = m_config->readBoolEntry("Inline Node Indentation");
}

struct Dependency{
  QString name;
  QString execName;
  QString url;
  QString description;
  enum Type{
    Executable = 0,
    Plugin
  };
  Type type;
};

void QuantaInit::checkRuntimeDependencies()
{

  QValueList<Dependency> dependencies;
  Dependency dependency;
  dependency.name = "Kommander";
  dependency.execName = "kmdr-executor";
  dependency.url = "http://kommander.kdewebdev.org";
  dependency.description = i18n("various script based dialogs including the Quick Start dialog");
  dependency.type = Dependency::Executable;
  dependencies.append(dependency);

  dependency.name = "Tidy";
  dependency.execName = "tidy";
  dependency.url = "http://tidy.sourceforge.net";
  dependency.description = i18n("HTML syntax checking");
  dependency.type = Dependency::Executable;
  dependencies.append(dependency);

  dependency.name = "Kompare";
  dependency.execName = "kompare";
  dependency.url = "http://bruggie.dnsalias.org/kompare";
  dependency.description = i18n("comparing of files by content");
  dependency.type = Dependency::Executable;
  dependencies.append(dependency);


  dependency.name = i18n("Control Center (kdebase)");
  dependency.execName = "kcmshell";
  dependency.url = "http://www.kde.org";
  dependency.description = i18n("preview browser configuration");
  dependency.type = Dependency::Executable;
  dependencies.append(dependency);

  dependency.name = "GPG (OpenPGP)";
  dependency.execName = "gpg";
  dependency.url = "http://www.gnupg.de";
  dependency.description = i18n("preview browser configuration");
  dependency.type = Dependency::Executable;
  dependencies.append(dependency);

  dependency.name = "KFileReplace";
  dependency.execName = "KFileReplace";
  dependency.url = "http://kfilereplace.kdewebdev.org";
  dependency.description = i18n("search and replace in files");
  dependency.type = Dependency::Plugin;
  dependencies.append(dependency);

  dependency.name = "KXSLDbg";
  dependency.execName = "XSLT Debugger";
  dependency.url = "http://xsldbg.sourceforge.net/";
  dependency.description = i18n("XSLT debugging");
  dependency.type = Dependency::Plugin;
  dependencies.append(dependency);


  dependency.name = "KImageMapEditor";
  dependency.execName = "KImageMapEditor";
  dependency.url = "http://www.nongnu.org/kimagemap/";
  dependency.description = i18n("editing HTML image maps");
  dependency.type = Dependency::Plugin;
  dependencies.append(dependency);


  dependency.name = "KLinkStatus";
  dependency.execName = "Link Checker";
  dependency.url = "http://kde-apps.org/content/show.php?content=12318";
  dependency.description = i18n("link validity checking");
  dependency.type = Dependency::Plugin;
  dependencies.append(dependency);

  dependency.name = "Cervisia";
  dependency.execName = "CVS Management (Cervisia)";
  dependency.url = "http://www.kde.org/apps/cervisia";
  dependency.description = i18n("CVS management plugin");
  dependency.type = Dependency::Plugin;
  dependencies.append(dependency);

  QString errorStr;
  QString stdErrorMsg = i18n("<br><b>-    %1</b> [<i>%2</i>] - %3 will not be available;");
  for (QValueList<Dependency>::ConstIterator it = dependencies.constBegin(); it != dependencies.constEnd(); ++it)
  {
    dependency = *it;
    if (dependency.type == Dependency::Executable)
    {
      if (KStandardDirs::findExe(dependency.execName).isNull())
        errorStr += QString(stdErrorMsg).arg(dependency.name).arg(dependency.url).arg(dependency.description);

    } else
    if (dependency.type == Dependency::Plugin)
    {
      if (!QuantaPlugin::validatePlugin(m_quanta->m_pluginInterface->plugin(dependency.execName)))
        errorStr += QString(stdErrorMsg).arg(dependency.name).arg(dependency.url).arg(dependency.description);
    }
  }

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
     connect(CVSService::ref(), SIGNAL(commandExecuted(const QString&, const QStringList&)), m_quanta, SLOT(slotCVSCommandExecuted(const QString&, const QStringList&)));
     //connect(CVSService::ref(), SIGNAL(statusMsg(const QString &)),  m_quanta, SLOT(slotStatusMsg(const QString & )));
     m_quanta->fTab->plugCVSMenu();
     pTab->plugCVSMenu();
  }
#endif

  if (!errorStr.isEmpty())
  {
      m_quanta->slotHideSplash();
      errorStr[errorStr.length() - 1] = '.';
      KMessageBox::information(m_quanta, "<qt>" + i18n("Some applications required for full functionality are missing:<br>") + errorStr + i18n("<br><br>You may download the applications from the specified locations.</qt>"), i18n("Missing Applications"), "RuntimeDependencyCheck");
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


int QuantaInit::runningQuantas()
{
  QCStringList list = kapp->dcopClient()->registeredApplications();
  int i = 0;
  for (QCStringList::iterator it = list.begin(); it != list.end(); ++it)
  {
    if (QString(*it).startsWith("quanta", false))
      ++i;
  }
  return i;
}

#include "quanta_init.moc"
