/***************************************************************************
                          projectprivate.h  -  description
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

#ifndef PROJECTPRIVATE_H
#define PROJECTPRIVATE_H

class ProjectList;

class ProjectPrivate
            : public QObject
{
   Q_OBJECT
public:
  ProjectPrivate(Project *p);
            
  ~ProjectPrivate() {}

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
  QString author;
  KRecentFilesAction *projectRecent;
 
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
  KMainWindow *m_parent;
  QString m_debuggerClientEdit;
  QMap<QString, QString> m_passwdList;
  ProjectList m_projectFiles; ///< the list with the _absolute_ URL's of the project  
  KDirWatch *m_dirWatch; ///< watches some project directories for modification
  KURL toolbarURL;
  KURL templateURL;
  QString email;
  QDomDocument dom;
  QString debuggerClient;

  KAction
    *closeprjAction, *insertFileAction, *insertDirAction,
    *uploadProjectAction,  *rescanPrjDirAction,
    *projectOptionAction, *saveAsProjectTemplateAction,
    *saveSelectionAsProjectTemplateAction,
    *savePrjViewAction, *saveAsPrjViewAction;

   KSelectAction *openPrjViewAction, *deletePrjViewAction;

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
  
  void slotSelectProjectType(const QString &title);

  bool createEmptyDom();

  /** load project from file: name */
  void slotLoadProject(const KURL &a_url);
  
  void slotDeleteCopytoDlg(CopyTo *dlg);

  void getStatusFromTree();

public slots:
  
  void slotDebuggerOptions();

  void slotDebuggerChanged(const QString &debugger);

  /** create new project */
  void slotNewProject();

  /** open project file */
  void slotOpenProject();

  /** close project and edited files */
  void slotCloseProject();

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
 
signals:  
  void setLocalFiles( bool );

private:
  QStringList treeStatusFromXML();
};


#endif
