/***************************************************************************
                          tagbody.cpp  -  description
                             -------------------
    begin                : Mon Apr 3 2000
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
#include "tagbody.h"
#include "../qextfileinfo.h"

#include <kfiledialog.h>
#include <qcolor.h>
#include <stdio.h>

extern const char *colorsList[];

TagBody::TagBody(QWidget *parent, const char *name) : TagWidget(parent,name)
{
	initDialog();
	
	setCaption(name);
	initDialog();
	
	comboALinkColor ->insertStrList( colorsList );
	comboBGColor    ->insertStrList( colorsList );
	comboLinkColor  ->insertStrList( colorsList );
	comboTextColor  ->insertStrList( colorsList );
	comboVLinkColor ->insertStrList( colorsList );
	
	connect( buttonFileSelect, SIGNAL(clicked()), this, SLOT(slotFileSelect()) );
	connect( cButtonBGColor, SIGNAL(changed(const QColor &)), this, SLOT(slotBGColor(const QColor &)) );
	connect( cButtonTextColor, SIGNAL(changed(const QColor &)), this, SLOT(slotTextColor(const QColor &)) );
	connect( cButtonLinkColor, SIGNAL(changed(const QColor &)), this, SLOT(slotLinkColor(const QColor &)) );
	connect( cButtonALinkColor, SIGNAL(changed(const QColor &)), this, SLOT(slotALinkColor(const QColor &)) );
	connect( cButtonVLinkColor, SIGNAL(changed(const QColor &)), this, SLOT(slotVLinkColor(const QColor &)) );
	
}

TagBody::~TagBody(){
}

void TagBody::readAttributes( QDict<char> *d )
{
  dict = d;

  updateDict("background",  lineBGImage );
  updateDict("bgcolor",     comboBGColor );
  updateDict("text",        comboTextColor );
  updateDict("link",        comboLinkColor );
  updateDict("vlink",       comboVLinkColor );
  updateDict("alink",       comboALinkColor );
  updateDict("onload",      lineOnLoad );
  updateDict("onunload",    lineOnUnload );
}

void TagBody::writeAttributes( QDict<char> *d )
{
  dict = d;
  char *t; // value of attr.

  if (( t=d->find("background") ))  setValue(t, lineBGImage);
  if (( t=d->find("bgcolor") )) {
    setValue(t, comboBGColor);
    setValue(t, cButtonBGColor);
  }
  if (( t=d->find("text") )) {
    setValue(t, comboTextColor);
    setValue(t, cButtonTextColor);
  }
  if (( t=d->find("link") )) {
    setValue(t, comboLinkColor);
    setValue(t, cButtonLinkColor);
  }
  if (( t=d->find("vlink") )) {
    setValue(t, comboVLinkColor);
    setValue(t, cButtonVLinkColor);
  }
  if (( t=d->find("alink") )) {
    setValue(t, comboALinkColor);
    setValue(t, cButtonALinkColor);
  }
  if (( t=d->find("onload") ))      setValue(t, lineOnLoad);
  if (( t=d->find("onunload") ))    setValue(t, lineOnUnload);
}

/** select BG Image */
void TagBody::slotFileSelect()
{
	QString fileName = KFileDialog::getOpenFileName( basePath, "*.gif *.png *.jpg| Image files\n*|All files");
	if (fileName.isEmpty()) return;
		
	QExtFileInfo file(fileName);
	file.convertToRelative( basePath );
	QString shortName = file.filePath();
	lineBGImage->setText(shortName);
}

/** insert BG color in combo */
void TagBody::slotBGColor(const QColor &newColor){
	char c[8];
	
	sprintf(c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboBGColor->setEditText((char *)c);
}

/** insert text color in combo */
void TagBody::slotTextColor(const QColor &newColor){
	char c[8];
	
	sprintf(c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboTextColor->setEditText((char *)c);
}

/** insert link color in combo */
void TagBody::slotLinkColor(const QColor &newColor){
	char c[8];
	
	sprintf(c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboLinkColor->setEditText((char *)c);
}

/** insert active link color in combo */
void TagBody::slotALinkColor(const QColor &newColor){
	char c[8];
	
	sprintf(c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboALinkColor->setEditText((char *)c);
}

/** insert visited link color in combo */
void TagBody::slotVLinkColor(const QColor &newColor){
	char c[8];
	
	sprintf(c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboVLinkColor->setEditText((char *)c);
}
