/***************************************************************************
                          projecttreeview.h  -  description
                             -------------------
    begin                : Tue Mar 14 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
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

#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

//own includes
#include "filestreeview.h"
#include "projecturl.h"

class KIO::Job;

class ProjectTreeViewItem : public FilesTreeViewItem {

public:
  ProjectTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch );
  /** makes document root italics */
  void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align);
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
  ProjectUrlList urlList;
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
    if (!m_ref) m_ref = new ProjectTreeView(parent, name);
    return m_ref;
  }

  virtual ~ProjectTreeView();
  void setDocumentRootItem(ProjectTreeViewItem* item) {m_documentRootItem = item;}
  ProjectTreeViewItem* documentRootItem() {return m_documentRootItem;}
  KURL documentRootURL() {return m_documentRootURL;}
  ProjectTreeBranch* rootBranch() {return m_projectDir;}

public slots: // Public slots
  void slotOpen();
  void slotLoadToolbar();
  void slotRemove();

  void slotMenu(KListView *listView, QListViewItem *item, const QPoint &point);
  void slotReloadTree(const ProjectUrlList &a_fileList, bool buildNewtree);
  /** Sets new project informations */
  void slotNewProjectLoaded(const QString &, const KURL &, const KURL &, const KURL &);
  /** reloads the tree again with current settings */
  void slotReload();
  virtual void slotPopulateFinished(KFileTreeViewItem* );

signals: // Signals
  void removeFromProject( const KURL& );
  /** No descriptions */
  void rescanProjectDir();
  /** No descriptions */
  void showProjectOptions();
  void insertToProject(const KURL&);
  void uploadSingleURL( const KURL& );
  void loadToolbarFile( const KURL& );
  void uploadProject();
  void changeFileDescription(const KURL& url, const QString& desc);
  void changeUploadStatus(const KURL& url, int status);
  void changeDocumentRoot(const KURL& url);
  void reloadProject();

private:
  /** The constructor is privat because we use singleton patter.
   *  If you need the class use ProjectTreeView::ref() for
   *  construction and reference
   */
  ProjectTreeView(QWidget *parent, const char *name);\
  void reload();
  void setUploadStatus(const KURL &url, int status);

  ProjectTreeBranch *m_projectDir;
  KURL m_oldURL;
  KURL m_newURL;
  KURL m_baseURL;
  KURL m_documentRootURL;
  ProjectTreeViewItem *m_documentRootItem;
  KPopupMenu *m_projectMenu;
  KPopupMenu *m_uploadStatusMenu;
  QString m_projectName;
  ProjectUrlList m_projectFiles;

  int m_openInQuantaId;  ///< remembers the menu entry
  int m_setDocumentRootId;
  int m_alwaysUploadId;
  int m_neverUploadId;
  int m_confirmUploadId;

protected:
  virtual void itemDescChanged(KFileTreeViewItem* item, const QString& newDesc);
  virtual bool isProjectView() const { return true; }

private slots: // Private slots
  /** No descriptions */
  void slotUploadProject();
  /** Bring up the project options dialog */
  void slotOptions();
  /** No descriptions */
  void slotRescan();
  void slotCreateFolder();
  void slotRemoveFromProject(int askForRemove = 1);
  void slotRename();
  void slotUploadSingleURL();
  void slotRenameFinished(KIO::Job *job);
  void slotAlwaysUpload();
  void slotNeverUpload();
  void slotConfirmUpload();
  void slotUploadMenuAboutToShow();
  void slotSetDocumentRoot();
};

#endif
