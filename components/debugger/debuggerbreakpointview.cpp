/***************************************************************************
                        debuggerbreakpointview.cpp
                        --------------------------
    begin                : 2004-06-27
    copyright            : (C) 2004 Linus McCabe

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
#include <kinputdialog.h>
#include <klistview.h>

// Quanta includes
#include "debuggerbreakpointview.h"
#include "debuggerbreakpoint.h"
#include "resource.h"
#include "quanta.h"
#include "debuggerclient.h"
#include "debuggermanager.h"

namespace DebuggerBreakpointViewColumns
{
  // The enums must correspond to the order of the columns
  // If you change here, change the column adding
  enum Columns
  {
    File = 0,
    Expression,
    Line,
    Value
  };
}

DebuggerBreakpointView::DebuggerBreakpointView(QWidget *parent, const char *name)
    : KListView(parent, name)
{
  // If you change here, change the DebuggerBreakpointViewColumns enums above
  addColumn(i18n("File"));
  addColumn(i18n("Expression"));
  addColumn(i18n("Line"));
  addColumn(i18n("State"));

  setResizeMode(QListView::AllColumns);
  setAllColumnsShowFocus(true);

  m_breakpointPopup = new KPopupMenu(this);
  m_breakpointPopup->insertItem(SmallIcon("editdelete"), i18n("&Remove"), this, SLOT(slotRemoveSelected()));

  connect(this, SIGNAL( contextMenu( KListView *, QListViewItem *, const QPoint & ) ), this, SLOT(slotBreakpointContextMenu(KListView *, QListViewItem *, const QPoint &)));
}


DebuggerBreakpointView::~DebuggerBreakpointView()
{}

void DebuggerBreakpointView::deleteBreakpoint(const DebuggerBreakpoint &bp)
{
  QListViewItem *item = findBreakpoint(bp, false);
  if(item)
  {
    delete item;
  }
}


void DebuggerBreakpointView::showBreakpoint(const DebuggerBreakpoint &bp)
{
  QListViewItem *item = findBreakpoint(bp);
  if(!item)
    return;

  if(bp.condition().isEmpty())
    item->setText(DebuggerBreakpointViewColumns::Value, "");
  else
    switch(bp.state())
    {
      case DebuggerBreakpointStates::Fulfilled:
        item->setText(DebuggerBreakpointViewColumns::Value, i18n("Fulfilled"));
        break;

      case DebuggerBreakpointStates::Error:
        item->setText(DebuggerBreakpointViewColumns::Value, i18n("Error"));
        break;

      case DebuggerBreakpointStates::Unfulfilled:
        item->setText(DebuggerBreakpointViewColumns::Value, "");
        break;

      default:
        item->setText(DebuggerBreakpointViewColumns::Value, i18n("Undefined"));
    }
  item->setText(DebuggerBreakpointViewColumns::File, bp.filePath());
  item->setText(DebuggerBreakpointViewColumns::Expression, bp.condition());
  item->setText(DebuggerBreakpointViewColumns::Line, (bp.condition().isEmpty() ? QString::number((bp.line())) : QString()));

}

QListViewItem* DebuggerBreakpointView::findBreakpoint(const DebuggerBreakpoint& bp, bool addIfNotExist)
{
  // Find the old item if its there
  QListViewItem * item = firstChild() ;
  while(item)
  {
    if(item->text(DebuggerBreakpointViewColumns::File) == bp.filePath()
    && item->text(DebuggerBreakpointViewColumns::Expression) == bp.condition()
    && item->text(DebuggerBreakpointViewColumns::Line) == (bp.condition().isEmpty() ? QString::number(bp.line()) : QString())
    )
      break;

    item = item->nextSibling();
  }

  // Insert a new item
  if(!item && addIfNotExist)
  {
    item = new QListViewItem(this);
    insertItem(item);
  }

  return item;
}


DebuggerBreakpoint* DebuggerBreakpointView::selected()
{
  if(!selectedItem())
    return NULL;

  DebuggerBreakpoint* bp = new DebuggerBreakpoint(
    selectedItem()->text(DebuggerBreakpointViewColumns::File),
    selectedItem()->text(DebuggerBreakpointViewColumns::Line).toInt(),
    selectedItem()->text(DebuggerBreakpointViewColumns::Expression));
  return bp;
}

void DebuggerBreakpointView::slotRemoveSelected()
{
  DebuggerBreakpoint* bp = selected();

  if(!bp)
    return;

  delete selectedItem();
  emit removeBreakpoint(bp);
  delete bp;
}

void DebuggerBreakpointView::keyPressEvent(QKeyEvent *e)
{
  if(e->key() != Qt::Key_Delete)
  {
    e->ignore();
    return;
  }

  slotRemoveSelected();
}

void DebuggerBreakpointView::clear()
{
  KListView::clear();
}

void DebuggerBreakpointView::slotBreakpointContextMenu(KListView *, QListViewItem *, const QPoint& point)
{
  if(!selectedItem())
    return;

  m_breakpointPopup->exec(point);
}

#include "debuggerbreakpointview.moc"
