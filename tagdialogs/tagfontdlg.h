/***************************************************************************
                          tagfontdlg.h  -  description
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

#ifndef TAGFONTDLG_H
#define TAGFONTDLG_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <kcolorbtn.h>
#include <qpushbutton.h>
//Generated area. DO NOT EDIT!!!(end)

#include "tagwidget.h"

/**
  *@author Dmitry Poplavsky & Yacovlev Alexander
  */

class TagFontDlg : public TagWidget  {
   Q_OBJECT
public: 
	TagFontDlg(QWidget *parent=0, const char *name=0);
	~TagFontDlg();
	
	void readAttributes( QDict<QString> *d);
  void writeAttributes( QDict<QString> *d );

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QComboBox *comboIncDec;
	QLabel *QLabel_3;
	QComboBox *comboColor;
	KColorButton *colorButton;
	QLabel *QLabel_4;
	QComboBox *comboFont;
	QPushButton *buttonOk;
	QPushButton *buttonCancel;
	//Generated area. DO NOT EDIT!!!(end)

private: 
public slots: // Public slots
  /** connect to colorButton,
change value of  comboColor */
  void slotColor(const QColor &newColor);
};

#endif
