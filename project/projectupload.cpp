/***************************************************************************
                          projectupload.cpp  -  description
                             -------------------
    begin                : Wed Nov 15 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2001, 2002 by Andras Mantia
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
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
#include <kio/netaccess.h>
#include <klocale.h>
#include <klineedit.h>
#include <kcompletion.h>
#include <kprogress.h>

//standard includes
#include <time.h>

//own includes
#include "projectupload.h"
#include "project.h"
#include "../quantacommon.h"
#include "../qextfileinfo.h"
#include "../resource.h"

ProjectUpload::ProjectUpload(Project* prg, const KURL& url, QWidget *parent, const char* name, bool modal, WFlags fl)
  :ProjectUploadS( parent, name, modal, fl)
{
    list->hide();
    initProjectInfo(prg);
    startUrl = url;
    QTimer::singleShot(10, this, SLOT(slotBuildTree())); 
}


ProjectUpload::~ProjectUpload()
{
  delete baseUrl;
}

void  ProjectUpload::initProjectInfo(Project *prg)
{

  p = prg;

  baseUrl = new KURL();

//  list->setMultiSelection(true);

  QDomElement uploadEl = p->dom.firstChild().firstChild().namedItem("upload").toElement();

  lineHost -> setText(uploadEl.attribute("remote_host",""));
  lineUser -> setText(uploadEl.attribute("user",""));
  linePath -> setText(uploadEl.attribute("remote_path",""));
  port -> setText( uploadEl.attribute("remote_port","") );
  QString def_p = uploadEl.attribute("remote_protocol","ftp");
  keepPasswd->setChecked(p->keepPasswd);
  if (p->keepPasswd)
  {
    linePasswd->setText(p->passwd);
  } else
  {
    linePasswd->clear();
  }

  QStringList protocols = KProtocolInfo::protocols();
  protocols.sort();
  for ( uint i=0; i<protocols.count(); i++ )
  {
    QString p = protocols[i];
    if ( KProtocolInfo::supportsWriting(p) &&
         KProtocolInfo::supportsMakeDir(p) &&
         KProtocolInfo::supportsDeleting(p) )
    {
      comboProtocol->insertItem(p);
      if ( p == def_p )
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
 QString s;
 QDomElement el;
 QDomNodeList nl = p->dom.elementsByTagName("item");
 totalProgress->setTotalSteps(nl.count() - 1 );
 totalProgress->setValue(0);
 totalText->setText(i18n("Scanning project files..."));

 KURL u = p->baseURL;
 KURL absUrl = u;
 for (uint i = 0; i < nl.count(); i++)
 {
   el = nl.item(i).toElement();
   s = el.attribute("url");
   if (startUrl.isEmpty() || s.startsWith(strUrl))
   {
     QuantaCommon::setUrl(u, s);
     absUrl.setPath(p->baseURL.path(1)+u.path());
     KIO::NetAccess::stat(absUrl, entry);
     KFileItem item(entry, absUrl, false, true);
     s = QString("%1").arg( (long int)item.size() );
     QListViewItem *it = list->addItem(u, s, item.timeString());
     if ( it != 0 )
     {
       int uploadTime = el.attribute("upload_time","1").toInt();
       int modifiedTime = item.time(KIO::UDS_MODIFICATION_TIME);

       if ( uploadTime < modifiedTime )
       {
         modified.append( u );
         it->setSelected(true);
       }
       totalProgress->setValue(i);
     }
   }
 }
 totalText->setText(i18n("Building the tree..."));
 kapp->processEvents();
 list->checkboxTree();
 list->show();
 totalText->setText(i18n("Total:"));
 totalProgress->setValue(0);
}


void ProjectUpload::buildSelectedItemList()
{
  QListViewItem *item;
  QListViewItemIterator it(list);
  toUpload.clear();
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

     if (!u.isEmpty()) toUpload.append(u);
   }
  }
}

void ProjectUpload::startUpload()
{
  stopUpload = false;
  QDomElement uploadEl = p->dom.firstChild().firstChild().namedItem("upload").toElement();

  uploadEl.setAttribute("remote_host", lineHost->text() );
  uploadEl.setAttribute("remote_path", linePath->text() );
  uploadEl.setAttribute("remote_port", port->text() );
  uploadEl.setAttribute("user", lineUser->text() );
  uploadEl.setAttribute("remote_protocol", comboProtocol->currentText() );

  baseUrl->setProtocol( comboProtocol->currentText() );
  baseUrl->setPort( port->text().toInt() );
  baseUrl->setHost( lineHost->text() );
  baseUrl->setPath( linePath->text() );
  baseUrl->setPass( linePasswd->text() );
  if (keepPasswd->isChecked())
  {
     p->keepPasswd = true;
     p->passwd = linePasswd->text();
  } else
  {
     p->keepPasswd = false;
     p->passwd = "";
  }

  buildSelectedItemList();
  int selectedNum = toUpload.count();
  
  totalProgress->setProgress(0);
  totalProgress->setTotalSteps(selectedNum);
  uploadInProgress = true;
  suspendUpload = false;
  KURL u = *baseUrl;
  u.setPath("");
  u.setUser(lineUser->text());
  if (QExtFileInfo::exists(u))
  {
    upload();
  } else
  {
    if (KMessageBox::warningYesNo(this, i18n("%1 seems to be unaccesible.\nDo you want to proceed with upload?")
                                         .arg(u.prettyURL())) == KMessageBox::Yes)
    {
      upload();
    }                                   
  }
}

void ProjectUpload::upload()
{
  if ( stopUpload ) return;
  QString pass = linePasswd->text();
  QString user = lineUser->text();
  KURL dir;
  KURL to;

  for ( KURL::List::Iterator file = toUpload.begin(); file != toUpload.end(); ++file )
  {
      currentURL = *file;
      
      KURL from = QExtFileInfo::toAbsolute(currentURL, p->baseURL);      
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
      to.setPass( pass );

      dir.setUser( user );
      dir.setPass( pass );

      if ( !madeDirs.contains(dir) )
      {
        madeDirs.append( dir );
        if (!QExtFileInfo::createDir(dir))
        {
          QuantaCommon::dirCreationError(this, dir.prettyURL());
          return;
        }
      }

      //qDebug("%s -> %s", from.url().data(), to.url().data() );
      if (!from.fileName(false).isEmpty())
      {
        KIO::FileCopyJob *job = KIO::file_copy( from, to, -1, true, false, false );

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
    QListViewItem *it = list->findItem( currentURL.path() );
    if (it)
    {
     it->setSelected(false);
     it->repaint();
    }
    toUpload.remove( currentURL );

    //update upload time
    QDomNodeList nl = p->dom.firstChild().firstChild().childNodes();
    for ( unsigned int i = 0; i < nl.count(); i++ )
    {
      QDomElement el = nl.item(i).toElement();
      if ( el.nodeName() == "item"  &&  el.attribute("url") == QuantaCommon::qUrl(currentURL) )
      {
        //QDateTime stime;
        //stime.setTime_t(1);
        //el.setAttribute( "upload_time", stime.secsTo( QDateTime::currentDateTime() ) );
        time_t stime;
        time(&stime);
        el.setAttribute( "upload_time", (int)stime);
        break;
      }
    }
    upload();
  }
}

void ProjectUpload::clearProjectModified()
{
  QDomNodeList nl = p->dom.elementsByTagName("item");
  for ( unsigned int i=0; i<nl.count(); i++ )
  {
    QDomElement el = nl.item(i).toElement();
    QDateTime stime;
    stime.setTime_t(1);
    el.setAttribute( "upload_time", stime.secsTo( QDateTime::currentDateTime() ) );
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
                                   i18n("Cancel upload")) == KMessageBox::No)
    {
        suspendUpload = false;
        emit uploadNext();
      return;
    }
  }

  QDialog::reject();
}


#include "projectupload.moc"
