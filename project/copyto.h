/***************************************************************************
                          copytodlg.h  -  description
                             -------------------
    begin                : Mon Mar 27 2000
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

#ifndef COPYTO_H
#define COPYTO_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlineedit.h>
#include <qpushbutton.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qdialog.h>

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class CopyTo : public QDialog  {
   Q_OBJECT
public: 
	CopyTo(QString dir, QWidget *parent=0, const char *name=0);
	~CopyTo();

public slots: // Public slots
  /** change dir */
  void slotDirChange();	
	
public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLineEdit *lineDir;
	QPushButton *buttonDir;
	QPushButton *buttonOk;
	QPushButton *buttonCancel;
	//Generated area. DO NOT EDIT!!!(end)
	
private: 
};

#endif
