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
#include <klineedit.h>
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
	KLineEdit *lineClick;
	QLabel *QLabel_2;
	QLabel *QLabel_3;
	KLineEdit *lineDblClick;
	KLineEdit *lineMouseDown;
	QLabel *QLabel_4;
	KLineEdit *lineMouseUp;
	QLabel *QLabel_5;
	KLineEdit *lineMouseOver;
	QLabel *QLabel_6;
	KLineEdit *lineMouseMove;
	QLabel *QLabel_7;
	KLineEdit *lineMouseOut;
	QLabel *QLabel_8;
	KLineEdit *lineKeyPress;
	QLabel *QLabel_9;
	KLineEdit *lineKeyDown;
	QLabel *QLabel_10;
	KLineEdit *lineKeyUp;
	//Generated area. DO NOT EDIT!!!(end)

private: 
};

#endif
