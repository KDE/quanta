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

ProjectTreeFolder::ProjectTreeFolder( ProjectTreeFolder *parent, const KURL& p_url)
  : FilesTreeFolder(0, parent, p_url )     
{
  filesTreeList.setAutoDelete( true );

  if ( text(0) == "CVS" ) setPixmap( 0, SmallIcon("log") );
	else                    setPixmap( 0, SmallIcon("folder") );
  
  QString fname = name;

  url = p_url;
  url.adjustPath(1);
}

ProjectTreeFolder::ProjectTreeFolder( QListView *parent, const QString &name, const KURL &p_url)
    : FilesTreeFolder( parent, name, p_url)
{
  filesTreeList.setAutoDelete( true );

  if ( text(0) == "CVS" ) setPixmap( 0, SmallIcon("log") );
	else                    setPixmap( 0, SmallIcon("folder") );

  url = p_url;
  url.adjustPath(1);
}

ProjectTreeFolder::~ProjectTreeFolder(){
}

/** insert item in file list */
void ProjectTreeFolder::insertItem(ProjectTreeFile *item, const KURL& url)
{
  filesTreeList.append( item );
  item->url = url;
}

void ProjectTreeFolder::setOpen( bool open )
{
  QListViewItem::setOpen( open );
  
  if (open)
    setPixmap( 0, SmallIcon("folder_open") );
  else
    setPixmap( 0, SmallIcon("folder") );
  	
  if ( text(0) == "CVS" ) setPixmap( 0, SmallIcon("log") );

}


void ProjectTreeFolder::setup()
{
    if ( text(0) == "CVS") setExpandable( false );
    else                   setExpandable( true );
    QListViewItem::setup();
}

/**  */
bool ProjectTreeFolder::contains(const KURL& url)
{
  bool result = false;
  for ( uint i = 0;i < filesTreeList.count(); i++ )
  {
    if ( filesTreeList.at(i)->url == url )
    {
      result = true;
      break;
    }
  }
  return result;
}

void ProjectTreeFolder::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int alignment)
{
  QColorGroup mycg(cg);
  if ( text(0) == "CVS" )
    mycg.setBrush(QColorGroup::Text,QColor("#808080"));
  FilesTreeFolder::paintCell(p,mycg,column,width,alignment);
}
