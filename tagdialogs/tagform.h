/***************************************************************************
                          tagform.h  -  description
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

#ifndef TAGFORM_H
#define TAGFORM_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <klineedit.h>
#include <qcombobox.h>
//Generated area. DO NOT EDIT!!!(end)

#include "tagwidget.h"

/**
  *@author Yacovlev Alexander & Dmitry Poplavski
  */

class TagForm : public TagWidget  {
   Q_OBJECT
public: 
	TagForm(QWidget *parent=0, const char *name=0);
	~TagForm();
	
	void readAttributes( QDict<QString> *d);
  void writeAttributes( QDict<QString> *d );

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	KLineEdit *lineAction;
	QLabel *QLabel_2;
	QComboBox *comboMethod;
	QLabel *QLabel_3;
	QComboBox *comboEnctype;
	QLabel *QLabel_4;
	QComboBox *comboTarget;
	//Generated area. DO NOT EDIT!!!(end)

private: 
};

#endif
