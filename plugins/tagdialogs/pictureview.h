/***************************************************************************
                          pictureview.h  -  description
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

#ifndef PICTUREVIEW_H
#define PICTUREVIEW_H

#include <qwidget.h>
#include <qframe.h>
class QImage;

/**Class for images preview
used by TagImgDlg
  *@author Dmitry Poplavsky & Yacovlev Alexander
  */

class PictureView : public QFrame  {
   Q_OBJECT
public:
  PictureView(QWidget *parent=0, char *file=0, const char *name=0);
  /** try scale  image */
  void scale();
public slots: // Public slots
  /** set image */
  void slotSetImage(const QString& file);
protected: // Protected methods
  /**  */
  virtual void resizeEvent ( QResizeEvent *  e);
  ~PictureView();

protected:
  virtual void paintEvent( QPaintEvent * );

private: // Private attributes
  /** height of image */
  int picheight;
  int x_of;
private: // Private attributes
  /** width of image */
  int picwidth;
  int y_of;

  QImage *pix;
};

#endif
