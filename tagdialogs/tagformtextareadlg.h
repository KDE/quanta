/***************************************************************************
                          tagformtextareadlg.h  -  description
                             -------------------
    begin                : Sun Apr 2 2000
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

#ifndef TAGFORMTEXTAREADLG_H
#define TAGFORMTEXTAREADLG_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <klineedit.h>
//Generated area. DO NOT EDIT!!!(end)

#include "tagwidget.h"

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class TagFormTextareaDlg : public TagWidget  {
   Q_OBJECT
public: 
	TagFormTextareaDlg(QWidget *parent=0, const char *name=0);
	~TagFormTextareaDlg();
	
	void readAttributes( QDict<QString> *d);
  void writeAttributes( QDict<QString> *d );

protected: 
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QLabel *QLabel_2;
	QLabel *QLabel_3;
	KLineEdit *lineName;
	KLineEdit *lineRows;
	KLineEdit *lineCols;
	//Generated area. DO NOT EDIT!!!(end)

private: 
};

#endif
