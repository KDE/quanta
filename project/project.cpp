/***************************************************************************
                          project.cpp  -  description
                             -------------------
    begin                : Thu Mar 16 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2001-2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "project.h"

// unix includes
#include <time.h>
#include <unistd.h>

// include QT files
#include <qdict.h>
#include <qdom.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qfileinfo.h>
#include <qtabdialog.h>
#include <qfiledialog.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <qradiobutton.h>
#include <qwidgetstack.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qregexp.h>
#include <qdict.h>

// include files for KDE
#include <kdebug.h>
#include <kdirwatch.h>
#include <kurl.h>
#include <kfile.h>
#include <kcharsets.h>
#include <kwizard.h>
#include <klocale.h>
#include <kaction.h>
#include <kactionclasses.h>
#include <kapplication.h>
#include <ktempfile.h>
#include <kstdaction.h>
#include <kstandarddirs.h>
#include <kfiledialog.h>
#include <kinputdialog.h>
#include <kmessagebox.h>
#include <kprogress.h>
#include <kio/job.h>
#include <kio/netaccess.h>
#include <klineedit.h>
#include <kurlrequesterdlg.h>
#include <kurlrequester.h>
#include <kprotocolinfo.h>
#include <kpassdlg.h>
#include <kstringhandler.h>
#include <kdeversion.h>
#include <kmainwindow.h>
#include <kparts/componentfactory.h>
#include <kinputdialog.h>
#include <kactioncollection.h>
#include <kiconloader.h>
#include <kxmlguifactory.h>

// application headers
#include "copyto.h"
#include "dtdselectdialog.h"
#include "qextfileinfo.h"
#include "projectnewgeneral.h"
#include "projectnewlocal.h"
#include "projectnewweb.h"
#include "projectnewfinal.h"
#include "projectupload.h"
#include "projectoptions.h"
#include "uploadprofilespage.h"
#include "rescanprj.h"
#include "resource.h"
#include "document.h"
#include "quantacommon.h"
#include "dtds.h"
#include "debuggerclient.h"
#include "viewmanager.h"
#include "projectlist.h"
#include "projectprivate.h"
#include "teammembersdlg.h"
#include "eventconfigurationdlg.h"
#include "qpevents.h"
#include "uploadprofiles.h"
#include "quanta.h"

extern QString simpleMemberStr;
extern QString taskLeaderStr;
extern QString teamLeaderStr;
extern QString subprojectLeaderStr;

ProjectPrivate::ProjectPrivate(Project *p)
          : QObject(), config(0L), m_dirWatch(0L), tempFile(0L)
{
  parent = p;
  m_projectFiles.setAutoDelete(true);
  m_showUploadTreeviews = true;
  m_events = new EventActions();
  init();
  m_wizTitle = i18n("<b>Insert Files in Project</b>");
}

ProjectPrivate::~ProjectPrivate()
{
  delete m_events;
}

/** setup of the actions */
void ProjectPrivate::initActions(KActionCollection *ac)
{
  (void) new KAction( i18n( "&New Project..." ), "window_new", 0,
                        this, SLOT( slotNewProject() ),
                        ac, "project_new" );

  (void) new KAction( i18n( "&Open Project..." ), "project_open", 0,
                        this, SLOT( slotOpenProject() ),
                        ac, "project_open" );
  projectRecent =
      KStdAction::openRecent(parent, SLOT(slotOpenProject(const KURL&)),
                            ac, "project_open_recent");
  projectRecent->setText(i18n("Open Recent Project"));
  projectRecent->setIcon("project_open");
  projectRecent->setMaxItems(32);
  projectRecent->setToolTip(i18n("Open/Open recent project"));
  connect(projectRecent, SIGNAL(activated()), this, SLOT(slotOpenProject()));

  closeprjAction =  new KAction( i18n( "&Close Project" ), "fileclose", 0,
                        this, SLOT( slotCloseProject() ),
                        ac, "project_close" );


  openPrjViewAction = new KSelectAction( i18n( "Open Project &View..." ), 0,
                        ac, "project_view_open" );
  connect(openPrjViewAction, SIGNAL(activated(const QString &)),
          this, SLOT(slotOpenProjectView(const QString &)));
  openPrjViewAction->setToolTip(i18n("Open Project View"));

  savePrjViewAction = new KAction( i18n( "&Save Project View" ), "filesave", 0,
                        this, SLOT( slotSaveProjectView() ),
                        ac, "project_view_save" );
  saveAsPrjViewAction = new KAction( i18n( "Save Project View &As..." ), "filesaveas", 0,
                        this, SLOT( slotSaveAsProjectView() ),
                        ac, "project_view_save_as" );
  deletePrjViewAction = new KSelectAction( i18n( "&Delete Project View" ), "editdelete", 0,
                        ac, "project_view_delete" );
  connect(deletePrjViewAction, SIGNAL(activated(const QString &)),
          this, SLOT(slotDeleteProjectView(const QString &)));
  deletePrjViewAction->setToolTip(i18n("Close Project View"));



  insertFileAction = new KAction( i18n( "&Insert Files..." ), 0,
                        this, SLOT( slotAddFiles() ),
                        ac, "project_insert_file" );

  insertDirAction = new KAction( i18n( "Inser&t Directory..." ), 0,
                        this, SLOT( slotAddDirectory() ),
                        ac, "project_insert_directory" );

  rescanPrjDirAction = new KAction( i18n( "&Rescan Project Folder..." ), "reload", 0,
                        parent, SLOT( slotRescanPrjDir() ),
                        ac, "project_rescan" );

  uploadProjectAction = new KAction( i18n( "&Upload Project..." ), "up", Key_F8,
                        parent, SLOT( slotUpload() ),
                        ac, "project_upload" );

  projectOptionAction = new KAction( i18n( "&Project Properties" ), "configure", SHIFT + Key_F7,
                        parent, SLOT( slotOptions() ),
                        ac, "project_options" );

  saveAsProjectTemplateAction =
            new KAction( i18n( "Save as Project Template..." ), 0,
                        m_parent, SLOT( slotFileSaveAsProjectTemplate() ),
                        ac, "save_project_template" );

  saveSelectionAsProjectTemplateAction =
                        new KAction( i18n( "Save Selection to Project Template File..." ), 0,
                                    m_parent, SLOT( slotFileSaveSelectionAsProjectTemplate() ),
                                    ac, "save_selection_project_template" );
  adjustActions();
}


void ProjectPrivate::adjustActions()
{
  bool projectExists = parent->hasProject();
  closeprjAction->setEnabled(projectExists);
  openPrjViewAction->setEnabled(projectExists);
  savePrjViewAction->setEnabled(projectExists);
  saveAsPrjViewAction->setEnabled(projectExists);
  deletePrjViewAction->setEnabled(projectExists);

  insertFileAction->setEnabled(projectExists);
  insertDirAction->setEnabled(projectExists);
  rescanPrjDirAction->setEnabled(projectExists);
  uploadProjectAction->setEnabled(projectExists);
  projectOptionAction->setEnabled(projectExists);
  saveAsProjectTemplateAction->setEnabled(projectExists);
  saveSelectionAsProjectTemplateAction->setEnabled(projectExists);

  adjustViewActions();
  parent->slotShowProjectToolbar(projectExists);
}


void ProjectPrivate::adjustViewActions()
{
  QStringList viewList = projectViewList();
  QString oldItem = openPrjViewAction->currentText();
  openPrjViewAction->clear();
  openPrjViewAction->setItems(viewList);
  int i = viewList.findIndex(oldItem);
  if (i > -1)
    openPrjViewAction->setCurrentItem(i);
  deletePrjViewAction->clear();
  deletePrjViewAction->setItems(viewList);
  bool hasView = !currentProjectView.isEmpty();
  savePrjViewAction->setEnabled(hasView);
}


QStringList ProjectPrivate::projectViewList()
{
  QStringList list;
  QDomNodeList nl = dom.elementsByTagName("projectview");
  QDomElement el;
  for (uint i = 0; i < nl.count(); i++)
  {
    el = nl.item(i).cloneNode().toElement();
    list += el.attribute("name");
  }
  list.sort();
  return list;
}

void ProjectPrivate::init()
{
  projectURL  = KURL();
  templateURL = KURL();
  projectName = QString::null;
  m_modified = false;
  m_defaultDTD = qConfig.defaultDocType;
  excludeRx.setPattern(".*~$");
  excludeList.clear();
  excludeList.append("*~");
  usePreviewPrefix = false;
  previewPrefix = KURL();
  m_excludeCvsignore = false;
  currentProjectView = QString::null;
  m_projectFiles.clear();
  m_mailingList = QString::null;
  m_teamLeader.name = QString::null;
  m_teamLeader.email = QString::null;
  m_taskLeaders.clear();
  m_subprojectLeaders.clear();
  m_subprojects.clear();
  m_simpleMembers.clear();
  UploadProfiles::ref()->clear();
}


void ProjectPrivate::openCurrentView()
{
  if (currentProjectView.isEmpty())
    return;
  KURL::List urlsToOpen, urlsInView;
  QDomNodeList nl = dom.elementsByTagName("projectview");
  QDomElement el;
  for (uint i = 0; i < nl.count(); i++)
  {
    el = nl.item(i).cloneNode().toElement();
    if (el.attribute("name") == currentProjectView)
    {
      QDomNodeList itemNodes = el.childNodes();
      for (int j = itemNodes.count()-1; j >= 0 ; j--)
      {
        QDomElement el2 = itemNodes.item(j).cloneNode().toElement();
        KURL url = baseURL;
        QuantaCommon::setUrl(url,el2.attribute("url"));
        url = QExtFileInfo::toAbsolute(url, baseURL);
        if (el2.nodeName() == "viewitem")
        {
          urlsInView.append(url);
          if (! ViewManager::ref()->isOpened(url))
            urlsToOpen.append(url);
        } else {
          if (el2.nodeName() == "viewtoolbar")
          {
            parent->loadToolbarFile(url);
          }
        }
      }
      // first we open what we want, might be that a wanted file is already open!
      parent->openFiles(urlsToOpen, m_defaultEncoding);
      // second we close what we don't want
      KURL::List openURLs = ViewManager::ref()->openedFiles(true); // get open urls
      KURL::List::Iterator it;
      for ( it = openURLs.begin(); it != openURLs.end(); ++it )
      {
        if (urlsInView.findIndex( *it ) == -1)
          parent->closeFile (*it);
      }
      break;
    }
  }
  QStringList viewList = projectViewList();
  int i = viewList.findIndex(currentProjectView);
  if (i > -1)
    openPrjViewAction->setCurrentItem(i);
  adjustViewActions();
}

/** Opens a project view (toolbars & files). */
void ProjectPrivate::slotOpenProjectView(const QString &view)
{
  currentProjectView = view;
  openCurrentView();
}

/** insert files */
void ProjectPrivate::insertFiles( KURL::List files )
{
  QDomElement  el;
  QDomNodeList nl = dom.elementsByTagName("item");
  parent->statusMsg( i18n("Adding files to the project...") );
  progressBar->setTotalSteps(2 * files.count() - 2);
  progressBar->setValue(0);
  progressBar->setTextEnabled(true);

  KURL::List::Iterator it;
  for ( it = files.begin(); it != files.end(); ++it )
  {
    if (m_projectFiles.contains(*it))
    {
        it = files.erase(it);
        --it;
    }
    progressBar->advance(1);
  }
  for ( it = files.begin(); it != files.end(); ++it )
  {
    if (! (*it).isEmpty())
    {
      KURL url = *it;
      url.setPath(url.directory(false));
      while ( baseURL.isParentOf(url) )
      {
        if (!m_projectFiles.contains(url))
        {
          el = dom.createElement("item");
          el.setAttribute("url", QuantaCommon::qUrl(QExtFileInfo::toRelative(url, baseURL)));
          dom.firstChild().firstChild().appendChild(el);
          m_projectFiles.insert( new ProjectURL(url, "", 1, false, el) );
          emit eventHappened("after_project_add", url.url(), QString::null);
          m_modified = true;
        }
        url.setPath(url.directory(false));
      }
      el = dom.createElement("item");
      url = *it;
      if (!excludeRx.exactMatch(url.path()))
      {
        el.setAttribute("url", QuantaCommon::qUrl(QExtFileInfo::toRelative(url, baseURL)));
        dom.firstChild().firstChild().appendChild(el);
        m_projectFiles.insert( new ProjectURL(url, "", 1, false, el) );
        emit eventHappened("after_project_add", url.url(), QString::null);
        m_modified = true;
      }
    }
    progressBar->advance(1);
  }
  progressBar->setTotalSteps(1);
  progressBar->setValue(0);
  progressBar->setTextEnabled(false);

  parent->reloadTree(&(m_projectFiles), false, QStringList());
  parent->newStatus();
  parent->statusMsg(QString::null);
}


/** insert files from dir recursive */
void ProjectPrivate::insertFiles(const KURL& pathURL, const QString& mask )
{
  KURL::List list;

  list.append(pathURL);
  list += QExtFileInfo::allFiles( pathURL, mask );
  insertFiles(list);
}


void ProjectPrivate::loadProjectXML()
{
//TODO: Optimize reading. For example iterate through all the nodes and handle them
//according to the found node type
  parent->statusMsg( i18n("Reading the project file...") );
  QDomNode    no;
  QDomElement el;
  KURL url;
  QDomNode projectNode = dom.firstChild().firstChild();
  projectName = projectNode.toElement().attribute("name");

  if ( projectNode.isNull() || projectName.isEmpty() )
  {
    parent->hideSplash();
    KMessageBox::sorry(m_parent, i18n("Invalid project file.") );
    adjustActions();
    return;
  }

  m_modified = false;
  QString tmpString = projectNode.toElement().attribute("previewPrefix");
  if ( !tmpString.isEmpty())
  {
    previewPrefix = KURL::fromPathOrURL( tmpString );
  }
  usePreviewPrefix = ( projectNode.toElement().attribute("usePreviewPrefix") == "1");
  m_defaultEncoding = projectNode.toElement().attribute("encoding");
  if (m_defaultEncoding.isEmpty())
  {
    m_defaultEncoding = qConfig.defaultEncoding;
  }
  no = projectNode.namedItem("author");
  author = no.firstChild().nodeValue();
  no = projectNode.namedItem("email");
  email = no.firstChild().nodeValue();
  no = projectNode.namedItem("defaultDTD");
  m_defaultDTD = no.firstChild().nodeValue();
  if (m_defaultDTD.isEmpty()) m_defaultDTD = qConfig.defaultDocType;

  no = projectNode.namedItem("autoload");
  currentProjectView = no.toElement().attribute("projectview");
  if (currentProjectView.isEmpty())
  {
    QStringList list = projectViewList();
    if (list.count() > 0)
      currentProjectView = list[0];
  }
  // Debugger
  no = projectNode.namedItem("debuggerclient");
  debuggerClient = no.firstChild().nodeValue();

  no = projectNode.namedItem("templates");
  tmpString = no.firstChild().nodeValue();
  templateURL = baseURL;
  if(no.isNull()) // compatability
  {
    templateURL.setPath("templates/");
    m_modified = true;
  }
  else
  {
    QuantaCommon::setUrl(templateURL, tmpString);
  }
  if (tmpString != QuantaCommon::qUrl(templateURL) )
  {
    el = no.toElement();
    url = QExtFileInfo::toRelative(templateURL, baseURL);
    if(el.isNull())
    {
      el = dom.createElement("templates");
      dom.firstChild().firstChild().appendChild(el);
      el.appendChild(dom.createTextNode(QuantaCommon::qUrl(url)));
    }
    else
    {
      el.firstChild().setNodeValue(QuantaCommon::qUrl(url));
    }
  }
  templateURL = QExtFileInfo::toAbsolute(templateURL, baseURL);

  no = projectNode.namedItem("toolbars");
  toolbarURL = baseURL;
  if (no.isNull()) // compatability
  {
    toolbarURL.setPath(baseURL.path(1) + "toolbars/");
    m_modified = true;
  }
  else
  {
    QuantaCommon::setUrl(toolbarURL,no.firstChild().nodeValue());
  }
  if (tmpString != QuantaCommon::qUrl(toolbarURL))
  {
    el = no.toElement();
    url = QExtFileInfo::toRelative(toolbarURL, baseURL);
    if(el.isNull())
    {
      el = dom.createElement("toolbars");
      dom.firstChild().firstChild().appendChild(el);
      el.appendChild(dom.createTextNode(QuantaCommon::qUrl(url)));
    }
    else
    {
      el.firstChild().setNodeValue(QuantaCommon::qUrl(url));
    }
  }
  toolbarURL = QExtFileInfo::toAbsolute(toolbarURL, baseURL);

  no = projectNode.namedItem("exclude");
  m_excludeCvsignore = (no.toElement().attribute("cvsignore", "false") == "true");
  QString excludeStr = no.firstChild().nodeValue();
  QString regExpStr = "";
  excludeList = QStringList::split(';', excludeStr);
  for (uint i = 0; i < excludeList.count(); i++)
  {
    excludeStr = excludeList[i].stripWhiteSpace();
    if (!excludeStr.endsWith("*"))
      excludeStr = excludeStr + "/*|"+ excludeStr + "$";
    if (!excludeStr.startsWith("*"))
      excludeStr.prepend("^");
    excludeStr.replace(".","\\.");
    excludeStr.replace("*",".*");
    excludeStr.replace("?",".");
    regExpStr.append(excludeStr);
    if (i+1 < excludeList.count())
      regExpStr.append("|");
  }
  QDomNodeList nl = dom.firstChild().firstChild().childNodes();
  if (m_excludeCvsignore && projectURL.isLocalFile())
  {
    QStringList cvsIgnoreList;
    uint nlCount = nl.count();
    for ( uint i = 0; i < nlCount; i++ )
    {
      el = nl.item(i).toElement();
      tmpString = el.attribute("url");
      if (!tmpString.endsWith("/")) continue;
      cvsIgnoreList.append(tmpString);
    }
    cvsIgnoreList.append("");
    for (QStringList::ConstIterator it = cvsIgnoreList.constBegin(); it != cvsIgnoreList.constEnd(); ++it)
    {
      tmpString = *it;
      QString rxStr;
      KURL cvsIgnoreURL;
      cvsIgnoreURL.setPath(baseURL.path(1) + tmpString + ".cvsignore");
      QFile f(cvsIgnoreURL.path());
      if (f.open(IO_ReadOnly))
      {
        QTextStream stream(&f);
        QString line;
        while (!stream.atEnd())
        {
            line = stream.readLine().stripWhiteSpace();
            line.prepend(tmpString);
            if (!line.endsWith("*"))
              line = line + "/*|"+ line + "$";
            if (!line.startsWith("*"))
              line.prepend("^");
            line.replace(".","\\.");
            line.replace("*",".*");
            line.replace("?",".");
            rxStr += line + "|";
        }
        regExpStr.prepend(rxStr);
        f.close();
      }
    }
  }
  excludeRx.setPattern(regExpStr);

  m_events->clear();
  nl = projectNode.toElement().elementsByTagName("event");
  uint nlCount = nl.count();
  for ( uint i = 0; i < nlCount; i++ )
  {
    el = nl.item(i).toElement();
    EventAction ev;
    if (el.attribute("type", "internal") == "internal")
      ev.type = EventAction::Internal;
    else
      ev.type = EventAction::External;
    ev.action = el.attribute("action");
    QDomNodeList nl2 = el.elementsByTagName("argument");
    for (uint j = 0; j < nl2.count(); j++)
    {
      QString s = nl2.item(j).toElement().text();
      if (s != "--not set--" && !s.isEmpty())
          ev.arguments << s;
    }
    m_events->insert(el.attribute("name"), ev);
  }

  QDomNode teamNode = projectNode.namedItem("teamdata");
  no = teamNode.namedItem("leader");
  if (!no.isNull())
  {
     m_teamLeader.name = no.namedItem("name").toElement().text();
     m_teamLeader.email = no.namedItem("email").toElement().text();
  }

  m_subprojects.clear();
  m_subprojectLeaders.clear();
  no = teamNode.namedItem("subprojectleaders");
  nl = no.toElement().elementsByTagName("subproject");
  for (uint i = 0; i < nl.count(); i++)
  {
     el = nl.item(i).toElement();
     QDomElement el2 = el.namedItem("subprojectleader").toElement();
     TeamMember member;
     member.name = el2.attribute("name");
     member.email = el2.attribute("email");
     SubProject subproject;
     subproject.name = el.attribute("name");
     subproject.location = el.attribute("location");
     m_subprojects.append(subproject);
     m_subprojectLeaders[subproject.name] = member;
  }

  m_taskLeaders.clear();
  no = teamNode.namedItem("taskleaders");
  nl = no.toElement().elementsByTagName("projecttask");
  for (uint i = 0; i < nl.count(); i++)
  {
     el = nl.item(i).toElement();
     TeamMember member;
     member.name = el.attribute("tasklead");
     member.email = el.attribute("email");
     m_taskLeaders[el.attribute("task")] = member;
  }

  m_simpleMembers.clear();
  nl = teamNode.toElement().elementsByTagName("member");
  for (uint i = 0; i < nl.count(); i++)
  {
     el = nl.item(i).toElement();
     TeamMember member;
     member.name = el.namedItem("name").toElement().text();
     member.email = el.namedItem("email").toElement().text();
     member.task = el.attribute("task");
     m_simpleMembers.append(member);
  }

  no = teamNode.namedItem("mailinglist");
  m_mailingList = no.toElement().attribute("address");

  if (m_projectFiles.readFromXML(dom, baseURL, templateURL, excludeRx))
   m_modified = true;
  QDomElement uploadEl = projectNode.namedItem("uploadprofiles").toElement();
  m_showUploadTreeviews = uploadEl.attribute("showtreeviews", "true") == "true";
  if (m_showUploadTreeviews)
  {
    // read the profiles and create treeviews for them
    UploadProfiles::ref()->readFromXML(dom);
  } else
    UploadProfiles::ref()->clear();
  parent->statusMsg(QString::null);
  parent->newProjectLoaded(projectName, baseURL, templateURL);
  parent->reloadTree(&(m_projectFiles), true, treeStatusFromXML());
  parent->newStatus();
  adjustActions();
}

void ProjectPrivate::slotAcceptCreateProject()
{
  bool errorOccured = false;

  projectName = png->linePrjName->text();
  QString basePath = png->linePrjDir ->text();

  KURL oldBaseURL = baseURL;
  baseURL = KURL::fromPathOrURL(basePath);
  /*
    it is important to set the fields only if there is some input
    otherwise you set them to an empty string and the treeview will
    not recognize it as parent url because:
                QString::Null != ""
  */
  if (png->lineHost->text() != "")
    baseURL.setHost(png->lineHost->text());
  if (png->lineUser->text() != "")
    baseURL.setUser(png->lineUser->text());
  if (png->linePasswd->text() != "")
    baseURL.setPass(png->linePasswd->text());
  if (png->linePort->text() != "")
    baseURL.setPort(png->linePort->text().toInt());
  if (png->comboProtocol->currentText() != "")
    baseURL.setProtocol(png->comboProtocol->currentText());
  if (baseURL.protocol() == i18n("Local")) baseURL.setProtocol("file");
  baseURL.adjustPath(1);
  if (!baseURL.path().startsWith("/")) baseURL.setPath("/"+ baseURL.path());
  if (!QExtFileInfo::createDir( baseURL ))
  {
    QuantaCommon::dirCreationError(m_parent, baseURL);
    baseURL = oldBaseURL;
  } else
  {
    projectURL = baseURL;
    projectURL.setPath(baseURL.path(1) + png->linePrjFile->text());

    errorOccured = !createEmptyDom();
    if (!errorOccured)
    {
      email = pnf->lineEmail->text();
      author = pnf->lineAuthor->text();
      m_defaultDTD = DTDs::ref()->getDTDNameFromNickName(pnf->dtdCombo->currentText());
      m_defaultEncoding  = pnf->encodingCombo->currentText();

      previewPrefix = KURL::fromPathOrURL( pnf->linePrefix->text() );
      usePreviewPrefix = pnf->checkPrefix->isChecked();

      QDomElement el;
      KURL url;

      el = dom.firstChild().firstChild().toElement();
      el.setAttribute("type", png->type());
      el.setAttribute("name", projectName );
      el.setAttribute("previewPrefix", previewPrefix.url() );
      el.setAttribute("usePreviewPrefix",usePreviewPrefix);
      el.setAttribute("encoding", m_defaultEncoding);

      el = dom.createElement("author");
      dom.firstChild().firstChild().appendChild( el );
      el.appendChild( dom.createTextNode( author ) );

      el = dom.createElement("email");
      dom.firstChild().firstChild().appendChild( el );
      el.appendChild( dom.createTextNode( email ) );

      el = dom.createElement("defaultDTD");
      dom.firstChild().firstChild().appendChild(el);
      el.appendChild(dom.createTextNode(m_defaultDTD));

      KURL::List list;
      if ( png->type() == "Local" ) list = pnl->files();
      if ( png->type() == "Web"   ) list = pnw->files();

      for ( KURL::List::Iterator it = list.begin(); it != list.end(); ++it )
      {
        url = QExtFileInfo::toRelative(*it, baseURL );
        el = dom.createElement("item");
        el.setAttribute("url",QuantaCommon::qUrl(url));
        dom.firstChild().firstChild().appendChild( el );
      }

    // el = dom.createElement("item");
    // el.setAttribute("url","templates/");
    // dom.firstChild().firstChild().appendChild(el);

    //setup the templates directory
      templateURL = baseURL;
      bool createTemplateDir = true;
      if (pnf->insertGlobalTemplates->isChecked())
      {
        KURL url;
        QuantaCommon::setUrl(url, qConfig.globalDataDir + resourceDir + "templates/");
        parent->slotAddDirectory(url, false);
        QuantaCommon::setUrl(templateURL, "templates/");
        createTemplateDir = false;
      }
      if (pnf->insertLocalTemplates->isChecked())
      {
        KURL url;
        QuantaCommon::setUrl(url, locateLocal("data", resourceDir + "templates/"));
        parent->slotAddDirectory(url, false);
        QuantaCommon::setUrl(templateURL, "templates/");
        createTemplateDir = false;
      }

    if (createTemplateDir)
    {
      QuantaCommon::setUrl(templateURL, png->linePrjTmpl->text());
      templateURL.adjustPath(1);
      templateURL = QExtFileInfo::toAbsolute(templateURL, baseURL);
      if (!QExtFileInfo::createDir(templateURL))
      {
        QuantaCommon::dirCreationError(m_parent, templateURL);
      }
    }
    //the nodes are already created in loadProjectXML() called from createEmptyDom()
    el = dom.firstChild().firstChild().namedItem("templates").toElement();
    url = QExtFileInfo::toRelative(templateURL, baseURL);
    el.firstChild().setNodeValue(QuantaCommon::qUrl(url));

    //setup the toolbars directory
    toolbarURL = baseURL;
    QuantaCommon::setUrl(toolbarURL,png->linePrjToolbar->text());
    toolbarURL.adjustPath(1);
    toolbarURL = QExtFileInfo::toAbsolute(toolbarURL, baseURL);
    if (!QExtFileInfo::createDir(toolbarURL))
    {
      QuantaCommon::dirCreationError(m_parent, toolbarURL);
    }
    el = dom.firstChild().firstChild().namedItem("toolbars").toElement();
    url = QExtFileInfo::toRelative(toolbarURL, baseURL);
    el.firstChild().setNodeValue(QuantaCommon::qUrl(url));

    parent->newProjectLoaded(projectName, baseURL, templateURL);
    m_projectFiles.readFromXML(dom, baseURL, templateURL, excludeRx);
    parent->reloadTree( &(m_projectFiles), true, QStringList() );
    saveProject();
    projectRecent->addURL(projectURL);
    // remember the project in config
    writeConfig();
  }
}
if (errorOccured)
{
  slotCloseProject();
}
}


/** Saves a project view (group of files & toolbars) without asking for a name. */
void ProjectPrivate::slotSaveProjectView()
{
  slotSaveAsProjectView(currentProjectView.isEmpty());
}


/** Deletes a project view */
void ProjectPrivate::slotDeleteProjectView(const QString &view)
{
  QDomNodeList nl = dom.elementsByTagName("projectview");
  QDomElement el;
  for (uint i = 0; i < nl.count(); i++)
  {
    QDomNode node = nl.item(i);
    el = node.cloneNode().toElement();
    if (el.attribute("name") == view)
    {
      node.parentNode().removeChild(node);
      if (currentProjectView == view)
        currentProjectView = "";
      parent->setModified();
      adjustViewActions();
      break;
    }
  }
}

/** Saves a project view (group of files & toolbars) asking for a name. */
void ProjectPrivate::slotSaveAsProjectView(bool askForName)
{
  if (askForName)
  {
    bool ok;
    QString newProjectView = KInputDialog::getText(i18n("Save Project View As"),
                                                        i18n("Enter the name of the view:"), "", &ok, m_parent).lower();
    if (!ok)
      return;
    currentProjectView = newProjectView;
  }
  QDomNodeList nl = dom.elementsByTagName("projectview");
  for (uint i = 0 ;i < nl.count(); i++)
  {
    QDomNode node = nl.item(i);
    if (node.toElement().attribute("name") == currentProjectView)
    {
      if (!askForName ||
          KMessageBox::warningYesNo(m_parent, i18n("<qt>A project view named <b>%1</b> already exists.<br>Do you want to overwrite it?</qt>")
                                            .arg(currentProjectView)) == KMessageBox::Yes)
      {
        node.parentNode().removeChild(node);
        break;
      } else
      {
        return;
      }
    }
  }

  QDomElement el = dom.createElement("projectview");
  el.setAttribute("name", currentProjectView);
  QDomElement item;
  KURL::List openURLs = ViewManager::ref()->openedFiles(true); // get open urls
  KURL::List::Iterator it;
  for ( it = openURLs.begin(); it != openURLs.end(); ++it )
  {
    KURL url = (*it);
    if (m_projectFiles.contains(url))
    {
      item = dom.createElement("viewitem");
      item.setAttribute("url", QuantaCommon::qUrl(QExtFileInfo::toRelative(url, baseURL)) );
      el.appendChild(item);
    }
  }

  KURL::List toolbarList;
  parent->getUserToolbarFiles(&toolbarList);
  for (uint i =0 ; i < toolbarList.count(); i++)
  {
    item = dom.createElement("viewtoolbar");
    KURL url = toolbarList[i];
    url = QExtFileInfo::toRelative(url, baseURL);
    item.setAttribute("url", QuantaCommon::qUrl(url) );
    el.appendChild(item);
  }

  dom.firstChild().firstChild().appendChild( el );
  parent->setModified();
  adjustViewActions();
}


void ProjectPrivate::slotSelectProjectType(const QString &title)
{
  if ( png->radioLocal->isChecked() ) stack->raiseWidget( 0 );
  if ( png->radioWeb  ->isChecked() ) stack->raiseWidget( 1 );
  if ( title == m_wizTitle )
    emit setLocalFiles( pnl->checkInsert->isChecked() );
}


bool ProjectPrivate::createEmptyDom()
{
  QString str;
  QTextStream stream( &str, IO_WriteOnly );
  stream.setEncoding(QTextStream::UnicodeUTF8);

  stream << "<!DOCTYPE webproject ><webproject>" << endl;
  stream << "\t<project name=\"" << projectName << "\">" << endl;
  stream << "\t\t<upload />" << endl;
  stream << "\t</project>" << endl;
  stream << "</webproject>" << endl;

  bool result = true;

  if (! projectURL.isLocalFile())
  {
    tempFile = new KTempFile(tmpDir); // tempFile will get deleted in slotProjectClose()
    tempFile->setAutoDelete(true);
    tempFile->textStream()->setEncoding(QTextStream::UnicodeUTF8);
    *(tempFile->textStream()) << str;
    tempFile->close();
    result = QExtFileInfo::createDir(baseURL);
    if (result)
      result = KIO::NetAccess::upload(tempFile->name(), projectURL, m_parent);
    if (result)
      m_tmpProjectFile = tempFile->name();
  } else
  {
    QFile f(projectURL.path());
    if (f.open( IO_WriteOnly ))
    {
      QTextStream fstream(&f);
      fstream.setEncoding(QTextStream::UnicodeUTF8);
      fstream << str;
      m_tmpProjectFile = projectURL.path();  // we are local: the temp file and the projectURL are the same
    } else
    {
      result = false;
    }
    f.close();
  }

  if (!result)
  {
    parent->hideSplash();
    KMessageBox::sorry(m_parent, i18n("<qt>Cannot open file <b>%1</b> for writing.</qt>").arg(projectURL.prettyURL(0, KURL::StripFileProtocol)));
    delete tempFile;
    tempFile = 0L;
    return false;
  }

  dom.setContent( str);
  m_projectFiles.clear();
  return true;
}


QStringList ProjectPrivate::treeStatusFromXML()
{
  QStringList folderList;
  QDomNodeList nl = dom.elementsByTagName("treestatus");
  if (nl.count() > 0) {
    nl = nl.item(0).childNodes();
    for ( unsigned int i = 0; i < nl.count(); i++ )
    {
      QString urlString = nl.item(i).toElement().attribute("url");
      folderList.append( baseURL.url(1) + urlString);
    }
  }
  return folderList;
}


void ProjectPrivate::getStatusFromTree()
{
  // remove old status
  QDomNodeList nl = dom.elementsByTagName("treestatus");
  QDomElement  el;
  for ( unsigned int i = 0; i < nl.count(); i++ )
  {
    el = nl.item(i).toElement();
    el.parentNode().removeChild( el );
    i--;
  }
  QStringList folderList;
  parent->getTreeStatus( &folderList );
  // toplevel folder is always open in a project and QExtFileInfo::toRelative
  // creates strange output -> we remove the toplevel folder
  if (folderList.count() > 0)
     folderList.remove(folderList.begin());
  if (folderList.count() > 0) {
    // create the root element
    QDomElement root = dom.createElement("treestatus");
    dom.firstChild().firstChild().appendChild( root );
    for (QStringList::Iterator it = folderList.begin(); it != folderList.end(); ++it) {
      el = dom.createElement("openfolder");
      el.setAttribute("url", QuantaCommon::qUrl( QExtFileInfo::toRelative(KURL(*it), baseURL) ) );
      root.appendChild( el );
    }
  }
}


/** create new project */
void ProjectPrivate::slotNewProject()
{
  slotCloseProject();
    QWizard *wiz = new QWizard(m_parent, "new", true);
  wiz->setCaption(i18n("New Project Wizard"));
  wiz->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  png = new ProjectNewGeneral( wiz );

  stack = new QWidgetStack( wiz );

  pnl = new ProjectNewLocal( stack );
  pnw = new ProjectNewWeb  ( stack );
  pnf = new ProjectNewFinal( wiz );

  stack->addWidget( pnl, 0);
  stack->addWidget( pnw, 1 );

  wiz->addPage( png,   i18n("<b>General Project Settings</b>"));
  wiz->addPage( stack, m_wizTitle );
  wiz->addPage( pnf,   i18n("<b>More Project Settings</b>"));

  wiz->setNextEnabled  ( png,   false );
  wiz->setBackEnabled  ( stack, true  );
  wiz->setNextEnabled  ( stack, true  );
  wiz->setNextEnabled  ( pnf,   false );
  wiz->setFinishEnabled( pnf,   true  );

  connect( png, SIGNAL(enableNextButton(QWidget *,bool)),
          wiz, SLOT(setNextEnabled(QWidget*,bool)));
  connect( png, SIGNAL(setBaseURL(const KURL&)),
          pnl, SLOT(  setBaseURL(const KURL&)));
  connect( png, SIGNAL(setBaseURL(const KURL&)),
          pnw, SLOT(  setBaseURL(const KURL&)));
  connect( this,SIGNAL(setLocalFiles(bool)),
          pnl, SLOT(slotSetFiles(bool)));

  connect( wiz, SIGNAL(selected(const QString &)),
          this, SLOT  (slotSelectProjectType(const QString &)));

  connect( pnw, SIGNAL(enableMessagesWidget()),
          parent, SIGNAL(enableMessageWidget()));
  connect( pnw, SIGNAL(messages(const QString&)),
          parent, SLOT  (slotGetMessages(const QString&)));
  connect( pnw, SIGNAL(enableNextButton(QWidget *,bool)),
          wiz, SLOT(setNextEnabled(QWidget*,bool)));
  connect( pnw, SIGNAL(enableNextButton(QWidget *,bool)),
          wiz, SLOT(setBackEnabled(QWidget*,bool)));

  QStringList lst = DTDs::ref()->nickNameList(true);
  pnf->dtdCombo->insertStringList(lst);
  QString defaultDTDName = DTDs::ref()->getDTDNickNameFromName(qConfig.defaultDocType.lower());
  int pos = lst.findIndex(defaultDTDName);
  if (pos >= 0)
    pnf->dtdCombo->setCurrentItem(pos);

  QStringList availableEncodingNames(KGlobal::charsets()->availableEncodingNames());
  pnf->encodingCombo->insertStringList( availableEncodingNames );
  QStringList::ConstIterator iter;
  int iIndex = -1;
  for (iter = availableEncodingNames.begin(); iter != availableEncodingNames.end(); ++iter)
  {
    ++iIndex;
    if ((*iter).lower() == qConfig.defaultEncoding.lower())
    {
      pnf->encodingCombo->setCurrentItem(iIndex);
      break;
    }
  }

  png->linePrjName->setFocus();
  if ( wiz->exec() )
    slotAcceptCreateProject();

  delete wiz;

  adjustActions();
  parent->newStatus();
}


/** close project and edited files */
void ProjectPrivate::slotCloseProject()
{
  if (!parent->hasProject()) return;
  if (!uploadProjectFile())
  {
    if (KMessageBox::warningYesNo(m_parent, i18n("Saving of project failed. Do you want to continue with closing (might cause data loss)?"), i18n("Project Saving Error")) == KMessageBox::No)
      return;
  }
  emit eventHappened("after_project_close", baseURL.url(), QString::null);
  // empty dom tree
  dom.clear();
  m_events->clear();
  config->writePathEntry("Last Project", QString::null);
  init();
  parent->closeFiles();
  parent->newProjectLoaded(projectName, baseURL, templateURL);
  parent->reloadTree( &(m_projectFiles), true, QStringList());
  adjustActions();
  parent->newStatus();
}


/** open project file */
void ProjectPrivate::slotOpenProject()
{
  KURL url = KFileDialog::getOpenURL( QString::null,
                      "*.wpj *.webprj"+i18n("|Project Files\n*|All Files"), m_parent,
                      i18n("Open Project"));

  if( !url.isEmpty() )
  {
    loadProject ( url );
  }
}


/* save project file */
bool ProjectPrivate::saveProject()
{
  if ( !parent->hasProject() ) return false;
  bool result = true;
  // remove old opened files
  QDomElement  el;
  QDomNodeList nl = dom.firstChild().firstChild().childNodes();

  for ( unsigned int i = 0; i < nl.count(); i++ )
  {
    el = nl.item(i).toElement();
    if ( el.nodeName() == "openfile" )
    {
      el.parentNode().removeChild( el );
      i--;
    }
  }
  getStatusFromTree();
  QFile f(m_tmpProjectFile);
  if (f.open(IO_WriteOnly))
  {
    QTextStream stream( &f );
    stream.setEncoding(QTextStream::UnicodeUTF8);
    dom.save(stream, 2);
    f.close();
    m_modified = false;
    parent->statusMsg(i18n( "Wrote project file %1" ).arg(m_tmpProjectFile));
  } else
  {
    parent->hideSplash();
    KMessageBox::error(m_parent, i18n("<qt>Cannot open the file <b>%1</b> for writing.</qt>").arg(m_tmpProjectFile));
    result = false;
  }
  return result;
}


void ProjectPrivate::loadProjectFromTemp(const KURL &url, const QString &tempFile)
{
  m_tmpProjectFile = tempFile;
  projectURL = url;
  QFile f(tempFile);
  if (f.open(IO_ReadOnly))
  {
    baseURL = url;
    baseURL.setPath(url.directory(true, true));
    if (baseURL.isLocalFile())
    {
      QDir dir(baseURL.path());
      baseURL.setPath(dir.canonicalPath());
      baseURL.adjustPath(-1);
    }
    dom.setContent( &f );
    f.close();
    loadProjectXML();
    openCurrentView();
    projectRecent->addURL(url);
    // remember the project in config
    writeConfig();
  } else
  {
    parent->hideSplash();
    KMessageBox::error(m_parent, i18n("<qt>Cannot open the file <b>%1</b> for reading.</qt>").arg(tempFile));
  }
}

/** load project from file: url */
void ProjectPrivate::loadProject(const KURL &url)
{
  if (projectURL == url)
    return;
  if (!url.isValid())
  {
    parent->hideSplash();
    KMessageBox::sorry(m_parent, i18n("<qt>Malformed URL: <b>%1</b></qt>").arg(url.prettyURL()));
    return;
  }
  if ( projectAlreadyOpen(url.url()) )
  {
    parent->hideSplash();
    KMessageBox::sorry(m_parent, i18n("<qt>Cannot open the file<br><b>%1</b><br>It is used by another Quanta instance.</qt>").arg(url.prettyURL()));
    return;
  }
  QString tmpFile = QString::null;
  // test if url is writeable and download to local file
  if (KIO::NetAccess::exists(url, false, m_parent) &&
      KIO::NetAccess::download(url, tmpFile, m_parent))
  {
    if (parent->hasProject())
    {
      slotCloseProject();
    }
    loadProjectFromTemp(url, tmpFile);
  } else
  {
    parent->hideSplash();
    KMessageBox::error(m_parent, i18n("<qt>Cannot access the project file <b>%1</b>.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)));
  }
}


/** dialog for add files */
void ProjectPrivate::slotAddFiles()
{
  KURL::List list = KFileDialog::getOpenURLs(
    baseURL.url(),  i18n("*"), m_parent, i18n("Insert Files in Project"));

  if ( !list.isEmpty() )
  {
    KURL firstURL = list.first();
    firstURL = QExtFileInfo::toRelative( firstURL, baseURL );

    if ( firstURL.path().startsWith("/") || firstURL.path().startsWith("."))
    {
      KURLRequesterDlg *urlRequesterDlg = new KURLRequesterDlg( baseURL.prettyURL(), m_parent, "");
      urlRequesterDlg->setCaption(i18n("Files: Copy to Project"));
      urlRequesterDlg->urlRequester()->setMode( KFile::Directory | KFile::ExistingOnly);
      urlRequesterDlg->exec();
      KURL destination = urlRequesterDlg->selectedURL();
      delete urlRequesterDlg;

      if ( !destination.isEmpty())
      {
        CopyTo *dlg = new CopyTo( baseURL);
        connect(dlg, SIGNAL(deleteDialog(CopyTo*)),
                    SLOT  (slotDeleteCopytoDlg(CopyTo*)));
        connect(dlg, SIGNAL(addFilesToProject(const KURL::List&)),
                    SLOT  (slotInsertFilesAfterCopying(const KURL::List&)));
        list = dlg->copy( list, destination );
        return;
      }
      else {
        return;
      }
    }

  insertFiles( list );
  //Take care also of the selected dirs
  KURL dirURL;
  for (uint i = 0; i < list.count(); i++)
  {
    dirURL = list[i];
    if (dirURL.path().endsWith("/"))
    {
      insertFiles( dirURL, "*" );
    }
  }

    parent->reloadTree( &(m_projectFiles), false, QStringList());
  }
}


void ProjectPrivate::slotDeleteCopytoDlg(CopyTo *dlg)
{
//The CopyTo dlg is deleted only here!!
  delete dlg;
}


void ProjectPrivate::slotAddDirectory()
{
KURL url = KURL();
url = KFileDialog::getExistingURL(baseURL.prettyURL(), m_parent,
                i18n("Insert Folder in Project"));
parent->slotAddDirectory(url);
}


void ProjectPrivate::slotDebuggerOptions()
{
  // Debuggers Combo
  KTrader::OfferList offers = KTrader::self()->query("Quanta/Debugger");
  KTrader::OfferList::ConstIterator iterDbg;
  for(iterDbg = offers.begin(); iterDbg != offers.end(); ++iterDbg)
  {
    KService::Ptr service = *iterDbg;
    if(m_debuggerClientEdit == service->name())
    {
      int errCode = 0;
      DebuggerClient::DebuggerClient* dbg = KParts::ComponentFactory::createInstanceFromService<DebuggerClient::DebuggerClient>(service, this, 0, QStringList(), &errCode);
      if(dbg)
      {
        QDomNode projectNode = dom.firstChild().firstChild();
        QDomNode nodeThisDbg;
        QDomNode nodeDbg  = projectNode.namedItem("debuggers");
        if(nodeDbg.isNull())
        {
          nodeDbg = dom.createElement("debuggers");
          projectNode.appendChild(nodeDbg);
        }

        nodeThisDbg = nodeDbg.namedItem(service->name());
        if(nodeThisDbg.isNull())
        {
          nodeThisDbg = dom.createElement(service->name());
          nodeDbg.appendChild(nodeThisDbg);
        }
        dbg->showConfig(nodeThisDbg);
        delete dbg;
      }
      else
      {
        parent->hideSplash();
        KMessageBox::error(NULL, i18n("<qt>Unable to load the debugger plugin, error code %1 was returned: <b>%2</b>.</qt>").arg(errCode).arg(KLibLoader::self()->lastErrorMessage()), i18n("Debugger Error"));
      }
    }
  }
}

void ProjectPrivate::slotDebuggerChanged(const QString &debugger)
{
  m_debuggerClientEdit = debugger;
}


void ProjectPrivate::writeConfig()
{
  config->reparseConfiguration();
  config->setGroup("Projects");
  // remember the last project in config
  config->writePathEntry("Last Project", projectURL.url());
  // add project to list
  if (!projectURL.isEmpty())
  {
    QStringList projectList = config->readPathListEntry("OpenProjects");
    if (projectList.contains( projectURL.url() ) == 0)
    {
      projectList.append( projectURL.url() );
      config->writePathEntry("OpenProjects", projectList);
      // add the temp file to list
      projectList = config->readPathListEntry("ProjectTempFiles");
      projectList.append(m_tmpProjectFile);
      config->writePathEntry("ProjectTempFiles", projectList);
    }
  }
  // save recent projects
  config->deleteGroup("RecentProjects");
  projectRecent->saveEntries(config, "RecentProjects");
  config->sync();
}


void ProjectPrivate::removeFromConfig(const QString & urlStr)
{
  config->reparseConfiguration();
  config->setGroup("Projects");
  QStringList projectList = config->readPathListEntry("OpenProjects");
  int i = projectList.findIndex( urlStr );
  if ( i > -1)
  {
    projectList.remove( projectList.at(i) );
    config->writePathEntry("OpenProjects", projectList);
    // remove the temp file from list
    projectList = config->readPathListEntry("ProjectTempFiles");
    projectList.remove( projectList.at(i) );
    config->writePathEntry("ProjectTempFiles", projectList);
  }
  config->sync();
}


bool ProjectPrivate::projectAlreadyOpen(const QString & urlStr)
{
  config->reparseConfiguration();
  config->setGroup("Projects");
  QStringList projectList = config->readPathListEntry("OpenProjects");
  return (projectList.contains(urlStr) != 0);
}


/* uploads project file */
bool ProjectPrivate::uploadProjectFile()
{
  if (m_tmpProjectFile == QString::null || !saveProject())
    return false;
  // no need to upload a local file because it is the same as the tempFile
  if (projectURL.isLocalFile())
  {
    removeFromConfig( projectURL.url() );    // remove the project from the list of open projects
    // delete all temp files we used
    delete tempFile;
    tempFile = 0L;
    m_tmpProjectFile = QString::null;
    return true;
  }
  if (KIO::NetAccess::upload(m_tmpProjectFile, projectURL, m_parent))
  {
    removeFromConfig( projectURL.url() );    // remove the project from the list of open projects
    if (quantaApp)
      parent->statusMsg(i18n( "Uploaded project file %1" ).arg( projectURL.prettyURL()));
    // delete all temp files we used
    // first the one from creating a new project
    delete tempFile;
    tempFile = 0L;
    // second the one from downloading a project
    KIO::NetAccess::removeTempFile(m_tmpProjectFile);
    // third if we recovered after crash
    KIO::NetAccess::del(KURL().fromPathOrURL(m_tmpProjectFile), m_parent);
    m_tmpProjectFile = QString::null;
  }
  else
  {
    if (quantaApp)
    {
      parent->statusMsg(QString::null );
      KMessageBox::error(m_parent, KIO::NetAccess::lastErrorString());
    }
    return false;
  }
  return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////
//
//                Project
//
/////////////////////////////////////////////////////////////////////////////////////////////

Project::Project(KMainWindow *parent)
        : QObject()
{
  d = new ProjectPrivate(this);
  connect(d, SIGNAL(eventHappened(const QString&, const QString&, const QString& )), this, SIGNAL(eventHappened(const QString&, const QString&, const QString& )));
  d->m_parent = parent;
  keepPasswd = true;
  d->initActions(parent->actionCollection());
}

Project::~Project()
{
  delete d;
  d = 0;
}


QStringList Project::fileNameList()
{
  QStringList list;
  ProjectList::Iterator  it( d->m_projectFiles );
  for ( ; it.current(); ++it) {
    if (! it.current()->fileName(false).isEmpty())
      list.append(it.current()->url());
  }
  list.sort();
  return list;
}

void Project::insertFile(const KURL& nameURL, bool repaint )
{
  if (d->excludeRx.exactMatch(nameURL.path()))
      return;
  KURL url = nameURL;

  if ( !d->baseURL.isParentOf(url) )
  {
    KURLRequesterDlg *urlRequesterDlg = new KURLRequesterDlg( d->baseURL.prettyURL(), d->m_parent, "");
    urlRequesterDlg->setCaption(i18n("%1: Copy to Project").arg(nameURL.prettyURL(0, KURL::StripFileProtocol)));
    urlRequesterDlg->urlRequester()->setMode( KFile::Directory | KFile::ExistingOnly);
    urlRequesterDlg->exec();
    KURL destination = urlRequesterDlg->selectedURL();
    delete urlRequesterDlg;
    if ( !destination.isEmpty() )
    {
      CopyTo *dlg = new CopyTo(d->baseURL);
      connect(dlg, SIGNAL(deleteDialog(CopyTo*)), d,
                   SLOT(slotDeleteCopytoDlg(CopyTo*)));
      url = dlg->copy( nameURL, destination );
    }
    else  // Copy canceled, addition aborted
    {
      return;
    }
  }
  QDomElement  el;
  while ( d->baseURL.isParentOf(url) )
  {
    if ( !d->m_projectFiles.contains(url) )
    {
      el = d->dom.createElement("item");
      el.setAttribute("url", QuantaCommon::qUrl( QExtFileInfo::toRelative(url, d->baseURL) ));
      d->dom.firstChild().firstChild().appendChild( el );
      KURL u = url.upURL();
      ProjectURL *parentURL = d->m_projectFiles.find(u);
      int uploadStatus = 1;
      if (parentURL)
        uploadStatus = parentURL->uploadStatus;
      d->m_projectFiles.insert( new ProjectURL(url, "", uploadStatus, false, el) );
    }
    url.setPath(url.directory(false));
  }
  emit eventHappened("after_project_add", url.url(), QString::null);
  setModified();
  if ( repaint )
  {
    emit reloadTree( &(d->m_projectFiles), false, QStringList());
    emit newStatus();
  }
}


void Project::readConfig (KConfig *config)
{
  d->config = config;

  config->setGroup  ("Projects");
  d->projectRecent->loadEntries(config, "RecentProjects");
}


void Project::loadLastProject(bool reload)
{
  d->config->setGroup("Projects");
  QStringList projectList = d->config->readPathListEntry("OpenProjects");
  QStringList tempList = d->config->readPathListEntry("ProjectTempFiles");
  // remove all local open projects because project and temp file are the same
  for (uint i = 0; i < projectList.count(); ++i)
  {
    if (KURL::fromPathOrURL( projectList[i] ).isLocalFile())
   {
      projectList.remove( projectList.at(i) );
      tempList.remove( tempList.at(i) );
      --i;
    }
  }
  QString urlPath;
  KURL url;
  // have we still a project left ?
  while (projectList.count() > 0)
  {
    urlPath = projectList[0];
    QuantaCommon::setUrl(url, urlPath);
    QString tempPath = tempList[0];
    // test if the remote project is available
    if ( KIO::NetAccess::exists(url, false, d->m_parent) )
    {
      KURL tempURL = KURL().fromPathOrURL(tempPath);
      if (KIO::NetAccess::exists(tempURL, false, d->m_parent) &&
          KMessageBox::questionYesNo(d->m_parent, i18n("<qt>Found a backup for project <b>%1</b>.<br> Do you want to open it?</qt>").arg(url.prettyURL()), i18n("Open Project Backup") )
          == KMessageBox::Yes)
      {
        d->m_tmpProjectFile = tempPath;
        d->loadProjectFromTemp(url, d->m_tmpProjectFile);
        // the lists might have changed
        d->config->writePathEntry("OpenProjects", projectList);
        d->config->writePathEntry("ProjectTempFiles", tempList);
        d->config->sync();
        return;
      }
    }
    projectList.remove( projectList.at(0) );
    tempList.remove( tempList.at(0) );
  }
  // the lists might have changed
  d->config->writePathEntry("OpenProjects", projectList);
  d->config->writePathEntry("ProjectTempFiles", tempList);
  // now we look for the last project
  urlPath = d->config->readPathEntry("Last Project");
  QuantaCommon::setUrl(url, urlPath);

  if ( reload && (!urlPath.isEmpty() && url.isValid()))
  {
    d->loadProject( url );
  }
  d->config->sync();
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// SLOTS implementation
//

void Project::slotOpenProject(const KURL &url)
{
  if ( !url.isEmpty() )
  {
    if ( !QExtFileInfo::exists(url) )
    {
      emit hideSplash();
      if (KMessageBox::questionYesNo(d->m_parent,
           i18n("<qt>The file <b>%1</b> does not exist.<br> Do you want to remove it from the list?</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)) )
           == KMessageBox::Yes)
      {
        d->projectRecent->removeURL(url);
      }
    } else
    {
      d->loadProject ( url );
      emit eventHappened("after_project_open", url.url(), QString::null);
    }
  }
}



// slot for insert file
void Project::slotInsertFile( const KURL& url )
{
  insertFile( url, true );
}


void Project::slotAddDirectory(const KURL& p_dirURL, bool showDlg)
{
  KURL dirURL = p_dirURL;
  if ( hasProject() && !dirURL.isEmpty() )
  {
    dirURL.adjustPath(1);
    if ( !d->baseURL.isParentOf(dirURL) )
    {
      KURL destination = d->baseURL;
      if (showDlg)
      {
        KURLRequesterDlg *urlRequesterDlg = new KURLRequesterDlg( d->baseURL.prettyURL(), d->m_parent, "");
        urlRequesterDlg->setCaption(i18n("%1: Copy to Project").arg(dirURL.prettyURL(0, KURL::StripFileProtocol)));
        urlRequesterDlg->urlRequester()->setMode( KFile::Directory | KFile::ExistingOnly);
        urlRequesterDlg->exec();
        destination = urlRequesterDlg->selectedURL();
        delete urlRequesterDlg;
      }
      if ( (showDlg == false) ||
            (!destination.isEmpty()) )
      {
        CopyTo *dlg = new CopyTo(d->baseURL);
        connect(dlg, SIGNAL(addFilesToProject(const KURL::List&)),
                     SLOT  (slotInsertFilesAfterCopying(const KURL::List&)));
        connect(dlg, SIGNAL(deleteDialog(CopyTo *)), d,
                     SLOT  (slotDeleteCopytoDlg(CopyTo *)));
        //if ( rdir.right(1) == "/" ) rdir.remove( rdir.length()-1,1);
        dirURL = dlg->copy(dirURL, destination);
        return;
      }
      else
      {
        return;
      }
    }
    d->insertFiles( dirURL, "*" );
  //And again, insert now directly the directory name into the project.
  //It's important if rdir doesn't contain any files or subdirectories.
    d->insertFiles(dirURL);
    emit reloadTree( &(d->m_projectFiles), false, QStringList() );
  }
}

void Project::slotInsertFilesAfterCopying(const KURL::List& a_urlList)
{
  KURL::List::ConstIterator it;
  KURL url;
  for (it = a_urlList.begin(); it != a_urlList.end(); ++it)
  {
    url = *it;
    //url.adjustPath(1);
    d->insertFiles( url, "*" );
  }
  emit reloadTree( &(d->m_projectFiles), false, QStringList() );
}

//rename the elements in the project dom tree
void Project::slotRenamed(const KURL& oldURL, const KURL& newURL)
{
  if ( oldURL == newURL)  // just in case
    return;

  // remove the target if already there
  // TODO: check if this is correct because it removes a folder but not the content?
  d->m_projectFiles.removeFromListAndXML(newURL);

  emit statusMsg(i18n("Renaming files..."));
  progressBar->setTotalSteps(d->m_projectFiles.count());
  progressBar->setValue(0);
  progressBar->setTextEnabled(true);

  QDomElement el;
  bool isFolder = oldURL.fileName(false).isEmpty();
  ProjectList::Iterator it( d->m_projectFiles );
  for ( ; it.current(); ++it)
  {
    ProjectURL * curUrl = it.current();
    if ( oldURL == *curUrl || (isFolder && oldURL.isParentOf(*curUrl)) )
    {
      curUrl->setPath( curUrl->path().replace(oldURL.path(), newURL.path()) );
      el = curUrl->domElement;
      el.setAttribute("url", d->m_projectFiles.toRelative(*curUrl).path());
//      el.setAttribute("upload_time", "");
      d->m_modified = true;
      if (! isFolder)
        break;
    }
    progressBar->advance(1);
  }

  progressBar->setTotalSteps(1);
  progressBar->setValue(0);
  progressBar->setTextEnabled(false);

  emit statusMsg(QString::null);
  if (d->m_modified)
    setModified();  // there happens more than setting the flag !

  emit reloadTree(&(d->m_projectFiles), false, QStringList());
  emit newStatus();
}


void Project::slotRemove(const KURL& urlToRemove)
{
  emit statusMsg(i18n("Removing files..."));
  progressBar->setTotalSteps(d->m_projectFiles.count());
  progressBar->setValue(0);
  progressBar->setTextEnabled(true);

  KURL url;
  bool isFolder = d->m_projectFiles.isFolder(urlToRemove);
  ProjectList::Iterator it( d->m_projectFiles );
  for ( ; it.current(); ++it)
  {
    url = *(it.current());
    if (urlToRemove == url || (isFolder && urlToRemove.isParentOf(url)) )
    {
      d->m_projectFiles.removeFromListAndXML(url);
      d->m_modified = true;
      emit eventHappened("after_project_remove", url.url(), QString::null);
      if (! isFolder)
        break;
    }
    progressBar->advance(1);
  }

  progressBar->setTotalSteps(1);
  progressBar->setValue(0);
  progressBar->setTextEnabled(false);

  emit statusMsg(QString::null);

  if (d->m_modified)
    setModified();  // there happens more than setting the flag !
  emit closeFile(urlToRemove);
  emit reloadTree( &(d->m_projectFiles), false, QStringList() );
  emit newStatus();
}



void Project::slotOptions()
{
  KURL url;
  KDialogBase optionsDlg(KDialogBase::Tabbed, WStyle_DialogBorder, d->m_parent, "project_options", true, i18n("Project Settings"), KDialogBase::Ok | KDialogBase::Cancel);
 // optionsDlg.setMainWidget(&optionsPage);

 //add the main options page
  QFrame *page = optionsDlg.addPage(i18n("Options"));
  ProjectOptions optionsPage(page);
  QVBoxLayout *topLayout = new QVBoxLayout( page, 0, KDialog::spacingHint() );
  topLayout->addWidget(&optionsPage);

  optionsPage.linePrjName->setText( d->projectName );
  url = QExtFileInfo::toRelative(d->templateURL, d->baseURL);
  optionsPage.linePrjTmpl->setText(QuantaCommon::qUrl(url));
  url = QExtFileInfo::toRelative(d->toolbarURL, d->baseURL);
  optionsPage.linePrjToolbar->setText( QuantaCommon::qUrl(url) );

  optionsPage.lineAuthor->setText( d->author );
  optionsPage.lineEmail->setText( d->email );

  // Signals to handle debugger settings
  connect(optionsPage.buttonDebuggerOptions, SIGNAL(clicked()),
          d, SLOT(slotDebuggerOptions()));
  connect(optionsPage.comboDebuggerClient, SIGNAL(activated(const QString &)),
          d, SLOT(slotDebuggerChanged(const QString &)));


  // Debuggers Combo
  KTrader::OfferList offers = KTrader::self()->query("Quanta/Debugger");
  KTrader::OfferList::ConstIterator iterDbg;
  optionsPage.comboDebuggerClient->clear();
  optionsPage.comboDebuggerClient->insertItem(i18n("No debugger"));
  int idxDbg = 0;
  d->m_debuggerClientEdit = d->debuggerClient;
  optionsPage.buttonDebuggerOptions->setEnabled(false);
  for(iterDbg = offers.begin(); iterDbg != offers.end(); ++iterDbg)
  {
    KService::Ptr service = *iterDbg;
    optionsPage.comboDebuggerClient->insertItem(service->name());
    idxDbg++;
    if(d->debuggerClient == service->name())
    {
      optionsPage.comboDebuggerClient->setCurrentItem(idxDbg);
      optionsPage.buttonDebuggerOptions->setEnabled(true);
    }
  }

  QString excludeStr;
  for (uint i = 0; i < d->excludeList.count(); i++)
  {
    excludeStr.append(d->excludeList[i]);
    excludeStr.append(";");
  }
  optionsPage.lineExclude->setText(excludeStr);
  optionsPage.checkCvsignore->setChecked(d->m_excludeCvsignore);

  optionsPage.linePrefix->setText(d->previewPrefix.prettyURL());
  QStringList lst = DTDs::ref()->nickNameList(true);
  uint pos = 0;
  for (uint i = 0; i < lst.count(); i++)
  {
    optionsPage.dtdCombo->insertItem(lst[i]);
    if (lst[i] == DTDs::ref()->getDTDNickNameFromName(d->m_defaultDTD))
       pos = i;
  }
  optionsPage.dtdCombo->setCurrentItem(pos);


  QStringList availableEncodingNames(KGlobal::charsets()->availableEncodingNames());
  optionsPage.encodingCombo->insertStringList( availableEncodingNames );
  QStringList::ConstIterator iter;
  int iIndex = -1;
  for (iter = availableEncodingNames.begin(); iter != availableEncodingNames.end(); ++iter)
  {
     ++iIndex;
     if ((*iter).lower() == d->m_defaultEncoding.lower())
     {
       optionsPage.encodingCombo->setCurrentItem(iIndex);
       break;
     }
  }


  QStringList list = d->projectViewList();
  QString defaultView = d->dom.firstChild().firstChild().namedItem("autoload").toElement().attribute("projectview");
  if (list.count() > 0)
  {
    optionsPage.viewCombo->insertStringList(list);
    for (uint i = 0; i < list.count(); i++)
    {
      if (list[i] == defaultView)
      {
        optionsPage.viewCombo->setCurrentItem(i);
        break;
      }
    }
  } else
  {
    optionsPage.viewCombo->insertItem(i18n("No view was saved yet."));
    optionsPage.viewCombo->setEnabled(false);
  }

  optionsPage.checkPrefix->setChecked(d->usePreviewPrefix);

//add upload profiles page
  page = optionsDlg.addPage(i18n("Up&load Profiles"));
  UploadProfilesPage uploadProfilesPage(page);
  topLayout = new QVBoxLayout( page, 0, KDialog::spacingHint() );
  topLayout->addWidget(&uploadProfilesPage);
  QDomElement uploadEl = d->dom.firstChild().firstChild().namedItem("uploadprofiles").toElement();
  uploadProfilesPage.profileLabel->setText(uploadEl.attribute("defaultProfile"));
  uploadProfilesPage.checkShowUploadTreeviews->setChecked(d->m_showUploadTreeviews);

//add the team members page
  page = optionsDlg.addPage(i18n("Team Configuration"));
  TeamMembersDlg membersPage(page);
  topLayout = new QVBoxLayout( page, 0, KDialog::spacingHint() );
  topLayout->addWidget(&membersPage);

  QListViewItem *item;
  if (!teamLeader().name.isEmpty())
  {
    TeamMember member = teamLeader();
    item = new QListViewItem(membersPage.membersListView, member.name, member.email, i18n("Team Leader"), member.task);
    membersPage.membersListView->insertItem(item);
  }
  for (QMap<QString, TeamMember>::ConstIterator it = d->m_subprojectLeaders.constBegin(); it != d->m_subprojectLeaders.constEnd(); ++it)
  {
    TeamMember member = it.data();
    item = new QListViewItem(membersPage.membersListView, member.name, member.email, i18n("Subproject Leader"), member.task, it.key());
  }
  for (QMap<QString, TeamMember>::ConstIterator it = d->m_taskLeaders.constBegin(); it != d->m_taskLeaders.constEnd(); ++it)
  {
    TeamMember member = it.data();
    item = new QListViewItem(membersPage.membersListView, member.name, member.email, i18n("Task Leader"), it.key());
  }
  for (QValueList<TeamMember>::ConstIterator it = d->m_simpleMembers.constBegin(); it != d->m_simpleMembers.constEnd(); ++it)
  {
    TeamMember member = *it;
    item = new QListViewItem(membersPage.membersListView, member.name, member.email, i18n("Simple Member"), member.task);
  }
  membersPage.mailingListEdit->setText(d->m_mailingList);

//add the event configuration page
  page = optionsDlg.addPage(i18n("Event Configuration"));
  EventConfigurationDlg eventsPage(page);
  topLayout = new QVBoxLayout( page, 0, KDialog::spacingHint() );
  topLayout->addWidget(&eventsPage);
  eventsPage.initEvents(d->m_events);

  if ( optionsDlg.exec() )
  {
    d->projectName = optionsPage.linePrjName->text();
    d->author = optionsPage.lineAuthor ->text();
    d->email = optionsPage.lineEmail  ->text();

    // Debugger
    d->debuggerClient = optionsPage.comboDebuggerClient->currentText();

    d->m_defaultDTD = DTDs::ref()->getDTDNameFromNickName(optionsPage.dtdCombo->currentText()).lower();
    d->m_defaultEncoding  = optionsPage.encodingCombo->currentText();

    QuantaCommon::setUrl(d->templateURL, optionsPage.linePrjTmpl->text());
    d->templateURL.adjustPath(1);
    d->templateURL = QExtFileInfo::toAbsolute(d->templateURL, d->baseURL);
    if (!QExtFileInfo::createDir(d->templateURL))
    {
      QuantaCommon::dirCreationError(d->m_parent, d->templateURL);
    }

    QuantaCommon::setUrl(d->toolbarURL, optionsPage.linePrjToolbar->text());
    d->toolbarURL.adjustPath(1);
    d->toolbarURL = QExtFileInfo::toAbsolute(d->toolbarURL, d->baseURL);
    if (!QExtFileInfo::createDir(d->toolbarURL))
    {
      QuantaCommon::dirCreationError(d->m_parent, d->toolbarURL);
    }

    d->previewPrefix = KURL::fromPathOrURL( optionsPage.linePrefix->text() );
    d->usePreviewPrefix = optionsPage.checkPrefix->isChecked();

    QDomNode projectNode = d->dom.firstChild().firstChild();
    QDomElement el;

    el = projectNode.toElement();
    el.setAttribute("name",d->projectName);
    el.setAttribute("previewPrefix", d->previewPrefix.url() );
    el.setAttribute("usePreviewPrefix", d->usePreviewPrefix );
    el.setAttribute("encoding", d->m_defaultEncoding);

    el =projectNode.namedItem("author").toElement();
    if (!el.isNull())
       el.parentNode().removeChild(el);
    el =d->dom.createElement("author");
    projectNode.appendChild( el );
    el.appendChild(d->dom.createTextNode( d->author ) );

    el = projectNode.namedItem("email").toElement();
    if (!el.isNull())
       el.parentNode().removeChild(el);
    el =d->dom.createElement("email");
    projectNode.appendChild( el );
    el.appendChild(d->dom.createTextNode( d->email ) );

    // Debugger
    el =projectNode.namedItem("debuggerclient").toElement();
    if (!el.isNull())
       el.parentNode().removeChild(el);
    el =d->dom.createElement("debuggerclient");
    projectNode.appendChild( el );
    el.appendChild(d->dom.createTextNode( d->debuggerClient ) );

    d->m_excludeCvsignore = optionsPage.checkCvsignore->isChecked();
    excludeStr = optionsPage.lineExclude->text();
    el =projectNode.namedItem("exclude").toElement();
    if (!el.isNull())
       el.parentNode().removeChild(el);
    el =d->dom.createElement("exclude");
    if (d->m_excludeCvsignore)
      el.setAttribute("cvsignore", "true");
    else
      el.setAttribute("cvsignore", "false");
    projectNode.appendChild( el );
    el.appendChild(d->dom.createTextNode( excludeStr ) );

    el =projectNode.namedItem("defaultDTD").toElement();
    if(el.isNull())
    {
      el =d->dom.createElement("defaultDTD");
      projectNode.appendChild(el);
      el.appendChild(d->dom.createTextNode(d->m_defaultDTD));
    }
    else
    {
      el.firstChild().setNodeValue(d->m_defaultDTD);
    }

    el = projectNode.namedItem("templates").toElement();
    url = QExtFileInfo::toRelative(d->templateURL, d->baseURL);
    if(el.isNull())
    {
      el =d->dom.createElement("templates");
      projectNode.appendChild(el);
      el.appendChild(d->dom.createTextNode(QuantaCommon::qUrl(url)));
    }
    else
    {
      el.firstChild().setNodeValue(QuantaCommon::qUrl(url));
    }

    url = QExtFileInfo::toRelative(d->toolbarURL, d->baseURL);
    el = projectNode.namedItem("toolbars").toElement();
    if(el.isNull())
    {
      el =d->dom.createElement("toolbars");
      projectNode.appendChild(el);
      el.appendChild(d->dom.createTextNode(QuantaCommon::qUrl(url)));
    }
    else
    {
      el.firstChild().setNodeValue(QuantaCommon::qUrl(url));
    }

    if (optionsPage.viewCombo->isEnabled())
    {
       defaultView = optionsPage.viewCombo->currentText();
       el = projectNode.namedItem("autoload").toElement();
       if (el.isNull())
       {
         el =d->dom.createElement("autoload");
         el.setAttribute("projectview", defaultView);
         projectNode.appendChild( el );
       } else
       {
        el.setAttribute("projectview", defaultView);
       }
    }
    uploadEl.setAttribute("showtreeviews", uploadProfilesPage.checkShowUploadTreeviews->isChecked() ? "true" : "false");

    QDomNode teamNode = projectNode.namedItem("teamdata");
    if (!teamNode.isNull())
      projectNode.removeChild(teamNode);
    teamNode = d->dom.createElement("teamdata");
    QDomNode taskLeadersNode = d->dom.createElement("taskleaders");
    teamNode.appendChild(taskLeadersNode);
    QDomNode subLeadersNode = d->dom.createElement("subprojectleaders");
    teamNode.appendChild(subLeadersNode);
    QListViewItemIterator it(membersPage.membersListView);
    QListViewItem *item;
    QStringList savedSubprojects;
    while (it.current())
    {
        item = it.current();
        QString role = item->text(2);
        if (role == i18n(teamLeaderStr.utf8()))
        {
           QDomElement leaderEl = d->dom.createElement("leader");
           teamNode.appendChild(leaderEl);
           el = d->dom.createElement("name");
           leaderEl.appendChild(el);
           el.appendChild(d->dom.createTextNode(item->text(0)));
           el = d->dom.createElement("email");
           leaderEl.appendChild(el);
           el.appendChild(d->dom.createTextNode(item->text(1)));
        } else
        if (role == i18n(subprojectLeaderStr.utf8()))
        {
           QString prjName = item->text(4);
           savedSubprojects.append(prjName);
           QDomElement subEl = d->dom.createElement("subproject");
           for (uint i = 0; i < d->m_subprojects.count(); i++)
           {
             if (d->m_subprojects[i].name == prjName)
             {
                 subEl.setAttribute("location", d->m_subprojects[i].location);
                 break;
             }
           }
           subEl.setAttribute("name", prjName);
           subLeadersNode.appendChild(subEl);
           el = d->dom.createElement("subprojectleader");
           el.setAttribute("name", item->text(0));
           el.setAttribute("email", item->text(1));
           subEl.appendChild(el);
        } else
        if (role == i18n(taskLeaderStr.utf8()))
        {
           el = d->dom.createElement("projecttask");
           el.setAttribute("tasklead", item->text(0));
           el.setAttribute("email", item->text(1));
           el.setAttribute("task", item->text(3));
           taskLeadersNode.appendChild(el);
        } else
        if (role == i18n(simpleMemberStr.utf8()))
        {
           QDomElement memberEl = d->dom.createElement("member");
           memberEl.setAttribute("task", item->text(3));
           teamNode.appendChild(memberEl);
           el = d->dom.createElement("name");
           memberEl.appendChild(el);
           el.appendChild(d->dom.createTextNode(item->text(0)));
           el = d->dom.createElement("email");
           memberEl.appendChild(el);
           el.appendChild(d->dom.createTextNode(item->text(1)));
        }
        ++it;
    }
    //subprojects without a leader
    for (uint i = 0; i < d->m_subprojects.count(); i++)
    {
      if (!savedSubprojects.contains(d->m_subprojects[i].name))
      {
          el = d->dom.createElement("subproject");
          el.setAttribute("name", d->m_subprojects[i].name);
          el.setAttribute("location", d->m_subprojects[i].location);
      }
    }

    el = d->dom.createElement("mailinglist");
    el.setAttribute("address", membersPage.mailingListEdit->text());
    teamNode.appendChild(el);
    projectNode.appendChild(teamNode);

    eventsPage.saveEvents(d->dom);

    setModified();
    d->loadProjectXML();
  }
}

void Project::slotUpload()
{
  if (! ViewManager::ref()->saveAll())
    return;

  ProjectUpload *dlg = new ProjectUpload(KURL(), false, i18n("Upload project items..."));
  connect(dlg, SIGNAL(eventHappened(const QString&, const QString&, const QString& )), this, SIGNAL(eventHappened(const QString&, const QString&, const QString& )));
  dlg->show();
}

void Project::slotUploadURL(const KURL& urlToUpload)
{
  if (!ViewManager::ref()->saveAll())
    return;

  KURL url = QExtFileInfo::toRelative( urlToUpload, d->baseURL);

  ProjectUpload *dlg = new ProjectUpload(url, false, i18n("Upload project items..."));
  connect(dlg, SIGNAL(eventHappened(const QString&, const QString&, const QString& )), this, SIGNAL(eventHappened(const QString&, const QString&, const QString& )));
  dlg->show();
}


void Project::slotGetMessages(const QString& data)
{
  emit messages(data);
}

void Project::slotRescanPrjDir()
{
  RescanPrj *dlg = new RescanPrj( d->m_projectFiles, d->baseURL, d->excludeRx,
                                    d->m_parent, i18n("New Files in Project's Folder"));
  if ( dlg->exec() )
  {
    d->insertFiles(dlg->files());
    setModified();
    emit reloadTree(&(d->m_projectFiles), false, QStringList());
  }
  delete dlg;
}
/** Returns the relative url with the prefix inserted. */
KURL Project::urlWithPrefix(const KURL& url)
{
  KURL returnUrl = url;

  if (d->usePreviewPrefix)
  {
    KURL tempUrl = QExtFileInfo::toRelative(url, d->baseURL );
    if ( !tempUrl.path().startsWith("/") )
    {
      returnUrl = d->previewPrefix;
      returnUrl.setPath(d->previewPrefix.path(1) + tempUrl.path());
    }
  }
  return returnUrl;
}


/*Returns true if url is already in the project.*/
bool Project::contains(const KURL& url)
{
  if (d->m_projectFiles.isEmpty())
    d->m_projectFiles.readFromXML(d->dom, d->baseURL, d->templateURL, d->excludeRx);
  return (d->m_projectFiles.contains(url) > 0);
}

void Project::updateTimeStamp(const KURL& url, int modifiedTime)
{
  ProjectURL *proUrl = d->m_projectFiles.find(url);
  if (! proUrl)
    return;

  QDomElement itemEl = proUrl->domElement;
  int origModifTime = itemEl.attribute("modified_time","1").toInt();
  QString qurl = QuantaCommon::qUrl(d->m_projectFiles.toRelative(*proUrl));

  QDomNodeList nl = d->dom.elementsByTagName("uploadeditem");
  uint nlCount = nl.count();
  for (uint i = 0; i < nlCount; ++i)
  {
    QDomElement el = nl.item(i).toElement();
    if (el.attribute("url") == qurl)
    {
      int uploadTime = el.attribute("upload_time","1").toInt();
      if (uploadTime == origModifTime)
      {
        itemEl.setAttribute("modified_time", modifiedTime);
        el.setAttribute("upload_time", modifiedTime);
        d->m_modified = true;
      }
    }
  }
  if (d->m_modified)
    setModified();
}

void Project::slotFileDescChanged(const KURL& url, const QString& desc)
{
  ProjectURL *proUrl = d->m_projectFiles.find(url);
  if (! proUrl)
    return;

  QDomElement el = proUrl->domElement;
  el.setAttribute("desc", desc);
  proUrl->fileDesc = desc;
  setModified();
}

void Project::slotUploadStatusChanged(const KURL& url, int status)
{
  if (! d->m_projectFiles.contains(url))
    return;

  QDomElement el;
  QString urlStr = url.url();
  ProjectList::Iterator it( d->m_projectFiles );
  for ( ; it.current(); ++it)
  {
    if ( it.currentKey().startsWith(urlStr) || it.currentKey() + "/" == urlStr)
    {
      it.current()->uploadStatus = status;
      el = it.current()->domElement;
      el.setAttribute("uploadstatus", status);
    }
  }
  setModified();
}

void Project::slotChangeDocumentFolderStatus(const KURL &url, bool status)
{
  ProjectURL *proUrl = d->m_projectFiles.find(url);
  if (! proUrl)
    return;

  QDomElement el = proUrl->domElement;
  el.setAttribute("documentFolder", (status ? "true" : "false"));
  proUrl->documentFolder = status;
  setModified();
}


/** Returns the project's base URL if it exists, the HOME dir if there is no project and no opened document (or the current opened document was not saved yet), and the base URL of the opened document, if it is saved somewhere. */
KURL Project::projectBaseURL()
{
  KURL result;
  if  (hasProject())
  {
     result = d->baseURL;
  } else
  {
    Document *w = ViewManager::ref()->activeDocument();
    if  ( !w || w->isUntitled() )
    {
      result = QExtFileInfo::home();
    } else
    {
       result = QExtFileInfo::path(w->url());
    }
  }
  return result;
}

void Project::savePassword(const QString &entry, const QString &passwd, bool store)
{
  d->m_passwdList[entry] = passwd;
  KConfig *config = kapp->config();
  config->setGroup("Projects");
  if (store)
  {
    config->writeEntry(d->projectName + " | " + entry, KStringHandler::obscure(passwd));
    //config->writeEntry(projectName + " | " + entry, passwd);
  }
  else
  {
    config->deleteEntry(d->projectName + " | " + entry);
  }
  config->sync();
}

QString Project::password(const QString &entry)
{
   if (d->m_passwdList.contains(entry))
     return d->m_passwdList[entry];
   else
   {
      KConfig *config = kapp->config();
      config->setGroup("Projects");
      QString passwd =  KStringHandler::obscure(config->readEntry(d->projectName + " | " + entry,""));
//      QString passwd =  config->readEntry(d->projectName + " | " + entry,"");
      return passwd;
   }
}

bool Project::passwordSaved(const QString &entry)
{
      KConfig *config = kapp->config();
      config->setGroup("Projects");
      QString passwd =  KStringHandler::obscure(config->readEntry(d->projectName + " | " + entry,""));
//      QString passwd =  config->readEntry(d->projectName + " | " + entry,"");
      return !passwd.isEmpty();
}

KURL Project::documentFolderForURL(const KURL& url)
{
  KURL docFolderURL = d->baseURL;
  ProjectList::Iterator it( d->m_projectFiles );
  for ( ; it.current(); ++it) {
    if (it.current()->documentFolder && it.current()->isParentOf(url)
        && docFolderURL.isParentOf(*(it.current())))
    {
      docFolderURL = *(it.current());
    }
  }
  return docFolderURL;
}

void Project::slotReloadProjectDocs()
{
  delete d->m_dirWatch;
  d->m_dirWatch = new KDirWatch(this);
  connect(d->m_dirWatch, SIGNAL(dirty(const QString &)), SIGNAL(reloadProjectDocs()));
  connect(d->m_dirWatch, SIGNAL(deleted(const QString &)), SIGNAL(reloadProjectDocs()));
  if (d->baseURL.isLocalFile())
    d->m_dirWatch->addDir(d->baseURL.path() + "/doc");
  KURL url;
  QString path;
  ProjectList::Iterator it( d->m_projectFiles );
  for ( ; it.current(); ++it) {
    url = *(it.current());
    path = d->m_projectFiles.toRelative(url).path();
    if (path.startsWith("doc/") && path.endsWith("/index.html"))
    {
      emit addProjectDoc(url);
      if (url.isLocalFile())
        d->m_dirWatch->addFile(url.path());
    }
  }
}


bool Project::hasProject()
{
  return !d->projectName.isNull();
}


const QString& Project::defaultDTD()
{
  return d->m_defaultDTD;
}


const QString& Project::defaultEncoding()
{
  return d->m_defaultEncoding;
}


KURL Project::toolbarURL()
{
  return d->toolbarURL;
}


KURL Project::templateURL()
{
  return d->templateURL;
}


QString Project::email()
{
  return d->email;
}

QDomDocument* Project::dom()
{
  return &d->dom;
}

QString Project::debuggerClient()
{
  return d->debuggerClient;
}


void Project::setModified(bool b)
{
  d->m_modified = b;
  if (b)
    d->saveProject();
}

EventActions* Project::events()
{
  if (hasProject())
    return d->m_events;
  else
    return 0L;
}

TeamMember Project::teamLeader()
{
  return d->m_teamLeader;
}

TeamMember Project::subprojectLeader(const QString &name)
{
  if (d->m_subprojectLeaders.contains(name))
      return d->m_subprojectLeaders[name];
  else
      return TeamMember();
}

TeamMember Project::taskLeader(const QString &name)
{
  if (d->m_taskLeaders.contains(name))
    return d->m_taskLeaders[name];
  else
    return TeamMember();
}

QValueList<TeamMember> Project::simpleMembers()
{
  return d->m_simpleMembers;
}

QString Project::mailingList()
{
  return d->m_mailingList;
}

QValueList<SubProject>* Project::subprojects()
{
  return &d->m_subprojects;
}

QStringList Project::tasks()
{
   QStringList result;
   for (QMap<QString, TeamMember>::ConstIterator it = d->m_taskLeaders.constBegin(); it != d->m_taskLeaders.constEnd(); ++it)
   {
      result << it.key();
   }
   return result;
}

QMap<QString, TeamMember> Project::allMembers()
{
   QMap<QString, TeamMember> members;
   for (QValueList<TeamMember>::ConstIterator it = d->m_simpleMembers.constBegin(); it != d->m_simpleMembers.constEnd(); ++it)
   {
      members[(*it).name] = *it;
   }
   if (!d->m_teamLeader.name.isEmpty())
     members[d->m_teamLeader.name] = d->m_teamLeader;
   for (QMap<QString, TeamMember>::ConstIterator it = d->m_subprojectLeaders.constBegin(); it != d->m_subprojectLeaders.constEnd(); ++it)
   {
      members[it.data().name] = it.data();
   }
   for (QMap<QString, TeamMember>::ConstIterator it = d->m_taskLeaders.constBegin(); it != d->m_taskLeaders.constEnd(); ++it)
   {
      members[it.data().name] = it.data();
   }

   return members;
}

void Project::urlMoved(const KURL& srcURL, const KURL &destURL)
{
   emit eventHappened("after_file_move", srcURL.url(), destURL.url());
}

bool Project::queryClose()
{
  bool canExit = true;
  if (hasProject())
  {
    d->config->writePathEntry("Last Project", d->projectURL.url());
    canExit = d->uploadProjectFile();
    if (! canExit)
    {
      if (KMessageBox::warningYesNo(d->m_parent, i18n("Saving of project failed. Do you want to continue with exit (might cause data loss)?"), i18n("Project Saving Error")) == KMessageBox::Yes)
          canExit = true;
    }
    if (canExit)
      emit eventHappened("after_project_close", d->baseURL.url(), QString::null);
  }
  return canExit;
}

void Project::slotShowProjectToolbar(bool show)
{
  if (quantaApp && quantaApp->factory())
  {
    QWidget *w = quantaApp->factory()->container("project_toolbar", quantaApp);
    if (w)
      w->setShown(show);
  }
}

#include "project.moc"
#include "projectprivate.moc"
