/***************************************************************************
                          tagform.cpp  -  description
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
#include "tagform.h"

TagForm::TagForm(QWidget *parent, const char *name)
  : TagWidget(parent,name)
{
	setCaption(name);
	initDialog();
}

TagForm::~TagForm(){
}

void TagForm::readAttributes( QDict<QString> *d )
{
  dict = d;

  updateDict("action",  lineAction );
  updateDict("method",  comboMethod );
  updateDict("enctype", comboEnctype );
  updateDict("target",  comboTarget );
}

void TagForm::writeAttributes( QDict<QString> *d )
{
  dict = d;
  QString *t; // value of attr.

  if (( t=d->find("action") ))  setValue(*t, lineAction);
  if (( t=d->find("method") ))  setValue(*t, comboMethod);
  if (( t=d->find("enctype") )) setValue(*t, comboEnctype);
  if (( t=d->find("target") ))  setValue(*t, comboTarget);
}
