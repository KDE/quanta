/***************************************************************************
                          tagquicktable.h  -  description
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

#ifndef TAGQUICKTABLE_H
#define TAGQUICKTABLE_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qdialog.h>

/**
  *@author Dmitry Poplavsky & Yacovlev Alexander
  */

class TagQuickTable : public QDialog  {
   Q_OBJECT
public: 
	TagQuickTable(QWidget *parent=0, const char *name=0);
	~TagQuickTable();

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QSpinBox *spinBoxRows;
	QSpinBox *spinBoxCollums;
	QLabel *QLabel_1;
	QLabel *QLabel_2;
	QPushButton *buttonOk;
	QPushButton *buttonCancel;
	//Generated area. DO NOT EDIT!!!(end)

private: 
};

#endif
