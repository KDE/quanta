/***************************************************************************
                          templatestreeview.h  -  description
                             -------------------
    begin                : Thu Dec 20 2001
    copyright            : (C) 2001-2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef TEMPLATESTREEVIEW_H
#define TEMPLATESTREEVIEW_H

//own includes
#include "basetreeview.h"
#include "quantacommon.h"

//forward declarations
class QuantaPropertiesPage;
class KMainWindow;

class TemplatesTreeBranch : public BaseTreeBranch {

public:
  TemplatesTreeBranch(KFileTreeView *parent, const KURL& url,
                      const QString& name, const QPixmap& pix,
                      bool showHidden = false, KFileTreeViewItem *branchRoot = 0L);

  /** read the mimetype */
  virtual KFileTreeViewItem* createTreeViewItem(KFileTreeViewItem *parent,
                                                KFileItem *fileItem );
};


class TemplatesTreeView : public BaseTreeView  {
   Q_OBJECT

public:

  /**
   *  since this class is a singleton you must use this function to access it
   *
   *  the parameter are only used at the first call to create the class
   *
   */
  static TemplatesTreeView* const ref(KMainWindow *parent = 0L, const char *name = 0L)
  {
    static TemplatesTreeView *m_ref;
    if (!m_ref) m_ref = new TemplatesTreeView (parent, name);
    return m_ref;
  }

  ~TemplatesTreeView();
  /** Writes a .dirinfo file from the selected item's path */
  bool writeDirInfo(const QString& dirInfoFile = QString::null);
  /** Reads a .dirinfo file from the selected item's path */
  DirInfo readDirInfo(const QString& dir = QString::null);


public slots:

  /**  Inserts the content of the selected template into the
    *  activ document
    */
  void slotInsertInDocument();

  /**
   *  displays the RBM
   *
   *
   *  @param listView KListView where the event comes from
   *
   *  @param item QListViewItem where the mousepointer is hovering
   *
   *  @param point QPoint coordinates of the event
   *
   */
  void slotMenu(KListView *listView, QListViewItem *item, const QPoint &point);
  /** Creates a new document based in the selected template. */
  void slotNewDocument();
  /** Insert the template as text, image, new document. */
  void slotInsert();
  /** Extracts the site template to an user specified directory */
  void slotExtractSiteTemplate();
  /** No descriptions */
  void slotNewDir();
 /** Handles dropping on the document from the template tree */
  void slotDragInsert(QDropEvent *);

  /**
   *  slot for the RBM
   *
   *  packs and sends files or folders as attachment to an email
   */
  void slotSendInMail();
  void slotUploadTemplate();
  virtual void slotSelectFile(QListViewItem *item);
  void slotOpen();
  void slotPaste();
  void slotDelete();
  /** Properties dialog for template view */
  virtual void slotProperties();
  /** Property application for template view */
  virtual void slotPropertiesApplied();
  void slotInsertTag();

  /** Sets the project template directory */
  void slotNewProjectLoaded(const QString &, const KURL &, const KURL &);

private:

  /** The constructor is privat because we use singleton pattern.
   *  If you need the class use TemplatesTreeView::ref() for
   *  construction and reference
   */
  TemplatesTreeView(KMainWindow *parent, const char *name = 0L);
  /** Filters the template through and action, and returns the modified/filtered
  template file */
  KURL filterTemplate();
  void writeTemplateInfo();
  void updateTypeDescription(KFileTreeViewItem *item, const QString &typeString);

  BaseTreeBranch *m_projectDir;
  int m_deleteMenuId;
  int m_openId;
  int m_reloadMenuId;
  int m_downloadMenuId;
  QuantaPropertiesPage *m_quantaProperties;
  QString m_projectName;
  KURL localURL;
  KURL globalURL;
  KPopupMenu *m_fileMenu;
  KPopupMenu *m_folderMenu;
  KPopupMenu *m_emptyAreaMenu;
  KMainWindow *m_mainWindow;
  DirInfo m_dirInfo;
  DirInfo m_parentDirInfo;
  int m_insertFileInProject;
  int m_insertFolderInProject;
  int m_menuClose;                ///< remembers the menu entry
  int m_menuPasteFolder;          ///< remembers the menu entry

signals: // Signals
  void insertFile(const KURL &);
  void downloadTemplate();
  void uploadTemplate(const QString&);

protected: // Protected methods
  KFileTreeBranch* newBranch(const KURL& url);
  virtual QDragObject * dragObject();
//  virtual void startDrag();
  void contentsDropEvent(QDropEvent *event);
  bool acceptDrag(QDropEvent* e ) const;
  QString createTemplateTarball();
};

#endif
