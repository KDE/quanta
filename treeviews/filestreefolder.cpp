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

// QT includes
#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qpixmap.h>

// include icons
#include "pix/folder.xpm"
#include "pix/folder_open.xpm"
#include "pix/folder_locked.xpm"

extern QString fileMaskHtml;
extern QString fileMaskText;
extern QString fileMaskImage;

FilesTreeFolder::FilesTreeFolder( FilesTreeFolder * parent, const char * name )
    : QListViewItem( parent ), file( name ), folderName( name )
{
    parentFolder 	= parent;

    readable 	= true;
    opened 		= false;
    showall 	= true;
}


FilesTreeFolder::FilesTreeFolder( QListView * parent, const char * name, const char *_dir )
    : QListViewItem( parent ), file(name), folderName(_dir)
{
    parentFolder = 0L;

    readable 	= true;
    showall 	= true;
}


void FilesTreeFolder::setOpen( bool open )
{
	QString fileMask = QString("*.webprj ")	+fileMaskHtml
																					+fileMaskText
								 													+fileMaskImage;

	QDir thisDir( fullName() );
	
  // exists ?
  if ( !thisDir.exists())	return;
  // readable ?
  if ( !thisDir.isReadable() )
  {
	  readable = false;
	  setExpandable( false );
	
	  setPixmap( 0, QPixmap((const char**)folder_locked_xpm) );
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
		  ditem->setPixmap( 0, QPixmap((const char**)folder_xpm) );
		}
		
		thisDir.setFilter( QDir::Files);
		
		QStringList fileList = thisDir.entryList();
		
		if ( showall )
		{
      for ( it = fileList.begin(); it != fileList.end(); ++it )
			  if ( !QDir::match( fileMask, *it ) )
				  new FilesTreeFile( this, *it, fullName()+(*it) );
		}
		
		FilesTreeFile* item;
		
		for ( it = fileList.begin(); it != fileList.end(); ++it )
		  if ( QDir::match( fileMask, *it ) )
		  {
			  item = new FilesTreeFile( this, *it, fullName()+(*it) );
			  item->setIcon( *it );
			}
  }

  if ( parentFolder)
  {
  	if ( open )
  		setPixmap( 0, QPixmap((const char**)folder_open_xpm) );
  	else
  		setPixmap( 0, QPixmap((const char**)folder_xpm) );
  }

  opened = open;

  QListViewItem::setOpen( open );
}

/** retun full name of the folder */
QString FilesTreeFolder::fullName(QListViewItem *)
{
	QString s;
	
  if ( parentFolder )
  {
		s = parentFolder->fullName();
		s+= file.name()+"/";
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

  setOpen( false );

  QListViewItem *child;
  while ( (child = firstChild()) )
    removeItem( child );

  setOpen( true );
}

/** need for sorting */
QString FilesTreeFolder::key ( int, bool ) const
{
  static QString key;

  key = QString("1") + text(0);

  return key;
}
