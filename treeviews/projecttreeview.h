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

class ProjectTreeBranch : public FilesTreeBranch {

public:
  ProjectTreeBranch(KFileTreeView *parent, const KURL& url,
                    const QString& name, const QPixmap& pix,
                    bool showHidden = false, KFileTreeViewItem *branchRoot = 0L);

  /** check for CVS */
  virtual KFileTreeViewItem* createTreeViewItem(KFileTreeViewItem *parent,
                                                KFileItem *fileItem );
};



class ProjectTreeView : public FilesTreeView  {
  friend class Project;
  Q_OBJECT

public:
  ProjectTreeView(QWidget *parent = 0L, const char *name = 0L);
  virtual ~ProjectTreeView();

  /** adds the baseURL of project */
  KURL addBaseURL(const KURL& );

public slots: // Public slots
  void slotOpen();
  void slotOpenInQuanta();
  void slotRemove();
  void slotRemoveFromProject(int askForRemove = 1);
  void slotUploadSingleURL();
  void slotRename();

  void slotMenu(KListView *listView, QListViewItem *item, const QPoint &point);
  void slotReloadTree(const KURL::List &a_urlList, bool buildNewtree);
  void slotSetBaseURL(const KURL& url);
  void slotSetProjectName(const QString& name);
  /** No descriptions */
  void slotRescan();
  /** Bring up the project options dialog */
  void slotOptions();
  /** Remove all the deleted - from the project - url's from the treeview. */
  void slotRemoveDeleted();

signals: // Signals
  void renameInProject( const KURL& );
  void removeFromProject( const KURL& );
  /** No descriptions */
  void rescanProjectDir();
  /** No descriptions */
  void showProjectOptions();
  void uploadSingleURL( const KURL& );
  void loadToolbarFile( const KURL& );
  void uploadProject();

private:
  FilesTreeBranch *m_projectDir;
  KURL m_baseURL;
  KURL::List m_urlList;
  QPopupMenu *m_projectMenu;
  QString m_projectName;

  int m_openInQuantaId;

protected:
  virtual void itemRenamed(const KURL& , const KURL& ) {};

private slots: // Private slots
  /** No descriptions */
  void slotUploadProject();
};

#endif
