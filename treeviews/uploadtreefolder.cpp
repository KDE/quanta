/***************************************************************************
                          uploadtreefolder.cpp  -  description
                             -------------------
    begin                : Sun Aug 25 2002
    copyright            : (C) 2002 by Andras Mantia <amantia@kde.org>
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
#include <qdragobject.h>
#include <qevent.h>
#include <qpixmap.h>

// KDE includes
#include <kiconloader.h>

// app includes
#include "uploadtreefolder.h"
#include "uploadtreefile.h"
#include "resource.h"

UploadTreeFolder::UploadTreeFolder(const KURL &a_url, UploadTreeFolder * parent, const char * name )
    : KListViewItem( parent, name, "", "", "" )
{
  parentFolder = parent;
  m_url = a_url;
  m_url.adjustPath(1);

  setPixmap( 0, SmallIcon("folder") );
  setPixmap( 1, SmallIcon("check") );
  setText(0, m_url.fileName());

}

UploadTreeFolder::UploadTreeFolder(const KURL &a_url, QListView * parent, const char * name )
    : KListViewItem( parent, name, "", "", "" )
{
  parentFolder = 0L;
  m_url = a_url;
  m_url.adjustPath(1);

  setPixmap( 0, SmallIcon("folder") );
  setPixmap( 1, SmallIcon("check") );
  setText(0, m_url.fileName());
}


void UploadTreeFolder::setOpen( bool open )
{
  QListViewItem::setOpen( open );
}

/** retun full name of the folder */
//TODO: This should go away. Use url() instead.
QString UploadTreeFolder::fullName()
{
  QString s="";

  if ( parentFolder )
  {
    s = parentFolder->fullName();
    s += m_url.fileName()+"/";
  }
  else {
    s = m_url.fileName();
  }

  return s;
}

/** setup */
void UploadTreeFolder::setup()
{
    setExpandable( true );
    QListViewItem::setup();
}

/** reload file list */
void UploadTreeFolder::reloadList()
{
  setOpen( false );

  QListViewItem *child;
  while ( (child = firstChild()) )
    removeItem( child );

  setOpen( true );
}

/** need for sorting */
QString UploadTreeFolder::key ( int, bool ) const
{
  static QString key;

  key = QString("0") + text(0);

  return key;
}

UploadTreeFolder::~UploadTreeFolder()
{
}

void UploadTreeFolder::setWhichPixmap(const QString& pixmap )
{
  setPixmap( 1, SmallIcon(pixmap) );
}

