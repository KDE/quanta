/***************************************************************************
                          toolbardata.cpp  -  description
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

#include "toolbardata.h"

#include <qpixmap.h>
#include <qlistview.h>

#include <kfm.h>
#include <kapp.h>
#include <kiconloader.h>
#include <kfiledialog.h>


ToolBarData::ToolBarData( const char *name )
{
  isSeparator = false;
  pos = 0;
  this->name = name ? name : "";
  iconFile = "";
  pix = Icon("ball.xpm");
  type = Tag;
  input= None;
  output = None;
  tag = "";
  endTag = "";
  script = "";
}

ToolBarData::~ToolBarData(){
}
