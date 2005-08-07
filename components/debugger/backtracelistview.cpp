/***************************************************************************
                          backtracelistview.cpp
                        --------------------------
    begin                : 2005-07-31
    copyright            : (C) 2005 Linus McCabe

 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/

// KDE Includes
#include <klocale.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <kdeversion.h>
#include <klistview.h>

// Quanta includes
#include "backtracelistview.h"
#include "debuggerclient.h"
#include "debuggermanager.h"
#include "resource.h"
#include "quanta.h"

namespace BacktraceListviewColumns
{
  // The enums must correspond to the order of the columns
  // If you change here, change the column adding
  enum Columns
  {
    Level = 0,
    Type,
    File,
    Line,
    Function
  };
}

BacktraceListviewItem::BacktraceListviewItem(BacktraceListview* view)
  : KListViewItem(view)
{
}


BacktraceListview::BacktraceListview(QWidget *parent, const char *name)
  : KListView(parent, name)
{
  // If you change here, change the BacktraceListviewColumns enums above
  addColumn("#");
  addColumn(i18n("Type"));
  addColumn(i18n("File"));
  addColumn(i18n("Line"));
  addColumn(i18n("Function"));

  setSorting(BacktraceListviewColumns::Level);  // Sort on the level column
  setResizeMode(QListView::AllColumns);
  setAllColumnsShowFocus(true);

  // Jump to bt
  connect(this, SIGNAL( doubleClicked( QListViewItem *, const QPoint &, int) ), this, SLOT(slotBacktraceDoubleClick( QListViewItem *, const QPoint &, int)));
}


BacktraceListview::~BacktraceListview()
{}

void BacktraceListview::backtraceShow(int level, BacktraceType type, const QString& filename, long line, const QString& func)
{
  BacktraceListviewItem item(this);
  item.setLevel(level);
  item.setType(type);
  item.setFilename(filename);
  item.setLine(line);
  item.setFunc(func);
  
  item.setText(BacktraceListviewColumns::File, filename);
  item.setText(BacktraceListviewColumns::Level, "level");
}

void BacktraceListview::keyPressEvent(QKeyEvent *e)
{
  if(e->key() != Qt::Key_Enter)
  {
    e->ignore();
    return;
  }

  if(selectedItem())
    jumpHistory(selectedItem());
}

void BacktraceListview::clear()
{
  KListView::clear();
}

void BacktraceListview::slotBacktraceDoubleClick(QListViewItem *item, const QPoint &, int )
{
  if(!item)
    return;

  jumpHistory(item);
}

void BacktraceListview::jumpHistory(QListViewItem *item)
{
  BacktraceListviewItem* btitem = dynamic_cast<BacktraceListviewItem*>(item);
  
  if(btitem->type() == File)
  {
    quantaApp->gotoFileAndLine(btitem->filename(), btitem->line(), 0);
  }

}

#include "backtracelistview.moc"
