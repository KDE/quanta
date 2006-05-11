/***************************************************************************
                          tagimgdlg.cpp  -  description
                             -------------------
    begin                : Sat Nov 27 1999
    copyright            : (C) 1999 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002 Andras Mantia
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
#include <QImage>
//kde includes
#include <kapplication.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kio/netaccess.h>
#include <kfiledialog.h>
#include <kurl.h>

//app includes
#include "pictureview.h"
#include "extfileinfo.h"
#include "tagdialog.h"
#include "tagimgdlg.h"

TagImgDlg::TagImgDlg(QWidget *parent)
  : QWidget(parent), TagWidget(parent)
{
  img = 0L;
  isTmpFile = false;
//  setWindowTitle(name);
  initDialog();

  buttonImgSource->setIcon( SmallIcon("fileopen") );
  buttonRecalcImgSize->setIcon( SmallIcon("reload") );
  QStringList align;
  align << "" << "left" << "right" << "top" << "bottom" << "middle";
  comboAlign->addItems(align);

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
void TagImgDlg::slotImageSet(const KUrl& imageURL)
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
  if (!img->isNull())
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
  if (img) {
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
 KUrl url = KFileDialog::getOpenURL( baseURL.url(), "*.gif *.jpg *.png *.jpeg *.bmp *.GIF *.JPG *.PNG *.JPEG *.BMP"+i18n("|Image Files\n*|All Files"));
 if ( !url.isEmpty() )
  {
    slotImageSet( url );
    url = KUrl::relativeURL(baseURL, url);
    lineImgSource->setText( url.path() );
  }
}

void TagImgDlg::slotLineFileSelect()
{
  slotImageSet(KUrl::fromPathOrURL(lineImgSource->text()));
}

void TagImgDlg::readAttributes( QHash<QString, QString*> *d )
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

void TagImgDlg::writeAttributes( QHash<QString, QString*> *d )
{
  dict = d;
  QString *t; // value of attr.

  if (( t=d->value("src") ))
  {
    lineImgSource->setText(*t);
    KUrl url = baseURL;
    url.addPath(*t);
    slotImageSet( url );
  }
  if (( t=d->value("alt") ))     setValue(*t, lineAltText);
  if (( t=d->value("width") ))   setValue(*t, lineWidth);
  if (( t=d->value("height") ))  setValue(*t, lineHeight);
  if (( t=d->value("hspace") ))  setValue(*t, lineHSpace);
  if (( t=d->value("vspace") ))  setValue(*t, lineVSpace);
  if (( t=d->value("align") ))   setValue(*t, comboAlign);
  if (( t=d->value("border") ))  setValue(*t, spinBorder);
}

#include "tagimgdlg.moc"
