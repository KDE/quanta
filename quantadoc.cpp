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
#include <kpopupmenu.h>

// application specific includes
#include "quantadoc.h"
#include "quanta.h"
#include "quantaview.h"

#include "resource.h"

#include "document.h"
#include "kwrite/kwdoc.h"
#include "kwrite/highlight/highlight.h"

QList<QuantaView> *QuantaDoc::pViewList = 0L;

extern QDict <QStrList> *tagsDict;


QuantaDoc::QuantaDoc( QuantaApp *app, QWidget *parent, const char *name) : QObject(parent, name)
{
	this->app = app;	

  if(!pViewList)
  {
    pViewList = new QList<QuantaView>();
  }

  docList = new QDict<Document>(1);

  pViewList->setAutoDelete(true);
  rbMenu = 0L;

  attribMenu = new KPopupMenu("Tag :");
  connect( attribMenu, SIGNAL(activated(int)), this, SLOT(slotInsertAttrib(int)));

  attribCoreMenu = new QPopupMenu();
  char *attr;
  for ( attr = lCore->first(); lCore->current(); attr = lCore->next() )
    attribCoreMenu->insertItem( attr, lCore->at() );

  connect( attribCoreMenu, SIGNAL(activated(int)), this, SLOT(slotInsertCoreAttrib(int)));

  
  attribEventsMenu = new QPopupMenu();
  for ( attr = lScript->first(); lScript->current(); attr = lScript->next() )
    attribEventsMenu->insertItem( attr, lScript->at() );

  connect( attribEventsMenu, SIGNAL(activated(int)), this, SLOT(slotInsertEventsAttrib(int)));
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
  if ( docList->count() )
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

  docList->remove( oldname );
  docList->insert( newname, write() );

  QDictIterator<Document> it1(*docList);
  QDictIterator<Document> it2(*docList);

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

  QDictIterator<Document> it( *docList ); // iterator for dict

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

void QuantaDoc::writeConfig( KConfig *config )
{
  write()->writeConfig( config );

  QDictIterator<Document> it( *docList ); // iterator for dict

  while ( Document *twrite = it.current() ) {
    ++it;

    twrite->readConfig( config );
  }
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
	
	docList->remove( fname );
	
	if ( !( app->view->removeWrite() ) )
    newDocument();

 	fname = write()->fileName();
 	
  QFileInfo fileInfo(fname);
  title=fileInfo.fileName();
}

void QuantaDoc::closeAll()
{
	QDictIterator<Document> it( *docList ); // iterator for dict

	QString fname;
	
  do
  {
  	if ( !saveModified() ) return;
  	
  	fname = write()->fileName();
		if ( fname.left(5) == "file:" ) fname.remove(0,5);
		docList->remove( fname );
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

  QDictIterator<Document> it( *docList ); // iterator for dict

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

  if ( !docList->find( fileName ) || fnew )
  {
  	if ( write() )
 	    if ( !write()->hasFileName() && !write()->isModified() && !fnew )
 	    {
 	    	QString t = write()->fileName();
 	    	docList->remove( t );
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
  	
  	docList->insert( fileName, w );
  	
  	setModified( false );
  }
  else {
  	Document *w = docList->find( fileName );
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

// options of kwrite
void QuantaDoc::editorOptions()
{
  write()->editorOptions();
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
  while ( docList->find( fname = fname.sprintf("Untitled%i.html",i) ) )
    i++;

 	write = new Document( writeDoc, parent, "write" , fname);
 	app->config->setGroup("General Options");
 	write -> readConfig( app->config );
 	write -> setHl( hl->nameFind( "HTML"));
 	write -> installPopup( rbMenu );

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
	QDictIterator<Document> it( *docList ); // iterator for dict

  while ( Document *twrite = it.current() )
  {
  	list.append( it.currentKey() );
  	++it;
  }

  emit openedFiles( list );
}


/** show popup menu with list of attributes for current tag */
void QuantaDoc::slotAttribPopup()
{
  attribMenu->clear();

  write()->currentTag();
  QString tag = write()->getTagAttr(0);
  QStrIList attrList = QStrIList();

  for (int i=1; i < write()->tagAttrNum; i++ )
      attrList.append( write()->getTagAttr(i) );

  if ( tagsList->find( tag.upper()) != -1 )
  {
    QString caption = QString("Attributes of <")+tag+">";
    attribMenu->setTitle( caption );
//    attribMenu->insertItem( caption.data() );
//    attribMenu->insertSeparator();
//    attribMenu->insertSeparator();

    QStrList *list = tagsDict->find( tag );
    char * item = list->first();
    bool haveAttributes = false; // if popup memu haven't members, dont show it
    while ( item ) {
      if ( !((lCore->find(item)!=-1) || (lI18n->find(item)!=-1) || (lScript->find(item)!=-1))) {
        haveAttributes = true;
        attribMenu->insertItem( item , /* KPM_FirstItem+ */list->at() );
        if ( attrList.find(item) != -1 )
          attribMenu->setItemEnabled( /* KPM_FirstItem+ */list->at(), false );
      }
      item = list->next();
    }

    if ( tagsCore->find(tag.upper()) != -1 ) {
      haveAttributes = true;
      attribMenu->insertItem("Core", attribCoreMenu, -2);
    }

    if ( tagsScript->find(tag.upper()) != -1 ) {
      haveAttributes = true;
      attribMenu->insertItem("Script", attribEventsMenu, -2);
    }

    if ( !haveAttributes )
      return;              // don't show empty menu, may be core dumped

    attribMenu->setActiveItem( /* KPM_FirstItem */ 0);

    QPoint globalPos = write()->getGlobalCursorPos();
    attribMenu->exec( globalPos );


  } else {
    QString message = i18n("Unknown tag : ");
    message += tag;
    app->slotStatusMsg( message.data() );
  }
}

void QuantaDoc::slotInsertAttrib( int id )
{
  write()->currentTag();
  QString tag = write()->getTagAttr(0);
  if ( tagsList->find( tag.upper()) != -1 ) {

    QStrList *list = tagsDict->find( tag.data() );

    write()->insertAttrib( list->at(id) );

  }
}

void QuantaDoc::slotInsertCoreAttrib( int id )
{
  write()->insertAttrib( lCore->at(id) );
}

void QuantaDoc::slotInsertEventsAttrib( int id )
{
	write()->insertAttrib( lScript->at(id) );
}

void QuantaDoc::prevDocument()
{
  QTabWidget *tab = app->view->writeTab;

  Document *d = (Document*)tab->currentPage();
  Document *new_d;

  QDictIterator<Document> it(*docList);

  new_d = it.toFirst();

 	while ( it.current() != d )
 		++it;
 		
 	++it;
 	
 	if ( it.current() ) {
 		new_d = it.current();
 	}
 	
 	tab->showPage( new_d );
}

void QuantaDoc::nextDocument()
{

  QTabWidget *tab = app->view->writeTab;

  Document *d = (Document*)tab->currentPage();
  Document *new_d = 0 , *prev = 0;

  QDictIterator<Document> it(*docList);

  while ( it.current() ) {
  	  new_d = it.current();
 		++it;
 	}
 	
 	
  it.toFirst();

 	while ( it.current() != d ) {
 	  prev = it.current();
 		++it;
 	}
 	
 	if ( prev )
 		new_d = prev;
 	
 	tab->showPage( new_d );
}
