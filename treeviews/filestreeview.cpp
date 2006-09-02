/***************************************************************************
                          filestreeview.cpp  -  description
                             -------------------
    begin                : Thu Jun 1 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2004 Andras Mantia <amantoa@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


// KDE includes
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kinputdialog.h>
#include <kopenwith.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kfiledialog.h>
#include <kprogress.h>
#include <kurl.h>

// app includes
#include "filestreeview.h"
#include "resource.h"
#include "project.h"
#ifdef ENABLE_CVSSERVICE
#include "cvsservice.h"
#endif
#include "qextfileinfo.h"


//FilesTreeView implementation

FilesTreeView::FilesTreeView(KConfig *config, QWidget *parent, const char *name)
  : BaseTreeView(parent, name)
{
  setAcceptDrops(true);
  setDragEnabled(true);
  setSaveOpenFolder(true);

  m_config = config;
  m_config->setGroup("General Options");
  // I must read this here because quanta_init has not done it yet
  qConfig.showHiddenFiles = m_config->readBoolEntry("Show Hidden Files", true);
  qConfig.saveTrees = m_config->readBoolEntry("Save Local Trees", true);
  QStringList topStrList = QuantaCommon::readPathListEntry(m_config, "Top folders");
  QStringList topStrAliasList = QuantaCommon::readPathListEntry(m_config, "Top folder aliases");
  KURL url;
  for (uint i = 0; i < topStrList.count(); i++)
  {
    url = KURL();
    QuantaCommon::setUrl(url, topStrList[i]);
    if (!topURLList.contains(url))
    {
      topURLList.append(url);
      if (i < topStrAliasList.count())
        topURLAliases.insert(url.url(), topStrAliasList[i]);
      else
        topURLAliases.insert(url.url(), url.fileName());
    }
  }

  m_config->setGroup("General Options");
  if (m_config->readBoolEntry("Home-Root Folder On", true))
  {
    url = KURL();
    url.setPath("/");
    if (!topURLList.contains(url))
        topURLList.append(url);
    url = KURL();
    url.setPath(QExtFileInfo::homeDirPath() + "/");
    if (!topURLList.contains(url))
        topURLList.append(url);
  }

  m_fileMenu = new KPopupMenu(this);

  m_fileMenu->insertItem(SmallIcon("fileopen"), i18n("&Open"), this ,SLOT(slotOpen()));
  m_fileMenu->insertItem(i18n("Insert &Tag"), this, SLOT(slotInsertTag()));
  m_menuClose = m_fileMenu->insertItem(SmallIcon("fileclose"), i18n("Clos&e"), this, SLOT(slotClose()));
  m_fileMenu->insertSeparator();
  m_insertFileInProject = m_fileMenu->insertItem(i18n("&Insert in Project..."), this, SLOT(slotInsertInProject()));
  m_fileMenu->insertItem(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));
  m_fileMenu->insertItem(i18n("Re&name"), this, SLOT(slotStartRename()));
  m_fileMenu->insertItem(SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(SmallIcon("info"), i18n("&Properties"), this, SLOT(slotProperties()));

  KPopupMenu *createNewMenu = new KPopupMenu(this);
  createNewMenu->insertItem(SmallIcon("folder_new"), i18n("F&older..."), this, SLOT(slotCreateFolder()));
  createNewMenu->insertItem(SmallIcon("document"), i18n("&File..."), this, SLOT(slotCreateFile()));

  m_folderMenu = new KPopupMenu();

  m_folderMenu->insertItem(SmallIcon("folder_new"), i18n("New Top &Folder..."), this, SLOT(slotNewTopFolder()));
  m_menuTop = m_folderMenu->insertItem(i18n("&Add Folder to Top"), this, SLOT(slotAddToTop()));
  m_folderMenu->insertItem(i18n("Create Site &Template..."), this, SLOT(slotCreateSiteTemplate()));
  m_folderMenu->insertItem(SmallIcon("empty"), i18n("&Create New"), createNewMenu);
  m_folderMenu->insertSeparator();
  m_insertFolderInProject = m_folderMenu->insertItem(i18n("&Insert in Project..."), this, SLOT(slotInsertDirInProject()));
  m_folderMenu->insertItem(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));
  m_menuPasteFolder = m_folderMenu->insertItem(SmallIcon("editpaste"), i18n("&Paste"), this, SLOT(slotPaste()));
  m_menuChangeAlias = m_folderMenu->insertItem(i18n("&Change Alias..."), this, SLOT(slotChangeAlias()));
  m_menuFolderRename = m_folderMenu->insertItem(i18n("Re&name"), this, SLOT(slotStartRename()));
  m_menuDel = m_folderMenu->insertItem( SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_folderMenu->insertSeparator();
  m_folderMenu->insertItem(SmallIcon("info"), i18n("&Properties"), this, SLOT(slotProperties()));
  m_reloadMenuId = m_folderMenu->insertItem(SmallIcon("revert"), i18n("&Reload"), this, SLOT(slotReload()));

  m_emptyMenu = new KPopupMenu();

  m_emptyMenu->insertItem(i18n("New Top &Folder..."), this, SLOT(slotNewTopFolder()), 0, -1 , 0);

  addColumn(i18n("Files Tree"), -1);
  addColumn("");

  connect(this, SIGNAL(open(QListViewItem *)),
          this, SLOT(slotSelectFile(QListViewItem *)));
  connect(this, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
          this, SLOT(slotMenu(KListView*, QListViewItem*, const QPoint&)));

  // generate top list of directories
  for (uint i = 0; i <  topURLList.count(); i++)
  {
    KURL url = topURLList[i];
    newBranch(url);
  }
  restoreLayout(m_config, className());
  // the restored size of the first column might be too large for the current content
  // we set it to 10 and the listview will adjust it to the size of the largest entry
  setColumnWidth(0, 10);
}

FilesTreeView::~FilesTreeView()
{
}

KFileTreeBranch* FilesTreeView::newBranch(const KURL& url)
{
  BaseTreeBranch* newBrnch = 0;
  KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, url);
  if (url.isLocalFile() && url.path() == "/")
  {
    newBrnch = new BaseTreeBranch(this, url, i18n("Root Folder"), SmallIcon(fileItem.iconName()), true);
  } else
  {
    if (url.isLocalFile() && url.equals(KURL(QExtFileInfo::homeDirPath() + "/"), true))
    {
      newBrnch = new BaseTreeBranch(this, url, i18n("Home Folder"), SmallIcon(fileItem.iconName()), true);
    } else
    {
      QString s = url.fileName();
      if (s.isEmpty())
        s = "/";
      if (topURLAliases.contains(url.url()))
        s = topURLAliases[url.url()];
      s += " [" + url.prettyURL() + "]";
      newBrnch = new BaseTreeBranch(this, url, s, SmallIcon(fileItem.iconName()), true);
    }
  }
  newBrnch->setShowingDotFiles(qConfig.showHiddenFiles);
  addBranch(newBrnch);
  return newBrnch;
}



/** RMB pressed, bring up the menu */
void FilesTreeView::slotMenu(KListView* listView, QListViewItem *item, const QPoint &point)
{
  Q_UNUSED(listView);
#ifdef ENABLE_CVSSERVICE
  KURL url = currentURL();
  if (Project::ref()->contains(url))
    CVSService::ref()->setRepository(Project::ref()->projectBaseURL().path());
  else
    CVSService::ref()->setRepository(url.directory());
  if (url.isLocalFile())
  {
    CVSService::ref()->setCurrentFile(url.path());
  }
  else
    CVSService::ref()->setCurrentFile("");
#endif
  if (item)
  {
    bool hasProject = !m_projectName.isNull();
    m_folderMenu->setItemVisible(m_menuChangeAlias, false);
    m_folderMenu->setItemVisible(m_menuFolderRename, true);
    m_folderMenu->setItemVisible(m_insertFolderInProject, hasProject);
    m_fileMenu->setItemVisible(m_insertFileInProject, hasProject);
    setSelected(item, true);
    KFileTreeViewItem *curItem = currentKFileTreeViewItem();
    if ( !curItem->isDir() )
    {
      m_fileMenu->setItemVisible(m_menuClose, isFileOpen(currentURL()));
      insertOpenWithMenu(m_fileMenu, 1);
      m_fileMenu->popup( point);
    } else {
      m_folderMenu->setItemVisible( m_menuDel, true );
      m_folderMenu->setItemVisible( m_menuTop, true );
      m_folderMenu->setItemVisible(m_menuPasteFolder, isPathInClipboard());
      KURL url = curItem->url();
      if ( curItem == curItem->branch()->root() )
      {
        m_folderMenu->setItemVisible(m_menuDel, false);
        m_folderMenu->changeItem(m_menuTop, i18n("Remove From &Top"));
        m_folderMenu->setItemVisible(m_menuChangeAlias, true);
        m_folderMenu->setItemVisible(m_menuFolderRename, false);

        m_config->setGroup("General Options");
        if ((url == KURL("file:/") || url == KURL("file:" + QExtFileInfo::homeDirPath() + "/")) &&
            m_config->readBoolEntry("Home-Root Folder On", true) )
          m_folderMenu ->setItemVisible(m_menuTop, false);
        m_folderMenu ->setItemVisible(m_reloadMenuId, true);
      }
      else
      {
        m_folderMenu->changeItem( m_menuTop, i18n("&Add Folder to Top"));
        url.adjustPath(+1);
        m_folderMenu->setItemVisible(m_menuTop, (topURLList.findIndex(url) == -1));
        m_folderMenu->setItemVisible(m_reloadMenuId, false);
      }
      m_folderMenu->popup( point);
   }
 } else {
   m_emptyMenu->popup( point );
 }
}



/** Add or remove folders to/from the top list */
void FilesTreeView::slotAddToTop()
{
  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if ( !curItem ) return;

  if ( curItem->isDir() )
  {
    KURL url(currentURL().url());
    url.adjustPath(+1);
    if ( curItem != curItem->branch()->root() )  //it is not a top folder
    { // add
      if (topURLList.findIndex(url) == -1)
      {
        topURLList.append(url);
        bool ok;
        QString aliasName = KInputDialog::getText(i18n("Set Alias"), i18n("Alternative folder name:"), url.fileName(), &ok, this);
        if (!ok)
          aliasName = url.fileName();
        topURLAliases.insert(url.url(), aliasName);
        newBranch(url);
      } else {
        KMessageBox::information(this, i18n("<qt><b>%1</b> is already a toplevel entry.</qt>").arg(url.url()));
      }
    } else
    { // remove
      topURLList.remove(url);
      topURLAliases.remove(url.url());
      removeBranch(curItem->branch());
    }
  }
}

void FilesTreeView::slotNewTopFolder()
{
  KURL url = KFileDialog::getExistingURL(QString::null, this, i18n("Choose Local or Remote Folder"));
  if (url.isEmpty()) return;
  url.adjustPath(+1);
  if (topURLList.findIndex(url) == -1)
  {
    bool ok;
    QString aliasName = KInputDialog::getText(i18n("Set Alias"), i18n("Alternative folder name:"), url.fileName(), &ok, this);
    if (!ok)
      aliasName = url.fileName();
    topURLAliases.insert(url.url(), aliasName);
    newBranch(url);
    topURLList.append(url);
  } else {
    KMessageBox::information(this, i18n("<qt><b>%1</b> is already a toplevel entry.</qt>").arg(url.url()));
  }
}


void FilesTreeView::plugCVSMenu()
{
#ifdef ENABLE_CVSSERVICE
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(SmallIcon("cervisia"), i18n("C&VS"), CVSService::ref()->menu());
  m_folderMenu->insertSeparator();
  m_folderMenu->insertItem(SmallIcon("cervisia"), i18n("C&VS"), CVSService::ref()->menu());
#endif
}

void FilesTreeView::slotChangeAlias()
{
  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if ( !curItem ) return;

  if ( curItem->isDir() )
  {
    KURL url(currentURL().url());
    url.adjustPath(+1);
    if ( curItem == curItem->branch()->root() )  //it is not a top folder
    {
      if (topURLList.findIndex(url) != -1)
      {
        bool ok;
        QString aliasName = KInputDialog::getText(i18n("Change Alias"), i18n("Alternative folder name:"), topURLAliases[url.url()], &ok, this);
        if (ok)
        {
          topURLAliases.replace(url.url(), aliasName);
          removeBranch(curItem->branch());
          newBranch(url);
        }
      } else {
        KMessageBox::information(this, i18n("<qt><b>%1</b> is already a toplevel entry.</qt>").arg(url.url()));
      }
    }
  }
}


#include "filestreeview.moc"
