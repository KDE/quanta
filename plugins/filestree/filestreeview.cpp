/***************************************************************************
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

// app includes
#include "filestreeview.h"
#include "quantafilestreepart.h"
#include "settings.h"

//kdevelop includes
#include <kdevcore.h>
#include <kdevconfig.h>
#include <kdevcontext.h>
#include <kdevmainwindow.h>

// KDE includes
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kinputdialog.h>
#include <kopenwith.h>
#include <kmessagebox.h>
#include <kmenu.h>
#include <kfiledialog.h>
#include <kurl.h>
#include <kapplication.h>


//FilesTreeView implementation

FilesTreeView::FilesTreeView(QuantaFilesTreePart *part, QWidget *parent)
  : BaseTreeView(part, parent), m_part(part)
{
  setAcceptDrops(true);
  setDragEnabled(true);
  setShowToolTips(Settings::self()->filesTreeTooltips());
  setSaveOpenFolder(Settings::self()->filesTreeSave());

  m_config = KDevConfig::standard();

  addColumn(i18n("Files Tree"), -1);
  addColumn("");

  connect(this, SIGNAL(open(Q3ListViewItem *)),
          this, SLOT(slotSelectFile(Q3ListViewItem *)));
  connect(this, SIGNAL(contextMenu(K3ListView*, Q3ListViewItem*, const QPoint&)),
          this, SLOT(slotMenu(K3ListView*, Q3ListViewItem*, const QPoint&)));

  restoreBranches();
  restoreLayout(m_config, metaObject()->className());
  // the restored size of the first column might be too large for the current content
  // we set it to 10 and the listview will adjust it to the size of the largest entry
  setColumnWidth(0, 10);
}

FilesTreeView::~FilesTreeView()
{
  m_config->setGroup(metaObject()->className());
  m_config->writePathEntry("Top folders", topURLAliases.keys());
  m_config->writePathEntry("Top folder aliases", topURLAliases.values());
  saveLayout(m_config, metaObject()->className());
}

KFileTreeBranch* FilesTreeView::newBranch(const KUrl& url)
{
  BaseTreeBranch* newBrnch = 0;
  QString s = topURLAliases[url.url()];
  if (url.isLocalFile() && url.path() == "/")
  {
    if (s.isEmpty())
      s = i18n("Root Folder");

    newBrnch = new BaseTreeBranch(this, url, s, SmallIcon(iconNameForURL(url)), Settings::self()->filesTreeShowHidden());
  } else
  {
    if (url.isLocalFile() && url.equals(KUrl(QDir::homePath()), KUrl::CompareWithoutTrailingSlash))
    {
      if (s.isEmpty())
        s = i18n("Home Folder");

      newBrnch = new BaseTreeBranch(this, url, s, SmallIcon(iconNameForURL(url)), Settings::self()->filesTreeShowHidden());
      newBrnch->root()->setText(1, url.path());
    } else
    {
      if (s.isEmpty())
        s = url.fileName().isEmpty() ? url.url() : url.fileName();

      newBrnch = new BaseTreeBranch(this, url, s, SmallIcon(iconNameForURL(url)), Settings::self()->filesTreeShowHidden());
      if (url.isLocalFile())
        newBrnch->root()->setText(1, url.path());
      else
        newBrnch->root()->setText(1, url.prettyUrl());
    }
  }
  addBranch(newBrnch);
  return newBrnch;
}


void FilesTreeView::emptyMenu(const QPoint &point)
{
  KMenu popup(this);
  popup.addTitle(i18n("Files Tree"));
  popup.addAction(i18n("New Top &Folder..."), this, SLOT(slotNewTopFolder()));
  popup.exec(point);
}


void FilesTreeView::folderMenu(const QPoint &point)
{
  KMenu popup(this);
  popup.addTitle(i18n("Files Tree"));

  popup.addAction(SmallIcon("folder_new"), i18n("New Top &Folder..."), this, SLOT(slotNewTopFolder()));

  KUrl url = currentKFileTreeViewItem()->url();
  bool rootItem = (currentKFileTreeViewItem() == currentKFileTreeViewItem()->branch()->root());
  if (!rootItem)
  {
    url.adjustPath(KUrl::AddTrailingSlash);
    if (!topURLAliases.contains(url.url()))
      popup.addAction(i18n("&Add Folder to Top"), this, SLOT(slotAddToTop()));
  }
  else
    if (!Settings::self()->filesTreeRootHome() || !blockedURL(url))
      popup.addAction(i18n("Remove From &Top"), this, SLOT(slotRemoveFromTop()));

  // submenu for creating files or folders
  KMenu createNewMenu(this);
  createNewMenu.setTitle(i18n("&Create New"));
  createNewMenu.setIcon(SmallIcon("empty"));
  createNewMenu.addAction(SmallIcon("folder_new"), i18n("F&older..."), this, SLOT(slotCreateFolder()));
  createNewMenu.addAction(SmallIcon("document"), i18n("&File..."), this, SLOT(slotCreateFile()));

  popup.addMenu(&createNewMenu);
  popup.addSeparator();
  popup.addAction(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));

  if (isPathInClipboard())
    popup.addAction(SmallIcon("editpaste"), i18n("&Paste"), this, SLOT(slotPaste()));

  if (rootItem)
    popup.addAction(i18n("&Change Alias..."), this, SLOT(slotChangeAlias()));

  if (!rootItem)
  {
    popup.addAction(i18n("Re&name"), this, SLOT(slotStartRename()));
    popup.addAction( SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  }
  popup.addSeparator();
  popup.addAction(SmallIcon("info"), i18n("&Properties"), this, SLOT(slotProperties()));

  if (rootItem)
    popup.addAction(SmallIcon("revert"), i18n("&Reload"), this, SLOT(slotReload()));

  // ask other plugins for menu entries
  KUrl menuURL(currentKFileTreeViewItem()->url());
  menuURL.adjustPath(KUrl::AddTrailingSlash);
  KUrl::List urlList(menuURL);
  FileContext context(urlList);
  KDevCore::mainWindow()->fillContextMenu(&popup, &context);

  popup.exec(point);
}


void FilesTreeView::fileMenu(const QPoint &point)
{
  KMenu popup(this);
  popup.addTitle(i18n("Files Tree"));

  if (isFileOpen(currentUrl()))
    popup.addAction(SmallIcon("fileclose"), i18n("Clos&e"), this, SLOT(slotClose()));
  else
    popup.addAction(SmallIcon("fileopen"), i18n("&Open"), this ,SLOT(slotOpen()));

  popup.addSeparator();
  popup.addAction(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));
  popup.addAction(i18n("Re&name"), this, SLOT(slotStartRename()));
  popup.addAction(SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  popup.addSeparator();
  popup.addAction(SmallIcon("info"), i18n("&Properties"), this, SLOT(slotProperties()));

  // ask other plugins for menu entries
  KUrl::List urlList(currentKFileTreeViewItem()->url());
  FileContext context(urlList);
  KDevCore::mainWindow()->fillContextMenu(&popup, &context);

  popup.exec(point);
}


/** remove folder from the top list */
void FilesTreeView::slotRemoveFromTop()
{
  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if (!curItem || !curItem->isDir() || curItem != curItem->branch()->root())
    return;

  KUrl url(currentUrl());
  url.adjustPath(KUrl::AddTrailingSlash);
  topURLAliases.remove(url.url());
  removeBranch(curItem->branch());
}


/** Add a folder to the top list */
void FilesTreeView::slotAddToTop()
{
  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if (!curItem || !curItem->isDir() || curItem == curItem->branch()->root())
    return;

  KUrl url(currentUrl());
  url.adjustPath(KUrl::AddTrailingSlash);
  if (!topURLAliases.contains(url.url()))
  {
    newBranch(url);
  } else {
    KMessageBox::information(this, i18n("<qt><b>%1</b> is already a toplevel entry.</qt>", url.url()));
  }
}


void FilesTreeView::slotNewTopFolder()
{
  KUrl url = KFileDialog::getExistingUrl(KUrl(), this, i18n("Choose Local or Remote Folder"));
  if (url.isEmpty())
    return;
  url.adjustPath(KUrl::AddTrailingSlash);
  if (!topURLAliases.contains(url.url()))
  {
    newBranch(url);
  } else {
    KMessageBox::information(this, i18n("<qt><b>%1</b> is already a toplevel entry.</qt>", url.url()));
  }
}


void FilesTreeView::slotChangeAlias()
{
  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  // test if we have an item, it is a folder and it is a root item
  if (!curItem || !curItem->isDir() || curItem != curItem->branch()->root())
    return;

  KUrl url(currentUrl().url());
  url.adjustPath(KUrl::AddTrailingSlash);
  QString s = topURLAliases[url.url()];
  if (s.isEmpty())
    s = curItem->text(0);
  bool ok;
  QString aliasName = KInputDialog::getText(i18n("Change Alias"), i18n("Alternative folder name:"), s, &ok, this);
  if (ok)
  {
    topURLAliases.remove(url.url());
    topURLAliases.insert(url.url(), aliasName);
    curItem->setText(0, aliasName);
  }
}

void FilesTreeView::slotSettingsChanged()
{
  setShowToolTips(Settings::self()->filesTreeTooltips());
  setSaveOpenFolder(Settings::self()->filesTreeSave());

  if (Settings::self()->filesTreeRootHome())
  {
    KUrl url;
    url.setPath("/");
    if (!topURLAliases.contains(url.url()))
    {
      newBranch(url);
    }
    url = KUrl();
    url.setPath(QDir::homePath() + '/');
    if (!topURLAliases.contains(url.url()))
    {
      newBranch(url);
    }
  }
  slotReloadAllTrees();
}


void FilesTreeView::restoreBranches()
{
  m_config->setGroup(metaObject()->className());
  QStringList topStrList = m_config->readPathListEntry("Top folders");
  QStringList topStrAliasList = m_config->readPathListEntry("Top folder aliases");
  for (int i = 0; i < topStrList.count(); i++)
  {
    if (i < topStrAliasList.count())
      topURLAliases.insert(topStrList[i], topStrAliasList[i]);
    else
      topURLAliases.insert(topStrList[i], KUrl(topStrList[i]).fileName());
  }
  if (Settings::self()->filesTreeRootHome())
  {
    KUrl url;
    url.setPath("/");
    if (!topURLAliases.contains(url.url()))
      topURLAliases.insert(url.url(), "");

    url = KUrl();
    url.setPath(QDir::homePath() + '/');
    if (!topURLAliases.contains(url.url()))
      topURLAliases.insert(url.url(), "");
  }
  // generate top list of directories
  BranchMap::ConstIterator it;
  for ( it = topURLAliases.constBegin(); it != topURLAliases.constEnd(); ++it )
  {
    newBranch(KUrl(it.key()));
  }
}


#include "filestreeview.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
