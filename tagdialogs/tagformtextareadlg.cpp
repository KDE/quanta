/***************************************************************************
                          tagformtextareadlg.cpp  -  description
                             -------------------
    begin                : Sun Apr 2 2000
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
#include "tagformtextareadlg.h"

TagFormTextareaDlg::TagFormTextareaDlg(QWidget *parent, const char *name)
  : TagWidget(parent,name)
{
	initDialog();
}

TagFormTextareaDlg::~TagFormTextareaDlg(){
}

void TagFormTextareaDlg::readAttributes( QDict<char> *d )
{
  dict = d;

  updateDict("name", lineName );
  updateDict("rows", lineRows );
  updateDict("cols", lineCols );
}

void TagFormTextareaDlg::writeAttributes( QDict<char> *d )
{
  dict = d;
  char *t; // value of attr.

  if (( t=d->find("name") ))    setValue(t, lineName);
  if (( t=d->find("rows") ))    setValue(t, lineRows);
  if (( t=d->find("cols") ))    setValue(t, lineCols);
}
