/***************************************************************************
                          filestreeview.h  -  description
                             -------------------
    begin                : Thu Jun 1 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
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

#ifndef FILESTREEVIEW_H
#define FILESTREEVIEW_H

//kde includes
#include <kfiletreebranch.h>
#include <kfiletreeview.h>
#include <kfiletreeviewitem.h>


//foward declarations
class QDragEnterEvent;
class QListViewItem;
class QString;
class QPixmap;
class QPoint;
class QRegExp;

class KFileItem;
class KPopupMenu;
class KPropertiesDialog;
class KURL;

struct DirInfo;

/** class for item with special sorting */
class FilesTreeViewItem : public KFileTreeViewItem {

public:
  FilesTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch );
    /* sorts dirs seperat from files */
  QString key (int column, bool ascending) const;
};

/** class for branch with special items */
class FilesTreeBranch : public KFileTreeBranch {

public:
  FilesTreeBranch(KFileTreeView *parent, const KURL& url,
                  const QString& name, const QPixmap& pix,
                  bool showHidden = false, KFileTreeViewItem *branchRoot = 0);
  /** get the special items */
  virtual KFileTreeViewItem* createTreeViewItem( KFileTreeViewItem *parent,
                                                  KFileItem *fileItem );
  /** does the filtering */
  bool matchesFilter(const KFileItem *item) const;

public:
  /** only files in list will be shown */
  KURL::List urlList;
  /** files matching to this will not be shown */
  QRegExp excludeFilterRx;
};


class FilesTreeView : public KFileTreeView {
   Q_OBJECT

public:
  FilesTreeView(QWidget *parent = 0L, const char *name = 0L);
  FilesTreeView(KURL::List topList, QWidget *parent = 0L, const char *name = 0L);
  virtual ~FilesTreeView();
  KURL::List topURLList;

public slots:
  virtual void slotMenu(KListView *listView, QListViewItem *item, const QPoint &point);
  virtual void slotSelectFile(QListViewItem *item);
  void slotAddToTop();
  void slotNewTopFolder();
  /** No descriptions */
  virtual void slotInsertTag();
  virtual void slotReload();
  virtual void slotProperties();

  virtual void slotOpen();
  void slotOpenWith();
  void slotOpenInQuanta();
  void slotCopy();
  virtual void slotPaste();
  virtual void slotDelete();
  virtual void slotPopulateFinished(KFileTreeViewItem *item);
  virtual void slotJobFinished( KIO::Job *job);
  /* sends value from KIO::Job to progressbar  */
  virtual void slotPercent(KIO::Job *job, unsigned long value);
  virtual void slotInsertInProject();
  virtual void slotInsertDirInProject();
 /** Handles dropping  */
//  void slotDragInsert(QDropEvent *);
  /** No descriptions */
  void contentsDragEnterEvent(QDragEnterEvent *event);

protected:
  KFileTreeBranch* newBranch(const KURL& url);
  virtual void itemRenamed(const KURL& , const KURL& );
  void addFileInfoPage(KPropertiesDialog *propDlg);
  int denyBinaryInsert();
  void contentsDropEvent(QDropEvent *event);

  KPopupMenu *m_fileMenu;
  KPopupMenu *m_folderMenu;
  KPopupMenu *m_emptyMenu;
  int m_insertFileInProject;
  int m_insertFolderInProject;

signals:
  void showPreviewWidget(bool);
  void openFile(const KURL&, const QString& );
  void openImage(const KURL&);
  void open(QListViewItem *name);
  void openInQuanta(QListViewItem *name);
  void insertDirInProject(const KURL&);
  void insertFileInProject(const KURL&);

  void insertTag(const KURL &, DirInfo);

private:
  int m_menuTop;
  int m_menuDel;
};

#endif
