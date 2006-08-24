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


QuantaFileManager::QuantaFileManager(QObject *parent) : KDevFileManager(parent), m_baseItem(0L)
{
}

QuantaFileManager::~QuantaFileManager()
{
}
   
KDevProjectItem *QuantaFileManager::import(KDevProjectModel *model, const KUrl &base)
{
  if (m_baseItem)
    model->removeItem(m_baseItem); 
  m_baseItem = new KDevProjectFolderItem(base, 0);
  emit folderAdded(m_baseItem);
  model->appendItem(m_baseItem);  
  return m_baseItem;
}

QList<KDevProjectFolderItem *> QuantaFileManager::parse(KDevProjectFolderItem *base)
{
  const QString prefix = "Item - ";
  KConfig *projectConfig = KDevConfig::localProject();
  QStringList groupList = projectConfig->groupList();
  QMap<KUrl, KDevProjectFolderItem*> folderList;
  KUrl baseUrl = base->url();
  folderList.insert(baseUrl, base);
  KDevProject *prj = project();
  QStack<KUrl> urlStack;
  QString relFileName;
  KUrl url, fileUrl;
  KDevProjectFolderItem *parent;

  //Just for testing with more files
//  KUrl::List urls = ExtFileInfo::allFiles(m_baseItem->url(), "*");
//  foreach( KUrl fileUrl, urls)
  
  foreach (QString group, groupList)
  {
    if (! group.startsWith(prefix))
      continue;

    relFileName = group.mid(prefix.length());
    fileUrl = prj->absoluteUrl(relFileName);
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
      KDevProjectFolderItem *item = new KDevProjectFolderItem(urlStack.top(), parent);
      parent->add(item);
      folderList.insert(urlStack.pop(), item);
      parent = item;
      emit folderAdded(item);
    }
    // add the file
    KDevProjectFileItem *item = new KDevProjectFileItem(fileUrl, parent);
    parent->add(item);
    emit fileAdded(item);
  }
  return QList<KDevProjectFolderItem *>();
}




#include "quantafilemanager.moc"
