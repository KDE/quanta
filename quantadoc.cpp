/***************************************************************************
                          quantadoc.cpp  -  description
                             -------------------
    begin                : Втр Май  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
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

// include files for Qt
#include <qdir.h>
#include <qfileinfo.h>
#include <qwidget.h>
#include <qtabwidget.h>
#include <qtabbar.h>

// include files for KDE
#include <klocale.h>
#include <kmessagebox.h>

// application specific includes
#include "quantadoc.h"
#include "quanta.h"
#include "quantaview.h"

#include "document.h"
#include "kwrite/kwdoc.h"
#include "kwrite/highlight/highlight.h"

QList<QuantaView> *QuantaDoc::pViewList = 0L;

QuantaDoc::QuantaDoc( QuantaApp *app, QWidget *parent, const char *name) : QObject(parent, name)
{
	this->app = app;	

  if(!pViewList)
  {
    pViewList = new QList<QuantaView>();
  }

  pViewList->setAutoDelete(true);

}

QuantaDoc::~QuantaDoc()
{
}

void QuantaDoc::addView(QuantaView *view)
{
  pViewList->append(view);
}

void QuantaDoc::removeView(QuantaView *view)
{
  pViewList->remove(view);
}

QString QuantaDoc::getAbsFilePath()
{
  QString fname = write()->fileName();

  if ( fname.left(5) == "file:" ) fname.remove(0,5);

  return fname;
}

void QuantaDoc::setTitle(const QString &_t)
{
  title=_t;
}

QString QuantaDoc::getTitle()
{
  if ( docList.count() )
  	return QString("[")+getAbsFilePath()+"]";
  else
		return title;
}

void QuantaDoc::slotUpdateAllViews(QuantaView *sender)
{
  QuantaView *w;
  if(pViewList)
  {
    for(w=pViewList->first(); w!=0; w=pViewList->next())
    {
      if(w!=sender)
        w->repaint();
    }
  }

}

/** change file name in dict and tabbar  from old ones to current */
void QuantaDoc::changeFileName( QString oldname )
{
	QString newname = write()->fileName();
  if ( newname.left(5) == "file:" ) newname.remove(0,5);
  if ( oldname.left(5) == "file:" ) oldname.remove(0,5);

  docList.remove( oldname );
  docList.insert( newname, write() );

  QDictIterator<Document> it1(docList);
  QDictIterator<Document> it2(docList);

 	int i,len;
 	while ( it1.current() )
 	{
 		QString name1 = it1.currentKey();
 		
 		len = 0;
 		i = name1.findRev( '/' );
 		
 		if ( i!=-1 ) len = name1.length()-i-1;
 		else		 		 len = name1.length();
 		
 		it2.toFirst();
 		while ( it2.current() && i != -1)
 		{
 			QString name2 = it2.currentKey();
 			if ( name1 != name2 )
 			{
 				while ( name1.right( len ) == name2.right( len ) && i != -1)
 				{
 					i = name1.findRev( '/', i-1 );
 					len = name1.length()-i-1;
 				}
 			}
 			
 			++it2;
 		}
 		
 		QString shortName = name1.right( len );
 		
 		app->view->writeTab->changeTab( it1.current() , shortName );
 		
 		++it1;
 	}
}


/** save all files */
bool QuantaDoc::saveAll(bool dont_ask){
  bool flagsave = true;

  QDictIterator<Document> it( docList ); // iterator for dict

  while ( Document *twrite = it.current() ) {
    ++it;

    if ( twrite->isModified() ) {
      app->view->writeTab->showPage(twrite);

      QString oldname = twrite->fileName();

      if ( dont_ask ) {
      	twrite->save();
      	if ( twrite->isModified() ) flagsave = false;
      }
      else
      	if ( !saveModified() ) flagsave = false;

      changeFileName( oldname );


    }
  }

  return flagsave;
}



bool QuantaDoc::saveModified()
{
  bool completed=true;

  if( isModified() )
  {
    QuantaApp *win=(QuantaApp *) parent();
    int want_save = KMessageBox::warningYesNoCancel(win,
                                             i18n("The current file has been modified.\n"
                                                  "Do you want to save it?"),
                                             i18n("Warning"));
    switch(want_save)
    {
      case KMessageBox::Yes :
           if ( !write()->hasFileName() )
           {
             win->slotFileSaveAs();
           }
           else
           {
             saveDocument( write()->fileName() );
       	   };

       	   deleteContents();
           completed=true;
           break;

      case KMessageBox::No :
           setModified(false);
           deleteContents();
           completed=true;
           break;	

      case KMessageBox::Cancel :
           completed=false;
           break;

      default:
           completed=false;
           break;
    }
  }

  return completed;
}

void QuantaDoc::closeDocument()
{
	
	if ( !saveModified() )
	  return;
	
	QString fname = write()->fileName();
	
	if ( fname.left(5) == "file:" ) fname.remove(0,5);
	
	docList.remove( fname );
	
	if ( !( app->view->removeWrite() ) )
    newDocument();

 	fname = write()->fileName();
 	
  QFileInfo fileInfo(fname);
  title=fileInfo.fileName();
}

void QuantaDoc::closeAll()
{
	QDictIterator<Document> it( docList ); // iterator for dict

	QString fname;
	
  do
  {
  	if ( !saveModified() ) return;
  	
  	fname = write()->fileName();
		if ( fname.left(5) == "file:" ) fname.remove(0,5);
		docList.remove( fname );
	}
	while ( app->view->removeWrite() ) ;

	newDocument();
	
	fname = write()->fileName();
 	
  QFileInfo fileInfo(fname);
  title=fileInfo.fileName();	
}

bool QuantaDoc::isModified()
{
	return write()->isModified();
}

bool QuantaDoc::isModifiedAll()
{
  bool modified = false;

  QDictIterator<Document> it( docList ); // iterator for dict

  while ( Document *twrite = it.current() ) {
    if ( twrite->isModified() ) modified = true;
    ++it;
  }

  return modified;
}


void QuantaDoc::setModified(bool _m)
{
	write()->setModified(_m);
};

bool QuantaDoc::newDocument( const char* name )
{
  QString fileName;

  bool fnew = false;
  if ( !name ) {
    fileName = "Untitled1.html";
    fnew = true;
  }
  else fileName = name;

  if ( fileName.left(5) == "file:" ) fileName.remove(0,5);

  if ( !docList.find( fileName ) || fnew )
  {
  	if ( write() )
 	    if ( !write()->hasFileName() && !write()->isModified() && !fnew )
 	    {
 	    	QString t = write()->fileName();
 	    	docList.remove( t );
 	      app->view->removeWrite();
 	  }

  	Document *w = newWrite( app->view->writeTab );
  	fileName = w->fileName();
  	if ( fileName.left(5) == "file:" ) fileName.remove(0,5);
  	
  	QString shortName = fileName;
  	int pos;
    while ( (pos = (int)shortName.find('/')) != -1 )
       shortName.remove(0,pos+1);
  	
  	app->view->addWrite( w, shortName );
  	
  	docList.insert( fileName, w );
  	
  	setModified( false );
  }
  else {
  	Document *w = docList.find( fileName );
  	app->view->writeTab->showPage( w );
  }

  setTitle( fileName );

  return true;
}

bool QuantaDoc::openDocument(const QString &filename, const char *)
{
  QString name = filename;
  if ( name.left(5) == "file:" ) name.remove(0,5);

  QFileInfo fi( name );
  title	= fi.fileName();

  if ( fi.exists() && fi.isReadable() && fi.isFile() )
  {
    newDocument( filename );

    title = fi.fileName();

    QString oldName = write()->fileName();
    write()->loadURL( filename );
    changeFileName(oldName);
  }
  else return false;

  write()->repaint();

  app->repaintPreview();
	
  setModified( false );
  return true;
}

bool QuantaDoc::saveDocument(const QString &filename, const char *)
{
	QString name = filename;
  if ( name.left(5) == "file:" ) name.remove(0,5);

  QFileInfo fileInfo( name );
  title=fileInfo.fileName();

  QString oldName = write()->fileName();
  write()->writeURL( filename );
  write()->doc()->setURL( filename, false );
  changeFileName( oldName );

  setModified( false );
  return true;
}

void QuantaDoc::deleteContents()
{
  /////////////////////////////////////////////////
  // TODO: Add implementation to delete the document contents
  /////////////////////////////////////////////////

}

/** returns the kwrite document */
Document* QuantaDoc::write()
{
	return app->view->write();
}

/** returns the kwrite document */
QString QuantaDoc::basePath()
{
	if ( write()->hasFileName() )
	{
		QString name = write()->fileName();
		if ( name.left(5) == "file:" ) name.remove(0,5);
		QFileInfo fileInfo( name );
		return fileInfo.dirPath()+"/";
	}
		
	return QDir::currentDirPath()+"/";
}

/** create new write classa */
Document* QuantaDoc::newWrite(QWidget *parent)
{
  Document *write;
  HlManager *hl = new HlManager();
  KWriteDoc *writeDoc = new KWriteDoc( hl);

  // find first free Untitledxx.html
  int i = 1;
  QString fname;
  while ( docList.find( fname = fname.sprintf("Untitled%i.html",i) ) )
    i++;

 	write = new Document( writeDoc, parent, "write" , fname);
 	write -> readConfig( app->config );
 	write -> setHl( hl->nameFind( "HTML"));

 	connect( write, SIGNAL(newStatus()), 		app, SLOT(slotNewStatus()));
 	connect( write, SIGNAL(newUndo()),   		app, SLOT(slotNewUndo()));
 	connect( write, SIGNAL(newMarkStatus()),app, SLOT(slotNewMarkStatus()));
 	
 	return write;
}

/** return bool need repaint preview or not */
bool QuantaDoc::needRepaintPreview()
{
	return write()->isModified();
}

/** for exchange opened files for project */
void QuantaDoc::slotRequestOpenedFiles()
{
	QStringList list;
	QDictIterator<Document> it( docList ); // iterator for dict

  while ( Document *twrite = it.current() )
  {
  	list.append( it.currentKey() );
  	++it;
  }

  emit openedFiles( list );
}
