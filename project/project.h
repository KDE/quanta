/***********************************************************************
                          project.h  -  description
                             -------------------
    begin                : Thu Mar 16 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2001-2003 by Andras Mantia <amantia@kde.org>
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

#include <kdeversion.h>
#include <kio/job.h>
#include <kurl.h>

#include "project_url.h"

/**project
  *@author Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia
  */

class QWizard;
class QWidgetStack;

class KConfig;
class KTempFile;
#if KDE_VERSION < KDE_MAKE_VERSION(3,1,92)
class KQRecentFilesAction;
#else
class KRecentFilesAction;
#endif

class ProjectNewWeb;
class ProjectNewLocal;
class ProjectNewFinal;
class ProjectNewGeneral;

class CopyTo;

class Project : public QObject  {
   Q_OBJECT
public:

  /**
   *  since this class is a singleton you must use this function to access it
   *
   *  the parameter are only used at the first call to create the class
   *
   */
  static Project* const ref()
  {
    static Project *m_ref;
    if (!m_ref) m_ref = new Project ();
    return m_ref;
  }

  ~Project();

  bool hasProject() {return !projectName.isNull();};

  KURL::List fileNameList(bool check = false);

  void insertFile( const KURL& nameURL, bool repaint );
  void readConfig(KConfig *);
  void readLastConfig(KConfig *c=0);
  void writeConfig(KConfig *);
  /** No descriptions */
  bool isModified() {return m_modified;}
  void setModified(bool modified);
  /** Returns the relative url with the prefix inserted. */
  KURL urlWithPrefix(const KURL& url);
  bool contains(const KURL &url);
  /** Read property of QString defaultDTD. */
  virtual const QString& Project::defaultDTD() {return m_defaultDTD;}
  virtual const QString& defaultEncoding() {return m_defaultEncoding;}

  /** Returns the project's base URL if it exists,
   * the HOME dir if there is no project and no opened document
   * (or the current  opened document was not saved yet),
   * and the base URL of the opened document, if it is saved somewhere.
   */
  KURL projectBaseURL();

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
  void slotInsertFilesAfterCopying(const KURL::List& a_url);
  void slotDeleteCopytoDlg(CopyTo *);

  /** if somewhere something was renamed */
  void slotRenamed(const KURL& oldURL, const KURL& newURL);
  void slotRemove(const KURL& urlToRemove);

  void slotAcceptCreateProject();

  void slotSelectProjectType(const QString &);

  void slotEnableMessages(bool enable);
  void slotGetMessages(const QString&);

  void slotRescanPrjDir();
  /** Saves a project view (group of files & toolbars) asking for a name if askForName==true. */
  void slotSaveAsProjectView(bool askForName = true);
  /** Opens a project view (toolbars & files). */
  void slotOpenProjectView();
  /** Saves a project view (group of files & toolbars) without asking for a name. */
  void slotSaveProjectView();
  /** Deletes a project view */
  void slotDeleteProjectView();
  void slotFileDescChanged(const KURL& url, const QString& desc);

signals:

  void openFile( const KURL&, const QString& );
  void closeFile( const KURL&);
  void closeFiles();

  void showTree();
  void reloadTree(const ProjectUrlList &, bool);

  void setLocalFiles( bool );

  void messages(const QString& );
  void enableMessageWidget(bool);

  void saveAllFiles();
  void newStatus();
  void statusMsg(QString);
  /** No descriptions */
  void newProjectLoaded(const QString &, const KURL &, const KURL &);
  void hideSplash();

public:
  QDomDocument dom;

  KURL    projectURL;
  QString projectName;

  KURL baseURL;
  KURL templateURL;
  KURL toolbarURL;

  KURL previewPrefix;
  bool usePreviewPrefix;

  QString email;
  QString author;

#if KDE_VERSION < KDE_MAKE_VERSION(3,1,92)
  KQRecentFilesAction *projectRecent;
#else
  KRecentFilesAction *projectRecent;
#endif

    /** Holds the upload password. It is not saved, and it is lost after the project is closed. */
  QString passwd;
  /**  */
  bool keepPasswd;

private:

  /** The constructor is privat because we use singleton patter.
   *  If you need the class use Project::ref() for
   *  construction and reference
   */
  Project();
  void loadProjectXML();

  bool createEmptyDom();
  void insertFiles( const KURL& pathURL, const QString& mask );
  void insertFiles( KURL::List files );
  QWizard *wiz;
  QWidgetStack *stack;
  QString currentProjectView;

  ProjectNewGeneral    *png;
  ProjectNewLocal      *pnl;
  ProjectNewWeb       *pnw;
  ProjectNewFinal     *pnf;

  KConfig *config;

  bool m_modified;
  bool storePasswdInFile;

  QBuffer buff;
  QRegExp excludeRx;
  QStringList excludeList;

protected: // Protected attributes
  /** Default DTD for this project. */
  QString m_defaultDTD;
  QString m_defaultEncoding;
  ProjectUrlList m_projectFiles; //stores the last result of fileNameList call

  void openCurrentView();
  /** pre-sets some variables */
  void init();

};

#endif
