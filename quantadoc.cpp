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
#include <kdirwatch.h>

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

#include "project/project.h"

QuantaDoc::QuantaDoc( QuantaApp *app, QWidget *parent, const char *name) : QObject(parent, name)
{
	this->app = app;

  m_docList = new QDict<Document>(1);

  fileWatcher = new KDirWatch(this);
  connect(fileWatcher, SIGNAL(dirty(const QString&)),SLOT(slotFileDirty(const QString&)));

  attribMenu = new KPopupMenu(i18n("Tag :"));
  connect( attribMenu, SIGNAL(activated(int)), this, SLOT(slotInsertAttrib(int)));
}

QuantaDoc::~QuantaDoc()
{
}

KURL QuantaDoc::url()
{
  KURL furl = "Untitled1.html";

  if (app->view->writeExists()) furl = write()->url();

  return furl;
}

QString QuantaDoc::basePath()
{
  if  ( app->project->hasProject())
  {
     return  app->project->basePath;
  } else
  {
  	if  ( !app->view->writeExists() || write()->isUntitled() )
  	{
    	return QExtFileInfo::home();
  	} else
  	{
	   	KURL furl = url();
  		return  QExtFileInfo::path( furl );
  	}
  }
}

QStringList QuantaDoc::openedFiles(bool noUntitled)
{
  QStringList list;
  QDictIterator<Document> it( *m_docList );
  
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
  Document *w;
  
  if ( !m_docList->find( furl ) || newfile ) // open new
  {
    // no modi and new -> we can remove                           !!!!
    if (app->view->writeExists())
    {
      w = write();
      if ( !w->isModified() &&
            w->isUntitled() && !w->busy) return true;
    }

    // now we can create new kwrite
    w = newWrite( app->view->writeTab);
    w->readConfig(app->config);

    if ( newfile ) furl = w->url().url();

  	app ->view->addWrite( w, w->url().url() );

    app->processDTD(defaultDocType);
  	
  	m_docList->insert( w->url().url(), w );
  }
  else // select opened
  {
    w = m_docList->find( furl );	
    app ->view->writeTab->showPage( w );
  	return false; // don't need loadURL
  }

  return true;
}

void QuantaDoc::openDocument(const KURL& url, QString encoding)
{
//  Document *ww = write();
  if ( !newDocument( url )) return;
  if ( !url.url().isEmpty())
  {
//    write()->busy = true;

    //see if we can watch this file
    if (url.isLocalFile() && QFileInfo(url.path()).exists())
    {
       fileWatcher->addFile(url.path());
    }

    Document *w = write();
    if (encoding.isEmpty()) encoding = defaultEncoding;
    w->kate_doc->setEncoding(encoding);
    if (w->doc()->openURL( url ))
    {
      w->setDirtyStatus(false);
      QDictIterator<Document> it(*m_docList);

   	  QString defUrl;
   	  while ( it.current() )
   	  {
        if ( w == it.current() ) defUrl = it.currentKey();
  	    ++it;
   	  }
      w->readConfig(app->config);

      app ->view->writeTab->showPage( w );
      changeFileTabName(defUrl);
      app->fileRecent->addURL( w->url() );

      emit newStatus();
      app->repaintPreview();

      app->processDTD();
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

  QDictIterator<Document> it(*m_docList);

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
  Document *w = write();

  if ( !url.url().isEmpty())
  {
    fileWatcher->removeFile(w->url().path());
    if (!w->doc()->saveAs( url ))
    {
      KMessageBox::error(app, i18n("Saving of the document failed.\nMaybe you should try to save in another directory."));
    }
    w->closeTempFile();
    w->createTempFile();
    w->setDirtyStatus(false);
    if (w->url().isLocalFile()) fileWatcher->addFile(w->url().path());
  }

  // fix
  if ( defUrl != url.url() ) changeFileTabName( defUrl );
  
  emit title( this->url().url() );

  return;
}

bool QuantaDoc::saveAll(bool dont_ask)
{
  bool flagsave = true;

//FIXME: I don't like the switching through the pages... We must optimize this. (Andras)
 Document *currentDoc = static_cast<Document*>(app ->view->writeTab->currentPage());

  QDictIterator<Document> it( *m_docList );

  while ( Document *w = it.current() ) 
  {
    if ( w->isModified() )
    {
    app ->view->writeTab->showPage( w );

      QString oldUrl = w->url().url();

      if ( dont_ask ) 
      {
      	w->save();
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
  fileWatcher->removeFile(write()->url().path());
	m_docList->remove( url().url() );
	if ( !app->view->removeWrite()) openDocument( KURL() );
}

void QuantaDoc::closeAll()
{
  do
  {
    if (app->view->writeExists())
    {
	    if ( !saveModified() ) return;
      write()->closeTempFile();
      fileWatcher->removeFile(write()->url().path());
		  m_docList->remove( url().url() );
    }
	}
	while ( app->view->removeWrite());
	
  // so we remove all kwrites
	openDocument( KURL() );
}

void QuantaDoc::readConfig( KConfig *config )
{
  config -> sync();

  QDictIterator<Document> it( *m_docList );

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
  if (app->view->writeExists())
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

  QDictIterator<Document> it( *m_docList );

  while ( Document *w = it.current() ) {
    if ( w->isModified() ) modified = true;
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
  while ( m_docList->find( fname.sprintf("Untitled%i.html",i) ) ) i++;
  
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

  app->config->setGroup("General Options");
  int tabWidth = app->config->readNumEntry("TabWidth",4);
  app->config->writeEntry("TabWidth",tabWidth);
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
 	
// 	w->clearFocus();
// 	w->setFocus();
 	
 	return w;
}

/** show popup menu with list of attributes for current tag */
void QuantaDoc::slotAttribPopup()
{
  Document *w = write();

  attribMenu->clear();
  DTDStruct *dtd = w->currentDTD();
  Tag *tag = w->tagAt(dtd);
  if (tag)
  {
    QString tagName = tag->name;
    QStrIList attrList = QStrIList();
    QString name;

    for (int i=0; i < tag->attrCount; i++ )
        attrList.append( tag->attribute(i) );

    if ( QuantaCommon::isKnownTag(w->getDTDIdentifier(),tagName) )
    {
      QString caption = QString(i18n("Attributes of <"))+tagName+">";
      attribMenu->setTitle( caption );

      AttributeList *list = QuantaCommon::tagAttributes(w->getDTDIdentifier(),tagName );
      uint menuId = 0;
      for ( uint i = 0; i < list->count(); i++ )
      {
        name = list->at(i)->name;
        attribMenu->insertItem( name , i);//list->findIndex(*item) );
        if (attrList.contains(name))
        {
          attribMenu->setItemEnabled( i , false );
        }
        menuId++;
      }

      QTag* qtag = QuantaCommon::tagFromDTD(w->getDTDIdentifier(), tagName);
      for (QStringList::Iterator it = qtag->commonGroups.begin(); it != qtag->commonGroups.end(); ++it)
      {
       QPopupMenu* popUpMenu = new QPopupMenu(attribMenu, (*it).latin1());
       AttributeList *attrs = qtag->parentDTD->commonAttrs->find(*it);
       for (uint j = 0; j < attrs->count(); j++)
       {
        name = attrs->at(j)->name;
        popUpMenu->insertItem(name, ++menuId);
        if (attrList.contains(name))
        {
          popUpMenu->setItemEnabled( menuId , false );
        }
       }
       connect( popUpMenu, SIGNAL(activated(int)), this, SLOT(slotInsertAttrib(int)));
       attribMenu->insertItem(*it, popUpMenu);
      }

      if (menuId > 0)    // don't show empty menu, may be core dumped
      {
        attribMenu->setActiveItem( 0);

        QPoint globalPos = w->getGlobalCursorPos();
        attribMenu->exec( globalPos );
      }
    }
    else {
      QString message = i18n("Unknown tag: ");
      message += tagName;
      app->slotStatusMsg( message.data() );
    }
    delete tag;
  }
}

void QuantaDoc::slotInsertAttrib( int id )
{
  Document *w = write();
  DTDStruct *dtd = w->currentDTD();
  Tag *tag = w->tagAt(dtd);
  if (tag)
  {
    QString tagName = tag->name;
    if ( QuantaCommon::isKnownTag(w->getDTDIdentifier(),tagName) )
    {
      int menuId;
      AttributeList *list = QuantaCommon::tagAttributes(w->getDTDIdentifier(),tagName.data() );
      menuId = list->count();
      QString attrStr;
      if (id <= menuId)
      {
        attrStr = list->at(id)->name;
      } else
      {
        QTag* qtag = QuantaCommon::tagFromDTD(w->getDTDIdentifier(), tagName);
        for (QStringList::Iterator it = qtag->commonGroups.begin(); it != qtag->commonGroups.end(); ++it)
        {
          AttributeList *attrs = qtag->parentDTD->commonAttrs->find(*it);
          menuId += attrs->count();
          if (id <= menuId)
          {
            attrStr = attrs->at(id - (menuId - attrs->count()) -1)->name;
            break;
          }
        }
      }
      w->insertAttrib( attrStr);
    }

    delete attribMenu;
    attribMenu = new KPopupMenu(i18n("Tag :"));
    connect( attribMenu, SIGNAL(activated(int)), this, SLOT(slotInsertAttrib(int)));
    delete tag;
  }
}

void QuantaDoc::prevDocument()
{
  QTabWidget *tab = app->view->writeTab;
  Document *d =dynamic_cast<Document*>(tab->currentPage());

  Document *new_d;

  QDictIterator<Document> it(*m_docList);

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

  QDictIterator<Document> it(*m_docList);

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
         
	if ( app->view->writeTab->tabToolTip(write()) != newUrl )
	{
    app->view->writeTab->changeTab( write(), QExtFileInfo::shortName( newUrl));
    app->view->writeTab->setTabToolTip( write(), newUrl );
  }
	
  if ( oldUrl != newUrl )
  {
    m_docList->remove( oldUrl );
    m_docList->insert( newUrl, write() );
  }
  
  QDictIterator<Document> it1(*m_docList);
// QDictIterator<Document> it2(*m_docList);

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
 		
		if ( app->view->writeTab->tabToolTip(it1.current()) != shortUrl)
    {
 		  app->view->writeTab->changeTab( it1.current() , shortUrl.section("/",-1) );
 		  app->view->writeTab->setTabToolTip( it1.current() , shortUrl );
    }
 		++it1;
 	}
}

/// SLOTS
                               
void QuantaDoc::undoHistory() {/*write()->undoHistory();*/}
void QuantaDoc::invertSelect(){/*write()->invertSelection();*/}


/** Called when a file on the disk has changed. */
void QuantaDoc::slotFileDirty(const QString& fileName)
{
  QDictIterator<Document> it( *m_docList );

  while ( Document *w = it.current() )
  {
    if ( w->url().path()==fileName ) w->setDirtyStatus(true);
    ++it;
  }

}

#include "quantadoc.moc"
