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
#include <qbuffer.h>
#include <qwidget.h>
#include <qstringlist.h>

#include <kio/job.h>

/**project
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class QWizard;
class QWidgetStack;

class KConfig;
class KTempFile;
class KRecentFilesAction;

class ProjectNewWeb;
class ProjectNewLocal;
class ProjectNewFinal;
class ProjectNewGeneral;

class Project : public QWidget  {
   Q_OBJECT
public: 
	Project( QWidget *parent=0, const char *name=0);
	~Project();
	
	bool hasProject();
	QStringList fileNameList(bool check = false);
	void loadProjectXML();
	
  void insertFile ( QString name, bool repaint );
  void insertFiles( QString path, QString mask );
  void insertFiles( QStringList files );

  void createEmptyDom();

  void  readConfig(KConfig *);
  void writeConfig(KConfig *);
  
public slots:

  void newProject();
  void openProject();
  void openProject(const KURL&);
  bool saveProject();
  void closeProject();
  void loadProject(const KURL &url);
  void options();
  void upload();

  void addFiles();
  void addDirectory();
  void addDirectory(QString rdir);
  void insertFile(QString name);
  void insertFilesAfterCopying(QString);

	void slotRemoveFile(QString);
	void slotRemoveFolder(QString);

	void slotAcceptCreateProject();
	
  void slotSelectProjectType(const QString &);

  void slotEnableMessages();
  void slotDisableMessages();
  void slotGetMessages(QString);
  
  void slotRescanPrjDir();
  
  void slotProjectReadFinish(KIO::Job *);
  void slotProjectReadData  (KIO::Job *,const QByteArray &);

  
signals:
	
	void openFile				 ( KURL& );
	void closeFiles			 ();
	
	void setBasePath		 ( QString );
	void setProjectName	 ( QString );
	
	void   showTree      ();
	void reloadTree			 ( QStringList,bool,bool );
	
  void setLocalFiles   ( bool );

  void messages				 			( QString );
  void selectMessageWidget	();
  void disableMessageWidget	();

  void saveAllFiles();
  void newStatus();
  void statusMsg(QString);

public:
  QDomDocument dom;

  KURL    url;
  QString name;

  QString basePath;
  QString remoteDir;

  QString previewPrefix;

  QString email;
  QString author;
  
  KRecentFilesAction *projectRecent;

private:
	QWizard *wiz;
	QWidgetStack *stack;
	
	ProjectNewGeneral		*png;
	ProjectNewLocal		  *pnl;
	ProjectNewWeb 			*pnw;
	ProjectNewFinal 		*pnf;
	
  QBuffer buff;

  KTempFile *m_tempSaveFile;
};

#endif
