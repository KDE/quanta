/***************************************************************************
                          eventswidgetdlg.h  -  description
                             -------------------
    begin                : Thu Mar 30 2000
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

#ifndef EVENTSWIDGETDLG_H
#define EVENTSWIDGETDLG_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qwidget.h>
#include "tagwidget.h"

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class EventsWidgetDlg : public TagWidget  {
   Q_OBJECT
public: 
	EventsWidgetDlg(QWidget *parent=0, const char *name=0);
	~EventsWidgetDlg();
	
  void readAttributes( QDict<QString> *d);
  void writeAttributes( QDict<QString> *d );	

protected: 
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QLineEdit *lineClick;
	QLabel *QLabel_2;
	QLabel *QLabel_3;
	QLineEdit *lineDblClick;
	QLineEdit *lineMouseDown;
	QLabel *QLabel_4;
	QLineEdit *lineMouseUp;
	QLabel *QLabel_5;
	QLineEdit *lineMouseOver;
	QLabel *QLabel_6;
	QLineEdit *lineMouseMove;
	QLabel *QLabel_7;
	QLineEdit *lineMouseOut;
	QLabel *QLabel_8;
	QLineEdit *lineKeyPress;
	QLabel *QLabel_9;
	QLineEdit *lineKeyDown;
	QLabel *QLabel_10;
	QLineEdit *lineKeyUp;
	//Generated area. DO NOT EDIT!!!(end)

private: 
};

#endif
