/***************************************************************************
                          tagfontdlg.cpp  -  description
                             -------------------
    begin                : Sat Nov 27 1999
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
#include "tagwidget.h"
#include "tagfontdlg.h"
#include <stdio.h>

const char *colorsList[] = {"","black","silver","gray","white","maroon","red","purple",
                      "fuchsia","green","lime","olive","yellow","navy","blue",
                      "teal","aqua",0};

TagFontDlg::TagFontDlg(QWidget *parent, const char *name)
  : TagWidget(parent,name)
{
	setCaption(name);
	
	initDialog();
	
	comboColor->insertStrList( colorsList );
	comboIncDec->insertItem("");
	
  for (int i=7;i>0;i--) {
  	char c[3];
  	sprintf(c,"+%i",i);
  	comboIncDec->insertItem(c);
  }
  for (int i=0;i>-8;i--) {
  	char c[3];
  	sprintf(c,"%i",i);
  	comboIncDec->insertItem(c);
  }
	
	connect( colorButton, SIGNAL(changed(const QColor &)), this, SLOT(slotColor(const QColor &)) );
}

TagFontDlg::~TagFontDlg(){
}


/** connect to colorButton,
change value of  comboColor */
void TagFontDlg::slotColor(const QColor &newColor){
	char c[8];
	
	sprintf((char *)c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboColor->setEditText((char *)c);
}

void TagFontDlg::readAttributes( QDict<QString> *d )
{
  dict = d;

  updateDict("size",    comboIncDec );
  updateDict("color",   comboColor );
  updateDict("face",    comboFont );
}

void TagFontDlg::writeAttributes( QDict<QString> *d )
{
  dict = d;
  QString *t; // value of attr.

  if (( t=d->find("size") ))    setValue(*t, comboIncDec);
  if (( t=d->find("color") )) {
    setValue(*t, comboColor);
    setValue(*t, colorButton);
  }
  if (( t=d->find("face") ))    setValue(*t, comboFont);
}
