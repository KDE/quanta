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

// Qt includes
#include <qpainter.h>

// KDE includes
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kopenwith.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kfiledialog.h>
#include <kprogress.h>
#include <kurl.h>

// app includes
#include "servertreeview.h"
#include "resource.h"
#include "project.h"
#include "qextfileinfo.h"


ServerTreeViewItem::ServerTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch )
: BaseTreeViewItem( parent, item, brnch)
{
}

void ServerTreeViewItem::paintCell(QPainter *p, const QColorGroup &cg,
                                   int column, int width, int align)
{
  QColorGroup _cg( cg );
  KURL baseURL = Project::ref()->projectBaseURL();
  KURL relURL = baseURL;
  relURL.setPath(QExtFileInfo::toRelative(url(), branch()->url()).path());
  KURL url = QExtFileInfo::toAbsolute(relURL, baseURL);
  if ( !Project::ref()->contains(url) )
  {
    QFont f = p->font();
    f.setItalic(true);
    p->setFont(f);
    int h, s, v;
    p->pen().color().getHsv(&h, &s, &v);
    v = (v < 155 ? v + 100 : 255);
    _cg.setColor(QColorGroup::Text, QColor(h, s, v, QColor::Hsv));
  };
  BaseTreeViewItem::paintCell( p, _cg, column, width, align );
}

//ServerTreeBranch implementation
ServerTreeBranch::ServerTreeBranch(KFileTreeView *parent, const KURL& url,
                                   const QString& name, const QPixmap& pix,
                                   bool showHidden, KFileTreeViewItem *branchRoot)
    : BaseTreeBranch(parent, url, name, pix, showHidden, branchRoot)
{
}

KFileTreeViewItem* ServerTreeBranch::createTreeViewItem(KFileTreeViewItem *parent, KFileItem *fileItem )
{
  BaseTreeViewItem  *tvi = 0;
  if( parent && fileItem )
  {
    tvi = new ServerTreeViewItem( parent, fileItem, this );
  }
  else
    kdDebug(24000) << "ServerTreeBranch::createTreeViewItem: Have no parent" << endl;
  return tvi;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// ServerTreeView implementation
//
//
ServerTreeView::ServerTreeView(KConfig *config, QWidget *parent, const KURL &url, const char *name)
  : BaseTreeView(parent, name)
{
  setAcceptDrops(true);
  setDragEnabled(true);

  m_config = config;
  m_config->setGroup("General Options");
  // I must read this here because quanta_init has not done it yet
  qConfig.showHiddenFiles = m_config->readBoolEntry("Show Hidden Files", true);

  m_fileMenu = new KPopupMenu();

  m_fileMenu->insertItem(SmallIcon("fileopen"), i18n("&Open"), this ,SLOT(slotOpen()));
  m_fileMenu->insertItem(i18n("Open &With..."), this, SLOT(slotOpenWith()));
  m_menuClose = m_fileMenu->insertItem(SmallIcon("fileclose"), i18n("Clos&e"), this, SLOT(slotClose()));
  m_fileMenu->insertSeparator();
//  m_insertFileInProject = m_fileMenu->insertItem(i18n("&Insert in Project..."), this, SLOT(slotInsertInProject()));
  m_fileMenu->insertItem(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));
  m_fileMenu->insertItem(SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_fileMenu->insertItem(i18n("Re&name"), this, SLOT(slotStartRename()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(SmallIcon("info"), i18n("&Properties"), this, SLOT(slotProperties()));

  m_folderMenu = new KPopupMenu();

//  m_insertFolderInProject = m_folderMenu->insertItem(i18n("&Insert in Project..."), this, SLOT(slotInsertDirInProject()));
  m_folderMenu->insertItem(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));
  m_menuPasteFolder = m_folderMenu->insertItem(SmallIcon("editpaste"), i18n("&Paste"), this, SLOT(slotPaste()));
  m_menuDel = m_folderMenu->insertItem( SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_renameId = m_folderMenu->insertItem(i18n("Re&name"), this, SLOT(slotStartRename()));
  m_folderMenu->insertSeparator();
  m_folderMenu->insertItem(SmallIcon("info"), i18n("&Properties"), this, SLOT(slotProperties()));
  m_reloadMenuId = m_folderMenu->insertItem(SmallIcon("revert"), i18n("&Reload"), this, SLOT(slotReload()));

  addColumn(i18n("Upload Tree"), -1);
  addColumn("");

  connect(this, SIGNAL(open(QListViewItem *)),
          this, SLOT(slotSelectFile(QListViewItem *)));
  connect(this, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
          this, SLOT(slotMenu(KListView*, QListViewItem*, const QPoint&)));
  connect(Project::ref(), SIGNAL(reloadTree(ProjectList *, bool, const QStringList &)),
          this, SLOT(slotReloadTree(ProjectList *, bool, const QStringList &)));

  restoreLayout(m_config, "UploadTreeView");  // a fixed name only for the tooltip configuration
  // the restored size of the first column might be too large for the current content
  // we set it to 10 and the listview will adjust it to the size of the largest entry
  setColumnWidth(0, 10);
  newBranch(url);
}

ServerTreeView::~ServerTreeView()
{
}

KFileTreeBranch* ServerTreeView::newBranch(const KURL& url)
{
  KURL branchURL = url;
  branchURL.adjustPath(+1);
  BaseTreeBranch* newBrnch = 0;
  newBrnch = new ServerTreeBranch(this, branchURL, branchURL.prettyURL(), SmallIcon("up"), true);
  newBrnch->setShowingDotFiles(qConfig.showHiddenFiles);
  addBranch(newBrnch);
  return newBrnch;
}


/** RMB pressed, bring up the menu */
void ServerTreeView::slotMenu(KListView* listView, QListViewItem *item, const QPoint &point)
{
  Q_UNUSED(listView);
  if (item)
  {
    setSelected(item, true);
    KFileTreeViewItem *curItem = currentKFileTreeViewItem();
    if ( !curItem->isDir() )
    {
      m_fileMenu->setItemVisible(m_menuClose, isFileOpen(currentURL()));
      m_fileMenu->popup(point);
    } else {
      m_folderMenu->setItemVisible(m_menuPasteFolder, isPathInClipboard());
      bool root = (curItem == curItem->branch()->root());
      m_folderMenu->setItemVisible(m_menuDel, ! root);
      m_folderMenu->setItemVisible(m_reloadMenuId, root);
      m_folderMenu->setItemVisible(m_renameId, ! root);
      m_folderMenu->popup(point);
   }
 }
}


void ServerTreeView::slotReloadTree( ProjectList *fileList, bool buildNewTree, const QStringList &folderToOpen)
{
  Q_UNUSED(fileList);
  Q_UNUSED(buildNewTree);
  Q_UNUSED(folderToOpen);
  
  QListViewItemIterator iter(this);
  for ( ; iter.current(); ++iter )
  {
    iter.current()->repaint();
  }
}

#include "servertreeview.moc"
