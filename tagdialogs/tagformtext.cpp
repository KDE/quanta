/***************************************************************************
                          tagformtext.cpp  -  description
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
#include "tagformtext.h"

TagFormText::TagFormText(QWidget *parent, const char *name)
  : TagWidget(parent,name)
{
	setCaption(name);
	initDialog();
}

TagFormText::~TagFormText(){
}

void TagFormText::readAttributes( QDict<char> *d )
{
  dict = d;

  updateDict("name",      lineName );
  updateDict("value",     lineValue );
  updateDict("size",      lineSize );
  updateDict("maxlength", lineMaxLen );
}

void TagFormText::writeAttributes( QDict<char> *d )
{
  dict = d;
  char *t; // value of attr.

  if (( t=d->find("name") ))      setValue(t, lineName);
  if (( t=d->find("value") ))     setValue(t, lineValue);
  if (( t=d->find("size") ))      setValue(t, lineSize);
  if (( t=d->find("maxlength") )) setValue(t, lineMaxLen);
}
