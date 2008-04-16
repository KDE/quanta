/***************************************************************************
    begin                : Wed Apr 16 2008
    copyright            : (C) 2008 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include "treelement.h"

#include <ktexteditor/range.h>

TreeElement::TreeElement(Type type)
{
  m_parent = 0L;
  m_child = 0L;
  m_next = 0L;
  m_previous = 0L;
  m_range = 0L;
  m_type = type;
}

TreeElement::~TreeElement()
{
  delete m_range;
}

void TreeElement::insertAfter(TreeElement *after)
{
  if (!after)
    return;
  m_parent = after->parent();  
  m_previous = after;
  TreeElement *oldNext = after->next();
  after->setNext(this);
  m_next = oldNext;
  if (oldNext)
    oldNext->setPrevious(this);
}

void TreeElement::insertBelow(TreeElement *parent)
{
  if (!parent)
    return;
  TreeElement *lastChild = parent->firstChild();
  if (!lastChild)
  {
    m_parent = parent;
    m_parent->setChild(this);
  } else
  {
    while (lastChild->next())
      lastChild = lastChild->next();
    insertAfter(lastChild);
  }
}

void TreeElement::setChild(TreeElement *child)
{
  m_child = child;
}

void TreeElement::setNext(TreeElement *element)
{
  m_next = element;
}

void TreeElement::setPrevious(TreeElement *element)
{
  m_previous = element;
}

void TreeElement::setRange(KTextEditor::Range *range)
{
  if (range != m_range)
    delete m_range;
  m_range = range;
}

void TreeElement::setName(const QString &name)
{
  m_name = name;
}

void TreeElement::setNameSpace(const QString &nameSpace)
{
  m_nameSpace = nameSpace;
}

void TreeElement::setAttributes(const QXmlAttributes & attributes)
{
  m_attributes = attributes;
}

void TreeElement::setAttributeRanges(const QVector<KTextEditor::Range> & attrRanges)
{
  m_attributeRanges = attrRanges;
}

//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8

