/***************************************************************************
                          filemanage.h  -  description
                             -------------------
    begin                : Mon Aug 7 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon & Andras Mantia
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

#ifndef FILEMANAGE_H
#define FILEMANAGE_H

#include <qlistview.h>
#include <kio/job.h>


class QPopupMenu;

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon & Andras Mantia
  */

/*This will remain here, as we need the inserTag(QString, DirInfo) signal.
  DirInfo used here to specify the pre/post text for the tag. */
typedef struct DirInfo{
      QString mimeType;
      QString preText;
      QString postText;
      bool    usePrePostText;
    };

class FileManage : public QListView {
Q_OBJECT
public:
    FileManage();
	FileManage( QWidget *parent, const char *name );
	~FileManage();

public slots:	
	void slotOpen();
	void slotOpenWith();
	void slotOpenInQuanta();
	void slotCopy();
	void slotPaste();
	void slotDelete();
	virtual void slotProperties();
	
	void slotInsertInProject();
	void slotInsertDirInProject();
	
	virtual void slotInsertTag();
	
	virtual void slotReload();
	void slotJobFinished( KIO::Job *);
  /** No descriptions */
  virtual void slotPropertiesApplied();
	
signals:
	void open( QListViewItem *name );
	void openInQuanta( QListViewItem *name );
	
	void insertDirInProject( QString );
	void insertFileInProject( QString );
	
	void insertTag(QString, DirInfo);
	
	void jobFinished();
	void changeMode();

  /** No descriptions */
  void reloadTreeviews();
	
public:
	QPopupMenu *fileMenu;
  QPopupMenu *folderMenu;

protected:
	virtual QString currentFileName();
  DirInfo dirInfo;

};

#endif
