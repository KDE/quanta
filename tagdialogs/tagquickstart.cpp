/***************************************************************************
                          tagquickstart.cpp  -  description
                             -------------------
    begin                : Sat Nov 27 1999
    copyright            : (C) 1999 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002 Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>

// qt includes
#include <qcolor.h>
#include <qlineedit.h>
#include <qpushbutton.h>

// kde includes
#include <kfiledialog.h>
#include <klocale.h>
#include <kurl.h>

// quanta app
#include "colorcombo.h"
#include "tagquickstart.h"
#include "../qextfileinfo.h"


TagQuickStart::TagQuickStart(const KURL& a_baseURL, QWidget *parent, const char *name)
    : TagQuickStartS(parent,name,true)
{
	baseURL = a_baseURL;
	setCaption(name);

	colorBG   ->setColor("#ffffff");
	colorText ->setColor("#000000");
	colorLink ->setColor("#0000a0");
	colorALink->setColor("#008080");
	colorVLink->setColor("#c06060");

	connect( buttonFileSelect, SIGNAL(clicked()),SLOT(slotFileSelect()) );

	connect( colorBG,    SIGNAL(activated(const QColor &)), this, SLOT(slotBGColor   (const QColor &)) );
	connect( colorText,  SIGNAL(activated(const QColor &)), this, SLOT(slotTextColor (const QColor &)) );
	connect( colorLink,  SIGNAL(activated(const QColor &)), this, SLOT(slotLinkColor (const QColor &)) );
	connect( colorALink, SIGNAL(activated(const QColor &)), this, SLOT(slotALinkColor(const QColor &)) );
	connect( colorVLink, SIGNAL(activated(const QColor &)), this, SLOT(slotVLinkColor(const QColor &)) );

	connect( comboBGColor,    SIGNAL(activated(const QString &)), this, SLOT(slotColorBG   (const QString &)) );
	connect( comboTextColor,  SIGNAL(activated(const QString &)), this, SLOT(slotColorText (const QString &)) );
	connect( comboLinkColor,  SIGNAL(activated(const QString &)), this, SLOT(slotColorLink (const QString &)) );
	connect( comboALinkColor, SIGNAL(activated(const QString &)), this, SLOT(slotColorALink(const QString &)) );
	connect( comboVLinkColor, SIGNAL(activated(const QString &)), this, SLOT(slotColorVLink(const QString &)) );

	connect( buttonOk,     SIGNAL(clicked()), SLOT(accept()) );
	connect( buttonCancel, SIGNAL(clicked()), SLOT(reject()) );
}

TagQuickStart::~TagQuickStart(){
}

/** select BG Image */
void TagQuickStart::slotFileSelect()
{

	KURL url = KFileDialog::getOpenURL( baseURL.url(), i18n("*.gif *.png *.jpg| Image files\n*|All files"));

  if ( !url.isEmpty() )
  {
    url = QExtFileInfo::toRelative(url, baseURL);
    lineBGImage->setText( url.path() );
  }
}

void TagQuickStart::slotBGColor(const QColor &)
{
	QString color = colorBG->colorName();
	comboBGColor->insertItem( (color[0]=='#') ? color.upper() : color ,0);
}

void TagQuickStart::slotTextColor(const QColor &)
{
	QString color = colorText->colorName();
	comboTextColor->insertItem( (color[0]=='#') ? color.upper() : color ,0);
}

void TagQuickStart::slotLinkColor(const QColor &)
{
	QString color = colorLink->colorName();
	comboLinkColor->insertItem( (color[0]=='#') ? color.upper() : color ,0);
}

void TagQuickStart::slotALinkColor(const QColor &)
{
	QString color = colorALink->colorName();
	comboALinkColor->insertItem( (color[0]=='#') ? color.upper() : color ,0);
}

void TagQuickStart::slotVLinkColor(const QColor &)
{
	QString color = colorVLink->colorName();
	comboVLinkColor->insertItem( (color[0]=='#') ? color.upper() : color ,0);
}

void TagQuickStart::slotColorBG   (const QString &newColor){colorBG   ->setColor(QColor(newColor));}
void TagQuickStart::slotColorText (const QString &newColor){colorText ->setColor(QColor(newColor));}
void TagQuickStart::slotColorLink (const QString &newColor){colorLink ->setColor(QColor(newColor));}
void TagQuickStart::slotColorALink(const QString &newColor){colorALink->setColor(QColor(newColor));}
void TagQuickStart::slotColorVLink(const QString &newColor){colorVLink->setColor(QColor(newColor));}

#include "tagquickstart.moc"
