/***************************************************************************
                          tagmaildlg.h  -  description
                             -------------------
    begin                : Fri Nov 26 1999
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

#ifndef TAGMAILDLG_H
#define TAGMAILDLG_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qlabel.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qdialog.h>

/**
  *@author Dmitry Poplavsky & Yacovlev Alexander
  */

class TagMailDlg : public QDialog  {
   Q_OBJECT
public: 
	TagMailDlg(QString email, QWidget *parent=0, const char *name=0);
	~TagMailDlg();

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QComboBox *comboBoxMail;
	QPushButton *QPushButtonCancel;
	QLabel *QLabel_1;
	QPushButton *QPushButtonOk;
	//Generated area. DO NOT EDIT!!!(end)

};

#endif
