/***************************************************************************
                          quantanetaccess.h  -  description
                             -------------------
    begin                : Jun 21 2004
    copyright            : (C) 2004 by Jens Herden <jhe@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include <qstring.h>
#include <qwidget.h>
#include <qdir.h>

#include <kio/netaccess.h>
#include <kurl.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kfileitem.h>
#include <kstringhandler.h>

#include "qextfileinfo.h"
#include "quantanetaccess.h"
#include "project.h"

bool QuantaNetAccess::upload(const QString& src, const KURL& target, QWidget* window, bool confirm)
{
  bool ok = KIO::NetAccess::upload(src, target, window);
  if (ok) {
    checkProjectInsert(target, window, confirm);
  } else {
    if (confirm)
      errorMsg(window);
  }
  return ok;
}


bool QuantaNetAccess::file_copy( const KURL& src, const KURL& target, int permissions,
                                 bool overwrite, bool resume, QWidget* window, bool confirm)
{
  bool ok = KIO::NetAccess::file_copy( src, target, permissions, overwrite, resume, window );
  if (ok) {
    checkProjectInsert(target, window, confirm);
  } else {
    if (confirm)
      errorMsg(window);
  }
  return ok;
}


bool QuantaNetAccess::file_move( const KURL& src, const KURL& target, int permissions,
                                 bool overwrite, bool resume, QWidget* window, bool confirm)
{
  // don't ask if move is inside of the project
  bool oldConfirm = confirm;
  if ( Project::ref()->projectBaseURL().isParentOf(src) &&
       Project::ref()->projectBaseURL().isParentOf(target) )
  {
    confirm = false;
  }
  if ( !checkProjectRemove(src, window, confirm)) {
    return false;
  }
  bool ok = KIO::NetAccess::file_move( src, target, permissions, overwrite, resume, window );
  if (ok) {
    checkProjectInsert(target, window, confirm);
  } else {
    if (oldConfirm)
      errorMsg(window);
  }
  return ok;
}


bool QuantaNetAccess::dircopy( const KURL::List & srcList, const KURL & target, QWidget* window, bool confirm )
{
  bool ok = KIO::NetAccess::dircopy( srcList, target, window );
  if (ok) {
    KURL url;
    for ( KURL::List::ConstIterator it = srcList.begin(); it != srcList.end(); ++it ) {
      url = target;
      url.adjustPath(+1);
      url.setFileName((*it).fileName());
      checkProjectInsert(url, window, confirm);
    }
  } else {
    if (confirm)
      errorMsg(window);
  }
  return ok;
}


bool QuantaNetAccess::move( const KURL::List& srcList, const KURL& target, QWidget* window, bool confirm )
{
  KURL targetURL = adjustURL(target);
  bool oldConfirm = confirm;
  bool moveInsideProject = false;
  bool targetInProject = Project::ref()->projectBaseURL().isParentOf(targetURL);
  KURL url;
  // first we ask about the URLs in the list without actually removing them from the project
  for ( KURL::List::ConstIterator it = srcList.begin(); it != srcList.end(); ++it ) {
    //don't ask if move is inside of the project
    url = adjustURL(*it);
    if (targetInProject && Project::ref()->projectBaseURL().isParentOf(url) )
    {
      confirm = false;
      moveInsideProject = true;
    }
    if ( !checkProjectRemove(*it, window, confirm, false)) {
      return false;
    confirm = oldConfirm;
    }
  }
  // all URLs are confirmed, we remove them from the project
  for ( KURL::List::ConstIterator it = srcList.begin(); it != srcList.end(); ++it ) {
    if ( Project::ref()->projectBaseURL().isParentOf(*it) )
      Project::ref()->slotRemove(*it);
  }
  bool ok = KIO::NetAccess::move( srcList, targetURL, window );
  if (ok) {
    KURL url;
    for ( KURL::List::ConstIterator it = srcList.begin(); it != srcList.end(); ++it ) {
      url = target;
      url.adjustPath(+1);
      url.setFileName((*it).fileName());
      checkProjectInsert(url, window, confirm);
      Project::ref()->urlMoved(*it, url);
   }
  } else {
    if (confirm)
      errorMsg(window);
  }
  return ok;
}


bool QuantaNetAccess::del( const KURL & url, QWidget* window, bool confirm )
{
  if ( !checkProjectDel(url, window, confirm)) {
    return false;
  }
  bool ok = KIO::NetAccess::del( url, window );
  if (!ok && confirm)
  {
      errorMsg(window);
  }
  return ok;
}


bool QuantaNetAccess::mkdir( const KURL & url, QWidget* window, int permissions, bool confirm )
{
  KURL u = url;
  u.adjustPath(-1); //some servers refuse to create directories ending with a slash
  bool ok = KIO::NetAccess::mkdir( u, window, permissions );
  if (ok) {
    checkProjectInsert(url, window, confirm);
  } else {
    if (confirm)
      errorMsg(window);
  }
  return ok;
}


void QuantaNetAccess::checkProjectInsert(const KURL& target, QWidget* window, bool confirm)
{
  if ( !Project::ref()->hasProject()) return;
  KURL saveUrl = adjustURL(target);
  KURL baseURL = Project::ref()->projectBaseURL();
  if ( baseURL.isParentOf(saveUrl) && !Project::ref()->contains(saveUrl) )
  {
    if (confirm)
    {
      QString nice = QExtFileInfo::toRelative(saveUrl, baseURL).path();
      nice = KStringHandler::lsqueeze(nice, 60);
      if ( KMessageBox::Yes != KMessageBox::questionYesNo(window, i18n("<qt>Do you want to add <br><b>%1</b><br> to the project?</qt>").arg(nice), i18n("Add to Project"), KStdGuiItem::add(), i18n("Do Not Add"), "AddToProject") )
      {
        return;
      }
    }
    KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, saveUrl);
    if ( fileItem.isDir() )
      Project::ref()->slotAddDirectory(saveUrl, false);
    else
      Project::ref()->slotInsertFile(saveUrl);
  }
}


bool QuantaNetAccess::checkProjectRemove(const KURL& src, QWidget* window, bool confirm, bool remove)
{
  if ( !Project::ref()->hasProject() ) return true;
  KURL url = adjustURL(src);
  KURL baseURL = Project::ref()->projectBaseURL();
  if ( baseURL.isParentOf(url) && Project::ref()->contains(url) )
  {
    if (confirm)
    {
      QString nice = QExtFileInfo::toRelative(url, baseURL).path();
      nice = KStringHandler::lsqueeze(nice, 60);
      if ( KMessageBox::Continue != KMessageBox::warningContinueCancel(window, i18n("<qt>Do you really want to remove <br><b>%1</b><br> from the project?</qt>").arg(nice), i18n("Remove From Project"), KStdGuiItem::remove(), "RemoveFromProject") )
      {
        return false;
      }
    }
    if (remove)
      Project::ref()->slotRemove(url);
  }
  return true;
}


bool QuantaNetAccess::checkProjectDel(const KURL& src, QWidget* window, bool confirm)
{
  KURL url = adjustURL(src);
  if ( Project::ref()->hasProject() )
  {
    if ( Project::ref()->projectBaseURL().isParentOf(url) && Project::ref()->contains(url) )
    {
      if (confirm)
      {
        QString nice = url.prettyURL(0, KURL::StripFileProtocol);
        nice = KStringHandler::csqueeze(nice, 60);
        if ( KMessageBox::Continue != KMessageBox::warningContinueCancel(window, i18n("<qt>Do you really want to delete <br><b>%1</b><br> and remove it from the project?</qt>").arg(nice), i18n("Delete & Remove From Project"), KStdGuiItem::del(), "DeleteAndRemoveFromProject") )
        {
          return false;
        }
      }
      Project::ref()->slotRemove(url);
      return true;
    }
  }
  // confirm normal delete if wanted
  if (confirm) {
    QString nice = url.prettyURL(0, KURL::StripFileProtocol);
    nice = KStringHandler::csqueeze(nice, 60);
    return (KMessageBox::Continue == KMessageBox::warningContinueCancel(window, i18n("<qt>Do you really want to delete <br><b>%1</b>?</qt>").arg(nice), i18n("Delete File or Folder"), KStdGuiItem::del(), "DeleteFileOrFolder") );
  }
  return true;
}

KURL QuantaNetAccess::adjustURL(const KURL &url)
{
   KURL u = url;
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
