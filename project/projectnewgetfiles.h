/***************************************************************************
                          projectnewgetfiles.h  -  description
                             -------------------
    begin                : Tue Aug 22 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PROJECTNEWGETFILES_H
#define PROJECTNEWGETFILES_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <klineedit.h>
#include <qpushbutton.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qwidget.h>

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class ProjectNewGetFiles : public QWidget  {
   Q_OBJECT
public: 
	ProjectNewGetFiles(QWidget *parent=0, const char *name=0);
	~ProjectNewGetFiles();

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QButtonGroup *QButtonGroup_1;
	QCheckBox *checkInsertFiles;
	QCheckBox *checkWebFiles;
	QLabel *QLabel_6;
	KLineEdit *lineFilesMask;
	QLabel *QLabel_2;
	QButtonGroup *QButtonGroup_2;
	QLabel *QLabel_3;
	KLineEdit *lineSiteUrl;
	QLabel *QLabel_4;
	KLineEdit *lineWgetOption;
	QLabel *QLabel_5;
	QPushButton *buttonEditRcFile;
	//Generated area. DO NOT EDIT!!!(end)

public slots:
	void slotEditRcFile();
	
private: 
};

#endif
