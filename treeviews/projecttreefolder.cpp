/***************************************************************************
                          ProjectTreeFolder.cpp  -  description
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

// KDE includes
#include <kiconloader.h>

// app includes
#include "projecttreefile.h"
#include "projecttreefolder.h"

ProjectTreeFolder::ProjectTreeFolder( ProjectTreeFolder *parent, const char *name)
  : FilesTreeFolder( parent, name )
{
  fileList.setAutoDelete( true );

  setPixmap( 0, SmallIcon("folder") );

  QString fname = name;

  if ( fname == "..")
  {
    path = parent->path;
    path.remove( path.length()-1, 1 );
    path.remove( path.findRev('/')+1 , 1000);
  }
  else {
    path = parent->path + fname;
    path += "/";
  }
}

ProjectTreeFolder::ProjectTreeFolder( QListView *parent, const char *name, const char *dir)
    : FilesTreeFolder( parent, name, dir)
{
  fileList.setAutoDelete( true );

  setPixmap( 0, SmallIcon("folder") );

  path = dir;
}

ProjectTreeFolder::~ProjectTreeFolder(){
}

/** return fullname of child element item */
QString ProjectTreeFolder::fullName(QListViewItem *item)
{
  if ( !item ) return 0;
  return path;
}

/** insert item in file list */
void ProjectTreeFolder::insertItem(ProjectTreeFile *item, QString name){
  fileList.append( item );
  item->fname = name;
}

void ProjectTreeFolder::setOpen( bool o )
{
//  ProjectTreeFile *item;

  if ( o && !childCount() )
  {

  }

  QListViewItem::setOpen( o );
//  if (p) {
  	if (o)
  		setPixmap( 0, SmallIcon("folder_open") );
  	else
  		setPixmap( 0, SmallIcon("folder") );
//  }
  opened = o;
}


void ProjectTreeFolder::setup()
{
    setExpandable( true );
    QListViewItem::setup();
}

/**  */
int ProjectTreeFolder::find(QString name)
{
  for ( fileList.first();fileList.current();fileList.next() ) {
    QString fname = fileList.current()->fname;
    if ( fname == name ) return 1;
  }
  return 0;
}

/** need for sorting */
QString ProjectTreeFolder::key ( int, bool ) const
{
  static QString k;
  k = QString("1") + text(0);
  return k.data();
}
