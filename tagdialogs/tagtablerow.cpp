/***************************************************************************
                          tagtablerow.cpp  -  description
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
#include "tagtablerow.h"
#include <stdio.h>

extern const char *colorsList[];

TagTableRow::TagTableRow(QWidget *parent, const char *name)
  : TagWidget(parent,name)
{
	setCaption(name);
	initDialog();
	
	static const char *talign[] = { "", "left", "right", "center",0};
	static const char *tvalign[] = { "", "top", "bottom", "middle",0};
	comboAlign->insertStrList( talign);
	comboVAlign->insertStrList( tvalign);
	comboColor->insertStrList( colorsList );
	
	connect( colorButton, SIGNAL(changed(const QColor &)), this, SLOT(slotColor(const QColor &)) );
}

TagTableRow::~TagTableRow(){
}

/** slot for color selected */
void TagTableRow::slotColor(const QColor &newColor){
	char c[8];
	
	sprintf((char *)c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboColor->setEditText((char *)c);
}

void TagTableRow::readAttributes( QDict<char> *d )
{
  dict = d;

  updateDict("align",   comboAlign );
  updateDict("valign",  comboVAlign );
  updateDict("bgcolor", comboColor );
}

void TagTableRow::writeAttributes( QDict<char> *d )
{
  dict = d;
  char *t; // value of attr.

  if (( t=d->find("align") ))   setValue(t, comboAlign);
  if (( t=d->find("valign") ))  setValue(t, comboVAlign);
  if (( t=d->find("bgcolor") )) {
    setValue(t, comboColor);
    setValue(t, colorButton);
  }
}
