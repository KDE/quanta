/***************************************************************************
                          toolbardata.h  -  description
                             -------------------
    begin                : Sun Apr 9 2000
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

#ifndef TOOLBARDATA_H
#define TOOLBARDATA_H

#include <qpixmap.h>
#include <qstring.h>

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class ToolBarData {
public:
	ToolBarData( const char *name=0);
	~ToolBarData();

	enum types { Tag = 0 , Script };	
	enum inputtypes  { None = 0, SelectedText, CurrentDoc };
	enum outputtypes { InsInCurPos = 1, ReplaceCurDoc, CreateNewDoc, InMesWindow };
	
	QString name;
	QString iconFile;
	QPixmap pix;
	int pos;
	bool isSeparator;
	bool haveEndTag;
	int type; // Tag, Script
	int input;
	int output;
	QString tag;
	QString endTag;
	QString script;	
	
};

#endif
