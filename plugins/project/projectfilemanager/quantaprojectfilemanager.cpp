/***************************************************************************
 *   Copyright (C) 2007 - Andras Mantia <amantia@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 *   See COPYING file that comes with this distribution for details.       *
 ***************************************************************************/
 
#include "quantaprojectfilemanager.h" 

#include <iproject.h>
#include <projectmodel.h>

#include <kgenericfactory.h>

#include <QStack>

typedef KGenericFactory<QuantaProjectFileManager> QuantaProjectFileManagerFactory;
K_EXPORT_COMPONENT_FACTORY( quantaprojectfilemanager, QuantaProjectFileManagerFactory( "quantaprojectfilemanager" ) )

    QuantaProjectFileManager::QuantaProjectFileManager(QObject *parent, const QStringList &args): KDevelop::IPlugin( QuantaProjectFileManagerFactory::componentData(), parent ), KDevelop::IProjectFileManager()
{
  KDEV_USE_EXTENSION_INTERFACE( KDevelop::IProjectFileManager )
  Q_UNUSED(args)
}


QuantaProjectFileManager::~QuantaProjectFileManager()
{
}

KDevelop::ProjectItem* QuantaProjectFileManager::import(KDevelop::IProject *project)
{
  m_project = project;
  m_baseItem = new KDevelop::ProjectItem(project, "base", 0);
  emit folderAdded(m_baseItem);
  return m_baseItem;
}

QList<KDevelop::ProjectFolderItem*> QuantaProjectFileManager::parse(KDevelop::ProjectFolderItem *base)
{
  const QString prefix = "Item - ";
  KSharedConfigPtr projectConfig = m_project->projectConfiguration();
  QStringList groupList = projectConfig->groupList();
  QMap<KUrl, KDevelop::ProjectFolderItem*> folderList;
  KUrl baseUrl = base->url();
  folderList.insert(baseUrl, base);
  QStack<KUrl> urlStack;
  QString relFileName;
  KUrl url, fileUrl;
  KDevelop::ProjectFolderItem *parent;

  //Just for testing with more files
//  KUrl::List urls = ExtFileInfo::allFiles(m_baseItem->url(), "*");
//  foreach( KUrl fileUrl, urls)
  
  foreach (QString group, groupList)
  {
    if (! group.startsWith(prefix))
      continue;

    relFileName = group.mid(prefix.length());
    fileUrl = m_project->urlRelativeToProject(relFileName);
    parent = m_baseItem;
    url = fileUrl.upUrl();
    // search for not yet created folders
    while (url != baseUrl)
    {
      if (folderList.contains(url))
      {
        parent = folderList[url];
        break;
      }
      urlStack.push(url);
      url = url.upUrl();
    }
    // add new folders
    while (! urlStack.isEmpty())
    {
      KDevelop::ProjectFolderItem *item = new KDevelop::ProjectFolderItem(m_project, urlStack.top(), parent);
      parent->add(item);
      folderList.insert(urlStack.pop(), item);
      parent = item;
      emit folderAdded(item);
    }
    // add the file
    KDevelop::ProjectFileItem *item = new KDevelop::ProjectFileItem(m_project, fileUrl, parent);
    parent->add(item);
    emit fileAdded(item);
  }
  return QList<KDevelop::ProjectFolderItem *>();
}

KDevelop::ProjectFolderItem* QuantaProjectFileManager::addFolder(const KUrl &folder, KDevelop::ProjectFolderItem *parent)
{
  //TODO implement it
}

KDevelop::ProjectFileItem* QuantaProjectFileManager::addFile(const KUrl &folder, KDevelop::ProjectFolderItem *parent)
{
  //TODO implement it
}
bool QuantaProjectFileManager::removeFolder(KDevelop::ProjectFolderItem *folder)
{
  //TODO implement it
}
  
bool QuantaProjectFileManager::removeFile(KDevelop::ProjectFileItem *file)
{
  //TODO implement it
}

bool QuantaProjectFileManager::renameFile(KDevelop::ProjectFileItem *oldFile, const KUrl &newFile)
{
  //TODO implement it
}
  
bool QuantaProjectFileManager::renameFolder(KDevelop::ProjectFolderItem *oldFolder, const KUrl &newFolder)
{
  //TODO implement it
}

#include "quantaprojectfilemanager.moc"


//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8#include "quantaprojectfilemanager.h"
