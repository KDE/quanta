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

FilesTreeFile::FilesTreeFile( FilesTreeFolder *parent, QString name, const KURL& p_url)
    : QListViewItem( parent, name )
{
	url = p_url;
	isDir = false;

  setDragEnabled(true);
  setDropEnabled(true);
}

FilesTreeFile::FilesTreeFile( QListView *parent, QString name, const KURL& p_url)
    : QListViewItem( parent, name )
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
//TODO
  if (QuantaCommon::checkMimeType(a_url,"html")) setPixmap( 0, SmallIcon("www"));
  else
    if (QuantaCommon::checkMimeType(a_url,"x-java") ) setPixmap( 0, SmallIcon("info"));
    else
      if (QuantaCommon::checkMimeGroup(a_url,"text")) setPixmap( 0, SmallIcon("txt"));
      else  if (QuantaCommon::checkMimeGroup(a_url,"image")) setPixmap( 0, SmallIcon("image") );
}

/** set icon of item  */
void FilesTreeFile::setDirIcon()
{
  setPixmap( 0, SmallIcon("folder") );
  isDir = true;
}


/** used for sorting */
/*
QString FilesTreeFile::key ( int, bool ) const
{
  static QString key;

//  key  = "2"+QFileInfo(text(0)).extension();

//  QString prefix = "5";

//FIXME: VERY slow on KDE/QT 3.0
//  if ( QDir::match( fileMaskHtml,  key) ) prefix = "2";
//  if ( QDir::match( fileMaskText,  key) ) prefix = "3";
//  if ( QDir::match( fileMaskImage, key) ) prefix = "4";
//  if ( isDir ) prefix = "1";

//  key = prefix + key;

//  return key;
//  if ( isDir ) key.prepend("0");
  return text(0);//key;

}                                                         */
/** No descriptions */
void FilesTreeFile::sortChildItems(int column, bool ascending)
{
  QListViewItem::sortChildItems(column, ascending);
}
