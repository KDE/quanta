/***************************************************************************
                          BaseTreeView.h  -  description
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

#ifndef BASETREEVIEW_H
#define BASETREEVIEW_H

//kde includes
#include <kfiletreebranch.h>
#include <kfiletreeview.h>
#include <kfiletreeviewitem.h>

// quanta includes
#include "quantacommon.h"

//foward declarations
class QListViewItem;
class QString;
class QPixmap;
class QPoint;
class QRegExp;
class QCloseEvent;

class KFileItem;
class KPopupMenu;
class KPropertiesDialog;
class KURL;
class FileInfoDlg;
class BaseTreeViewToolTip;

/**
 * @short a KFileTreeViewItem with some specials.
 *
 * @author Jens Herden <jens@kdewebdev.org>
 */
class BaseTreeViewItem : public KFileTreeViewItem {

public:
  BaseTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch );

  /**
  sorts folders separate from files
  */
  QString key (int column, bool ascending) const;

  /**
  makes compare independent from locale
  */
  int compare( QListViewItem *i, int col, bool ascending ) const;

  /**
  makes open files bold and the second column lighter
  */
  void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align);

  /**
  refresh icon
  */
  void refreshIcon();
};

/**
 * @short a KFileTreeBranch that uses @ref BaseTreeViewItem and does some special filtering.
 *
 * @author Jens Herden <jens@kdewebdev.org>
 */
class BaseTreeBranch : public KFileTreeBranch {
   Q_OBJECT

public:
  BaseTreeBranch(KFileTreeView *parent, const KURL& url,
                  const QString& name, const QPixmap& pix,
                  bool showHidden = false, KFileTreeViewItem *branchRoot = 0);
  /**
  get the BaseTreeViewItem
  */
  virtual KFileTreeViewItem* createTreeViewItem( KFileTreeViewItem *parent,
                                                 KFileItem *fileItem );
  /**
  does custom filtering
  */
  bool matchesFilter(const KFileItem *item) const;

  /**
  files matching to this will not be shown
  */
  QRegExp excludeFilterRx;

};


/**
 * @short the base class for many of Quantas treeviews
 *
 * @ref FilesTreeView, @ref ScriptTreeView, @ref ProjectTreeView and
 * @ref TemplatesTreeView use this class.
 *
 */
class BaseTreeView : public KFileTreeView {
   Q_OBJECT

public:
  BaseTreeView(QWidget *parent = 0L, const char *name = 0L);
  virtual ~BaseTreeView();

public slots:
  /**
  sets new project information
  */
  void slotNewProjectLoaded(const QString &, const KURL &, const KURL &);
  /**
  repaints all treeview items
  */
  void slotDocumentClosed();

protected slots:

  virtual void slotCopy();
  virtual void slotPaste();
  virtual void slotDelete();
  virtual void slotJobFinished( KIO::Job *job);
  virtual void slotInsertInProject();
  virtual void slotInsertDirInProject();
  virtual void slotReturnPressed(QListViewItem *item);
  virtual void slotDropped (QWidget *, QDropEvent *, KURL::List&, KURL&);
  /**
   sends value from KIO::Job to progressbar
   */
  virtual void slotPercent(KIO::Job *job, unsigned long value);

  virtual void slotPopulateFinished(KFileTreeViewItem *item);
  /**
  shows open with dialog for the current item
  */
  virtual void slotOpenWith();
  /**
  Called for: double click, return, Open

  emits a signal (@ref openFile or @ref openImage)
  or expands an archiv (@ref expandArchiv) in the tree
  */
  virtual void slotSelectFile(QListViewItem *item);
  /**
  emits the signal @ref insertTag
  */
  virtual void slotInsertTag();
  /**
  reloads the current branch
  */
  virtual void slotReload();
  /**
  opens the properties dialog
  */
  virtual void slotProperties();
  /**
  applies changed properties
  */
  virtual void slotPropertiesApplied();
  /**
  emits the signal @ref open
  */
  virtual void slotOpen();
  /**
  emits the signal @ref closeFile
  */
  virtual void slotClose();

  /**
  starts the inline rename of the current item
  */
  virtual void slotStartRename();

  /**
  connect signal itemRenamed to this slot to handle inline rename
   */
  virtual void slotRenameItem(QListViewItem* kvtvi, const QString & newText, int col);
  void slotRenameFinished(KIO::Job *job);

protected:
  /**
  implement this to create a branch and add this branch to the treeview
  @param url the root of the branch
  @return the new branch
  */
  virtual KFileTreeBranch* newBranch(const KURL& url) = 0;

  /**
  called when an item was renamed in the properties dialog
  the default implementation emits the signal @ref renamed
  */
  virtual void itemRenamed(const KURL& , const KURL& );

  /**
  called when the description of an item was changed,
  the default implementation does nothing
  @param item the treeview item
  @param newDesc the new description of item
  */
  virtual void itemDescChanged(KFileTreeViewItem* item, const QString& newDesc)
  {
    Q_UNUSED(item);
    Q_UNUSED(newDesc);
  };
  /**
  @return true if the clipboard contains a valid path
  */
  virtual bool isPathInClipboard();

  /**
  adds the Quanta fileinfopage to the properties dialog
  @param propDlg the dialog where to add the page
  @return pointer to to just added page
  */
  virtual FileInfoDlg* addFileInfoPage(KPropertiesDialog *propDlg);

  /**
  expands an archiv inside of the tree
  @return true if expanding was possible
  */
  bool expandArchiv (KFileTreeViewItem *item);
  bool acceptDrag(QDropEvent* e ) const;
  void findDrop(const QPoint &pos, QListViewItem *&parent, QListViewItem *&after);

  /**
  this is for project and template tree to reduce includes there
  */
  bool isFileOpen(const KURL &url);
  KPropertiesDialog *propDlg;
  QString m_projectName;
  KURL m_projectBaseURL;
  FileInfoDlg* fileInfoDlg;

  BaseTreeViewToolTip * m_tooltip;
  // some stuff for renaming
  KFileTreeViewItem* m_kftvi;
  KURL m_oldURL;
  KURL m_newURL;
  void doRename(KFileTreeViewItem* kvtvi, const QString & newName);
  /** reimplemented to reset renameable */
  void cancelRename(int col);

signals:
  void openFile(const KURL&);
  void openImage(const KURL&);
  void open(QListViewItem *name);
  void insertDirInProject(const KURL&);
  void insertFileInProject(const KURL&);
  void insertTag(const KURL &, DirInfo);
  /**
  file or folder has been renamed
  */
  void renamed(const KURL &, const KURL &);
  /**
  close the file in Quanta
  */
  void closeFile( const KURL& );

};

#endif
