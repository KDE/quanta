/***************************************************************************
                          toolbaritem.cpp  -  description
                             -------------------
    begin                : Sun Apr 9 2000
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

#include "toolbaritem.h"

#include <qpixmap.h>
#include <qlistview.h>

#include <kapp.h>
#include <kiconloader.h>
#include <kfiledialog.h>


ToolBarItem::ToolBarItem(QListView *parent, const char *name, int pos = 0 )
  : QListViewItem(parent,name)
{
  data = ToolBarData(name);

  this->pos = pos;

  setPixmap(0,data.pix);
  QListViewItem::setText(0, name );
}

ToolBarItem::~ToolBarItem()
{
}

QPixmap ToolBarItem::icon()
{
  return data.pix;
}

const char* ToolBarItem::key ( int, bool ) const
{

  static QString skey;
  if ( pos > 9 )
    skey = skey.setNum( pos);
  else
    skey = QString("0") + skey.setNum( pos);

  return skey.data();

}

const char* ToolBarItem::text ( int )
{
  return data.name.data();
}

void ToolBarItem::setIcon( QString fname )
{
   data.iconFile = fname;
   data.pix = QPixmap(fname);
   if ( data.pix.isNull() )
      data.pix = Icon("ball.xpm");
   setPixmap( 0, data.pix );
}

void ToolBarItem::setText( QString text )
{
   data.name = text;
   QListViewItem::setText( 0,text.data() );
}

void ToolBarItem::setText ( int i, const char * text)
{
  QListViewItem::setText( i, text );
}