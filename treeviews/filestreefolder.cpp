/***************************************************************************
                          filestreefolder.cpp  -  description
                             -------------------
    begin                : Mon Feb 21 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski
                           (C) 2002-2003 Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// QT includes
#include <qdir.h>
#include <qdragobject.h>
#include <qevent.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qpixmap.h>
#include <qstringlist.h>
#include <qregexp.h>

// KDE includes
#include <kiconloader.h>
#include <kurl.h>
#include <kdirlister.h>

// app includes
#include "filestreefolder.h"
#include "filestreefile.h"
#include "filestreeview.h"
#include "../resource.h"

FilesTreeFolder::FilesTreeFolder(QListView* parentListView, FilesTreeFolder *parent, const KURL& p_url)
    : KListViewItem( parent )
{
  parentView = dynamic_cast<FilesTreeView*>(parentListView);
  parentFolder   = parent;
//  opened     = false;
  name = p_url.fileName();
  url = p_url;
  init();
}


FilesTreeFolder::FilesTreeFolder( QListView * parentListView, const QString &p_name, const KURL& p_url)
    : KListViewItem( parentListView, 0L )
{
  parentView = dynamic_cast<FilesTreeView*>(parentListView);
  parentFolder = 0L;
  name = p_name;
  url = p_url;
  init();
}

FilesTreeFolder::~FilesTreeFolder()
{
}

/** No descriptions */
void FilesTreeFolder::init()
{
//  readable   = true;
//  showall   = true;
  setDragEnabled(true);
  setDropEnabled(true);
  url.adjustPath(1);   //add an ending "/" to the directory urls
}



void FilesTreeFolder::setOpen( bool open )
{

  if (open)
  {
    parentView->readDir(url);
  }

  QListViewItem::setOpen(open);
  if (open)
    setPixmap( 0, SmallIcon("folder_open") );
  else
    setPixmap( 0, SmallIcon("folder") );
}

/** retun full name of the folder */
QString FilesTreeFolder::fullName()
{
  QString s="";

//  if (!item) item = this;

  if ( parentFolder )
  {
    s = parentFolder->fullName();
    s += url.path(1);
  }
  else {
    s = url.path(1);
  }

  return s;
}

/** return column text */
QString FilesTreeFolder::text( int ) const
{
  return name;
}

/** setup */
void FilesTreeFolder::setup()
{
 setExpandable( true );
 QListViewItem::setup();
}

/** reload file list */
void FilesTreeFolder::reloadList()
{
  setOpen( false );

  QListViewItem *child;
  while ( (child = firstChild()) )
    removeItem( child );

  setOpen( true );
}

/** need for sorting */
QString FilesTreeFolder::key ( int, bool ) const
{
  return " 0"+text(0);
}

/** No descriptions */
void FilesTreeFolder::dropped(QDropEvent *e)
{
  QString droppedText;

  if (QTextDrag::decode(e, droppedText))
  {

  }
}
/** No descriptions */
bool FilesTreeFolder::acceptDrop(const QMimeSource *mime) const
{
  Q_UNUSED(mime);
  return true;
}
/** No descriptions */
void FilesTreeFolder::sortChildItems(int column, bool ascending)
{
  QListViewItem::sortChildItems(column, ascending);
}

