/***************************************************************************
                          projectprivate.h  -  description
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

#ifndef PROJECTPRIVATE_H
#define PROJECTPRIVATE_H

//qt includes
#include <qmap.h>
#include <qregexp.h>
#include <qguardedptr.h>

//app includes
#include "project.h"
#include "projectlist.h"
#include "projectupload.h"

class QWidgetStack;

class KAction;
class KActionCollection;
class KConfig;
class KDirWatch;
class KRecentFilesAction;
class KSelectAction;
class KTempFile;

class CopyTo;
class ProjectNewGeneral;
class ProjectNewLocal;
class ProjectNewWeb;
class ProjectNewFinal;
class ProjectList;
struct EventAction;
typedef QMap<QString, QValueList<EventAction> > EventActions;

class ProjectPrivate
            : public QObject
{
   Q_OBJECT
public:
  ProjectPrivate(Project *p);

  ~ProjectPrivate();

  Project *parent;
  /** Point to the .webprj file */
  KURL projectURL;
  QString projectName;
  /** Points to the directory of the .webprj file */
  KURL baseURL;
/** Points to the document root inside the project. Item under this folder
are treated as the actual documents belonging to the site. They are automatically selected
for upload, searching in project default to this directory, etc. Items outside of the
document root are treated as external, control files and they are by default not selected for
upload.*/

  KURL previewPrefix;
  bool usePreviewPrefix;
  bool m_persistentBookmarks;
  QString author;
  KRecentFilesAction *m_projectRecent;

  ProjectNewGeneral    *png;
  ProjectNewLocal      *pnl;
  ProjectNewWeb       *pnw;
  ProjectNewFinal     *pnf;

  QWidgetStack *stack;
  QString currentProjectView;

  KConfig *config;
  bool m_modified;
  bool m_excludeCvsignore;
  /** Default DTD for this project. */
  QString m_defaultDTD;
  QString m_defaultEncoding;
  QRegExp excludeRx;
  QStringList excludeList;
  KMainWindow *m_mainWindow;
  QString m_debuggerClientEdit;
  QMap<QString, QString> m_passwdList;
  ProjectList m_projectFiles; ///< the list with the _absolute_ URL's of the project
  KDirWatch *m_dirWatch; ///< watches some project directories for modification
  KURL toolbarURL;
  KURL templateURL;
  QString email;
  QDomDocument dom;
  QDomDocument m_sessionDom;
  QString debuggerClient;
  bool m_debuggerPersistentBreakpoints;
  bool m_debuggerPersistentWatches;
  QGuardedPtr<ProjectUpload> m_uploadDialog;

  TeamMember m_teamLeader;
  QMap<QString, TeamMember> m_subprojectLeaders;
  QMap<QString, TeamMember> m_taskLeaders;
  QValueList<TeamMember> m_simpleMembers;
  QValueList<SubProject> m_subprojects;
  QString m_yourself;
  QString m_mailingList;
  bool m_showUploadTreeviews;
  bool m_eventsEnabled;

  KAction
    *closeprjAction, *insertFileAction, *insertDirAction,
    *uploadProjectAction,  *rescanPrjDirAction,
    *projectOptionAction, *saveAsProjectTemplateAction,
    *saveSelectionAsProjectTemplateAction,
    *savePrjViewAction, *saveAsPrjViewAction;

   KSelectAction *openPrjViewAction, *deletePrjViewAction;
  /** the filename of the local webprj file after download */
  QString m_tmpProjectFile;
  QString m_tmpSessionFile;
  /** used for creating a new project */
  KTempFile *tempFile;
  KTempFile *sessionTempFile;
  EventActions *m_events;

  /** setup of the actions */
  void initActions(KActionCollection *ac);

  void adjustActions();

  void adjustViewActions();

  QStringList projectViewList();

  void init();

  void openCurrentView();
 /** insert files */
  void insertFiles( KURL::List files );

  /** insert files from dir recursive */
  void insertFiles(const KURL& pathURL, const QString& mask );

  void loadProjectXML();

  void slotAcceptCreateProject();

  bool createEmptyDom();

  /** load project file
      @param url the url of the project file
   */
  bool loadProject(const KURL &url);

  /** load project from temp file
      @param url the url of the project file
      @param tempFile the path of the already downloaded temp file
      @param sessionTempFile the path of the downloaded project session file. Empty if no such file was downloaded
   */
  void loadProjectFromTemp(const KURL &url, const QString &tempFile, const QString &sessionTempFile);


  void getStatusFromTree();

  /** save project file to local file @ref tmpFileName
      @return false if there was nothing to save or an error occured during save
  */
  bool saveProject();

  /**
      adds information about the current open project into config
   */
  void writeConfig();

  /**
      removes the project from the list of open projects in config

      @param the url string of the project to remove
   */
  void removeFromConfig(const QString & urlStr);

  /**
      test if a project is in the list of open projects in config

      @param the url string of the project to test
   */
  bool projectAlreadyOpen(const QString & urlStr);

  /** uploads the project file
      @return false if there was nothing to upload or an error occured during upload
  */
  bool uploadProjectFile();

public slots:

  void slotDebuggerOptions();

  void slotDebuggerChanged(const QString &debugger);

  /** create new project */
  void slotNewProject();

  /** open project file */
  void slotOpenProject();

  /** iniate closing of the project and the edited files */
  void slotCloseProject();

  /** Proceed with project closing.*/
  void slotProceedWithCloseProject(bool success);

  void slotAddDirectory();

  void slotAddFiles();

  /** Deletes a project view */
  void slotDeleteProjectView(const QString &view);

  /** Saves a project view (group of files & toolbars) asking for a name. */
  void slotSaveAsProjectView(bool askForName = true);

  /** Saves a project view (group of files & toolbars) without asking for a name. */
  void slotSaveProjectView();

  /** Opens a project view (toolbars & files). */
  void slotOpenProjectView(const QString &view);

  /** for the new project wizard */
  void slotSelectProjectType(const QString &title);

  void slotDeleteCopytoDlg(CopyTo *dlg);
  
private slots:
  void slotNewProjectHelpClicked();

signals:
  void setLocalFiles(bool);
  void eventHappened(const QString&, const QString&, const QString& );

private:
  QStringList treeStatusFromXML();
  QString m_wizTitle ;
  bool m_createSessionDom;
};


#endif
