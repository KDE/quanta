/***************************************************************************
                          projectnewgeneral.h  -  description
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

#ifndef PROJECTNEWGENERAL_H
#define PROJECTNEWGENERAL_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qwidget.h>

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class ProjectNewGeneral : public QWidget  {
   Q_OBJECT
public: 
	ProjectNewGeneral(QWidget *parent=0, const char *name=0);
	~ProjectNewGeneral();

public:
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QLabel *QLabel_1;
	QLineEdit *linePrjName;
	QLabel *QLabel_2;
	QLineEdit *linePrjDir;
	QPushButton *buttonDir;
	QLabel *QLabel_3;
	QLineEdit *linePrjFile;
	QLabel *QLabel_4;
	QLineEdit *lineBaseURL;
	QLabel *QLabel_5;
	QLineEdit *lineAuthor;
	QLabel *QLabel_6;
	QLineEdit *lineEmail;
	QLabel *QLabel_7;
	QLabel *QLabel_8;
	QLabel *QLabel_9;
	QLineEdit *lineImagesDir;
	QPushButton *buttonImagesDir;
	QLineEdit *lineCgiDir;
	QPushButton *buttonCgiDir;
	QLineEdit *lineScriptsDir;
	QPushButton *buttonScriptsDir;
	//Generated area. DO NOT EDIT!!!(end)

public slots: // Public slots
  /** chenge file name of project */
  void slotNameChanged(const QString &text);
  /** change dir name */
  void slotDirChange();
  void slotImagesDirChange();
  void slotCgiDirChange();
  void slotScriptsDirChange();
	
private: 
};

#endif
