/***************************************************************************
                          corewidgetdlg.h  -  description
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

#ifndef COREWIDGETDLG_H
#define COREWIDGETDLG_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <kseparator.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qwidget.h>
#include <qdict.h>

#include "tagwidget.h"

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class CoreWidgetDlg : public TagWidget  {
   Q_OBJECT
public: 
	CoreWidgetDlg(QWidget *parent=0, const char *name=0);
	~CoreWidgetDlg();
	
  void readAttributes( QDict<char> *d );
  void writeAttributes( QDict<char> *d );
	
public:
	void initDialog();
  /**  */
  void disableI18nAttribs();
  /**  */
  void disableCoreAttribs();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QLabel *QLabel_2;
	QLabel *QLabel_3;
	QLabel *QLabel_4;
	QLineEdit *lineId;
	QLineEdit *lineClass;
	QLineEdit *lineStyle;
	QLineEdit *lineTitle;
	KSeparator *KSeparator_1;
	QLabel *QLabel_5;
	QLabel *QLabel_6;
	QLabel *QLabel_7;
	QLineEdit *lineLang;
	QLineEdit *lineDir;
	//Generated area. DO NOT EDIT!!!(end)

private: 
};

#endif
