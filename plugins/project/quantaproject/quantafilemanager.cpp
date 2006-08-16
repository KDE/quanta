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
  model->appendItem(m_baseItem);
  return m_baseItem;
}

QList<KDevProjectFolderItem *> QuantaFileManager::parse(KDevProjectFolderItem *base)
{
  KConfig *projectConfig = KDevConfig::localProject();
  QStringList fileList = projectConfig->groupList();
  
  QStringList::Iterator it = fileList.begin();
  while (it != fileList.end())
  {
    if (!(*it).startsWith("Item - "))
    {
      it = fileList.erase(it);
    } else
      ++it;
  }

  KUrl url;
  KDevProject *prj = project();
  QString relFileName;
  QMap<QString, KDevProjectFolderItem*> folderList;
  folderList.insert(base->url().path(KUrl::AddTrailingSlash), base);
  for (it = fileList.begin(); it != fileList.end(); ++it)
  {
    relFileName = (*it).mid(7);
    url = prj->absoluteUrl(*it);
    QStringList dirPartList = relFileName.split('/', QString::SkipEmptyParts);
    QString directory = base->url().path(KUrl::AddTrailingSlash);
    QStringList::Iterator lastIt = dirPartList.end();
    --lastIt;
    for (QStringList::Iterator it2 = dirPartList.begin(); it2 != dirPartList.end(); ++it2)
    {
      QString parentDir = directory;
      directory.append(*it2);
      if (it2 != lastIt) //it means we are adding the directories for the file
      {
        if (!folderList.contains(directory))
        {
          KDevProjectFolderItem *parent = folderList[parentDir];
          KDevProjectFolderItem *item = new KDevProjectFolderItem(directory, parent);
          parent->add(item);
          folderList.insert(directory +"/", item);
        }
        directory.append('/');
      } else //it means it2 points to the name of the file
      {
         KDevProjectFolderItem *parent = folderList[parentDir];
         KDevProjectFileItem *item = new KDevProjectFileItem(directory, parent);
         parent->add(item);
      }
    }
  }

  return QList<KDevProjectFolderItem *>();
}




#include "quantafilemanager.moc"
