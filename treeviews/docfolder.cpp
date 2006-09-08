/***************************************************************************
                          docfolder.cpp  -  description
                             -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2002 Andras Mantia <amantia@kde.org>
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
#include <kconfig.h>
#include <kstandarddirs.h>
#include <kiconloader.h>

// app includes
#include "docfolder.h"
#include "docitem.h"

DocFolder::DocFolder(QListViewItem *parent, const QString &_name, KConfig *config, const QString &basePath)
  : KListViewItem(parent)
{
  name = _name;
  topLevel = false;
  url = "";

  QStrList list;

  config->readListEntry( name, list );

  char *item;

  for ( list.last(); ( item = list.current() ) ; list.prev() ) {
    if ( item[0] != '#' ) {
      QString url = config->readEntry( item );
      DocItem *el = new DocItem( this, QString(item), basePath+url);
      el->setPixmap( 0, SmallIcon("info") );
    } else
    if ( item[0] == '#' ) { // current item is folder
      item++; // remove leading #
      QString l_url = config->readEntry( QString("folder_")+item, "" );
      DocFolder *el = new DocFolder(this, QString(item), config, basePath);
      if ( ! l_url.isEmpty() )
        el->url = basePath+l_url;
      el->setPixmap( 0, UserIcon("mini-book1") );
      el->setOpen( false );
    }
  }
}

DocFolder::DocFolder(QListView *parent, const QString &_name, KConfig *config, const QString &basePath)
  : KListViewItem(parent)
{
  name = _name;
  topLevel = false;
  url = "";
  QStrList list;

  config->readListEntry( name, list );

  char *item;

  for ( list.last(); ( item = list.current() ) ; list.prev() ) {
    if ( item[0] != '#' ) {
      QString url = config->readEntry( item );
      DocItem *el = new DocItem( this, QString(item), basePath+url);
      el->setPixmap( 0, SmallIcon("info") );
    } else
    if ( item[0] == '#' ) { // current item is folder
      item++; // remove leading #
      QString l_url = config->readEntry( QString("folder_")+item, "" );
      DocFolder *el = new DocFolder(this, QString(item), config, basePath);
      if ( ! l_url.isEmpty() )
        el->url = basePath+l_url;
      el->setPixmap( 0, UserIcon("mini-book1") );
      el->setOpen( false );
    }
  }
}



DocFolder::~DocFolder(){
}


QString DocFolder::text( int i) const
{
  if (i == 0)
    return name;
  else
    return "";
}

void DocFolder::setup()
{
    setExpandable( true );
    QListViewItem::setup();
}
/**  */
void DocFolder::setOpen( bool o)
{
  QListViewItem::setOpen( o );
  if ( !topLevel ) {
      if (o)
        setPixmap( 0, UserIcon("mini-book2") );
      else
        setPixmap( 0, UserIcon("mini-book1") );
  } else {
      if (o)
        setPixmap( 0, SmallIcon("folder_open") );
      else
        setPixmap( 0, SmallIcon("folder") );
  }
}
