/***************************************************************************
                          filesmask.h  -  description
                             -------------------
    begin                : Fri May 19 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
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

#ifndef FILESMASK_H
#define FILESMASK_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <kseparator.h>
#include <qlineedit.h>
#include <qpushbutton.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qwidget.h>

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class FilesMask : public QWidget  {
   Q_OBJECT
public: 
	FilesMask(QWidget *parent=0, const char *name=0);
	~FilesMask();

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QLabel *QLabel_2;
	KSeparator *KSeparator_1;
	QLabel *QLabel_3;
	QLabel *QLabel_4;
	QLabel *QLabel_5;
	QLineEdit *lineHTML;
	QLineEdit *linePHP;
	QLineEdit *lineImages;
	QLineEdit *lineText;
	QPushButton *buttonDefault;
	//Generated area. DO NOT EDIT!!!(end)

public slots:
  /** set masks to default */
  void setToDefault();
};

#endif
