/***************************************************************************
                          tagimgdlg.cpp  -  description
                             -------------------
    begin                : Sat Nov 27 1999
    copyright            : (C) 1999 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002, 2006 Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//qt includes
#include <qspinbox.h>
#include <qimage.h>
//kde includes
#include <kapplication.h>
#include <klocale.h>
#include <kio/netaccess.h>
#include <kfiledialog.h>
#include <kurl.h>

//app includes
#include "pictureview.h"
#include "qextfileinfo.h"
#include "quantacommon.h"
#include "tagdialog.h"
#include "tagimgdlg.h"
#include "tagimgdlg.moc"


TagImgDlg::TagImgDlg(const DTDStruct *dtd, QWidget *parent, const char *name)
  : QWidget(parent,name), TagWidget(parent,name)
{
  m_dtd = dtd;
  img = 0L;
  isTmpFile = false;
  setCaption(name);
  initDialog();


  connect( buttonImgSource, SIGNAL(clicked()), this, SLOT(slotFileSelect()) );
  connect( buttonRecalcImgSize, SIGNAL(clicked()), this, SLOT(slotRecalcImgSize()) );
  connect( lineImgSource, SIGNAL( returnPressed()), this, SLOT(slotLineFileSelect()) );
}

TagImgDlg::~TagImgDlg()
{
  if (isTmpFile)
  {
    KIO::NetAccess::removeTempFile(imgFileName);
    isTmpFile = false;
  }
}

/** Choose new image */
void TagImgDlg::slotImageSet(const KURL& imageURL)
{
  if (isTmpFile)
  {
    KIO::NetAccess::removeTempFile(imgFileName);
    isTmpFile = false;
  }
  if (imageURL.isLocalFile())
  {
    imgFileName = imageURL.path();
  } else
  {
    KIO::NetAccess::download(imageURL, imgFileName, this); 
    isTmpFile = true; 
  }
  widgetImg->slotSetImage(imgFileName);
  
  img = new QImage(imgFileName);
  if (!img->isNull() && lineWidth && lineHeight)
  {
    QString s;
    s.setNum(img->width());
    lineWidth->setText(s);
    s.setNum(img->height());
    lineHeight->setText(s);
  }
}

/** recalculate image size */
void TagImgDlg::slotRecalcImgSize()
{
  if (img && lineWidth && lineHeight) {
    QString s;
    s.setNum( img->width() );
    lineWidth->setText( s );
    s.setNum( img->height() );
    lineHeight->setText( s );
  }
}

/** select image */
void TagImgDlg::slotFileSelect()
{
//TODO: Implemented only for local files, but maybe it is enough
 KURL url = KFileDialog::getOpenURL( baseURL.url(), "*.gif *.jpg *.png *.jpeg *.bmp *.GIF *.JPG *.PNG *.JPEG *.BMP"+i18n("|Image Files\n*|All Files"));
 if ( !url.isEmpty() )
  {
    slotImageSet( url );
    url = QExtFileInfo::toRelative(url, baseURL);
    lineImgSource->setText( url.path() );
  }
}

void TagImgDlg::slotLineFileSelect()
{
  KURL url;
  QuantaCommon::setUrl(url, lineImgSource->text());
  slotImageSet(url);
}

void TagImgDlg::readAttributes( QDict<QString> *d )
{
  dict = d;

  updateDict("src",   lineImgSource );
  updateDict("alt",   lineAltText );
  if (lineWidth)
    updateDict("width", lineWidth );
  if (lineHeight)
    updateDict("height",lineHeight );
  if (lineHSpace)
    updateDict("hspace",lineHSpace );
  if (lineVSpace)
    updateDict("vspace",lineVSpace );
  if (comboAlign)
    updateDict("align", comboAlign );
  if (spinBorder)
    updateDict("border",spinBorder );
}

void TagImgDlg::writeAttributes( QDict<QString> *d )
{
  dict = d;
  QString *t; // value of attr.

  if (( t=d->find("src") ))
  {
    lineImgSource->setText(*t);
    KURL url = baseURL;
    QuantaCommon::setUrl(url, *t);
    url = QExtFileInfo::toAbsolute(url, baseURL);
    slotImageSet( url );
  }
  if ( (t=d->find("alt")) )
    setValue(*t, lineAltText);
  if ( (t=d->find("width")) && lineWidth )
    setValue(*t, lineWidth);
  if ( (t=d->find("height")) && lineHeight )
    setValue(*t, lineHeight);
  if ( (t=d->find("hspace")) && lineHSpace)
    setValue(*t, lineHSpace);
  if ( (t=d->find("vspace")) && lineVSpace )
    setValue(*t, lineVSpace);
  if ( (t=d->find("align")) && comboAlign )
    setValue(*t, comboAlign);
  if ( (t=d->find("border")) && spinBorder )
    setValue(*t, spinBorder);
}
