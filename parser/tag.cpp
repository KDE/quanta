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

#include "../document.h"

Tag::Tag()
{
  name = "";
  type = "unknown";
  single = false;
  attrCount = 0;
}

Tag::Tag( const Tag &t)
{
	name = t.name;
	single = t.single;
  beginLine = t.beginLine;
  beginCol = t.beginCol;
  endLine = t.endLine;
  endCol = t.endCol;
  m_tagStr = t.m_tagStr;
  m_write = t.m_write;
  type = t.type;

	for (int i=0; i<t.attrCount; i++)
  {
    attrs[i] = t.attrs[i];
  }

  attrCount = t.attrCount;
}

Tag::~Tag()
{
}

void Tag::parseAttr( QString text, int &line, int &col)
{
  QString tagname;
  int currentAttrNum = attrCount;

  while ( text[col].isSpace() && !text[col].isNull()) col++;

  if ( text[col]=='>' || col >= (int)text.length() ) return;

  int begin = col;

  //go through the string
  while (col < (int) text.length() && text[col] != '>')
  {
   //find where the attr name begins
   while ( text[col].isSpace() && !text[col].isNull()) col++;
   begin = col++;
   //go to the first non-space char
   while ( !text[col].isSpace() &&  text[col] != '=' && text[col] != '>'
            && !text[col].isNull())
   {
    QString s = text.mid(col,1);
    col++;
   }
   if (text[col].isNull()) break;
   if (text[col] == '=') //an attribute value comes
   {
     attrs[currentAttrNum].name = text.mid(begin, col-begin).stripWhiteSpace();
     attrs[currentAttrNum].nameLine = line;
     attrs[currentAttrNum].nameCol = begin;
     col++;
     while ( text[col].isSpace() ) col++;
     if (text[col] == '\"') //the attribute value is quoted
     {
       begin = ++col;
//TODO: recognize the different quoting styles as it is specified in options
       while (  text[col] != '\"' &&
                col < (int) text.length())
       {
         col++;
       }
       attrs[currentAttrNum].value = text.mid(begin, col-begin);
       attrs[currentAttrNum].quoted = true;
       col++;
     }
     else
     {
       begin = col;
       while ( !text[col].isSpace() &&
                text[col] != '>' &&
                col < (int) text.length())
       {
        col++;
       }
       attrs[currentAttrNum].value = text.mid(begin, col-begin);
       attrs[currentAttrNum].quoted = false;
     } //else
     attrs[currentAttrNum].valueLine = line;
     attrs[currentAttrNum].valueCol = begin;
   }
   else           // no attribute value, the next attr comes
   {
     //FIXME: This values are not correct for every DTD
     attrs[currentAttrNum].name = text.mid(begin, col-begin).stripWhiteSpace();
     attrs[currentAttrNum].nameLine = line;
     attrs[currentAttrNum].nameCol = begin;
     attrs[currentAttrNum].value = "true";
     attrs[currentAttrNum].quoted = false;
     attrs[currentAttrNum].valueLine = line;
     attrs[currentAttrNum].valueCol = begin;
   }
   currentAttrNum++;
  }
 attrCount = currentAttrNum;
}

void Tag::parse(const QString &p_tagStr, Document *p_write)
{
 m_tagStr = p_tagStr;
 m_write = p_write;

 attrCount = 0;
 int line = beginLine;
 int col = beginCol;
 int begin;

 QString textLine = m_write->editIf->textLine(line);

 while (  textLine[col] != '<' && !textLine[col].isNull())
   col++;

 begin = ++col;
 while ( !textLine[col].isSpace() &&
          textLine[col] != '>' &&
         !textLine[col].isNull())
 {
   col++;
 }
 name = textLine.mid(begin, col-begin);

 while ( textLine[col] != '>' && attrCount < 50)
  {
    if ( textLine.isNull() )
      break;

    parseAttr( textLine, line, col);

    if ( col >= (int)textLine.length() && textLine[col] != '>')
    {
      textLine = m_write->editIf->textLine(++line);
      col = 0;
    }
  }
}

QString Tag::attribute(int index)
{
  QString attr="";
  if ( attrCount )
  {
   attr = attrs[index].name;
  }
  return attr;
}

QString Tag::attributeValue(int index)
{
  QString val = "";
  if ( attrCount )
  {
    val = attrs[index].value;
  }
  return val;
}

QString Tag::attributeValue(QString attr)
{
 QString val = "";
 for (int i=0 ; i < attrCount; i++)
 {

  if ( attr.upper() == attrs[i].name.upper())
  {
    val = attrs[i].value;
    break;
  }
 }
 return val;
}
/*
void Tag::parseStr ( const QString &tag )
{
  QString t = tag;

  attrcount = 0;

  int i = 0;

  t = t.stripWhiteSpace();
  if ( t.right(1) == "/" )  {
    single = true;
    t.truncate( t.length() - 1 );
  }

  while ( !t[i].isSpace() && !t[i].isNull() )	i++;

  name = t.left(i);

  t = t.remove(0,i).stripWhiteSpace();

  while ( !t.isEmpty() ) {
  
   	attrcount++;
   	
   	if ( attrcount >= 20 )
   	  break;
   	  
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
	
}           */

bool Tag::hasAttribute( const QString &attr )
{
	for (int i=0; i < attrCount; i++)
		if ( attrs[i].name.lower() ==  attr.lower()  )
			return true;
	return false;
}

/** No descriptions */
void Tag::setTagPosition(int bLine, int bCol, int eLine, int eCol)
{
  beginLine = bLine;
  beginCol = bCol;
  endLine = eLine;
  endCol = eCol;
}
