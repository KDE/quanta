/***************************************************************************
                          tagquickstart.cpp  -  description
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
#include "tagquickstart.h"
#include "../qextfileinfo.h"

#include <kfiledialog.h>
#include <qcolor.h>
#include <stdio.h>

TagQuickStart::TagQuickStart(QString basePath, QWidget *parent, const char *name)
    : QDialog(parent,name,true)
{
	this->basePath = basePath;
	setCaption(name);
	
	initDialog();
	
	cButtonBGColor    ->setColor("#ffffff");
	cButtonTextColor  ->setColor("#000000");
	cButtonLinkColor  ->setColor("#0000a0");
	cButtonALinkColor ->setColor("#008080");
	cButtonVLinkColor ->setColor("#c06060");
	
	connect( buttonOk, SIGNAL(clicked()), SLOT(accept()) );
	connect( buttonCancel, SIGNAL(clicked()), SLOT(reject()) );
	connect( buttonFileSelect, SIGNAL(clicked()), this, SLOT(slotFileSelect()) );
	connect( cButtonBGColor, SIGNAL(changed(const QColor &)), this, SLOT(slotBGColor(const QColor &)) );
	connect( cButtonTextColor, SIGNAL(changed(const QColor &)), this, SLOT(slotTextColor(const QColor &)) );
	connect( cButtonLinkColor, SIGNAL(changed(const QColor &)), this, SLOT(slotLinkColor(const QColor &)) );
	connect( cButtonALinkColor, SIGNAL(changed(const QColor &)), this, SLOT(slotALinkColor(const QColor &)) );
	connect( cButtonVLinkColor, SIGNAL(changed(const QColor &)), this, SLOT(slotVLinkColor(const QColor &)) );
}

TagQuickStart::~TagQuickStart(){
}

/** select BG Image */
void TagQuickStart::slotFileSelect()
{
	QString fileName = KFileDialog::getOpenFileName( basePath, "*.gif *.png *.jpg| Image files\n*|All files");
	if (fileName.isEmpty()) return;
		
	QExtFileInfo file(fileName);
	file.convertToRelative( basePath );
	QString shortName = file.filePath();
	lineBGImage->setText(shortName);
}

/** insert BG color in combo */
void TagQuickStart::slotBGColor(const QColor &newColor){
	char c[8];
	
	sprintf(c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboBGColor->setEditText((char *)c);
}

/** insert text color in combo */
void TagQuickStart::slotTextColor(const QColor &newColor){
	char c[8];
	
	sprintf(c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboTextColor->setEditText((char *)c);
}

/** insert link color in combo */
void TagQuickStart::slotLinkColor(const QColor &newColor){
	char c[8];
	
	sprintf(c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboLinkColor->setEditText((char *)c);
}

/** insert active link color in combo */
void TagQuickStart::slotALinkColor(const QColor &newColor){
	char c[8];
	
	sprintf(c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboALinkColor->setEditText((char *)c);
}

/** insert visited link color in combo */
void TagQuickStart::slotVLinkColor(const QColor &newColor){
	char c[8];
	
	sprintf(c,"#%2X%2X%2X",newColor.red(),newColor.green(),newColor.blue());
	for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
	
	comboVLinkColor->setEditText((char *)c);
}
