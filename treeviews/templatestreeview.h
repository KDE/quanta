/***************************************************************************
                          templatestreeview.h  -  description
                             -------------------
    begin                : Thu Dec 20 2001
    copyright            : (C) 2001-2003 by Andras Mantia <amantia@kde.org>
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
#include "filestreeview.h"
#include "../quantacommon.h"

//forward declarations
class QuantaPropertiesPage;


class TemplatesTreeView : public FilesTreeView  {
   Q_OBJECT
public:
  TemplatesTreeView(const KURL& projectBaseURL, QWidget *parent = 0L, const char *name = 0L);
  ~TemplatesTreeView();
  /** Writes a .dirinfo file from the selected item's path */
  void writeDirInfo(const QString& dirInfoFile = QString::null);
  /** Reads a .dirinfo file from the selected item's path */
  void readDirInfo(const QString& dir = QString::null);


public slots:
  /** Sets the project template directory */
  void slotSetTemplateURL(const KURL& newTemplateURL);
  /** No descriptions */
  void slotInsertInDocument();
  void slotMenu(KListView *listView, QListViewItem *item, const QPoint &point);
  /** No descriptions */
  void slotNewDocument();
  /** Insert the template as text, image, new document. */
  void slotInsert();
  /** No descriptions */
  void slotNewDir();
 /** Handles dropping on the document from the template tree */
  void slotDragInsert(QDropEvent *);
  /** No descriptions */
  void contentsDragEnterEvent(QDragEnterEvent *event);
  /** packs and sends files or folders */
  void slotSendInMail();
  virtual void slotSelectFile(QListViewItem *item);
  virtual void slotOpen();
  virtual void slotPaste();
  virtual void slotDelete();
  /** Properties dialog for template view */
  virtual void slotProperties();
  /** Property application for template view */
  virtual void slotPropertiesApplied();
  virtual void slotInsertTag();

private:

  FilesTreeBranch *m_globalDir;
  FilesTreeBranch *m_localDir;
  FilesTreeBranch *m_projectDir;
  int m_deleteMenuId;
  int m_openId;
  QuantaPropertiesPage *m_quantaProperties;
  /** Filters the template through and action, and returns the modified/filtered
  template file */
  KURL filterTemplate();

signals: // Signals
  /** No descriptions */
  void insertFile(const KURL &);

protected: // Protected methods
  /** No descriptions */
  virtual QDragObject * dragObject();
  DirInfo m_dirInfo;
//  virtual void startDrag();
  /** No descriptions */
  void contentsDropEvent(QDropEvent *event);
  void writeTemplateInfo();
};

#endif
