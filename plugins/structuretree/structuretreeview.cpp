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

StructureTreeView::StructureTreeView(QWidget *parent)
 : QTreeView(parent)
{
  m_parseResult = 0L;
}


StructureTreeView::~StructureTreeView()
{
}

void StructureTreeView::newDataArrived(const ParseResult *data)
{
  m_parseResult = data;
  if (data)
    setModel(m_parseResult->model);
  else
    setModel(0L);
}
