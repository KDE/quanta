/***************************************************************************
                          previewoptions.h  -  description
                             -------------------
    begin                : Fri Aug 4 2000
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

#ifndef PREVIEWOPTIONS_H
#define PREVIEWOPTIONS_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qwidget.h>

class KConfig;

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class PreviewOptions : public QWidget  {
   Q_OBJECT
public: 
	PreviewOptions(QWidget *parent=0, const char *name=0);
	~PreviewOptions();

	QString position();
	void setPosition( QString );
	
protected: 
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QButtonGroup *groupPreviewPlace;
	QRadioButton *radioBottom;
	QRadioButton *radioRight;
	QRadioButton *radioFullworkspace;
	QRadioButton *radioDisabled;
	QWidget *illustration;
	//Generated area. DO NOT EDIT!!!(end)

private: 
};

#endif
