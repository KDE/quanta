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
#ifndef SIMPLEFILETREE_H
#define SIMPLEFILETREE_H

#include <k3listview.h>

class SimpleTreeViewItem;

class KMenu;
class KUrl;

/**
Simple tree showing a directory sturcture, but using KUrl as a base, not files from the disk.

	@author Andras Mantia <amantia@kde.org>
*/
class SimpleFileTree : public K3ListView
{
  Q_OBJECT
public:
  SimpleFileTree(QWidget *parent = 0);
  ~SimpleFileTree();
  /**
   * Creates an item for a url
   * @param url the url
   * @return the newly created item
   */
  SimpleTreeViewItem *createItem(const QString& startPath, const KUrl& url);

private slots:
  void slotShowContextMenu(Q3ListViewItem *item, const QPoint &pos, int col);
  void slotRemoveItem();

private:
  /**
   * Returns the item associated with url.
   * @param url the url to test
   * @return the item associated with url or null, if no such item were found
   */
  SimpleTreeViewItem *itemForURL(const KUrl& url);

  /**
   * Returns the item associated with a path.
   * @param path the path to test
   * @return the item associated with url or null, if no such item were found
   */
  SimpleTreeViewItem *itemForPath(const QString& path);

  KMenu *m_popupMenu;
};

#endif
