/***************************************************************************
                          projectupload.cpp  -  description
                             -------------------
    begin                : Wed Nov 15 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
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

#include "projectupload.h"
#include <qdatetime.h>
#include "project.h"
#include <qlistview.h>
#include <qfileinfo.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qstringlist.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qprogressbar.h>
#include <qcombobox.h>

#include <kurl.h>
#include <qurl.h>
#include <kio/job.h>
#include <kmessagebox.h>
#include <kprotocolinfo.h>


ProjectUpload::ProjectUpload( Project* prg, QWidget* parent,  const char* name, bool modal, WFlags fl )
  :ProjectUploadS( parent,  name, modal, fl )
{
	p = prg;
	
	baseUrl = new KURL();
	
	list->setMultiSelection(true);
	
	list->setColumnAlignment(1,Qt::AlignRight);
	list->setColumnAlignment(2,Qt::AlignRight);
	list->setShowSortIndicator (true);
	
	QDomNodeList nl = p->dom.firstChild().firstChild().childNodes();
	
	QDateTime stime;
	stime.setTime_t(1);
	
	QDomElement uploadEl = p->dom.firstChild().firstChild().namedItem("upload").toElement();
	
	lineHost -> setText(uploadEl.attribute("remote_host",""));
	lineUser -> setText(uploadEl.attribute("user",""));
	linePath -> setText(uploadEl.attribute("remote_path",""));
	port -> setText( uploadEl.attribute("remote_port","") );
	QString def_p = uploadEl.attribute("remote_protocol","ftp");
	
	QStringList protocols = KProtocolInfo::protocols();
	for ( uint i=0; i<protocols.count(); i++ ) {
	  QString p = protocols[i];
	  if ( KProtocolInfo::supportsWriting(p) &&
	       KProtocolInfo::supportsMakeDir(p) &&
	       KProtocolInfo::supportsDeleting(p) ) {
       comboProtocol->insertItem(p);
       if ( p == def_p )
         comboProtocol->setCurrentItem( comboProtocol->count()-1 );
    }
	}
	
	for ( unsigned int i=0; i<nl.count(); i++ )
  {
		QDomElement el = nl.item(i).toElement();
		if ( el.nodeName() == "item" )	{
			QString url = el.attribute("url");
			files.append( url );
			
			QFileInfo fi( p->basePath + url );
			
			QString size;
      size.sprintf( "%i", fi.size() );
      
      QDate d = fi.lastModified().date();
      QString date;
      
      date.sprintf( "%4i.%2i.%2i", d.year(), d.month(), d.day() );
			date.replace( QRegExp(" "), "0" );
      
			QListViewItem *it = new QListViewItem( list, url, date, size );
			
			int uploadTime = el.attribute("upload_time","1").toInt();
			int modifiedTime = stime.secsTo( fi.lastModified() );
			
			if ( uploadTime < modifiedTime ) {
				 modified.append( url );
				 it->setSelected(true);
			}
			
		}
  }

}

ProjectUpload::~ProjectUpload()
{
  delete baseUrl;
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
	
	debug( baseUrl->url() );
	
	upload();
}

void ProjectUpload::upload()
{
	if ( stopUpload ) return;
	QString pass = linePasswd->text();
	QString user = lineUser->text();
	
  for ( QListViewItem *it = list->firstChild(); it; it = it->nextSibling() ) {
  		if ( it->isSelected() ) {	
  			currentFile = it->text(0);
  			list->ensureItemVisible(it);                                                
  			
  			
  			KURL from; 
  			from.setPath(p->basePath + currentFile);
  			
      KURL to( *baseUrl );
      to.addPath( currentFile );
      
      KURL dir( to.upURL() );
      
      to.setUser( user );
      to.setPass( pass );
      
      dir.setUser( user );
      dir.setPass( pass );
			
  			if ( !madeDirs.contains(dir.url()) ) {
  				//currentFile = "";
  				madeDirs.append( dir.url() );
  				KIO::Job *job = KIO::mkdir(dir);
  				/*
  			  connect( job, SIGNAL( result( KIO::Job * ) ),
              this, SLOT( uploadFinished( KIO::Job * ) ) );
        connect( job, SIGNAL( percent( KIO::Job *,unsigned long ) ),
              this, SLOT( uploadProgress( KIO::Job *,unsigned long ) ) );
        connect( job, SIGNAL( infoMessage( KIO::Job *,const QString& ) ),
              this, SLOT( uploadMessage( KIO::Job *,const QString& ) ) );

  				return;
  				*/
  			} 
			
  			
  			qDebug("%s -> %s", from.url().data(), to.url().data() );
  			qDebug( dir.url() );
  			
      KIO::FileCopyJob *job = KIO::file_copy( from, to, -1, true, false, false );
      
  			connect( job, SIGNAL( result( KIO::Job * ) ),
              this, SLOT( uploadFinished( KIO::Job * ) ) );
      connect( job, SIGNAL( percent( KIO::Job *,unsigned long ) ),
              this, SLOT( uploadProgress( KIO::Job *,unsigned long ) ) );
      connect( job, SIGNAL( infoMessage( KIO::Job *,const QString& ) ),
              this, SLOT( uploadMessage( KIO::Job *,const QString& ) ) );
 			
  			labelCurFile->setText( currentFile );
  			ProgressBar1->setProgress( 0 );
			return;
  		}
  }
  reject();

}

void ProjectUpload::uploadFinished( KIO::Job *job )
{
   if ( !job )  	return;
   
   if ( job->error() ) {
      job->showErrorDialog( this  );
      return;
   }

     for ( QListViewItem *it = list->firstChild(); it; it = it->nextSibling() )
    		if ( it->text(0) == currentFile ) {
    			it->setSelected(false);
    			it->repaint();
      			
    			//update upload time
    			QDomNodeList nl = p->dom.firstChild().firstChild().childNodes();
    			for ( unsigned int i=0; i<nl.count(); i++ ) {
						QDomElement el = nl.item(i).toElement();
						if ( el.nodeName() == "item"  &&  el.attribute("url") == currentFile ) {
							QDateTime stime;
							stime.setTime_t(1);
							el.setAttribute( "upload_time", stime.secsTo( QDateTime::currentDateTime() ) );
						}
				}
    		}
   	upload();

}

void ProjectUpload::uploadProgress ( KIO::Job *, unsigned long percent  ) 
{
	ProgressBar1->setProgress( percent );
}

void ProjectUpload::uploadMessage ( KIO::Job *, const QString & msg )
{
  labelCurFile->setText( currentFile + " : " + msg );
  debug("msg:"+currentFile + " : " + msg);
}

void ProjectUpload::selectAll()
{
	list->selectAll(true);
}

void ProjectUpload::selectModified()
{
	for ( QListViewItem *it = list->firstChild(); it; it = it->nextSibling() )
		it->setSelected( modified.contains(it->text(0)) );

}

void ProjectUpload::clearSelection()
{
	list->selectAll(false);
}

void ProjectUpload::resizeEvent ( QResizeEvent *t )
{
  ProjectUploadS::resizeEvent(t);
  list->setColumnWidth(0,list->width()-list->columnWidth(1)-list->columnWidth(2)-20);
}

