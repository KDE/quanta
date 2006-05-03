/***************************************************************************
    begin                : Jun 21 2004
    copyright            : (C) 2004 - 2005 Jens Herden <jhe@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include "extfileinfo.h"
#include "quantanetaccess.h"
#include "quantaprojectif.h"

#include <qstring.h>
#include <qwidget.h>
#include <qdir.h>

#include <kio/netaccess.h>
#include <kurl.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kfileitem.h>
#include <kmainwindow.h>
#include <kstringhandler.h>

//kdevelop includes
#include <interfaces/kdevmainwindow.h>
#include <interfaces/kdevplugin.h>


bool QuantaNetAccess::upload(const QString& src, const KUrl& target, KDevPlugin* plugin, bool confirm)
{
  QWidget *window = plugin->mainWindow()->main();
  bool ok = KIO::NetAccess::upload(src, target, window);
  if (ok)
  {
    checkProjectInsert(src, target, plugin, confirm);
  } else
  {
    if (confirm)
      errorMsg(window);
  }
  return ok;
}


bool QuantaNetAccess::file_copy( const KUrl& src, const KUrl& target, int permissions,
                                 bool overwrite, bool resume, KDevPlugin* plugin, bool confirm)
{
  QWidget *window = plugin->mainWindow()->main();
  bool ok;
  if (src == target)
    ok = true;
  else
    ok = KIO::NetAccess::file_copy( src, target, permissions, overwrite, resume, window );
  if (ok)
  {
    checkProjectInsert(src, target, plugin, confirm);
  } else
  {
    if (confirm)
      errorMsg(window);
  }
  return ok;
}


bool QuantaNetAccess::file_move( const KUrl& src, const KUrl& target, int permissions,
                                 bool overwrite, bool resume, KDevPlugin* plugin, bool confirm)
{
  QWidget *window = 0L;
  QuantaProjectIf *project = 0L;
  if (plugin)
  {
    window = plugin->mainWindow()->main();
    project = dynamic_cast<QuantaProjectIf*>(plugin->project());
  }
  // don't ask if move is inside of the project
  bool oldConfirm = confirm;
  if ( project && project->projectBase().isParentOf(src) &&
       project->projectBase().isParentOf(target) )
  {
    confirm = false;
  }
  if ( !checkProjectRemove(src, plugin, confirm) )
  {
    return false;
  }
  bool ok;
  if (src == target)
    ok = true;
  else
    ok = KIO::NetAccess::file_move( src, target, permissions, overwrite, resume, window );
  if (ok)
  {
     checkProjectInsert(src, target, plugin, confirm);
  } else
  {
    if (oldConfirm)
      errorMsg(window);
  }
  return ok;
}


bool QuantaNetAccess::dircopy( const KUrl::List & srcList, const KUrl & target, KDevPlugin* plugin, bool confirm )
{
  QWidget *window = 0L;
  if (plugin)
  {
    window = plugin->mainWindow()->main();
  }
  if (!ExtFileInfo::exists(target))
  {
    if (!mkdir(target, plugin, -1, confirm))
    {
      return false;
    }
  }
  bool ok = KIO::NetAccess::dircopy( srcList, target, window );
  if (ok)
  {
    KUrl url;
    KUrl::List::ConstIterator end(srcList.end());
    for ( KUrl::List::ConstIterator it = srcList.begin(); it != end; ++it )
    {
      url = target;
      url.adjustPath(KUrl::AddTrailingSlash);
      url.setFileName((*it).fileName());
      checkProjectInsert(*it, url, plugin, confirm);
    }
  } else
  {
    if (confirm)
      errorMsg(window);
  }
  return ok;
}


bool QuantaNetAccess::move( const KUrl::List& srcList, const KUrl& target, KDevPlugin* plugin, bool confirm )
{
  QWidget *window = 0L;
  QuantaProjectIf *project = 0L;
  if (plugin)
  {
    window = plugin->mainWindow()->main();
    project = dynamic_cast<QuantaProjectIf*>(plugin->project());
  }
  KUrl targetURL = adjustURL(target);
  if (project)
  {
    bool oldConfirm = confirm;
    bool moveInsideProject = false;
    bool targetInProject = project->projectBase().isParentOf(targetURL);
    KUrl url;
    KUrl baseURL = project->projectBase();
    // first we ask about the URLs in the list without actually removing them from the project
    KUrl::List::ConstIterator end(srcList.end());
    for ( KUrl::List::ConstIterator it = srcList.begin(); it != end; ++it )
    {
      //don't ask if move is inside of the project
      url = adjustURL(*it);
      if (targetInProject && baseURL.isParentOf(url) )
      {
        confirm = false;
        moveInsideProject = true;
      }
      if ( !checkProjectRemove(*it, plugin, confirm, false))
      {
        return false;
        confirm = oldConfirm;
      }
    }
    // all URLs are confirmed, we remove them from the project
    end = srcList.end();
    for ( KUrl::List::ConstIterator it = srcList.begin(); it != end; ++it )
    {
      if ( baseURL.isParentOf(*it) )
      {
        url = KUrl::relativeURL(baseURL, *it);
        project->removeFile(url.path());
      }
    }
  }
  bool ok = KIO::NetAccess::move( srcList, targetURL, window );
  if (ok)
  {
    if (project)
    {
      KUrl url;
      KUrl::List::ConstIterator end(srcList.end());
      for ( KUrl::List::ConstIterator it = srcList.begin(); it != end; ++it )
      {
        url = target;
        url.adjustPath(KUrl::AddTrailingSlash);
        url.setFileName((*it).fileName());
        checkProjectInsert(*it, url, plugin, confirm);
        //FIXME project->urlMoved(*it, url); //emits the "after_file_move" event
      }
    }
  } else
  {
    if (confirm)
      errorMsg(window);
  }
  return ok;
}


bool QuantaNetAccess::del( const KUrl & url, KDevPlugin* plugin, QWidget* window, bool confirm )
{
  if ( !checkProjectDel(url, plugin, window, confirm)) {
    return false;
  }
  bool ok = KIO::NetAccess::del( url, window );
  if (!ok && confirm)
  {
      errorMsg(window);
  }
  return ok;
}


bool QuantaNetAccess::mkdir( const KUrl & path, KDevPlugin* plugin, int permissions, bool confirm )
{
  QWidget *window = 0L;
  if (plugin)
  {
    window = plugin->mainWindow()->main();
  }
  KUrl url = path;
  if (ExtFileInfo::exists(url))
  {
    checkProjectInsert(url, url, plugin, confirm);
    return true;
  }
  url.setPath("/");
  if (!ExtFileInfo::exists(url))
  {
    if (confirm)
      errorMsg(window);
    return false; //the root is not accessible, possible wrong username/password supplied
  }
  url = path;

  bool firstConfirm = confirm;
  // move up the path until I find an existing folder
  QStringList stack;
  do {
    stack.push_front(url.fileName());
    url = url.upURL();
  } while (!ExtFileInfo::exists(url)); // we know that the root exists, so this will terminate

  // move down the path and create all folders
  do {
    url.addPath(stack.front());
    if (KIO::NetAccess::mkdir(url, (QWidget*)0L, permissions))
    {
       checkProjectInsert(url, url, plugin, firstConfirm);
       firstConfirm = false;
    } else
    {
      if (confirm)
        errorMsg(window);
      return false;
    }
    stack.pop_front();
  } while (!stack.empty());

  return true;
}


void QuantaNetAccess::checkProjectInsert(const KUrl& source, const KUrl& target, KDevPlugin* plugin, bool confirm)
{
  QWidget *window = 0L;
  QuantaProjectIf *project = 0L;
  if (plugin)
  {
    window = plugin->mainWindow()->main();
    project = dynamic_cast<QuantaProjectIf*>(plugin->project());
  }
  if ( !project )
    return;

  KUrl saveUrl = adjustURL(target);
  KUrl baseURL = project->projectBase();
  KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, source);
  if ( baseURL.isParentOf(saveUrl) && (fileItem.isDir() || !project->isProjectFile(saveUrl)) ) //directories should always be checked as might contain files that were not yet added to the project
  {
    if (confirm && !project->isProjectFile(saveUrl))
    {
      QString nice = KUrl::relativeURL(baseURL, saveUrl);
      nice = KStringHandler::lsqueeze(nice, 60);
      if ( KMessageBox::Yes != KMessageBox::questionYesNo(window, i18n("<qt>Do you want to add <br><b>%1</b><br> to the project?</qt>", nice), i18n("Add to Project"), KStdGuiItem::yes(), KStdGuiItem::no(), "AddToProject") )
      {
        return;
      }
    }
    if ( fileItem.isDir() )
    {
      if (target == source) //mkdir case
      {
        project->addFile(target.path(KUrl::AddTrailingSlash).mid(baseURL.path(KUrl::AddTrailingSlash).length()));
      } else
      {
        QString prefix = saveUrl.path(KUrl::AddTrailingSlash);
        prefix.remove(0, baseURL.path(KUrl::AddTrailingSlash).length());
        QStringList targetList(prefix);
        KUrl url;
        KUrl::List urls = ExtFileInfo::allFilesRelative(source, "*");
        KUrl::List::ConstIterator end = urls.constEnd();
        for (KUrl::List::ConstIterator it = urls.constBegin() ; it != end; ++it)
        {
            targetList += prefix + (*it).path();
        }
        project->addFiles(targetList);
      }
    }
    else
    {
      saveUrl = KUrl::relativeURL(baseURL, saveUrl);
      project->addFile(saveUrl.path());
    }
  }
}


bool QuantaNetAccess::checkProjectRemove(const KUrl& src, KDevPlugin* plugin, bool confirm, bool remove)
{
  QWidget *window = 0L;
  QuantaProjectIf *project = 0L;
  if (plugin)
  {
    window = plugin->mainWindow()->main();
    project = dynamic_cast<QuantaProjectIf*>(plugin->project());
  }
  if (!project)
    return true;

  KUrl url = adjustURL(src);
  KUrl baseURL = project->projectBase();
  if ( baseURL.isParentOf(url) && project->isProjectFile(url) )
  {
    if (confirm)
    {
      QString nice = KUrl::relativeURL(baseURL, url);
      nice = KStringHandler::lsqueeze(nice, 60);
      if ( KMessageBox::Continue != KMessageBox::warningContinueCancel(window, i18n("<qt>Do you really want to remove <br><b>%1</b><br> from the project?</qt>", nice), i18n("Remove From Project"), KStdGuiItem::del(), "RemoveFromProject") )
      {
        return false;
      }
    }
    if (remove)
    {
      url = KUrl::relativeURL(baseURL, url);
      project->removeFile(url.path());
    }
  }
  return true;
}


bool QuantaNetAccess::checkProjectDel(const KUrl& src, KDevPlugin* plugin, QWidget* window, bool confirm)
{
  QuantaProjectIf *project = 0L;
  if (plugin)
  {
    project = dynamic_cast<QuantaProjectIf*>(plugin->project());
  }
  KUrl url = adjustURL(src);
  if ( project )
  {
    if ( project->projectBase().isParentOf(url) && project->isProjectFile(url) )
    {
      if (confirm)
      {
        QString nice = url.pathOrURL();
        nice = KStringHandler::csqueeze(nice, 60);
        if ( KMessageBox::Continue != KMessageBox::warningContinueCancel(window, i18n("<qt>Do you really want to delete <br><b>%1</b><br> and remove it from the project?</qt>", nice), i18n("Delete & Remove From Project"), KStdGuiItem::del(), "DeleteAndRemoveFromProject") )
        {
          return false;
        }
      }
      url = KUrl::relativeURL(project->projectBase(), url);
      project->removeFile(url.path());
      return true;
    }
  }
  // confirm normal delete if wanted
  if (confirm) {
    QString nice = url.pathOrURL();
    nice = KStringHandler::csqueeze(nice, 60);
    return (KMessageBox::Continue == KMessageBox::warningContinueCancel(window, i18n("<qt>Do you really want to delete <br><b>%1</b>?</qt>", nice), i18n("Delete File or Folder"), KStdGuiItem::del(), "DeleteFileOrFolder") );
  }
  return true;
}

KUrl QuantaNetAccess::adjustURL(const KUrl &url)
{
   KUrl u = url;
   if ( u.isLocalFile() )
   {
      QDir dir(u.path());
      u.setPath(dir.canonicalPath());
    }
    return u;
}


void QuantaNetAccess::errorMsg(QWidget* window)
{
  QString msg = KIO::NetAccess::lastErrorString();
  if ( !msg.isEmpty())
    KMessageBox::sorry(window, msg);
}
