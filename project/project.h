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

#include <qdom.h>
#include <qwidget.h>
#include <qstringlist.h>

#include <kio/job.h>

/**project
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class QWizard;
class KProcess;
class QWidgetStack;
class ProjectNewGeneral;
class ProjectNewLocal;
class ProjectNewWebS;
class ProjectNewFinalS;

class Project : public QWidget  {
   Q_OBJECT
public: 
	Project( QWidget *parent=0, const char *name=0);
	~Project();
	
	bool hasProject();
	QStringList fileNameList();
	
  void insertFile ( QString name, bool repaint );
  void insertFiles( QString path, QString mask );
  void insertFiles( QStringList files );

  void createEmptyDom();

public slots:

  void newProject();
  void openProject();
  void saveProject();
  void closeProject();
  void loadProject(QString fname);
  void options();

  void addFiles();
  void addDirectory();
  void addDirectory(QString rdir);
  void insertFile(QString name);

	void slotRemoveFile(QString);
	void slotRemoveFolder(QString);

	void slotAcceptCreateProject();
	
  void slotOpenedFiles(QStringList);

  void slotSelectProjectType(const QString &);

  void slotGetWgetExited(KProcess *);
  void slotGetWgetOutput(KProcess *proc, char *buffer, int buflen);

signals:
	
	void openFile				 ( QString );
	void closeFiles			 ();
	
	void setBasePath		 ( QString );
	void setProjectName	 ( QString );
	
	void reloadTree			 ( QStringList,bool,bool );
	
  void addRecentProject( const QString &);

  void requestOpenedFiles();
  void setLocalFiles( bool );

  void messages				 			( QString );
  void selectMessageWidget	();
  void disableMessageWidget	();

public:
  QDomDocument dom;

  QString projectName;
  QString projectFileName;

  QString basePath;
  QString remoteDir;

  QString previewPrefix;

  QString email;
  QString author;

  QStringList openedFiles;

private:
	QWizard *wiz;
	QWidgetStack *stack;
	
	ProjectNewGeneral		*png;
	ProjectNewLocal		  *pnl;
	ProjectNewWebS			*pnw;
	ProjectNewFinalS		*pnf;
};

#endif
