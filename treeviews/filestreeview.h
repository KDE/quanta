/***************************************************************************
                          filestreeview.h  -  description
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

#ifndef FILESTREEVIEW_H
#define FILESTREEVIEW_H

// quanta includes
#include "basetreeview.h"

//foward declarations
class QListViewItem;
class QPoint;
class KPopupMenu;
class KURL;


class FilesTreeView : public BaseTreeView {
   Q_OBJECT

public:
  FilesTreeView(KConfig *config, QWidget *parent, const char *name = 0L);
  virtual ~FilesTreeView();
  KURL::List topURLList;
  void plugCVSMenu();

protected slots:
  /**
  emit a signal @ref openInQuanta
  */
  virtual void slotOpenInQuanta();
  /**
  opens dialog to choose a url for a new branch
  */
  void slotNewTopFolder();

  virtual void slotMenu(KListView *listView, QListViewItem *item, const QPoint &point);
  /**
  creates a new branch from the current url
  */
  void slotAddToTop();

protected:
  virtual KFileTreeBranch* newBranch(const KURL& url);

  KPopupMenu *m_fileMenu;
  KPopupMenu *m_folderMenu;
  KPopupMenu *m_emptyMenu;
  int m_menuClose;                ///< remembers the menu entry
  int m_menuPasteFolder;          ///< remembers the menu entry
  int m_insertFileInProject;
  int m_insertFolderInProject;
  // config
  KConfig *m_config;

signals:
  void openInQuanta(QListViewItem *name);
  /**
   *  emited to make the script describtion visible
   */
  void showPreviewWidget(bool);

private:
  int m_menuTop;
  int m_menuDel;
  int m_reloadMenuId;
};

#endif
