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
#include <qvaluelist.h>

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
  /** Open a subfolder. */
  void openFolder(ProjectTreeFolder *folder);
	
	
public slots: // Public slots
	void slotOpen();
	void slotOpenWith();
	void slotOpenInQuanta();
	void slotRemove();
	void slotRemoveFromProject(int askForRemove=1);
	void slotUploadSingleURL();
	void slotRename();
	
  void slotMenu(QListViewItem*, const QPoint&, int);
  void slotReloadTree(const KURL::List &a_urlList, bool buildNewtree);
  void slotSetBaseURL( const KURL& url );
  void slotSetProjectName( QString name );
  /** No descriptions */
  void slotRescan();
  /** Bring up the project options dialog */
  void slotOptions();
  /** Remove all the deleted - from the project - url's from the treeview. */
  void slotRemoveDeleted();

signals: // Signals
	void open( QListViewItem *name );
 // void insertTag( QString );
  void renameInProject( const KURL& );
  void removeFromProject( const KURL& );
  /** No descriptions */
  void rescanProjectDir();
  /** No descriptions */
  void showProjectOptions();
  void uploadSingleURL( const KURL& );
  void loadToolbarFile( const KURL& );
  void uploadProject();

public:
  ProjectTreeFolder *projectDir;

private:
	KURL baseURL;
  KURL::List urlList;
  QPopupMenu *projectMenu;
	QString projectName;
	
  int openInQuantaId;

protected:
  virtual void itemRenamed(const KURL& , const KURL& ) {};
	virtual KURL currentURL();

private slots: // Private slots
  /** No descriptions */
  void slotUploadProject();
};

#endif
