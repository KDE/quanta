/***************************************************************************
                          tagtabledlg.cpp  -  description
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
#include "tagtabledlg.h"
#include <stdio.h>

extern const char *colorsList[];

TagTableDlg::TagTableDlg(QWidget *parent, const char *name)
    : TagWidget(parent,name)
{
	setCaption(name);
	initDialog();
	
	static const char *talign[] = { "", "left", "right", "center",0};
	comboAlign->insertStrList( talign);
	comboBGColor->insertStrList( colorsList );
	
	connect( colorButton, SIGNAL(changed(const QColor &)), this, SLOT(slotColor(const QColor &)) );
}

TagTableDlg::~TagTableDlg(){
}

/** for select color */
void TagTableDlg::slotColor(const QColor &newColor){
	char c[8];
	
	sprintf((char *)c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboBGColor->setEditText((char *)c);
}

void TagTableDlg::readAttributes( QDict<char> *d )
{
  dict = d;

  updateDict("width",       lineWidth );
  updateDict("border",      lineBorder );
  updateDict("cellspacing", lineCellSpacing );
  updateDict("cellpadding", lineCellPadding );
  updateDict("align",       comboAlign );
  updateDict("bgcolor",     comboBGColor );
}

void TagTableDlg::writeAttributes( QDict<char> *d )
{
  dict = d;
  char *t; // value of attr.

  if (( t=d->find("width") ))       setValue(t, lineWidth);
  if (( t=d->find("border") ))      setValue(t, lineBorder);
  if (( t=d->find("cellspacing") )) setValue(t, lineCellPadding);
  if (( t=d->find("cellpadding") )) setValue(t, lineCellSpacing);
  if (( t=d->find("align") ))       setValue(t, comboAlign);
  if (( t=d->find("bgcolor") )) {
    setValue(t, comboBGColor);
    setValue(t, colorButton);
  }
}
