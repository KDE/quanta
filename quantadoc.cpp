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
#include <kdeversion.h>
//#include <kdebug.h>

#include <ktexteditor/configinterface.h>
#include <ktexteditor/highlightinginterface.h>
#include <ktexteditor/popupmenuinterface.h>
#include <ktexteditor/markinterface.h>
#include <ktexteditor/undointerface.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/clipboardinterface.h>
#include <ktexteditor/selectioninterface.h>

#if KDE_VERSION >= 308
#include <ktexteditor/encodinginterface.h>
#include <ktexteditor/dynwordwrapinterface.h>
#endif

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
  fileWatcher = new KDirWatch(this);
  connect(fileWatcher, SIGNAL(dirty(const QString&)),SLOT(slotFileDirty(const QString&)));

  attribMenu = new KPopupMenu(i18n("Tag :"));
  connect( attribMenu, SIGNAL(activated(int)), this, SLOT(slotInsertAttrib(int)));
}

QuantaDoc::~QuantaDoc()
{
}

KURL::List QuantaDoc::openedFiles(bool noUntitled)
{
  KURL::List list;
  if (quantaApp->getView()) //need to check otherwise it may crash on exit
  {
    QTabWidget *tab = quantaApp->getView()->writeTab;
    for (int i = 0; i < tab->count(); i++) 
    {
      Document *w = dynamic_cast<Document *>(tab->page(i));
      if ( !w->isUntitled() || !noUntitled )
        list.append( w->url() );
    }
  }

  return list;
}

bool QuantaDoc::newDocument( const KURL& url )
{
  bool newfile = false;
  QString furl = url.url();
  if ( furl.isEmpty() ) newfile = true;
  Document *w;

  if (!isOpened(furl) || newfile)
  {
    // no modi and new -> we can remove                           !!!!
    if (quantaApp->view->writeExists())
    {
      w = write();
      if ( !w->isModified() &&
            w->isUntitled() && !w->busy) return true;
    }

    // now we can create new kwrite
    w = newWrite( );
    w->readConfig(quantaApp->config);

    if ( newfile ) furl = w->url().url();

  	quantaApp ->view->addWrite( w, w->url().url() );

    quantaApp->processDTD(quantaApp->getProject()->defaultDTD());

    w->kate_view->setIconBorder(qConfig.iconBar);
    w->kate_view->setLineNumbersOn(qConfig.lineNumbers);
    quantaApp->viewBorder->setChecked(qConfig.iconBar);
    quantaApp->viewLineNumbers->setChecked(qConfig.lineNumbers);

  #if (KDE_VERSION > 308)
    dynamic_cast<KTextEditor::DynWordWrapInterface*>(w->view())->setDynWordWrap(qConfig.dynamicWordWrap);
    quantaApp->viewDynamicWordWrap->setChecked(dynamic_cast<KTextEditor::DynWordWrapInterface*>(w->view())->dynWordWrap());
  #endif
  }
  else // select opened
  {
    w = isOpened(furl);
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
    if (encoding.isEmpty()) encoding = quantaApp->defaultEncoding();

    #if KDE_VERSION >= 308
      dynamic_cast<KTextEditor::EncodingInterface*>(w->doc())->setEncoding(encoding);
    #else
      w->kate_doc->setEncoding(encoding);
    #endif

    w->readConfig(quantaApp->config);
    if (w->doc()->openURL( url ))
    {
      w->setDirtyStatus(false);

      quantaApp ->view->writeTab->showPage( w );
      changeFileTabName();
      quantaApp->fileRecent->addURL( w->url() );

      emit newStatus();
      quantaApp->repaintPreview();

      quantaApp->processDTD();
      quantaApp->reparse();

      loaded = true;

      write()->kate_view->setIconBorder(qConfig.iconBar);
      write()->kate_view->setLineNumbersOn(qConfig.lineNumbers);
      quantaApp->viewBorder->setChecked(qConfig.iconBar);
      quantaApp->viewLineNumbers->setChecked(qConfig.lineNumbers);

  #if (KDE_VERSION > 308)
      dynamic_cast<KTextEditor::DynWordWrapInterface*>(w->view())->setDynWordWrap(qConfig.dynamicWordWrap);
      quantaApp->viewDynamicWordWrap->setChecked(dynamic_cast<KTextEditor::DynWordWrapInterface*>(w->view())->dynWordWrap());
  #endif

      write()->createTempFile();
      emit title( write()->url().prettyURL() );
      write()->view()->setFocus();     
    } 
  }
 if (!loaded && !url.isEmpty()) //the open of the document has failed
 {
   KMessageBox::error(quantaApp, i18n("Cannot open document \"%1\".").arg(url.prettyURL()));
   closeDocument();
 }
 
}

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
    changeFileTabName();
  }

  emit title( w->url().url() );

  return;
}

bool QuantaDoc::saveAll(bool dont_ask)
{
  bool flagsave = true;

  Document *currentDoc = static_cast<Document*>(quantaApp ->view->writeTab->currentPage());

  QTabWidget *docTab =quantaApp->getView()->writeTab;
  Document *w;
  for (int i = docTab->count() -1; i >=0; i--) 
  {
    w = dynamic_cast<Document*>(docTab->page(i));
    if ( w->isModified() )
    {
      if (!w->isUntitled())
          fileWatcher->removeFile(w->url().path());
      docTab->showPage(w);
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
    if (!w->isUntitled())
        fileWatcher->removeFile(w->url().path());
  	if ( !quantaApp->view->removeWrite()) openDocument( KURL() );
  }
}

void QuantaDoc::closeAll()
{
  disconnect( quantaApp->view->writeTab, SIGNAL(currentChanged(QWidget*)), quantaApp, SLOT(slotUpdateStatus(QWidget*)));
  Document *w;
  do
  {
    if (quantaApp->view->writeExists())
    {
	    if (saveModified() )
      {
        w = write();
        w->closeTempFile();
        if (!w->isUntitled())
            fileWatcher->removeFile(w->url().path());
      } else
      {
        connect( quantaApp->view->writeTab, SIGNAL(currentChanged(QWidget*)), quantaApp, SLOT(slotUpdateStatus(QWidget*)));
        return; //save failed, so don't close anything
      }
    }
	}
	while ( quantaApp->view->removeWrite());
  connect( quantaApp->view->writeTab, SIGNAL(currentChanged(QWidget*)), quantaApp, SLOT(slotUpdateStatus(QWidget*)));
  //all documents were removed, so open an empty one
	openDocument( KURL() );
}

void QuantaDoc::readConfig( KConfig *config )
{
  config -> sync();
  QTabWidget *docTab =quantaApp->getView()->writeTab;
  Document *w;
  for (int i = docTab->count() -1; i >=0; i--)
  {
    w = dynamic_cast<Document*>(docTab->page(i));
    config->setGroup("General Options");

    w -> readConfig( config );
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

  QTabWidget *docTab =quantaApp->getView()->writeTab;
  Document *w;
  for (int i = docTab->count() -1; i >=0; i--)
  {
    w = dynamic_cast<Document*>(docTab->page(i));
    if ( w->isModified() ) modified = true;
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


Document* QuantaDoc::newWrite()
{
  DTDStruct *dtd = dtds->find(quantaApp->newFileType());
//  if (!dtd) dtd = dtds->find(qConfig.newFileType);   //fallback, but not really needed
  int i = 1;
  while ( isOpened(KURL("file:/"+QString("Untitled%1.").arg(i)+dtd->defaultExtension)))
  {
   i++;
  }
  QString fname = QString("Untitled%1.").arg(i)+dtd->defaultExtension;
  
  KTextEditor::Document *doc = KParts::ComponentFactory::createPartInstanceFromQuery<KTextEditor::Document>( "KTextEditor/Document",
													     QString::null,
													     quantaApp->view->writeTab, 0,
													     quantaApp->view->writeTab, 0 );

  Document *w = new Document(quantaApp->projectBaseURL(), doc, quantaApp->getProject(),
                             quantaApp->m_pluginInterface, quantaApp->view->writeTab);
  QString encoding = quantaApp->defaultEncoding();
#if KDE_VERSION >= 308
  dynamic_cast<KTextEditor::EncodingInterface*>(doc)->setEncoding(encoding);
#else
  w->kate_doc->setEncoding(encoding);
#endif

  KTextEditor::View * v = w->view();

  //[MB02] connect all kate views for drag and drop
  connect((QObject *)w->view(), SIGNAL(dropEventPass(QDropEvent *)), (QObject *)quantaApp->tTab, SLOT(slotDragInsert(QDropEvent *)));

  quantaApp->config->setGroup("Kate Document");
  int tabWidth = quantaApp->config->readNumEntry("TabWidth",4);
  quantaApp->config->writeEntry("TabWidth",tabWidth);
  w->readConfig( quantaApp->config );
 	w->setUntitledUrl( fname);

  for (unsigned int i=0; i< dynamic_cast<KTextEditor::HighlightingInterface*>(w->doc())->hlModeCount(); i++)
  {
    if (quantaApp->newFileType().contains(dynamic_cast<KTextEditor::HighlightingInterface*>(w->doc())->hlModeName(i).lower()))
    {
      dynamic_cast<KTextEditor::HighlightingInterface*>(w->doc())->setHlMode(i);
      break;
    }
  }
  dynamic_cast<KTextEditor::PopupMenuInterface*>(w->view())->installPopup((QPopupMenu *)quantaApp->factory()->container("popup_editor", quantaApp));

 	quantaApp->setFocusProxy(w->view());
  w->view()->setFocusPolicy(QWidget::WheelFocus);
/*  quantaApp->setFocusPolicy(QWidget::StrongFocus);
  */
 	connect( v, SIGNAL(newStatus()),quantaApp, SLOT(slotNewStatus()));

/* !!!!
 	connect( dynamic_cast<KTextEditor::UndoInterface *>(v), SIGNAL(undoChanged()),
           quantaApp, SLOT(slotNewUndo()) );
*/

// 	connect( w, SIGNAL(statusMsg(const QString &)),quantaApp, SLOT(slotStatusMsg(const QString &)));

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
        quantaApp->config->setGroup("Kate Document");
        QFont font;
        font.fromString(quantaApp->config->readEntry("Font"));
        globalPos.setY(globalPos.y() + QFontMetrics(font).height());
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

  int index = tab->currentPageIndex();
  if (index > 0) index--;
  else index = tab->count()-1;
  tab->showPage(tab->page(index));
}

void QuantaDoc::nextDocument()
{
  QTabWidget *tab = quantaApp->view->writeTab;

  int index = tab->currentPageIndex();
  if (index + 1 < tab->count() ) index++;
  else index = 0;
  tab->showPage(tab->page(index));
}

void QuantaDoc::changeFileTabName(KURL newURL)
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
}

/// SLOTS

void QuantaDoc::undoHistory() {/*write()->undoHistory();*/}
void QuantaDoc::invertSelect(){/*write()->invertSelection();*/}


/** Called when a file on the disk has changed. */
void QuantaDoc::slotFileDirty(const QString& fileName)
{
  Document *w;
  
  QTabWidget *tab = quantaApp->view->writeTab;
  for( int i = 0; i < tab->count(); i++)
  {
    w = dynamic_cast<Document*>(tab->page(i));
    if ( w->url().path()==fileName )
    {
      w->setDirtyStatus(true);
      if (w == write())
      {
        w->checkDirtyStatus();
      }
    }
  }

}

/** Check if url is opened or not. */
Document* QuantaDoc::isOpened(const KURL& url)
{
  Document *w = 0L;
  QTabWidget *tab = quantaApp->getView()->writeTab;
  for (int i = 0; i < tab->count(); i++)
  {
    if (dynamic_cast<Document*>(tab->page(i))->url() == url)
    {
      w = dynamic_cast<Document*>(tab->page(i));
      break;
    } 
  }
  return w;
}

#include "quantadoc.moc"
