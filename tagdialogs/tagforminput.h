/***************************************************************************
                          tagforminput.h  -  description
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

#ifndef TAGFORMINPUT_H
#define TAGFORMINPUT_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
//Generated area. DO NOT EDIT!!!(end)

#include <tagwidget.h>

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class TagFormInput : public TagWidget  {
   Q_OBJECT
public: 
	TagFormInput(QWidget *parent=0, const char *name=0);
	~TagFormInput();

  void readAttributes( QDict<char> *d);
  void writeAttributes( QDict<char> *d );

public slots:
  void slotSrcSelect();	
	
  /**  */
  void slotTypeChange(int i);
protected: 
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QComboBox *comboType;
	QLabel *QLabel_2;
	QLineEdit *lineName;
	QLabel *QLabel_3;
	QLineEdit *lineValue;
	QLabel *QLabel_4;
	QLineEdit *lineSize;
	QLabel *QLabel_5;
	QLineEdit *lineMaxLen;
	QLabel *QLabel_6;
	QLineEdit *lineSrc;
	QPushButton *buttonSrc;
	QLabel *QLabel_7;
	QLineEdit *lineAlt;
	QLabel *QLabel_8;
	QComboBox *comboAlign;
	QCheckBox *checkCheck;
	QCheckBox *checkReadOnly;
	QCheckBox *checkDisabled;
	QLabel *QLabel_9;
	QLineEdit *lineOnFocus;
	QLabel *QLabel_10;
	QLineEdit *lineOnBlur;
	QLabel *QLabel_11;
	QLineEdit *lineOnSelect;
	QLabel *QLabel_12;
	QLineEdit *lineOnChange;
	//Generated area. DO NOT EDIT!!!(end)

private:

};

#endif
