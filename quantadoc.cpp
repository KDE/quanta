/***************************************************************************
                          quantadoc.cpp  -  description
                             -------------------
    begin                : ���� 9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2003 Andras Mantia <amantia@kde.org>
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
#include <kdebug.h>
#include <kiconloader.h>

#include <ktexteditor/configinterface.h>
#include <ktexteditor/highlightinginterface.h>
#include <ktexteditor/popupmenuinterface.h>
#include <ktexteditor/markinterface.h>
#include <ktexteditor/undointerface.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/clipboardinterface.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/encodinginterface.h>
#include <ktexteditor/dynwordwrapinterface.h>
#include <ktexteditor/editorchooser.h>

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

  attribMenu = new KPopupMenu(i18n("Tag"));
  connect( attribMenu, SIGNAL(activated(int)), this, SLOT(slotInsertAttrib(int)));
}

QuantaDoc::~QuantaDoc()
{
}

KURL::List QuantaDoc::openedFiles(bool noUntitled)
{
  KURL::List list;
  if (quantaApp->view()) //need to check otherwise it may crash on exit
  {
    QTabWidget *tab = quantaApp->view()->writeTab();
    for (int i = 0; i < tab->count(); i++)
    {
      Document *w = dynamic_cast<Document *>(tab->page(i));
      if ( w && (!w->isUntitled() || !noUntitled) )
        list.append( w->url() );
    }
  }

  return list;
}

bool QuantaDoc::newDocument( const KURL& url, bool switchToExisting )
{
  bool newfile = false;
  QString furl = url.url();
  if ( furl.isEmpty() ) newfile = true;
  Document *w;

  if (!isOpened(furl) || newfile)
  {
    // no modi and new -> we can remove                           !!!!
    if (quantaApp->view()->writeExists())
    {
      w = write();
      if ( !w->isModified() &&
            w->isUntitled() && !w->busy) return true;
    }

    // now we can create new kwrite
    w = newWrite( );
    quantaApp->view()->addWrite( w, w->url().url() );

    quantaApp->processDTD(quantaApp->project()->defaultDTD());

    w->kate_view->setIconBorder(qConfig.iconBar);
    w->kate_view->setLineNumbersOn(qConfig.lineNumbers);
    quantaApp->viewBorder->setChecked(qConfig.iconBar);
    quantaApp->viewLineNumbers->setChecked(qConfig.lineNumbers);

    dynamic_cast<KTextEditor::DynWordWrapInterface*>(w->view())->setDynWordWrap(qConfig.dynamicWordWrap);
    quantaApp->viewDynamicWordWrap->setChecked(dynamic_cast<KTextEditor::DynWordWrapInterface*>(w->view())->dynWordWrap());
  }
  else // select opened
  if (switchToExisting)
  {
    w = isOpened(furl);
    w->checkDirtyStatus();
    quantaApp->view()->writeTab()->showPage( w );
    return false; // don't need loadURL
  }

  return true;
}

void QuantaDoc::openDocument(const KURL& urlToOpen, const QString &a_encoding, bool switchToExisting)
{
  KURL url = urlToOpen;
  if (url.isLocalFile())
  {
    QString path = QDir(url.path()).canonicalPath();
    if (!path.isEmpty())
        url.setPath(path);
  }
  QString encoding = a_encoding;
  if (!newDocument(url, switchToExisting))
      return;
  Document *w = write();
  bool loaded = false;
  if ( !url.isEmpty() && QExtFileInfo::exists(url))
  {
    //see if we can watch this file
    if (url.isLocalFile() && QFileInfo(url.path()).exists())
    {
       fileWatcher->addFile(url.path());
    }

    if (encoding.isEmpty())
        encoding = quantaApp->defaultEncoding();

    dynamic_cast<KTextEditor::EncodingInterface*>(w->doc())->setEncoding(encoding);

    if (w->doc()->openURL( url ))
    {
      w->setDirtyStatus(false);

      changeFileTabName();
      quantaApp->fileRecent->addURL( w->url() );

      quantaApp->repaintPreview();

      w->kate_view->setIconBorder(qConfig.iconBar);
      w->kate_view->setLineNumbersOn(qConfig.lineNumbers);
      quantaApp->viewBorder->setChecked(qConfig.iconBar);
      quantaApp->viewLineNumbers->setChecked(qConfig.lineNumbers);

      dynamic_cast<KTextEditor::DynWordWrapInterface*>(w->view())->setDynWordWrap(qConfig.dynamicWordWrap);
      quantaApp->viewDynamicWordWrap->setChecked(dynamic_cast<KTextEditor::DynWordWrapInterface*>(w->view())->dynWordWrap());

      w->createTempFile();
      w->view()->setFocus();

      quantaApp->processDTD();
      quantaApp->reparse(true);

      emit title( w->url().prettyURL() );
      emit newStatus();
      loaded = true;
    }
  } else
  { /*
    Project *project = quantaApp->project();
    KTextEditor::HighlightingInterface *highlightinginterface = dynamic_cast<KTextEditor::HighlightingInterface*>(w->doc());
    for (unsigned int i=0; i< highlightinginterface->hlModeCount(); i++)
    {
      kdDebug(24000) << QString("HL mode #%1 : %2").arg(i).arg(highlightinginterface->hlModeName(i)) << endl;
      if (project->defaultDTD().contains(highlightinginterface->hlModeName(i), false))
      {
        highlightinginterface->setHlMode(i);
        break;
      }
    } */
  }
  if (!loaded && !url.isEmpty()) //the open of the document has failed*/
  {
    emit hideSplash();
    KMessageBox::error(quantaApp, i18n("<qt>Cannot open document <b>%1</b>.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)));
    closeDocument();
  }
}

bool QuantaDoc::saveDocument(const KURL& url)
{
  Document *w = write();
  KURL oldURL = w->url();
  bool result = true;
  if ( !url.isEmpty())
  {
    fileWatcher->removeFile(oldURL.path());
    KTextEditor::Document *wdoc = w->doc();
    if (!wdoc->saveAs( url ))
    {
#if KDE_VERSION < KDE_MAKE_VERSION(3,1,90)
      KMessageBox::error(quantaApp, i18n("<qt>Saving of the document <b>%1</b>failed.<br>Maybe you should try to save in another directory.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)));
#endif
      result = false;
    } else
    {
      w->closeTempFile();
      if (dynamic_cast<KTextEditor::HighlightingInterface*>(wdoc)->hlMode()==0)
       {
         uint line,col;
         w->viewCursorIf->cursorPositionReal(&line, &col);
        wdoc->openURL(url);
         w->viewCursorIf->setCursorPosition(line, col);
     }
      w->createTempFile();
      w->setDirtyStatus(false);
    }
    if (w->url().isLocalFile())
    {
      fileWatcher->addFile(w->url().path());
    }
  } else
  {
    result = false;
  }

  // fix
  if ( oldURL != url )
  {
    changeFileTabName();
  }

  emit title( w->url().url() );

  return result;
}

bool QuantaDoc::saveAll(bool dont_ask)
{
  bool flagsave = true;

  Document *currentDoc = static_cast<Document*>(quantaApp->view()->writeTab()->currentPage());

  QTabWidget *docTab =quantaApp->view()->writeTab();
  Document *w;
  for (int i = docTab->count() -1; i >=0; i--)
  {
    w = dynamic_cast<Document*>(docTab->page(i));
    if ( w && w->isModified() )
    {
      if (!w->isUntitled())
          fileWatcher->removeFile(w->url().path());
      docTab->showPage(w);
      if ( dont_ask && !w->isUntitled())
      {
#ifdef BUILD_KAFKAPART
        w->docUndoRedo->fileSaved();
#endif
        w->save();
        w->closeTempFile();
        w->createTempFile();
        if ( w->isModified() ) flagsave = false;
      }
      else
      {
        if ( !saveModified() ) flagsave = false;
#ifdef BUILD_KAFKAPART
        else w->docUndoRedo->fileSaved();
#endif
      }

      if (w->url().isLocalFile()) fileWatcher->addFile(w->url().path());
    }
  }

  quantaApp->view()->writeTab()->showPage( currentDoc );
  return flagsave;
}

void QuantaDoc::closeDocument()
{
  if (saveModified())
  {
    if (quantaApp->view()->writeExists())
    {
      Document *w = write();
      w->closeTempFile();
      if (!w->isUntitled())
        fileWatcher->removeFile(w->url().path());
    }
    if ( !quantaApp->view()->removeWrite())
    {
      openDocument( KURL() );
    }
  }
}

void QuantaDoc::closeAll()
{
  QuantaView *view = quantaApp->view();
  disconnect( view->writeTab(), SIGNAL(currentChanged(QWidget*)), quantaApp, SLOT(slotUpdateStatus(QWidget*)));
  Document *w;
  do
  {
    if (view->writeExists())
    {
      if (saveModified() )
      {
        w = view->write();
        w->closeTempFile();
        if (!w->isUntitled())
            fileWatcher->removeFile(w->url().path());
      } else
      {
        connect( view->writeTab(), SIGNAL(currentChanged(QWidget*)), quantaApp,   SLOT(slotUpdateStatus(QWidget*)));
        return; //save failed, so don't close anything
      }
    }
  }
  while (  view->removeWrite());
  connect( view->writeTab(), SIGNAL(currentChanged(QWidget*)), quantaApp,   SLOT(slotUpdateStatus(QWidget*)));

  //all documents were removed, so open an empty one
  openDocument( KURL() );
}

void QuantaDoc::readConfig( KConfig *config )
{
  config -> sync();
  QTabWidget *docTab =quantaApp->view()->writeTab();
  Document *w;
  for (int i = docTab->count() -1; i >=0; i--)
  {
    w = dynamic_cast<Document*>(docTab->page(i));
    if (w)
    {
      config->setGroup("General Options");
      w -> readConfig( config );
    }
  }
}

void QuantaDoc::writeConfig( KConfig *config )
{
  if (quantaApp->view()->writeExists())
  {
    config->setGroup("General Options");
    write()-> writeConfig( config );
    config -> sync();
    readConfig( config );
  }
}

bool QuantaDoc::saveModified()
{
  bool completed=true;
  QString fileName = quantaApp->view()->writeTab()->label(quantaApp->view()->writeTab()->currentPageIndex());

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
             completed = quantaApp->slotFileSaveAs();
           }
           else
           {
             completed = saveDocument( write()->url());
           };

           break;

      case KMessageBox::No :
           {
	     write()->removeBackup(quantaApp->config());
      	     completed=true;
	   }
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
  if (quantaApp->view()->writeExists())
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

  QTabWidget *docTab = quantaApp->view()->writeTab();
  Document *w;
  for (int i = docTab->count() -1; i >=0; i--)
  {
    w = dynamic_cast<Document*>(docTab->page(i));
    if (w && w->isModified() ) modified = true;
  }

  return modified;
}

void QuantaDoc::setModified(bool flag)
{
  write()->setModified(flag);
}

Document* QuantaDoc::write() const
{
  return quantaApp->view()->write();
}


Document* QuantaDoc::newWrite()
{
  DTDStruct *dtd = dtds->find(quantaApp->project()->defaultDTD());
  if (!dtd)
      dtd = dtds->find(qConfig.defaultDocType);   //fallback, but not really needed
  int i = 1;
  //while ( isOpened("file:/"+i18n("Untitled%1.").arg(i)+dtd->defaultExtension)) i++;
  while ( isOpened("file:/"+i18n("Untitled%1").arg(i))) i++;

//  QString fname = i18n("Untitled%1.").arg(i)+dtd->defaultExtension;
  QString fname = i18n("Untitled%1").arg(i);

  KTextEditor::Document *doc =
  KTextEditor::createDocument ("libkatepart", quantaApp->view()->writeTab(), "KTextEditor::Document");
/*                               KTextEditor::EditorChooser::createDocument(
                                quantaApp->view->writeTab(),
                                "KTextEditor::Document"
                                );*/
  Document *w = new Document(quantaApp->projectBaseURL(), doc, quantaApp->project(),
                             quantaApp->m_pluginInterface, quantaApp->view()->writeTab());
  w->readConfig(quantaApp->config());
  QString encoding = quantaApp->defaultEncoding();
  dynamic_cast<KTextEditor::EncodingInterface*>(doc)->setEncoding(encoding);

  KTextEditor::View * v = w->view();

  //[MB02] connect all kate views for drag and drop
  connect((QObject *)w->view(), SIGNAL(dropEventPass(QDropEvent *)), (QObject *)quantaApp->gettTab(), SLOT(slotDragInsert(QDropEvent *)));

  w->setUntitledUrl( fname );
  dynamic_cast<KTextEditor::PopupMenuInterface*>(w->view())->installPopup((QPopupMenu *)quantaApp->factory()->container("popup_editor", quantaApp));

  quantaApp->setFocusProxy(w->view());
  w->view()->setFocusPolicy(QWidget::WheelFocus);
  connect( v, SIGNAL(newStatus()),quantaApp, SLOT(slotNewStatus()));

  return w;
}

/** show popup menu with list of attributes for current tag */
void QuantaDoc::slotAttribPopup()
{
  Document *w = write();

  attribMenu->clear();
  uint line, col;
  w->viewCursorIf->cursorPositionReal(&line, &col);

  Node *node = parser->nodeAt(line, col, false);
  if (node && node->tag)
  {
    Tag *tag = node->tag;
    QString tagName = tag->name;
    QStrIList attrList = QStrIList();
    QString name;

    for (int i=0; i < tag->attrCount(); i++ )
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

        QPoint globalPos = w->mapToGlobal(w->viewCursorIf->cursorCoordinates());
        quantaApp->config()->setGroup("Kate Document");
        QFont font;
        font.fromString(quantaApp->config()->readEntry("Font"));
        globalPos.setY(globalPos.y() + QFontMetrics(font).height());
        attribMenu->exec( globalPos );
      }
    }
    else {
      QString message = i18n("Unknown tag: %1").arg(tagName);
      quantaApp->slotStatusMsg( message );
    }
  }
}

void QuantaDoc::slotInsertAttrib( int id )
{
  Document *w = write();
  uint line, col;
  w->viewCursorIf->cursorPositionReal(&line, &col);
  Node *node = parser->nodeAt(line, col);
  if (node && node->tag)
  {
    Tag *tag = node->tag;
    QString tagName = tag->name;
    if ( QuantaCommon::isKnownTag(w->getDTDIdentifier(), tagName) )
    {
      int menuId;
      AttributeList *list = QuantaCommon::tagAttributes(w->getDTDIdentifier(), tagName);
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
      //now insert the new attribute into the tag
      int el, ec;
      tag->endPos(el, ec);
      w->viewCursorIf->setCursorPositionReal( el, ec );
      w->insertTag( " " + QuantaCommon::attrCase(attrStr) + "="+qConfig.attrValueQuotation, qConfig.attrValueQuotation );
    }

    delete attribMenu;
    attribMenu = new KPopupMenu(i18n("Tag"));
    connect( attribMenu, SIGNAL(activated(int)), this, SLOT(slotInsertAttrib(int)));
  }
}

void QuantaDoc::prevDocument()
{
  QTabWidget *tab = quantaApp->view()->writeTab();

  int index = tab->currentPageIndex();
  if (index > 0) index--;
  else index = tab->count()-1;
  tab->showPage(tab->page(index));
}

void QuantaDoc::nextDocument()
{
  QTabWidget *tab = quantaApp->view()->writeTab();

  int index = tab->currentPageIndex();
  if (index + 1 < tab->count() ) index++;
  else index = 0;
  tab->showPage(tab->page(index));
}

void QuantaDoc::changeFileTabName(const KURL &newURL)
{
  Document *w = write();
  KURL url = newURL;

  if (url.isEmpty())
  {
    url = w->url();
  }
  QTabWidget *tab = quantaApp->view()->writeTab();
  if (tab->tabToolTip(w) != url.prettyURL() )
  {
    tab->changeTab( w, QExtFileInfo::shortName(url.path()));
    tab->setTabToolTip( w, url.prettyURL() );
  }

/*  // try to set the icon from mimetype
  QIconSet mimeIcon (KMimeType::pixmapForURL(url, 0, KIcon::Small));
  if (mimeIcon.isNull())
    mimeIcon = QIconSet(SmallIcon("document"));

  tab->setTabIconSet( w, mimeIcon);*/
}

/// SLOTS

void QuantaDoc::undoHistory() {/*write()->undoHistory();*/}
void QuantaDoc::invertSelect(){/*write()->invertSelection();*/}


/** Called when a file on the disk has changed. */
void QuantaDoc::slotFileDirty(const QString& fileName)
{
  Document *w;

  QTabWidget *tab = quantaApp->view()->writeTab();
  for( int i = 0; i < tab->count(); i++)
  {
    w = dynamic_cast<Document*>(tab->page(i));
    if ( w && w->url().path() == fileName )
    {
      w->setDirtyStatus(true);
      if (quantaApp->view()->writeExists() && w == write())
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
  QTabWidget *tab = quantaApp->view()->writeTab();
  for (int i = 0; i < tab->count(); i++)
  {
    w = dynamic_cast<Document*>(tab->page(i));
    if (w && w->url() == url)
    {
      break;
    }
    w = 0L;
  }
  return w;
}

#include "quantadoc.moc"
