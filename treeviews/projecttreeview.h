/***************************************************************************
                          projecttreeview.h  -  description
                             -------------------
    begin                : Tue Mar 14 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
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

#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

//own includes
#include "filestreeview.h"

class KIO::Job;

/** class for item with special paint */
class ProjectTreeViewItem : public FilesTreeViewItem {

public:
  ProjectTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch );

  void paintCell(QPainter *p, const QColorGroup &cg,
                 int column, int width, int align);
};

class ProjectTreeBranch : public FilesTreeBranch {

public:
  ProjectTreeBranch(KFileTreeView *parent, const KURL& url,
                    const QString& name, const QPixmap& pix,
                    bool showHidden = false, KFileTreeViewItem *branchRoot = 0L);

  /** check for CVS and visible*/
  virtual KFileTreeViewItem* createTreeViewItem(KFileTreeViewItem *parent,
                                                KFileItem *fileItem );

  /** only files in list will be shown */
  KURL::List urlList;
};



class ProjectTreeView : public FilesTreeView  {
  Q_OBJECT

public:
  /**
   *  since this class is a singleton you must use this function to access it
   *
   *  the parameters are only used at the first call to create the class
   *
   */
  static ProjectTreeView* const ref(QWidget *parent = 0L, const char *name = 0L)
  {
    static ProjectTreeView *m_ref;
    if (!m_ref) m_ref = new ProjectTreeView (parent, name);
    return m_ref;
  }

  virtual ~ProjectTreeView();

public slots: // Public slots
  void slotOpen();
  void slotOpenInQuanta();
  void slotRemove();

  void slotMenu(KListView *listView, QListViewItem *item, const QPoint &point);
  void slotReloadTree(const KURL::List &a_urlList, bool buildNewtree);
  /** Sets new project informations */
  void slotNewProjectLoaded(const QString &, const KURL &, const KURL &);
  /** triggers repaint of treeview */
  void slotDocumentClosed();
  /** reloads the tree again with current settings */
  void slotReload();

signals: // Signals
  void removeFromProject( const KURL& );
  /** No descriptions */
  void rescanProjectDir();
  /** No descriptions */
  void showProjectOptions();
  void uploadSingleURL( const KURL& );
  void loadToolbarFile( const KURL& );
  void uploadProject();

private:
  /** The constructor is privat because we use singleton patter.
   *  If you need the class use ProjectTreeView::ref() for
   *  construction and reference
   */
  ProjectTreeView(QWidget *parent = 0L, const char *name = 0L);

  ProjectTreeBranch *m_projectDir;
  KURL m_oldURL;
  KURL m_newURL;
  KURL m_baseURL;
  KURL::List m_urlList;
  QPopupMenu *m_projectMenu;
  QString m_projectName;

  int m_openInQuantaId;

private slots: // Private slots
  /** No descriptions */
  void slotUploadProject();
  /** Bring up the project options dialog */
  void slotOptions();
  /** No descriptions */
  void slotRescan();
  void slotRemoveFromProject(int askForRemove = 1);
  void slotRename();
  void slotUploadSingleURL();
  void slotRenameFinished(KIO::Job *job);
};

#endif
