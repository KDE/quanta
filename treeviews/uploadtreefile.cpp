/***************************************************************************
                          uploadtreefile.cpp  -  description
                             -------------------
    begin                : Sun Aug 25 2002
    copyright            : (C) 2002, 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

// QT includes
#include <qdir.h>
#include <qpixmap.h>
#include <qfileinfo.h>

// KDE includes
#include <kfileitem.h>
#include <kiconloader.h>
#include <kurl.h>

// app includes
#include "uploadtreefile.h"
#include "resource.h"

UploadTreeFile::UploadTreeFile( UploadTreeFolder *parent, const KURL &a_url, const KFileItem &a_fileItem)
    : KListViewItem( parent, a_url.fileName(), "", QString("%1").arg( (long int)a_fileItem.size() ), a_fileItem.timeString())
{
  m_url = a_url;
  isDir = false;
  parentFolder = parent;
  m_fileItem = new KFileItem(a_fileItem);
  m_confirm = false;

  setWhichPixmap("check_clear");
  setText(0, m_url.fileName());
}

UploadTreeFile::UploadTreeFile( QListView *parent, const KURL &a_url, const KFileItem &a_fileItem)
    : KListViewItem( parent, a_url.fileName(), "", QString("%1").arg( (long int)a_fileItem.size() ), a_fileItem.timeString())
{
  m_url = a_url;
  isDir = false;
  parentFolder = 0L;
  m_fileItem = new KFileItem(a_fileItem);
  m_confirm = false;

  //setPixmap( 1, SmallIcon("check") );
  setWhichPixmap("check_clear");
  setText(0, m_url.fileName());
}


UploadTreeFile::~UploadTreeFile()
{
  delete m_fileItem;
}

int UploadTreeFile::permissions()
{
  if (m_fileItem)
      return m_fileItem->permissions();
  else
      return -1;
}

/** used for sorting */
QString UploadTreeFile::key ( int, bool ) const
{
  static QString key;
  key  = QString( "1" + text(0) );
  return key;
//  return QFileInfo(key).extension()+key;
}

void UploadTreeFile::setWhichPixmap(const QString& pixmap )
{
  setPixmap( 1, SmallIcon(pixmap) );
}

/** No descriptions */
KURL UploadTreeFile::url()
{
  return m_url;
}
