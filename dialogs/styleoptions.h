/***************************************************************************
                          styleoptions.h  -  description
                             -------------------
    begin                : Fri Apr 7 2000
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

#ifndef STYLEOPTIONS_H
#define STYLEOPTIONS_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qcheckbox.h>
#include <kseparator.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qwidget.h>

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class StyleOptions : public QWidget  {
   Q_OBJECT
friend class QuantaApp;
public: 
	StyleOptions(QWidget *parent=0, const char *name=0);
	~StyleOptions();

protected: 
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QCheckBox *checkTagsCapital;
	QCheckBox *checkAttrCapital;
	KSeparator *KSeparator_1;
	KSeparator *KSeparator_2;
	QCheckBox *checkEndTag;
	//Generated area. DO NOT EDIT!!!(end)

private: 
};

#endif
