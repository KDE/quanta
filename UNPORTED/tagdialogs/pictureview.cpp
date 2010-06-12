/***************************************************************************
                          pictureview.cpp  -  description
                             -------------------
    begin                : Mon Nov 29 1999
    copyright            : (C) 1999 by Dmitry Poplavsky & Yacovlev Alexander
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

#include "pictureview.h"
#include "pictureview.moc"
#include "qpainter.h"
#include <qimage.h>

PictureView::PictureView(QWidget *parent, char *file, const char *name ) : QFrame(parent,name)
{
    if ( file ) {
      pix = new QImage(file);
      picwidth  = pix->width();
      picheight = pix->height();
      x_of = 0;
      y_of = 0;
      scale();
    } else {
      pix = new QImage();
      picwidth  = 0;
      picheight = 0;
      x_of = 0;
      y_of = 0;
    }


    setFrameStyle ( Box|Sunken );
}

PictureView::~PictureView()
{
}

void PictureView::paintEvent( QPaintEvent * e )
{
    QPainter p;
    p.begin( this );
    p.drawImage( x_of, y_of, *pix );
    p.end();

    QFrame::paintEvent(e);

}

void PictureView::resizeEvent ( QResizeEvent *  e){
  scale();
  QFrame::resizeEvent(e);
}

/** set image */
void PictureView::slotSetImage(const QString& file)
{
  pix->load( file );
  picwidth  = pix->width();
  picheight = pix->height();

  scale();
  repaint();
}

/** try scale  image */
void PictureView::scale(){
  float width_ot,height_ot;

  width_ot  = 1;
  height_ot = 1;
  x_of = y_of = 0;

  if ( picwidth  > size().width()  ) width_ot  = (float)size().width()/(float)picwidth;
    else x_of = (size().width()-picwidth)/2;
  if ( picheight > size().height() ) height_ot = (float)size().height()/(float)picheight;
    else y_of = (size().height()-picheight)/2;

  if (  width_ot < 1 || height_ot < 1) {
    if ( width_ot < height_ot) {
      *pix = pix->smoothScale( (int)(width_ot*picwidth), (int)(width_ot*picheight));
      y_of = ( size().height()-(int)(width_ot*picheight) )/2;
    }
    else {
      *pix = pix->smoothScale( (int)(height_ot*picwidth), (int)(height_ot*picheight));
      x_of = ( size().width()-(int)(height_ot*picwidth) )/2;
    }

    picwidth  = size().width();
    picheight = size().height();
  }


}
