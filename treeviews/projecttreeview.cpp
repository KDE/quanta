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
#include <qfont.h>
#include <qpainter.h>
#include <qlistview.h>

// KDE includes
#include <krun.h>
#include <klocale.h>
#include <kio/job.h>
#include <kopenwith.h>
#include <kiconloader.h>
#include <klineeditdlg.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kpopupmenu.h>
#include <kpropertiesdialog.h>
#include <kprogress.h>
#include <kstatusbar.h>

// app includes
#include "projecttreeview.h"
#include "quantacommon.h"
#include "qextfileinfo.h"
#include "resource.h"
#include "quanta.h"
#include "quantadoc.h"

//ProjectTreeViewItem implementation
ProjectTreeViewItem::ProjectTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch )
: FilesTreeViewItem( parent, item, brnch)
{
}

// make open project files bold
void ProjectTreeViewItem::paintCell(QPainter *p, const QColorGroup &cg,
                                    int column, int width, int align)
{
    QFont f = p->font();
    f.setBold(quantaApp->doc()->isOpened(this->url()));
    p->setFont(f);
    FilesTreeViewItem::paintCell( p, cg, column, width, align );
}


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
  FilesTreeViewItem  *tvi = 0;
  if( parent && fileItem )
  {
    tvi = new ProjectTreeViewItem( parent, fileItem, this );
    if (tvi)
    {
      // we assume there are childs
      tvi->setExpandable(tvi->isDir());
      tvi->setVisible(urlList.contains(fileItem->url()));  // only listelements
      if (fileItem->url().fileName() == "CVS") {
          tvi->setPixmap( 0, SmallIcon("log") );
          tvi->setExpandable( false );
          tvi->setEnabled( false );
      };
    }
  }
  else
    kdDebug(24000) << "ProjectTreeBranch::createTreeViewItem: Have no parent" << endl;
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
  setShowSortIndicator(true);
  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn(i18n("Project Files"), -1);
  addColumn("");
  setFullWidth(true);
  setDragEnabled(true);

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
  m_fileMenu->insertItem(SmallIcon("info"), i18n("Properties..."), this, SLOT(slotProperties()));
  
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
  m_folderMenu->insertItem(SmallIcon("info"), i18n("Properties..."), this, SLOT(slotProperties()));

  m_projectMenu = new QPopupMenu(this);
  m_projectMenu->insertItem(SmallIcon("dirsynch"), i18n("&Upload Project..."), this, SLOT(slotUploadProject()));
  m_projectMenu->insertItem(SmallIcon("reload"), i18n("&Rescan Project Folder..."), this, SLOT(slotRescan()));
  m_projectMenu->insertItem(i18n("Pro&ject Properties..."), this, SLOT(slotOptions()));
  m_projectMenu->insertItem(i18n("Reload"), this, SLOT(slotReload()));


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

void ProjectTreeView::slotReload()
{
  if (m_projectDir)
    removeBranch(m_projectDir);
  QString m_projectNameStr = m_projectName+" ";
  if (m_projectName)
  {
    if (m_baseURL.protocol() == "file")
    {
      m_projectNameStr += i18n("[local disk]");
    } else
    {
      m_projectNameStr += "["+m_baseURL.protocol()+"://"+m_baseURL.user()+"@"+m_baseURL.host()+"]";
    }
    m_projectDir =  new ProjectTreeBranch( this, m_baseURL, m_projectNameStr, UserIcon("ptab"));
  } else
    m_projectDir =  new ProjectTreeBranch( this, m_baseURL, i18n("No Project"), UserIcon("ptab"));

  addBranch(m_projectDir);
  m_projectDir->urlList = m_urlList;  // set list for filter
  if (m_projectName)
    m_projectDir->populate(m_projectDir->rootUrl(), m_projectDir->root());
  else
    m_projectDir->root()->setEnabled(false);

  if ( m_urlList.isEmpty() )
    m_projectDir->root()->setExpandable( false );
  else
    m_projectDir->setOpen( true );
}

/** Sets the project template directory */
void ProjectTreeView::slotNewProjectLoaded(const QString &name, const KURL &baseURL, const KURL &)
{
  m_baseURL = baseURL;
  m_projectName = name;
  if (!m_projectName.isNull())
    m_projectDir->root()->setText( 0, name);
  else
    m_projectDir->root()->setText( 0, i18n("No Project"));
}

void ProjectTreeView::slotReloadTree( const KURL::List &urlList, bool buildNewTree)
{
  m_urlList.clear();
  KURL url;

  // m_urlList must be absolute, otherwise filter doesn't work
  for (KURL::List::ConstIterator it = urlList.begin(); it != urlList.end(); ++it )
  {
    url = QExtFileInfo::toAbsolute(*it, m_baseURL);
    url.adjustPath(-1);
    m_urlList.append(url);
  }

  if (buildNewTree)
  {
    slotReload();
  } else
  {
    if (m_projectDir){
      m_projectDir->urlList = m_urlList;  // set list for filter
      KFileTreeViewItem *item;
      KFileTreeViewItem *rootItem = m_projectDir->root();
      QListViewItemIterator iter(this);
      for ( ; iter.current(); ++iter )
      {
        item = dynamic_cast <KFileTreeViewItem*> (iter.current());
        item->setVisible(m_urlList.contains(item->url()) || item == rootItem);
      }
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
    if (currentKFileTreeViewItem()->isDir()) url.adjustPath(+1);
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
    m_oldURL = currentURL();
    QString oldName;
    QString caption;
    bool folder = false;
    if ( currentKFileTreeViewItem()->isDir()) 
    {
      caption = i18n("Rename Folder");
      oldName = m_oldURL.fileName(true);
      folder = true;
    } else
    {
      oldName = m_oldURL.fileName();
      caption = i18n("Rename File");
    }      
    
    KLineEditDlg dlg(i18n("Enter the new name:"), oldName, this);
    dlg.setCaption(caption);
    if (dlg.exec())
    {
      m_newURL = m_oldURL;
      if (folder)
      {
        m_newURL.setPath(QFileInfo(m_oldURL.path(-1)).dirPath() + '/'+dlg.text()+'/');
        m_oldURL.adjustPath(1);
      } else
      {
        m_newURL.setFileName(dlg.text());
      }
      if ( m_oldURL != m_newURL )
      {
        //set the class global attributes
        KURL oldURL = QExtFileInfo::toAbsolute(m_oldURL, m_baseURL);
        KURL newURL = QExtFileInfo::toAbsolute(m_newURL, m_baseURL);
        bool proceed = true;
        if (QExtFileInfo::exists(newURL))
        {
          proceed = KMessageBox::warningYesNo(this, i18n("<qt>The file <b>%1</b> already exists.<br>Do you want to overwrite it?</qt>").arg(newURL.prettyURL(0, KURL::StripFileProtocol)),i18n("Overwrite")) == KMessageBox::Yes;
        }
        if (proceed)
        {
        //start the rename job
          KIO::SimpleJob *job = KIO::rename( oldURL, newURL, true );
          connect(job, SIGNAL( result( KIO::Job *) ), SLOT(slotRenameFinished( KIO::Job *)));
        }
      }  
    }
  }
}

void ProjectTreeView::slotRenameFinished(KIO::Job *)
{
  emit renamed(m_oldURL, m_newURL);
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


void ProjectTreeView::slotDocumentClosed()
{
  QListViewItemIterator iter(this);
  for ( ; iter.current(); ++iter )
  {
    iter.current()->repaint();
  }
}

#include "projecttreeview.moc"
