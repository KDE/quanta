/***************************************************************************
                          quanta.cpp  -  description
                             -------------------
    begin                : ?? ???  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2001-2003 by Andras Mantia <amantia@freemail.hu>
                           (C) 2003 by Eric Laffoon <sequitur@kde.org>
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
#include <kprogress.h>
#include <kspell.h>
#include <ktip.h>
#include <kio/netaccess.h>

// application specific includes

#include "quanta.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "resource.h"
#include "document.h"

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

QuantaApp::QuantaApp() : KDockMainWindow(0L,"Quanta"), DCOPObject("WindowManagerIf")
{
  quantaStarted = true;
  tempFileList.setAutoDelete(true);
  toolbarList.setAutoDelete(true);
  userToolbarsCount = 0;
  baseNode = 0L;
  currentToolbarDTD = QString::null;
  m_config=kapp->config();

  qConfig.globalDataDir = KGlobal::dirs()->findResourceDir("data","quanta/toolbar/quantalogo.png");
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
  m_spellChecker = new SpellChecker();
  idleTimer = new QTimer(this);
  connect(idleTimer, SIGNAL(timeout()), SLOT(slotIdleTimerExpired()));
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
 delete m_spellChecker;
 QStringList tmpDirs = KGlobal::dirs()->resourceDirs("tmp");
 tmpDir = tmpDirs[0];
 for (uint i = 0; i < tmpDirs.count(); i++)
 {
   if (tmpDirs[i].contains("kde-"))
      tmpDir = tmpDirs[i];
 }
 QDir dir;
 dir.rmdir(tmpDir + "quanta");
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

  initTagDict  ();
  initStatusBar();
  initDocument ();
  initView     ();
  initProject  ();
  initActions();

  readOptions();

  m_pluginInterface = new QuantaPluginInterface();

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

  connect(messageOutput, SIGNAL(clicked(const QString&,int)),
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

  autosaveTimer = new QTimer( this );
  connect(autosaveTimer, SIGNAL(timeout()), SLOT(slotAutosaveTimer()));
  autosaveTimer->start( 60000*qConfig.autosaveInterval, false );

  m_config->setGroup("General Options");

  if(!m_config->hasKey("Autosave List"))
  {
   m_config->writeEntry("Autosave List",QString::null);
   m_config->sync();
  }
  qConfig.autosaveEntryList = m_config->readEntry("Autosave List",qConfig.autosaveEntryList);

  if(!m_config->hasKey("List of backedup files"))
  {
   m_config->writeEntry("List of backedup files",QString::null);
   m_config->sync();
  }
  qConfig.backedupFilesEntryList = m_config->readEntry("List of backedup files",qConfig.backedupFilesEntryList);
}


void QuantaApp::initToolBars()
{
 if (toolbarList.count() == 0)
     loadToolbarForDTD(m_project->defaultDTD());
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
}

void QuantaApp::initProject()
{
  m_project = new Project(this);

  connect(m_project,  SIGNAL(openFile    (const KURL &, const QString&)),
          this,     SLOT  (slotFileOpen(const KURL &, const QString&)));
  connect(m_project,  SIGNAL(reloadTree(const KURL::List & ,bool)),
          pTab,     SLOT  (slotReloadTree(const KURL::List &,bool)));
  connect(m_project,  SIGNAL(setBaseURL(const KURL&)),
          pTab,     SLOT  (slotSetBaseURL(const KURL&)));
  connect(m_project,  SIGNAL(setProjectName(QString)),
          pTab,     SLOT  (slotSetProjectName(QString)));
  connect(m_project,  SIGNAL(closeFiles()),
          m_doc,      SLOT  (closeAll()));
  connect(m_project,  SIGNAL(showTree()),
          this,     SLOT  (slotShowProjectTree()));
  connect(m_project,  SIGNAL(removeFromProject(int)),
          pTab,     SLOT  (slotRemoveFromProject(int)));
  connect(m_project,  SIGNAL(templateURLChanged(const KURL &)),
          tTab,     SLOT  (slotSetTemplateURL(const KURL &)));

  connect(fTab,    SIGNAL(insertDirInProject(const KURL&)),
          m_project,  SLOT  (slotAddDirectory(const KURL&)));

  connect(fTab,    SIGNAL(insertFileInProject(const KURL&)),
          m_project,  SLOT  (slotInsertFile(const KURL&)));

  connect(pTab,     SIGNAL(renameInProject(const KURL&)),
          m_project,  SLOT  (slotRename(const KURL&)));
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

  connect(m_project,  SIGNAL(selectMessageWidget()),
          this,     SLOT  (slotMessageWidgetEnable()));
  connect(m_project,  SIGNAL(disableMessageWidget()),
          this,     SLOT  (slotMessageWidgetDisable()));

  connect(m_project,  SIGNAL(messages(const QString&)),
          messageOutput, SLOT(showMessage(const QString&)));

  connect(m_project,  SIGNAL(saveAllFiles()),
          this, SLOT(slotFileSaveAll()));
  connect(m_project,  SIGNAL(newStatus()),
          this, SLOT(slotNewStatus()));

  connect(m_project,  SIGNAL(newProjectLoaded()),
          this,     SLOT  (slotNewProjectLoaded()));

}

void QuantaApp::initView()
{
  ////////////////////////////////////////////////////////////////////
  // create the main widget here that is managed by KTMainWindow's view-region and
  // connect the widget to your document to display document contents.

  maindock = createDockWidget( "Editor", UserIcon("textarea"  ), 0L, i18n("Editor"), 0L);
  bottdock = createDockWidget( "Output", UserIcon("output_win"), 0L, i18n("Output"), 0L);

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
  dtabdock = createDockWidget("Docs", BarIcon ("contents2"), 0L, i18n("Documentation"), "");
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
  url = KURL();
  url.setPath("/");
  if (!topList.contains(url))
      topList.append(url);
  url = KURL();
  url.setPath(QDir::homeDirPath()+"/");
  if (!topList.contains(url))
      topList.append(url);

  fTab = new FilesTreeView(topList, ftabdock);
#ifdef BUILD_KAFKAPART
  aTab = new EnhancedTagAttributeTree(atabdock);
#else
  aTab = new TagAttributeTree(atabdock);
#endif
  pTab = new ProjectTreeView(ptabdock );
  tTab = new TemplatesTreeView("" , ttabdock);
  dTab = new DocTreeView(dtabdock);
  sTab = new StructTreeView(m_config, stabdock ,"struct");
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

  messageOutput = new MessageOutput(bottomWidgetStack);
  messageOutput->setFocusPolicy(QWidget::NoFocus);

  bottomWidgetStack->addWidget(messageOutput, 0);
//  bottomWidgetStack->addWidget( m_htmlPart   ->view(), 1 );
//  bottomWidgetStack->addWidget( htmlPartDoc->view(), 2 );

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

  connect(dTab, SIGNAL(openURL(QString)), SLOT(openDoc(QString)));

  connect(m_view, SIGNAL(dragInsert(QDropEvent *)), tTab, SLOT(slotDragInsert(QDropEvent *)));

  setMainDockWidget( maindock );
  setView(maindock);


  maindock ->setEnableDocking( KDockWidget::DockNone );
  ftabdock ->manualDock(maindock, KDockWidget::DockLeft,   30);
  bottdock ->manualDock(maindock, KDockWidget::DockBottom, 80);

  ptabdock->manualDock(ftabdock, KDockWidget::DockCenter);
  ttabdock->manualDock(ftabdock, KDockWidget::DockCenter);
  scripttabdock->manualDock(ftabdock, KDockWidget::DockCenter);
  dtabdock->manualDock(ftabdock, KDockWidget::DockCenter);
  KDockWidget *w = stabdock->manualDock(ftabdock, KDockWidget::DockCenter);
  atabdock->manualDock(w, KDockWidget::DockBottom, 70);
  qConfig.windowLayout = "Default";
  KDockManager *mng = ftabdock->dockManager();
  connect(mng, SIGNAL(change()), this, SLOT(slotDockChanged()));

//signal docking ststus changes
  connectDockSignals(maindock);
  connectDockSignals(bottdock);
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
    m_config->writeEntry("Follow Cursor", sTab->followCursor() );
    m_config->writeEntry("PHP Debugger Port", phpDebugPort );
    //If user choose the timer interval, it needs to restart the timer too
    m_config->writeEntry("Autosave interval",qConfig.autosaveInterval);
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
    m_config->writeEntry("DynamicWordWrap",qConfig.dynamicWordWrap);
   // m_doc->writeConfig(m_config); // kwrites
    m_project->writeConfig(m_config); // project
    manager()->writeConfig(m_config);
    //saveMainWindowSettings(m_config);
    writeDockConfig(m_config);
    m_spellChecker->writeConfig(m_config);
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

  qConfig.defaultDocType = m_config->readEntry("Default DTD",DEFAULT_DTD);
  if (! dtds->find(qConfig.defaultDocType))
     qConfig.defaultDocType = DEFAULT_DTD;

  qConfig.defaultEncoding = m_config->readEntry("Default encoding", QTextCodec::codecForLocale()->name());

  phpDebugPort = m_config->readNumEntry("PHP Debugger Port", 7869);

  sTab->setFollowCursor( m_config->readBoolEntry("Follow Cursor", true));

  qConfig.previewPosition   = m_config->readEntry("Preview position","Right");

  QSize s(800,580);
  resize( m_config->readSizeEntry("Geometry", &s));
  qConfig.autosaveInterval = m_config->readNumEntry("Autosave interval", 10);

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
  /**m_config->setGroup("Kafka Synchronization options");
  qConfig.quantaRefreshOnFocus = (m_config->readEntry("Source refresh", "delay") == "focus");
  qConfig.quantaRefreshDelay = m_config->readNumEntry("Source refresh delay", 4000);
  qConfig.kafkaRefreshOnFocus = (m_config->readEntry("Kafka refresh", "focus") == "focus");
  qConfig.kafkaRefreshDelay = m_config->readNumEntry("Kafka refresh delay", 4000);
  view()->reloadUpdateTimers();*/
  /**(static_cast<HTMLEnhancer *>(quantaApp->view()->getKafkaInterface()->mainEnhancer))->
    showIconsForScripts(m_config->readBoolEntry("Show Scripts Icons", true));*/
#endif

  showPreviewAction  ->setChecked( false );
  #ifdef BUILD_KAFKAPART
  showKafkaAction    ->setChecked( false );
  #endif
  showMessagesAction ->setChecked( bottdock->parent() != 0L );

  m_doc    ->readConfig(m_config); // kwrites
  m_project->readConfig(m_config); // project

  if (m_view->writeExists() && m_view->write()->isUntitled())
  {
    m_doc->closeDocument();
    m_doc->newDocument(KURL());
  }

  debuggerStyle = "None";
  m_config->setGroup  ("General Options");
  if (m_config->readBoolEntry("Enable Debugger", true))
    if (m_config->readEntry("PHP Debugger style","None") == "PHP4")
         enablePhp4Debug(true);
    else enablePhp3Debug(true);

  m_spellChecker->readConfig(m_config);
  readDockConfig(m_config);

  m_config->setGroup  ("General Options");
  QString layout = m_config->readEntry("Window layout", "Custom");
  if (layout == "Default")
  {
    ftabdock ->manualDock(maindock, KDockWidget::DockLeft,   30);
    bottdock ->manualDock(maindock, KDockWidget::DockBottom, 80);

    ptabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    ttabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    scripttabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    dtabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    KDockWidget *w = stabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    atabdock->manualDock(w, KDockWidget::DockBottom, 70);
  } else
  if (layout == "Tabbed")
  {
    ftabdock ->manualDock(maindock, KDockWidget::DockLeft,   30);
    bottdock ->manualDock(maindock, KDockWidget::DockBottom, 80);

    ptabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    ttabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    scripttabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    dtabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    stabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    atabdock->manualDock(ftabdock, KDockWidget::DockCenter);
  }
  qConfig.windowLayout = layout;

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
        messageOutput->insertItem(i18n("PHP3 debugger listens at port %1").arg(phpDebugPort));
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

    if (!isPrj || fu.isLocalFile())
        m_doc->openDocument(fu, QString::null, false);
 //   kapp->eventLoop()->processEvents( QEventLoop::ExcludeUserInput | QEventLoop::ExcludeSocketNotifiers);
  }
  m_config->sync();
  m_doc->blockSignals(false);
  m_view->writeTab()->blockSignals(false);
  Document *w = m_view->write();
  setCaption(w->url().prettyURL() );
  slotUpdateStatus(w);
}

/** Loads the initial project */
void QuantaApp::loadInitialProject(const QString& url)
{
  if(url.isNull())
    m_project->readLastConfig();
  else
    m_project->slotOpenProject(url);
}


bool QuantaApp::queryClose()
{
  bool canExit = true;
  if (quantaStarted)
  {
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
      m_project->slotCloseProject();
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
    if (n.nodeName() == "children")
   {
     QDomElement el = n.toElement();
     QDomElement item = el.firstChild().toElement();
     while ( !item.isNull() )
     {
       if (item.tagName() == "child")
       {
          QString childTag = item.attribute("name");
          if (!tag->parentDTD->caseSensitive)
              childTag = childTag.upper();
          tag->childTags.insert(childTag, item.attribute("usage") == "required");
       }
       item = item.nextSibling().toElement();
     }
   }
   if (n.nodeName() == "stoppingtags") //read what tag can act as closing tag
   {
     QDomElement el = n.toElement();
     QDomElement item = el.firstChild().toElement();
     while ( !item.isNull() )
     {
       if (item.tagName() == "stoppingtag")
       {
         QString stopTag = item.attribute("name");
         if (!tag->parentDTD->caseSensitive)
            stopTag = stopTag.upper();
         tag->stoppingTags.append(stopTag);
       }
       item = item.nextSibling().toElement();
     }
   } else
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
uint QuantaApp::readTagFile(const QString& fileName, DTDStruct* parentDTD, QTagList *tagList)
{

 QFile f( fileName );
 f.open( IO_ReadOnly );
 QDomDocument *m_doc = new QDomDocument();
 QString errorMsg;
 int errorLine, errorCol;
 if (!m_doc->setContent( &f, &errorMsg, &errorLine, &errorCol ))
 {
   KMessageBox::error(this, i18n("<qt>The DTD tag file %1 is not valid.<br> The error message is: <i>%2 in line %3, column %4.</i></qt>").arg(fileName).arg(errorMsg).arg(errorLine).arg(errorCol),
   i18n("Invalid tag file"));
   kdWarning() << fileName << ": " << errorMsg << ": " << errorLine << "," << errorCol << endl;
 }

 f.close();
 QDomNodeList nodeList = m_doc->elementsByTagName("tag");
 uint numOfTags = nodeList.count();
 for (uint i = 0; i < numOfTags; i++)
 {
    QDomNode n = nodeList.item(i);
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
 }

 delete m_doc;
 return numOfTags;
}

/** Reads the tag files and the description.rc from tagDir in order to
    build up the internal DTD and tag structures. */
void QuantaApp::readTagDir(QString &dirName)
{
 if (!QFile::exists(dirName + "description.rc"))
     return;
 QString tmpStr = dirName + "description.rc";
 QStringList tmpStrList;
 KConfig *dtdConfig = new KConfig(tmpStr);
 dtdConfig->setGroup("General");
 QString dtdName = dtdConfig->readEntry("Name", "Unknown");
 if (dtds->find(dtdName.lower()))
 {
   delete dtdConfig;
   return;
 }

 //read the general DTD info
 DTDStruct *dtd = new DTDStruct;
 dtd->fileName = tmpStr;
 dtd->commonAttrs = new AttributeListDict();
 dtd->commonAttrs->setAutoDelete(true);

 bool caseSensitive = dtdConfig->readBoolEntry("CaseSensitive");
 dtd->name = dtdName.lower();
 dtd->nickName = dtdConfig->readEntry("NickName", dtdName);
 dtd->url = dtdConfig->readEntry("URL");
 dtd->doctypeStr = dtdConfig->readEntry("DoctypeString");
 if (dtd->doctypeStr.isEmpty())
 {
   dtd->doctypeStr = "PUBLIC \"" + dtdName + "\"";
   if (!dtd->url.isEmpty())
      dtd->doctypeStr += " \"" + dtd->url + "\"";
 }
 dtd->doctypeStr.prepend(' ');
 dtd->inheritsTagsFrom = dtdConfig->readEntry("Inherits");

 dtd->defaultExtension = dtdConfig->readEntry("DefaultExtension", "html");
 dtd->mimeTypes = dtdConfig->readListEntry("MimeTypes");
 dtd->caseSensitive = caseSensitive;
 dtd->family = dtdConfig->readNumEntry("Family", Xml);
 if (dtd->family != Xml)
     dtd->toplevel = dtdConfig->readBoolEntry("TopLevel", false);
 else
     dtd->toplevel = true;
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
   tmpStr = (*it_f).fileName();
   if (!tmpStr.isEmpty())
   {
     tmpStr.prepend(dirName);
     numOfTags += readTagFile(tmpStr,dtd, tagList);
   }
 }

 //read the toolbars
 dtdConfig->setGroup("Toolbars");
 tmpStr = dtdConfig->readPathEntry("Location"); //holds the location of the toolbars
 if (!tmpStr.endsWith("/") && !tmpStr.isEmpty())
 {
   tmpStr.append("/");
 }
 dtd->toolbars = dtdConfig->readListEntry("Names");
 for (uint i = 0; i < dtd->toolbars.count(); i++)
 {
   dtd->toolbars[i] = tmpStr + dtd->toolbars[i].stripWhiteSpace() + toolbarExtension;
 }

 //read the extra tags and their attributes
 dtdConfig->setGroup("Extra tags");
 dtd->defaultAttrType = dtdConfig->readEntry("DefaultAttrType","input");
 QStrList extraTagsList;
 dtdConfig->readListEntry("List",extraTagsList);
 QString option;
 QStrList optionsList;
 QStrList attrList;
 for (uint i = 0 ; i < extraTagsList.count(); i++)
 {
   QTag *tag = new QTag();
   tag->setName(QString(extraTagsList.at(i)).stripWhiteSpace());

   tmpStr = (dtd->caseSensitive) ? tag->name() : tag->name().upper();
   if (tagList->find(tmpStr)) //the tag is already defined in a .tag file
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
   //read the possible tag options
   optionsList.clear();
   dtdConfig->readListEntry(tag->name() + "_options",optionsList);
   for (uint j = 0; j < optionsList.count(); j++)
   {
     option = QString(optionsList.at(j)).stripWhiteSpace();
     QDictIterator<AttributeList> it(*(dtd->commonAttrs));
     for( ; it.current(); ++it )
     {
       tmpStr = "has" + QString(it.currentKey()).stripWhiteSpace();
       if (option == tmpStr)
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
   attrList.clear();
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


/**** Code for the new parser *****/

 dtdConfig->setGroup("Parsing rules");
//Which DTD can be present in this one?
 dtd->insideDTDs = dtdConfig->readListEntry("MayContain");
 for (uint i = 0; i < dtd->insideDTDs.count(); i++)
 {
   dtd->insideDTDs[i] = dtd->insideDTDs[i].stripWhiteSpace().lower();
 }
//Read the special areas and area names
 QStringList specialAreasList = dtdConfig->readListEntry("SpecialAreas");
 QStringList specialAreaNameList = dtdConfig->readListEntry("SpecialAreaNames");
 QString rxStr = "";
 for (uint i = 0; i < specialAreasList.count(); i++)
 {
   if (!specialAreasList[i].stripWhiteSpace().isEmpty())
   {
     tmpStrList = QStringList::split(" ",specialAreasList[i].stripWhiteSpace());
     tmpStr = tmpStrList[0].stripWhiteSpace();
     rxStr.append(QuantaCommon::makeRxCompatible(tmpStr)+"|");
     dtd->specialAreas[tmpStr] = tmpStrList[1].stripWhiteSpace();
     dtd->specialAreaNames[tmpStr] = specialAreaNameList[i];
   }
 }
 if (rxStr.isEmpty())
 {
   dtd->specialAreaStartRx.setPattern("");
 } else
 {
  dtd->specialAreaStartRx.setPattern(rxStr.left(rxStr.length() - 1));
 }
 //Read the special tags
 tmpStrList = dtdConfig->readListEntry("SpecialTags");
 for (uint i = 0; i < tmpStrList.count(); i++)
 {
   tmpStr = tmpStrList[i].stripWhiteSpace();
   int pos = tmpStr.find('(');
   dtd->specialTags[tmpStr.left(pos).stripWhiteSpace()] = tmpStr.mid(pos+1, tmpStr.findRev(')')-pos-1).stripWhiteSpace();
 }

 //static const QString quotationStr = "\\\\\"|\\\\'";
 rxStr = "\\\\\"|\\\\'|";
 QStringList commentsList = dtdConfig->readListEntry("Comments");
 QString tmpStr2;
 for (uint i = 0; i < commentsList.count(); i++)
 {
   tmpStrList = QStringList::split(" ",commentsList[i].stripWhiteSpace());
   tmpStr = tmpStrList[0].stripWhiteSpace();
   rxStr += QuantaCommon::makeRxCompatible(tmpStr);
   rxStr += "|";
   tmpStr2 = tmpStrList[1].stripWhiteSpace();
   if (tmpStr2 == "EOL")
       tmpStr2 = '\n';
   dtd->comments[tmpStr] = tmpStr2;
 }
 dtd->commentsStartRx.setPattern(rxStr.left(rxStr.length()-1));

 //Read the tags that define this DTD
 tmpStrList = dtdConfig->readListEntry("Tags");
 for (uint i = 0; i < tmpStrList.count(); i++)
 {
   tmpStr = tmpStrList[i].stripWhiteSpace();
   int pos = tmpStr.find('(');
   dtd->definitionTags[tmpStr.left(pos).stripWhiteSpace()] = tmpStr.mid(pos+1, tmpStr.findRev(')')-pos-1).stripWhiteSpace();
 }

 //Read the areas that define the areas
 QStringList definitionAreaBorders = dtdConfig->readListEntry("AreaBorders");
 for (uint i = 0; i < definitionAreaBorders.count(); i++)
 {
   tmpStrList = QStringList::split(" ", definitionAreaBorders[i].stripWhiteSpace());
   dtd->definitionAreas[tmpStrList[0].stripWhiteSpace()] = tmpStrList[1].stripWhiteSpace();
 }

/**** End of code for the new parser *****/

//read the definition of a structure, and the structure keywords
 QStringList structKeywords = dtdConfig->readListEntry("StructKeywords",';');
 if (structKeywords.count() !=0 )
 {
    tmpStr = "\\b(";
    for (uint i = 0; i < structKeywords.count(); i++)
    {
      tmpStr += structKeywords[i].stripWhiteSpace()+"|";
    }
    tmpStr.truncate(tmpStr.length()-1);
    tmpStr += ")\\b";
 } else
 {
   tmpStr = "\\b[\\d\\S\\w]+\\b";
 }
 dtd->structKeywordsRx.setPattern(tmpStr);

 structKeywords = dtdConfig->readListEntry("LocalScopeKeywords",';');
 if (structKeywords.count() !=0 )
 {
    tmpStr = "\\b(";
    for (uint i = 0; i < structKeywords.count(); i++)
    {
      tmpStr += structKeywords[i].stripWhiteSpace()+"|";
    }
    tmpStr.truncate(tmpStr.length()-1);
    tmpStr += ")\\b";
 } else
 {
   tmpStr = "\\b[\\d\\S\\w]+\\b";
 }
 dtd->localScopeKeywordsRx.setPattern(tmpStr);

 dtd->structRx.setPattern(dtdConfig->readEntry("StructRx","\\{|\\}").stripWhiteSpace());
 dtd->structBeginStr = dtdConfig->readEntry("StructBeginStr","{").stripWhiteSpace();
 dtd->structEndStr = dtdConfig->readEntry("StructEndStr","}").stripWhiteSpace();


 dtdConfig->setGroup("Extra rules");
 dtd->minusAllowedInWord = dtdConfig->readBoolEntry("MinusAllowedInWord", false);
 tmpStr = dtdConfig->readEntry("TagAutoCompleteAfter", "<").stripWhiteSpace();
 if (tmpStr.upper() == "NONE")
    dtd->tagAutoCompleteAfter = '\0';
 else
 if (tmpStr.upper() == "ALWAYS")
    dtd->tagAutoCompleteAfter = '\1';
 else
    dtd->tagAutoCompleteAfter = tmpStr.at(0);
 dtd->attrAutoCompleteAfter = dtdConfig->readEntry("AttributeAutoCompleteAfter","(").stripWhiteSpace().at(0);
 dtd->attributeSeparator = dtdConfig->readEntry("AttributeSeparator").stripWhiteSpace().at(0);
 if (dtd->attributeSeparator.isNull())
 {
   dtd->attributeSeparator = (dtd->family == Xml) ? '\"' : ',';
 }
 dtd->tagSeparator = dtdConfig->readEntry("TagSeparator").stripWhiteSpace().at(0);
 if (dtd->tagSeparator.isNull())
     dtd->tagSeparator = dtd->attributeSeparator;

 dtd->booleanAttributes = dtdConfig->readEntry("BooleanAttributes","extended");
 dtd->booleanTrue = dtdConfig->readEntry("BooleanTrue","true");
 dtd->booleanFalse = dtdConfig->readEntry("BooleanFalse","false");
 dtd->singleTagStyle = dtdConfig->readEntry("Single Tag Style", "html").lower();

//read the definition of different structure groups, like links, images, functions
//classes, etc.
 uint structGroupsCount = dtdConfig->readNumEntry("StructGroupsCount", 0);
 if (structGroupsCount > MAX_STRUCTGROUPSCOUNT)
     structGroupsCount = MAX_STRUCTGROUPSCOUNT; //max. 10 groups

 if (dtd->family == Script)
 {
    StructTreeGroup group;
    QRegExp attrRx("\\([^\\)]*\\)");
    QString tagStr;
    for (uint index = 1; index <= structGroupsCount; index++)
    {
      dtdConfig->setGroup(QString("StructGroup_%1").arg(index));
      //new code
      group.name = dtdConfig->readEntry("Name").stripWhiteSpace();
      group.noName = dtdConfig->readEntry("No_Name").stripWhiteSpace();
      group.icon = dtdConfig->readEntry("Icon").stripWhiteSpace();
      group.searchRx = dtdConfig->readEntry("SearchRx").stripWhiteSpace();
      group.hasSearchRx = !group.searchRx.pattern().isEmpty();
      group.clearRx = dtdConfig->readEntry("ClearRx").stripWhiteSpace();
      tagStr = dtdConfig->readEntry("Tag").stripWhiteSpace();
      group.tag = "";
      if (!tagStr.isEmpty())
      {
        attrRx.search(tagStr);
        tmpStr = attrRx.cap();
        tmpStrList = QStringList::split(',', tmpStr.mid(1, tmpStr.length()-2));
        group.tag = tagStr.left(tagStr.find('(')).lower();
        group.attributes.clear();
        for (uint i = 0; i < tmpStrList.count(); i++)
          group.attributes += tmpStrList[i].stripWhiteSpace();
      }
      tagStr = dtdConfig->readEntry("TagType", "Text").stripWhiteSpace();
      if (tagStr == "XmlTag")
          group.tagType = Tag::XmlTag;
      else if (tagStr == "XmlTagEnd")
          group.tagType = Tag::XmlTagEnd;
      else if (tagStr == "Text")
          group.tagType = Tag::Text;
      else if (tagStr == "Comment")
          group.tagType = Tag::Comment;
      else if (tagStr == "CSS")
          group.tagType = Tag::CSS;
      else if (tagStr == "ScriptTag")
          group.tagType = Tag::ScriptTag;
      else if (tagStr == "ScriptStructureBegin")
          group.tagType = Tag::ScriptStructureBegin;
      else if (tagStr == "ScriptStructureEnd")
          group.tagType = Tag::ScriptStructureEnd;
      else group.tagType = -1;
      tmpStr = dtdConfig->readEntry("AutoCompleteAfter").stripWhiteSpace();
      group.autoCompleteAfterRx.setPattern(tmpStr);
      tmpStr = dtdConfig->readEntry("RemoveFromAutoCompleteWord").stripWhiteSpace();
      group.removeFromAutoCompleteWordRx.setPattern(tmpStr);
      group.hasFileName = dtdConfig->readBoolEntry("HasFileName", false);
      group.parseFile = dtdConfig->readBoolEntry("ParseFile", false);
      tmpStr = dtdConfig->readEntry("FileNameRx").stripWhiteSpace();
      group.fileNameRx.setPattern(tmpStr);
      dtd->structTreeGroups.append(group);
    }
  } else
  {
    XMLStructGroup group;
    QRegExp attrRx("\\([^\\)]*\\)");
    QString tagName;
    for (uint index = 1; index <= structGroupsCount; index++)
    {
      dtdConfig->setGroup(QString("StructGroup_%1").arg(index));
      group.name = dtdConfig->readEntry("Name").stripWhiteSpace();
      group.noName = dtdConfig->readEntry("No_Name").stripWhiteSpace();
      group.icon = dtdConfig->readEntry("Icon").stripWhiteSpace();
      QString tagStr = dtdConfig->readEntry("Tag").stripWhiteSpace();
      if (!tagStr.isEmpty())
      {
        attrRx.search(tagStr);
        tmpStr = attrRx.cap();
        tmpStrList = QStringList::split(',', tmpStr.mid(1, tmpStr.length()-2));
        tagName = tagStr.left(tagStr.find('(')).lower();
        group.attributes.clear();
        for (uint i = 0; i < tmpStrList.count(); i++)
          group.attributes += tmpStrList[i].stripWhiteSpace();
        group.hasFileName = dtdConfig->readBoolEntry("HasFileName", false);
        tmpStr = dtdConfig->readEntry("FileNameRx").stripWhiteSpace();
        group.fileNameRx.setPattern(tmpStr);
        dtd->xmlStructTreeGroups.insert(tagName, group);
      }
    }
  }
 //read the abbreviations files
  QString abbrevFile = dirName;
  tmpStr = dirName;
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
   QDomDocument abbrevDom;
   if (abbrevDom.setContent(&f))
   {
     QDomNodeList nodeList = abbrevDom.elementsByTagName("Template");
     for (uint i = 0; i < nodeList.count(); i++)
     {
       QDomElement e = nodeList.item(i).toElement();
       dtd->abbreviations.insert(e.attribute("name")+" "+e.attribute("description"), e.attribute("code"));
     }
   }
   f.close();
 }

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

  QStringList tagsResourceDirs = KGlobal::instance()->dirs()->findDirs("appdata", "dtep");
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

  DTDStruct *dtd;
//Resolve the inheritence
  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    dtd = it.current();
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
      QMap<QString, QString>::Iterator abbrevIt;
      for (abbrevIt = parent->abbreviations.begin(); abbrevIt != parent->abbreviations.end(); ++abbrevIt)
      {
        dtd->abbreviations.insert(abbrevIt.key(), abbrevIt.data());
      }
    }
  }

//Read the pseudo DTD area definition strings (special area/tag string)
//from the DTD's which may be present in an other DTD (May_Contain setting)
  it.toFirst();
  QMap<QString, QString>::Iterator mapIt;
  QString tmpStr;
  for( ; it.current(); ++it)
  {
    dtd = it.current();
    QString specialAreaStartRxStr = dtd->specialAreaStartRx.pattern();
    if (!specialAreaStartRxStr.isEmpty())
        specialAreaStartRxStr += "|";
    for (uint i = 0; i < dtd->insideDTDs.count(); i++)
    {
      DTDStruct *insideDTD = dtds->find(dtd->insideDTDs[i]);
      if (!insideDTD)
          insideDTD = dtds->find(QuantaCommon::getDTDNameFromNickName(dtd->insideDTDs[i]));
      if (insideDTD)
      {
        for (mapIt = insideDTD->definitionAreas.begin(); mapIt != insideDTD->definitionAreas.end(); ++mapIt)
        {
          tmpStr = mapIt.key();
          dtd->specialAreas[tmpStr] = mapIt.data();
          dtd->specialAreaNames[tmpStr] = dtd->insideDTDs[i];
          specialAreaStartRxStr.append("(?:"+ QuantaCommon::makeRxCompatible(tmpStr)+")|");
        }

        for (mapIt = insideDTD->definitionTags.begin(); mapIt != insideDTD->definitionTags.end(); ++mapIt)
        {
          dtd->specialTags[mapIt.key()] = mapIt.data();
        }
      }
    }
    dtd->specialAreaStartRx.setPattern(specialAreaStartRxStr.left(specialAreaStartRxStr.length() - 1));
  }

  scriptBeginRx.setCaseSensitive(false);
  scriptBeginRx.setPattern(scriptBeginRxStr);
  scriptEndRx.setCaseSensitive(false);
  scriptEndRx.setPattern(scriptEndRxStr);
  if (!dtds->find(qConfig.defaultDocType)) qConfig.defaultDocType = DEFAULT_DTD;
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

    KStdAction::selectAll(m_view, SLOT(slotSelectAll()), ac);
    KStdAction::deselect(m_view, SLOT(slotDeselectAll()), ac);
    (void) new KAction( i18n( "&Toggle Block Selection" ), Key_F4, m_view,
                        SLOT( toggleVertical() ), ac, "set_verticalSelect" );
    new KAction(i18n("Toggle &Insert"), Key_Insert, m_view, SLOT(toggleInsert()), ac, "set_insert" );


    KStdAction::find(this, SLOT(slotFind()), ac);
    KStdAction::findNext(this, SLOT(slotFindAgain()), ac);
    KStdAction::findPrev(this, SLOT(slotFindAgainB()), ac, "edit_find_prev");
    KStdAction::replace(this, SLOT(slotReplace()), ac);

    new KAction(i18n("&Indent"), "indent", CTRL+Key_I, m_view,
                SLOT(slotIndent()), ac, "edit_indent");
    new KAction(i18n("&Unindent"), "unindent", CTRL+SHIFT+Key_I, m_view,
                SLOT(slotUnIndent()), ac, "edit_unindent");
    new KAction(i18n("&Clean Indentation"), "cleanindent", 0, m_view,
                SLOT(slotCleanIndent()), ac, "edit_cleanindent");

    new KAction(i18n("Co&mment"),  CTRL+Qt::Key_NumberSign,
                m_view, SLOT(slotComment()), ac, "edit_comment");
    new KAction(i18n("Unc&omment"),
                CTRL+SHIFT+Qt::Key_NumberSign, m_view, SLOT(slotUnComment()),
                ac, "edit_uncomment");
    new KAction(i18n("Apply Word Wrap"), "", 0, m_view, SLOT(slotApplyWordWrap()),
                ac, "edit_apply_wordwrap");



//Tools menu
    KStdAction::gotoLine(m_view, SLOT(slotGotoLine()), ac, "edit_goto_line");
    KStdAction::spelling(m_view, SLOT(slotSpellcheck()), ac);

//Bookmarks
    bookmarkToggle = new KAction(i18n("Toggle &Bookmark"), Qt::CTRL+Qt::Key_B,
          m_view, SLOT(toggleBookmark()), ac, "bookmarks_toggle");
    bookmarkClear = new KAction(i18n("Clear Bookmarks"), 0, m_view,
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


    setEndOfLine = new KSelectAction(i18n("&End of Line"), 0, ac,"set_eol");
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

    (void) new KAction( i18n( "&Context Help..." ), CTRL+Key_H,
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

    (void) new KAction( i18n( "Tag &Attributes" ), ALT+Key_Down,
                        m_doc, SLOT( slotAttribPopup() ),
                        ac, "tag_attributes" );

    (void) new KAction( i18n( "&Change DTD Type" ), 0,
                        this, SLOT( slotToolsChangeDTD() ),
                        ac, "tools_change_dtd" );

    (void) new KAction( i18n( "Send DTD in E-&Mail" ), 0,
                        this, SLOT( slotEmailDTD() ),
                        ac, "tools_send_dtd" );

#ifdef BUILD_KAFKAPART
    (void) new KAction( i18n( "Document Properties" ), 0,
                        this, SLOT( slotDocumentProperties() ),
                        ac, "tools_document_properties" );
#endif

 /*   (void) new KAction( i18n( "Weblint &Syntax Check" ), 0,
                        this, SLOT( slotToolSyntaxCheck() ),
                        ac, "syntax_check" );
*/
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

    #ifdef BUILD_KAFKAPART
    KToggleAction *ta;
      ta =
      new KToggleAction( i18n( "&Source Editor") , "show_quanta_editor", ALT+Key_F9,
                         m_view, SLOT( slotShowQuantaEditor()),
                         ac, "show_quanta_editor");
      ta->setExclusiveGroup("view");

     showKafkaAction =
      new KToggleAction( i18n( "&VPL Editor"), UserIcon ("vpl"), CTRL+SHIFT+Key_F9,
      m_view, SLOT( slotShowKafkaPart() ),
                          ac, "show_kafka_view");
     showKafkaAction->setExclusiveGroup("view");

     ta =
      new KToggleAction( i18n("V&PL && Source Editors"), "show_kafka_and_quanta", Key_F9,
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
      new KToggleAction( i18n( "Pr&eview" ), "preview", Key_F6,
                         this, SLOT( slotShowPreview() ),
                         ac, "show_preview" );
     showPreviewAction->setExclusiveGroup("view");

    (void) new KAction( i18n( "&Reload Preview" ), "reload",
                        KStdAccel::shortcut(KStdAccel::Reload).keyCodeQt(),
                        this, SLOT( slotViewRepaint() ),
                        ac, "reload" );

    (void) new KAction( i18n( "View with Net&scape" ), "netscape", ALT+Key_F6,
                        m_view, SLOT( slotViewInNetscape() ),
                        ac, "view_with_netscape" );

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
    (void) new KAction( i18n( "&New Project..." ), 0,
                        m_project, SLOT( slotNewProject() ),
                        ac, "project_new" );

    (void) new KAction( i18n( "&Open Project..." ), BarIcon("folder_new"), 0,
                        m_project, SLOT( slotOpenProject() ),
                        ac, "project_open" );
#if KDE_VERSION < KDE_MAKE_VERSION(3,1,92)
    m_project -> projectRecent = new KQRecentFilesAction(i18n("Open Recent Project"),
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


    openPrjViewAction = new KAction( i18n( "Open Project View..." ), 0,
                        m_project, SLOT( slotOpenProjectView() ),
                        ac, "project_view_open" );

    savePrjViewAction = new KAction( i18n( "Save Project View" ), 0,
                        m_project, SLOT( slotSaveProjectView() ),
                        ac, "project_view_save" );
    saveAsPrjViewAction = new KAction( i18n( "Save Project View As..." ), 0,
                        m_project, SLOT( slotSaveAsProjectView() ),
                        ac, "project_view_save_as" );



    insertFileAction = new KAction( i18n( "&Insert Files..." ), 0,
                        m_project, SLOT( slotAddFiles() ),
                        ac, "project_insert_file" );

    insertDirAction = new KAction( i18n( "&Insert Directory..." ), 0,
                        m_project, SLOT( slotAddDirectory() ),
                        ac, "project_insert_directory" );

    rescanPrjDirAction = new KAction( i18n( "&Rescan Project Directory" ), SmallIcon("reload"), 0,
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
    new KAction(i18n("&Add User Toolbar"),  0, this, SLOT(slotAddToolbar()), ac, "toolbars_add");
    new KAction(i18n("&Remove User Toolbar"), 0, this, SLOT(slotRemoveToolbar()), ac, "toolbars_remove");
    new KAction(i18n("Re&name User Toolbar"), 0, this, SLOT(slotRenameToolbar()), ac, "toolbars_rename");
    new KAction(i18n("Send Toolbar in E-&Mail"), 0, this, SLOT(slotSendToolbar()), ac, "toolbars_send");

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
/*    new KAction(i18n("Upload Opened Project Files"), 0, this, SLOT(slotUploadOpenedFiles()), ac, "upload_opened_files"); */

    m_actions = new QDomDocument();
//load the global actions
    QFile f(qConfig.globalDataDir + "quanta/actions.rc");
    if ( f.open( IO_ReadOnly ))
    {
      if (m_actions->setContent(&f))
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
      }
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
        if (m_actions->setContent(&f))
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
        }
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
void QuantaApp::recoverCrashed()
{
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
  QStringList urls = m_config->readPathListEntry("List of backedup files");
  QStringList autosaveUrls = m_config->readPathListEntry("Autosave List");
#else
  QStringList urls = m_config->readListEntry("List of backedup files");
  QStringList autosaveUrls = m_config->readListEntry("Autosave List");
#endif
  m_doc->blockSignals(true);
  m_view->writeTab()->blockSignals(true);

  for ( QStringList::Iterator it = urls.begin(); it != urls.end(); ++it )
  {
   // when quanta crash and file autoreloading option is on
    // then if user restart quanta, the backup copies will reload
   QString autosavePath = searchPathListEntry((*it),autosaveUrls.join(","));

    if(!autosavePath.isEmpty())
    {
     KURL originalVersion;
     QuantaCommon::setUrl(originalVersion, *it);
     KURL autosavedVersion;
     QuantaCommon::setUrl(autosavedVersion,autosavePath);

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
          DirtyDlg *dlg = new DirtyDlg(autosavedVersion.path(), originalVersion.path(), false, this);
          dlg->setCaption(i18n("Restore file"));
          dlg->textLabel->setText(i18n("<qt>A backup copy of a file was found:<br><br>"
           "Original file: <b>%1</b><br>"
           "Original file size: <b>%2</b><br>"
           "Original file last modified on: <b>%3</b><br><br>"
           "Backup file size: <b>%4</b><br>"
           "Backup created on: <b>%5</b><br><br>"
           "</qt>")
           .arg(originalVersion.prettyURL(0, KURL::StripFileProtocol ))
           .arg(KIO::convertSize(origSize)).arg(origTime)
           .arg(KIO::convertSize(backupSize)).arg(backupTime));
          dlg->buttonLoad->setText(i18n("&Restore the file from backup"));
          dlg->buttonIgnore->setText(i18n("Do &not restore the file from backup"));
          delete dlg->warningLabel;
          dlg->warningLabel = 0L;
          dlg->adjustSize();
          if (!KStandardDirs::findExe("kompare").isEmpty())
          {
            dlg->buttonCompare->setEnabled(false);
            dlg->buttonLoad->setChecked(true);
          }
          if (dlg->exec())
          {
            KURL backupURL = originalVersion;
            backupURL.setPath(backupURL.path()+".backup");
          //TODO: Replace with KIO::NetAccess::file_move, when KDE 3.1 support
  //is dropped
            QExtFileInfo::copy(originalVersion, backupURL, -1, true, false, this);
          //TODO: Replace with KIO::NetAccess::file_copy, when KDE 3.1 support
          //is dropped
            QExtFileInfo::copy(autosavedVersion, originalVersion, -1, true, false, this);
            
            //slotFileOpenRecent(originalVersion);
          }
          delete dlg;
       }
     }
     if(QFile::exists(autosavedVersion.path())) QFile::remove(autosavedVersion.path());     
    }
  }
  m_config->setGroup("General Options");
  m_config->writeEntry("Autosave List",QString::null);
  m_config->writeEntry("List of backedup files",QString::null);
  qConfig.autosaveEntryList = "";
}


