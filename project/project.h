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


/**project
  *@author Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia & Jens Herden
  */

class KConfig;
class KMainWindow;

/** Describes an event action. */
struct EventAction {
 /** Possible event types */
  enum Types {
     Internal = 0,
     External = 1
  };
  /** The type of the event. See @ref Types */
  uint type;
  /** the name of the action to be executed. In case of external events
     this is the name of the script, in case of internal events it can be one of the
     following: "email"
  */
  QString action;
  /** The arguments for the event action. It is different for each action.
  */
  QStringList arguments;
};

/** The configured events. The key is the event name, the data is the event description.
For example: events["before_save"] points to the event data that needs to be used
before a file is saved. Possible key names are: before_save, after_save, after_open,
after_project_open, after_project_save, before_upload, after_upload, after_project_add,
after_project_remove, after_commit
*/
typedef  QMap<QString, EventAction> EventActions;


struct TeamMember {
  QString name;
  QString email;
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
  void readLastConfig(KConfig *c=0);
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

  /** save project file */
  bool slotSaveProject();

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

  EventActions m_events;
};

#endif
