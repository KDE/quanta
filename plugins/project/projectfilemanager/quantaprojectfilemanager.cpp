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

#include <icore.h>
#include <iproject.h>
#include <iuicontroller.h>
#include <projectmodel.h>

#include <kdebug.h>
#include <kio/netaccess.h>
#include <kmessagebox.h>
#include <kgenericfactory.h>
#include <kparts/mainwindow.h>

#include <QDomDocument>
#include <QDir>
#include <QFile>
#include <QStack>

typedef KGenericFactory<QuantaProjectFileManager> QuantaProjectFileManagerFactory;
K_EXPORT_COMPONENT_FACTORY( quantaprojectfilemanager, QuantaProjectFileManagerFactory( "quantaprojectfilemanager" ) )

    QuantaProjectFileManager::QuantaProjectFileManager(QObject *parent, const QStringList &args): KDevelop::IPlugin( QuantaProjectFileManagerFactory::componentData(), parent ), KDevelop::IProjectFileManager()
{
  kDebug(24000) << "Creating QuantaProjectFileManager Part" << endl;
  KDEV_USE_EXTENSION_INTERFACE( KDevelop::IProjectFileManager )
  Q_UNUSED(args)
}


QuantaProjectFileManager::~QuantaProjectFileManager()
{
}

KDevelop::ProjectItem* QuantaProjectFileManager::import(KDevelop::IProject *project)
{
  m_project = project;
  m_baseItem = new KDevelop::ProjectItem(project, project->folder().pathOrUrl(), 0L);
  kDebug(24000) << "Importing project url: " << project->folder() <<  " base item: " << m_baseItem->url() << endl;
  emit folderAdded(m_baseItem);
  return m_baseItem;
}

QList<KDevelop::ProjectFolderItem*> QuantaProjectFileManager::parse(KDevelop::ProjectFolderItem *base)
{
  KUrl url = base->url();
  url.adjustPath(KUrl::AddTrailingSlash);
  url.setFileName(base->project()->name() + ".webprj");
  kDebug(24000) << "Parsing project file: " << url << endl;
  QString projectTmpFile;
  KParts::MainWindow *mainWindow = core()->uiController()->activeMainWindow();
  QDomDocument dom;

  // test if url is writeable and download to local file
  if (KIO::NetAccess::exists(url, false, mainWindow) &&
      KIO::NetAccess::download(url, projectTmpFile, mainWindow))
  {
    QFile f(projectTmpFile);
    if (f.open(IO_ReadOnly))
    {
      KUrl baseURL = url;
      baseURL.setPath(url.directory());
      if (baseURL.isLocalFile())
      {
        QDir dir(baseURL.path());
        baseURL.setPath(dir.canonicalPath());
        baseURL.adjustPath(KUrl::RemoveTrailingSlash);
      }
      dom.setContent(&f);   
      kDebug(24000) << "Project content: " << dom.toString() << endl;
      f.close();     
    }
  } else      
  {
    KMessageBox::error(mainWindow, i18n("<qt>Cannot access the project file <b>%1</b>.</qt>").arg(url.pathOrUrl()));
    return QList<KDevelop::ProjectFolderItem*>();
  }
  
/*  
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
  return QList<KDevelop::ProjectFolderItem *>();*/
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
