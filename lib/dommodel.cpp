/***************************************************************************
    begin                : Sep 03 2006
    copyright            : (C) 2006 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

//own includes
#include "dommodel.h"
#include "areastruct.h"

//qt includes
#include <QVariant>

//kde includes
#include <ktexteditor/range.h>


DomModelItem::DomModelItem(DomModelItem * parent) 
  : m_parentItem(parent)
{
  m_type = Empty;
  if (parent)
  {
    parent->appendItem(this);
  }
  m_range = 0L;
}


DomModelItem::~DomModelItem()
{
  delete m_range;   
}

DomModelItem * DomModelItem::child(int row) const
{
  return m_childItems.value(row); 
}


int DomModelItem::childCount() const
{
  return m_childItems.count();
}


int DomModelItem::row() const
{
  if (m_parentItem)
    return m_parentItem->m_childItems.indexOf(const_cast<DomModelItem *>(this));

  return 0;
}


QVariant DomModelItem::data(int column, int role) const
{  
  if (role == Qt::UserRole)
  {
    switch (column)
    {
      case 0: 
      {
        AreaStruct *area = new AreaStruct(m_range->start().line(), m_range->start().column(), m_range->end().line(), m_range->end().column());
        return qVariantFromValue((void*)area);
      }     
      default:
        return QVariant();      
    }
  }
      
  if (role != Qt::DisplayRole)
    return QVariant();

  switch (column)
  {
    case 0: return m_name;
//     case 1: return nodeValue().string();
  }
  return QVariant();
}
    
    
DomModelItem * DomModelItem::parent() const
{
  return m_parentItem;
}


void DomModelItem::appendItem(DomModelItem * newChild)
{
  m_childItems.append(newChild);
}

void DomModelItem::setRange(KTextEditor::Range *range)
{
  if (range != m_range)
    delete m_range;
  m_range = range;
}

void DomModelItem::setName(const QString &name)
{
  m_name = name;
}

void DomModelItem::setNameSpace(const QString &nameSpace)
{
  m_nameSpace = nameSpace;
}

void DomModelItem::setAttributes(const QXmlAttributes & attributes)
{
  m_attributes = attributes;
}

void DomModelItem::setAttributeRanges(const QVector<KTextEditor::Range> & attrRanges)
{
  m_attributeRanges = attrRanges;
}


DomModel::DomModel(DomModelItem *rootItem, QObject * parent)
  : QAbstractItemModel(parent), m_rootItem(rootItem)
{
/*  if (node)
  {
    setupModelData(&m_rootItem);
  } */
}




DomModel::~DomModel()
{
}
    
    
QModelIndex DomModel::index(int row, int column, const QModelIndex &parent) const
{
  DomModelItem * childItem;
      
  if (!parent.isValid())
    childItem = m_rootItem->child(row);
  else
    childItem = static_cast<DomModelItem *>(parent.internalPointer())->child(row);

  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}


QModelIndex DomModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  DomModelItem * childItem = static_cast<DomModelItem *>(index.internalPointer());
  DomModelItem * parentItem = childItem->parent();

  if (parentItem == m_rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}


int DomModel::rowCount(const QModelIndex &parent) const
{
  const DomModelItem * parentItem;

  if (!parent.isValid())
    parentItem = m_rootItem;
  else
    parentItem = static_cast<DomModelItem *>(parent.internalPointer());

  return parentItem->childCount();
}


int DomModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return static_cast<DomModelItem *>(parent.internalPointer())->columnCount();
  else
    return m_rootItem->columnCount();
}


QVariant DomModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  DomModelItem * item = static_cast<DomModelItem *>(index.internalPointer());

  return item->data(index.column(), role);
}


Qt::ItemFlags DomModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant DomModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return m_rootItem->data(section, role);

  return QVariant();
}
    

void DomModel::setupModelData(DomModelItem * parent)
{
/*  TreeElement *el = parent->firstChild();
  while (el)
  {
    DomModelItem * newItem = new DomModelItem(el, parent);
    parent->appendItem(newItem);
    setupModelData(newItem);
    el = el->next();
  }*/
}

#include "dommodel.moc"


//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
