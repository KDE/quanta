/***************************************************************************
                          toolbarscriptconfigure.h  -  description
                             -------------------
    begin                : Wed Apr 12 2000
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

#ifndef TOOLBARSCRIPTCONFIGURE_H
#define TOOLBARSCRIPTCONFIGURE_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qdialog.h>

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class ToolbarScriptConfigure : public QDialog  {
   Q_OBJECT
public: 
	ToolbarScriptConfigure(QWidget *parent=0, const char *name=0);
	~ToolbarScriptConfigure();

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QButtonGroup *groupInput;
	QRadioButton *radioInputNone;
	QRadioButton *radioSelectedText;
	QRadioButton *radioCurrentDoc;
	QLabel *QLabel_2;
	QButtonGroup *groupOutput;
	QRadioButton *radioOutputNone;
	QRadioButton *radioInsInCurPos;
	QRadioButton *radioReplaceCurDoc;
	QRadioButton *radioCreateNewDoc;
	QRadioButton *radioInMesWindow;
	QPushButton *buttonOk;
	QPushButton *buttonCancel;
	//Generated area. DO NOT EDIT!!!(end)

private: 
};

#endif
