/***********************************************************************
                          project.h  -  description
                             -------------------
    begin                : Thu Mar 16 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
    								  (C) 2001 by Andras Mantia
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

class CopyTo;

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
  void readLastConfig(KConfig *c=0);
  void writeConfig(KConfig *);
  /** No descriptions */
  bool isModified() {return modified;}
  /** Returns the relative url with the prefix inserted. */
  QString urlWithPrefix(const KURL& url);
  
public slots:

  void newProject();
  void openProject();
  void openProject(const KURL&);
  bool saveProject();
  void closeProject();
  void loadProject(const KURL &url);
  void options();
  void upload();
  void uploadFile(QString);

  void addFiles();
  void addDirectory();
  void addDirectory(QString rdir);
  void addDirectory(QString rdir, bool showDlg);
  void insertFile(QString name);
  void insertFilesAfterCopying(QString,CopyTo*);
  void renameFinished();

	void slotRenameFile(QString);
	void slotRenameFolder(QString);
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
	
	void openFile				 ( const KURL&, const QString& );
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
  void checkOpenAction(bool);
  /** No descriptions */
  void removeFromProject(int);
  /** No descriptions */
  void newProjectLoaded();
  /** No descriptions */
  void templateDirChanged(const QString &);

public:
  QDomDocument dom;

  KURL    url;
  QString name;

  QString basePath;
  QString remoteDir;
  QString templateDir;
  QString toolbarDir;

  QString previewPrefix;
  bool usePreviewPrefix;

  QString email;
  QString author;
  
  KRecentFilesAction *projectRecent;

    /** Holds the upload password. It is not saved, and it is lost after the project is closed. */
  QString passwd;
  /**  */
  bool keepPasswd;

private:
	QWizard *wiz;
	QWidgetStack *stack;
	
	ProjectNewGeneral		*png;
	ProjectNewLocal		  *pnl;
	ProjectNewWeb 			*pnw;
	ProjectNewFinal 		*pnf;
	
	KConfig *config;
	
	bool modified;
	bool olfwprj; //for internal use
	
  QBuffer buff;
};

#endif
