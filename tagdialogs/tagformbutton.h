/***************************************************************************
                          tagformbutton.h  -  description
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

#ifndef TAGFORMBUTTON_H
#define TAGFORMBUTTON_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcheckbox.h>
//Generated area. DO NOT EDIT!!!(end)

#include "tagwidget.h"

/**
  *@author Yacovlev Alexander & Dmitry Poplavski
  */

class TagFormButton : public TagWidget {
   Q_OBJECT
public: 
	TagFormButton(QWidget *parent=0, const char *name=0);
	~TagFormButton();
	
	void readAttributes( QDict<char> *d);
  void writeAttributes( QDict<char> *d );

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QLineEdit *lineName;
	QLabel *QLabel_2;
	QLineEdit *lineValue;
	QCheckBox *checkChecked;
	//Generated area. DO NOT EDIT!!!(end)

private: 
};

#endif
