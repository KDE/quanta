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

void Tag::parse(const QString &p_tagStr, Document *p_write)
{
 m_tagStr = p_tagStr;
 cleanStr = m_tagStr;
 m_write = p_write;
 m_nameLine = beginLine;
 m_nameCol = beginCol + 1;
 uint pos = 1;
 while (pos < m_tagStr.length() &&
        !m_tagStr[pos].isSpace() && m_tagStr[pos] != '>' && m_tagStr[pos] != '\n')
 {
   pos++;
 }
 name = m_tagStr.mid(1, pos - 1);
 QString attrStr;
 TagAttr attr;
 int sPos = ++pos;
 int valueStartPos = 0;
 while (pos < m_tagStr.length())
 {
    //find the attribute name
    while (pos < m_tagStr.length() &&
           !m_tagStr[pos].isSpace() && m_tagStr[pos] != '=')
    {
      pos++;
    }
    attr.name = m_tagStr.mid(sPos, pos - sPos);
    attr.nameLine = m_tagStr.left(sPos).contains('\n') + beginLine;
    if (attr.nameLine == beginLine)
        attr.nameCol = sPos;
    else
        attr.nameCol = m_tagStr.left(sPos).section('\n',-1).length();

    while (pos < m_tagStr.length() && m_tagStr[pos].isSpace())
            pos++;
    //if the attribute is just listed and there is no value specified,
    //treate it as a "true" boolean
    if (m_tagStr[pos] != '=' || pos == m_tagStr.length())
    {
      attr.value = (dtd != 0) ? dtd->booleanTrue : QString("true");
      attr.valueCol = attr.nameCol;
      attr.valueLine = attr.nameLine;
      attr.quoted = false;
    } else
    {
      pos++;
      while (pos < m_tagStr.length() && m_tagStr[pos].isSpace())
            pos++;
      if (m_tagStr[pos] == '\'' || m_tagStr[pos] == '"')
      {
        attr.quoted = true;
        valueStartPos = pos + 1;
        QChar quotation = m_tagStr[pos];
        pos += 1;
        while (pos < m_tagStr.length() &&
               (m_tagStr[pos] != quotation ||
               (m_tagStr[pos] == quotation && m_tagStr[pos-1] == '\\')))
        {
          pos++;
        }
        attr.value = m_tagStr.mid(valueStartPos, pos - valueStartPos);
      } else
      {
        attr.quoted = false;
        valueStartPos = pos;
        while (pos < m_tagStr.length() && !m_tagStr[pos].isSpace())
          pos++;
        attr.value = m_tagStr.mid(valueStartPos, pos - valueStartPos -1);
      }
      attr.valueLine = m_tagStr.left(valueStartPos).contains('\n') + beginLine;
      if (attr.valueLine == beginLine)
          attr.valueCol = valueStartPos;
      else
          attr.valueCol = m_tagStr.left(valueStartPos).section('\n',-1).length();
    }

    attrs.append(attr);
    //go to the first non-space char. This is where the next attribute name starts
    pos++;
    while (pos < m_tagStr.length() && m_tagStr[pos].isSpace())
          pos++;
    sPos = pos++;
 }
}


QString Tag::attribute(int index)
{
  QString attr="";
  if ( index != -1 && index < (int)attrs.count() )
  {
   attr = attrs[index].name;
  }
  return attr;
}

QString Tag::attributeValue(int index)
{
  QString val = "";
  if ( index != -1 && index < (int)attrs.count() )
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

  if ( attr == attrs[i].name ||
      (!dtd->caseSensitive && attrs[i].name == attr.lower()))
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
    if ( attrs[i].name ==  attr ||
         (!dtd->caseSensitive && attrs[i].name == attr.lower()))
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
    if (attrs[i].name == attr ||
       (!dtd->caseSensitive && attrs[i].name == attr.lower()))
       index = i;
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
   if  (attrs[i].valueLine == line &&
       (attrs[i].valueLine != attrs[i].nameLine || attrs[i].valueCol != attrs[i].nameCol))
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
 if ( index != -1 && index < (int)attrs.count() )
 {
  line = attrs[index].nameLine;
  col = attrs[index].nameCol;
 }
}

void Tag::attributeValuePos(int index, int &line, int &col)
{
 line = -1;
 col = -1;
 if ( index != -1 && index < (int)attrs.count() )
 {
  line = attrs[index].valueLine;
  col = attrs[index].valueCol;
 }
}
