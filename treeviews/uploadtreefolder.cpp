/***************************************************************************
                          uploadtreefolder.cpp  -  description
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

// app includes
#include "uploadtreefolder.h"
#include "uploadtreefile.h"
#include "../resource.h"

// QT includes
#include <qdir.h>
#include <qdragobject.h>
#include <qevent.h>
#include <qpixmap.h>

// KDE includes
#include <kiconloader.h>


UploadTreeFolder::UploadTreeFolder( UploadTreeFolder * parent, const char * name )
    : QListViewItem( parent, name, "", "", "" )
{
	parentFolder = parent;
  folderName = name;
  //file = name;

	readable = true;
	opened = false;
	showall = true;

  setPixmap( 0, SmallIcon("folder") );
  setPixmap( 1, SmallIcon("check") );

	//setDragEnabled(true);
	//setDropEnabled(true);
}

UploadTreeFolder::UploadTreeFolder( QListView * parent, const char * name )
    : QListViewItem( parent, name, "", "", "" )
{
	parentFolder = 0L;
  folderName = name;
  //file = name;

	readable 	= true;
	showall 	= true;

  setPixmap( 0, SmallIcon("folder") );
  setPixmap( 1, SmallIcon("check") );

	//setDragEnabled(true);
	//setDropEnabled(true);
}


void UploadTreeFolder::setOpen( bool open )
{
  opened = open;
  QListViewItem::setOpen( open );
}

/** retun full name of the folder */
QString UploadTreeFolder::fullName()
{
	QString s="";
	
//  if (!item) item = this;

  if ( parentFolder )
  {
		s = parentFolder->fullName();
		s += folderName+"/";
  }
  else {
		s = folderName;
  }

  return s;
}

/** setup */
void UploadTreeFolder::setup()
{
    setExpandable( true );
    QListViewItem::setup();
}

/** reload file list */
void UploadTreeFolder::reloadList()
{
  saveOpenStatus();
  setOpen( false );

  QListViewItem *child;
  while ( (child = firstChild()) )
    removeItem( child );

  setOpen( true );
  restoreOpenStatus();
}

/** need for sorting */
QString UploadTreeFolder::key ( int, bool ) const
{
  static QString key;

  key = QString("1") + text(0);

  return key;
}

UploadTreeFolder::~UploadTreeFolder()
{
}

/** No descriptions */
void UploadTreeFolder::saveOpenStatus()
{
 QListViewItem *child = firstChild();
 UploadTreeFolder *folder;

 openedList.clear();
 QString s;

 while (child)
 {
    folder = dynamic_cast<UploadTreeFolder *> (child);
  	if ( folder )
    {
     s = folder->fullName();
     folder->saveOpenStatus();
     if (folder->isOpen())
     {
        openedList.append(s);
     }
    }
    child = child->nextSibling();
 }
}
/** No descriptions */
void UploadTreeFolder::restoreOpenStatus()
{
 QListViewItem *child = firstChild();
 UploadTreeFolder *folder;
 QString s;

 while (child )
   {
    folder = dynamic_cast<UploadTreeFolder *> (child);
  	if ( folder )
    {
      s = folder->fullName();
      if (openedList.contains(s))
         folder->setOpen(true);
      folder->restoreOpenStatus();
    }
    child = child->nextSibling();
   }
}

void UploadTreeFolder::setWhichPixmap( QString pixmap )
{
  setPixmap( 1, SmallIcon(pixmap) );
}
