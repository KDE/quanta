/***************************************************************************
                          projecttreeview.cpp  -  description
                             -------------------
    begin                : Tue Mar 14 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2001-2004 Andras Mantia <amantia@kde.org>
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
#include <qpainter.h>
#include <qregexp.h>
#include <qlistview.h>

// KDE includes
#include <krun.h>
#include <klocale.h>
#include <kio/job.h>
#include <kopenwith.h>
#include <kiconloader.h>
#include <kinputdialog.h>
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
#ifdef ENABLE_CVSSERVICE
#include "cvsservice.h"
#endif

//ProjectTreeViewItem implementation
ProjectTreeViewItem::ProjectTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch )
: FilesTreeViewItem( parent, item, brnch)
{
}

void ProjectTreeViewItem::paintCell(QPainter *p, const QColorGroup &cg,
                                  int column, int width, int align)
{
    QFont f = p->font();
    if (ProjectTreeView::ref()->isDocumentFolder(url()))
    {
       f.setItalic(true);
       if (text(1).isEmpty())
         setText(1, i18n("Document-base folder"));
    } else
    {
      if (text(1) == i18n("Document-base folder"))
          setText(1, "");
    }
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
  setRootIsDecorated(false);
 // header()->hide();
  setSorting( 0 );
  m_projectFiles.clear();
  setShowSortIndicator(true);
  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn(i18n("Project Files"), -1);
  addColumn(i18n("Description"), -1);
  setFullWidth(true);
  setDragEnabled(false);
  setShowFolderOpenPixmap(false);

  setFocusPolicy(QWidget::ClickFocus);

  m_projectDir =  new ProjectTreeBranch( this, KURL(), i18n("No Project"), SmallIcon("ptab"), true);
  addBranch(m_projectDir);
  m_projectDir->root()->setEnabled(false);

  m_uploadStatusMenu = new KPopupMenu(this);
  m_alwaysUploadId = m_uploadStatusMenu->insertItem(i18n("&When Modified"), this, SLOT(slotAlwaysUpload()));
  m_neverUploadId = m_uploadStatusMenu->insertItem(i18n("&Never"), this, SLOT(slotNeverUpload()));
  m_confirmUploadId = m_uploadStatusMenu->insertItem(i18n("&Confirm"), this, SLOT(slotConfirmUpload()));
  connect(m_uploadStatusMenu, SIGNAL(aboutToShow()), this, SLOT(slotUploadMenuAboutToShow()));

  m_fileMenu = new KPopupMenu(this);

  m_fileMenu->insertItem(SmallIcon("fileopen"), i18n("&Open"), this, SLOT(slotOpen()));
  m_fileMenu->insertItem(i18n("Open &With..."), this, SLOT(slotOpenWith()));
  m_openInQuantaId = m_fileMenu->insertItem(i18n("Load Toolbar"), this, SLOT(slotLoadToolbar()));
  m_fileMenu->insertItem(i18n("Insert &Tag"), this, SLOT(slotInsertTag()));
  m_menuClose = m_fileMenu->insertItem(SmallIcon("fileclose"), i18n("Clos&e"), this, SLOT(slotClose()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem( i18n("&Create Folder..."), this, SLOT(slotCreateFolder()));
  m_fileMenu->insertItem(SmallIcon("editdelete"), i18n("Remove From &Disc (and Project)"), this, SLOT(slotRemove()));
  m_fileMenu->insertItem( i18n("&Remove From Project"), this, SLOT(slotRemoveFromProject(int)));
  m_fileMenu->insertItem(SmallIcon("dirsynch"), i18n("&Upload File..."), this, SLOT(slotUploadSingleURL()));
  m_fileMenu->insertItem(i18n("Rename..."), this, SLOT(slotRename()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(i18n("Upload &status"), m_uploadStatusMenu);
  m_fileMenu->insertItem(SmallIcon("info"), i18n("&Properties..."), this, SLOT(slotProperties()));

  m_folderMenu = new KPopupMenu(this);

  m_folderMenu->insertItem( i18n("&Create Folder..."), this, SLOT(slotCreateFolder()));
  m_folderMenu->insertItem(SmallIcon("editdelete"), i18n("Remove From &Disc (and Project)"), this, SLOT(slotRemove()));
  m_folderMenu->insertItem(i18n("&Remove From Project"), this, SLOT(slotRemoveFromProject(int)));
  m_folderMenu->insertItem(SmallIcon("dirsynch"), i18n("&Upload Folder..."), this, SLOT(slotUploadSingleURL()));
  m_folderMenu->insertItem(i18n("Rename..."), this, SLOT(slotRename()));
  m_folderMenu->insertSeparator();
  m_setDocumentRootId = m_folderMenu->insertItem(i18n("Document-base folder"), this, SLOT(slotChangeDocumentFolderStatus()));
  m_folderMenu->insertItem(i18n("Upload &status"), m_uploadStatusMenu);
  m_folderMenu->insertItem(SmallIcon("info"), i18n("&Properties..."), this, SLOT(slotProperties()));

  m_projectMenu = new KPopupMenu(this);
  m_projectMenu->insertItem(SmallIcon("dirsynch"), i18n("&Upload Project..."), this, SLOT(slotUploadProject()));
  m_projectMenu->insertItem(SmallIcon("reload"), i18n("Re&scan Project Folder..."), this, SLOT(slotRescan()));
  m_projectMenu->insertItem(i18n("Project &Properties..."), this, SLOT(slotOptions()));
  m_projectMenu->insertItem(i18n("&Reload"), this, SLOT(slotReload()));
  m_projectMenu->insertSeparator();
  m_projectMenu->insertItem(i18n("Upload &status"), m_uploadStatusMenu);


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
  KURL url = currentURL();
#ifdef ENABLE_CVSSERVICE
  CVSService::ref()->setRepository(m_baseURL.path());
  if (url.isLocalFile())
  {
    CVSService::ref()->setCurrentFile(url.path());
  }
  else
    CVSService::ref()->setCurrentFile("");
#endif
  ProjectTreeViewItem *curItem = static_cast<ProjectTreeViewItem *>(currentKFileTreeViewItem());
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
        if (url.fileName().endsWith(toolbarExtension))
        {
          m_fileMenu->setItemVisible(m_openInQuantaId, true);
        } else
        {
          m_fileMenu->setItemVisible(m_openInQuantaId, false);
        }
        m_fileMenu->setItemVisible(m_menuClose, isFileOpen(url));
        m_fileMenu->popup(point);
      } else
      {
        if (m_documentFolderList.contains(url))
          m_folderMenu->setItemChecked(m_setDocumentRootId, true);
        else
          m_folderMenu->setItemChecked(m_setDocumentRootId, false);
        m_folderMenu->popup(point);
      }
    }
  }
}

void ProjectTreeView::slotReload()
{
  emit reloadProject();
}

void ProjectTreeView::reload()
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
    m_projectDir =  new ProjectTreeBranch( this, m_baseURL, m_projectNameStr, UserIcon("ptab"), true);
    m_projectDir->root()->setText(1, m_baseURL.prettyURL());
    setDragEnabled(true);
    setRootIsDecorated(true);
  } else {
    m_projectDir =  new ProjectTreeBranch( this, m_baseURL, i18n("No Project"), UserIcon("ptab"), true);
    m_projectDir->root()->setText(1, "");
    setDragEnabled(false);
    setRootIsDecorated(false);
  }

  connect(m_projectDir, SIGNAL(populateFinished(KFileTreeViewItem*)),
          this,           SLOT(slotPopulateFinished(KFileTreeViewItem*)));
  addBranch(m_projectDir);
  m_projectDir->urlList = m_projectFiles;  // set list for filter
  if (m_projectName)
    m_projectDir->populate(m_projectDir->rootUrl(), m_projectDir->root());
  else
    m_projectDir->root()->setEnabled(false);

  if ( m_projectFiles.isEmpty() )
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
  {
    QString projectNameStr = m_projectName+" ";
    if (m_baseURL.protocol() == "file")
    {
      projectNameStr += i18n("[local disk]");
    } else
    {
      projectNameStr += "["+m_baseURL.protocol()+"://"+m_baseURL.user()+"@"+m_baseURL.host()+"]";
    }
    m_projectDir->root()->setText(0, projectNameStr);
    m_projectDir->root()->setText(1, m_baseURL.prettyURL());
    setDragEnabled(true);
  }
  else
  {
    m_projectDir->root()->setText(0, i18n("No Project"));
    m_projectDir->root()->setText(1, "");
    setDragEnabled(false);
  }
  m_documentFolderList.clear();
  m_documentFolderList.append(m_baseURL);
}

void ProjectTreeView::slotReloadTree( const ProjectUrlList &fileList, bool buildNewTree)
{
  m_documentFolderList.clear();
  m_documentFolderList.append(m_baseURL);
  m_projectFiles.clear();
  KURL url;

  // m_projectFiles must be absolute, otherwise filter doesn't work
  for (ProjectUrlList::ConstIterator it = fileList.begin(); it != fileList.end(); ++it )
  {
    url = QExtFileInfo::toAbsolute(*it, m_baseURL);
    url.adjustPath(-1);
    m_projectFiles.append(ProjectURL(url, (*it).fileDesc, (*it).uploadStatus, (*it).documentFolder));
    if ((*it).documentFolder)
      m_documentFolderList.append(url);
  }

  if (buildNewTree)
  {
    reload();
  } else
  {
    if (m_projectDir){
      m_projectDir->urlList = m_projectFiles;  // set list for filter
      KFileTreeViewItem *item;
      KFileTreeViewItem *rootItem = m_projectDir->root();
      QListViewItemIterator iter(this);
      for ( ; iter.current(); ++iter )
      {
        item = dynamic_cast <KFileTreeViewItem*> (iter.current());
        item->setVisible(m_projectFiles.contains(item->url()) || item == rootItem);
      }
    }
  }
}

void ProjectTreeView::slotOpen()
{
  FilesTreeView::slotSelectFile(currentItem());
}

void ProjectTreeView::slotLoadToolbar()
{
 if (currentItem())
 {
   KURL urlToOpen = currentURL();
   if (urlToOpen.fileName().endsWith(toolbarExtension))
   {
      emit loadToolbarFile(urlToOpen);
   }
 }
}

void ProjectTreeView::slotCreateFolder()
{
  bool ok;
  QString folderName = KInputDialog::getText(i18n("Create New Folder"), i18n("Folder name:"), "", &ok, this);
  if (ok)
  {
     KURL url = currentURL();
     if (currentKFileTreeViewItem()->isDir())
       url.setPath(url.path() + "/" + folderName + "/");
     else
       url.setPath(url.directory() + "/" + folderName +"/");
     if (QExtFileInfo::createDir(url))
     {
        emit insertToProject(url);
     }
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

    bool ok;
    QString newName = KInputDialog::getText(caption, i18n("Enter the new name:"), oldName, &ok, this);
    if (ok)
    {
      m_newURL = m_oldURL;
      if (folder)
      {
        m_newURL.setPath(QFileInfo(m_oldURL.path(-1)).dirPath() + '/'+newName+'/');
        m_oldURL.adjustPath(1);
      } else
      {
        m_newURL.setFileName(newName);
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

void ProjectTreeView::slotPopulateFinished(KFileTreeViewItem* item)
{
  FilesTreeView::slotPopulateFinished(item);
  // populate descriptions
  for (ProjectUrlList::ConstIterator it = m_projectFiles.begin();
       it != m_projectFiles.end(); ++it) {
    KFileTreeViewItem* file_item = m_projectDir->findTVIByURL(*it);
    if (file_item)
      file_item->setText(1, (*it).fileDesc);
  }
}

void ProjectTreeView::itemDescChanged(KFileTreeViewItem* item, const QString& newDesc)
{
  if (item) {
    item->setText(1, newDesc);
    emit changeFileDescription(item->url(), newDesc);
  }
}

void ProjectTreeView::slotAlwaysUpload()
{
    KURL url = currentURL();
    int idx = m_projectFiles.findIndex(url);
    if (idx != -1 || url == m_baseURL)
    {
       setUploadStatus(url, ProjectURL::AlwaysUpload);
    }
}

void ProjectTreeView::slotNeverUpload()
{
    KURL url = currentURL();
    int idx = m_projectFiles.findIndex(url);
    if (idx != -1 || url == m_baseURL)
    {
       setUploadStatus(url, ProjectURL::NeverUpload);
    }
}

void ProjectTreeView::slotConfirmUpload()
{
    KURL url = currentURL();
    int idx = m_projectFiles.findIndex(url);
    if (idx != -1 || url == m_baseURL)
    {
       setUploadStatus(url, ProjectURL::ConfirmUpload);
    }
}

void ProjectTreeView::setUploadStatus(const KURL& url, int status)
{
   QString urlStr = url.url();
   ProjectUrlList::Iterator it;
   for (it = m_projectFiles.begin(); it != m_projectFiles.end(); ++it)
   {
       if ((*it).url().startsWith(urlStr))
         (*it).uploadStatus = status;
   }
   emit changeUploadStatus(url, status);
}

void ProjectTreeView::slotUploadMenuAboutToShow()
{
    m_uploadStatusMenu->setItemChecked(m_alwaysUploadId, false);
    m_uploadStatusMenu->setItemChecked(m_neverUploadId, false);
    m_uploadStatusMenu->setItemChecked(m_confirmUploadId, false);
    KURL url = currentURL();
    int idx = m_projectFiles.findIndex(url);
    if (idx != -1)
    {
       ProjectURL pUrl = m_projectFiles[idx];
        switch (pUrl.uploadStatus)
        {
            case ProjectURL::NeverUpload:
               {
                 m_uploadStatusMenu->setItemChecked(m_neverUploadId, true);
                 break;
               }
            case ProjectURL::ConfirmUpload:
               {
                 m_uploadStatusMenu->setItemChecked(m_confirmUploadId, true);
                 break;
               }
            case ProjectURL::AlwaysUpload:
            default:
               {
                 m_uploadStatusMenu->setItemChecked(m_alwaysUploadId, true);
                 break;
               }
        }
    }
}

void ProjectTreeView::slotChangeDocumentFolderStatus()
{
    KURL url = currentURL();
    if (!m_documentFolderList.contains(url))
    {
      m_documentFolderList.append(url);
      setUploadStatus(url, ProjectURL::AlwaysUpload);
      emit changeDocumentFolderStatus(url, true);
    } else
    {
      m_documentFolderList.remove(url);
      emit changeDocumentFolderStatus(url, false);
    }
    currentItem()->repaint();
}

bool ProjectTreeView::isDocumentFolder(const KURL &url)
{
   return (m_documentFolderList.contains(url) > 0);
}

#include "projecttreeview.moc"
