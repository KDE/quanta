/***************************************************************************
                          tagtablehead.cpp  -  description
                             -------------------
    begin                : Sun Nov 28 1999
    copyright            : (C) 1999 by Yacovlev Alexander & Dmitry Poplavsky
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
#include "tagtablehead.h"
#include <stdio.h>

extern const char *colorsList[];

TagTableHead::TagTableHead(QWidget *parent, const char *name)
  : TagWidget(parent,name)
{
	setCaption(name);
	initDialog();
	
	static const char *talign[] = { "", "left", "right", "center",0};
	static const char *tvalign[] = { "", "top", "bottom", "middle",0};
	comboAlign->insertStrList( talign);
	comboVAlign->insertStrList( tvalign);
	comboBGColor->insertStrList( colorsList );
	
	connect( colorButton, SIGNAL(changed(const QColor &)), this, SLOT(slotColor(const QColor &)) );
}

TagTableHead::~TagTableHead(){
}

/** for color select */
void TagTableHead::slotColor(const QColor &newColor){
	char c[8];
	
	sprintf((char *)c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboBGColor->setEditText((char *)c);
}

void TagTableHead::readAttributes( QDict<QString> *d )
{
  dict = d;

  updateDict("rowspan", lineRowSpan );
  updateDict("colspan", lineColSpan );
  updateDict("align",   comboAlign );
  updateDict("valign",  comboVAlign );
  updateDict("width",   lineWidth );
  updateDict("height",  lineHeight );
  updateDict("bgcolor", comboBGColor);
}

void TagTableHead::writeAttributes( QDict<QString> *d )
{
  dict = d;
  QString *t; // value of attr.

  if (( t=d->find("rowspan") ))  setValue(*t, lineRowSpan);
  if (( t=d->find("colspan") ))  setValue(*t, lineColSpan);
  if (( t=d->find("align") ))    setValue(*t, comboAlign);
  if (( t=d->find("valign") ))   setValue(*t, comboVAlign);
  if (( t=d->find("width") ))    setValue(*t, lineWidth);
  if (( t=d->find("height") ))   setValue(*t, lineHeight);
  if (( t=d->find("bgcolor") )) {
    setValue(*t, comboBGColor);
    setValue(*t, colorButton);
  }
}
