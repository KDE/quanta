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
#include "node.h"
#include "tag.h"
#include <kdebug.h>

QTag::QTag()
{
 attrs.setAutoDelete(true);
 single = false;
 optional = false;
 type = "xmltag";
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
  comment = t.comment;
  commonGroups = t.commonGroups;
  stoppingTags = t.stoppingTags;
  childTags = t.childTags;
  className = t.className;

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
  if (createNew)
    a = new Attribute;
  a->name = attr->name;
  a->type = attr->type;
  QStringList::ConstIterator end = attr->values.constEnd();
  for ( QStringList::ConstIterator it = attr->values.constBegin(); it != end; ++it )
  {
     a->values.append(*it);
  }
  a->defaultValue = attr->defaultValue;
  a->status = attr->status;
  a->source = attr->source;
  a->method = attr->method;
  a->interface = attr->interface;
  a->arguments = attr->arguments;
  if (createNew)
    attrs.append(a);
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

/** Returns true if the attribute exists */
bool QTag::isAttribute(const QString &attrName)
{
  Attribute *a;
  int i;
  AttributeList *groupAttrs;

  //Check in the QTag specific attributes
  for(a = attrs.first(); a; a = attrs.next())
  {
    if(a->name == attrName)
      return true;
  }
  //Check in the common attributes
  for(i = 0; i < (signed)commonGroups.count(); i++)
  {
    groupAttrs = parentDTD->commonAttrs->find(commonGroups[i]);
    if(groupAttrs)
    {
      for(a = groupAttrs->first(); a; a = groupAttrs->next())
      {
        if(a->name == attrName)
         return true;
      }
    }
  }
  return false;
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
QString QTag::name(bool doNotConvert)
{
  if (doNotConvert || !parentDTD || parentDTD->caseSensitive)
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
  comment = t.comment;
  commonGroups = t.commonGroups;
  stoppingTags = t.stoppingTags;
  className = t.className;

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

bool QTag::isChild(const QString& tag, bool trueIfNoChildsDefined)
{
 QString tagName = tag;
  tagName = parentDTD->caseSensitive ? tagName : tagName.upper();
  if(trueIfNoChildsDefined)
    return (childTags.isEmpty() || childTags.contains(tagName));
  else
    return (!childTags.isEmpty() && childTags.contains(tagName));
}

bool QTag::isChild(Node *node, bool trueIfNoChildsDefined, bool treatEmptyNodesAsText)
{
  QString nodeName;

  if(!node)
    return false;
  else if(node->tag->type == Tag::Text)
  {
    if(trueIfNoChildsDefined)
      return(childTags.isEmpty() || childTags.contains("#text") || childTags.contains("#TEXT"));
    else
      return(!childTags.isEmpty() && (childTags.contains("#text") || childTags.contains("#TEXT")));
  }
  else if(node->tag->type == Tag::Empty && !treatEmptyNodesAsText)
      return true;
  else if(node->tag->type == Tag::Empty && treatEmptyNodesAsText)
  {
      if(trueIfNoChildsDefined)
          return(childTags.isEmpty() || childTags.contains("#text") || childTags.contains("#TEXT"));
      else
          return(!childTags.isEmpty() && (childTags.contains("#text") || childTags.contains("#TEXT")));      
  }
  else if(node->tag->type == Tag::XmlTagEnd)
  {
    nodeName = node->tag->name;
    if(nodeName.left(1) == "/")
      nodeName = nodeName.mid(1);
    return isChild(nodeName, trueIfNoChildsDefined);
  }
  else if(node->tag->type == Tag::ScriptTag)
    //FIXME: It might depend of scripts...
    return true;
  else
    return isChild(node->tag->name, trueIfNoChildsDefined);
}

QPtrList<QTag> QTag::parents()
{
  QPtrList<QTag> qTagList;
  QDictIterator<QTag> it((*parentDTD->tagsList));
  for(; it.current(); ++it)
  {
    if(it.current()->isChild(name()))
      qTagList.append(it.current());
  }
  return qTagList;
}
