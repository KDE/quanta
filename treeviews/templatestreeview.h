/***************************************************************************
                          templatestreeview.h  -  description
                             -------------------
    begin                : Thu Dec 20 2001
    copyright            : (C) 2001-2002 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TEMPLATESTREEVIEW_H
#define TEMPLATESTREEVIEW_H

#include "filestreeview.h"

#include <qwidget.h>
#include <qlistview.h>

/**
  *@author Andras Mantia
  */

class FilesTreeFolder;
class KURL;

class TemplatesTreeView : public FilesTreeView  {
   Q_OBJECT
public: 
  TemplatesTreeView(const KURL& projectBaseURL,QWidget *parent=0, const char *name=0);
  ~TemplatesTreeView();
  /** Writes a .dirinfo file from the selected item's path */
  void writeDirInfo(QString dirInfoFile="");
  /** Reads a .dirinfo file from the selected item's path */
  void readDirInfo(QString startDir = "");
  

public slots:
  /** Sets the project template directory */
  void slotSetTemplateURL(const KURL& newTemplateURL);
  /** No descriptions */
  void slotInsertInDocument();
  void slotMenu(QListViewItem *item, const QPoint &point, int);
  /** No descriptions */
  void slotNewDocument();
  /** Insert the template as text, image, new document. */
  void slotInsert();
  /** No descriptions */
  void slotNewDir();
  virtual void slotReload();
  /** Properties dialog for template view */
  virtual void slotProperties();
  /** Property application for template view */
  virtual void slotPropertiesApplied();
 /** Handles dropping on the document from the template tree */
  void slotDragInsert(QDropEvent *);
  virtual void slotInsertTag();
  /** No descriptions */
  void contentsDragEnterEvent(QDragEnterEvent *event);

private:
  KURL baseURL;
  
  FilesTreeFolder *globalDir;
    FilesTreeFolder *localDir;
  /**  */
  int deleteMenuId;
  int openId;
  FilesTreeFolder *projectDir;
  QuantaPropertiesPageDlg *quantaProperties;
  
signals: // Signals
  /** No descriptions */
  void insertFile(const KURL &);
protected: // Protected methods
  /** No descriptions */
  virtual QDragObject * dragObject ();
//  virtual void startDrag();
  /** No descriptions */
  void contentsDropEvent(QDropEvent *);
};

#endif
