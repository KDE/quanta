/***************************************************************************
                          filestreefile.cpp  -  description
                             -------------------
    begin                : Wed May 31 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
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

// app includes
#include "filestreefile.h"
#include "filestreefolder.h"
#include "../resource.h"

FilesTreeFile::FilesTreeFile( FilesTreeFolder *parent, QString name, QString filename)
    : QListViewItem( parent, name )
{
	fname = filename;
	isDir = false;

  setDragEnabled(true);
  setDropEnabled(true);
}

FilesTreeFile::FilesTreeFile( QListView *parent, QString name, QString filename)
    : QListViewItem( parent, name )
{
	fname = filename;
	isDir = false;
  setDragEnabled(true);
  setDropEnabled(true);
}


FilesTreeFile::~FilesTreeFile(){
}

/** set icon of item  */
void FilesTreeFile::setIcon( QString name )
{
  if ( QDir::match( fileMaskHtml,  name) ) setPixmap( 0, SmallIcon("www")   );
  if ( QDir::match( fileMaskText,  name) ) setPixmap( 0, SmallIcon("txt")   );
  if ( QDir::match( fileMaskImage, name) ) setPixmap( 0, SmallIcon("image") );
  if ( QDir::match( fileMaskJava,  name) ) setPixmap( 0, SmallIcon("info")  );
}

/** set icon of item  */
void FilesTreeFile::setDirIcon()
{
  setPixmap( 0, SmallIcon("folder") );
  isDir = true;
}


/** used for sorting */
QString FilesTreeFile::key ( int, bool ) const
{
  static QString key;

  key  = QString( text(0) );

/*  QString prefix = "5";

//FIXME: VERY slow on KDE/QT 3.0
  if ( QDir::match( fileMaskHtml,  key) ) prefix = "2";
  if ( QDir::match( fileMaskText,  key) ) prefix = "3";
  if ( QDir::match( fileMaskImage, key) ) prefix = "4";
  if ( isDir ) prefix = "1";

  key = prefix + key;

  return key;                                           */
  return QFileInfo(key).extension()+key;

}
