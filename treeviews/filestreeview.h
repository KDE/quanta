/***************************************************************************
                          filestreeview.h  -  description
                             -------------------
    begin                : Thu Jun 1 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FILESTREEVIEW_H
#define FILESTREEVIEW_H

#include <qwidget.h>

#include "filemanage.h"

class FilesTreeFolder;

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class FilesTreeView : public FileManage  {
   Q_OBJECT
public:
	FilesTreeView( QString dir, QStringList topList = 0L, QWidget *parent=0L, const char *name=0L);
	~FilesTreeView();
	
public slots:
	void slotMenu(QListViewItem *, const QPoint &, int);

	void slotSelectFile(QListViewItem *);
	void slotSelectAnyFile(QListViewItem *item);
	void slotSelectImage(QListViewItem *);
	
	void slotNewMode();
	virtual void slotReload();
	
signals:
	void activatePreview();
	void openFile(  QString fileToOpen );
	void openImage( QString fileToOpen );
	
protected:
	virtual QString currentFileName();
	
public:
	FilesTreeFolder *homeDir;
  FilesTreeFolder *rootDir;

  QStringList dirList;
};

#endif
