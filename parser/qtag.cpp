/***************************************************************************
                          qtag.cpp  -  description
                             -------------------
    begin                : Thu Aug 15 2002
    copyright            : (C) 2002 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include "qtag.h"

QTag::QTag()
{
 attrs.setAutoDelete(true);
 single = false;
 optional = false;
 tagName = "";
 m_fileName = "";
 type = "xmltag";
 returnType = "";
 parentDTD = 0L;
}

QTag::QTag( QTag &t)
{
  tagName = t.tagName;
  single = t.single;
  optional = t.optional;
  m_fileName = t.m_fileName;
  parentDTD = t.parentDTD;
  type = t.type;
  returnType = t.returnType;
  commonGroups = t.commonGroups;
  stoppingTags = t.stoppingTags;
  childTags = t.childTags;

  for (int i=0; i < t.attributeCount(); i++)
  {
    addAttribute(t.attributeAt(i));
  }
}

QTag::~QTag()
{
}

/** Add an attribute to the tag. */
void QTag::addAttribute(Attribute* attr)
{
 if (attr)
 {
  Attribute* a = attribute(attr->name);
  bool createNew = !a;
  if (createNew) a = new Attribute;
  a->name = attr->name;
  a->type = attr->type;
  for ( QStringList::Iterator it = attr->values.begin(); it != attr->values.end(); ++it )
  {
     a->values.append(*it);
  }
  a->defaultValue = attr->defaultValue;
  a->status = attr->status;
  if (createNew) attrs.append(a);
 }
}
/** Returns the number of attributes for the tag. */
int QTag::attributeCount()
{
  return attrs.count();
}
/** Returns the attribute at index. */
Attribute* QTag::attributeAt(int index)
{
  return attrs.at(index);
}
/** No descriptions */
void QTag::setSingle(bool isSingle)
{
 single = isSingle;
}
/** No descriptions */
void QTag::setOptional(bool isOptional)
{
 optional = isOptional;
}
/** No descriptions */
void QTag::setName(const QString& theName)
{
  tagName = theName;
}
/** No descriptions */
QString QTag::name()
{
  if (!parentDTD || parentDTD->caseSensitive)
      return tagName;
  else
      return tagName.upper();
}
/** No descriptions */
bool QTag::isSingle()
{
  return single;
}
/** No descriptions */
bool QTag::isOptional()
{
  return optional;
}
/** No descriptions */
void QTag::setFileName(const QString& fileName)
{
  m_fileName = fileName;
}

/** No descriptions */
QString QTag::fileName()
{
 return m_fileName;
}

QTag QTag::operator = (QTag &t)
{
  tagName = t.tagName;
  single = t.single;
  optional = t.optional;
  m_fileName = t.m_fileName;
  parentDTD = t.parentDTD;
  type = t.type;
  returnType = t.returnType;
  commonGroups = t.commonGroups;
  stoppingTags = t.stoppingTags;

  for (int i=0; i < t.attributeCount(); i++)
  {
    addAttribute(t.attributeAt(i));
  }

  return *this;
}

/** Returns the attribute with name, or 0 if the tag does not have any attribute with name. */
Attribute* QTag::attribute(const QString& name)
{
 Attribute *attr = 0L;
 for (uint i = 0; i < attrs.count(); i++)
 {
   if (attrs.at(i)->name == name)
   {
     attr = attrs.at(i);
     break;
   }
 }

 return attr;
}

bool QTag::isChild(const QString& tag)
{
  QString tagName = parentDTD->caseSensitive ? tag : tag.upper();
  return (childTags.isEmpty() || childTags.contains(tagName));
}
