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
//#include <kdebug.h>

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
#include "plugins/quantaplugininterface.h"

QuantaDoc::QuantaDoc(QWidget *parent, const char *name) : QObject(parent, name)
{

  m_docList = new QDict<Document>(1);

  fileWatcher = new KDirWatch(this);
  connect(fileWatcher, SIGNAL(dirty(const QString&)),SLOT(slotFileDirty(const QString&)));

  attribMenu = new KPopupMenu(i18n("Tag :"));
  connect( attribMenu, SIGNAL(activated(int)), this, SLOT(slotInsertAttrib(int)));
}

QuantaDoc::~QuantaDoc()
{
}

/*
KURL QuantaDoc::url()
{
  KURL furl;
  furl.setPath("Untitled1.html");

  if (quantaApp->view->writeExists()) furl = write()->url();

  return furl;
}

KURL QuantaDoc::baseURL()
{
  KURL result;
  if  ( quantaApp->project->hasProject())
  {
     result = quantaApp->project->baseURL;
  } else
  {
  	if  ( !quantaApp->view->writeExists() || write()->isUntitled() )
  	{
    	result = QExtFileInfo::home();
  	} else
  	{
  		result =  QExtFileInfo::path( url() );
  	}
  }
  return result;
}                */

KURL::List QuantaDoc::openedFiles(bool noUntitled)
{
  KURL::List list;
  QDictIterator<Document> it( *m_docList );

  while ( Document *w = it.current() )
  {
    if ( !w->isUntitled() || !noUntitled )
      list.append( w->url() );
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
    if (quantaApp->view->writeExists())
    {
      w = write();
      if ( !w->isModified() &&
            w->isUntitled() && !w->busy) return true;
    }

    // now we can create new kwrite
    w = newWrite( quantaApp->view->writeTab);
    w->readConfig(quantaApp->config);

    if ( newfile ) furl = w->url().url();

  	quantaApp ->view->addWrite( w, w->url().url() );

    quantaApp->processDTD(quantaApp->getProject()->defaultDTD());

  	m_docList->insert( w->url().url(), w );
  }
  else // select opened
  {
    w = m_docList->find( furl );
    w->checkDirtyStatus();
    quantaApp ->view->writeTab->showPage( w );
  	return false; // don't need loadURL
  }

  return true;
}

void QuantaDoc::openDocument(const KURL& url, QString encoding)
{
//  Document *ww = write();
  //kdDebug() << k_funcinfo << "Encoding is: " << encoding << endl;
  if ( !newDocument( url )) return;
  bool loaded = false;
  if ( !url.isEmpty() && QExtFileInfo::exists(url))
  {
//    write()->busy = true;

    //see if we can watch this file
    if (url.isLocalFile() && QFileInfo(url.path()).exists())
    {
       fileWatcher->addFile(url.path());
    }

    Document *w = write();
    if (encoding.isEmpty()) encoding = qConfig.defaultEncoding;
    w->kate_doc->setEncoding(encoding);
    w->readConfig(quantaApp->config);
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

      quantaApp ->view->writeTab->showPage( w );
      changeFileTabName(defUrl);
      quantaApp->fileRecent->addURL( w->url() );

      emit newStatus();
      quantaApp->repaintPreview();

      quantaApp->processDTD();
      quantaApp->reparse();

      loaded = true;
      write()->createTempFile();
      emit title( write()->url().prettyURL() );
    } 
  }
 if (!loaded && !url.isEmpty()) //the open of the document has failed
 {
   KMessageBox::error(quantaApp, i18n("Cannot open document \"%1\".").arg(url.prettyURL()));
   closeDocument();
 }
 
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

    quantaApp ->view->writeTab->showPage( w );

 changeFileTabName(defUrl);

  quantaApp->fileRecent->addURL( w->url() );

  emit newStatus();
  quantaApp->repaintPreview();

  w -> busy = false;
  quantaApp->reparse();
}     */

void QuantaDoc::saveDocument(const KURL& url)
{
  Document *w = write();
  KURL oldURL = w->url();

  if ( !url.isEmpty())
  {
    fileWatcher->removeFile(oldURL.path());
    if (!w->doc()->saveAs( url ))
    {
      KMessageBox::error(quantaApp, i18n("Saving of the document\n%1\nfailed.\nMaybe you should try to save in another directory.").arg(url.prettyURL()));
    }
    w->closeTempFile();
    w->createTempFile();
    w->setDirtyStatus(false);
    if (w->url().isLocalFile())
    {
      fileWatcher->addFile(w->url().path());
    }
  }

  // fix
  if ( oldURL != url )
  {
    changeFileTabName( oldURL );
    m_docList->remove(oldURL.url());
    m_docList->insert(url.url(), w);
  }

  emit title( w->url().url() );

  return;
}

bool QuantaDoc::saveAll(bool dont_ask)
{
  bool flagsave = true;

//FIXME: I don't like the switching through the pages... We must optimize this. (Andras)
  Document *currentDoc = static_cast<Document*>(quantaApp ->view->writeTab->currentPage());

  QDictIterator<Document> it( *m_docList );

  QTabWidget *docTab =quantaApp->getView()->writeTab;
  Document *w;
  for (int i = docTab->count() -1; i >=0; i--) 
  {
    docTab->setCurrentPage(i);
    w = dynamic_cast<Document*>(docTab->currentPage());
    if ( w->isModified() )
    {
      if (!w->url().path().isEmpty()) fileWatcher->removeFile(w->url().path());
      quantaApp->view->writeTab->showPage( w );
      if ( dont_ask && !w->isUntitled())
      {
      	w->save();
        w->closeTempFile();
        w->createTempFile();
      	if ( w->isModified() ) flagsave = false;
      }
      else
      	if ( !saveModified() ) flagsave = false;
       
      if (w->url().isLocalFile()) fileWatcher->addFile(w->url().path());
    }
  }

  quantaApp->view->writeTab->showPage( currentDoc );
  return flagsave;
}

void QuantaDoc::closeDocument()
{
	if (saveModified())
  {
    Document *w = write();
    w->closeTempFile();
    if (!w->url().path().isEmpty()) fileWatcher->removeFile(w->url().path());
  	m_docList->remove( w->url().url() );
  	if ( !quantaApp->view->removeWrite()) openDocument( KURL() );
  }
}

void QuantaDoc::closeAll()
{
  Document *w;
  do
  {
    if (quantaApp->view->writeExists())
    {
	    if (saveModified() )
      {
        w = write();
        w->closeTempFile();
        if (!w->url().path().isEmpty()) fileWatcher->removeFile(w->url().path());
  		  m_docList->remove( w->url().url() );
      } else
      {
        return; //save failed, so don't close anything
      }
    }
	}
	while ( quantaApp->view->removeWrite());

  //all documents were removed, so open an empty one
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
  QString fileName = quantaApp->getView()->writeTab->label(quantaApp->getView()->writeTab->currentPageIndex());

  if( isModified() )
  {
    int want_save
      = KMessageBox::warningYesNoCancel(quantaApp,
          i18n("The file \"%1\" has been modified.\nDo you want to save it?").arg(fileName),
          i18n("Warning"));

    switch(want_save)
    {
      case KMessageBox::Yes :
           if ( write()->isUntitled() )
           {
             quantaApp->slotFileSaveAs();
           }
           else
           {
             saveDocument( write()->url());
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
  if (quantaApp->view->writeExists())
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
	return quantaApp->view->write();
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

  Document  *w    = new Document (quantaApp->projectBaseURL(), doc, quantaApp->getProject(), _parent, 0, 0, quantaApp->m_pluginInterface);
  KTextEditor::View * v = w->view();

  //[MB02] connect all kate views for drag and drop
  connect((QObject *)w->view(), SIGNAL(dropEventPass(QDropEvent *)), (QObject *)quantaApp->tTab, SLOT(slotDragInsert(QDropEvent *)));

  quantaApp->config->setGroup("General Options");
  int tabWidth = quantaApp->config->readNumEntry("TabWidth",4);
  quantaApp->config->writeEntry("TabWidth",tabWidth);
  w->readConfig( quantaApp->config );
 	w->setUntitledUrl( fname );
  w->kate_view->installPopup((QPopupMenu *)quantaApp->factory()->container("popup_editor", quantaApp));

  for (unsigned int i=0; i< w->kate_doc->hlModeCount(); i++)
  {
    if (w->kate_doc->hlModeName(i).contains("HTML"))
    {
      w->kate_doc->setHlMode(i);
    }
  }

//FIXME:set to HTML
//  dynamic_cast<KTextEditor::PopupMenuInterface *>(v)->installPopup((QPopupMenu *)quantaApp->factory()->container("popup_editor", quantaApp));
//  dynamic_cast<KTextEditor::PopupMenuInterface *>(v)->installPopup((QPopupMenu *)quantaApp->factory()->container("rb_popup", quantaApp));

 	w->parentWidget()->setFocusProxy(w);

 	connect( v, SIGNAL(newStatus()),quantaApp, SLOT(slotNewStatus()));

/* !!!!
 	connect( dynamic_cast<KTextEditor::UndoInterface *>(v), SIGNAL(undoChanged()),
           quantaApp, SLOT(slotNewUndo()) );
*/

// 	connect( w, SIGNAL(statusMsg(const QString &)),quantaApp, SLOT(slotStatusMsg(const QString &)));

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
      QString caption = i18n("Attributes of <%1>").arg(tagName);
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
      QString message = i18n("Unknown tag: %1").arg(tagName);
      quantaApp->slotStatusMsg( message.data() );
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
  QTabWidget *tab = quantaApp->view->writeTab;
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
  QTabWidget *tab = quantaApp->view->writeTab;
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

void QuantaDoc::changeFileTabName(const KURL& oldURL, KURL newURL)
{
  Document *w = write();

	if (newURL.isEmpty())
  {
    newURL = w->url();
  }

	if ( quantaApp->view->writeTab->tabToolTip(w) != newURL.prettyURL() )
	{
    quantaApp->view->writeTab->changeTab( w, QExtFileInfo::shortName(newURL.path()) );
    quantaApp->view->writeTab->setTabToolTip( w, newURL.prettyURL() );
  }

  if ( oldURL != newURL )
  {
    m_docList->remove( oldURL.url() );
    m_docList->insert( newURL.url(), w );
  }


  //TODO Why is this useful??
/*
  QDictIterator<Document> it1(*m_docList);
// QDictIterator<Document> it2(*m_docList);

// 	int i,len;
 	while ( it1.current() )
 	{
 		QString name1 = it1.currentKey();
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
 		QString shortUrl = name1.right( len );
  	 QString shortUrl = KURL(name1).prettyURL();
  	 if (shortUrl.left(5) == "file:") shortUrl.remove(0,5);
     if (shortUrl.left(1) == "/")
    {
       	shortUrl = QExtFileInfo::toRelative(shortUrl,baseURL());
        if (shortUrl.contains("../"))
        {
          shortUrl.replace(QRegExp("\\.\\./"),"");
          shortUrl = ".../" + shortUrl;
        }
    }

		if ( quantaApp->view->writeTab->tabToolTip(it1.current()) != shortUrl)
    {
 		  quantaApp->view->writeTab->changeTab( it1.current() , shortUrl.section("/",-1) );
 		  quantaApp->view->writeTab->setTabToolTip( it1.current() , shortUrl );
    }                                       
 		++it1;
 	}
  */
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
    if ( w->url().path()==fileName )
    {
      w->setDirtyStatus(true);
      if (w == write())
      {
        w->checkDirtyStatus();
      }
    }
    ++it;
  }

}

#include "quantadoc.moc"
