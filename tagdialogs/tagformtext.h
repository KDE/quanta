/***************************************************************************
                          tagformtext.h  -  description
                             -------------------
    begin                : Mon Feb 21 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski
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

#ifndef TAGFORMTEXT_H
#define TAGFORMTEXT_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
//Generated area. DO NOT EDIT!!!(end)

#include "tagwidget.h"

/**
  *@author Yacovlev Alexander & Dmitry Poplavski
  */

class TagFormText : public TagWidget {
   Q_OBJECT
public: 
	TagFormText(QWidget *parent=0, const char *name=0);
	~TagFormText();
	
	void readAttributes( QDict<QString> *d);
  void writeAttributes( QDict<QString> *d );

protected: 
	void initDialog();
public:	
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QLineEdit *lineName;
	QLabel *QLabel_2;
	QLineEdit *lineValue;
	QLabel *QLabel_3;
	QLineEdit *lineSize;
	QLabel *QLabel_4;
	QLineEdit *lineMaxLen;
	//Generated area. DO NOT EDIT!!!(end)

private: 
};

#endif
