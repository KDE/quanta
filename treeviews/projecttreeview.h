/***************************************************************************
                          projecttreeview.h  -  description
                             -------------------
    begin                : Tue Mar 14 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia
    email                : pdima@mail.univ.kiev.ua
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

#include <qwidget.h>
#include <qlistview.h>
#include <qstringlist.h>

#include <kurl.h>

#include "filestreeview.h"


class QPopupMenu;
class ProjectTreeFolder;
/**
  *@author Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia
  */

class ProjectTreeView : public FilesTreeView  {
	friend class Project;
	Q_OBJECT

public: 
	ProjectTreeView(QWidget *parent=0, const char *name=0);
	virtual ~ProjectTreeView();
	
	QString currentFileName();
	
public slots: // Public slots
	void slotOpen();
	void slotOpenWith();
	void slotOpenInQuanta();
	void slotRemove();
	void slotRemoveFromProject(int askForRemove=1);
	void slotUploadSingleFile();
	void slotUploadSingleFolder();
	void slotRenameFile();
	void slotRenameFolder();
	
  void slotMenu(QListViewItem*, const QPoint&, int);
  void slotFileTag();
  void slotReloadTree( QStringList, bool newtree, bool opened = false );
  void slotSetBasePath( QString dir );
  void slotSetProjectName( QString name );

signals: // Signals
	void open( QListViewItem *name );
 // void insertTag( QString );
  void renameFileInProject( QString );
  void renameFolderInProject( QString );
  void removeFileFromProject( QString );
  void removeFolderFromProject( QString );
  void uploadSingleFile( QString );
  void uploadSingleFolder( QString );
  void loadToolbarFile( const KURL& );

public:
  ProjectTreeFolder *projectDir;

private:
	QString basePath;
	QString projectName;
	
	QPopupMenu *fileMenu;
	QPopupMenu *folderMenu;

  int openInQuantaId;
};

#endif
