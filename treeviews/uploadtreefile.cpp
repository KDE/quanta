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
#include <kurl.h>

// app includes
#include "uploadtreefile.h"
#include "../resource.h"

UploadTreeFile::UploadTreeFile( UploadTreeFolder *parent, const KURL &a_url, QString date, QString size )
    : QListViewItem( parent, a_url.fileName(), "", date, size )
{
	m_url = a_url;
	isDir = false;

  setWhichPixmap("check_clear");
  setText(0, m_url.fileName());

  //setDragEnabled(true);
  //setDropEnabled(true);
}

UploadTreeFile::UploadTreeFile( QListView *parent, const KURL &a_url, QString date, QString size )
    : QListViewItem( parent, a_url.fileName(), "", date, size )
{
	m_url = a_url;
	isDir = false;

  //setPixmap( 1, SmallIcon("check") );
  setWhichPixmap("check_clear");
  setText(0, m_url.fileName());

	//setDragEnabled(true);
	//setDropEnabled(true);
}


UploadTreeFile::~UploadTreeFile(){
}

/** set icon of item  */
void UploadTreeFile::setIcon( QString name )
{
//TODO: possible UploadTreeFile should inherit from somewhere??? This code is duplicate
  if (QuantaCommon::checkMimeType(m_url,"html")) setPixmap( 0, SmallIcon("www"));
  else
    if (QuantaCommon::checkMimeType(m_url,"x-java") ) setPixmap( 0, SmallIcon("info"));
    else
      if (QuantaCommon::checkMimeGroup(m_url,"text")) setPixmap( 0, SmallIcon("txt"));
      else  if (QuantaCommon::checkMimeGroup(m_url,"image")) setPixmap( 0, SmallIcon("image") );
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

/** No descriptions */
KURL UploadTreeFile::url()
{
  return m_url;
}
