/***************************************************************************
                          projecttreeview.cpp  -  description
                             -------------------
    begin                : Tue Mar 14 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
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

// QT includes
#include <qdir.h>
#include <qpixmap.h>
#include <qheader.h>
#include <qstringlist.h>
#include <qregexp.h>

// KDE includes
#include <krun.h>
#include <klocale.h>
#include <kio/job.h>
#include <kopenwith.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kpopupmenu.h>
#include <kpropertiesdialog.h>
#include <kprogress.h>
#include <kstatusbar.h>

// app includes
#include "projecttreeview.h"
#include "../quantacommon.h"
#include "../qextfileinfo.h"
#include "../resource.h"

//ProjectTreeBranch implementation
ProjectTreeBranch::ProjectTreeBranch(KFileTreeView *parent, const KURL& url,
                                     const QString& name, const QPixmap& pix,
                                     bool showHidden,
                                     KFileTreeViewItem *branchRoot)
    : FilesTreeBranch(parent, url, name, pix, showHidden, branchRoot)
{
}

KFileTreeViewItem* ProjectTreeBranch::createTreeViewItem(KFileTreeViewItem *parent,
                                                        KFileItem *fileItem )
{
  KFileTreeViewItem *tvi = FilesTreeBranch::createTreeViewItem(parent, fileItem);
  if ( tvi && fileItem->url().fileName() == "CVS") {
      tvi->setPixmap( 0, SmallIcon("log") );
      tvi->setExpandable( false );
      tvi->setEnabled( false );
  };
  return tvi;
}

//ProjectTreeView implementation
ProjectTreeView::ProjectTreeView(QWidget *parent, const char *name )
  : FilesTreeView(parent,name)
{
  setRootIsDecorated( true );
 // header()->hide();
  setSorting( 0 );
  m_urlList.clear();

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn(i18n("Project Files"), -1);
  addColumn("");
  setFullWidth(true);

  setFocusPolicy(QWidget::ClickFocus);

  m_projectDir =  new ProjectTreeBranch( this, KURL(), i18n("No project"), SmallIcon("ptab"));
  addBranch(m_projectDir);
  m_projectDir->root()->setEnabled(false);

  m_fileMenu = new KPopupMenu(this);

  m_fileMenu->insertItem(SmallIcon("fileopen"), i18n("&Open"), this, SLOT(slotOpen()));
  m_fileMenu->insertItem(i18n("Open With..."), this, SLOT(slotOpenWith()));
  m_openInQuantaId = m_fileMenu->insertItem(i18n("Open in Quanta"), this, SLOT(slotOpenInQuanta()));
  m_fileMenu->insertItem(i18n("Insert Tag"), this, SLOT(slotInsertTag()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(SmallIcon("editdelete"), i18n("Remove From Disc (and Project)"), this, SLOT(slotRemove()));
  m_fileMenu->insertItem( i18n("Remove From Project"), this, SLOT(slotRemoveFromProject(int)));
  m_fileMenu->insertItem(SmallIcon("dirsynch"), i18n("Upload File..."), this, SLOT(slotUploadSingleURL()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(i18n("Rename..."), this, SLOT(slotRename()));
  m_fileMenu->insertItem(SmallIcon("info"), i18n("Properties"), this, SLOT(slotProperties()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(SmallIcon("reload"), i18n("&Rescan Project Folder"), this, SLOT(slotRescan()));
  m_fileMenu->insertItem(i18n("Pro&ject Options"), this, SLOT(slotOptions()));

  m_folderMenu = new KPopupMenu(this);

/* this can be done by mouse and keyboard
  m_folderMenu->insertItem( SmallIcon("fileopen"), i18n("&Open"), this, SLOT(slotOpen()));
   m_folderMenu->insertSeparator();
*/

  m_folderMenu->insertItem(SmallIcon("editdelete"), i18n("Remove From Disc (and Project)"), this, SLOT(slotRemove()));
  m_folderMenu->insertItem(i18n("Remove From Project"), this, SLOT(slotRemoveFromProject(int)));
  m_folderMenu->insertItem(SmallIcon("dirsynch"), i18n("Upload Folder..."), this, SLOT(slotUploadSingleURL()));
  m_folderMenu->insertSeparator();
  m_folderMenu->insertItem(i18n("Rename..."), this, SLOT(slotRename()));
  m_folderMenu->insertItem(SmallIcon("info"), i18n("Properties"), this, SLOT(slotProperties()));
  m_folderMenu->insertSeparator();
  m_folderMenu->insertItem(SmallIcon("reload"), i18n("&Rescan Project Folder"), this, SLOT(slotRescan()));
  m_folderMenu->insertItem(i18n("Pro&ject Options"), this, SLOT(slotOptions()));

  m_projectMenu = new QPopupMenu(this);
  m_projectMenu->insertItem(SmallIcon("dirsynch"), i18n("&Upload Project..."), this, SLOT(slotUploadProject()));
  m_projectMenu->insertSeparator();
  m_projectMenu->insertItem(SmallIcon("reload"), i18n("&Rescan Project Folder"), this, SLOT(slotRescan()));
  m_projectMenu->insertItem(i18n("Pro&ject Options"), this, SLOT(slotOptions()));


  connect(this, SIGNAL(executed(QListViewItem *)),
          this, SLOT  (slotSelectFile(QListViewItem *)));

  connect(this, SIGNAL(returnPressed(QListViewItem *)),
          this, SLOT  (slotReturnPressed(QListViewItem *)));

  connect(this, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
          this, SLOT(slotMenu(KListView*, QListViewItem*, const QPoint&)));

  connect(this, SIGNAL(open(QListViewItem *)),
          this, SLOT(slotSelectFile(QListViewItem *)));

}

ProjectTreeView::~ProjectTreeView(){
}

/** slot for context menu */
void ProjectTreeView::slotMenu(KListView *listView, QListViewItem *item, const QPoint& point)
{
  Q_UNUSED(listView);
  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if ( item && curItem)
  {
    setSelected(item, true);
    if (curItem == curItem->branch()->root())
    {
     m_projectMenu->popup( point);
    } else
    {
      if (!curItem->isDir())
      {
        if (currentURL().fileName().endsWith(toolbarExtension))
        {
          m_fileMenu->changeItem(m_openInQuantaId, i18n("Load Toolbar File"));
        } else
        {
          m_fileMenu->changeItem(m_openInQuantaId, i18n("Open in Quanta"));
        }
        m_fileMenu->popup(point);
      } else
      {
        m_folderMenu->popup(point);
      }
    }
  }
}

void ProjectTreeView::slotSetBaseURL( const KURL& url )
{
  m_baseURL = url;
}

void ProjectTreeView::slotSetProjectName(const QString& name )
{
  m_projectName = name;
  m_projectDir->root()->setText( 0, name);
}

void ProjectTreeView::slotReloadTree( const KURL::List &urlList, bool buildNewTree)
{
  m_urlList.clear();
  KURL url;

  // m_urlList must be absolute, otherwise filter doesn't work
  for (KURL::List::ConstIterator it = urlList.begin(); it != urlList.end(); ++it )
  {
    url = addBaseURL(*it);
    url.adjustPath(-1);
    m_urlList.append(url);
  }

  if (buildNewTree)
  {
    if (m_projectDir) removeBranch(m_projectDir);
    QString m_projectNameStr = m_projectName+" ";
    bool hasProject = (m_projectName != i18n("No Project"));
    if (hasProject)
    {
     if (m_baseURL.protocol() == "file")
     {
       m_projectNameStr += i18n("[local disk]");
     } else
     {
       m_projectNameStr += "["+m_baseURL.protocol()+"://"+m_baseURL.user()+"@"+m_baseURL.host()+"]";
     }
    }
    m_projectDir =  new ProjectTreeBranch( this, m_baseURL, m_projectNameStr, UserIcon("ptab"));
    addBranch(m_projectDir);
    m_projectDir->urlList = m_urlList;  // set list for filter
    if (hasProject)
      m_projectDir->populate(m_projectDir->rootUrl(), m_projectDir->root());
    else
      m_projectDir->root()->setEnabled(false);

    if ( m_urlList.isEmpty() )
      m_projectDir->root()->setExpandable( false );
    else
      m_projectDir->setOpen( true );
  } else
  {
    if (m_projectDir){
      m_projectDir->urlList = m_urlList;  // set list for filter
      slotRemoveDeleted();
      slotReload();
    }
  }
}

void ProjectTreeView::slotOpen()
{
  FilesTreeView::slotSelectFile(currentItem());
}

void ProjectTreeView::slotOpenInQuanta()
{
 if (currentItem())
 {
   KURL urlToOpen = currentURL();
   if (urlToOpen.fileName().endsWith(toolbarExtension))
   {
      emit loadToolbarFile(urlToOpen);
      return;
   } else
     FilesTreeView::slotSelectFile(currentItem());
  }
}

void ProjectTreeView::slotRemove()
{
  KURL url = currentURL();
  QListViewItem *item = currentItem();
  if ( item &&
       KMessageBox::warningYesNo(this,i18n("<qt>Do you really want to remove <b>%1</b> from the disk?</qt>")
                                      .arg(url.prettyURL(0, KURL::StripFileProtocol)))== KMessageBox::Yes )
  {
    KIO::del( url );
    emit removeFromProject( url);
  }
}

void ProjectTreeView::slotRemoveFromProject(int askForRemove)
{
  QListViewItem *item = currentItem();
  if (item)
  {
    KURL url = currentURL();
    if ( !askForRemove ||
         KMessageBox::warningYesNo(this,i18n("<qt>Do you really want to remove <b>%1</b> from the project?</qt>")
                                        .arg(url.prettyURL(0, KURL::StripFileProtocol))) == KMessageBox::Yes )
    {
      if ( currentKFileTreeViewItem()->isDir() ) url.adjustPath(+1);
      emit removeFromProject(url);
    }
  }
}

void ProjectTreeView::slotRename()
{
  if ( currentItem() )
  {
    emit renameInProject(currentURL());
  }
}


void ProjectTreeView::slotUploadSingleURL()
{
  if (currentItem())
  {
    KURL url = currentURL();
    if ( currentKFileTreeViewItem()->isDir() ) url.adjustPath(+1);
    emit uploadSingleURL(url);
  }
}

/** No descriptions */
void ProjectTreeView::slotRescan()
{
  emit rescanProjectDir();
}

/** Bring up the project options dialog */
void ProjectTreeView::slotOptions()
{
  emit showProjectOptions();
}

/** No descriptions */
void ProjectTreeView::slotUploadProject()
{
  emit uploadProject();
}


/** Remove all the deleted - from the project - url's from the treeview. */
void ProjectTreeView::slotRemoveDeleted()
{
  KFileTreeViewItem *item;
  KURL url;
  QListViewItemIterator iter(this);
  for ( ; iter.current(); ++iter )
  {
    item = dynamic_cast <KFileTreeViewItem*> (iter.current());
    if (item && !m_urlList.contains(item->url()) && item != m_projectDir->root())
    {
      delete item;
    }
  }
}

  KURL ProjectTreeView::addBaseURL(const KURL&  url) {

//  kdDebug(24000) << url.path() << "|" << m_baseURL.path() << endl;
    if (m_baseURL.isEmpty())
      return url;
    else
      return QExtFileInfo::toAbsolute(url, m_baseURL);;
  }

void ProjectTreeView::itemRenamed(const KURL& oldUrl, const KURL& newUrl )
{
  // simply undo the renaming, user must use rbm menu for a save rename,
  // because project musst be changed also
  KIO::SimpleJob *job = KIO::rename( newUrl, oldUrl, true );
  Q_UNUSED(job);
}



#include "projecttreeview.moc"
