/***************************************************************************
                          tag.cpp  -  description
                             -------------------
    begin                : Sun Sep 1 2002
    copyright            : (C) 2002, 2003 by Andras Mantia
    email                : amantia@freemail.hu
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
  dtd = 0L;
  type = Unknown;
  single = false;
  closingMissing = false;
  attrCount = 0;
  structBeginStr = "";
  cleanStr = "";
}

Tag::Tag( const Tag &t)
{
  name = t.name;
  dtd = t.dtd;
  single = t.single;
  closingMissing = t.closingMissing;
  beginLine = t.beginLine;
  beginCol = t.beginCol;
  endLine = t.endLine;
  endCol = t.endCol;
  m_tagStr = t.m_tagStr;
  cleanStr = t.cleanStr;
  m_write = t.m_write;
  type = t.type;
  structBeginStr = t.structBeginStr;

	for (int i=0; i<t.attrCount; i++)
  {
    attrs[i] = t.attrs[i];
  }

  attrCount = t.attrCount;
}

Tag::~Tag()
{
}

/** Parse the attributes in the string and build the attrs vector */
void Tag::parseAttr( QString text, int &line, int &col)
{
  QString tagname;
  int currentAttrNum = attrCount;

  while ( text[col].isSpace() && !text[col].isNull()) col++;

  if ( text[col]=='>' || col >= (int)text.length() ) return;

  int begin = col;

  //go through the string
  while (col < (int) text.length() && text[col] != '>' && currentAttrNum < MAX_ATTR_NUM)
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
 cleanStr = m_tagStr;
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

/** Check if this tag has the attr attribute defined */
bool Tag::hasAttribute( const QString &attr )
{
	for (int i=0; i < attrCount; i++)
		if ( attrs[i].name.lower() ==  attr.lower()  )
			return true;
	return false;
}

/** Set the coordinates of tag inside the document */
void Tag::setTagPosition(int bLine, int bCol, int eLine, int eCol)
{
  beginLine = bLine;
  beginCol = bCol;
  endLine = eLine;
  endCol = eCol;
}

/** Return the index of attr. */
int Tag::attributeIndex(QString attr)
{
  int index = -1;
  int i = 0;
  do{
    if (attrs[i].name == attr) index = i;
    i++;
  } while (index == -1 && i < attrCount);
  return index;
}
/** Returns the index of attribute at (line,col). */
int Tag::attributeIndexAtPos(int line, int col)
{
 int index = -1;
 int i = 0;
 do
 {
   if (attrs[i].nameLine == line)
   {
     if (attrs[i].nameCol <= col &&
         (int) (attrs[i].nameCol + attrs[i].name.length()) >=col)
     {
        index = i;
     }
   }
   i++;
 } while (i < attrCount && index == -1);
 return index;
}

/** Returns the index of attributevalue at (line,col). */
int Tag::valueIndexAtPos(int line, int col)
{
 int index = -1;
 int i = 0;
 do
 {
   if (attrs[i].valueLine == line)
   {
     if (attrs[i].valueCol <= col &&
         (int) (attrs[i].valueCol + attrs[i].value.length()) >=col)
     {
        index = i;
     }
   }
   i++;
 } while (i < attrCount && index == -1);
 return index;
}
