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
#include <qlayout.h>
#include <qdragobject.h>
#include <qobject.h>


// include files for KDE
#include <klocale.h>
#include <kaction.h>
#include <kconfig.h>
#include <kpopupmenu.h>
#include <kmessagebox.h>

#include <ktexteditor/configinterface.h>
#include <ktexteditor/highlightinginterface.h>
#include <ktexteditor/popupmenuinterface.h>
#include <ktexteditor/markinterface.h>
#include <ktexteditor/undointerface.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/clipboardinterface.h>
#include <ktexteditor/selectioninterface.h>

#include <kparts/componentfactory.h>

#include <klibloader.h>
#include <ktrader.h>


// application specific includes
#include "document.h"
#include "quanta.h"
#include "quantadoc.h"
#include "quantaview.h"

#include "qextfileinfo.h"

#include "resource.h"

//#include "kwrite/kwdoc.h"
//#include "kwrite/highlight/highlight.h"

#include "project/project.h"

extern QDict <QStrList> *tagsDict;

QuantaDoc::QuantaDoc( QuantaApp *app, QWidget *parent, const char *name) : QObject(parent, name)
{
	this->app = app;	
	
  docList = new QDict<Document>(1);

#warning fix Alt->Down menu
//  attribMenu = new KPopupMenu("Tag :");
//  connect( attribMenu, SIGNAL(activated(int)), this, SLOT(slotInsertAttrib(int)));

//  attribCoreMenu = new QPopupMenu();
//  char *attr;
//  for ( attr = lCore->first(); lCore->current(); attr = lCore->next() )
//    attribCoreMenu->insertItem( attr, lCore->at() );

//  connect( attribCoreMenu, SIGNAL(activated(int)), this, SLOT(slotInsertCoreAttrib(int)));

  
//  attribEventsMenu = new QPopupMenu();
//  for ( attr = lScript->first(); lScript->current(); attr = lScript->next() )
//    attribEventsMenu->insertItem( attr, lScript->at() );

//  connect( attribEventsMenu, SIGNAL(activated(int)), this, SLOT(slotInsertEventsAttrib(int)));
}

QuantaDoc::~QuantaDoc()
{
}

KURL QuantaDoc::url()
{
  KURL furl = write()->url();
  
  return furl;
}

QString QuantaDoc::basePath()
{
/*	 if (write()->isUntitled())
	 {
	   if ( app->project->hasProject())
	   {
	     return  app->project->basePath;
	   } else
	   {
	   	return QExtFileInfo::home();
	   }
	 }  else
	 {
	 	return QExtFileInfo::path( furl );
	 }*/
  if  ( app->project->hasProject())
  {
     return  app->project->basePath;
  } else
  {
  	if  ( (write() == 0L) ||  (write()->isUntitled()) )
  	{
    	return QExtFileInfo::home();
  	} else
  	{
		KURL furl = url();
  		return  QExtFileInfo::path( furl );
  	}
  }

//	return (write()->isUntitled()) ? ( app->project->hasProject()?app->project->basePath:QExtFileInfo::home() ) : QExtFileInfo::path( furl );
}

QStringList QuantaDoc::openedFiles(bool noUntitled)
{
  QStringList list;
  QDictIterator<Document> it( *docList );
  
  while ( Document *w = it.current() )
  {
    if ( !w->isUntitled() || !noUntitled )
      list.append( it.currentKey() );
    ++it;
  }
  
  return list;
}

bool QuantaDoc::newDocument( const KURL& url )
{
  bool newfile = false;
  QString furl = url.url();
  if ( furl.isEmpty() ) newfile = true;
  
  if ( !docList->find( furl ) || newfile ) // open new
  {
    if ( write() ) // check if first kwrite exists
    {
      // no modi and new -> we can remove                           !!!!
      if ( !write()->isModified() &&
            write()->isUntitled() && !write()->busy) return true;
    }
    // now we can create new kwrite
    Document *w = newWrite( app->view->writeTab);

    if ( newfile ) furl = w->url().url();
    
  	app ->view  ->addWrite( w, QExtFileInfo::shortName(w->url().url()) );
  	
  	docList->insert( w->url().url(), w );
  }
  else // select opened
  {
  	 Document *w = docList->find( furl );	
    app ->view->writeTab->showPage( w );
  	return false; // don't need loadURL
  }

  return true;
}

void QuantaDoc::openDocument(const KURL& url)
{
  if ( !newDocument( url )) return;
  if ( !url.url().isEmpty()) {
//    write()->busy = true;

    if (write()->doc()->openURL( url ))
    {
      Document *w = write();

      QDictIterator<Document> it(*docList);

    	QString defUrl;
    	while ( it.current() )
    	{
 	      if ( w == it.current() ) defUrl = it.currentKey();
    	  ++it;
    	}
 	
     app ->view->writeTab->showPage( w );
  	
     changeFileTabName(defUrl);

     app->fileRecent->addURL( w->url() );

     emit newStatus();
     app->repaintPreview();

     app->reparse();
    }

  }

  write()->createTempFile();

  emit title( url.url() );
}

/*!!!!
void QuantaDoc::finishLoadURL(KWrite *_w)
{
  Document *w = (Document *)_w;

  QDictIterator<Document> it(*docList);

 	QString defUrl;
 	while ( it.current() )
 	{
 	  if ( w == it.current() ) defUrl = it.currentKey();
 	  ++it;
 	}
 	
    app ->view->writeTab->showPage( w );
 	
 changeFileTabName(defUrl);
  
  app->fileRecent->addURL( w->url() );
  
  emit newStatus();
  app->repaintPreview();
  
  w -> busy = false;
  app->reparse();
}     */

void QuantaDoc::saveDocument(const KURL& url)
{
	QString defUrl = this->url().url();

  if ( !url.url().isEmpty()) 
  {
    write()->doc()->saveAs( url );
    write()->closeTempFile();
    write()->createTempFile();
//    write()->setURL( url, false );
  }
  
  // fix
  if ( defUrl != url.url() ) changeFileTabName( defUrl );
  
  emit title( this->url().url() );

  return;
}
  /*
void QuantaDoc::finishSaveURL(KWrite *_w)
{
  Document *w = (Document *)_w;
  w -> busy = false;
}   */

bool QuantaDoc::saveAll(bool dont_ask)
{
  bool flagsave = true;

//FIXME: I don't like the switching through the pages... We must optimize this. (Andras)
 Document *currentDoc = static_cast<Document*>(app ->view->writeTab->currentPage());

  QDictIterator<Document> it( *docList );

  while ( Document *w = it.current() ) 
  {
    if ( w->isModified() )
    {
    app ->view->writeTab->showPage( w );

      QString oldUrl = w->url().url();

      if ( dont_ask ) 
      {
      	w->doc()->save();
        w->closeTempFile();
        w->createTempFile();
      	if ( w->isModified() ) flagsave = false;
      }
      else
      	if ( !saveModified() ) flagsave = false;
      changeFileTabName( oldUrl );
    }
    ++it;
  }

    app ->view->writeTab->showPage( currentDoc );
  return flagsave;
}

void QuantaDoc::closeDocument()
{
	if ( !saveModified() ) return;
  write()->closeTempFile();
	docList->remove( url().url() );
	if ( !app->view->removeWrite()) openDocument( KURL() );
}

void QuantaDoc::closeAll()
{
  do
  {
    if (app->view->write() !=0)
    {
  	  if ( !saveModified() ) return;
      app->view->write()->closeTempFile();
		  docList->remove( url().url() );
    }
	}
	while ( app->view->removeWrite());
	
  // so we remove all kwrites
	openDocument( KURL() );
}

void QuantaDoc::readConfig( KConfig *config )
{
  config -> sync();

  QDictIterator<Document> it( *docList );

  while ( Document *w = it.current() ) 
  {
    config->setGroup("General Options");
    
    w -> readConfig( config );
    ++it;
  }
}

void QuantaDoc::writeConfig( KConfig *config )
{
  config->setGroup("General Options");
  
  write()-> writeConfig( config );
  config -> sync();

  // read the config
  // with all kwrite
  readConfig( config );
}

bool QuantaDoc::saveModified()
{
  bool completed=true;

  if( isModified() )
  {
    int want_save 
      = KMessageBox::warningYesNoCancel(app,
          i18n("The current file has been modified.\nDo you want to save it?"),
          i18n("Warning"));
    
    switch(want_save)
    {
      case KMessageBox::Yes :
           if ( write()->isUntitled() )
           {
             app->slotFileSaveAs();
           }
           else
           {
             saveDocument( url());
       	   };

           completed=true;
           break;

      case KMessageBox::No :
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

bool QuantaDoc::isModified()
{
  if (write() != 0)
  {
    return write()->isModified();
  } else
  {
    return false;
  }
}

bool QuantaDoc::isModifiedAll()
{
  bool modified = false;

  QDictIterator<Document> it( *docList );

  while ( Document *twrite = it.current() ) {
    if ( twrite->isModified() ) modified = true;
    ++it;
  }

  return modified;
}

void QuantaDoc::setModified(bool flag)
{
	write()->setModified(flag);
}

Document* QuantaDoc::write()
{
	return app->view->write();
}


Document* QuantaDoc::newWrite(QWidget *_parent)
{
  int i = 1;
  QString fname;
  while ( docList->find( fname.sprintf("Untitled%i.html",i) ) ) i++;
  
//  KTextEditor::Document *doc = KTextEditor::createDocument("katepart");

  KTextEditor::Document *doc = KParts::ComponentFactory::createPartInstanceFromQuery<KTextEditor::Document>( "KTextEditor/Document",
													      QString::null,
													      _parent, 0,
													      this, 0 );

  Document  *w    = new Document (basePath(), doc, _parent);
  KTextEditor::View * v = w->view();

  //[MB02] connect all kate views for drag and drop
  connect((QObject *)w->view(), SIGNAL(dropEventPass(QDropEvent *)), (QObject *)app->tTab, SLOT(slotDragInsert(QDropEvent *)));
  // TODO : find out what's wrong with this connect

 	app-> config->setGroup("General Options");
  w->readConfig( app->config );
 	w->setUntitledUrl( fname );
  w->kate_view->installPopup((QPopupMenu *)app->factory()->container("popup_editor", app));

  for (unsigned int i=0; i< w->kate_doc->hlModeCount(); i++)
  {
    if (w->kate_doc->hlModeName(i).contains("HTML"))
    {
      w->kate_doc->setHlMode(i);
    }
  }

//FIXME:set to HTML
//  dynamic_cast<KTextEditor::PopupMenuInterface *>(v)->installPopup((QPopupMenu *)app->factory()->container("popup_editor", app));
//  dynamic_cast<KTextEditor::PopupMenuInterface *>(v)->installPopup((QPopupMenu *)app->factory()->container("rb_popup", app));

 	w->parentWidget()->setFocusProxy(w);

 	connect( v, SIGNAL(newStatus()),app, SLOT(slotNewStatus()));

/* !!!!
 	connect( dynamic_cast<KTextEditor::UndoInterface *>(v), SIGNAL(undoChanged()),
           app, SLOT(slotNewUndo()) );
*/

// 	connect( w, SIGNAL(statusMsg(const QString &)),app, SLOT(slotStatusMsg(const QString &)));
 	
//FIXME: should this remain?
// 	connect( w, SIGNAL(finishLoadURL(KWrite *)), this, SLOT(finishLoadURL(KWrite *)));
// 	connect( w, SIGNAL(finishSaveURL(KWrite *)), this, SLOT(finishSaveURL(KWrite *)));
 	
// 	w->clearFocus();
// 	w->setFocus();
 	
 	return w;
}

/** show popup menu with list of attributes for current tag */
void QuantaDoc::slotAttribPopup()
{
/*  
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
        attribMenu->insertItem( item , list->at() );
        if ( attrList.find(item) != -1 )
          attribMenu->setItemEnabled( list->at(), false );
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

    attribMenu->setActiveItem( 0);

    QPoint globalPos = write()->getGlobalCursorPos();
    attribMenu->exec( globalPos );
  } 
  else {
    QString message = i18n("Unknown tag : ");
    message += tag;
    app->slotStatusMsg( message.data() );
  }
  */
}

void QuantaDoc::slotInsertAttrib( int id )
{
  write()->currentTag();
  QString tag = write()->getTagAttr(0);
  
  if ( tagsList->find( tag.upper()) != -1 ) 
  {

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
  Document *d =dynamic_cast<Document*>(tab->currentPage());

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
  Document *d =dynamic_cast<Document*>(tab->currentPage());

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

void QuantaDoc::changeFileTabName( QString oldUrl, QString newUrl )
{
	if ( newUrl.isNull() ) newUrl = url().url();
         
	if ( app->view->writeTab->tabLabel(write()) != QExtFileInfo::shortName( newUrl ))
		  app->view->writeTab->changeTab( write(), QExtFileInfo::shortName( newUrl ));
	
  if ( oldUrl != newUrl )
  {
    docList->remove( oldUrl );
    docList->insert( newUrl, write() );
  }
  
  QDictIterator<Document> it1(*docList);
// QDictIterator<Document> it2(*docList);

// 	int i,len;
 	while ( it1.current() )
 	{
 		QString name1 = it1.currentKey();
 /*		
 		len = 0;
 		i = name1.findRev( '/' );
 		
 		if ( i!=-1 ) len = name1.length()-i-1;
 		else		 		 len = name1.length();
 		
 		it2.toFirst();
 		
 		while ( it2.current() && i != -1 && it1.current() != it2.current() )
 		{
 			QString name2 = it2.currentKey();
 			
 			if ( name1 != name2 )
 			{
 				while ( name1.right( len ) == name2.right( len ) && i != -1 && len>=0 )
 				{
 					i = name1.findRev( '/', i-1 );
 					len = name1.length()-i-1;
 				}
 			}
 			++it2;
 		}
 		QString shortUrl = name1.right( len );   */
  	 QString shortUrl = KURL(name1).prettyURL();
  	 if (shortUrl.left(5) == "file:") shortUrl.remove(0,5);
     if (shortUrl.left(1) == "/")
    {
       	shortUrl = QExtFileInfo::toRelative(shortUrl,basePath());
        if (shortUrl.contains("../"))
        {
          shortUrl.replace(QRegExp("\\.\\./"),"");
          shortUrl = ".../" + shortUrl;
        }
    }     	
 		
		if ( app->view->writeTab->tabLabel(it1.current()) != shortUrl)
 		  app->view->writeTab->changeTab( it1.current() , shortUrl );

 		++it1;
 	}
}

/// SLOTS
                               
void QuantaDoc::undoHistory() {/*write()->undoHistory();*/}
void QuantaDoc::invertSelect(){/*write()->invertSelection();*/}

#include "quantadoc.moc"
