/***************************************************************************
                          servertreeview.h  -  description
                             -------------------
    begin                : Thu Jun 1 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
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

#ifndef SERVERTREEVIEW_H
#define SERVERTREEVIEW_H

// quanta includes
#include "basetreeview.h"

//foward declarations
class QListViewItem;
class QPoint;
class KPopupMenu;
class KURL;
class ProjectList;

/**
      TreeViewItem with special painter
*/

class ServerTreeViewItem : public BaseTreeViewItem {

public:
  ServerTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch );
  /** makes document not in project lighter */
  void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align);
};

/**
      the branch for using @ref ServerTreeViewItem
*/

class ServerTreeBranch : public BaseTreeBranch {

public:
  ServerTreeBranch(KFileTreeView *parent, const KURL& url,
                   const QString& name, const QPixmap& pix,
                   bool showHidden = false, KFileTreeViewItem *branchRoot = 0L);

  /** check for CVS and visible*/
  virtual KFileTreeViewItem* createTreeViewItem(KFileTreeViewItem *parent,
                                                KFileItem *fileItem );
};


class ServerTreeView : public BaseTreeView {
   Q_OBJECT

public:
  ServerTreeView(KConfig *config, QWidget *parent, const KURL &url, const char *name = 0L);
  virtual ~ServerTreeView();

protected slots:

  void slotMenu(KListView *listView, QListViewItem *item, const QPoint &point);
  
  /** repaints the treeview items, because they might be added to or removed from the project*/
  void slotReloadTree( ProjectList *fileList, bool buildNewTree, const QStringList &folderToOpen);
  
protected:
  KFileTreeBranch* newBranch(const KURL& url);

  KPopupMenu *m_fileMenu;
  KPopupMenu *m_folderMenu;
  int m_menuClose;                ///< remembers the menu entry
  int m_menuPasteFolder;          ///< remembers the menu entry
  int m_insertFileInProject;
  int m_insertFolderInProject;
  // config
  KConfig *m_config;

private:
  int m_menuDel;
  int m_reloadMenuId;
  int m_renameId;
};

#endif
