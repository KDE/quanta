/***************************************************************************
    begin                : Thu Apr 17 2008
    copyright            : (C) 2008 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/


//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
#include "structuretreeview.h"
#include "parseresult.h"
#include "dommodel.h"

#include <ktexteditor/range.h>

#include <QHeaderView>
#include <QModelIndex>

StructureTreeView::StructureTreeView(QWidget *parent)
 : QTreeView(parent)
{
  m_parseResult = 0L;
  connect(this, SIGNAL(activated(const QModelIndex &)), this, SLOT(modelIndexActivated(const QModelIndex &)));
  connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(modelIndexClicked(const QModelIndex &)));
  header()->hide();
}


StructureTreeView::~StructureTreeView()
{
}

void StructureTreeView::newDataArrived(const ParseResult *data)
{
  m_parseResult = data;
  if (data)
  {
    setModel(m_parseResult->model);
    expandToDepth(2);
  }
  else
    setModel(0L);
}

void StructureTreeView::modelIndexActivated(const QModelIndex &index)
{
  DomModelItem *item = static_cast<DomModelItem*>(index.internalPointer());
  if (!item)
    return;
  
  KTextEditor::Range range(*(item->range()));
  if (item->type() == DomModelItem::TagStart && item->parent())
  {
    int row = item->row();
    DomModelItem *closingItem = item->parent()->child(row + 1);
    if (closingItem)
    {
      range.end() = closingItem->range()->end();
    }
  } 
  
  emit selectRange(range);
}

void StructureTreeView::modelIndexClicked(const QModelIndex &index)
{
  DomModelItem *item = static_cast<DomModelItem*>(index.internalPointer());
  if (!item)
    return;
  
  KTextEditor::Cursor cursor(item->range()->start());
  if (item->type() == DomModelItem::TagStart || item->type() == DomModelItem::TagEnd)
  {
    cursor.setColumn(cursor.column() + 1);
  } 
  
  emit setCursorPosition(cursor);
}
