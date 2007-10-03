/***************************************************************************
                          rescanprj.cpp -  description
                             -------------------
    begin                : ?
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2002, 2003 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// qt includes
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>

// kde includes
#include <kdebug.h>
#include <kurl.h>
#include <klocale.h>
#include <kfileitem.h>
#include <kglobal.h>
#include <kprogress.h>

// app includes
#include "rescanprj.h"
#include "qextfileinfo.h"
#include "quantacommon.h"
#include "uploadtreefolder.h"
#include "uploadtreeview.h"
#include "resource.h"
#include "projectlist.h"


RescanPrj::RescanPrj(const ProjectList &p_prjFileList, const KURL& p_baseURL, const QRegExp &p_excludeRx,
                       QWidget *parent, const char *name, bool modal )
  : RescanPrjDir(parent,name,modal)
{
  setCaption(name);

  listView->setColumnText(1, i18n("Add"));
  baseURL = p_baseURL;
  baseURL.adjustPath(1);

  prjFileList = p_prjFileList;
  excludeRx = p_excludeRx;

  progressText->setText(i18n("Reading folder:"));
  KIO::ListJob *job = KIO::listRecursive( baseURL, false );
  m_listJobCount = 1;

  connect( job, SIGNAL(entries(KIO::Job *,const KIO::UDSEntryList &)),
           this,SLOT  (addEntries(KIO::Job *,const KIO::UDSEntryList &)));
  connect( job, SIGNAL(result(KIO::Job *)),
           this,SLOT  (slotListDone(KIO::Job *)));


  connect( buttonSelect,   SIGNAL(clicked()),
           this,           SLOT(slotSelect()));
  connect( buttonDeselect, SIGNAL(clicked()),
           this,           SLOT(slotDeselect()));
  connect( buttonInvert,   SIGNAL(clicked()),
           this,           SLOT(slotInvert()));
  connect( buttonExpand,   SIGNAL(clicked()),
           this,           SLOT(slotExpand()));
  connect( buttonCollapse, SIGNAL(clicked()),
           this,           SLOT(slotCollapse()));
}

RescanPrj::~RescanPrj()
{
  for (uint i = 0; i < urlList.count(); i++)
  {
    delete urlList[i].fileItem;
  }
  urlList.clear();
}

void RescanPrj::addEntries(KIO::Job *job,const KIO::UDSEntryList &list)
{
  KURL url = static_cast<KIO::ListJob *>(job)->url();
  url.adjustPath(-1);
  // avoid creating these QStrings again and again
  static const QString& dot = KGlobal::staticQString(".");
  static const QString& dotdot = KGlobal::staticQString("..");

  KIO::UDSEntryListConstIterator it  = list.begin();
  KIO::UDSEntryListConstIterator end = list.end();
  KURL itemURL;
  URLListEntry urlEntry;
  QString name;
  QPtrList<KFileItem> linkItems;
  linkItems.setAutoDelete(true);
  for ( ; it != end; ++it )
  {
    KFileItem item( *it, url, false, true );
    name = item.name();
    if (item.isDir() && item.isLink())
    {
      QString linkDest = item.linkDest();
      kdDebug(24000) << "Got link: " << name << " Points to:" << linkDest << endl;
      KURL u = item.url();
      if (linkDest.startsWith("."))
      {
        u.setPath(u.directory(false, true) + linkDest);
        u.cleanPath();
      }
      else       
        u.setPath(linkDest);
      u.adjustPath(+1);
      KURL u2 = QExtFileInfo::toRelative(u, baseURL);
      bool found = false;
      for (uint i = 0; i < urlList.count(); i++)
       if (urlList[i].url == u2)
       { 
         found = true;
         break;
       }
      if (!prjFileList.contains(u) && !found)
      {
        linkItems.append(new KFileItem(item));
      } else
      {
        kdDebug(24000) << "Recursive link - points to a place inside the project" << endl;
        continue;
      }
    }
    if (!name.isEmpty() && name != dot && name != dotdot && !excludeRx.exactMatch(name))
    {
      itemURL = item.url();
      if (item.isDir())
        itemURL.adjustPath(+1);
      ProjectURL *proUrl = prjFileList.find(itemURL);
      if (!proUrl)
      {
        urlEntry.url = prjFileList.toRelative(itemURL);
        urlEntry.fileItem = new KFileItem(item);
        urlList.append(urlEntry);
      }
    }
  }
 for (QPtrList<KFileItem>::ConstIterator it = linkItems.constBegin(); it != linkItems.constEnd(); ++it)
  {
    KIO::ListJob *ljob = KIO::listRecursive( (*it)->url(), false );
    m_listJobCount++;

    connect( ljob, SIGNAL(entries(KIO::Job *,const KIO::UDSEntryList &)),
             this,SLOT  (addEntries(KIO::Job *,const KIO::UDSEntryList &)));
    connect( ljob, SIGNAL(result(KIO::Job *)),
             this,SLOT  (slotListDone(KIO::Job *)));
  }
}

void RescanPrj::resizeEvent ( QResizeEvent *t )
{
  RescanPrjDir::resizeEvent(t);
//  listView->setColumnWidth(0,listView->width()-listView->columnWidth(1)-20);
//  MainLayout->setGeometry(childrenRect());
}

void RescanPrj::slotSelect()
{
  listView->selectAll(true);
  listView->checkboxTree();
}

void RescanPrj::slotDeselect()
{
  listView->selectAll(false);
  listView->checkboxTree();
}

void RescanPrj::slotInvert()
{
  listView->invertAll();
  listView->checkboxTree();
}

void RescanPrj::slotExpand()
{
  listView->expandAll();
}

void RescanPrj::slotCollapse()
{
  listView->collapseAll();
}

KURL::List RescanPrj::files()
{
  KURL::List r;

  QListViewItem *item;
  QListViewItemIterator it(listView);
  for ( ; it.current(); ++it )
  {
   item = it.current();
   if ( listView->isSelected( item ))
   {
     KURL u;
     if (dynamic_cast<UploadTreeFolder*>(item))
     {
      u = dynamic_cast<UploadTreeFolder*>(item)->url();
     } else
     {
      u = dynamic_cast<UploadTreeFile*>(item)->url();
     }

     if (!u.isEmpty())
       r.append( QExtFileInfo::toAbsolute(u, baseURL) );
   }
  }
  return r;
}
/** No descriptions */
void RescanPrj::slotListDone(KIO::Job *)
{
  m_listJobCount--;
 // kdDebug(24000) << "slotListDone " << m_listJobCount << endl;
  if (m_listJobCount == 0)
  {
    progressText->setText(i18n("Building tree:"));
    progressText->repaint();
    progress->setTotalSteps(urlList.count());
    progress->setValue(0);
    URLListEntry urlEntry;
    for (uint i = 0; i < urlList.count(); i++)
    {
      urlEntry = urlList[i];
      listView->addItem(urlEntry.url, *(urlEntry.fileItem));
      progress->advance(1);
    }

    progress->setTotalSteps(1);
    progress->setValue(0);
    progress->setTextEnabled(false);
    progressText->setText(i18n("Progress:"));
    slotSelect();
  }
}

#include "rescanprj.moc"
