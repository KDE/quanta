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
#include <qstrlist.h>

#include <kconfig.h>
#include <kpropertiesdialog.h>

#include "filemanage.h"

class FilesTreeFolder;
class QuantaPropertiesPageDlg;

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon & Andras Mantia
  */


class FilesTreeView : public FileManage  {
   Q_OBJECT
public:
    FilesTreeView();
	FilesTreeView( QString dir, QStrList topList, QWidget *parent=0L, const char *name=0L);
	~FilesTreeView();
  void addFileInfoPage(KPropertiesDialog *propDlg);
	
public slots:
	void slotMenu(QListViewItem *, const QPoint &, int);

	void slotSelectFile   (QListViewItem *);
	void slotSelectAnyFile(QListViewItem *);
	void slotSelectImage  (QListViewItem *);
	void slotAddToTop     ();
	
	void slotNewMode();
  /** No descriptions */
  virtual void slotInsertTag();
	virtual void slotReload();
  virtual void slotProperties();
	
signals:
	void activatePreview();
	void openFile ( const KURL&, const QString& );
	void openImage( QString fileToOpen );
	
protected:
	virtual QString currentFileName();
  /** Return true, if the file is a text file. */
  bool isText(QString fileName);
	
public:
	FilesTreeFolder *homeDir;
  FilesTreeFolder *rootDir;

  QStrList dirList;
};

#endif
