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
  	attr[i] = value[i] = 0L;
}

Tag::Tag( const Tag &t)
{
	name = t.name;
	
	for (int i=0; i<20; i++) {
  		
  	if ( t.attr[i] ) {
  		attr[i] = new char[ strlen(t.attr[i])+1 ];
  		strcpy( attr[i], t.attr[i] );
  	}
  	else	attr[i] = 0L;
  			
  	if ( t.value[i] ) {
  		value[i] = new char[ strlen(t.value[i])+1 ];
  		strcpy( value[i], t.value[i] );
  	}
  	else	value[i] = 0L;
  }

  attrcount = t.attrcount;
}

Tag::~Tag()
{

  for (int i=0; i<20; i++) {
  	if (attr[i])
  		delete [] (char*)attr[i];
  	if (value[i])
  		delete [] (char*)value[i];
  }

}

void Tag::parseStr ( const char *tag )
{
  const char *t = tag; //tag.data();


  const char *tagname;
  const char *attrname;
  const char *attrvalue;

  QString attrName;
  QString attrVal;

  int len = 0;

  t = passSpaces( t );

  if ( *t ) tagname = t; else return;
  // find end of tag name
  while ( !isspace(*t) && *t ) { t++;len++;}

  name = QCString( tagname, len+1 ).lower();

  while ( *t )
  {
    t = passSpaces( t );

    len = 0;
    if ( *t ) attrname = t; else return;
    while ( !isspace(*t) && *t!='=' && *t ) { t++;len++;}

    attrName = QCString( attrname, len+1).lower();
    //attr[attrcount] = QString( attrname, len+1).lower();

    attr[attrcount] = new char[len+2];
    qstrcpy( attr[attrcount], attrName.data() );
    attrcount++;

    t = passSpaces( t );
    if ( !*t || *t!='=' ) continue;else t++;
    t = passSpaces( t );

    len = 0;
    if ( *t ) attrvalue = t; else return;
    if ( *t=='"' && *(t+1)) {
    	t++;
      attrvalue = t;
      while ( *t!='"' && *t ) { t++;len++;}
    }
    else
      while ( !isspace(*t) && *t!='=' && *t ) { t++;len++;}

    if ( *t=='"' ) t++;

    attrVal = QCString( attrvalue, len+1);

    value[attrcount-1] = new char[len+2];
    qstrcpy( value[attrcount-1], attrVal.data() );

    if ( attrcount == 20)
    	return;

//    attr.insert( attrName, attrValue );
  }

}

const char * Tag::passSpaces( const char *t )
{
  while ( isspace(*t) ) t++;
  return t;
}

Tag Tag::operator = (const Tag &t)
{
	name = t.name;
	
	for (int i=0; i<20; i++) {
  	if ( attr[i] )
  		delete [] (char*)attr[i];
  	attr[i] = 0L;
  		
  	if ( t.attr[i] ) {
  		attr[i] = new char[ strlen(t.attr[i])+1 ];
  		strcpy( attr[i], t.attr[i] );
  	}
  			
  	if (value[i])
  		delete [] (char*)value[i];
  	value[i] = 0L;
  		
  	if ( t.value[i] ) {
  		value[i] = new char[ strlen(t.value[i])+1 ];
  		strcpy( value[i], t.value[i] );
  	}
  }

  attrcount = t.attrcount;
  return *this;
	
}/**  */
bool Tag::haveAttrib( const char *attr )
{
	for (int i=0; i<attrcount;i++)
		if ( !strcasecmp(attr, this->attr[i]) )
			return true;
	return false;
}
/**  */
QString Tag::attrValue( const char *attr)
{
	for (int i=0; i<attrcount;i++)
		if ( !strcasecmp(attr, this->attr[i]) )
			return QString( value[i] );
	return QString();
}
