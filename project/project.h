/***********************************************************************
                          project.h  -  description
                             -------------------
    begin                : Thu Mar 16 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2001-2005 by Andras Mantia <amantia@kde.org>
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

#include <qobject.h>

#include "projecturl.h"

class QDom;
class ProjectPrivate;
class ProjectList;
namespace KTextEditor{
  class MarkInterface;
  class ViewCursorInterface;
}
struct EventAction;
typedef  QMap<QString, QValueList<EventAction> > EventActions;

/**project
  *@author Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia & Jens Herden
  */

class KConfig;
class KMainWindow;

struct TeamMember {
  QString name;
  QString email;
  QString nickName;
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
  QString projectName();

  QStringList fileNameList();
  KURL::List files();

  void insertFile( const KURL& nameURL, bool repaint );
  void readConfig(KConfig *);
  /** loads the last project again if reload == true
      but checks in any case if there is a left over project from a crash
  */
  void loadLastProject(bool reload);
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
  QDomDocument *dom();
  QDomDocument *sessionDom();
  QString debuggerClient();
  bool debuggerPersistentBreakpoints();
  bool debuggerPersistentWatches();
  bool keepPasswd;
  QString email();
  KURL templateURL();
  KURL toolbarURL();
  EventActions* events();
  /**
   * Get the nickname of the current user (you)
   * @return the nickname
   */
  QString yourself();
  /**
   * Get the roles of the current user in the form "role:task".
   * Examples: "team leader", "project leader:subprojectname", "task leader:taskname".
   * @return the roles (in lowercase)
   */
  QStringList yourRoles();
  TeamMember teamLeader();
  TeamMember subprojectLeader(const QString &name);
  TeamMember taskLeader(const QString &name);
  QValueList<TeamMember> simpleMembers();
  QString mailingList();
  QValueList<SubProject>* subprojects();
  QStringList tasks();
  QMap<QString, TeamMember> allMembers();
  bool eventsEnabled();
  void setProjectToolbarVisible(bool visible) {m_projectToolbarVisible = visible;}

  /** Called when an url was moved inside the project with drag &drop */
  void urlMoved(const KURL& srcURL, const KURL &destURL);
  /**
       test if application can close

       @return true if close is possible
  */
  bool queryClose();

  /** Saves the bookmarks for the url into the project file */
  void saveBookmarks(const KURL &url, KTextEditor::MarkInterface *markIf);
  /** Loads the bookmarks for the url from the project file and sets them in the view*/
  void loadBookmarks(const KURL &url, KTextEditor::MarkInterface *markIf);
  void saveCursorPosition(const KURL &url, KTextEditor::ViewCursorInterface *viewCursorIf);
  void loadCursorPosition(const KURL &url, KTextEditor::ViewCursorInterface *viewCursorIf);

public slots:

  void slotOpenProject(const KURL&);
  void slotOptions();
  void slotUpload();
  void slotUploadURL(const KURL &urlToUpload, const QString &profileName, bool quickUpload, bool markOnly);

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

  void slotShowProjectToolbar(bool show);

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
  void eventHappened(const QString&, const QString&, const QString& );

private:
  /** The constructor is privat because we use singleton patter.
   *  If you need the class use Project::ref() for
   *  construction and reference
   */
  Project(KMainWindow *parent);

  ProjectPrivate *d;
  bool m_projectToolbarVisible;
};

#endif
