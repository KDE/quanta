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
#include <kparts/mainwindow.h>
#include <kpluginfactory.h>
#include <kpluginloader.h>
#include <klocale.h>

#include <QDomDocument>
#include <QDir>
#include <QFile>
#include <QStack>

K_PLUGIN_FACTORY(QuantaProjectFileManagerFactory, registerPlugin<QuantaProjectFileManager>();)
K_EXPORT_PLUGIN(QuantaProjectFileManagerFactory( "quantaprojectfilemanager" ))


    QuantaProjectFileManager::QuantaProjectFileManager(QObject *parent, const QVariantList &args): KDevelop::IPlugin( QuantaProjectFileManagerFactory::componentData(), parent ), KDevelop::IProjectFileManager()
{
  kDebug(24000) << "Creating QuantaProjectFileManager Part";
  KDEV_USE_EXTENSION_INTERFACE( KDevelop::IProjectFileManager )
  Q_UNUSED(args)
}


QuantaProjectFileManager::~QuantaProjectFileManager()
{
}

KDevelop::ProjectFolderItem* QuantaProjectFileManager::import(KDevelop::IProject *project)
{
  KUrl url = project->folder();
  QString name = project->name();
  //TODO: this is temporary implementation, so the Quanta4 project can use any Quanta3 project file
  name.replace(".kdev4", ".webprj");
  url.adjustPath(KUrl::AddTrailingSlash);
  url.setFileName(name);
  kDebug(24000) << "Parsing project file: " << url;
  QString projectTmpFile;
  KParts::MainWindow *mainWindow = core()->uiController()->activeMainWindow();
  QDomDocument dom;

  // test if url is writeable and download to local file
  if (KIO::NetAccess::exists(url, KIO::NetAccess::DestinationSide, mainWindow) &&
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
      f.close();

      url = baseUrl;
      url.adjustPath(KUrl::AddTrailingSlash);
      m_projectFolders[url] = QStringList();
      QStack<KUrl> urlStack;
      KUrl parent;
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
                  if (m_projectFolders.contains(url))
                  {
                    parent = url;
                    break;
                  }
                  urlStack.push(url);
                  url = url.upUrl();
                }
              // add new folders
                while (!urlStack.isEmpty())
                {
                  url = urlStack.pop();
                  if (!m_projectFiles.contains(url))
                  {
                    kDebug(24000) << "Adding folder: " << url;
                    m_projectFiles[url] = QStringList();
                    m_projectFolders[parent] << url;
                  }
                  parent = url;
                }
              
                if ( fileUrl.url().endsWith("/") )
                {
                  // add the folder
                  if (!m_projectFiles.contains(fileUrl))
                  {
                    kDebug(24000) << "Adding folder: " << fileUrl;
                    m_projectFiles[fileUrl] = QStringList();
                    m_projectFolders[parent] << fileUrl;
                  }
                }
                else
                {
                  // add the file
                  kDebug(24000) << "Adding file: " << fileUrl;
                  m_projectFiles[parent] << fileUrl;
                }
              }
            }
          }
      }
    } else
      KMessageBox::error(mainWindow, i18n("<qt>Cannot open the downloaded project file.</qt>"));
  } else
  {
    KMessageBox::error(mainWindow, i18n("<qt>Cannot access the project file <b>%1</b>.</qt>").arg(url.pathOrUrl()));
  }

  //create the base item
  KDevelop::ProjectFolderItem *baseItem = new KDevelop::ProjectFolderItem(project, project->folder().pathOrUrl(), 0L);
  baseItem->setProjectRoot( true );
  return baseItem;
}

QList<KDevelop::ProjectFolderItem*> QuantaProjectFileManager::parse(KDevelop::ProjectFolderItem *base)
{
  QList<KDevelop::ProjectFolderItem *> subFolders;
  KUrl url = base->url();
  url.adjustPath(KUrl::AddTrailingSlash);
  KDevelop::IProject *project = base->project();
//   kDebug(24000) << "Request parse for : " << url;

  KUrl::List::ConstIterator itBegin = m_projectFolders[url].constBegin();
  KUrl::List::ConstIterator itEnd = m_projectFolders[url].constEnd();
  for (KUrl::List::ConstIterator it = itBegin; it != itEnd; ++it)
  {
    subFolders.append(new KDevelop::ProjectFolderItem(project, *it, base));
  }

  itBegin = m_projectFiles[url].constBegin();
  itEnd = m_projectFiles[url].constEnd();
  for (KUrl::List::ConstIterator it = itBegin; it != itEnd; ++it)
  {
    new KDevelop::ProjectFileItem(project, *it, base);
  }

  return subFolders;
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
