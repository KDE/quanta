/***************************************************************************
 *   Copyright (C) 2007 - Andras Mantia <amantia@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 *   See COPYING file that comes with this distribution for details.       *
 **************************************************************************/
 
#ifndef QUANTAPROJECTFILEMANAGER_H
#define QUANTAPROJECTFILEMANAGER_H

#include <interfaces/iplugin.h>
#include <project/interfaces/iprojectfilemanager.h>

namespace KDevelop
{
  class ProjectBaseItem;
  class ProjectFolderItem;
  class ProjectFileItem;
  class ProjectItem;
}

class KDialogBase;

/**
 * @short This class operates on the Quanta specific project file.
 * 
 * @author Andras Mantia <amantia@kde.org>
*/
class QuantaProjectFileManager : public KDevelop::IPlugin, public KDevelop::IProjectFileManager
{
  Q_OBJECT
  Q_INTERFACES( KDevelop::IProjectFileManager )
public:
  explicit QuantaProjectFileManager( QObject *parent = 0,
                            const QStringList &args = QStringList());

  virtual  ~QuantaProjectFileManager();
  
  //Reimplemented methods from IProjectFileManager
  virtual QList<KDevelop::ProjectFolderItem*> parse(KDevelop::ProjectFolderItem *dom);
  /**
   * Create the root project item for a project.
   * @param project the project
   * @return the root item
   */
  virtual KDevelop::ProjectItem* import(KDevelop::IProject *project);
  virtual KDevelop::ProjectFolderItem* addFolder(const KUrl &folder, KDevelop::ProjectFolderItem *parent);
  virtual KDevelop::ProjectFileItem* addFile(const KUrl &folder, KDevelop::ProjectFolderItem *parent);
  virtual bool removeFolder(KDevelop::ProjectFolderItem *folder);
  virtual bool removeFile(KDevelop::ProjectFileItem *file);
  virtual bool renameFile(KDevelop::ProjectFileItem *oldFile, const KUrl &newFile);
  virtual bool renameFolder(KDevelop::ProjectFolderItem *oldFolder, const KUrl &newFolder);
  
Q_SIGNALS:
  //Declare again the signals as IProjectFileManager doesn't inherit from QObject
  void projectItemConfigWidget(const QList<KDevelop::ProjectBaseItem*> &dom, KDialogBase *dialog);
  void folderAdded( KDevelop::ProjectFolderItem* folder );
  void folderRemoved( KDevelop::ProjectFolderItem* folder );
  void folderRenamed( const KUrl& oldFolder, KDevelop::ProjectFolderItem* newFolder );
  void fileAdded(KDevelop::ProjectFileItem* file);
  void fileRemoved(KDevelop::ProjectFileItem* file);
  void fileRenamed(const KUrl& oldFile, KDevelop::ProjectFileItem* newFile);
  
private:
  KDevelop::ProjectItem  *m_baseItem;
  KDevelop::IProject *m_project;
};

#endif

//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
