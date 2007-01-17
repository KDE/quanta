/**************************************************************************
*   Copyright (C) 2006 by Andras Mantia                                   *
*   amantia@kde.org                                                       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

//own includes
#include "quantafilemanager.h"
#include "extfileinfo.h"
// QT includes

#include <QStack>

//kde includes
#include <kdebug.h>

#include <kdevconfig.h>
#include <kdevproject.h>
#include <kdevprojectmodel.h>


QuantaFileManager::QuantaFileManager(KInstance* instance, QObject *parent) 
  : Koncrete::FileManager(instance, parent), m_baseItem(0L)
{
}

QuantaFileManager::~QuantaFileManager()
{
}
   
Koncrete::ProjectItem *QuantaFileManager::import(Koncrete::ProjectModel *model, const KUrl &base)
{
//   if (m_baseItem)
//     model->removeFolder(m_baseItem); 
  m_baseItem = new Koncrete::ProjectFolderItem(base, 0);
  emit folderAdded(m_baseItem);
//   model->appendItem(m_baseItem);  
  return m_baseItem;
}

QList<Koncrete::ProjectFolderItem *> QuantaFileManager::parse(Koncrete::ProjectFolderItem *base)
{
  const QString prefix = "Item - ";
  KConfig *projectConfig = Koncrete::Config::localProject();
  QStringList groupList = projectConfig->groupList();
  QMap<KUrl, Koncrete::ProjectFolderItem*> folderList;
  KUrl baseUrl = base->url();
  folderList.insert(baseUrl, base);
  Koncrete::Project *prj = project();
  QStack<KUrl> urlStack;
  QString relFileName;
  KUrl url, fileUrl;
  Koncrete::ProjectFolderItem *parent;

  //Just for testing with more files
//  KUrl::List urls = ExtFileInfo::allFiles(m_baseItem->url(), "*");
//  foreach( KUrl fileUrl, urls)
  
  foreach (QString group, groupList)
  {
    if (! group.startsWith(prefix))
      continue;

    relFileName = group.mid(prefix.length());
    fileUrl = prj->urlRelativeToProject(relFileName);
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
      Koncrete::ProjectFolderItem *item = new Koncrete::ProjectFolderItem(urlStack.top(), parent);
      parent->add(item);
      folderList.insert(urlStack.pop(), item);
      parent = item;
      emit folderAdded(item);
    }
    // add the file
    Koncrete::ProjectFileItem *item = new Koncrete::ProjectFileItem(fileUrl, parent);
    parent->add(item);
    emit fileAdded(item);
  }
  return QList<Koncrete::ProjectFolderItem *>();
}




#include "quantafilemanager.moc"
