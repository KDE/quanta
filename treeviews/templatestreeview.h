/***************************************************************************
                          templatestreeview.h  -  description
                             -------------------
    begin                : Thu Dec 20 2001
    copyright            : (C) 2001 by Andras Mantia
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

class TemplatesTreeView : public FilesTreeView  {
   Q_OBJECT
public: 
	TemplatesTreeView(const QString& projectBasePath,QWidget *parent=0, const char *name=0);
	~TemplatesTreeView();

public slots:
  /** No descriptions */
  void slotInsertInDocument();
  void slotMenu(QListViewItem *item, const QPoint &point, int);
  /** No descriptions */
  void slotNewDocument();
  /** Insert the template as text, image, new document. */
  void slotInsert();
  virtual void slotReload();

private:
	QString basePath;
	
	FilesTreeFolder *globalDir;
  	FilesTreeFolder *localDir;
  /**  */
  int deleteMenuId;
  int openId;
  FilesTreeFolder *projectDir;
	
signals: // Signals
  /** No descriptions */
  void insertFile(QString);
};

#endif
