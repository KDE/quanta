/***************************************************************************
                          uploadtreefile.cpp  -  description
                             -------------------
    begin                : Sun Aug 25 2002
    copyright            : (C) 2002 by Andras Mantia
    email                : amantia@freemail.hu
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
#include "uploadtreefile.h"
#include "../resource.h"

UploadTreeFile::UploadTreeFile( UploadTreeFolder *parent, QString filename, QString date, QString size )
    : QListViewItem( parent, filename, "", date, size )
{
	fname = filename;
	isDir = false;

  setWhichPixmap("check_clear");

  //setDragEnabled(true);
  //setDropEnabled(true);
}

UploadTreeFile::UploadTreeFile( QListView *parent, QString filename, QString date, QString size )
    : QListViewItem( parent, filename, "", date, size )
{
	fname = filename;
	isDir = false;

  //setPixmap( 1, SmallIcon("check") );
  setWhichPixmap("check_clear");

	//setDragEnabled(true);
	//setDropEnabled(true);
}


UploadTreeFile::~UploadTreeFile(){
}

/** set icon of item  */
void UploadTreeFile::setIcon( QString name )
{
  if ( QDir::match( fileMaskHtml,  name) ) setPixmap( 0, SmallIcon("www")   );
  if ( QDir::match( fileMaskText,  name) ) setPixmap( 0, SmallIcon("txt")   );
  if ( QDir::match( fileMaskImage, name) ) setPixmap( 0, SmallIcon("image") );
  if ( QDir::match( fileMaskJava,  name) ) setPixmap( 0, SmallIcon("info")  );
}


/** used for sorting */
QString UploadTreeFile::key ( int, bool ) const
{
  static QString key;
  key  = QString( text(0) );
  return QFileInfo(key).extension()+key;
}

void UploadTreeFile::setWhichPixmap( QString pixmap )
{
  setPixmap( 1, SmallIcon(pixmap) );
}

