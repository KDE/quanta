/***********************************************************************
                          project.h  -  description
                             -------------------
    begin                : Thu Mar 16 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2001-2003 by Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@freemail.hu
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
#include <qregexp.h>

#include <kio/job.h>


/**project
  *@author Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia
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
  KURL::List fileNameList(bool check = false);
  void loadProjectXML();

  void insertFile( const KURL& nameURL, bool repaint );
  void insertFiles( const KURL& pathURL, const QString& mask );
  void insertFiles( KURL::List files );

  bool createEmptyDom();

  void readConfig(KConfig *);
  void readLastConfig(KConfig *c=0);
  void writeConfig(KConfig *);
  /** No descriptions */
  bool isModified() {return modified;}
  /** Returns the relative url with the prefix inserted. */
  KURL urlWithPrefix(const KURL& url);
  /** Write property of QString defaultDTD. */
  virtual void setDefaultDTD( const QString& p_defaultDTD);
  /** Read property of QString defaultDTD. */
  virtual const QString& defaultDTD();
  virtual const QString& defaultEncoding() {return m_defaultEncoding;}

public slots:

  void slotNewProject();
  void slotOpenProject();
  void slotOpenProject(const KURL&);
  bool slotSaveProject();
  void slotCloseProject();
  void slotLoadProject(const KURL &a_url);
  void slotOptions();
  void slotUpload();
  void slotUploadURL(const KURL &urlToUpload);

  void slotAddFiles();
  void slotAddDirectory();
  void slotAddDirectory(const KURL& dirURL, bool showDlg = true);
  void slotInsertFile(const KURL& url);
  void slotInsertFilesAfterCopying(const KURL& url, CopyTo*);
  void slotRenameFinished( KIO::Job *);

  void slotRename(const KURL& url);
  void slotRemove(const KURL& urlToRemove);

  void slotAcceptCreateProject();

  void slotSelectProjectType(const QString &);

  void slotEnableMessages();
  void slotDisableMessages();
  void slotGetMessages(QString);

  void slotRescanPrjDir();
  /** Saves a project view (group of files & toolbars) asking for a name if askForName==true. */
  void slotSaveAsProjectView(bool askForName = true);
  /** Opens a project view (toolbars & files). */
  void slotOpenProjectView();
  /** Saves a project view (group of files & toolbars) without asking for a name. */
  void slotSaveProjectView();

signals:

  void openFile( const KURL&, const QString& );
  void closeFiles();

  void setBaseURL( const KURL& url );
  void setProjectName( QString );

  void showTree();
  void reloadTree(const KURL::List &, bool);
  
  void setLocalFiles( bool );

  void messages( QString );
  void selectMessageWidget();
  void disableMessageWidget();

  void saveAllFiles();
  void newStatus();
  void statusMsg(QString);
//  void checkOpenAction(bool);
  /** No descriptions */
  void removeFromProject(int);
  /** No descriptions */
  void newProjectLoaded();
  /** No descriptions */
  void templateURLChanged(const KURL &);

public:
  QDomDocument dom;

  KURL    projectURL;
  QString projectName;

  KURL baseURL;
  KURL remoteDir;
  KURL templateURL;
  KURL toolbarURL;

  KURL previewPrefix;
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
  QString currentProjectView;
  
  ProjectNewGeneral    *png;
  ProjectNewLocal      *pnl;
  ProjectNewWeb       *pnw;
  ProjectNewFinal     *pnf;
  
  KConfig *config;
  
  bool modified;
  bool olfwprj; //for internal use
  
  QBuffer buff;
  QRegExp excludeRx;
  QStringList excludeList;
protected: // Protected attributes
  /** Default DTD for this project. */
  QString m_defaultDTD;
  QString m_defaultEncoding;
  /** The old name of the url. (Used when renaming/removing an url from the project). */
  KURL oldURL;
  /** The new name of the url (used when renaming an url). */
  KURL newURL;

  void openCurrentView();
};

#endif
