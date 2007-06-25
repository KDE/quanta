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
#include "extfileinfo.h"

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
  //This method parses the whole project file for items belonging to the project instead of parsing
  //the content of only one directory as it seems the framework waits. So simply return nothing when
  //the request is for a folder which is not the base item.
  if (base != m_baseItem)
    return QList<KDevelop::ProjectFolderItem*>();
  KUrl url = base->url();
  QString name = base->project()->name();
  name.replace(".kdev4", ".webprj");
  url.adjustPath(KUrl::AddTrailingSlash);
  url.setFileName(name);
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
      KUrl baseUrl = url;
      baseUrl.setPath(url.directory());
      if (baseUrl.isLocalFile())
      {
        QDir dir(baseUrl.path());
        baseUrl.setPath(dir.canonicalPath());
        baseUrl.adjustPath(KUrl::RemoveTrailingSlash);
      }
      dom.setContent(&f);   
      //kDebug(24000) << "Project content: " << dom.toString() << endl;
      f.close();    
      
      QMap<KUrl, KDevelop::ProjectFolderItem*> folderList;
      url = baseUrl;
      url.adjustPath(KUrl::AddTrailingSlash);        
      folderList.insert(url, base);
      QStack<KUrl> urlStack;
      KDevelop::ProjectFolderItem *parent;
      //read the items from the dom 
      QString tmpString;
      QDomNodeList nl = dom.firstChild().firstChild().childNodes();
      QDomElement el;
      uint nlCount = nl.count();
      for ( uint i = 0; i < nlCount; i++ )
      {
        KUrl url = baseUrl;
        el = nl.item(i).toElement();
        tmpString = el.attribute("url");
        if (!tmpString.isEmpty())
        {
          ExtFileInfo::setUrl(url,tmpString);
        } else
          continue;
//         kDebug(24000) << "tmpstring " << tmpString <<" url " << url << " baseUrl " << baseUrl << endl;
        url = ExtFileInfo::toAbsolute(url, baseUrl);
        if ( el.nodeName() == "item" )
        {
//           if (excludeRx.exactMatch(path) || find(url.url(-1)))
//           {
//             el.parentNode().removeChild(el);
//             modified = true;
//             i--;
//           } else
          {
            bool docFolder = (el.attribute("documentFolder", "false") == "true");
            int uploadStatus = el.attribute("uploadstatus", "-1").toInt();
            if (uploadStatus == -1)
              el.setAttribute("uploadstatus", 1);
        //remove non-existent local files
            bool skipItem = false;
            if (url.isLocalFile())
            {
              QFileInfo fi(url.path());
              if ( !fi.exists() )
              {
                el.parentNode().removeChild( el );
                i--;
                skipItem = true;
              } 
            } 
            if (!skipItem)
            {
              KUrl fileUrl = url;
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
              while (!urlStack.isEmpty())
              {
//                 kDebug(24000) << "Adding folder: " << urlStack.top()<< endl;
                KDevelop::ProjectFolderItem *item = new KDevelop::ProjectFolderItem(m_project, urlStack.top(), parent);
                folderList.insert(urlStack.pop(), item);
                parent = item;
              }
              // add the file
//               kDebug(24000) << "Adding file: " << fileUrl<< endl;
              KDevelop::ProjectFileItem *item = new KDevelop::ProjectFileItem(m_project, fileUrl, parent);
            }
          }
        }
      }  
      //the base should not be returned as it was already created in the import method
      url = baseUrl;
      url.adjustPath(KUrl::AddTrailingSlash);       
      folderList.remove(url);
      return folderList.values();
    }
    KMessageBox::error(mainWindow, i18n("<qt>Cannot open the downloaded project file.</qt>"));
    return QList<KDevelop::ProjectFolderItem*>();
  } else      
  {
    KMessageBox::error(mainWindow, i18n("<qt>Cannot access the project file <b>%1</b>.</qt>").arg(url.pathOrUrl()));
    return QList<KDevelop::ProjectFolderItem*>();
  }
}

KDevelop::ProjectFolderItem* QuantaProjectFileManager::addFolder(const KUrl &folder, KDevelop::ProjectFolderItem *parent)
{
  //TODO implement it
    return 0;
}

KDevelop::ProjectFileItem* QuantaProjectFileManager::addFile(const KUrl &folder, KDevelop::ProjectFolderItem *parent)
{
  //TODO implement it
    return 0;
}
bool QuantaProjectFileManager::removeFolder(KDevelop::ProjectFolderItem *folder)
{
  //TODO implement it
    return false;
}
  
bool QuantaProjectFileManager::removeFile(KDevelop::ProjectFileItem *file)
{
  //TODO implement it
    return false;
}

bool QuantaProjectFileManager::renameFile(KDevelop::ProjectFileItem *oldFile, const KUrl &newFile)
{
  //TODO implement it
    return false;
}
  
bool QuantaProjectFileManager::renameFolder(KDevelop::ProjectFolderItem *oldFolder, const KUrl &newFolder)
{
  //TODO implement it
    return false;
}

#include "quantaprojectfilemanager.moc"


//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8#include "quantaprojectfilemanager.h"
