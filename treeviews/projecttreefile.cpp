/***************************************************************************
                          projectfile.cpp  -  description
                             -------------------
    begin                : Wed Mar 15 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
    email                : pdima@mail.univ.kiev.ua
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

// app includes
#include "projecttreefile.h"
#include "projecttreefolder.h"

// include images
#include "pix/html.xpm"
#include "pix/image.xpm"
#include "pix/txt.xpm"
#include "pix/java.xpm"

extern QString fileMaskHtml;
extern QString fileMaskText;
extern QString fileMaskJava;
extern QString fileMaskImage;


ProjectTreeFile::ProjectTreeFile( ProjectTreeFolder *parent, const char *name, const char *filename)
    : QListViewItem( parent, name )
{
  fname = filename;
  setIcon( filename );
}

ProjectTreeFile::~ProjectTreeFile(){
}

/**  */
void ProjectTreeFile::setIcon(QString name)
{
	if ( QDir::match( fileMaskHtml,  name) ) setPixmap( 0, QPixmap((const char**)html_xpm) );
  if ( QDir::match( fileMaskText,  name) ) setPixmap( 0, QPixmap((const char**)txt_xpm) );
  if ( QDir::match( fileMaskImage, name) ) setPixmap( 0, QPixmap((const char**)image_xpm) );
  if ( QDir::match( fileMaskJava,  name) ) setPixmap( 0, QPixmap((const char**)java_xpm) );
}

/** used for sorting */
QString ProjectTreeFile::key ( int, bool ) const
{
  static QString key;

  key  = QString( text(0) );

  QString prefix = "5";

  if ( QDir::match( fileMaskHtml,  key) ) prefix = "2";
  if ( QDir::match( fileMaskText,  key) ) prefix = "3";
  if ( QDir::match( fileMaskImage, key) ) prefix = "4";
  	
  key = prefix + key;
  return key;
}
