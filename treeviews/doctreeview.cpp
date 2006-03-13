/***************************************************************************
                          doctreeview.cpp  -  description
                             -------------------
    begin                : Sat Mar 4 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2002, 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// QT clases
#include <qstrlist.h>
#include <qheader.h>
#include <qpixmap.h>
#include <qdir.h>

// KDE clases
#include <kconfig.h>
#include <kapplication.h>
#include <klocale.h>
#include <kpopupmenu.h>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <kurl.h>

// application clases
#include "doctreeview.h"
#include "docfolder.h"
#include "docitem.h"

DocTreeView::DocTreeView(QWidget *parent, const char *name )
  : KListView(parent,name)
{

  contextHelpDict = new QDict<QString>( 101, false );

  setRootIsDecorated( true );
  header()->hide();
  setSorting(-1,false);

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn(i18n("Name"), -1);
  addColumn("");
  setFullWidth(true);

  projectDocFolder = new KListViewItem(this, i18n("Project Documentation"));
  projectDocFolder->setOpen(true);
  slotRefreshTree();
  setFocusPolicy(QWidget::ClickFocus);

  connect(this, SIGNAL(executed(QListViewItem *)), SLOT(clickItem(QListViewItem *)) );
  connect(this, SIGNAL(returnPressed(QListViewItem *)), SLOT(clickItem(QListViewItem *)));
  connect(this, SIGNAL(doubleClicked(QListViewItem *)), SLOT(slotDoubleClicked(QListViewItem *)));

  m_contextMenu  = new KPopupMenu(this);
  m_menuReload = m_contextMenu->insertItem(i18n("&Reload"), this, SLOT(slotReloadProjectDocs()));
  m_contextMenu->insertItem(SmallIcon("network"), i18n("&Download Documentation..."), this, SIGNAL(downloadDoc()));
  connect(this, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
          this, SLOT(slotMenu(KListView*, QListViewItem*, const QPoint&)));
}


DocTreeView::~DocTreeView(){
  contextHelpDict->setAutoDelete(true);
  delete contextHelpDict;
}

void DocTreeView::slotRefreshTree()
{
  for (QValueList<DocFolder *>::Iterator it = m_folderList.begin(); it != m_folderList.end(); ++it)
  {
    delete *it;
  }
  m_folderList.clear();
  QStringList docDirs = KGlobal::instance()->dirs()->findDirs("appdata", "doc");

  for ( QStringList::Iterator it = docDirs.begin(); it != docDirs.end(); ++it )
  {
    QString docDir = *it;
    QDir dir(docDir, "*.docrc");
    QStringList files = dir.entryList();

    for ( QStringList::Iterator it_f = files.begin(); it_f != files.end(); ++it_f )
    {
      KConfig config( docDir + *it_f );
      config.setGroup("Tree");

      QString relDocDir = config.readEntry("Doc dir");
      QString name = config.readEntry("Name").lower();

      DocFolder *folder = new DocFolder(this, config.readEntry("Top Element"), &config , QDir::cleanDirPath(docDir+relDocDir)+"/");
      folder->setPixmap( 0, SmallIcon("folder_open") );
      folder->topLevel = true;
      folder->setOpen(true);
      m_folderList.append(folder);

      config.setGroup("Context");
      QStrList list;
      config.readListEntry("ContextList", list );

      for ( unsigned int i=0; i<list.count(); i++ )
      {
        QString keyword = list.at(i);
        QString *url = new QString(QDir::cleanDirPath(docDir + relDocDir + "/" + config.readEntry( list.at(i) )));
        contextHelpDict->insert( name + "|" + keyword, url );
      }
    }
  }

}

void DocTreeView::clickItem( QListViewItem *)
{
  QListViewItem *it = currentItem();
  if ( !it )
    return;
  DocItem *dit = dynamic_cast< DocItem *>(it);
  if ( dit )
    if ( ! dit->url.isEmpty() )
        emit openURL( dit->url);

  DocFolder *dfol = dynamic_cast< DocFolder *>(it);
  if ( dfol )
    if ( ! dfol->url.isEmpty() )
      emit openURL( dfol->url );
  //else
  //  emit openURL( locate("appdata","doc/documentation.html") );
}


QString * DocTreeView::contextHelp(const QString &keyword)
{
  QString word = keyword.mid(keyword.find("|"));
  if (contextHelpDict->find(keyword))
    return contextHelpDict->find(keyword);
  else
    return contextHelpDict->find(word); //to support old documentation packages
}

void DocTreeView::slotDoubleClicked(QListViewItem *item )
{
  if (item)
  {
    item->setOpen(!item->isOpen());
  }
}

void DocTreeView::slotAddProjectDoc(const KURL& url)
{
  QString path = url.path();
  int pos = path.find("/doc/");
  path = path.mid(pos + 5);
  new DocItem(projectDocFolder, path, url.url());
}

void DocTreeView::slotMenu(KListView *, QListViewItem *item, const QPoint &point)
{
  m_contextMenu->setItemVisible(m_menuReload, false);
  if (item)
  {
    setSelected(item, true);
    if (currentItem() == projectDocFolder)
    {
      m_contextMenu->setItemVisible(m_menuReload, true);
    }
  }
  m_contextMenu->popup(point);
}

void DocTreeView::slotNewProjectLoaded(const QString &, const KURL &, const KURL &)
{
  slotReloadProjectDocs();
}

void DocTreeView::slotReloadProjectDocs()
{
  QListViewItem *child = projectDocFolder->firstChild();
  while (child) {
      QListViewItem *c = child;
      child = child->nextSibling();
      delete c;
  }
  emit reloadProjectDocs();
}

#include "doctreeview.moc"
