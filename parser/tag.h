/***************************************************************************
                          tag.h  -  description
                             -------------------
    begin                : Sun Apr 16 2000
    copyright            : (C) 2000 by Dmitry Poplavsky
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

#ifndef TAG_H
#define TAG_H

#include <qstring.h>
#include <qdict.h>

/**
  *@author Dmitry Poplavsky
  */

class Tag {
public: 
	Tag();
	Tag( const Tag&);
	~Tag();
	
	void parseStr ( const char *tag );
	
	Tag operator = ( const Tag& );
  /**  */
  QString attrValue( const char *attr);
  /**  */
  bool haveAttrib( const char *attr );
	
	QString name;
	
	char* attr[20];  //atributes
	char* value[20]; // values of attributes.
	int attrcount;

//	QDict<char> attr;

private:	
	const char *passSpaces( const char * );
};

#endif
