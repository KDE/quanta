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
  bool oldConfirm = confirm;
  bool targetInProject = Project::ref()->projectBaseURL().isParentOf(target);
  for ( KURL::List::ConstIterator it = srcList.begin(); it != srcList.end(); ++it ) {
    //don't ask if move is inside of the project
    if (targetInProject && Project::ref()->projectBaseURL().isParentOf(*it) )
    {
      confirm = false;
    }
    if ( !checkProjectRemove(*it, window, confirm)) {
      return false;
    confirm = oldConfirm;
    }
  }
  bool ok = KIO::NetAccess::move( srcList, target, window );
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
  bool ok = KIO::NetAccess::mkdir( url, window, permissions );
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
  if ( Project::ref()->projectBaseURL().isParentOf(saveUrl) && !Project::ref()->contains(saveUrl) )
  {
    if ( !confirm ||
      KMessageBox::Yes == KMessageBox::questionYesNo(window, i18n("<qt>Do you want to add <br><b>%1</b><br> to the project?</qt>").arg(saveUrl.prettyURL(0, KURL::StripFileProtocol)), i18n("Add to Project")) )
    {
      KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, saveUrl);
      if ( fileItem.isDir() )
        Project::ref()->slotAddDirectory(saveUrl, false);
      else
        Project::ref()->slotInsertFile(saveUrl);
    }
  }
}

                    
bool QuantaNetAccess::checkProjectRemove(const KURL& src, QWidget* window, bool confirm)
{
  if ( !Project::ref()->hasProject() ) return true;
  KURL url = adjustURL(src);
  if ( Project::ref()->projectBaseURL().isParentOf(url) && Project::ref()->contains(url) )
  {
    if ( !confirm ||
      KMessageBox::Yes == KMessageBox::questionYesNo(window,
      i18n("<qt>Do you really want to remove <br><b>%1</b><br> from the project?</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol), i18n("Remove from Project"))) )
    {
      Project::ref()->slotRemove(url);
    } else {
      return false;
    }
  }
  return true;
}

                    
bool QuantaNetAccess::checkProjectDel(const KURL& src, QWidget* window, bool confirm)
{
  if ( !Project::ref()->hasProject() ) return true;
  KURL url = adjustURL(src);
  if ( Project::ref()->projectBaseURL().isParentOf(url) && Project::ref()->contains(url) )
  {
    if ( !confirm ||
      KMessageBox::Yes == KMessageBox::questionYesNo(window,
      i18n("<qt>Do you really want to delete <br><b>%1</b><br> and remove it from the project?</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)), i18n("Delete and Remove from Project")) )
    {
      Project::ref()->slotRemove(url);
    } else
    {
      return false;
    }
  } else {
    if (confirm) {
      return (KMessageBox::Yes == KMessageBox::questionYesNo(window,
              i18n("<qt>Do you really want to delete <br><b>%1</b>  ?</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol))), i18n("Delete File or Folder") );
    }
  }
  return true;
}

KURL QuantaNetAccess::adjustURL(KURL url)
  {
    if ( url.isLocalFile() )
    {
      QDir dir(url.path());
      url.setPath(dir.canonicalPath());
    }
    return url;
  }


void QuantaNetAccess::errorMsg(QWidget* window)
{
  QString msg = KIO::NetAccess::lastErrorString();
  if ( !msg.isEmpty())
    KMessageBox::sorry(window, msg);
}
