/***************************************************************************
                          tagimgdlg.cpp  -  description
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
#include "tagimgdlg.h"
#include <qspinbox.h>

#include <kapp.h>
#include <kiconloader.h>
#include <kfiledialog.h>

#include "pictureview.h"
#include "../qextfileinfo.h"

#include "tagdialog.h"

static const char *align[] = { "", "left", "right", "top", "bottom", "middle",0};

TagImgDlg::TagImgDlg(QWidget *parent, const char *name)
    : TagWidget(parent,name)
{
	img = 0L;
	setCaption(name);
	initDialog();
	
	buttonImgSource     ->setPixmap( UserIcon("open") );
	buttonRecalcImgSize ->setPixmap( UserIcon("repaint") );
	comboAlign       		->insertStrList(align);
	
	connect( buttonImgSource, SIGNAL(clicked()), this, SLOT(slotFileSelect()) );
	connect( buttonRecalcImgSize, SIGNAL(clicked()), this, SLOT(slotRecalcImgSize()) );
	connect( lineImgSource, SIGNAL( returnPressed()), this, SLOT(slotLineFileSelect()) );
}

TagImgDlg::~TagImgDlg(){
}

/** Choose new image */
void TagImgDlg::slotImageSet( const char * file){
	widgetImg->slotSetImage( (char *)file);
	
	img = new QImage(file);
	QString filename = file;
	if ( img->isNull() ) {
		return;
	}
	
	QString s;
	s.setNum( img->width() );
	lineWidth->setText( s.data() );
	s.setNum( img->height() );
	lineHeight->setText( s.data() );
	
}

/** recalculate image size */
void TagImgDlg::slotRecalcImgSize()
{
  if ( img ) {
    QString s;
	  s.setNum( img->width() );
	  lineWidth->setText( s.data() );
	  s.setNum( img->height() );
	  lineHeight->setText( s.data() );
	}
}

/** select image */
void TagImgDlg::slotFileSelect()
{
		QString fileName = KFileDialog::getOpenFileName( basePath, "*.gif *.jpg *.png *.jpeg *.bmp *.GIF *.JPG *.PNG *.JPEG *.BMP|Image files\n*|All files");
		if (fileName.isEmpty()) return;
		
		QExtFileInfo file(fileName);
		file.convertToRelative( basePath );
		QString shortName = file.filePath();
		lineImgSource->setText(shortName);
		
		slotImageSet( fileName.data() );
}

void TagImgDlg::slotLineFileSelect(){
		slotImageSet( lineImgSource->text().data() );
}

void TagImgDlg::readAttributes( QDict<char> *d )
{
  dict = d;

  updateDict("src",   lineImgSource );
  updateDict("alt",   lineAltText );
  updateDict("width", lineWidth );
  updateDict("height",lineHeight );
  updateDict("hspace",lineHSpace );
  updateDict("vspace",lineVSpace );
  updateDict("align", comboAlign );
  updateDict("border",spinBorder );
}

void TagImgDlg::writeAttributes( QDict<char> *d )
{
  dict = d;
  char *t; // value of attr.

  if (( t=d->find("src") ))     {
    lineImgSource   ->setText(t);
    slotImageSet( basePath  + "/" + t );
  }
  if (( t=d->find("alt") ))     setValue(t, lineAltText);
  if (( t=d->find("width") ))   setValue(t, lineWidth);
  if (( t=d->find("height") ))  setValue(t, lineHeight);
  if (( t=d->find("hspace") ))  setValue(t, lineHSpace);
  if (( t=d->find("vspace") ))  setValue(t, lineVSpace);
  if (( t=d->find("align") ))   setValue(t, comboAlign);
  if (( t=d->find("border") ))  setValue(t, spinBorder);
}
