/***************************************************************************
                          filestreefile.cpp  -  description
                             -------------------
    begin                : Wed May 31 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2002 Andras Mantia <amantia@freemail.hu>
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
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
#include <qpixmap.h>
#include <qfileinfo.h>

// KDE includes
#include <kiconloader.h>
#include <kmimetype.h>

// app includes
#include "filestreefile.h"
#include "filestreefolder.h"
#include "../resource.h"

FilesTreeFile::FilesTreeFile( FilesTreeFolder *parent, QString name, const KURL& p_url)
    : KListViewItem( parent, name )
{
  url = p_url;
  isDir = false;

  setDragEnabled(true);
  setDropEnabled(true);
}

FilesTreeFile::FilesTreeFile( QListView *parent, QString name, const KURL& p_url)
    : KListViewItem( parent, name )
{
  url = p_url;
  isDir = false;
  setDragEnabled(true);
  setDropEnabled(true);
}


FilesTreeFile::~FilesTreeFile(){
}

/** set icon of item  */
void FilesTreeFile::setIcon(const KURL& a_url)
{
 setPixmap(0, KMimeType::pixmapForURL(a_url, 0, KIcon::Small));
}

/** No descriptions */
void FilesTreeFile::sortChildItems(int column, bool ascending)
{
  QListViewItem::sortChildItems(column, ascending);
}
