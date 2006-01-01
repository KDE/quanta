/***************************************************************************
                          tagimgdlg.h  -  description
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

#ifndef TAGIMGDLG_H
#define TAGIMGDLG_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <klineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>
//Generated area. DO NOT EDIT!!!(end)

#include "tagwidget.h"
#include "qtag.h"

class PictureView;
class QSpinBox;
class QImage;

/**
  *@author Dmitry Poplavsky & Yacovlev Alexander
  */

class TagImgDlg : public QWidget, TagWidget {
   Q_OBJECT
public: 
  TagImgDlg(const DTDStruct *dtd, QWidget *parent=0, const char *name=0);
  ~TagImgDlg();

public:
  void initDialog();
  //Generated area. DO NOT EDIT!!!(begin)
  QLabel *labelImgSource;
  KLineEdit *lineImgSource;
  QPushButton *buttonImgSource;
  QPushButton *buttonRecalcImgSize;
  QLabel *QLabel_4;
  KLineEdit *lineWidth;
  QLabel *QLabel_5;
  KLineEdit *lineHeight;
  QLabel *QLabel_6;
  KLineEdit *lineHSpace;
  QLabel *QLabel_7;
  KLineEdit *lineVSpace;
  QLabel *QLabel_8;
  KLineEdit *lineAltText;
  QLabel *QLabel_9;
  QSpinBox *spinBorder;
  QLabel *QLabel_10;
  QComboBox *comboAlign;
  PictureView *widgetImg;
  QPushButton *buttonOk;
  QPushButton *buttonCancel;
  //Generated area. DO NOT EDIT!!!(end)

private:
  QImage *img;
  QString imgFileName;
  bool isTmpFile;
  const DTDStruct *m_dtd;

public slots: // Public slots
  /** Choose new image */
  void slotImageSet( const KURL& imageURL);
  /** select image */
  void slotFileSelect();
  void slotLineFileSelect();

  void readAttributes( QDict<QString> *d );
  void writeAttributes( QDict<QString> *d );
  /** recalculate image size */
  void slotRecalcImgSize();
};

#endif

