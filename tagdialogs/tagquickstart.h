/***************************************************************************
                          tagquickstart.h  -  description
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

#ifndef TAGQUICKSTART_H
#define TAGQUICKSTART_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <kcolorbtn.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qdialog.h>

/**
  *@author Dmitry Poplavsky & Yacovlev Alexander
  */

class TagQuickStart : public QDialog  {
   Q_OBJECT
public: 
	TagQuickStart( QString basePath, QWidget *parent=0, const char *name=0);
	~TagQuickStart();

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *labelTitle;
	QLabel *labelBGImage;
	QLabel *labelBGColor;
	QLabel *labelTextColor;
	QLabel *labelActiveLinkColor;
	QLineEdit *lineTitle;
	QLineEdit *lineBGImage;
	QPushButton *buttonFileSelect;
	QComboBox *comboBGColor;
	KColorButton *cButtonBGColor;
	QComboBox *comboTextColor;
	KColorButton *cButtonTextColor;
	QComboBox *comboLinkColor;
	KColorButton *cButtonLinkColor;
	QComboBox *comboALinkColor;
	KColorButton *cButtonALinkColor;
	QLabel *QLabel_8;
	QComboBox *comboVLinkColor;
	KColorButton *cButtonVLinkColor;
	QLabel *QLabel_7;
	QPushButton *buttonOk;
	QPushButton *buttonCancel;
	//Generated area. DO NOT EDIT!!!(end)

private:
	QString basePath;
	
public slots: // Public slots
  /** select BG Image */
  void slotFileSelect();
  /** insert BG color in combo */
  void slotBGColor(const QColor &newColor);
  /** insert text color in combo */
  void slotTextColor(const QColor &newColor);
  /** insert link color in combo */
  void slotLinkColor(const QColor &newColor);
  /** insert active link color in combo */
  void slotALinkColor(const QColor &newColor);
  /** insert visited link color in combo */
  void slotVLinkColor(const QColor &newColor);
};

#endif
