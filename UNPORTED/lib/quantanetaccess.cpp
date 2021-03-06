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

//own includes
#include "quantanetaccess.h"
#include "extfileinfo.h"

//qt includes
#include <QString>
#include <qwidget.h>
#include <QDir>

//kde includes
#include <kio/netaccess.h>
#include <kio/jobuidelegate.h>
#include <kurl.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kfileitem.h>
#include <kxmlguiwindow.h>
#include <kstringhandler.h>
#include <kparts/mainwindow.h>

//kdevelop includes
#include <shell/core.h>
#include <interfaces/iplugin.h>
#include <interfaces/iproject.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/iuicontroller.h>

bool QuantaNetAccess::upload(const QString& src, const KUrl& target, KDevelop::IPlugin* plugin, bool confirm)
{
  QWidget *window = KDevelop::Core::self()->uiController()->activeMainWindow();
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

// TODO port the overwrite and resume bools to KIO::JobFlags? (Remove resume, you probably don't need it)
bool QuantaNetAccess::file_copy( const KUrl& src, const KUrl& target, int permissions,
                                 bool overwrite, bool resume, KDevelop::IPlugin* plugin, bool confirm)
{
  QWidget *window = KDevelop::Core::self()->uiController()->activeMainWindow();
  bool ok;
  if (src == target)
    ok = true;
  else {
    KIO::JobFlags flags = KIO::DefaultFlags;
    if (overwrite) flags |= KIO::Overwrite;
    if (resume) flags |= KIO::Resume;
    KIO::Job* job = KIO::file_copy( src, target, permissions, flags );
    job->ui()->setWindow( window );
    ok = job->exec();
  }
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


// TODO port the overwrite and resume bools to KIO::JobFlags? (Remove resume, you probably don't need it)
bool QuantaNetAccess::file_move( const KUrl& src, const KUrl& target, int permissions,
                                 bool overwrite, bool resume, KDevelop::IPlugin* plugin, bool confirm)
{
  QWidget *window = 0L;
  KDevelop::IProject *project = 0L;
  if (plugin)
  {
    window = KDevelop::Core::self()->uiController()->activeMainWindow();    
    //FIXME: no currentProject!
    project = KDevelop::Core::self()->projectController()->projectAt(0);
  }
  // don't ask if move is inside of the project
  bool oldConfirm = confirm;
  if ( project && project->folder().isParentOf(src) &&
       project->folder().isParentOf(target) )
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
  else {
    KIO::JobFlags flags = KIO::DefaultFlags;
    if (overwrite) flags |= KIO::Overwrite;
    if (resume) flags |= KIO::Resume;
    KIO::Job* job = KIO::file_move( src, target, permissions, flags );
    job->ui()->setWindow( window );
    ok = job->exec();
  }
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


bool QuantaNetAccess::dircopy( const KUrl::List & srcList, const KUrl & target, KDevelop::IPlugin* plugin, bool confirm )
{
  QWidget *window = 0L;
  if (plugin)
  {
    window = KDevelop::Core::self()->uiController()->activeMainWindow();      
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


bool QuantaNetAccess::move( const KUrl::List& srcList, const KUrl& target, KDevelop::IPlugin* plugin, bool confirm )
{
  QWidget *window = 0L;
  KDevelop::IProject *project = 0L;
  if (plugin)
  {
    window = KDevelop::Core::self()->uiController()->activeMainWindow();
    //FIXME: no currentProject!
    project = KDevelop::Core::self()->projectController()->projectAt(0);
  }
  KUrl targetURL = adjustURL(target);
  if (project)
  {
    bool oldConfirm = confirm;
    bool moveInsideProject = false;
    bool targetInProject = project->folder().isParentOf(targetURL);
    KUrl url;
    KUrl baseURL = project->folder();
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
        url = KUrl::relativeUrl(baseURL, *it);
        //FIXME!!! Not ported to KDevelop4        
        //project->fileManager()->removeFile(url);
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


bool QuantaNetAccess::del( const KUrl & url, KDevelop::IPlugin* plugin, QWidget* window, bool confirm )
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


bool QuantaNetAccess::mkdir( const KUrl & path, KDevelop::IPlugin* plugin, int permissions, bool confirm )
{
  QWidget *window = 0L;
  if (plugin)
  {
    window = KDevelop::Core::self()->uiController()->activeMainWindow();
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
    url = url.upUrl();
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


void QuantaNetAccess::checkProjectInsert(const KUrl& source, const KUrl& target, KDevelop::IPlugin* plugin, bool confirm)
{
  QWidget *window = 0L;
  KDevelop::IProject *project = 0L;
  if (plugin)
  {
    window = KDevelop::Core::self()->uiController()->activeMainWindow();
    //FIXME: no currentProject!
    project = KDevelop::Core::self()->projectController()->projectAt(0);
  }
  if ( !project )
    return;

  KUrl saveUrl = adjustURL(target);
  KUrl baseURL = project->folder();
  KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, source);
  if ( baseURL.isParentOf(saveUrl) && (fileItem.isDir() || !project->inProject(saveUrl)) ) //directories should always be checked as might contain files that were not yet added to the project
  {
    if (confirm && !project->inProject(saveUrl))
    {
      QString nice = KUrl::relativeUrl(baseURL, saveUrl);
      nice = KStringHandler::lsqueeze(nice, 60);
      if ( KMessageBox::Yes != KMessageBox::questionYesNo(window, i18n("<qt>Do you want to add<br /><b>%1</b><br />to the project?</qt>", nice), i18n("Add to Project"), KStandardGuiItem::yes(), KStandardGuiItem::no(), "AddToProject") )
      {
        return;
      }
    }
    if ( fileItem.isDir() )
    {
      if (target == source) //mkdir case
      {
        //FIXME!!! Not ported to KDevelop4
        //project->fileManager()->addFile(target.path(KUrl::AddTrailingSlash).mid(baseURL.path(KUrl::AddTrailingSlash).length()));
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
 //FIXME!!! Not ported to KDevelop4
               //project->fileManager()->addFiles(targetList);
      }
    }
    else
    {
      saveUrl = KUrl::relativeUrl(baseURL, saveUrl);
//FIXME!!! Not ported to KDevelop4
            //project->fileManager()->addFile(saveUrl.path());
    }
  }
}


bool QuantaNetAccess::checkProjectRemove(const KUrl& src, KDevelop::IPlugin* plugin, bool confirm, bool remove)
{
  QWidget *window = 0L;
  KDevelop::IProject *project = 0L;
  if (plugin)
  {
    window = KDevelop::Core::self()->uiController()->activeMainWindow();
    //FIXME: no currentProject!
    project = KDevelop::Core::self()->projectController()->projectAt(0);
  }
  if (!project)
    return true;

  KUrl url = adjustURL(src);
  KUrl baseURL = project->folder();
  if ( baseURL.isParentOf(url) && project->inProject(url) )
  {
    if (confirm)
    {
      QString nice = KUrl::relativeUrl(baseURL, url);
      nice = KStringHandler::lsqueeze(nice, 60);
      if ( KMessageBox::Continue != KMessageBox::warningContinueCancel(window, i18n("<qt>Do you really want to remove<br /><b>%1</b><br />from the project?</qt>", nice), i18n("Remove From Project"), KStandardGuiItem::del(), KStandardGuiItem::cancel(), "RemoveFromProject") )
      {
        return false;
      }
    }
    if (remove)
    {
      url = KUrl::relativeUrl(baseURL, url);
      //FIXME!!! Not ported to KDevelop4
      //project->fileManager()->removeFile(url.path());
    }
  }
  return true;
}


bool QuantaNetAccess::checkProjectDel(const KUrl& src, KDevelop::IPlugin* plugin, QWidget* window, bool confirm)
{
  KDevelop::IProject *project = 0L;
  if (plugin)
  {
    //FIXME: no currentProject!
    project = KDevelop::Core::self()->projectController()->projectAt(0);
  }
  KUrl url = adjustURL(src);
  if ( project )
  {
    if ( project->folder().isParentOf(url) && project->inProject(url) )
    {
      if (confirm)
      {
        QString nice = url.pathOrUrl();
        nice = KStringHandler::csqueeze(nice, 60);
        if ( KMessageBox::Continue != KMessageBox::warningContinueCancel(window, i18n("<qt>Do you really want to delete<br /><b>%1</b><br />and remove it from the project?</qt>", nice), i18n("Delete & Remove From Project"), KStandardGuiItem::del(), KStandardGuiItem::cancel(), "DeleteAndRemoveFromProject") )
        {
          return false;
        }
      }
      url = KUrl::relativeUrl(project->folder(), url);
      //FIXME!!! Not ported to KDevelop4
      //project->fileManager()->removeFile(url.path());
      return true;
    }
  }
  // confirm normal delete if wanted
  if (confirm) {
    QString nice = url.pathOrUrl();
    nice = KStringHandler::csqueeze(nice, 60);
    return (KMessageBox::Continue == KMessageBox::warningContinueCancel(window, i18n("<qt>Do you really want to delete<br /><b>%1</b>?</qt>", nice), i18n("Delete File or Folder"), KStandardGuiItem::del(), KStandardGuiItem::cancel(), "DeleteFileOrFolder") );
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
