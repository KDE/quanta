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
#include <ktexteditor/editinterface.h>

#include <kparts/componentfactory.h>

#include <klibloader.h>
#include <ktrader.h>


// application specific includes
#include "document.h"
#include "quanta.h"
#include "quantadoc.h"
#include "quantaview.h"
#include "viewmanager.h"

#include "quantacommon.h"
#include "qextfileinfo.h"
#include "resource.h"
#include "templatestreeview.h"
#include "phpdebuggerinterface.h"

#include "project.h"
//#include "dtds.h"

#ifdef BUILD_KAFKAPART
#include "undoredo.h"
#endif

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


bool QuantaDoc::newDocument( const KURL& url, bool switchToExisting )
{
  bool newfile = false;
  if ( url.url().isEmpty() ) newfile = true;
  Document *w = 0L;
  QuantaView *view = ViewManager::ref()->isOpened(url);
  if (!view || newfile)
  {
    // no modi and new -> we can remove                           !!!!
    if (ViewManager::ref()->activeView())
      w = ViewManager::ref()->activeView()->document();
    if (w && !w->isModified() &&
         w->isUntitled() && !w->busy)
         return true;

    // now we can create new kwrite
    ViewManager::ref()->createNewDocument();
    view = ViewManager::ref()->activeView();

    quantaApp->processDTD(Project::ref()->defaultDTD());
  }
  else // select opened
  if (switchToExisting)
  {
    view->document()->checkDirtyStatus();
    view->activate();
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
  Document *w = ViewManager::ref()->activeView()->document();
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

#if KDE_IS_VERSION(3,1,90)
    connect(w->doc(), SIGNAL(completed()), this, SLOT(slotOpeningCompleted()));
#endif
    if (w->doc()->openURL( url ))
    {
      loaded = true;
#if KDE_VERSION < KDE_MAKE_VERSION(3,1,90)
      slotOpeningCompleted();
#endif
    }
  }
  if (!loaded && !url.isEmpty()) //the open of the document has failed*/
  {
    bool signalStatus = signalsBlocked();
    blockSignals(false);
    emit hideSplash();
    KMessageBox::error(quantaApp, i18n("<qt>Cannot open document <b>%1</b>.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)));
    ViewManager::ref()->removeActiveView();
    blockSignals(signalStatus);
  }
  if (url.isEmpty())
  {
    quantaApp->processDTD();
    quantaApp->reparse(true);
  }
}

void QuantaDoc::slotOpeningCompleted()
{
  Document *w = ViewManager::ref()->activeView()->document();
  w->setDirtyStatus(false);
  //  kdDebug(24000) << "Text: " << w->editIf->text() << endl;

  //changeFileTabName(); //FIXME:
  quantaApp->fileRecent->addURL( w->url() );

  quantaApp->slotRepaintPreview();

  w->createTempFile();
  w->view()->setFocus();

  quantaApp->processDTD();
  quantaApp->reparse(true);

  quantaApp->debugger()->fileOpened(w->url());
  emit newStatus();
#if KDE_IS_VERSION(3,1,90)
   disconnect(w->doc(), SIGNAL(completed()), this, SLOT(slotOpeningCompleted()));
#endif
}



void QuantaDoc::readConfig( KConfig *config )
{
//FIXME:
/*
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
  } */
}

void QuantaDoc::writeConfig( KConfig *config )
{
//FIXME:
/*
  if (quantaApp->view()->writeExists())
  {
    config->setGroup("General Options");
    write()-> writeConfig( config );
    config -> sync();
    readConfig( config );
  } */
}

/** show popup menu with list of attributes for current tag */
void QuantaDoc::slotAttribPopup()
{
  Document *w = ViewManager::ref()->activeView()->document();
  if (!w)  return;

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
  Document *w = ViewManager::ref()->activeView()->document();
  if (!w)  return;
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
//FIXME:
/*
  QTabWidget *tab = quantaApp->view()->writeTab();

  int index = tab->currentPageIndex();
  if (index > 0) index--;
  else index = tab->count()-1;
  tab->showPage(tab->page(index)); */
}

void QuantaDoc::nextDocument()
{
//FIXME:
/*
  QTabWidget *tab = quantaApp->view()->writeTab();

  int index = tab->currentPageIndex();
  if (index + 1 < tab->count() ) index++;
  else index = 0;
  tab->showPage(tab->page(index)); */
}

void QuantaDoc::changeFileTabName(const KURL &newURL)
{
//FIXME:
/*
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

    // try to set the icon from mimetype
  QIconSet mimeIcon (KMimeType::pixmapForURL(url, 0, KIcon::Small));
  if (mimeIcon.isNull())
    mimeIcon = QIconSet(SmallIcon("document"));
#if KDE_IS_VERSION(3,1,90)
  if (qConfig.showCloseButtons)
  {
    if (w->isModified())
    {
      tab->changeTab( w, SmallIcon("fileclose"), tab->tabLabel(w) + " " + i18n("[modified]"));
    } else
    {
      tab->changeTab( w, SmallIcon("fileclose"), tab->tabLabel(w));
    }
  } else
#endif
  {
    if ( w->isModified() )
      tab->changeTab( w, UserIcon("save_small"), tab->tabLabel(w));
    else
      tab->changeTab( w, mimeIcon,  tab->tabLabel(w));
  } */
}

/// SLOTS

void QuantaDoc::undoHistory() {/*write()->undoHistory();*/}
void QuantaDoc::invertSelect(){/*write()->invertSelection();*/}


/** Called when a file on the disk has changed. */
void QuantaDoc::slotFileDirty(const QString& fileName)
{
    Document *w;
    KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
    QuantaView *view;
    for (it->first(); !it->isDone(); it->next())
    {
        view = dynamic_cast<QuantaView*>(it->currentItem());
        if (view)
        {
            w = view->document();
            if ( w && w->url().path() == fileName && !w->dirty())
            {
              w->setDirtyStatus(true);
              Document *activeW = ViewManager::ref()->activeView()->document();
              if (activeW && w == activeW)
              {
                  w->checkDirtyStatus();
              }
            }
        }
    }
    delete it;
}

#include "quantadoc.moc"
