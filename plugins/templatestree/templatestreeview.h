/***************************************************************************
    begin                : Thu Dec 20 2001
    copyright            : (C) 2001-2004 by Andras Mantia <amantia@kde.org>
                           (C) 2005         Jens Herden <jens@kdewebdev.org>
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
#include "helper.h"

#include <ktempdir.h>

//forward declarations
class Q3DragObject;
class KMainWindow;
class KTemporaryFile;

namespace Ui
{
  class QuantaPropertiesPage;
}
class TemplatesTreePart;

class TemplatesTreeBranch : public BaseTreeBranch {

public:
  TemplatesTreeBranch(BaseTreeView *parent, const KUrl& url,
                      const QString& name, const QPixmap& pix,
                      bool showHidden = false, K3FileTreeViewItem *branchRoot = 0L);

  /** read the mimetype */
  virtual K3FileTreeViewItem* createTreeViewItem(K3FileTreeViewItem *parent,
                                                KFileItem *fileItem );
};


class TemplatesTreeView : public BaseTreeView  {
   Q_OBJECT

public:

  TemplatesTreeView(TemplatesTreePart *plugin);

  ~TemplatesTreeView();

public slots:
  /**
   * adjust the treeview to the new settings
   */
  void slotSettingsChanged();

  /**  Inserts the content of the selected template into the
    *  active document
    */
  void slotInsertInDocument();
  
  /** Creates a new document based in the selected template. */
  void slotNewDocument();
  
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
  virtual void slotSelectFile(Q3ListViewItem *item);
  void slotOpen();
  void slotPaste();
  void slotDelete();
  
  /** Properties dialog for template view */
  virtual void slotProperties();
  
  /** Property application for template view */
  virtual void slotPropertiesApplied();
  
  void slotInsertTag();

  /** Sets the project template directory */
  void slotProjectOpened();

signals:

  void insertFile(const KUrl &);
  void downloadTemplate();
  void uploadTemplate(const QString&);
  void insertTag(const KUrl &, Helper::DirInfo *);

private:
  
  /** Writes a .dirinfo file from the selected item's path */
  bool writeDirInfo(const QString& dirInfoFile = QString::null);
  
  /** Reads a .dirinfo file from the selected item's path */
  Helper::DirInfo readDirInfo(const QString& dir = QString::null);

  /** Filters the template through and action, and returns the modified/filtered
  template file */
  KUrl filterTemplate();
  
  void writeTemplateInfo();
  void updateTypeDescription(K3FileTreeViewItem *item, const QString &typeString);

  BaseTreeBranch *m_projectDir;
  Ui::QuantaPropertiesPage *m_quantaProperties;
  QString m_projectName;
  KUrl localURL;
  KUrl globalURL;
  KMainWindow *m_mainWindow;
  Helper::DirInfo m_dirInfo;
  Helper::DirInfo m_parentDirInfo;
  int m_insertFileInProject;
  int m_insertFolderInProject;
  QList<KTempDir*> tempDirList;
  QList<KTemporaryFile*> tempFileList;
  TemplatesTreePart * m_part;
  
protected: // Protected methods

  /**
   * show a context menu if no file or folder is available
   * @param point position to popup
   */
  void emptyMenu(const QPoint &point);
  
  /**
   * show a context menu for the current file
   * @param point position to popup
   */
  void fileMenu(const QPoint &point);
  
  /**
   * show a context menu for the current folder
   * @param point position to popup
   */
  void folderMenu(const QPoint &point);

  KFileTreeBranch* newBranch(const KUrl& url);
  virtual Q3DragObject * dragObject();
//  virtual void startDrag();
  void contentsDropEvent(QDropEvent *event);
  bool acceptDrag(QDropEvent* e ) const;
  QString createTemplateTarball();
};

#endif
