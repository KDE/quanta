/***************************************************************************
                          docitem.cpp  -  description
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

#include <qstrlist.h>

#include <kconfig.h>

#include "docitem.h"

DocItem::DocItem(QListViewItem *parent, const QString &_name, const QString &_url)
  : KListViewItem(parent)
{
  url = _url;
  name = _name;
}

DocItem::DocItem(QListView *parent, const QString &_name, const QString &_url)
  : KListViewItem(parent)
{
  url = _url;
  name = _name;
}


DocItem::~DocItem()
{
}


QString DocItem::text( int i) const
{
  if (i == 0)
    return name;
  else
    return "";
}
//#include "docitem.moc"

