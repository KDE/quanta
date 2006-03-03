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
#include "basetreeview.h"
//#include "projecturl.h"

class KIO::Job;
class FileInfoDlg;
class ProjectList;

class ProjectTreeViewItem : public BaseTreeViewItem {

public:
  ProjectTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch );
  /** makes document root italics */
  void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align);
};

class ProjectTreeBranch : public BaseTreeBranch {

public:
  ProjectTreeBranch(KFileTreeView *parent, const KURL& url,
                    const QString& name, const QPixmap& pix,
                    bool showHidden = false, KFileTreeViewItem *branchRoot = 0L);

  /** check for CVS and visible*/
  virtual KFileTreeViewItem* createTreeViewItem(KFileTreeViewItem *parent,
                                                KFileItem *fileItem );

  /** only files in list will be shown */
  ProjectList *urlList;
};



class ProjectTreeView : public BaseTreeView  {
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
  bool isDocumentFolder(const KURL &url);
  ProjectTreeBranch* rootBranch() {return m_projectDir;}
  void plugCVSMenu();

public slots: // Public slots
  void slotOpen();
  void slotLoadToolbar();
  void slotReloadTree(ProjectList *a_fileList, bool buildNewtree, const QStringList &folderToOpen);
  /** Sets new project information */
  void slotNewProjectLoaded(const QString &, const KURL &, const KURL &);
  void slotPopulateFinished(KFileTreeViewItem* );
  /** makes the url visible in the tree */
  void slotViewActivated(const KURL&);
  /** fills the list with the url's of the open folder  */
  void slotGetTreeStatus(QStringList *folderToOpen)
  {
    m_projectDir->addOpenFolder(folderToOpen);
  }



protected slots:
  void slotMenu(KListView *listView, QListViewItem *item, const QPoint &point);
  void slotRenameItem(QListViewItem* kvtvi, const QString & newText, int col);
  virtual void slotCreateFolder();
  virtual void slotCreateFile();

signals: // Signals
  void removeFromProject( const KURL& );
  /** No descriptions */
  void rescanProjectDir();
  /** No descriptions */
  void showProjectOptions();
  void insertToProject(const KURL&);
  void uploadSingleURL(const KURL&, const QString&, bool, bool);
  void loadToolbarFile(const KURL&);
  void uploadProject();
  void changeFileDescription(const KURL& url, const QString& desc);
  void changeUploadStatus(const KURL& url, int status);
  void changeDocumentFolderStatus(const KURL& url, bool status);
  void reloadProject();

private:
  /** The constructor is privat because we use singleton patter.
   *  If you need the class use ProjectTreeView::ref() for
   *  construction and reference
   */
  ProjectTreeView(QWidget *parent, const char *name);

  ProjectTreeBranch *m_projectDir;
  KPopupMenu *m_fileMenu;
  KPopupMenu *m_folderMenu;
  KPopupMenu *m_projectMenu;
  KURL m_documentRootURL;
  ProjectTreeViewItem *m_documentRootItem;
  KURL::List m_documentFolderList;
  KPopupMenu *m_uploadStatusMenu;
  ProjectList *m_projectFiles;
  int m_menuClose;                ///< remembers the menu entry

  int m_openInQuantaId;  ///< remembers the menu entry
  int m_setDocumentRootId;
  int m_alwaysUploadId;
  int m_neverUploadId;
  int m_confirmUploadId;

protected:
  /**
  creates a branch and adds this branch to the treeview
  @param url the root of the branch
  @return the new branch
  */
  virtual KFileTreeBranch* newBranch(const KURL& url);
  /**
  called when the description of an item was changed,
  @param item the treeview item
  @param newDesc the new description of item
  */
  void itemDescChanged(KFileTreeViewItem* item, const QString& newDesc);
  /**
  adds the Quanta fileinfopage to the properties dialog
  overwritten to enable the file description
  @param propDlg the dialog where to add the page
  @return pointer to to just added page
  */
  virtual FileInfoDlg* addFileInfoPage(KPropertiesDialog *propDlg);

private slots: // Private slots
  /** No descriptions */
  void slotUploadProject();
  /** Bring up the project options dialog */
  void slotOptions();
  /** No descriptions */
  void slotRescan();
  void slotRemoveFromProject(int askForRemove = 1);
  void slotUploadSingleURL();
  void slotQuickUploadURL();
  void slotAlwaysUpload();
  void slotNeverUpload();
  void slotConfirmUpload();
  void slotUploadMenuAboutToShow();
  void slotChangeDocumentFolderStatus();
};

#endif
