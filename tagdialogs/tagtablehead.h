/***************************************************************************
                          tagtablehead.h  -  description
                             -------------------
    begin                : Sun Nov 28 1999
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

#ifndef TAGTABLEHEAD_H
#define TAGTABLEHEAD_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <kcolorbtn.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
//Generated area. DO NOT EDIT!!!(end)

#include "tagwidget.h"

/**
  *@author Dmitry Poplavsky & Yacovlev Alexander
  */

class TagTableHead : public TagWidget  {
   Q_OBJECT
public: 
	TagTableHead(QWidget *parent=0, const char *name=0);
	~TagTableHead();
	
	void readAttributes( QDict<char> *d);
  void writeAttributes( QDict<char> *d );

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QLineEdit *lineWidth;
	QLabel *QLabel_2;
	QComboBox *comboAlign;
	QLabel *QLabel_3;
	QLineEdit *lineColSpan;
	QLabel *QLabel_4;
	QLineEdit *lineHeight;
	QLabel *QLabel_5;
	QComboBox *comboVAlign;
	QLabel *QLabel_6;
	QLineEdit *lineRowSpan;
	QLabel *QLabel_7;
	QComboBox *comboBGColor;
	KColorButton *colorButton;
	//Generated area. DO NOT EDIT!!!(end)

private: 
public slots: // Public slots
  /** for color select */
  void slotColor(const QColor &newColor);
};

#endif
