/***************************************************************************
                          tagbody.h  -  description
                             -------------------
    begin                : Mon Apr 3 2000
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

#ifndef TAGBODY_H
#define TAGBODY_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <kcolorbtn.h>
#include <kseparator.h>
//Generated area. DO NOT EDIT!!!(end)

#include <tagwidget.h>

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class TagBody : public TagWidget  {
   Q_OBJECT
public: 
	TagBody(QWidget *parent=0, const char *name=0);
	~TagBody();
	
  void readAttributes( QDict<char> *d);
  void writeAttributes( QDict<char> *d );	

protected: 
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *labelBGImage;
	QLabel *labelBGColor;
	QLabel *labelTextColor;
	QLabel *labelActiveLinkColor;
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
	KSeparator *KSeparator_1;
	QLabel *QLabel_9;
	QLineEdit *lineOnLoad;
	QLabel *QLabel_10;
	QLineEdit *lineOnUnload;
	//Generated area. DO NOT EDIT!!!(end)

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

private:

};

#endif
