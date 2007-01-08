/***************************************************************************
                          projectupload.cpp  -  description
                             -------------------
    begin                : Wed Nov 15 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2001-2003 by Andras Mantia
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/



//qt includes
#include <qlistview.h>
#include <qeventloop.h>
#include <qfileinfo.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qstringlist.h>
#include <qregexp.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qurl.h>
#include <qcheckbox.h>
#include <qtimer.h>
#include <qframe.h>
#include <qtooltip.h>

//kde includes
#include <kapplication.h>
#include <kurl.h>
#include <kio/job.h>
#include <kmessagebox.h>
#include <kprotocolinfo.h>
#include <kdebug.h>
#include <kinputdialog.h>
#include <kio/netaccess.h>
#include <klocale.h>
#include <klineedit.h>
#include <kcombobox.h>
#include <kprogress.h>
#include <kpassdlg.h>
#include <kpushbutton.h>
#include <ktempfile.h>
#include <kuser.h>

//standard includes
#include <time.h>

//own includes
#include "uploadprofiledlgs.h"
#include "projectupload.h"
#include "project.h"
#include "quantacommon.h"
#include "qextfileinfo.h"
#include "resource.h"

ProjectUpload::ProjectUpload(const KURL& url, const QString& profileName, bool showOnlyProfiles, bool quickUpload, bool markOnly, const char* name)
  :ProjectUploadS( 0L, name, false, Qt::WDestructiveClose)
{
    m_quickUpload = quickUpload;
    m_profilesOnly = showOnlyProfiles;
    list->hide();
    m_project = Project::ref();
    initProjectInfo(profileName);
    startUrl = url;
    if (m_profilesOnly)
    {
      clearWFlags(Qt::WDestructiveClose);
      uploadFrame->hide();
      buttonCancel->hide();
      adjustSize();
      buttonUpload->setText(i18n("&Close"));
      setCaption(i18n("Upload Profiles"));
    } else
    {
      if (markOnly)
        markAsUploaded->setChecked(true);
      QTimer::singleShot(10, this, SLOT(slotBuildTree()));
      currentItem = 0L;
    }
}


ProjectUpload::~ProjectUpload()
{
  m_project->setModified(true);
  delete baseUrl;
}

void  ProjectUpload::initProjectInfo(const QString& defaultProfile)
{
  baseUrl = new KURL();

//  list->setMultiSelection(true);
  QDomDocument *dom = m_project->sessionDom();
  m_profilesNode = dom->firstChild().firstChild().namedItem("uploadprofiles");
  if (m_profilesNode.isNull()) //compat code, remove when upgrade from 3.2 is not supported
  {
     m_currentProfileElement = dom->firstChild().firstChild().namedItem("upload").toElement();
     m_defaultProfile = m_currentProfileElement.attribute("user","") + "@" + m_currentProfileElement.attribute("remote_host","");
     QDomElement e = dom->createElement("uploadprofiles");
     e.setAttribute("defaultProfile", m_defaultProfile);
     QDomElement el = dom->createElement("profile");
     el.setAttribute("remote_host", m_currentProfileElement.attribute("remote_host",""));
     el.setAttribute("user", m_currentProfileElement.attribute("user",""));
     el.setAttribute("remote_path", m_currentProfileElement.attribute("remote_path",""));
     el.setAttribute("remote_port", m_currentProfileElement.attribute("remote_port",""));
     el.setAttribute("remote_protocol", m_currentProfileElement.attribute("remote_protocol","ftp"));
     el.setAttribute("name", m_defaultProfile);
     e.appendChild(el);
//     m_project->dom.firstChild().firstChild().removeChild(m_currentProfileElement);
     m_currentProfileElement = el;
     dom->firstChild().firstChild().appendChild(e);
     m_profilesNode = e;
     comboProfile->insertItem(m_defaultProfile);
     m_project->setModified(true);
  } else
  {
      if (defaultProfile.isEmpty())
        m_defaultProfile = m_profilesNode.toElement().attribute("defaultProfile");
      else
        m_defaultProfile = defaultProfile;
      QDomNodeList profileList = m_profilesNode.toElement().elementsByTagName("profile");
      QDomElement e;
      m_currentProfileElement = profileList.item(0).toElement();
      QString s;
      int defaultIdx = 0;
      for (uint i = 0; i < profileList.count(); i++)
      {
          e = profileList.item(i).toElement();
          s = e.attribute("name");
          comboProfile->insertItem(s);
          if (s == m_defaultProfile)
          {
            defaultIdx = i;
            m_currentProfileElement = e;
          }
      }
      comboProfile->setCurrentItem(defaultIdx);
  }
  buttonRemoveProfile->setEnabled(comboProfile->count() > 1);
  keepPasswords->setChecked(m_project->keepPasswd);
  uploadInProgress = false;
  connect( this, SIGNAL( uploadNext() ), SLOT( slotUploadNext() ) );
  setProfileTooltip();
}

/** No descriptions */
void ProjectUpload::slotBuildTree()
{
 emit eventHappened("upload_requested", m_project->projectBaseURL().url(), "");
 loadRemoteUploadInfo();
 KIO::UDSEntry entry;
 QString strUrl = QuantaCommon::qUrl(startUrl);
 bool isDirectory = strUrl.endsWith("/");
 bool forceUpload = !startUrl.isEmpty();
 QString s;
 QDomElement el;
 QDomNodeList nl = m_project->dom()->elementsByTagName("item");
 totalProgress->setTotalSteps(nl.count() - 1 );
 totalProgress->setValue(0);
 totalText->setText(i18n("Scanning project files..."));

 KURL u = m_project->projectBaseURL();
 if (!startUrl.isEmpty())
 {
   u = QExtFileInfo::toAbsolute(startUrl, u);
 }
 QDict<KFileItem> projectDirFiles;

 if (startUrl.isEmpty() || strUrl.endsWith("/")) //upload a folder
 {
   projectDirFiles = QExtFileInfo::allFilesDetailed(u, "*", this);
 } else
 {
   projectDirFiles.insert(u.url(), new KFileItem(KFileItem::Unknown, KFileItem::Unknown, u, true));
 }

 QTime t;
 t.start();

 u = m_project->projectBaseURL();
 KURL absUrl = u;
 for (uint i = 0; i < nl.count(); i++)
 {
   el = nl.item(i).toElement();
   s = el.attribute("url");
   if (startUrl.isEmpty() || (s.startsWith(strUrl) && isDirectory) || s == strUrl)
   {
     QuantaCommon::setUrl(u, s);
     absUrl.setPath(m_project->projectBaseURL().path(1)+u.path(-1));
/*     if (!QExtFileInfo::exists(absUrl))
       continue; */
     KFileItem *p_item = projectDirFiles.find(absUrl.url());
     if (!p_item)
       continue;
     KFileItem item(*p_item);
     UploadTreeFile *it = list->addItem(u, item);
     if ( it != 0 )
     {
       int uploadedTime = -1;
       if (m_uploadTimeList.contains(s))
         uploadedTime = m_uploadTimeList[s];
       int modifiedTime = item.time(KIO::UDS_MODIFICATION_TIME);
       el.setAttribute("modified_time", modifiedTime);
       int uploadStatus = el.attribute("uploadstatus", "1").toInt();
       if (m_quickUpload || (forceUpload && uploadStatus == 0))
        uploadStatus = 1;
       if (m_quickUpload || (uploadedTime != modifiedTime && uploadStatus != 0))
       {
         modified.append( u );
         it->setSelected(true);
       }
       if (uploadStatus == 2)
         it->setConfirmUpload(true);
       totalProgress->setValue(i);
     }
   }
 }
 projectDirFiles.setAutoDelete(true);
 projectDirFiles.clear();
 totalText->setText(i18n("Building the tree..."));
 list->checkboxTree();
 if (!startUrl.isEmpty())
      expandAll();
 list->show();
 totalText->setText(i18n("Total:"));
 totalProgress->setTotalSteps(1);
 totalProgress->setValue(0);
 //hack to force repainting of the treeview
 resize(width() + 1, height());
 resize(width() - 1, height());
 if (m_quickUpload)
   startUpload();
}


void ProjectUpload::buildSelectedItemList()
{
  QListViewItem *item;
  QListViewItemIterator it(list);
  toUpload.clear();
  needsConfirmation.clear();
  for ( ; it.current(); ++it )
  {
   item = it.current();
   if ( list->isSelected( item ))
   {
     KURL u;
     if (dynamic_cast<UploadTreeFolder*>(item))
     {
      u = dynamic_cast<UploadTreeFolder*>(item)->url();
     } else
     {
       UploadTreeFile* fileItem = dynamic_cast<UploadTreeFile*>(item);
       u = fileItem->url();
       if (fileItem->confirmUpload() && !u.isEmpty())
         needsConfirmation.append(item);
     }

     if (!u.isEmpty())
         toUpload.append(item);
   }
  }
}

void ProjectUpload::initBaseUrl()
{
  QString path = m_currentProfileElement.attribute("remote_path","");
  if (!path.startsWith("/"))
    path.prepend("/");

  baseUrl->setProtocol(m_currentProfileElement.attribute("remote_protocol","ftp"));
  baseUrl->setPort(m_currentProfileElement.attribute("remote_port","").toInt());
  baseUrl->setHost(m_currentProfileElement.attribute("remote_host",""));
  baseUrl->setPath(path);
  baseUrl->setUser(m_currentProfileElement.attribute("user",""));
  QString password;
  if (keepPasswords->isChecked())
  {
     m_project->keepPasswd = true;
     password = m_project->password(m_currentProfileElement.attribute("remote_protocol") + "://" + m_currentProfileElement.attribute("user") + "@" + m_currentProfileElement.attribute("remote_host"));
  } else
  {
     m_project->keepPasswd = false;
     if (m_currentProfileElement != m_lastEditedProfileElement)
     {
         m_lastPassword = "";
     }
     password = m_lastPassword;
  }
  baseUrl->setPass(password);
}

void ProjectUpload::startUpload()
{
  if (m_profilesOnly)
  {
    QDialog::accept();
    return;
   }
  stopUpload = false;
  initBaseUrl();
  if (markAsUploaded->isChecked())
  {
      QStringList selectedList;
      QListViewItem *item;
      QListViewItemIterator it(list);
      for ( ; it.current(); ++it )
      {
          item = it.current();
          if ( list->isSelected( item ))
          {
            KURL u;
            if (dynamic_cast<UploadTreeFolder*>(item))
            {
              u = dynamic_cast<UploadTreeFolder*>(item)->url();
            } else
            {
              u = dynamic_cast<UploadTreeFile*>(item)->url();
            }

            if (!u.isEmpty())
                selectedList.append(QuantaCommon::qUrl(u));
          }
     }
    //update upload time
    QDomNodeList nl = m_project->dom()->elementsByTagName("item");
    QDomElement el;
    for ( uint i = 0; i < nl.count(); i++ )
    {
      el = nl.item(i).toElement();
      if ( selectedList.contains(el.attribute("url")))
      {
        m_uploadTimeList[el.attribute("url")] = el.attribute("modified_time").toInt();
      }
    }
    saveRemoteUploadInfo();
    accept();
  } else
  {
    buildSelectedItemList();
    int confirmCount = needsConfirmation.count();
    if (confirmCount > 0)
    {
        QValueList<QListViewItem*>::Iterator it;
        QStringList confirmList;
        for (it = needsConfirmation.begin(); it != needsConfirmation.end(); ++it)
        {
            confirmList.append(((UploadTreeFile*)(*it))->url().prettyURL(0, KURL::StripFileProtocol));
        }
        bool ok;
        QStringList confirmedList = KInputDialog::getItemList(i18n("Confirm Upload"), i18n("Confirm that you want to upload the following files (unselect the files you do not want to upload):"), confirmList, confirmList, true, &ok, this);
        if (!ok)  return;
        for (it = needsConfirmation.begin(); it != needsConfirmation.end(); ++it)
        {
            if (!confirmedList.contains(((UploadTreeFile*)(*it))->url().prettyURL(0, KURL::StripFileProtocol)))
              toUpload.remove(*it);
        }

    }
    int selectedNum = toUpload.count();
    totalProgress->setProgress(0);
    totalProgress->setTotalSteps(selectedNum);
    uploadInProgress = true;
    suspendUpload = false;
    buttonUpload->setEnabled(false);
    KURL u = *baseUrl;
    u.setPath(u.protocol() == "file" ? "/" : "");
    if (QExtFileInfo::exists(u, false, this))
    {
      upload();
      return;
    } else
    {
      if (KMessageBox::warningContinueCancel(this, i18n("<qt><b>%1</b> seems to be unaccessible.<br>Do you want to proceed with upload?</qt>")
          .arg(u.prettyURL(0, KURL::StripFileProtocol)),QString::null,KStdGuiItem::cont()) == KMessageBox::Continue)
      {
        upload();
        return;
      }
    }
  }
  buttonUpload->setEnabled(true);
  uploadInProgress = false;
}

void ProjectUpload::upload()
{
  if (stopUpload)
  {
    saveRemoteUploadInfo();
    return;
  }
  KURL dir;
  KURL to;
  UploadTreeFile *fileItem;
  UploadTreeFolder *folderItem;

  if (toUpload.isEmpty())
  {
    saveRemoteUploadInfo();
    accept();
    return;
  }
  currentItem = toUpload.at(0);
  if (dynamic_cast<UploadTreeFile*>(currentItem))
  {
    fileItem = static_cast<UploadTreeFile*>(currentItem);
    folderItem = 0L;
    currentURL = fileItem->url();
  } else
  {
    fileItem = 0L;
    folderItem = static_cast<UploadTreeFolder*>(currentItem);
    currentURL = folderItem->url();
  }


  KURL from = QExtFileInfo::toAbsolute(currentURL, m_project->projectBaseURL());
  to = *baseUrl;
  to.addPath( currentURL.path() );
  if (to.fileName(false).isEmpty())
  {
    dir = to;
  }
  else
  {
    dir = to.upURL() ;
  }

  if ( !madeDirs.contains(dir) )
  {
    madeDirs.append( dir );
    if (!QExtFileInfo::createDir(dir, this))
    {
      QuantaCommon::dirCreationError(this, KURL( dir.prettyURL(0, KURL::StripFileProtocol) ));
      buttonUpload->setEnabled(true);
      uploadInProgress = false;
      saveRemoteUploadInfo();
      return;
    }
  }

  // qDebug("%s -> %s", from.url().data(), to.url().data() );
  if (!from.fileName(false).isEmpty() && fileItem)
  {
    emit eventHappened("before_upload", from.url(), to.url());
    KIO::FileCopyJob *job = KIO::file_copy( from, to, fileItem->permissions(), true, false, false );

    connect( job, SIGNAL( result( KIO::Job * ) ),this,
                      SLOT( uploadFinished( KIO::Job * ) ) );
    connect( job, SIGNAL( percent( KIO::Job *,unsigned long ) ),
                this, SLOT( uploadProgress( KIO::Job *,unsigned long ) ) );
    connect( job, SIGNAL( infoMessage( KIO::Job *,const QString& ) ),
                this, SLOT( uploadMessage( KIO::Job *,const QString& ) ) );

    labelCurFile->setText(i18n("Current: %1").arg(currentURL.fileName()));
    currentProgress->setProgress( 0 );
    return;
  } else  //it is a dir, so just go to the next item
  {
    emit uploadNext();
    return;
  }
  saveRemoteUploadInfo();
  buttonUpload->setEnabled(true);
  uploadInProgress = false;
  reject();
}

void ProjectUpload::uploadFinished( KIO::Job *job )
{
   if ( !job ) return;

   if ( job->error() )
   {
      job->showErrorDialog( this  );
      uploadInProgress = false;
      buttonUpload->setEnabled(true);
      saveRemoteUploadInfo();
      return;
   }
   KIO::FileCopyJob *fJob = dynamic_cast<KIO::FileCopyJob *>(job);
   if (fJob)
    emit eventHappened("after_upload", fJob->srcURL().url(), fJob->destURL().url());
   slotUploadNext();
}

void ProjectUpload::uploadProgress ( KIO::Job *, unsigned long percent  )
{
  currentProgress->setProgress( percent );
}

void ProjectUpload::uploadMessage ( KIO::Job *, const QString & msg )
{
    labelCurFile->setText( currentURL.fileName() + " : " + msg );
}

void ProjectUpload::selectAll()
{
  list->selectAll(true);
  list->checkboxTree();
}

void ProjectUpload::selectModified()
{
  for ( KURL::List::Iterator file = modified.begin(); file != modified.end(); ++file )
  {
    QListViewItem *it = list->findItem( (*file).path() );
    it->setSelected(true);
    it->repaint();
  }
  list->checkboxTree();
}

void ProjectUpload::clearSelection()
{
  list->selectAll(false);
  list->checkboxTree();
}

void ProjectUpload::invertSelection()
{
  list->invertAll();
  list->checkboxTree();
}

void ProjectUpload::expandAll()
{
  list->expandAll();
}

void ProjectUpload::collapseAll()
{
  list->collapseAll();
}

void ProjectUpload::resizeEvent ( QResizeEvent *t )
{
  ProjectUploadS::resizeEvent(t);
  list->setColumnWidth(0,list->width()-list->columnWidth(1)-list->columnWidth(2)-20);
}

/** No descriptions */
void ProjectUpload::slotUploadNext()
{
  if (!suspendUpload)
  {
    totalProgress->setProgress(totalProgress->progress()+1);
   // QListViewItem *it = list->findItem( currentURL.path() );
    QListViewItem *it = currentItem;
    if (it)
    {
     it->setSelected(false);
     UploadTreeFile *itf = dynamic_cast<UploadTreeFile*>(it);
     if (itf)
      itf->setWhichPixmap( "check_clear" );
     it->repaint();
    }
    toUpload.remove( it );

    //update upload time
    QDomNodeList nl = m_project->dom()->elementsByTagName("item");
    QDomElement el;
    for ( uint i = 0; i < nl.count(); i++ )
    {
      el = nl.item(i).toElement();
      if ( el.attribute("url") == QuantaCommon::qUrl(currentURL) )
      {
        m_uploadTimeList[el.attribute("url")] = el.attribute("modified_time").toInt();
        break;
      }
    }

    upload();
  }
}

void ProjectUpload::clearProjectModified()
{
  QDomNodeList nl = m_project->dom()->elementsByTagName("item");
  for ( unsigned int i=0; i<nl.count(); i++ )
  {
    QDomElement el = nl.item(i).toElement();
    m_uploadTimeList[el.attribute("url")] = el.attribute("modified_time").toInt();
  }
  modified.clear();
  list->clearSelection();
  list->checkboxTree();
}

void ProjectUpload::slotNewProfile()
{
  UploadProfileDlgS *profileDlg = new UploadProfileDlgS(this);
  QDomElement el = m_currentProfileElement;
  m_currentProfileElement = m_project->dom()->createElement("profile");
  fillProfileDlg(profileDlg);
  if (profileDlg->exec())
  {
     readProfileDlg(profileDlg);
     m_profilesNode.appendChild(m_currentProfileElement);
     m_project->setModified(true);
     comboProfile->insertItem(m_currentProfileElement.attribute("name"), 0);
     setProfileTooltip();
  } else
    m_currentProfileElement = el;
  delete profileDlg;
  buttonRemoveProfile->setEnabled(comboProfile->count() > 1);
}

void ProjectUpload::slotEditProfile()
{
  UploadProfileDlgS *profileDlg = new UploadProfileDlgS(this);
  fillProfileDlg(profileDlg);
  if (profileDlg->exec())
  {
    readProfileDlg(profileDlg);
    m_project->setModified(true);
    comboProfile->changeItem(profileDlg->lineProfileName->text(), comboProfile->currentItem());
    setProfileTooltip();
  }
  delete profileDlg;
}

void ProjectUpload::slotRemoveProfile()
{
   if (comboProfile->count() == 1)
   {
       KMessageBox::error(this, i18n("You cannot remove the last profile."), i18n("Profile Removal Error") );
   } else
   {
      QString profileName = comboProfile->currentText();
      if (KMessageBox::warningContinueCancel(this, i18n("<qt>Do you really want to remove the <b>%1</b> upload profile?</qt>").arg(profileName),
                                   i18n("Profile Removal"), KStdGuiItem::del()) == KMessageBox::Continue)
       {
          m_profilesNode.removeChild(m_currentProfileElement);
          int idx = comboProfile->currentItem();
          int newIdx = idx + 1;
          if (newIdx >= comboProfile->count())
            newIdx = idx - 1;
          comboProfile->setCurrentItem(newIdx);
          QString currentProfile = comboProfile->currentText();
          slotNewProfileSelected(currentProfile);
          if (profileName == defaultProfile())
          {
              KMessageBox::information(this, i18n("<qt>You have removed your default profile.<br>The new default profile will be <b>%1</b>.</qt>").arg(currentProfile), i18n("Profile Removal"));
              m_profilesNode.toElement().setAttribute("defaultProfile", currentProfile);
          }
          comboProfile->removeItem(idx);
          m_project->setModified(true);
       }
   }
  buttonRemoveProfile->setEnabled(comboProfile->count() > 1);
}

void ProjectUpload::fillProfileDlg(UploadProfileDlgS *profileDlg)
{
  profileDlg->lineProfileName->setText(m_currentProfileElement.attribute("name",""));
  profileDlg->lineHost->setText(m_currentProfileElement.attribute("remote_host",""));
  profileDlg->lineUser->setText(m_currentProfileElement.attribute("user",""));
  profileDlg->linePath->setText(m_currentProfileElement.attribute("remote_path",""));
  profileDlg->port->setText( m_currentProfileElement.attribute("remote_port","") );
  QString def_p = m_currentProfileElement.attribute("remote_protocol","ftp");

  QStringList protocols = KProtocolInfo::protocols();
  protocols.sort();
  for ( uint i=0; i<protocols.count(); i++ )
  {
    QString protocol = protocols[i];
    KURL p;
    p.setProtocol(protocol);
    if ( KProtocolInfo::supportsWriting(p) &&
         KProtocolInfo::supportsMakeDir(p) &&
         KProtocolInfo::supportsDeleting(p) )
    {
      profileDlg->comboProtocol->insertItem(protocol);
      if ( protocol == def_p )
        profileDlg->comboProtocol->setCurrentItem(profileDlg->comboProtocol->count()-1 );
    }
  }
  QString entry = profileDlg->comboProtocol->currentText() + "://" + profileDlg->lineUser->text() + "@" + profileDlg->lineHost->text();
  if (m_project->keepPasswd || m_project->passwordSaved(entry))
  {
    profileDlg->linePasswd->insert(m_project->password(entry));
    profileDlg->keepPasswd->setChecked(m_project->passwordSaved(entry));
 } else
  {
    profileDlg->linePasswd->clear();
    profileDlg->keepPasswd->setChecked(false);
  }
  if (m_profilesNode.toElement().attribute("defaultProfile") ==  profileDlg->lineProfileName->text())
    profileDlg->defaultProfile->setChecked(true);
}

void ProjectUpload::readProfileDlg(UploadProfileDlgS *profileDlg)
{
  QString path = profileDlg->linePath->text();
  if (path.startsWith("~/"))
  {
    KUser user;
    path = user.homeDir() + path.mid(1);
  }
  m_currentProfileElement.setAttribute("name", profileDlg->lineProfileName->text());
  m_currentProfileElement.setAttribute("remote_host", profileDlg->lineHost->text());
  m_currentProfileElement.setAttribute("user", profileDlg->lineUser->text());
  m_currentProfileElement.setAttribute("remote_path", path);
  m_currentProfileElement.setAttribute("remote_port", profileDlg->port->text());
  m_currentProfileElement.setAttribute("remote_protocol", profileDlg->comboProtocol->currentText());
  QString passwd = QString(profileDlg->linePasswd->password());
  m_project->savePassword(profileDlg->comboProtocol->currentText() + "://" + profileDlg->lineUser->text() + "@" + profileDlg->lineHost->text(), passwd,  profileDlg->keepPasswd->isChecked());
  m_lastEditedProfileElement = m_currentProfileElement;
  m_lastPassword = passwd;
  if (profileDlg->defaultProfile->isChecked())
    m_profilesNode.toElement().setAttribute("defaultProfile", profileDlg->lineProfileName->text());
}

void ProjectUpload::slotNewProfileSelected(const QString& profileName)
{
  QDomNodeList profileList = m_profilesNode.toElement().elementsByTagName("profile");
  QDomElement e;
  QString s;
  for (uint i = 0; i < profileList.count(); i++)
  {
      e = profileList.item(i).toElement();
      s = e.attribute("name");
      if (s == profileName)
      {
        m_currentProfileElement = e;
        break;
      }
  }
  m_project->setModified(true);
  setProfileTooltip();
  if (!m_profilesOnly)
  {
    list->clear();
    slotBuildTree();
  }
}

QString ProjectUpload::defaultProfile()
{
  return m_profilesNode.toElement().attribute("defaultProfile");
}

void ProjectUpload::reject()
{
  if (uploadInProgress && !m_profilesOnly)
  {
    suspendUpload = true;
    //TODO when message freeze if lift: i18n("Upload") -> i18n("Continue") or "Continue upload"
    if (KMessageBox::questionYesNo(this,i18n("Do you really want to abort the upload?"),
                                   i18n("Abort Upload"), i18n("Abort the uploading", "Abort"), i18n("Upload")) == KMessageBox::No)
    {
      suspendUpload = false;
      emit uploadNext();
      return;
    }
  }

  QDialog::reject();
}


void ProjectUpload::setProfileTooltip()
{
  QString tip = m_currentProfileElement.attribute("remote_protocol","ftp") + "://";
  QString user = m_currentProfileElement.attribute("user","");
  if (! user.isEmpty()) {
    tip += user + "@";
  }
  tip += m_currentProfileElement.attribute("remote_host","");
  QString port = m_currentProfileElement.attribute("remote_port","");
  if (! port.isEmpty()) {
    tip += ":" + port;
  }
  tip += m_currentProfileElement.attribute("remote_path","");

  QToolTip::add(comboProfile, tip);
}

void ProjectUpload::loadRemoteUploadInfo()
{
    QDomNodeList nl = m_currentProfileElement.elementsByTagName("uploadeditem");
    for (uint i = 0; i < nl.count(); i++)
    {
      QDomElement el = nl.item(i).toElement();
      m_uploadTimeList[el.attribute("url")] = el.attribute("upload_time").toInt();
    }
}

void ProjectUpload::saveRemoteUploadInfo()
{
    QDomNode parent = m_currentProfileElement.parentNode();
    QDomNode profileNode = m_currentProfileElement.cloneNode(false);
    parent.removeChild(m_currentProfileElement);
    parent.appendChild(profileNode);
    QMap<QString, int>::ConstIterator it;
    for (it = m_uploadTimeList.constBegin(); it != m_uploadTimeList.constEnd(); ++it)
    {
      QDomElement el = m_uploadStatusDom.createElement("uploadeditem");
      el.setAttribute("url", it.key());
      el.setAttribute("upload_time", it.data());
      profileNode.appendChild(el);
    }
    m_project->setModified(true);
}


#include "projectupload.moc"
