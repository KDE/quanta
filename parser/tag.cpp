/***************************************************************************
                          tag.cpp  -  description
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

#include "tag.h"
#include <qdict.h>
#include <qstring.h>
#include <qcstring.h>
#include <ctype.h>

Tag::Tag()
{
  name = "";
//  attr.setAutoDelete(true);
  attrcount = 0;
  for (int i=0; i<20;i++)
  	attr[i] = value[i] = "";
}

Tag::Tag( const Tag &t)
{
	name = t.name;
	
	for (int i=0; i<t.attrcount; i++) {
	  attr[i] = t.attr[i];
	  value[i] = t.value[i];
  }

  attrcount = t.attrcount;
}

Tag::~Tag()
{
}

void Tag::parseStr ( const QString tag )
{
  QString t = tag;

  attrcount = 0;

  int i = 0;

  t = t.stripWhiteSpace();

  while ( !t[i].isSpace() && !t[i].isNull() )	i++;

  name = t.left(i);

  t = t.remove(0,i).stripWhiteSpace();

  while ( !t.isEmpty() ) {
  	attrcount++;
  	i=0;
  	while ( !t[i].isSpace() && !t[i].isNull() && t[i] != '=' )	i++;
  	
  	attr[attrcount] = t.left(i);
  	t = t.remove(0,i).stripWhiteSpace();
  	
  	if ( t[0] == '=' ) {
  		t = t.remove(0,1).stripWhiteSpace();
  		
  		if ( t[0] == '"' ) {
  			i = 1;
  			while ( t[i] != '"' && !t[i].isNull() ) i++;
  			if ( t[i] == '"' )
  				value[attrcount] = t.mid(1,i-1);
  			else
  			  value[attrcount] = t.mid(1,i);
  			t = t.remove(0,i).stripWhiteSpace();
  		}
  		else
  		if ( t[0] == '\'' ) {
  			i = 1;
  			while ( t[i] != '\'' && !t[i].isNull() ) i++;
  			if ( t[i] == '\'' )
  				value[attrcount] = t.mid(1,i-1);
  			else
  			  value[attrcount] = t.mid(1,i);
  			t = t.remove(0,i).stripWhiteSpace();
  		}
  		else {
  		
    		i=0;
  	  	while ( !t[i].isSpace() && !t[i].isNull() )	i++;
    	
    		value[attrcount] = t.left(i);
    		t = t.remove(0,i).stripWhiteSpace();
  		}
  		
  		// debug ( name+" , "+attr[attrcount]+"="+value[attrcount]+";" );
  	
  	}
  	else {
  	  value[attrcount]="";
  	}
  	
  }

}


Tag Tag::operator = (const Tag &t)
{
	name = t.name;
	
	for (int i=0; i<t.attrcount; i++) {
	  attr[i]  = t.attr[i];
	  value[i] = t.value[i];
 	}

  attrcount = t.attrcount;
  return *this;
	
}

bool Tag::haveAttrib( const QString attr )
{
	for (int i=0; i<attrcount;i++)
		if ( attr.lower() ==  this->attr[i].lower()  )
			return true;
	return false;
}
/**  */
QString Tag::attrValue( const QString attr)
{
	for (int i=0; i<attrcount;i++)
		if ( attr.lower() ==  this->attr[i].lower() )
			return value[i];
	return QString();
}
