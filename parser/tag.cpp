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

#include <ctype.h>

#include <qdict.h>
#include <qstring.h>
#include <qcstring.h>

#include "tag.h"
#include "../document.h"

Tag::Tag()
{
  name = "";
  dtd = 0L;
  type = Unknown;
  single = false;
  closingMissing = false;
  structBeginStr = "";
  cleanStr = "";
  m_nameLine = -1;
  m_nameCol = -1;
  validXMLTag = true;
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
  m_nameLine = t.m_nameLine;
  m_nameCol = t.m_nameCol;
  attrs = t.attrs;
}

Tag::~Tag()
{
}

/** Parse the attributes in the string and build the attrs vector */
void Tag::parseAttr( QString text, int &line, int &col)
{
  QString tagname;

  while ( text[col].isSpace() && !text[col].isNull()) col++;

  if ( text[col]=='>' || col >= (int)text.length() ) return;

  int begin = col;
  TagAttr attr;

  //go through the string
  while (col < (int) text.length() && text[col] != '>')
  {
   //find where the attr name begins
   while ( text[col].isSpace() && !text[col].isNull())
         col++;
   begin = col;
   //go to the first non-space char
   while ( !text[col].isSpace() &&  text[col] != '=' && text[col] != '>'
            && !text[col].isNull())
     col++;
   if (text[col].isNull()) break;
   if (text[col] == '=') //an attribute value comes
   {
     attr.name = text.mid(begin, col-begin).stripWhiteSpace();
     attr.nameLine = line;
     attr.nameCol = begin;
     col++;
     while ( text[col].isSpace() )
        col++;
     if (text[col] == '"' || text[col] == '\'') //the attribute value is quoted
     {
       QChar quotation = text[col];
       begin = ++col;
       while (  (text[col] != quotation ||
                (text[col] == quotation && text[col-1] == '\\') )
              && col < (int) text.length())
       {
         col++;
       }
       attr.value = text.mid(begin, col-begin);
       attr.quoted = true;
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
       attr.value = text.mid(begin, col-begin);
       attr.quoted = false;
     } //else
     attr.valueLine = line;
     attr.valueCol = begin;
   }
   else           // no attribute value, the next attr comes
   {
     //FIXME: This values are not correct for every DTD
     if(begin == col) return;//if begin == col, the "attribute" was >
     attr.name = text.mid(begin, col-begin).stripWhiteSpace();
     attr.nameLine = line;
     attr.nameCol = begin;
     attr.value = "true";
     attr.quoted = false;
     attr.valueLine = line;
     attr.valueCol = begin;
   }
   attrs.append(attr);
  }
}

void Tag::parse(const QString &p_tagStr, Document *p_write)
{
 m_tagStr = p_tagStr;
 cleanStr = m_tagStr;
 m_write = p_write;

 m_nameLine = beginLine;
 m_nameCol = beginCol;
 int line = beginLine;
 int col = beginCol;
 int begin;

 QString textLine = m_write->editIf->textLine(line);

 if ( textLine[col] == '<' &&
      (( beginLine == endLine && col < endCol) ||
       ( beginLine != endLine)) )
     col++;

 while ( (textLine[col].isSpace() || textLine[col] == '>') &&
         !textLine[col].isNull() &&
         ( ( beginLine == endLine && col < endCol) ||
           ( beginLine != endLine) )
       )
 {
   col++;
 }
 begin = col;
 if ( ( beginLine == endLine && col < endCol) ||
      ( beginLine != endLine) )
     col++;
 while ( !textLine[col].isSpace() &&
          textLine[col] != '>' &&
         !textLine[col].isNull() &&
         ( ( beginLine == endLine && col < endCol) ||
           ( beginLine != endLine) )
       )
 {
   col++;
 }
 if (beginLine == endLine && col == endCol && textLine[col] != '>')
    col++;
 name = textLine.mid(begin, col - begin).stripWhiteSpace();
 if (!name.isEmpty())
    m_nameCol = begin;

 while ( textLine[col] != '>')
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

QString Tag::attribute(uint index)
{
  QString attr="";
  if ( index != -1 && index < attrs.count() )
  {
   attr = attrs[index].name;
  }
  return attr;
}

QString Tag::attributeValue(uint index)
{
  QString val = "";
  if ( index != -1 && index < attrs.count() )
  {
    val = attrs[index].value;
  }
  return val;
}

QString Tag::attributeValue(QString attr)
{
 QString val = "";
 for (uint i = 0 ; i < attrs.count(); i++)
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
  for (uint i = 0; i < attrs.count(); i++)
  {
    if ( attrs[i].name.lower() ==  attr.lower()  )
      return true;
  }
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
  uint i = 0;
  do{
    if (attrs[i].name == attr) index = i;
    i++;
  } while (index == -1 && i < attrs.count());
  return index;
}
/** Returns the index of attribute at (line,col). */
int Tag::attributeIndexAtPos(int line, int col)
{
 int index = -1;
 uint i = 0;
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
 } while (i < attrs.count() && index == -1);
 return index;
}

/** Returns the index of attributevalue at (line,col). */
int Tag::valueIndexAtPos(int line, int col)
{
 int index = -1;
 uint i = 0;
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
 } while (i < attrs.count() && index == -1);
 return index;
}

void Tag::namePos(int &line, int &col)
{
  line = m_nameLine;
  col = m_nameCol;
}

void Tag::setStr(const QString &p_tagStr)
{
  m_tagStr = p_tagStr; cleanStr = m_tagStr;
}

int Tag::size()
{
  int l = sizeof(name) +  8*sizeof(int) + 2*sizeof(bool);
  l += sizeof(cleanStr) + sizeof(m_tagStr);
  l += sizeof(structBeginStr) + sizeof(attrs);
  l += sizeof(dtd) + sizeof(m_write);

  return l;
}

void Tag::attributeNamePos(int index, int &line, int &col)
{
 line = -1;
 col = -1;
 if ( index != -1 && index < attrs.count() )
 {
  line = attrs[index].nameLine;
  col = attrs[index].nameCol;
 }
}

void Tag::attributeValuePos(int index, int &line, int &col)
{
 line = -1;
 col = -1;
 if ( index != -1 && index < attrs.count() )
 {
  line = attrs[index].valueLine;
  col = attrs[index].valueCol;
 }
}
