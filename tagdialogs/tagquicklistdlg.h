/***************************************************************************
                          tagquicklistdlg.h  -  description
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

#ifndef TAGQUICKLISTDLG_H
#define TAGQUICKLISTDLG_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qgroupbox.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qdialog.h>

/**
  *@author Dmitry Poplavsky & Yacovlev Alexander
  */

class TagQuickListDlg : public QDialog  {
   Q_OBJECT
public: 
	TagQuickListDlg(QWidget *parent=0, const char *name=0);
	~TagQuickListDlg();

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QSpinBox *spinBoxRows;
	QPushButton *buttonOk;
	QLabel *QLabel_2;
	QRadioButton *radioOrdered;
	QRadioButton *radioUnordered;
	QPushButton *buttonCancel;
	QButtonGroup *groupBox;
	//Generated area. DO NOT EDIT!!!(end)

private: 
};

#endif
