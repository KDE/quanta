/***************************************************************************
                          projectupload.cpp  -  description
                             -------------------
    begin                : Wed Nov 15 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                              (C) 2001 by Andras Mantia
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

#include <iostream.h>

 //own includes
#include "projectupload.h"
#include "project.h"

//qt includes
#include <qdatetime.h>
#include <qlistview.h>
#include <qfileinfo.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qstringlist.h>
#include <qregexp.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qprogressbar.h>
#include <qcombobox.h>
#include <qurl.h>
#include <qcheckbox.h>

//kde includes
#include <kurl.h>
#include <kio/job.h>
#include <kmessagebox.h>
#include <kprotocolinfo.h>
#include <kdebug.h>
#include <kio/netaccess.h>
#include <klocale.h>
#include <klineedit.h>
#include <kcompletion.h>


ProjectUpload::ProjectUpload(QString file, Project* prg, QWidget *parent, const char* name, bool modal, WFlags fl)
  :ProjectUploadS( parent, name, modal, fl)
{
    initProjectInfo(prg);

    QFileInfo fi( p->basePath + file );

    if (fi.isDir())
    {
	   	kdDebug() << file << " is a directory" << endl;
  		QDomNodeList nl = p->dom.firstChild().firstChild().childNodes();

  		QDateTime stime;
  		stime.setTime_t(1);

  		for ( unsigned int i=0; i<nl.count(); i++ )
  		{
  		    QDomElement el = nl.item(i).toElement();
  	    	if ( el.nodeName() == "item" )
  	    	{
    				QString url = el.attribute("url");
    				if (url.contains(file) > 0)
    				{
    					files.append( url );
    					QFileInfo fi( p->basePath + url );

    					QString size;
    					size.sprintf( "%i", fi.size() );

    					QDate d = fi.lastModified().date();
    					QString date;
    					date.sprintf( "%4i.%2i.%2i", d.year(), d.month(), d.day() );
    					date.replace( QRegExp(" "), "0" );

    					//QListViewItem *it = new QListViewItem( list, url, date, size );
    					QListViewItem *it = list->addItem( url, size, date );

              if ( it != 0 )
              {
                int uploadTime = el.attribute("upload_time","1").toInt();
                int modifiedTime = stime.secsTo( fi.lastModified() );

                if ( uploadTime < modifiedTime )
                {
                  modified.append( url );
                  it->setSelected(true);
                }
              }
            }
          }
      } //for
    } else  //it is not a directory
    {
   		kdDebug() << file << " is a file" << endl;

          files.append( file );

  		QString size;
  		size.sprintf( "%i", fi.size() );

  		QDate d = fi.lastModified().date();
  		QString date;
  		date.sprintf( "%4i.%2i.%2i", d.year(), d.month(), d.day() );
  		date.replace( QRegExp(" "), "0" );

  		//QListViewItem *it = new QListViewItem( list, file, size, date );
  		//QListViewItem *it = list->addItem( url, size, date );
  		QListViewItem *it = list->addItem( file, size, date );

  		modified.append( file );
  		it->setSelected(true);
    }
 list->slotSelectFile();
}


ProjectUpload::ProjectUpload( Project* prg, QWidget* parent,  const char* name, bool modal, WFlags fl )
  :ProjectUploadS( parent,  name, modal, fl )
{

	initProjectInfo(prg);

  //list->addDir(p->basePath);

	QDomNodeList nl = p->dom.firstChild().firstChild().childNodes();

	QDateTime stime;
	stime.setTime_t(1);

	for ( unsigned int i=0; i<nl.count(); i++ )
	{
	    QDomElement el = nl.item(i).toElement();
	    if ( el.nodeName() == "item" )
	    {
	      	QString url = el.attribute("url");

	        files.append( url );

	      	QFileInfo fi( p->basePath + url );
	        QString size;
	      	size.sprintf( "%i", fi.size() );

	      	QDate d = fi.lastModified().date();
	      	QString date;
	      	date.sprintf( "%4i.%2i.%2i", d.year(), d.month(), d.day() );
	      	date.replace( QRegExp(" "), "0" );

				QListViewItem *it = list->addItem( url, size, date );
        if ( it != 0 )
        {
  				int uploadTime = el.attribute("upload_time","1").toInt();
  				int modifiedTime = stime.secsTo( fi.lastModified() );

  	     	if ( uploadTime < modifiedTime )
  	     	{
  					modified.append( url );
  					it->setSelected(true);
  					// Find this node and highlight it.
  					//UploadTreeFile *it = list->getNode
          }
        }
	    }
	} //for
 list->slotSelectFile();
}

ProjectUpload::~ProjectUpload()
{
  delete baseUrl;
}

void  ProjectUpload::initProjectInfo(Project *prg)
{

	p = prg;

	baseUrl = new KURL();

	list->setMultiSelection(true);

	list->setColumnAlignment(1,Qt::AlignRight);
	list->setColumnAlignment(2,Qt::AlignRight);
	list->setShowSortIndicator (true);

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

int ProjectUpload::selectedItemCount( QListViewItem *item, int numSelected )
{
  for ( QListViewItem *it = item; it; it = it->nextSibling() ) {
    if ( it->childCount() > 0 ) {
      numSelected += selectedItemCount( it->firstChild(), numSelected );
    }
    else {
      if ( it->isSelected() ) numSelected++;
    }
  }
  return numSelected;
}

void ProjectUpload::buildSelectedItemList( QListViewItem *item, QString curPath )
{
	for ( QListViewItem *it = item; it; it = it->nextSibling() ) {
	if ( it->childCount() > 0 ) {
			if ( curPath == "" ) buildSelectedItemList( it->firstChild(), it->text(0) );
			else buildSelectedItemList( it->firstChild(), curPath + "/" + it->text(0) );
		}
		else {
			if ( it->isSelected() )
			{
				if ( curPath == "" ) toUpload.append( it->text(0) );
				else toUpload.append( curPath + "/" + it->text(0) );
			}
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
	if (keepPasswd->isChecked())
	{
	   p->keepPasswd = true;
	   p->passwd = linePasswd->text();
   	   baseUrl->setPass( linePasswd->text() );
	} else
	{
	   p->keepPasswd = false;
	   p->passwd = "";
	}

	int selectedNum = selectedItemCount( list->firstChild());

	buildSelectedItemList( list->firstChild(), "" );

	totalProgress->setProgress(0);
	totalProgress->setTotalSteps(selectedNum);
	uploadInProgress = true;
	suspendUpload = false;
	upload();
}

void ProjectUpload::upload()
{
  cout << "filename: " << *(toUpload.begin()) << endl;
	if ( stopUpload ) return;
	QString pass = linePasswd->text();
	QString user = lineUser->text();
	QString dirStr;
	int pos;
  KURL dir;
  KURL to;

//	for ( QListViewItem *it = list->firstChild(); it; it = it->nextSibling() )
	for ( QStringList::Iterator file = toUpload.begin(); file != toUpload.end(); ++file )
	{
			//These files are already selected, so we don't have to check for ->isSelected().
			//currentFile = it->text(0);
			currentFile = *file;
			// :TODO: Duplicate this function on the tree structure.
			//list->ensureItemVisible(it);

			KURL from;
			from.setPath( p->basePath + currentFile );
			to = *baseUrl;
			to.addPath( currentFile );
			if (to.fileName(false).isEmpty()) dir = to;
			else dir = to.upURL() ;

			to.setUser( user );
			to.setPass( pass );

			dir.setUser( user );
			dir.setPass( pass );
			dirStr = dir.url();

			if ( !madeDirs.contains(dirStr) )
			{
				madeDirs.append( dirStr );
				pos = dirStr.find("/",1);
				while ( pos != -1)
				{
cout << "Trying to make: " << dirStr.left(pos) << endl;
					KIO::NetAccess::mkdir(dirStr.left(pos));
					pos = dirStr.find("/",pos+1);
				}
				KIO::NetAccess::mkdir(dirStr);
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

				labelCurFile->setText(i18n("Current: %1").arg(currentFile));
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
  	labelCurFile->setText( currentFile + " : " + msg );
}

void ProjectUpload::selectAll()
{
	list->selectAll(true);
  list->slotSelectFile();
}

void ProjectUpload::selectModified()
{
  for ( QStringList::Iterator file = modified.begin(); file != modified.end(); ++file )
  {
    QListViewItem *it = list->findItem( *file );
    it->setSelected(true);
    it->repaint();
  }
  list->slotSelectFile();
}

void ProjectUpload::clearSelection()
{
	list->selectAll(false);
  list->slotSelectFile();
}

void ProjectUpload::invertSelection()
{
  list->invertAll();
  list->slotSelectFile();
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
  	QListViewItem *it = list->findItem( currentFile );
  	it->setSelected(false);
  	it->repaint();
    toUpload.remove( currentFile );

  	//update upload time
  	QDomNodeList nl = p->dom.firstChild().firstChild().childNodes();
  	for ( unsigned int i=0; i<nl.count(); i++ )
  	{
  		QDomElement el = nl.item(i).toElement();
  		if ( el.nodeName() == "item"  &&  el.attribute("url") == currentFile )
  		{
  			QDateTime stime;
  			stime.setTime_t(1);
  			el.setAttribute( "upload_time", stime.secsTo( QDateTime::currentDateTime() ) );
  		}
  	}
  	upload();
  }
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
