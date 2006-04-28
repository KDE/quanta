/***************************************************************************
    begin                : Tue May 17 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

//own includes
#include "usertoolbarspart.h"
#include "actionconfigdialog.h"
#include "askforsavedlg.h"
#include "extfileinfo.h"
#include "helper.h"
#include "maildialog.h"
#include "newtoolbarstuff.h"
#include "resizablewidget.h"
#include "toolbarguibuilder.h"
#include "toolbartabwidget.h"
#include "toolbarxmlgui.h"
#include "useraction.h"
#include "usertoolbarsglobalconfig.h"

//qt includes
#include <qbuffer.h>
#include <qdir.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmenu.h>
#include <QMenuItem>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qtextedit.h>
#include <qtextstream.h>
#include <qtimer.h>
#include <QMap>

//kde includes
#include <kaction.h>
#include <kconfig.h>
#include <kedittoolbar.h>
#include <kfiledialog.h>
#include <kiconloader.h>
#include <kinputdialog.h>
#include <kio/netaccess.h>
#include <kmainwindow.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <ktar.h>
#include <ktempdir.h>
#include <ktempfile.h>
#include <ktoolinvocation.h>
#include <kxmlguifactory.h>

#include <configwidgetproxy.h>
#include <kdevappfrontend.h>
#include <kdevcore.h>
#include <kdevgenericfactory.h>
#include <kdevmainwindow.h>
#include <kdevdocumentcontroller.h>
#include <kdevplugincontroller.h>
#include <kdevplugininfo.h>
#include <kdevproject.h>

typedef KDevGenericFactory<UserToolbarsPart> UserToolbarsFactory;
KDevPluginInfo data("kdevusertoolbars");
K_EXPORT_COMPONENT_FACTORY( libkdevusertoolbars, UserToolbarsFactory( data ) );


#define GLOBALDOC_OPTIONS 1
#define PROJECTDOC_OPTIONS 2


const QString resourceDir = "quanta/";


UserToolbarsPart::UserToolbarsPart(QObject *parent, const char *name, const QStringList &/*args*/)
    : KDevPlugin(&data, parent)
{
    kDebug(24000) << "Creating UserToolbars Part" << endl;
    setObjectName(name);
    setInstance(UserToolbarsFactory::instance());
    setXMLFile("kdevusertoolbars.rc");

    setupActions();

    m_configProxy = new ConfigWidgetProxy(core());
    m_configProxy->createGlobalConfigPage(i18n("UserToolbars"), GLOBALDOC_OPTIONS, info()->icon());
    m_configProxy->createProjectConfigPage(i18n("UserToolbars"), PROJECTDOC_OPTIONS, info()->icon());
    connect(m_configProxy, SIGNAL(insertConfigWidget(const KDialogBase*, QWidget*, unsigned int )),
        this, SLOT(insertConfigWidget(const KDialogBase*, QWidget*, unsigned int)));

    connect(core(), SIGNAL(contextMenu(QMenu *, const Context *)),
        this, SLOT(contextMenu(QMenu *, const Context *)));
    connect(core(), SIGNAL(projectOpened()), this, SLOT(projectOpened()));
    connect(core(), SIGNAL(projectClosed()), this, SLOT(projectClosed()));

//setup some member variables
    m_newToolbarStuff = 0L;
    m_userToolbarsCount = 0;
    m_actionsMenuId = -1;
    m_outputPlugin = 0L;
    QStringList tmpDirs = KGlobal::dirs()->resourceDirs("tmp");
    kDebug(24000) << "tmpDirs: " << tmpDirs << endl;
    QDir dir;
    m_tmpDir = tmpDirs[0];
    for (int i = 0; i < tmpDirs.count(); i++)
    {
      if (tmpDirs[i].contains("kde-"))
        m_tmpDir = tmpDirs[i];
    }
    dir.mkdir(m_tmpDir + "kdevelop");
    dir.mkdir(m_tmpDir + "kdevelop/usertoolbars");
    m_tmpDir += "kdevelop/usertoolbars/toolbar-";

//initialize the tabwidget and the gui builder
    ToolbarTabWidget *toolbarTab = ToolbarTabWidget::ref();
    ResizableWidget *w = new ResizableWidget(toolbarTab, 0L);
    w->hide();
    connect(toolbarTab, SIGNAL(removeToolbar(const QString&)),
            this, SLOT(slotRemoveToolbar(const QString&)));
    connect(toolbarTab, SIGNAL(renameToolbar(const QString&)),
            this, SLOT(slotRenameToolbar(const QString&)));
    connect(toolbarTab, SIGNAL(editToolbar(const QString&)),
            this, SLOT(slotConfigureToolbars(const QString&)));
    connect(toolbarTab, SIGNAL(newAction()),
            this, SLOT(slotNewAction()));
    connect(toolbarTab, SIGNAL(addToolbar()),
            this, SLOT(slotAddToolbar()));
    connect(toolbarTab, SIGNAL(toolbarInserted(const QString&)),
            this, SLOT(slotToolbarLoaded(const QString&)));
    connect(toolbarTab, SIGNAL(toolbarRemoved(const QString&)),
            this, SLOT(slotToolbarRemoved(const QString&)));
    connect(toolbarTab, SIGNAL(removeAction(const QString&, const QString&)),
            this, SLOT(slotRemoveAction(const QString&, const QString&)));
    connect(toolbarTab, SIGNAL(editAction(const QString&)),
            this, SLOT(slotEditAction(const QString&)));

//let the ToolbarGUIBuilder handle the creation of the GUI for this plugin, so
//creation of the main user toolbar is detected as well
    setClientBuilder(ToolbarGUIBuilder::ref(mainWindow()->main()));
    QTimer::singleShot(0, this, SLOT(init()));
}

UserToolbarsPart::~UserToolbarsPart()
{
//delete temporary files and directories
  qDeleteAll(m_tempFileList.begin(), m_tempFileList.end());
  m_tempFileList.clear();
  for (int i = 0; i < m_tempDirList.count(); i++)
  {
      KIO::NetAccess::del(KUrl().fromPathOrURL(m_tempDirList.at(i)->name()), 0L);
  }
  qDeleteAll(m_tempDirList.begin(), m_tempDirList.end());
  m_tempDirList.clear();
  QDir dir;
  m_tmpDir.remove("/toolbar-");
  dir.rmdir(m_tmpDir);
  m_tmpDir.remove("/usertoolbars");
  dir.rmdir(m_tmpDir);

  delete m_configProxy;
  delete m_newToolbarStuff;
  m_newToolbarStuff = 0L;
  m_toolbarList.clear();
}

void UserToolbarsPart::init()
{
  KConfig *config = UserToolbarsFactory::instance()->config();
  config->setGroup("General");
  m_separateToolbars = config->readEntry("Separate toolbars", false);
  m_createActionsMenu = config->readEntry("Create Actions menu", true);
  slotAdjustActions();
  ToolbarGUIBuilder::ref(mainWindow()->main())->setSeparateToolbars(m_separateToolbars);
  QMenu *actionsMenu = static_cast<QMenu*>(factory()->container("actions", this));

  if (actionsMenu)
    actionsMenu->setVisible(false);

/* I keep this for reference for the moment. Jens
  KMenuBar *menuBar = mainWindow()->main()->menuBar();
  for (int i = 0; i < menuBar->count(); i++)
  {
    QMenuItem *it = menuBar->findItem(menuBar->idAt(i));
    if (it->menu() == actionsMenu)
    {
      m_actionsMenuId = menuBar->idAt(i);
      mainWindow()->main()->menuBar()->setItemVisible(m_actionsMenuId, false);
      break;
    }
  }*/
}

void UserToolbarsPart::slotAdjustActions()
{
  bool toolbarsLoaded = (ToolbarTabWidget::ref()->count() > 0);
  bool projectLoaded = (project() != 0L);
  KActionCollection *ac = actionCollection();
  ac->action("toolbars_save_local")->setEnabled(toolbarsLoaded);
  ac->action("toolbars_save_project")->setEnabled(toolbarsLoaded && projectLoaded);
  ac->action("toolbars_remove")->setEnabled(toolbarsLoaded);
  ac->action("toolbars_rename")->setEnabled(toolbarsLoaded);
  ac->action("toolbars_send")->setEnabled(toolbarsLoaded);
  ac->action("toolbars_upload")->setEnabled(toolbarsLoaded);
  m_projectToolbarFiles->setEnabled(projectLoaded);
}


void UserToolbarsPart::setupActions()
{
  KActionCollection *ac = actionCollection();
  m_projectToolbarFiles = new KRecentFilesAction(i18n("Load &Project Toolbar"), ac, "toolbars_load_project");
  connect(m_projectToolbarFiles, SIGNAL(urlSelected(const KUrl &)), SLOT(slotLoadToolbarFile(const KUrl&)));

  KAction *action = new KAction(i18n("Load &Global Toolbar..."), ac, "toolbars_load_global");
  connect(action, SIGNAL(triggered(bool)), SLOT(slotLoadGlobalToolbar()));
  action = new KAction(i18n("Load &Local Toolbar..."), ac, "toolbars_load_user");
  connect(action, SIGNAL(triggered(bool)), SLOT(slotLoadToolbar()));
  action = new KAction(i18n("Save as &Local Toolbar..."), ac, "toolbars_save_local");
  connect(action, SIGNAL(triggered(bool)), SLOT(slotSaveLocalToolbar()));
  action = new KAction(i18n("Save as &Project Toolbar..."), ac, "toolbars_save_project");
  connect(action, SIGNAL(triggered(bool)), SLOT(slotSaveProjectToolbar()));
  action = new KAction(i18n("&New User Toolbar..."), ac, "toolbars_add");
  connect(action, SIGNAL(triggered(bool)), SLOT(slotAddToolbar()));
  action = new KAction(i18n("&Remove User Toolbar..."), ac, "toolbars_remove");
  connect(action, SIGNAL(triggered(bool)), SLOT(slotRemoveToolbar()));
  action = new KAction(i18n("Re&name User Toolbar..."), ac, "toolbars_rename");
  connect(action, SIGNAL(triggered(bool)), SLOT(slotRenameToolbar()));
  action = new KAction(KIcon("mail_send"), i18n("Send Toolbar in E&mail..."), ac, "toolbars_send");
  connect(action, SIGNAL(triggered(bool)), SLOT(slotSendToolbar()));
  action = new KAction(KIcon("network"), i18n("&Upload Toolbar..." ), ac, "toolbars_upload" );
  connect(action, SIGNAL(triggered(bool)), SLOT(slotUploadToolbar()));
  action = new KAction(KIcon("network"), i18n("&Download Toolbar..." ), ac, "toolbars_download" );
  connect(action, SIGNAL(triggered(bool)), SLOT(slotDownloadToolbar()));
  action = new KAction(KIcon("ball"), i18n( "Configure &Actions..." ), ac, "configure_actions" );
  connect(action, SIGNAL(triggered(bool)), SLOT(slotConfigureActions()));
}

void UserToolbarsPart::insertConfigWidget(const KDialogBase *dlg, QWidget *page, unsigned int pageNo)
{
// create configuraton dialogs here
    switch (pageNo)
    {
        case GLOBALDOC_OPTIONS:
        {
            UserToolbarsGlobalConfig *w = new UserToolbarsGlobalConfig(this, page);
            connect(dlg, SIGNAL(okClicked()), w, SLOT(accept()));
            break;
        }
    }
}

void UserToolbarsPart::contextMenu(QMenu *popup, const Context *context)
{
  Q_UNUSED(popup);
  Q_UNUSED(context);
}

void UserToolbarsPart::projectOpened()
{
// do something when the project is opened
  slotAdjustActions();
}

void UserToolbarsPart::projectClosed()
{
// do something when the project is closed
  QTimer::singleShot(0, this, SLOT(slotAdjustActions()));
}


void UserToolbarsPart::setCreateActionsMenu(bool create)
{
 if (m_createActionsMenu != create)
  {
    m_createActionsMenu = create;
    QHashIterator<QString, ToolbarEntry*> it(m_toolbarList);
    ToolbarEntry *p_toolbar;
    while (it.hasNext())
    {
      it.next();
      p_toolbar = it.value();
      if (m_createActionsMenu)
        slotToolbarLoaded(p_toolbar->id);
      else
        slotToolbarRemoved(p_toolbar->id);
    }
  }
}

void UserToolbarsPart::setSeparateToolbars(bool separate)
{
  if (m_separateToolbars != separate)
  {
    KMainWindow *mw = mainWindow()->main();
    ToolbarGUIBuilder::ref(mw)->setSeparateToolbars(separate);
    m_separateToolbars = separate;
    QHashIterator<QString, ToolbarEntry*> it(m_toolbarList);
    ToolbarEntry *p_toolbar;
    while (it.hasNext())
    {
      it.next();
      p_toolbar = it.value();
      kDebug(24000) << "p_toolbar->guiClient in setSeparateToolbars:" <<  p_toolbar->guiClient->domDocument().toString() << endl;
      mw->guiFactory()->removeClient(p_toolbar->guiClient);
/*      if (m_separateToolbars)
        p_toolbar->guiClient->setClientBuilder(mw->clientBuilder());
      else*/
        p_toolbar->guiClient->setClientBuilder(ToolbarGUIBuilder::ref(mw));
      mw->guiFactory()->addClient(p_toolbar->guiClient);
    }
  }
}


void UserToolbarsPart::slotLoadToolbarFile(const KUrl& url)
{
//check if the toolbar is already loaded or not
  QHashIterator<QString, ToolbarEntry*> it(m_toolbarList);
  ToolbarEntry *p_toolbar;
  while (it.hasNext())
  {
    it.next();
    p_toolbar = it.value();
    if (url == p_toolbar->url)
        return;
  }

  QDomDocument actionDom;
  QDomDocument *toolbarDom = new QDomDocument();

  QTextStream str;
  str.setCodec(QTextCodec::codecForName("UTF-8"));
  QString fileName = url.path();

  if (url.fileName().endsWith(Helper::toolbarExtension()))
  {
  //extract the files from the archives
    KTar tar(fileName);
    if (tar.open(IO_ReadOnly))
    {
      QString base = QFileInfo(fileName).baseName();
      KArchiveFile* file = (KArchiveFile *)(tar.directory()->entry(base + ".toolbar"));
      if (file)
      {
        toolbarDom->setContent(file->device());
      }
      file = (KArchiveFile *)(tar.directory()->entry(base + ".actions"));
      if (file)
      {
        QIODevice *device = file->device();
        actionDom.setContent(device);
        delete device;
      }
      tar.close();
    }
    if ( (toolbarDom->toString().isEmpty()) ) //|| (actionContent.isEmpty()))
    {
      KMessageBox::error(mainWindow()->main(), i18n("Cannot load the toolbars from the archive.\nCheck that the filenames inside the archives begin with the archive name."));
      delete toolbarDom;
      return;
    }

    QDomNodeList nodeList = toolbarDom->elementsByTagName("ToolBar");
    QString name = nodeList.item(0).cloneNode().toElement().attribute("tabname");

    //search for another toolbar with the same name
    QList<KXMLGUIClient*> xml_clients = mainWindow()->main()->guiFactory()->clients();
    QString newName = name;
    QString i18nName = i18n(name.toUtf8());
    QString origName = name;
    bool found = false;
    bool nameModified = false;
    int count = 2;
    do
    {
      int index = 0;
      while (index < xml_clients.count())
      {
        name = newName;
        if (index == 0)
          found = false;
        nodeList = xml_clients.at(index)->domDocument().elementsByTagName("ToolBar");
        for (int i = 0; i < nodeList.count(); i++)
        {
          if ((nodeList.item(i).cloneNode().toElement().attribute("name").toLower() ) == name.toLower())
          {
            newName = origName + QString(" (%1)").arg(count);
            i18nName = i18n(origName.toUtf8()) + QString(" (%1)").arg(count);
            nameModified = true;
            count++;
            found = true;
            break;
          }
        }
        if (found)
        {
          index = 0;
        } else
        {
          index++;
        }
      }
    } while (name == newName && found);
    name = newName;

    //create the ToolbarEntry for this toolbar
    p_toolbar = new ToolbarEntry;
    p_toolbar->menu = 0L;

    QDomDocument *dom = new QDomDocument();
    dom->setContent(toolbarDom->toString());
    p_toolbar->dom = dom;
    p_toolbar->nameModified = nameModified;

    //be sure that the id is unique
    QString s = i18nName.toLower();
    QString toolbarId = s;
    QRegExp rx("\\s|\\.");
    toolbarId.replace(rx, "_");
    int n = 1;
    while (m_toolbarList.value(toolbarId) != 0L)
    {
      toolbarId = s + QString("%1").arg(n);
      toolbarId.replace(rx, "_");
      n++;
    }


    m_userToolbarsCount++;

    //Change the name also in the XML File -> create a temp XML file
    KTempFile* tempFile = new KTempFile(m_tmpDir);
    tempFile->setAutoDelete(true);

    nodeList = toolbarDom->elementsByTagName("ToolBar");
    QDomElement el = nodeList.item(0).toElement();
    el.setAttribute("name", name.toLower());
    el.setAttribute("tabname", name);
    el.setAttribute("i18ntabname", i18nName);
    el.setAttribute("id", toolbarId);
    nodeList = toolbarDom->elementsByTagName("text");
    el.firstChild().setNodeValue(name);
    tempFile->textStream()->setCodec(QTextCodec::codecForName("UTF-8"));
    * (tempFile->textStream()) << toolbarDom->toString();
    tempFile->close();

    //create the new toolbar GUI from the temp file
    ToolbarXMLGUI * toolbarGUI = new ToolbarXMLGUI(tempFile->name());

    //setup the actions
    KActionCollection *ac = mainWindow()->main()->actionCollection();
    nodeList = actionDom.elementsByTagName("action");
    for (int i = 0; i < nodeList.count(); i++)
    {
      QDomNode node = nodeList.item(i).cloneNode();
      el = node.toElement();
      QString actionName = el.attribute("name");
      //if there is no such action yet, add to the available actions
      if (!ac->action(actionName))
      {
        bool toggable = (el.attribute("toggable", "") == "true");
        UserAction *userAction = new UserAction(&el, this, toggable);
        m_userActions.append(userAction);

        //add the actions to every toolbar xmlguiclient, so these actions can be added
        //to any toolbar
        QHashIterator<QString, ToolbarEntry*> it(m_toolbarList);
        while (it.hasNext())
        {
          it.next();
          it.value()->guiClient->actionCollection()->insert(userAction);
        }
      } else
      {
  //      kDebug(24000) << "The action " << actionName << " is already present!" << endl;
        UserAction *userAction = dynamic_cast<UserAction*>(ac->action(actionName));
        if (userAction)
           userAction->setModified(true);
      }
    }

    //add all actions to the xmlguiclient of this toolbar, otherwise it will not be
    //possible to add other actions to this toolbar in the Configure Toolbar dialog
    for (int i = 0 ; i < ac->actions().count(); i++)
        toolbarGUI->actionCollection()->insert(ac->actions().value(i));

    m_tempFileList.append(tempFile);
    p_toolbar->guiClient = toolbarGUI;
    p_toolbar->name = name;
    p_toolbar->id = toolbarId;
    p_toolbar->url = url;
    p_toolbar->visible = true;
    p_toolbar->user = true; //TODO
    m_toolbarList.insert(toolbarId, p_toolbar);

//     if (!m_separateToolbars)
      toolbarGUI->setClientBuilder(ToolbarGUIBuilder::ref(mainWindow()->main()));
    mainWindow()->main()->guiFactory()->addClient(toolbarGUI);

    delete toolbarDom;
 }
 slotAdjustActions();
}

void UserToolbarsPart::slotLoadToolbar()
{
  KUrl::List urls = KFileDialog::getOpenURLs(locateLocal("data", resourceDir + "toolbars/"), "*" + Helper::toolbarExtension(), mainWindow()->main());
  if (!urls.isEmpty())
  {
    for (KUrl::List::ConstIterator it = urls.constBegin(); it != urls.constEnd(); ++it)
        slotLoadToolbarFile(*it);
  }
}

void UserToolbarsPart::slotLoadGlobalToolbar()
{
 QString globalDataDir = KGlobal::dirs()->findResourceDir("data",resourceDir + "global");
 KUrl::List urls = KFileDialog::getOpenURLs(globalDataDir + resourceDir + "toolbars/", "*" + Helper::toolbarExtension() + "\n*", mainWindow()->main());
 if (!urls.isEmpty())
 {
   for (KUrl::List::ConstIterator it = urls.constBegin(); it != urls.constEnd(); ++it)
      slotLoadToolbarFile(*it);
 }
}

bool UserToolbarsPart::slotRemoveToolbar(const QString& id)
{
  QString name = id;  // increase reference counter for this string
  ToolbarEntry *p_toolbar = m_toolbarList.value(name);
  QRegExp i18ntabnameRx("\\si18ntabname=\"[^\"]*\"");
  QRegExp idRx("\\sid=\"[^\"]*\"");
  KActionCollection *ac = mainWindow()->main()->actionCollection();
  if (p_toolbar)
  {
    KXMLGUIClient* toolbarGUI = p_toolbar->guiClient;

    if (toolbarGUI)
    {
      KAction *action;
      //check if the actions on the toolbar were modified or not
      bool actionsModified = false;
      QDomNodeList nodeList = toolbarGUI->domDocument().elementsByTagName("Action");
      for (int i = 0; i < nodeList.count(); i++)
      {
        action = ac->action(nodeList.item(i).toElement().attribute("name"));
        if (dynamic_cast<UserAction*>(action) &&
            dynamic_cast<UserAction*>(action)->isModified())
        {
            actionsModified = true;
            break;
        }
      }

      //check if the toolbar's XML GUI was modified or not
      QString s1 = p_toolbar->dom->toString();
      QString s2 = toolbarGUI->domDocument().toString();
      s1.remove(i18ntabnameRx);
      s2.remove(i18ntabnameRx);
      s1.remove(idRx);
      s2.remove(idRx);
      if (p_toolbar->nameModified)
      {
        QRegExp tabnameRx("\\stabname=\"[^\"]*\"");
        tabnameRx.indexIn(s2);
        QString name1 = tabnameRx.cap();
        name1.indexOf(" tab");
        QString name2 = name1;
        name2.remove(QRegExp("[\\s]\\([0-9]+\\)"));
        s2.replace(name1, name2);
        s2.replace(name1.toLower(), name2.toLower());
      }
      bool useToolbarGUI = true;
      if ( s1 != s2 /*|| actionsModified */)
      {
        int result;
        if (p_toolbar->url.isEmpty())
        {
          result = KMessageBox::warningYesNoCancel(mainWindow()->main(), i18n("<qt>Toolbar <b>%1</b> is new and unsaved. Do you want to save it before it is removed?</qt>").arg(p_toolbar->name),
              i18n("Save Toolbar"), KStdGuiItem::save(), KStdGuiItem::discard());
        } else
        {
          AskForSaveDlg dlg(i18n("Save Toolbar"), i18n("<qt>The toolbar <b>%1</b> was modified. Do you want to save it before it is removed?</qt>").arg(p_toolbar->name), mainWindow()->main());

          dlg.exec();
          result = dlg.status();
          switch (result)
          {
            case -1: result = KMessageBox::Cancel;
                      break;
            case  1: result = KMessageBox::Continue; //hack - this means Save
                      break;
            case  2: result = KMessageBox::Yes; //hack - this means Save As
                      break;
            case  3: result = KMessageBox::No; //this means Don't Save
                      break;
          }
        }

      KDevProject *prj = project();
      switch (result)
      {
        case KMessageBox::Yes:
             {
               bool local = true;
               if (prj && p_toolbar->url.url().startsWith(prj->projectDirectory()))
                  local = false;
                if (!saveToolbar(local, p_toolbar->id))
                    return false;
                break;
             }
        case KMessageBox::Continue:
             {
                bool local = true;
               if (prj && p_toolbar->url.url().startsWith(prj->projectDirectory()))
                  local = false;
                if (!saveToolbar(local, p_toolbar->id, p_toolbar->url))
                    return false;
                break;
             }
        case KMessageBox::No:
             {
               useToolbarGUI = false;
               break;
             }
        case KMessageBox::Cancel: return false;

      }

     }
     mainWindow()->main()->guiFactory()->removeClient(toolbarGUI);
//unplug the actions and remove them if they are not used in other places
     if (useToolbarGUI)
       nodeList = toolbarGUI->domDocument().elementsByTagName("Action");
     else
       nodeList = p_toolbar->dom->elementsByTagName("Action");
     for (int i = 0; i < nodeList.count(); i++)
     {
       action = ac->action(nodeList.item(i).toElement().attribute("name"));
       if (action && !action->isPlugged())
       {
         if (dynamic_cast<UserAction*>(action) &&
             !dynamic_cast<UserAction*>(action)->isModified())
         {
             //take out the action from every toolbar's xmlguiclient
             //this avoid a crash when removing a toolbar
            QHashIterator<QString, ToolbarEntry*> it(m_toolbarList);
            while (it.hasNext())
            {
              it.next();
              it.value()->guiClient->actionCollection()->take(action);
            }
            delete action;
         }
       }
     }
     delete p_toolbar->dom;
     delete p_toolbar->guiClient;
     m_toolbarList.remove(name);
    }
  }

  slotAdjustActions();
  emit toolbarRemoved(name);
  return true;
}


QString UserToolbarsPart::selectToolbarDialog(const QString &caption)
{
  ToolbarTabWidget *tb = ToolbarTabWidget::ref();

  QMap<QString, QString> tabData;
  for (int i = 0; i < tb->count(); ++i)
  {
    tabData.insert(tb->tabText(i), tb->id(i));
  }
  int current = tabData.keys().indexOf(tb->tabText(tb->currentIndex()));
  bool ok = FALSE;
  QString res = KInputDialog::getItem(caption, i18n( "Please select a toolbar:" ),
                      tabData.keys(), current, FALSE, &ok, mainWindow()->main() );
  if (ok)
    return tabData.value(res);
  else
    return QString::Null();
}


bool UserToolbarsPart::slotRemoveToolbar()
{
  QString toolbar = selectToolbarDialog(i18n("Remove Toolbar"));
  if (toolbar.isNull())
    return false;

  return slotRemoveToolbar(toolbar);
}

bool UserToolbarsPart::saveToolbar(bool localToolbar, const QString& toolbarToSave, const KUrl& destURL)
{
  int query;
  KUrl url;
  KUrl projectToolbarsURL;
  QString toolbarName;
  QString localToolbarsDir = locateLocal("data", resourceDir + "toolbars/");
  KDevProject *prj = project();

  if (toolbarToSave.isEmpty())
  {
    toolbarName = selectToolbarDialog(i18n("Save Toolbar"));
    if (toolbarName.isNull())
      return false;
  } else
  {
    toolbarName = toolbarToSave;
  }

  if (destURL.isEmpty())
  {
    do {
      query = KMessageBox::Yes;

      if (localToolbar)
      {
        url = KFileDialog::getSaveURL(localToolbarsDir, "*" + Helper::toolbarExtension(), mainWindow()->main());
      } else
      {
//FIXME no toolbarURL() in KDevProject. Check all hardcoded "toolbars" dir below!
        url = KFileDialog::getSaveURL(prj->projectDirectory() + "/toolbars", "*" + Helper::toolbarExtension(), mainWindow()->main());
      }

      if (url.isEmpty())
          return false;

      if (prj)
          projectToolbarsURL = KUrl::fromPathOrURL(prj->projectDirectory() + "/toolbars");
      if ( ((!localToolbar) && (projectToolbarsURL.isParentOf(url)) ) ||
            ((localToolbar) && (KUrl(localToolbarsDir).isParentOf(url))) )
      {
         if (!ExtFileInfo::checkOverwrite(url))
          query = KMessageBox::No;
      } else
      {
        if (!localToolbar)
            localToolbarsDir = projectToolbarsURL.prettyURL();
        KMessageBox::sorry(mainWindow()->main(),i18n("<qt>You must save the toolbars to the following folder: <br><br><b>%1</b></qt>")
                                  .arg(localToolbarsDir));
        query = KMessageBox::No;
      }
    } while (query != KMessageBox::Yes);
  } else
  {
    url = destURL;
    query = KMessageBox::Yes;
  }
  if( query != KMessageBox::Cancel )
  {
    KUrl tarName = saveToolbarToFile(toolbarName, url);
    if (tarName.isEmpty())
    {
      return false;
    }
/*FIXME    if (!localToolbar)
        Project::ref()->insertFile(tarName, true);*/
  }
  return true;
}

KUrl UserToolbarsPart::saveToolbarToFile(const QString& toolbarName, const KUrl& destFile)
{
  KUrl tarFile = destFile;

  if (!destFile.fileName().endsWith(Helper::toolbarExtension()))
  {
    tarFile.setFileName(destFile.fileName() + Helper::toolbarExtension());
  }

  QBuffer buffer;
  buffer.open(IO_ReadWrite);
  QString toolStr;
  QTextStream toolStream(&toolStr, IO_ReadWrite);
  toolStream.setCodec(QTextCodec::codecForName("UTF-8"));

  QBuffer buffer2;
  buffer2.open(IO_WriteOnly);
  QTextStream actStr(&buffer2);
  actStr.setCodec(QTextCodec::codecForName("UTF-8"));

  QDomNodeList nodeList, nodeList2;

  toolStream << "<!DOCTYPE kpartgui SYSTEM \"kpartgui.dtd\">\n<kpartgui name=\"quanta\" version=\"2\">\n";
  actStr << QString("<!DOCTYPE actionsconfig>\n<actions>\n");

//look up the clients
  KActionCollection *ac = mainWindow()->main()->actionCollection();
  QList<KXMLGUIClient*> xml_clients = factory()->clients();
  for (int index = 0; index < xml_clients.count(); index++)
  {
      nodeList = xml_clients.at(index)->domDocument().elementsByTagName("ToolBar");
      for (int i = 0; i < nodeList.count(); i++)
      {
      //find the actual toolbar in the XML GUI
        if ((nodeList.item(i).cloneNode().toElement().attribute("id") ) == toolbarName)
        {

          //find the actions registered to the toolbar
          QDomNode n = nodeList.item(i).firstChild();
          while (! n.isNull())
          {
           QDomElement e = n.toElement();
           if (e.tagName() == "Action")
           {
             UserAction *action = dynamic_cast<UserAction*>(ac->action(e.attribute("name")));
             if (action)
             {
               action->data().save(actStr,1);
               action->setModified(false);
             }
           } else
           if (e.tagName() == "_Separator_")
           {
             e.setTagName("Separator");
           }
           n = n.nextSibling();
          }
          QDomElement e = nodeList.item(0).toElement();
          QString i18nName = e.attribute("i18ntabname");
          QString id = e.attribute("id");
          e.removeAttribute("i18ntabname");
          e.removeAttribute("id");
          nodeList.item(i).save(toolStream,2);
          e.setAttribute("i18ntabname", i18nName);
          e.setAttribute("id", id);
        }
      }
  }
  toolStream << QString("\n</kpartgui>");
  actStr << QString("\n</actions>");
  //buffer.flush();

  ToolbarEntry *p_toolbar = m_toolbarList.value(toolbarName);
  QDomDocument *oldDom = p_toolbar->dom;
  QDomDocument *dom = new QDomDocument();
  QString s = toolStr;
  QString error;
  int el, ec;
  if (!dom->setContent(s, &error, &el, &ec))
      kError(24000) << QString("Error %1 at (%2, %3)").arg(error).arg(el).arg(ec)<<endl;
  p_toolbar->dom = dom;

  QTextStream bufferStr(&buffer);
  bufferStr.setCodec(QTextCodec::codecForName("UTF-8"));
  bufferStr << toolStr;
  buffer.close();
  buffer2.close();

  KTempFile *tempFile = new KTempFile(m_tmpDir);
  tempFile->setAutoDelete(true);
  tempFile->close();
  KTar tar(tempFile->name(), "application/x-gzip");
  if (!tar.open(IO_WriteOnly))
      return KUrl();
  if (!tar.writeFile(QFileInfo(tarFile.path()).baseName() + ".toolbar", "user", "group", buffer.buffer().data(), buffer.buffer().size()))
      return KUrl();
  if (!tar.writeFile(QFileInfo(tarFile.path()).baseName() + ".actions", "user", "group", buffer2.buffer().data(), buffer2.buffer().size()))
      return KUrl();
  tar.close();
  if (!ExtFileInfo::copy(KUrl::fromPathOrURL(tempFile->name()), tarFile, -1, true, false, mainWindow()->main()))
  {
    KMessageBox::error(mainWindow()->main(), i18n("<qt>An error happened while saving the <b>%1</b> toolbar.<br>"
        "Check that you have write permissions for<br><b>%2</b></qt>").arg(p_toolbar->name).arg(tarFile.pathOrURL()), i18n("Toolbar Saving Error"));
    tarFile = KUrl();
    delete p_toolbar->dom;
    p_toolbar->dom = oldDom;
  } else
    delete oldDom;
  return tarFile;
}

void UserToolbarsPart::slotAddToolbar()
{
 bool ok;
 QString name = KInputDialog::getText(i18n("New Toolbar"), i18n("Enter toolbar name:"), i18n("User_%1").arg(m_userToolbarsCount), &ok, mainWindow()->main());
 if (ok)
 {
  m_userToolbarsCount++;

  QString toolbarId = name;
  int n = 1;
  while (m_toolbarList.value(toolbarId) != 0L)
  {
    toolbarId = name + QString("%1").arg(n);
    n++;
  }
  toolbarId = toolbarId.toLower();

  KTempFile* tempFile = new KTempFile(m_tmpDir);
  tempFile->setAutoDelete(true);
  tempFile->textStream()->setCodec(QTextCodec::codecForName("UTF-8"));
  * (tempFile->textStream()) << QString("<!DOCTYPE kpartgui SYSTEM \"kpartgui.dtd\">\n<kpartgui name=\"quanta\" version=\"2\">\n<ToolBar name=\"%1\" tabname=\"%2\" i18ntabname=\"%3\" id=\"%4\">\n<text>%5</text>\n</ToolBar>\n</kpartgui>\n")
      .arg(name.toLower()).arg(name).arg(name).arg(toolbarId).arg(name);
  tempFile->close();

  ToolbarXMLGUI * toolbarGUI = new ToolbarXMLGUI(tempFile->name());
  KActionCollection *ac = mainWindow()->main()->actionCollection();

//add all actions to the xmlguiclient of this toolbar
  for (int i = 0 ; i < ac->actions().count(); i++)  // FIXME can we add the whole collection at once?
     toolbarGUI->actionCollection()->insert(ac->actions().value(i));

  m_tempFileList.append(tempFile);
  ToolbarEntry *p_toolbar = new ToolbarEntry;
  p_toolbar->guiClient = toolbarGUI;

  QDomDocument *dom = new QDomDocument(toolbarGUI->domDocument());

  p_toolbar->dom = dom;
  p_toolbar->name = name;
  p_toolbar->user = true;
  p_toolbar->visible = true;
  p_toolbar->nameModified = false;
  p_toolbar->menu = new QMenu;
  p_toolbar->id = toolbarId;
  m_toolbarList.insert(toolbarId, p_toolbar);

//   if (!m_separateToolbars)
    toolbarGUI->setClientBuilder(ToolbarGUIBuilder::ref(mainWindow()->main()));
  mainWindow()->main()->guiFactory()->addClient(toolbarGUI);
  ToolbarTabWidget::ref()->setCurrentIndex(ToolbarTabWidget::ref()->count()-1);

  slotAdjustActions();
 }
}

void UserToolbarsPart::slotRenameToolbar()
{
  QString toolbar = selectToolbarDialog(i18n("Rename Toolbar"));
  if (!toolbar.isNull())
    slotRenameToolbar(toolbar);
}

void UserToolbarsPart::slotRenameToolbar(const QString& id)
{
  ToolbarEntry *p_toolbar = m_toolbarList.value(id);
  if (p_toolbar)
  {
    KMainWindow *mw = mainWindow()->main();
    bool ok;
    QString newName = KInputDialog::getText(i18n("Rename Toolbar"), i18n("Enter the new name:"), p_toolbar->name, &ok, mw);
    if (ok && newName != p_toolbar->name)
    {
      kDebug(24000) << "p_toolbar->guiClient before rename:" <<  p_toolbar->guiClient->domDocument().toString() << endl;
      m_toolbarList.take(id);
      p_toolbar->name = newName;
      QDomElement el = p_toolbar->guiClient->domDocument().firstChild().firstChild().toElement();
      el.setAttribute("tabname", p_toolbar->name);
      el.removeAttribute("i18ntabname");
      el.setAttribute("name", p_toolbar->name.toLower());
      QDomNodeList nodeList = p_toolbar->guiClient->domDocument().elementsByTagName("text");
      nodeList.item(0).firstChild().setNodeValue(p_toolbar->name);
    //Rename the _Separator_ tags back to Separator, so they are not treated
    //as changes
      nodeList = p_toolbar->guiClient->domDocument().elementsByTagName("_Separator_");
      for (int i = 0; i < nodeList.count(); i++)
      {
        nodeList.item(i).toElement().setTagName("Separator");
      }
      KXMLGUIFactory::saveConfigFile(p_toolbar->guiClient->domDocument(),
          p_toolbar->guiClient->xmlFile(), p_toolbar->guiClient->instance());
      ToolbarTabWidget *tb = ToolbarTabWidget::ref();
      QMenu *actionsMenu = static_cast<QMenu*>(factory()->container("actions", this));
/*      if (m_separateToolbars)
      {
        KToolBar *toolbar = dynamic_cast<KToolBar*>(mainWindow()->main()->factory()->container(id,  p_toolbar->guiClient));
        if (toolbar)
          toolbar->setTitle(i18n(p_toolbar->name.toUtf8()));
      }// else*/
      {
        for (int i = 0; i < tb->count(); i++)
        {
          if (tb->id(i) == id)
          {
            tb->setTabText(tb->indexOf(tb->page(id)->parentWidget()), i18n(p_toolbar->name.toUtf8()));
            actionsMenu->changeItem(actionsMenu->idAt(i), i18n(p_toolbar->name.toUtf8()));
/*            if (m_separateToolbars)
            {
              mw->guiFactory()->removeClient(p_toolbar->guiClient);
              mw->guiFactory()->addClient(p_toolbar->guiClient);
             // dynamic_cast<KToolBar*>(tb->page(id)->parentWidget())->setTitle(i18n(p_toolbar->name.toUtf8()));
            }*/
            break;
          }
        }
      }
      KXMLGUIFactory::readConfigFile(
          p_toolbar->guiClient->xmlFile(), p_toolbar->guiClient->instance());
      m_toolbarList.insert(id, p_toolbar);
kDebug(24000) << "p_toolbar->guiClient after rename:" <<  p_toolbar->guiClient->domDocument().toString() << endl;

    }
  }
}

QString UserToolbarsPart::createToolbarTarball()
{
  QString toolbar = selectToolbarDialog(i18n("Save Toolbar"));
  if (toolbar.isNull())
    return QString::Null();
  QString prefix="quanta";
  KTempDir* tempDir = new KTempDir(m_tmpDir);
  tempDir->setAutoDelete(true);
  m_tempDirList.append(tempDir);
  QString tempFileName = tempDir->name() + toolbar;

  KUrl tempURL;
  tempURL.setPath(tempFileName);
  saveToolbarToFile(toolbar, tempURL);

  return tempFileName + ".toolbar.tgz";
}

void UserToolbarsPart::slotSendToolbar()
{
  QString tempFileName = createToolbarTarball();
  if (tempFileName.isNull())
    return;

  QStringList toolbarFile;
  toolbarFile += tempFileName;

  MailDialog *mailDlg = new MailDialog( mainWindow()->main(), i18n("Send toolbar in email"));
  QString toStr;
  QString message = i18n("Hi,\n This is a KDevelop/Quanta Plus [http://www.kdevelop.org, http://quanta.kdewebdev.org] toolbar.\n\nHave fun.\n");
  QString titleStr;
  QString subjectStr;

  mailDlg->TitleLabel->setText(i18n("Content:"));
  mailDlg->titleEdit->setFixedHeight(60);
  if ( mailDlg->exec() ) {
    if ( !mailDlg->lineEmail->text().isEmpty())
    {
      toStr = mailDlg->lineEmail->text();
      subjectStr = (mailDlg->lineSubject->text().isEmpty())?i18n("KDevelop/Quanta Plus toolbar"):mailDlg->lineSubject->text();
      if ( !mailDlg->titleEdit->text().isEmpty())
        message = mailDlg->titleEdit->text();
    } else
    {
      KMessageBox::error(mainWindow()->main(),i18n("No destination address was specified.\n Sending is aborted."),i18n("Error Sending Email"));
      delete mailDlg;
      return;
    }

    KToolInvocation::invokeMailer(toStr, QString::null, QString::null, subjectStr, message, QString::null, toolbarFile);
  }
  delete mailDlg;
}


//FIXME: this should be called before the application exits and should abort
//the exit if the toolbars are not saved!
bool UserToolbarsPart::removeToolbars()
{
  QHashIterator<QString, ToolbarEntry*> it(m_toolbarList);
  while (it.hasNext())
  {
    it.next();
    if (!slotRemoveToolbar(it.value()->id))
        return false;
  }

  KActionCollection *ac = mainWindow()->main()->actionCollection();
  QString s = "<!DOCTYPE actionsconfig>\n<actions>\n</actions>\n";
  QDomDocument actions;
  actions.setContent(s);
  UserAction *action;
  for (int i = 0; i < ac->actions().count(); i++)
  {
    action = dynamic_cast<UserAction *>(ac->actions().value(i));
    if (action)
    {
      QDomElement el = action->data();
      actions.firstChild().appendChild(el);
    }
  }

  //save the user actions that aren't on any toolbar, so they are not lost
  QFile f(KGlobal::instance()->dirs()->saveLocation("data") + resourceDir + "actions.rc" );
  if (f.open( IO_ReadWrite | IO_Truncate ))
  {
    if (!actions.firstChild().firstChild().isNull())
    {
      QTextStream qts(&f);
      qts.setCodec(QTextCodec::codecForName("UTF-8"));
      actions.save(qts,0);
      f.close();
    } else
    f.remove();
  }

 return true;
}

void UserToolbarsPart::slotDownloadToolbar()
{
  if (!m_newToolbarStuff)
    m_newToolbarStuff = new NewToolbarStuff("kdevelop/toolbar", mainWindow()->main());
  m_newToolbarStuff->downloadResource();
}

void UserToolbarsPart::slotUploadToolbar()
{
  QString tempFileName = createToolbarTarball();
  if (tempFileName.isNull())
    return;
  if (!m_newToolbarStuff)
    m_newToolbarStuff = new NewToolbarStuff("kdevelop/toolbar", mainWindow()->main());
  m_newToolbarStuff->uploadResource(tempFileName);
}

void UserToolbarsPart::slotConfigureToolbars(const QString &defaultToolbar)
{
 ToolbarTabWidget *tb = ToolbarTabWidget::ref();
 KMainWindow *mw = mainWindow()->main();
 m_currentTabPage = tb->currentIndex();
 QDomNodeList nodeList;
 mw->saveMainWindowSettings(KGlobal::config(), mw->autoSaveGroup());
 KEditToolbar dlg(defaultToolbar, mw->factory(), mw);
 connect(&dlg, SIGNAL(newToolbarConfig()), SLOT(slotNewToolbarConfig()));
 dlg.exec();
 tb->setCurrentIndex(m_currentTabPage);
}

void UserToolbarsPart::slotNewToolbarConfig()
{
  mainWindow()->main()->applyMainWindowSettings(KGlobal::config(), mainWindow()->main()->autoSaveGroup());
  ToolbarTabWidget::ref()->setCurrentIndex(m_currentTabPage);
}

void UserToolbarsPart::slotToolbarLoaded(const QString &id)
{
  ToolbarEntry *p_toolbar = m_toolbarList.value(id);
  if (!p_toolbar || !m_createActionsMenu)
    return;
  //Plug in the actions & build the menu
  QMenu *menu = new QMenu;
  menu->setTitle(i18n(p_toolbar->name.toUtf8()));
  KAction *action;
  KActionCollection *ac = p_toolbar->guiClient->actionCollection();
  QDomNodeList nodeList = p_toolbar->guiClient->domDocument().elementsByTagName("Action");
  for (int i = 0; i < nodeList.count(); i++)
  {
    action = ac->action(nodeList.item(i).cloneNode().toElement().attribute("name") );
    if (action)
    {
      menu->addAction(action);
    }
  }

  QMenu *actionsMenu = static_cast<QMenu*>(factory()->container("actions", this));

  actionsMenu->addMenu(menu);
  actionsMenu->setVisible(true);
/*  if (m_actionsMenuId == -1)
  {
    KMenuBar *menuBar = mainWindow()->main()->menuBar();
    for (uint i = 0; i < menuBar->count(); i++)
    {
      QMenuItem *it = menuBar->findItem(menuBar->idAt(i));
      if (it->menu() == actionsMenu)
      {
        m_actionsMenuId = menuBar->idAt(i);
        break;
      }
    }
  }
  if (m_actionsMenuId != -1)
     mainWindow()->main()->menuBar()->setItemVisible(m_actionsMenuId, true);*/
  p_toolbar->menu = menu;
}

void UserToolbarsPart::slotToolbarRemoved(const QString &id)
{
  ToolbarEntry *p_toolbar = m_toolbarList.value(id);
  if (p_toolbar || !m_createActionsMenu)
  {
    delete p_toolbar->menu;
    p_toolbar->menu = 0L;
  }
  QMenu *actionsMenu = static_cast<QMenu*>(factory()->container("actions", this));
  if (actionsMenu->actions().count() == 0)
  {
    actionsMenu->setVisible(false);
  }
}

void UserToolbarsPart::slotEditAction(const QString& actionName)
{
  ActionConfigDialog dlg(m_toolbarList, this, "actions_config_dlg", true, 0, actionName);
  dlg.exec();
}

void UserToolbarsPart::slotNewAction()
{
  ActionConfigDialog dlg(m_toolbarList, this, "actions_config_dlg");
  dlg.slotNewAction();
  dlg.exec();
}

void UserToolbarsPart::slotRemoveAction(const QString& id, const QString& a_actionName)
{
  KAction *action = 0L;
  QString actionName = a_actionName;
  actionName.replace('&',"&&");
  KActionCollection *ac = mainWindow()->main()->actionCollection();
  int actionCount = ac->actions().count();
  QString str;
  for (int i = 0; i < actionCount; i++)
  {
    str = ac->actions().value(i)->text();
    if (str == actionName || str.remove('&') == actionName)
    {
      action = ac->actions().value(i);
      break;
    }
  }
  // FIXME I think this loop can get merged with the previous one?
  if (!action) //workaround for actionnames ending with "...". It's stripped from the end
              //of the text when plugged into a toolbar.
  {
    actionName += "...";
    for (int i = 0; i < actionCount; i++)
    {
      if (ac->actions().value(i)->text() == actionName)
      {
        action = ac->actions().value(i);
        break;
      }
    }
  }

  if (action)
  {
    ToolbarEntry *p_toolbar = m_toolbarList.value(id);
    if (p_toolbar)
    {
      QDomNode node = p_toolbar->guiClient->domDocument().firstChild().firstChild().firstChild();
      while (!node.isNull())
      {
        if (node.nodeName() == "Action" &&
            node.toElement().attribute("name") == action->objectName())
        {
          ToolbarTabWidget::ref()->page(id)->removeAction(action);
          p_toolbar->menu->removeAction(action);
          node.parentNode().removeChild(node);
        }
        node = node.nextSibling();
      }
      KXMLGUIFactory::saveConfigFile(p_toolbar->guiClient->domDocument(),
        p_toolbar->guiClient->xmlFile(), p_toolbar->guiClient->instance());
    }
  }
}

void UserToolbarsPart::slotConfigureActions()
{
  ActionConfigDialog dlg(m_toolbarList, this, "actions_config_dlg", true);
  dlg.exec();
}

void UserToolbarsPart::slotDeleteAction(KAction *action)
{
//remove all references to this action
  QDomElement el = static_cast<UserAction*>(action)->data();
  QString text = el.attribute("text");
  QString actionName = action->objectName();

  QList<KXMLGUIClient*> guiClients = mainWindow()->main()->factory()->clients();
  KXMLGUIClient *guiClient = 0;
  QDomNodeList nodeList;
  for (int i = 0; i < guiClients.count(); i++)
  {
    guiClient = guiClients.at(i);
    guiClient->domDocument().setContent(KXMLGUIFactory::readConfigFile( guiClient->xmlFile(), guiClient->instance() ));
    nodeList = guiClient->domDocument().elementsByTagName("Action");
    for (int j = 0; j < nodeList.count(); j++)
    {
      //we found a toolbar that contains the action
      if (nodeList.item(j).toElement().attribute("name") == actionName)
      {
        nodeList.item(j).parentNode().removeChild(nodeList.item(j));
        KXMLGUIFactory::saveConfigFile(guiClient->domDocument(), guiClient->xmlFile());
        break;
      }
    }
    guiClient->actionCollection()->take(action);
  }
  delete action;
  action = 0L;
}

void UserToolbarsPart::slotShowOutputView()
{
  if (!m_outputPlugin)
  {
    m_outputPlugin = extension<KDevAppFrontend>("KDevelop/AppFrontend");
  }
  if (m_outputPlugin)
  {
    m_outputPlugin->clearView();
  }
}

void UserToolbarsPart::slotShowMessage(const QString &message, bool append)
{
  Q_UNUSED(append)
  if (m_outputPlugin)
  {
    m_outputPlugin->insertStderrLine(message);
  }
}

void UserToolbarsPart::saveConfig()
{
  KConfig *config = UserToolbarsFactory::instance()->config();
  config->setGroup("General");
  config->writeEntry("Separate toolbars", m_separateToolbars);
  config->writeEntry("Create Actions menu", m_createActionsMenu);
  config->sync();
}


void UserToolbarsPart::go()
{
  QMessageBox::information(0, "test", "go go go");
}

#include "usertoolbarspart.moc"