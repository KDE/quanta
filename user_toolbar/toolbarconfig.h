/***************************************************************************
                          toolbarconfig.h  -  description
                             -------------------
    begin                : Sat Apr 8 2000
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

#ifndef TOOLBARCONFIG_H
#define TOOLBARCONFIG_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <kseparator.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qdialog.h>
#include <kbutton.h>

#include "toolbaritem.h"

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class KConfig;

class ToolBarConfig : public QDialog  {
   Q_OBJECT
public: 
	ToolBarConfig(KConfig *config, QWidget *parent=0, const char *name=0);
	~ToolBarConfig();
  /** read configuration */
  void readConfig();
  /** save toolbar configuration */
  void saveConfig();

protected: 
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QListView *list;
	KButton *buttonNew;
	KButton *buttonUp;
	KButton *buttonDown;
	KButton *buttonDel;
	QLineEdit *lineName;
	QCheckBox *checkSeparator;
	QLabel *QLabel_1;
	QLabel *QLabel_2;
	QLineEdit *lineIcon;
	QPushButton *buttonIcon;
	QPushButton *buttonKIconEdit;
	KSeparator *KSeparator_2;
	QButtonGroup *groupBox;
	QRadioButton *radioInsertTag;
	QRadioButton *radioExecuteScript;
	QLabel *QLabel_3;
	QLineEdit *lineTag;
	QCheckBox *checkEndTag;
	QLineEdit *lineEndTag;
	QLabel *QLabel_4;
	QLineEdit *lineScript;
	QPushButton *buttonConfigScript;
	QPushButton *buttonOk;
	QPushButton *buttonCancel;
	//Generated area. DO NOT EDIT!!!(end)

public:
  KConfig *config;
private:
  int maxpos;
public slots: // Public slots
private slots: // Private slots
  /**  */
  void slotHaveEndTag();
  /** togle separator | button */
  void slotSeparator(bool);
  /**  */
  void slotRadioToggled();

public slots: // Public slots
  /** select icon file name */
  void slotOpenIcon();
  /** open icon in kiconedit */
  void slotKIconEdit();
  /**  */
  void listNew();
  void listUp();
  void listDown();
  void listDel();
  /**  */
  void listCurrentChanged( QListViewItem * );
  /**  */
  void slotConfigScript();
protected slots: // Protected slots
  /**  */
  void slotLineChanged( const char *);
};

#endif

