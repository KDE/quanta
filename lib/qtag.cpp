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
#include "dtdstruct.h"

#include <kdebug.h>

QTag::QTag()
{
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

  uint count = t.attributeCount();
  for (uint i = 0; i < count; i++)
  {
    addAttribute(t.attributeAt(i));
  }
}

QTag::~QTag()
{
   while (!attrs.isEmpty())
        delete attrs.takeFirst();
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
  QStringList::ConstIterator end(attr->values.constEnd());
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
int QTag::attributeCount() const
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
//   Attribute *a;
  AttributeList *groupAttrs;

  //Check in the QTag specific attributes
  for (int i = 0; i < attrs.size(); ++i)
  {
        if (attrs.at(i)->name == attrName)
            return true;
  }

  //Check in the common attributes
  uint count = commonGroups.count();
  for (uint i = 0; i < count; i++)
  {
    groupAttrs = parentDTD->commonAttrs->value(commonGroups[i]);
    if (groupAttrs)
    {
      for (int i = 0; i < groupAttrs->size(); ++i)
      {
        if(groupAttrs->at(i)->name == attrName)
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
      return tagName.toUpper();
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

  uint count = t.attributeCount();
  for (uint i = 0; i < count; ++i)
  {
    addAttribute(t.attributeAt(i));
  }
  return *this;
}

/** Returns the attribute with name, or 0 if the tag does not have any attribute with name. */
Attribute* QTag::attribute(const QString& name)
{
 uint count = attrs.count();
 for (uint i = 0; i < count; i++)
 {
   if (attrs.at(i)->name == name)
     return attrs.at(i);
 }
 return 0;
}

bool QTag::isChild(const QString& tag, bool trueIfNoChildsDefined)
{
  QString tagName = parentDTD->caseSensitive ? tag : tag.toUpper();
  if(trueIfNoChildsDefined)
    return (childTags.isEmpty() || childTags.contains(tagName));
  else
    return (!childTags.isEmpty() && childTags.contains(tagName));
}

bool QTag::isChild(Node *node, bool trueIfNoChildsDefined, bool treatEmptyNodesAsText)
{
  if(!node)
    return false;

  if(node->tag->isType(Tag::Text))
  {
    if(trueIfNoChildsDefined)
      return(childTags.isEmpty() || childTags.contains("#text") || childTags.contains("#TEXT"));
    else
      return(!childTags.isEmpty() && (childTags.contains("#text") || childTags.contains("#TEXT")));
  }

  if(node->tag->isType(Tag::Empty) && !treatEmptyNodesAsText)
    return true;

  if(node->tag->isType(Tag::Empty) && treatEmptyNodesAsText)
  {
    if(trueIfNoChildsDefined)
      return(childTags.isEmpty() || childTags.contains("#text") || childTags.contains("#TEXT"));
    else
      return(!childTags.isEmpty() && (childTags.contains("#text") || childTags.contains("#TEXT")));
  }

  if(node->tag->isType(Tag::XmlTagEnd))
  {
    QString nodeName;
    nodeName = node->tag->name();
    if(nodeName.left(1) == "/")
      nodeName = nodeName.mid(1);

    return isChild(nodeName, trueIfNoChildsDefined);
  }

  if(node->tag->isType(Tag::ScriptTag))
    //FIXME: It might depend of scripts...
    return true;

  return isChild(node->tag->name(), trueIfNoChildsDefined);
}

QList<QTag*> QTag::parents()
{
  QList<QTag*> qTagList;
  QHashIterator<QString, QTag*> it(*parentDTD->tagsList);
  while (it.hasNext())
  {
    it.next();
    if(it.value()->isChild(name()))
      qTagList.append(it.value());
  }
  return qTagList;
}

/** Returns the QTag object for the tag "tag" from the DTD named "dtdname". */
// QTag* QTag::tagFromDTD(const QString& dtdName, const QString& tag)
// {
//   const DTDStruct* dtd = DTDs::ref()->contains(dtdName);
//   return tagFromDTD(dtd, tag);
// }

/** Returns the QTag object for the tag "tag" from the DTD. */
QTag* QTag::tagFromDTD(const DTDStruct *dtd, const QString& tag)
{
  QTag *qtag = 0;
  if (dtd && !tag.isEmpty())
  {
    QString searchForTag = (dtd->caseSensitive) ? tag : tag.toUpper();
    qtag = dtd->tagsList->value(searchForTag);
  }

 return qtag;
}

/** Returns the QTag object for the node "node" from node's DTD. */
QTag* QTag::tagFromDTD(Node *node)
{
  if(!node || !node->tag)
    return 0L;

  return tagFromDTD(node->tag->dtd(), node->tag->name());
}
