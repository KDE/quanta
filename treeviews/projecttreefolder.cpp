/***************************************************************************
                          ProjectTreeFolder.cpp  -  description
                             -------------------
    begin                : Wed Mar 15 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002 Andras Mantia
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

// KDE includes
#include <kiconloader.h>

// app includes
#include "projecttreefile.h"
#include "projecttreefolder.h"
#include "projecttreeview.h"

ProjectTreeFolder::ProjectTreeFolder(QListView *parentListView, ProjectTreeFolder *parent, const KURL& p_url)
  : FilesTreeFolder(parentListView, parent, p_url )     
{
  filesTreeList.setAutoDelete( true );

  if ( text(0) == "CVS" ) setPixmap( 0, SmallIcon("log") );
  else                    setPixmap( 0, SmallIcon("folder") );
  
  QString fname = name;
  parentView =  dynamic_cast<ProjectTreeView*>(parentListView);
  url = p_url;
  url.adjustPath(1);
}

ProjectTreeFolder::ProjectTreeFolder(QListView *parentListView, const QString &name, const KURL &p_url)
    : FilesTreeFolder( parentListView, name, p_url)
{
  filesTreeList.setAutoDelete( true );

  if ( text(0) == "CVS" ) setPixmap( 0, SmallIcon("log") );
  else                    setPixmap( 0, SmallIcon("folder") );

  parentView =  dynamic_cast<ProjectTreeView*>(parentListView);
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
  if (open)
  {
    parentView->openFolder(this);
  }
  
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
