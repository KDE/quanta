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
#include <kcompletion.h>
#include <kprogress.h>
#include <kpassdlg.h>

//standard includes
#include <time.h>

//own includes
#include "projectupload.h"
#include "project.h"
#include "quantacommon.h"
#include "qextfileinfo.h"
#include "resource.h"

ProjectUpload::ProjectUpload(const KURL& url, const char* name)
  :ProjectUploadS( 0L, name, true, Qt::WDestructiveClose)
{
    list->hide();
    m_project = Project::ref();
    initProjectInfo();
    startUrl = url;
    QTimer::singleShot(10, this, SLOT(slotBuildTree()));
    currentItem = 0L;
}


ProjectUpload::~ProjectUpload()
{
  m_project->setModified(true);
  delete baseUrl;
}

void  ProjectUpload::initProjectInfo()
{
  baseUrl = new KURL();

//  list->setMultiSelection(true);

  QDomElement uploadEl = m_project->dom.firstChild().firstChild().namedItem("upload").toElement();

  lineHost -> setText(uploadEl.attribute("remote_host",""));
  lineUser -> setText(uploadEl.attribute("user",""));
  linePath -> setText(uploadEl.attribute("remote_path",""));
  port -> setText( uploadEl.attribute("remote_port","") );
  QString def_p = uploadEl.attribute("remote_protocol","ftp");
  keepPasswd->setChecked(m_project->keepPasswd);
  if (m_project->keepPasswd)
  {
    linePasswd->insert(m_project->passwd);
  } else
  {
    linePasswd->clear();
  }

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
      comboProtocol->insertItem(protocol);
      if ( protocol == def_p )
        comboProtocol->setCurrentItem( comboProtocol->count()-1 );
    }
  }

//  KCompletion *comp = linePath->completionObject();
//  connect(linePath,SIGNAL(returnPressed(const QString&)),
//                  comp,SLOT(addItem(const QString&)));

  uploadInProgress = false;
  connect( this, SIGNAL( uploadNext() ), SLOT( slotUploadNext() ) );

  lineHost->setFocus();
}

/** No descriptions */
void ProjectUpload::slotBuildTree()
{
 KIO::UDSEntry entry;
 QString strUrl = QuantaCommon::qUrl(startUrl);
 bool isDirectory = strUrl.endsWith("/");
 QString s;
 QDomElement el;
 QDomNodeList nl = m_project->dom.elementsByTagName("item");
 totalProgress->setTotalSteps(nl.count() - 1 );
 totalProgress->setValue(0);
 totalText->setText(i18n("Scanning project files..."));

 QDict<KFileItem> projectDirFiles = QExtFileInfo::allFilesDetailed(m_project->projectBaseURL(), "*");

 KURL u = m_project->projectBaseURL();
 KURL absUrl = u;
 for (uint i = 0; i < nl.count(); i++)
 {
   el = nl.item(i).toElement();
   s = el.attribute("url");
   if (startUrl.isEmpty() || (s.startsWith(strUrl) && isDirectory) || s == strUrl)
   {
     QuantaCommon::setUrl(u, s);
     absUrl.setPath(m_project->projectBaseURL().path(1)+u.path());
     KFileItem *p_item = projectDirFiles.find(absUrl.url());
     if (!p_item)
       continue;
     KFileItem item(*p_item);
     /*
     KIO::NetAccess::stat(absUrl, entry);
     KFileItem item(entry, absUrl, false, true); */
     s = QString("%1").arg( (long int)item.size() );
     UploadTreeFile *it = list->addItem(u, item);
     if ( it != 0 )
     {
       int uploadTime = el.attribute("upload_time","1").toInt();
       int modifiedTime = item.time(KIO::UDS_MODIFICATION_TIME);
       el.setAttribute("modified_time", modifiedTime);
       //kdDebug(24000) << "Last upload at: " << uploadTime << endl;
       //kdDebug(24000) << "Last modified at: " << modifiedTime << endl;
       int uploadStatus = el.attribute("uploadstatus").toInt();
       if ( uploadTime < modifiedTime && uploadStatus != 0)
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
 projectDirFiles.setAutoDelete(false);
 totalText->setText(i18n("Building the tree..."));
// kapp->eventLoop()->processEvents( QEventLoop::ExcludeUserInput | QEventLoop::ExcludeSocketNotifiers);
 list->checkboxTree();
 if (!startUrl.isEmpty())
      expandAll();
 list->show();
 totalText->setText(i18n("Total:"));
 totalProgress->setTotalSteps(1);
 totalProgress->setValue(0);
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

void ProjectUpload::startUpload()
{
  stopUpload = false;
  QDomElement uploadEl = m_project->dom.firstChild().firstChild().namedItem("upload").toElement();
  QString path = linePath->text();
  if (!path.startsWith("/"))
    path.prepend("/");
  uploadEl.setAttribute("remote_host", lineHost->text() );
  uploadEl.setAttribute("remote_path", path);
  uploadEl.setAttribute("remote_port", port->text() );
  uploadEl.setAttribute("user", lineUser->text() );
  uploadEl.setAttribute("remote_protocol", comboProtocol->currentText() );
  m_project->setModified(true);

  baseUrl->setProtocol(comboProtocol->currentText());
  baseUrl->setPort(port->text().toInt());
  baseUrl->setHost(lineHost->text());
  baseUrl->setPath(path);
  password = linePasswd->password();
  baseUrl->setPass(password);
  if (keepPasswd->isChecked())
  {
     m_project->keepPasswd = true;
     m_project->passwd = password;
  } else
  {
     m_project->keepPasswd = false;
     m_project->passwd = "";
  }

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
    QDomNodeList nl = m_project->dom.elementsByTagName("item");
    QDomElement el;
    for ( uint i = 0; i < nl.count(); i++ )
    {
      el = nl.item(i).toElement();
      if ( selectedList.contains(el.attribute("url")))
      {
        el.setAttribute( "upload_time", el.attribute("modified_time") );
      }
    }
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
    user = lineUser->text();
    KURL u = *baseUrl;
    u.setPath("");
    u.setUser(user);
    if (QExtFileInfo::exists(u))
    {
      upload();
      return;
    } else
    {
      if (KMessageBox::warningYesNo(this, i18n("<qt><b>%1</b> seems to be unaccessible.<br>Do you want to proceed with upload?</qt>")
                                          .arg(u.prettyURL(0, KURL::StripFileProtocol))) == KMessageBox::Yes)
      {
        upload();
        return;
      }
    }
  }
  uploadInProgress = false;
}

void ProjectUpload::upload()
{
  if ( stopUpload ) return;
  KURL dir;
  KURL to;
  UploadTreeFile *fileItem;
  UploadTreeFolder *folderItem;

  uint toUploadCount = toUpload.count();
  currentItem = 0L;
  for (uint i = 0; i < toUploadCount; i++)
  {
      currentItem = toUpload.at(i);
      if (dynamic_cast<UploadTreeFile*>(currentItem))
      {
        fileItem = dynamic_cast<UploadTreeFile*>(currentItem);
        folderItem = 0L;
        currentURL = fileItem->url();
      } else
      {
        fileItem = 0L;
        folderItem = dynamic_cast<UploadTreeFolder*>(currentItem);
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

      to.setUser( user );
      to.setPass( password );

      dir.setUser( user );
      dir.setPass( password );

      if ( !madeDirs.contains(dir) )
      {
        madeDirs.append( dir );
        if (!QExtFileInfo::createDir(dir))
        {
          QuantaCommon::dirCreationError(this, KURL( dir.prettyURL(0, KURL::StripFileProtocol) ));
          return;
        }
      }

     // qDebug("%s -> %s", from.url().data(), to.url().data() );
      if (!from.fileName(false).isEmpty() && fileItem)
      {
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
  }
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
      return;
   }
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
     it->repaint();
    }
    toUpload.remove( it );

    //update upload time
    QDomNodeList nl = m_project->dom.elementsByTagName("item");
    QDomElement el;
    for ( uint i = 0; i < nl.count(); i++ )
    {
      el = nl.item(i).toElement();
      if ( el.attribute("url") == QuantaCommon::qUrl(currentURL) )
      {
        el.setAttribute( "upload_time", el.attribute("modified_time") );
      //  kdDebug(24000) << "Upload time for " << el.attribute("url") << " is: " << el.attribute("upload_time") << "\n";
        break;
      }
    }

    upload();
  }
}

void ProjectUpload::clearProjectModified()
{
  QDomNodeList nl = m_project->dom.elementsByTagName("item");
  for ( unsigned int i=0; i<nl.count(); i++ )
  {
    QDomElement el = nl.item(i).toElement();
    el.setAttribute( "upload_time", el.attribute("modified_time"));
  }
  modified.clear();
  list->clearSelection();
  list->checkboxTree();
}

void ProjectUpload::reject()
{

  if (uploadInProgress)
  {
    suspendUpload = true;
    if (KMessageBox::questionYesNo(this,i18n("Do you really want to cancel the upload?"),
                                   i18n("Cancel Upload")) == KMessageBox::No)
    {
      suspendUpload = false;
      emit uploadNext();
      return;
    }
  }

  QDialog::reject();
}


#include "projectupload.moc"
