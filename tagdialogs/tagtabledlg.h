/***************************************************************************
                          tagtabledlg.h  -  description
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

#ifndef TAGTABLEDLG_H
#define TAGTABLEDLG_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <kcolorbtn.h>
#include <qpushbutton.h>
//Generated area. DO NOT EDIT!!!(end)

#include "tagwidget.h"

/**
  *@author Dmitry Poplavsky & Yacovlev Alexander
  */

class TagTableDlg : public TagWidget  {
   Q_OBJECT
public: 
	TagTableDlg(QWidget *parent=0, const char *name=0);
	~TagTableDlg();
	
	void readAttributes( QDict<QString> *d);
  void writeAttributes( QDict<QString> *d );

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QLineEdit *lineCellPadding;
	QLabel *QLabel_2;
	QComboBox *comboAlign;
	QLabel *QLabel_3;
	QLineEdit *lineWidth;
	QLabel *QLabel_4;
	QLineEdit *lineCellSpacing;
	QLabel *QLabel_5;
	QLineEdit *lineBorder;
	QLabel *QLabel_6;
	QLabel *QLabel_7;
	QComboBox *comboBGColor;
	KColorButton *colorButton;
	//Generated area. DO NOT EDIT!!!(end)

private: 
public slots: // Public slots
  /** for select color */
  void slotColor(const QColor &newColor);
};

#endif
