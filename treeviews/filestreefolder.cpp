/***************************************************************************
                          dirview.cpp  -  description
                             -------------------
    begin                : Mon Feb 21 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski
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

// app includes
#include "filestreefile.h"
#include "filestreefolder.h"
#include "../resource.h"

// QT includes
#include <qdir.h>
#include <qdragobject.h>
#include <qevent.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qpixmap.h>

// KDE includes
#include <kiconloader.h>


FilesTreeFolder::FilesTreeFolder( FilesTreeFolder * parent, const char * name )
    : QListViewItem( parent ), file( name ), folderName( name )
{
    parentFolder 	= parent;

    readable 	= true;
    opened 		= false;
    showall 	= true;

    setDragEnabled(true);
    setDropEnabled(true);
}


FilesTreeFolder::FilesTreeFolder( QListView * parent, const char * name, const char *_dir )
    : QListViewItem( parent ), file(name), folderName(_dir)
{
    parentFolder = 0L;

    readable 	= true;
    showall 	= true;

  setDragEnabled(true);
  setDropEnabled(true);
}


void FilesTreeFolder::setOpen( bool open )
{
/*	QString fileMask = QString("*.webprj ")	+fileMaskHtml
																					+fileMaskText
								 													+fileMaskImage;
 */
	QDir thisDir( fullName() );
	
  // exists ?
  if ( !thisDir.exists())	return;
  // readable ?
  if ( !thisDir.isReadable() )
  {
	  readable = false;
	  setExpandable( false );
	
	  setPixmap( 0, SmallIcon("folder_locked") );
	  return;
	}

  if ( open && !childCount() )
  {
		thisDir.setSorting( QDir::Name);
		thisDir.setFilter ( QDir::Dirs);
		
		QStringList::Iterator it;
		
		QStringList dirList =  thisDir.entryList();
		
		dirList.remove(".");
		dirList.remove("..");
		
		FilesTreeFolder *ditem;
		
		for ( it = dirList.begin(); it != dirList.end(); ++it )
		{
		  ditem = new FilesTreeFolder( this, *it );
		  ditem->setPixmap( 0, SmallIcon("folder") );
		}
		
		thisDir.setFilter( QDir::Files);
		
		QStringList fileList = thisDir.entryList();
		
		FilesTreeFile* item;
		
		for ( it = fileList.begin(); it != fileList.end(); ++it )
	//	  if ( QDir::match( fileMask, *it ) )
		  {
			  item = new FilesTreeFile( this, *it, fullName()+(*it) );
			  item->setIcon( *it );
			}
  }

  if ( parentFolder)
  {
  	  if ( open ) setPixmap( 0, SmallIcon("folder_open") );
  	  else        setPixmap( 0, SmallIcon("folder") );
  }

  opened = open;

  QListViewItem::setOpen( open );
}

/** retun full name of the folder */
QString FilesTreeFolder::fullName()
{
	QString s="";
	
//  if (!item) item = this;

  if ( parentFolder )
  {
		s = parentFolder->fullName();
		s += file.name()+"/";
  }
  else {
		s = folderName;
  }

  return s;
}

/** return column text */
QString FilesTreeFolder::text( int ) const
{
	return file.name();
}

/** setup */
void FilesTreeFolder::setup()
{
    setExpandable( true );
    QListViewItem::setup();
}

/** reload file list */
void FilesTreeFolder::reloadList()
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
QString FilesTreeFolder::key ( int, bool ) const
{
  return " 0"+text(0);
}

FilesTreeFolder::~FilesTreeFolder()
{
}
/** No descriptions */
void FilesTreeFolder::saveOpenStatus()
{
 QListViewItem *child = firstChild();
 FilesTreeFolder *folder;

 openedList.clear();
 QString s;

 while (child)
 {
    folder = dynamic_cast<FilesTreeFolder *> (child);
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
void FilesTreeFolder::restoreOpenStatus()
{
 QListViewItem *child = firstChild();
 FilesTreeFolder *folder;
 QString s;

 while (child )
   {
    folder = dynamic_cast<FilesTreeFolder *> (child);
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
/** No descriptions */
void FilesTreeFolder::dropped(QDropEvent *e)
{
  QString droppedText;

  if (QTextDrag::decode(e, droppedText))
  {

  }
}
/** No descriptions */
bool FilesTreeFolder::acceptDrop(const QMimeSource *mime) const
{
  return true;
}
/** No descriptions */
void FilesTreeFolder::sortChildItems(int column, bool ascending)
{
  QListViewItem::sortChildItems(column, ascending);
}
