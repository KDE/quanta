/***********************************************************************
                          project.h  -  description
                             -------------------
    begin                : Thu Mar 16 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
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

#ifndef PROJECT_H
#define PROJECT_H

#include <qwidget.h>
#include <qlist.h>
#include <qstringlist.h>

#include <kio/job.h>

#include "projectitem.h"

/**project
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class KConfig;
class KWizard;
class KProcess;
class ProjectNewGeneral;
class ProjectNewGetFiles;

class Project : public QWidget  {
   Q_OBJECT
public: 
	Project( QWidget *parent=0, const char *name=0);
	~Project();
	
	bool hasProject();
  void loadProject(QString name);
  void insertFilesRecursive( QString path, bool, int level = 0);

  QStringList fileNameList();

public slots: // Public slots
  void closeProject();
  void openProject();
  void newProject();
  void saveProject();
  void addFiles();
  void addDirectory();
  void addDirectory(QString dir);
  void slotInsertDirFinished( KIO::Job *);

	void slotRemoveFile(QString);
	void slotRemoveFolder(QString);

  void slotOpenedFiles(QStringList);
  /**  */
//  void insertFromTree(QListViewItem *item);
  /**  */
  void insertFile( ProjectItem *file );
  void insertFile( QString name );
  /** remove file from project */
//  void removeFile(QListViewItem *);
  /**  */
//  void addFiles();
//  void rescanDirectory();
  /** upload current project */
//  void uploadProject();
  /** project properties */
//  void properties();
  /** setup properties */
  /** for ok button on new project wizard */
  void acceptNewPrj();
//  void acceptProperties();
  /**  */
  void slotGetWgetExited(KProcess*);
  void slotGetWgetOutput(KProcess *proc, char *buffer, int buflen);
signals:
	void setBasePath		 ( QString );
	void setProjectName	 ( QString );
	void reloadTree			 ( QStringList,bool,bool );
	void openFile				 ( QString );
  void addRecentProject( const QString &);
  void messages				 ( QString );
  void closeFiles			 ();
  void requestOpenedFiles();

  void selectMessageWidget();
  void disableMessageWidget();

public:
  KConfig* projectFile;
  /** list of ProjectFiles */
  QList<ProjectItem> fileList;
  /** base path of project files */
  QString basePath;
  QString baseURL;
  QString host;
  QString remoteDir;
  QString login;
  QString projectFileName;
  QString projectName;
  QString email;
  QString author;

  QStringList openedFiles;
  /** piece of properties dialog */
//  PrjPropertiesMain *mainDlg;
//  PrjPropertiesFTP  *ftpDlg;
  /** wizard for new project */
  KWizard *wizardNewPrj;
  ProjectNewGeneral  *pageNewPrj1;
  ProjectNewGetFiles *pageNewPrj2;
//  PrjPropertiesFTP *pageNewPrj3;
private:
	QString tmppath;
};

#endif
