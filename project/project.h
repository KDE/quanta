/***********************************************************************
                          project.h  -  description
                             -------------------
    begin                : Thu Mar 16 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2001-2004 by Andras Mantia <amantia@kde.org>
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

#include "projecturl.h"

class QDom;
class ProjectPrivate;
class ProjectList;
struct EventAction;
typedef  QMap<QString, EventAction> EventActions;

/**project
  *@author Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia & Jens Herden
  */

class KConfig;
class KMainWindow;

struct TeamMember {
  QString name;
  QString email;
  QString task;
};

struct SubProject {
  QString name;
  QString location;
};

class Project : public QObject  {
   Q_OBJECT
   friend class ProjectPrivate;  // need this because I use the signals

public:

  /**
   *  since this class is a singleton you must use this function to access it
   *
   *  the parameter is only used at the first call to create the class
   *
   */
  static Project* const ref(KMainWindow *parent = 0L)
  {
    static Project *m_ref;
    if (!m_ref) m_ref = new Project(parent);
    return m_ref;
  }

  ~Project();

  bool hasProject();

  QStringList fileNameList();

  void insertFile( const KURL& nameURL, bool repaint );
  void readConfig(KConfig *);
  /** loads the last project again if reload == true
      but checks in any case if there is a left over project from a crash
  */
  void loadLastProject(bool reload);
  void writeConfig(KConfig *);
  /** Returns the relative url with the prefix inserted. */
  KURL urlWithPrefix(const KURL& url);
  bool contains(const KURL &url);
  /** Read property of QString defaultDTD. */
  const QString& defaultDTD();
  const QString& defaultEncoding();

  /** Returns the project's base URL if it exists,
   * the HOME dir if there is no project and no opened document
   * (or the current  opened document was not saved yet),
   * and the base URL of the opened document, if it is saved somewhere.
   */
  KURL projectBaseURL();
  KURL documentFolderForURL(const KURL &url);

  /** Saves the password for entry into a list. Stores on disc if store == true */
  void savePassword(const QString& entry, const QString& passwd, bool store);
  /** Returns the saved password for entry */
  QString password(const QString &entry);
  bool passwordSaved(const QString &entry);
  void updateTimeStamp(const KURL& url, int modifiedTime);
  QDomDocument *dom();
  QString debuggerClient();
  bool keepPasswd;
  QString email();
  KURL templateURL();
  KURL toolbarURL();
  EventActions* events();
  TeamMember teamLeader();
  TeamMember subprojectLeader(const QString &name);
  TeamMember taskLeader(const QString &name);
  QValueList<TeamMember> simpleMembers();
  QString mailingList();
  QValueList<SubProject>* subprojects();
  QMap<QString, TeamMember> allMembers();

  /** uploads the project file
      @return false if there was nothing to upload or an error occured during upload
  */
  bool uploadProjectFile();

public slots:

  void slotOpenProject(const KURL&);
  void slotOptions();
  void slotUpload();
  void slotUploadURL(const KURL &urlToUpload);

  void slotAddDirectory(const KURL& dirURL, bool showDlg = true);
  void slotInsertFile(const KURL& url);
  void slotInsertFilesAfterCopying(const KURL::List& a_url);

  /** if somewhere something was renamed */
  void slotRenamed(const KURL& oldURL, const KURL& newURL);
  void slotRemove(const KURL& urlToRemove);


  void slotGetMessages(const QString&);

  void slotRescanPrjDir();
  void slotFileDescChanged(const KURL& url, const QString& desc);
  void slotUploadStatusChanged(const KURL& url, int status);
  void slotChangeDocumentFolderStatus(const KURL& url, bool status);

  void slotReloadProjectDocs();

  void setModified(bool b = true);

signals:

  void openFile( const KURL&, const QString& );
  void openFiles( const KURL::List&, const QString& );
  void closeFile( const KURL&);
  void closeFiles();

  void reloadTree(ProjectList *, bool, const QStringList &);

  void messages(const QString& );
  void enableMessageWidget();

  void newStatus();
  void statusMsg(const QString &);
  /** No descriptions */
  void newProjectLoaded(const QString &, const KURL &, const KURL &);
  void reloadProjectDocs();
  void hideSplash();
  void addProjectDoc(const KURL &);
  void getUserToolbarFiles(KURL::List *);
  void loadToolbarFile(const KURL &);
  /** ask for the tree status for saving in project */
  void getTreeStatus(QStringList *);

private:
  /** The constructor is privat because we use singleton patter.
   *  If you need the class use Project::ref() for
   *  construction and reference
   */
  Project(KMainWindow *parent);

  ProjectPrivate *d;
};

#endif
