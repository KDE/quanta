/***************************************************************************
                          docfolder.cpp  -  description
                             -------------------
    begin                : Fri Mar 3 2000
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
#include <qstrlist.h>
#include <qpixmap.h>

// KDE includes
#include <kapp.h>
#include <kconfig.h>
#include <kstddirs.h>

// app includes
#include "docfolder.h"
#include "docitem.h"

#include "pix/mini-book1.xpm"
#include "pix/mini-book2.xpm"
// #include "pix/greenbook.xpm"
#include "pix/mini-doc.xpm"
#include "pix/folder.xpm"
#include "pix/folder_open.xpm"

DocFolder::DocFolder(QListViewItem *parent, QString _name, KConfig *config, QString basePath)
  : QListViewItem(parent)
{
  name = _name;
  topLevel = false;

  QStrList list;

  config->readListEntry( name, list );

  char *item;

  for ( list.last(); ( item = list.current() ) ; list.prev() ) {
    if ( item[0] != '#' ) {
      QString url = config->readEntry( item );
      DocItem *el = new DocItem( this, QString(item), basePath+url);
      el->setPixmap( 0, QPixmap((const char**)mini_doc_xpm) );
    }
  }

  for ( list.last(); ( item = list.current() ) ; list.prev() ) {
    if ( item[0] == '#' ) { // current item is folder
      item++; // remove leading #
      DocFolder *el = new DocFolder(this, QString(item), config, basePath);
      el->setPixmap( 0, QPixmap((const char**)mini_book1_xpm) );
      el->setOpen( false );
    }
  }
}

DocFolder::DocFolder(QListView *parent, QString _name, KConfig *config, QString basePath)
  : QListViewItem(parent)
{
  name = _name;
  topLevel = false;
  QStrList list;

  config->readListEntry( name, list );

  char *item;

  for ( list.last(); ( item = list.current() ) ; list.prev() ) {
    if ( item[0] != '#' ) {
      QString url = config->readEntry( item );
      DocItem *el = new DocItem( this, QString(item), basePath+url);
      el->setPixmap( 0, QPixmap((const char**)mini_doc_xpm) );
    }
  }

  for ( list.last(); ( item = list.current() ) ; list.prev() ) {
    if ( item[0] == '#' ) { // current item is folder
      item++; // remove leading #
      DocFolder *el = new DocFolder(this, QString(item), config, basePath);
      el->setPixmap( 0, QPixmap((const char**)mini_book1_xpm) );
      el->setOpen( false );
    }
  }
}



DocFolder::~DocFolder(){
}


QString DocFolder::text( int ) const
{
  return name.data();
}

void DocFolder::setup()
{
    setExpandable( TRUE );
    QListViewItem::setup();
}
/**  */
void DocFolder::setOpen( bool o)
{
  QListViewItem::setOpen( o );
  if ( !topLevel ) {
      if (o)
  		  setPixmap( 0, QPixmap((const char**)mini_book2_xpm) );
      else
  		  setPixmap( 0, QPixmap((const char**)mini_book1_xpm) );
  } else {
      if (o)
    		setPixmap( 0, QPixmap((const char**)folder_open_xpm) );
      else
  		  setPixmap( 0, QPixmap((const char**)folder_xpm) );
  }
}
