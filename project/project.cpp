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
#include <qcheckbox.h>

// include files for KDE
#include <kapplication.h>
#include <kcharsets.h>
#include <kdirwatch.h>
#include <kio/netaccess.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kparts/componentfactory.h>
#include <kprogress.h>
#include <kstringhandler.h>
#include <kurlrequester.h>
#include <kurlrequesterdlg.h>

// application headers
#include "copyto.h"
#include "document.h"
#include "dtds.h"
#include "dtdselectdialog.h"
#include "eventconfigurationdlg.h"
#include "qextfileinfo.h"
#include "quanta.h"
#include "quantacommon.h"
#include "projectprivate.h"
#include "projectupload.h"
#include "projectoptions.h"
#include "rescanprj.h"
#include "resource.h"
#include "teammembersdlg.h"
#include "uploadprofilespage.h"
#include "viewmanager.h"

extern QString simpleMemberStr;
extern QString taskLeaderStr;
extern QString teamLeaderStr;
extern QString subprojectLeaderStr;

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
  eventsPage.enableEventsBox->setChecked(d->m_eventsEnabled);

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
    d->m_eventsEnabled = eventsPage.enableEventsBox->isChecked();
    projectNode.toElement().setAttribute("enableEvents", d->m_eventsEnabled?"true":"false");

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
    emit eventHappened("before_project_close", d->baseURL.url(), QString::null);
    d->config->setGroup("Projects");
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

bool Project::eventsEnabled()
{
   return d->m_eventsEnabled;
}

void Project::slotShowProjectToolbar(bool show)
{
  if (quantaApp && quantaApp->factory())
  {
    QWidget *w = quantaApp->factory()->container("project_toolbar", quantaApp);
    if (w)
    {
      if (show && m_projectToolbarVisible)
        w->setShown(true);
      else
      if (!show)
      {
        m_projectToolbarVisible = w->isShown();
        w->setShown(false);
      }
    }
  }
}

#include "project.moc"
