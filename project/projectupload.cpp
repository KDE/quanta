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
#include <qurloperator.h>
#include <qcstring.h>
#include <qspinbox.h>
#include <qlistview.h>
#include <qprogressbar.h>

#include <kurl.h>
#include <qurl.h>
#include <kio/job.h>
#include <kmessagebox.h>

ProjectUpload::ProjectUpload( Project* prg, QWidget* parent,  const char* name, bool modal, WFlags fl )
  :ProjectUploadS( parent,  name, modal, fl )
{
	p = prg;
	
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
	spinPort -> setValue( uploadEl.attribute("remote_port","21").toInt() );
	
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

ProjectUpload::~ProjectUpload(){
}

void ProjectUpload::startUpload()
{
	stopUpload = false;
	QDomElement uploadEl = p->dom.firstChild().firstChild().namedItem("upload").toElement();
	
	uploadEl.setAttribute("remote_host", lineHost->text() );
	uploadEl.setAttribute("remote_path", linePath->text() );
	uploadEl.setAttribute("remote_port", spinPort->value() );
	uploadEl.setAttribute("user", lineUser->text() );
	
	
	QString url = "ftp://" + lineHost->text();
	
	op = url;
	op.setPath( linePath->text() );
	op.setUser( lineUser->text() );
	op.setPassword( linePasswd->text() );
	op.setPort(21);
	
	connect(&op, SIGNAL(finished(QNetworkOperation*)), this, SLOT(uploadFinished(QNetworkOperation*)));
	connect(&op, SIGNAL(dataTransferProgress(int,int,QNetworkOperation*)), this, SLOT(uploadProgress(int,int,QNetworkOperation*)));
	
	upload();
}

void ProjectUpload::upload()
{
	if ( stopUpload ) return;
		
  for ( QListViewItem *it = list->firstChild(); it; it = it->nextSibling() ) {
  		if ( it->isSelected() ) {	
  			currentFile = it->text(0);
  			list->ensureItemVisible(it);
  			
  			QString dir = QUrl(currentFile).dirPath();
  			dir.truncate( dir.length()-1 );  // remove '/' from end of dir
  			
  			QString sfrom = QUrl(p->basePath + currentFile);
  			QUrl sto(op, dir);
  			
  			if ( !madeDirs.contains( dir ) && dir != "." ) {
  				currentFile = "";
  				madeDirs.append(dir);
  				op.mkdir(dir);
  				return;
  			}
  			
  			qDebug("%s -> %s", QString(sfrom).data(), QString(sto).data() );
  			//debug( QString(op) );
  			
 			op.copy( sfrom, sto );
 			// op.copy( sfrom, op );
 			
  			labelCurFile->setText( currentFile );
  			ProgressBar1->setProgress( 0 );
  			return;
  		}
  }
  reject();

}

void ProjectUpload::uploadFinished( QNetworkOperation *res )
{
   if ( !res )  	return;
   if ( res->state() == QNetworkProtocol::StFailed) {
   	 // KMessageBox::error(this, res->protocolDetail() );
   	 // stopUpload = true;
   	 debug( res->protocolDetail() );
   	 return;
   }

   if ( res->state() == QNetworkProtocol::StDone) {

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
}

void ProjectUpload::uploadProgress (int bytesDone, int bytesTotal, QNetworkOperation * ) {
	ProgressBar1->setProgress( int( 100*bytesDone/bytesTotal ) );
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

