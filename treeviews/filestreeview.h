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
class FileInfoDlg;

struct DirInfo;

/** class for item with special sorting */
class FilesTreeViewItem : public KFileTreeViewItem {

public:
  FilesTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch );
  /** sorts dirs seperat from files */
  QString key (int column, bool ascending) const;
  /** makes compare independent from locale */
  int compare( QListViewItem *i, int col, bool ascending ) const;
  /** makes open files bold and 2. column gray */
  void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align);

};

/** class for branch with special items */
class FilesTreeBranch : public KFileTreeBranch {
   Q_OBJECT

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
  /** close the document in Quanta */
  virtual void slotClose();

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
  void slotReturnPressed(QListViewItem *item);
  /** Sets new project informations */
  void slotNewProjectLoaded(const QString &, const KURL &, const KURL &);

protected slots:
  void slotDropped (KURL::List&, KURL&);

protected:
  virtual KFileTreeBranch* newBranch(const KURL& url);
  virtual void itemRenamed(const KURL& , const KURL& );
  virtual void itemDescChanged(KFileTreeViewItem* item, const QString& newDesc);
  virtual bool isProjectView() const { return false; }
  virtual bool isPathInClipboard();
  const FileInfoDlg* addFileInfoPage(KPropertiesDialog *propDlg);
  /** expands an archiv, if possible */
  bool expandArchiv (KFileTreeViewItem *item);
  bool acceptDrag(QDropEvent* e ) const;
  void findDrop(const QPoint &pos, QListViewItem *&parent, QListViewItem *&after);

  KPopupMenu *m_fileMenu;
  KPopupMenu *m_folderMenu;
  KPopupMenu *m_emptyMenu;
  int m_menuClose;                ///< remembers the menu entry
  int m_menuPasteFolder;          ///< remembers the menu entry
  int m_insertFileInProject;
  int m_insertFolderInProject;
  // config
  KConfig *m_config;
  /** this is mainly for project and template tree to reduce includes there */
  bool isFileOpen(const KURL &url);

signals:
  void showPreviewWidget(bool);
  void openFile(const KURL&, const QString& );
  void openImage(const KURL&);
  void open(QListViewItem *name);
  void openInQuanta(QListViewItem *name);
  void insertDirInProject(const KURL&);
  void insertFileInProject(const KURL&);

  void insertTag(const KURL &, DirInfo);
  /** file or folder has been renamed */
  void renamed(const KURL &, const KURL &);
  /** close the file in Quanta */
  void closeFile( const KURL& );

private:
  int m_menuTop;
  int m_menuDel;
  int m_reloadMenuId;
  QString m_projectName;
  KURL m_projectBaseURL;
};

#endif
