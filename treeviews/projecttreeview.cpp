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
#include <qpainter.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qdict.h>

// KDE includes
#include <klocale.h>
#include <kiconloader.h>
#include <kinputdialog.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kpopupmenu.h>
#include <kpropertiesdialog.h>
#include <ktempfile.h>
#include <kapplication.h>
#include <kstringhandler.h>

// app includes
#include "projecttreeview.h"
#include "projectlist.h"
#include "projecturl.h"
#include "quantacommon.h"
#include "qextfileinfo.h"
#include "resource.h"
#include "fileinfodlg.h"
#ifdef ENABLE_CVSSERVICE
#include "cvsservice.h"
#endif

//ProjectTreeViewItem implementation
ProjectTreeViewItem::ProjectTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch )
: BaseTreeViewItem( parent, item, brnch)
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
         setText(1, i18n("Document Base Folder"));
    } else
    {
      if (text(1) == i18n("Document Base Folder"))
          setText(1, "");
    }
    p->setFont(f);
    BaseTreeViewItem::paintCell( p, cg, column, width, align );
}

//ProjectTreeBranch implementation
ProjectTreeBranch::ProjectTreeBranch(KFileTreeView *parent, const KURL& url,
                                     const QString& name, const QPixmap& pix,
                                     bool showHidden,
                                     KFileTreeViewItem *branchRoot)
    : BaseTreeBranch(parent, url, name, pix, showHidden, branchRoot)
{
}

KFileTreeViewItem* ProjectTreeBranch::createTreeViewItem(KFileTreeViewItem *parent,
                                                         KFileItem *fileItem )
{
  BaseTreeViewItem  *tvi = 0;
  if( parent && fileItem )
  {
    tvi = new ProjectTreeViewItem( parent, fileItem, this );
    if (tvi)
    {
      // we assume there are childs
      tvi->setExpandable(tvi->isDir());
      if (urlList) {
        tvi->setVisible(urlList->contains(fileItem->url()));  // only listelements
      }
    }
  }
  else
    kdDebug(24000) << "ProjectTreeBranch::createTreeViewItem: Have no parent" << endl;
  return tvi;
}

//ProjectTreeView implementation
ProjectTreeView::ProjectTreeView(QWidget *parent, const char *name )
  : BaseTreeView(parent,name), m_projectFiles(0)
{
  //setSelectionModeExt(KListView::Extended);
  setRootIsDecorated(false);
  addColumn(i18n("Project Files"), -1);
  addColumn(i18n("Description"), -1);
  setDragEnabled(true);
  setAcceptDrops(true);
  setItemsRenameable(true);
  setRenameable(0, false);
  setRenameable(1, true);

  m_projectBaseURL = KURL();
  newBranch(m_projectBaseURL);  // create an empty project branch
  m_projectDir->root()->setEnabled(false);

  m_uploadStatusMenu = new KPopupMenu(this);
  m_alwaysUploadId = m_uploadStatusMenu->insertItem(i18n("&When Modified"), this, SLOT(slotAlwaysUpload()));
  m_neverUploadId = m_uploadStatusMenu->insertItem(i18n("&Never"), this, SLOT(slotNeverUpload()));
  m_confirmUploadId = m_uploadStatusMenu->insertItem(i18n("&Confirm"), this, SLOT(slotConfirmUpload()));
  connect(m_uploadStatusMenu, SIGNAL(aboutToShow()), this, SLOT(slotUploadMenuAboutToShow()));

  m_fileMenu = new KPopupMenu(this);

  m_fileMenu->insertItem(SmallIcon("fileopen"), i18n("&Open"), this, SLOT(slotOpen()));
  m_openInQuantaId = m_fileMenu->insertItem(i18n("Load Toolbar"), this, SLOT(slotLoadToolbar()));
  m_fileMenu->insertItem(i18n("Insert &Tag"), this, SLOT(slotInsertTag()));
  m_menuClose = m_fileMenu->insertItem(SmallIcon("fileclose"), i18n("Clos&e"), this, SLOT(slotClose()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(SmallIcon("up"), i18n("&Upload File..."), this, SLOT(slotUploadSingleURL()));
  m_fileMenu->insertItem(SmallIcon("up"), i18n("&Quick File Upload"), this, SLOT(slotQuickUploadURL()));
  m_fileMenu->insertItem(i18n("Re&name"), this, SLOT(slotStartRename()));
  m_fileMenu->insertItem( i18n("&Remove From Project"), this, SLOT(slotRemoveFromProject(int)));
  m_fileMenu->insertItem(SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(i18n("Upload &Status"), m_uploadStatusMenu);
  m_fileMenu->insertItem(SmallIcon("info"), i18n("&Properties"), this, SLOT(slotProperties()));

  KPopupMenu *createNewMenu = new KPopupMenu(this);
  createNewMenu->insertItem(SmallIcon("folder_new"), i18n("F&older..."), this, SLOT(slotCreateFolder()));
  createNewMenu->insertItem(SmallIcon("document"), i18n("&File..."), this, SLOT(slotCreateFile()));

  m_folderMenu = new KPopupMenu(this);

  m_folderMenu->insertItem(SmallIconSet("filenew"), i18n("&Create New"), createNewMenu);
  m_folderMenu->insertSeparator();
  m_folderMenu->insertItem(SmallIcon("up"), i18n("&Upload Folder..."), this, SLOT(slotUploadSingleURL()));
  m_folderMenu->insertItem(SmallIcon("up"), i18n("&Quick Folder Upload"), this, SLOT(slotQuickUploadURL()));
  m_folderMenu->insertItem(i18n("Create Site &Template..."), this, SLOT(slotCreateSiteTemplate()));
  m_folderMenu->insertItem(i18n("Re&name"), this, SLOT(slotStartRename()));
  m_folderMenu->insertItem(i18n("&Remove From Project"), this, SLOT(slotRemoveFromProject(int)));
  m_folderMenu->insertItem(SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_folderMenu->insertSeparator();
  m_setDocumentRootId = m_folderMenu->insertItem(i18n("Document-&Base Folder"), this, SLOT(slotChangeDocumentFolderStatus()));
  m_folderMenu->insertItem(i18n("Upload &Status"), m_uploadStatusMenu);
  m_folderMenu->insertItem(SmallIcon("info"), i18n("&Properties"), this, SLOT(slotProperties()));

  m_projectMenu = new KPopupMenu(this);
  m_projectMenu->insertItem(SmallIconSet("filenew"), i18n("&Create New"), createNewMenu);
  m_projectMenu->insertSeparator();
  m_projectMenu->insertItem(SmallIcon("up"), i18n("&Upload Project..."), this, SLOT(slotUploadProject()));
  m_projectMenu->insertItem(SmallIcon("reload"), i18n("Re&scan Project Folder..."), this, SLOT(slotRescan()));
  m_projectMenu->insertItem(SmallIcon("configure"), i18n("Project &Properties"), this, SLOT(slotOptions()));
  m_projectMenu->insertItem(SmallIcon("revert"), i18n("&Reload"), this, SLOT(slotReload()));
  m_projectMenu->insertSeparator();
  m_projectMenu->insertItem(i18n("Upload &Status"), m_uploadStatusMenu);


  connect(this, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
          this, SLOT(slotMenu(KListView*, QListViewItem*, const QPoint&)));

  connect(this, SIGNAL(open(QListViewItem *)),
          this, SLOT(slotSelectFile(QListViewItem *)));

  restoreLayout(kapp->config(), className());
  // the restored size of the first column might be too large for the current content
  // we set it to 10 and the listview will adjust it to the size of the largest entry
  setColumnWidth(0, 10);
}

ProjectTreeView::~ProjectTreeView(){
}


KFileTreeBranch* ProjectTreeView::newBranch(const KURL& url)
{
  QString m_projectNameStr = m_projectName+" ";
  if (m_projectName)
  {
    if (url.protocol() == "file")
    {
      m_projectNameStr += i18n("[local disk]");
    } else
    {
      m_projectNameStr += "[" + url.protocol() + "://" + url.user() + "@" + url.host() + "]";
    }
    m_projectDir =  new ProjectTreeBranch( this, url, m_projectNameStr, SmallIcon("ptab"), true);
    m_projectDir->root()->setText(1, url.prettyURL());
    setDragEnabled(true);
    setRootIsDecorated(true);
  } else {
    m_projectDir =  new ProjectTreeBranch( this, url, i18n("No Project"), SmallIcon("ptab"), true);
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

  return m_projectDir;
}


/** slot for context menu */
void ProjectTreeView::slotMenu(KListView *listView, QListViewItem *item, const QPoint& point)
{
  Q_UNUSED(listView);
  KURL url = currentURL();
#ifdef ENABLE_CVSSERVICE
  CVSService::ref()->setRepository(m_projectBaseURL.path());
  if (url.isLocalFile())
  {
    QString path = url.path();
    if (path == m_projectBaseURL.path())
      path += "/.";
    CVSService::ref()->setCurrentFile(path);
  }
  else
    CVSService::ref()->setCurrentFile("");
#endif
  ProjectTreeViewItem *curItem = static_cast<ProjectTreeViewItem *>(currentKFileTreeViewItem());
  if (item && curItem)
  {
    setSelected(item, true);
    if (curItem == curItem->branch()->root())
    {
      m_projectMenu->popup(point);
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
        insertOpenWithMenu(m_fileMenu, 1);
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
  } else
  if (!m_projectName.isNull())
  {
    setSelected(firstChild(), true);
    m_projectMenu->popup(point);
  }
}


/** Sets the project template directory */
void ProjectTreeView::slotNewProjectLoaded(const QString &name, const KURL &baseURL, const KURL &templateURL)
{
  BaseTreeView::slotNewProjectLoaded(name, baseURL, templateURL); // set m_projectName and m_projectBaseURL
  if (!m_projectName.isNull())
  {
    QString projectNameStr = m_projectName+" ";
    if (m_projectBaseURL.protocol() == "file")
    {
      projectNameStr += i18n("[local disk]");
    } else
    {
      projectNameStr += "["+m_projectBaseURL.protocol()+"://"+m_projectBaseURL.user()+"@"+m_projectBaseURL.host()+"]";
    }
    m_projectDir->root()->setText(0, projectNameStr);
    m_projectDir->root()->setText(1, m_projectBaseURL.prettyURL());
    setDragEnabled(true);
  }
  else
  {
    m_projectDir->root()->setText(0, i18n("No Project"));
    m_projectDir->root()->setText(1, "");
    setDragEnabled(false);
  }
  m_documentFolderList.clear();
  m_documentFolderList.append(m_projectBaseURL);
}

void ProjectTreeView::slotReloadTree( ProjectList *fileList, bool buildNewTree, const QStringList &folderToOpen)
{
  m_projectFiles = fileList;
  m_documentFolderList.clear();
  m_documentFolderList.append(m_projectBaseURL);

  KURL url;
  ProjectList::Iterator it( *m_projectFiles );
  for ( ; it.current(); ++it) {
    if (it.current()->documentFolder) {
      url = *(it.current());
      url.adjustPath(-1);
      m_documentFolderList.append( url );
    }
  }

  if (buildNewTree)
  {
    if (m_projectDir)
    {
      removeBranch(m_projectDir);
      m_projectDir = 0L;
    }
    BaseTreeBranch *btb = dynamic_cast<BaseTreeBranch *>(newBranch(m_projectBaseURL));
    if (btb && folderToOpen.count() > 0) {
      btb->folderToOpen = folderToOpen;
      btb->reopenFolder();
      btb->updateOpenFolder();
    }
  } else
  {
    if (m_projectDir){
      m_projectDir->urlList = m_projectFiles;  // set list for filter
      KFileTreeViewItem *item;
      KFileTreeViewItem *rootItem = m_projectDir->root();
      KURL url;
      QListViewItemIterator iter(this);
      for ( ; iter.current(); ++iter )
      {
        item = dynamic_cast <KFileTreeViewItem*> (iter.current());
        item->setVisible(m_projectFiles->contains(item->url()) || item == rootItem);
      }
      rootItem->setEnabled(true);
    }
  }
}

void ProjectTreeView::slotOpen()
{
  BaseTreeView::slotSelectFile(currentItem());
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
     if (QExtFileInfo::createDir(url, this))
     {
        emit insertToProject(url);
     }
  }
}

void ProjectTreeView::slotCreateFile()
{
  bool ok;
  QString fileName = KInputDialog::getText(i18n("Create New File"), i18n("File name:"), "", &ok, this);
  if (ok)
  {
    KURL url = currentURL();
    if (currentKFileTreeViewItem()->isDir())
      url.setPath(url.path() + "/" + fileName);
    else
      url.setPath(url.directory() + "/" + fileName);
    if (QExtFileInfo::exists(url, false, this))
    {
      KMessageBox::error(this, i18n("<qt>Cannot create file, because a file named <b>%1</b> already exists.</qt>").arg(fileName), i18n("Error Creating File"));
      return;
    }
    KTempFile *tempFile = new KTempFile(tmpDir);
    tempFile->setAutoDelete(true);
    tempFile->close();
    if (QExtFileInfo::copy(KURL::fromPathOrURL(tempFile->name()), url))
    {
      emit insertToProject(url);
      emit openFile(url);
    }
    delete tempFile;
  }
}


void ProjectTreeView::slotRemoveFromProject(int askForRemove)
{
  QListViewItem *item = currentItem();
  if (item)
  {
    KURL url = currentURL();
    QString nice = QExtFileInfo::toRelative(url, m_projectBaseURL).path();
    nice = KStringHandler::lsqueeze(nice, 60);
    if ( !askForRemove ||
         KMessageBox::warningContinueCancel(this,i18n("<qt>Do you really want to remove <br><b>%1</b><br> from the project?</qt>").arg(nice), i18n("Remove From Project"), KStdGuiItem::remove(), "RemoveFromProject") == KMessageBox::Continue )
    {
      if ( currentKFileTreeViewItem()->isDir() ) url.adjustPath(+1);
      emit removeFromProject(url);
    }
  }
}


void ProjectTreeView::slotUploadSingleURL()
{
  if (currentItem())
  {
    KURL url = currentURL();
    if ( currentKFileTreeViewItem()->isDir() ) url.adjustPath(+1);
    emit uploadSingleURL(url, "", false, false);
  }
}

void ProjectTreeView::slotQuickUploadURL()
{
  if (currentItem())
  {
    KURL url = currentURL();
    if ( currentKFileTreeViewItem()->isDir() ) url.adjustPath(+1);
    emit uploadSingleURL(url, "", true, false);
  }
}

void ProjectTreeView::slotRescan()
{
  emit rescanProjectDir();
  if (!m_projectBaseURL.isLocalFile())
    slotReload();
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
  BaseTreeView::slotPopulateFinished(item);
  // populate descriptions
  QString desc;
  KFileTreeViewItem* file_item;
  ProjectList::Iterator it( *m_projectFiles );
  for ( ; it.current(); ++it)
  {
    desc = it.current()->fileDesc;
    if (! desc.isEmpty())
    {
      file_item = m_projectDir->findTVIByURL( *(it.current()) );
      if (file_item)
        file_item->setText(1, desc);
    }
  }

   if ( m_projectFiles->isEmpty() )
     m_projectDir->root()->setExpandable( false );
   /* FIXME:
    * The below code can cause infinite recursion, possibly over slow links.
    * setOpen call KFileTreeBranch::openURL, that calls KDirListerCache::listDir,
    * that calls KDirListerCache::stop, that emit the signal cancelled, that
    * is connected to KFileTreeBranch::slotCanceled, that emit populateFinished
    * and we are back in this same function with item being the same and
    * everything starts over again. */
//   else
//     m_projectDir->setOpen( true );

}


void ProjectTreeView::itemDescChanged(KFileTreeViewItem* item, const QString& newDesc)
{
  if (item) {
    item->setText(1, newDesc);
    KURL url = item->url();
    if (item->isDir())
      url.adjustPath(1);
    emit changeFileDescription(url, newDesc);
  }
}

void ProjectTreeView::slotAlwaysUpload()
{
    KFileTreeViewItem *kftvi = currentKFileTreeViewItem();
    if (! kftvi)
      return;
    KURL url = currentURL();
    if (kftvi->isDir())
      url.adjustPath(+1);
    if (m_projectFiles->contains(url) || url == m_projectBaseURL)
    {
       emit changeUploadStatus(url, ProjectURL::AlwaysUpload);
    }
}

void ProjectTreeView::slotNeverUpload()
{
    KFileTreeViewItem *kftvi = currentKFileTreeViewItem();
    if (! kftvi)
      return;
    KURL url = currentURL();
    if (kftvi->isDir())
      url.adjustPath(+1);
    if (m_projectFiles->contains(url) || url == m_projectBaseURL)
    {
       emit changeUploadStatus(url, ProjectURL::NeverUpload);
    }
}

void ProjectTreeView::slotConfirmUpload()
{
    KFileTreeViewItem *kftvi = currentKFileTreeViewItem();
    if (! kftvi)
      return;
    KURL url = currentURL();
    if (kftvi->isDir())
      url.adjustPath(+1);
    if (m_projectFiles->contains(url) || url == m_projectBaseURL)
    {
       emit changeUploadStatus(url, ProjectURL::ConfirmUpload);
    }
}

void ProjectTreeView::slotUploadMenuAboutToShow()
{
    m_uploadStatusMenu->setItemChecked(m_alwaysUploadId, false);
    m_uploadStatusMenu->setItemChecked(m_neverUploadId, false);
    m_uploadStatusMenu->setItemChecked(m_confirmUploadId, false);
    KFileTreeViewItem *kftvi = currentKFileTreeViewItem();
    if (! kftvi)
      return;
    KURL url = currentURL();
    if (kftvi->isDir())
      url.adjustPath(+1);
    ProjectURL *proUrl = m_projectFiles->find( url );
    if (proUrl)
    {
        switch (proUrl->uploadStatus)
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
    KFileTreeViewItem *kftvi = currentKFileTreeViewItem();
    if (! kftvi)
      return;
    KURL url = currentURL();
    if (kftvi->isDir())
      url.adjustPath(+1);
    if (!m_documentFolderList.contains(currentURL()))
    {
      m_documentFolderList.append(currentURL());
      emit changeUploadStatus(url, ProjectURL::AlwaysUpload);
      emit changeDocumentFolderStatus(url, true);
    } else
    {
      m_documentFolderList.remove(currentURL());
      emit changeUploadStatus(url, false);
    }
    currentItem()->repaint();
}

bool ProjectTreeView::isDocumentFolder(const KURL &url)
{
   return (m_documentFolderList.contains(url) > 0);
}


void ProjectTreeView::slotRenameItem(QListViewItem* kftvi, const QString& newText, int col)
{
  if (!kftvi)
    return;

  if (col == 1) {
    itemDescChanged(dynamic_cast <KFileTreeViewItem*> (kftvi), newText);
  } else {
    BaseTreeView::slotRenameItem(kftvi, newText, col);
  }
}


FileInfoDlg* ProjectTreeView::addFileInfoPage(KPropertiesDialog* propDlg)
{
  FileInfoDlg* dlg = BaseTreeView::addFileInfoPage(propDlg);
  if (dlg) {
    dlg->fileDescLbl->show();
    dlg->fileDesc->show();
  }
  return dlg;
}


void ProjectTreeView::plugCVSMenu()
{
#ifdef ENABLE_CVSSERVICE
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(SmallIcon("cervisia"), i18n("C&VS"), CVSService::ref()->menu());
  m_folderMenu->insertSeparator();
  m_folderMenu->insertItem(SmallIcon("cervisia"), i18n("C&VS"), CVSService::ref()->menu());
  m_projectMenu->insertSeparator();
  m_projectMenu->insertItem(SmallIcon("cervisia"), i18n("C&VS"), CVSService::ref()->menu());
#endif

}


void ProjectTreeView::slotViewActivated(const KURL& url)
{
  if (! m_projectDir) return;

  KFileTreeViewItem* item = m_projectDir->findTVIByURL(url);
  if (item){
    ensureItemVisible(item);
    setSelected(item, true);
  }
}

#include "projecttreeview.moc"
