/***************************************************************************
                          docitem.cpp  -  description
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

#include "docitem.h"
#include "kconfig.h"
#include "qstrlist.h"

DocItem::DocItem(QListViewItem *parent, QString _name, QString _url)
  : QListViewItem(parent)
{
  url = _url;
  name = _name;
}

DocItem::DocItem(QListView *parent, QString _name, QString _url)
  : QListViewItem(parent)
{
  url = _url;
  name = _name;
}


DocItem::~DocItem()
{
}


QString DocItem::text( int ) const
{
  return name.data();
}

#include "docitem.moc"
