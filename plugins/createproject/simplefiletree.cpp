/***************************************************************************
    begin                : Fri Jul 22 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "simplefiletree.h"
#include "simpletreeviewitem.h"

#include <q3header.h>
#include <QStringList>

#include <kdebug.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmenu.h>

SimpleFileTree::SimpleFileTree(QWidget *parent)
 : K3ListView(parent)
{
  setTreeStepSize(15);
  setRootIsDecorated(true);
  header()->hide();
  setSorting(0, true);
  setAcceptDrops(false);
  setDropVisualizer(true);
  setDragEnabled(false);
  setSelectionModeExt(Single);
  addColumn(i18n("Name"), -1);
  addColumn("");
  setFullWidth(true);
  setFocusPolicy(Qt::ClickFocus);

  m_popupMenu = new KMenu(this);
  m_popupMenu->addAction(SmallIconSet("delete"), i18n("Remove"), this, SLOT(slotRemoveItem()));
  connect(this, SIGNAL(contextMenuRequested(Q3ListViewItem *, const QPoint &, int)), SLOT(slotShowContextMenu(Q3ListViewItem *, const QPoint &, int)));
}


SimpleFileTree::~SimpleFileTree()
{
}


SimpleTreeViewItem *SimpleFileTree::createItem(const QString& startPath, const KUrl& url)
{
  QStringList pathSections = startPath.split('/', QString::SkipEmptyParts);
  QStringList::ConstIterator end = pathSections.constEnd();
  QString currentPath;
  int sectionCount = pathSections.count() - 1;
  SimpleTreeViewItem *parent = static_cast<SimpleTreeViewItem*>(firstChild());
  SimpleTreeViewItem *item = 0L;
  for (QStringList::ConstIterator it = pathSections.constBegin(); it != end; ++it)
  {
    currentPath += *it + "/";
    item = itemForPath(currentPath);
    if (!item)
    {
      KUrl u = url;
      for (int i = 0; i < sectionCount; i++)
        u = u.upURL();
      if (parent)
      {
        parent->setOpen(true);
        item = new SimpleTreeViewItem(u, currentPath, parent, 0);
      }
      else
        item = new SimpleTreeViewItem(u, currentPath, this);
    }
    parent = item;
    sectionCount--;
  }
  if (item)
    item->setOpen(true);
  return item;
}

SimpleTreeViewItem *SimpleFileTree::itemForURL(const KUrl& url)
{
  Q3ListViewItemIterator it(this);
  while (it.current())
  {
    SimpleTreeViewItem *item = static_cast<SimpleTreeViewItem*>(it.current());
    if (item->url() == url)
      return item;
    ++it;
  }
  return 0L;
}

SimpleTreeViewItem *SimpleFileTree::itemForPath(const QString& path)
{
  Q3ListViewItemIterator it(this);
  while (it.current())
  {
    SimpleTreeViewItem *item = static_cast<SimpleTreeViewItem*>(it.current());
    if (item->treePath() == path)
      return item;
    ++it;
  }
  return 0L;
}

void SimpleFileTree::slotShowContextMenu(Q3ListViewItem *item, const QPoint &pos, int col)
{
  if (item && item->depth() > 0)
    m_popupMenu->popup(pos);
}

void SimpleFileTree::slotRemoveItem()
{
  delete currentItem();
}

#include "simplefiletree.moc"
