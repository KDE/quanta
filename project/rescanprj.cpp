/***************************************************************************
                          rescanprj.cpp -  description
                             -------------------
    begin                : ?
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2002 Andras Mantia <amantia@freemail.hu>
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "rescanprj.h"
#include "rescanprj.moc"
// qt includes
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qregexp.h>
#include <qlabel.h>

// kde includes
#include <kurl.h>
#include <klocale.h>
#include <kfileitem.h>
#include <kglobal.h>
#include <kprogress.h>

// app includes
#include "rescanprj.h"
#include "../qextfileinfo.h"
#include "../quantacommon.h"
#include "../treeviews/uploadtreefolder.h"
#include "../treeviews/uploadtreeview.h"
#include "../resource.h"

RescanPrj::RescanPrj(KURL::List p_prjFileList, const KURL& p_baseURL, QWidget *parent, const char *name, bool modal )
  : RescanPrjDir(parent,name,modal)
{
  setCaption(name);

  listView->setColumnText(1, i18n("Add"));
  baseURL = p_baseURL;
  baseURL.adjustPath(1);

  prjFileList = p_prjFileList;

  progressText->setText(i18n("Reading directory:"));
  KIO::ListJob *job = KIO::listRecursive( baseURL, false );

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

RescanPrj::~RescanPrj(){
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
  for ( ; it != end; ++it )
  {
    KFileItem item( *it, url, false, true );
    name = item.name();
    if ( ! name.isEmpty() && name != dot && name != dotdot)
    {
      itemURL = item.url();
      if (item.isDir()) itemURL.adjustPath(1);
//    itemURL = QExtFileInfo::toRelative(itemURL, baseURL);
      itemURL.setPath(itemURL.path().replace(QRegExp(baseURL.path()),""));
      if (prjFileList.findIndex(itemURL) == -1 )
      {
        urlEntry.url = itemURL;
        urlEntry.date = item.timeString();
        urlEntry.size = QString("%1").arg( (long int)item.size() );
        urlList.append(urlEntry);
      }
    }
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

     if (!u.isEmpty()) r.append(u);
   }
  }
  return r;
}
/** No descriptions */
void RescanPrj::slotListDone(KIO::Job *)
{
  progressText->setText(i18n("Building tree:"));
  progressText->repaint();
  progress->setTotalSteps(urlList.count());
  progress->setValue(0);
  URLListEntry urlEntry;
  for (uint i = 0; i < urlList.count(); i++)
  {
    urlEntry = urlList[i];
    listView->addItem(urlEntry.url, urlEntry.size, urlEntry.date);
    progress->advance(1);
  }
  
  progress->setValue(0);
  progress->setTextEnabled(false);
  slotSelect();
  progressText->setText(i18n("Progress:"));
}
