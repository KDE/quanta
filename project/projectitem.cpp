/***************************************************************************
                          projectfile.cpp  -  description
                             -------------------
    begin                : Thu Mar 16 2000
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
#include <qfileinfo.h>
#include "projectitem.h"

ProjectItem::ProjectItem( QString basePath)
{
  this->basePath = basePath;
}

ProjectItem::~ProjectItem()
{
}

/**  */
QString ProjectItem::fullName(){
  return basePath+fname;
}

/** size of file */
uint ProjectItem::size()
{
  QFileInfo fi( fullName() );
  return fi.size();
}

/** return modified time */
time_t ProjectItem::modifiedTime()
{
  QFileInfo fi( fullName() );
  QDateTime time;
  time.setTime_t( 0 );
  return time.secsTo( fi.lastModified() );
}

/** need upload ??? */
bool ProjectItem::needUpload()
{
  return ( modifiedTime() > time );
}

/**  */
void ProjectItem::updateUploadTime()
{
  QDateTime time;
  time.setTime_t( 0 );
  uint i = time.secsTo( QDateTime::currentDateTime() );
  this->time = i;
}
