/***************************************************************************
                          tagadlg.h  -  description
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

#ifndef TAGADLG_H
#define TAGADLG_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <klineedit.h>
//Generated area. DO NOT EDIT!!!(end)

#include "tagwidget.h"
#include <qdict.h>

/**
  *@author Dmitry Poplavsky & Yacovlev Alexander
  */

class KWrite;

class TagADlg : public TagWidget  {
   Q_OBJECT
public: 
	TagADlg(QWidget *parent=0, const char *name=0);
	~TagADlg();

public:
	void initDialog();
  /**  */

  void readAttributes( QDict<QString> *d);
  void writeAttributes( QDict<QString> *d );

	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QComboBox *comboHREF;
	QPushButton *buttonHREF;
	QLabel *QLabel_2;
	QComboBox *comboTarget;
	QLabel *QLabel_3;
	KLineEdit *lineName;
	QLabel *QLabel_4;
	KLineEdit *lineDescription;
	//Generated area. DO NOT EDIT!!!(end)

private: 
public slots: // Public slots
  /**  */
  void slotFileSelect();
};

#endif
