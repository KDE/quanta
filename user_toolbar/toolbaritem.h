/***************************************************************************
                          toolbaritem.h  -  description
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

#ifndef TOOLBARITEM_H
#define TOOLBARITEM_H

#include <qwidget.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qlistview.h>

#include "toolbardata.h"

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class ToolBarItem : public QListViewItem  {

public:
	ToolBarItem(QListView *parent=0, const char *name=0, int pos =0);
	~ToolBarItem();
	
	virtual const char* key ( int, bool ) const;
	const char* text ( int );
	
  QPixmap icon();
  void setIcon( QString fname );
  void setText( QString text );
  virtual void setText ( int, const char * );

  int pos;
  ToolBarData data;

};

#endif
