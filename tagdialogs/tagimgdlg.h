/***************************************************************************
                          tagimgdlg.h  -  description
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

#ifndef TAGIMGDLG_H
#define TAGIMGDLG_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>
//Generated area. DO NOT EDIT!!!(end)

#include "tagwidget.h"

class PictureView;
class QSpinBox;
class QImage;

/**
  *@author Dmitry Poplavsky & Yacovlev Alexander
  */

class TagImgDlg : public TagWidget  {
   Q_OBJECT
public: 
	TagImgDlg(QWidget *parent=0, const char *name=0);
	~TagImgDlg();

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *labelImgSource;
	QLineEdit *lineImgSource;
	QPushButton *buttonImgSource;
	QPushButton *buttonRecalcImgSize;
	QLabel *QLabel_4;
	QLineEdit *lineWidth;
	QLabel *QLabel_5;
	QLineEdit *lineHeight;
	QLabel *QLabel_6;
	QLineEdit *lineHSpace;
	QLabel *QLabel_7;
	QLineEdit *lineVSpace;
	QLabel *QLabel_8;
	QLineEdit *lineAltText;
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
public slots: // Public slots
  /** Choose new image */
  void slotImageSet( const char * file);
  /** select image */
  void slotFileSelect();
  void slotLineFileSelect();

  void readAttributes( QDict<char> *d );
  void writeAttributes( QDict<char> *d );
  /** recalculate image size */
  void slotRecalcImgSize();
};

#endif

