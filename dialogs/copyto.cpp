/***************************************************************************
                          copytodlg.cpp  -  description
                             -------------------
    begin                : Mon Mar 27 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <dima@mail.univ.kiev.ua>
                           (C) 2001-2002 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
// qt includes

// kde includes
#include <kio/job.h>
#include <kio/netaccess.h>

//app includes
#include "copyto.h"
#include "qextfileinfo.h"

CopyTo::CopyTo(const KURL& dirURL)
{
  m_InitialDirUrl = dirURL;
}

CopyTo::~CopyTo(){
}

KURL CopyTo::copy(const KURL& urlToCopy, const KURL& destination)
{
  m_destList.clear();
  KURL targetDirURL = KURL();
  if ( destination.isEmpty() )
  {
    targetDirURL = m_InitialDirUrl;
  } else
  {
   targetDirURL = destination;
  }
  targetDirURL.adjustPath(1);

  bool doCopy = true;
  if (!QExtFileInfo::exists(targetDirURL, false, 0L))
  {
    doCopy = QExtFileInfo::createDir(targetDirURL, 0L);
  }

  KURL destURL;
  if (doCopy)
  {
    KIO::UDSEntry entry;
    KIO::NetAccess::stat(urlToCopy, entry, 0);
    KFileItem item(entry, urlToCopy, false, true);
    destURL = targetDirURL;
    destURL.setPath(destURL.path(1) + urlToCopy.fileName(false));
    if (item.isDir())
        destURL.adjustPath(1);

    KIO::CopyJob *job = KIO::copy(urlToCopy, destURL, true);
    connect( job, SIGNAL(result( KIO::Job *)),
                  SLOT  (slotResult( KIO::Job *)));

    QString path = destURL.path();
    if (path != "." && path != "..")
      m_destList.append(destURL);
  }

  return destURL;
}

void CopyTo::slotResult( KIO::Job *)
{
  emit addFilesToProject(m_destList);
  emit deleteDialog(this);
}

KURL::List CopyTo::copy(const KURL::List& sourceList, const KURL& destination )
{
  m_listCopy = true;
  m_destList.clear();
  KURL targetDirURL = KURL();
  if ( destination.isEmpty() )
  {
    targetDirURL = m_InitialDirUrl;
  } else
  {
    targetDirURL = destination;
  }
  bool doCopy = true;
  if (!QExtFileInfo::exists(targetDirURL, false, 0L))
  {
    doCopy = QExtFileInfo::createDir(targetDirURL, 0L);
  }

  KIO::UDSEntry entry;
  if (doCopy)
  {
    QString path;
    for (uint i = 0; i < sourceList.count(); i++)
    {
      KURL srcURL = sourceList[i];
      KIO::NetAccess::stat(srcURL, entry, 0);
      KFileItem item(entry, srcURL, false, true);
      KURL u = targetDirURL;
      u.setPath(targetDirURL.path(1) + srcURL.fileName());
      if (item.isDir())
         u.adjustPath(1);
      path = u.path();
      if (path != "." && path != "..")
        m_destList.append(u);
    }

    KIO::CopyJob *job = KIO::copy(sourceList, targetDirURL, true);
    connect( job, SIGNAL(result( KIO::Job *)),
                  SLOT  (slotResult( KIO::Job *)));
  }

  return m_destList;

}
#include "copyto.moc"
