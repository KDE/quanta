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
#include "debuggerclient.h"
#include "debuggermanager.h"
#include "resource.h"
#include "quanta.h"

namespace DebuggerBreakpointViewColumns
{
  // The enums must correspond to the order of the columns
  // If you change here, change the column adding
  enum Columns
  {
    Expression = 0,
    File,
    Class,
    Function,
    Line,
    Value
  };
}

DebuggerBreakpointViewItem::DebuggerBreakpointViewItem(DebuggerBreakpointView* view)
   : KListViewItem(view)
{
}


DebuggerBreakpointView::DebuggerBreakpointView(QWidget *parent, const char *name)
    : KListView(parent, name)
{
  // If you change here, change the DebuggerBreakpointViewColumns enums above
  addColumn(i18n("Expression"));
  addColumn(i18n("File"));
  addColumn(i18n("Class"));
  addColumn(i18n("Function"));
  addColumn(i18n("Line"));
  addColumn(i18n("Value"));

  setResizeMode(QListView::AllColumns);
  setAllColumnsShowFocus(true);

  m_breakpointPopup = new KPopupMenu(this);
  m_breakpointPopup->insertItem(SmallIcon("editdelete"), i18n("&Remove"), this, SLOT(slotRemoveSelected()));

  connect(this, SIGNAL( contextMenu( KListView *, QListViewItem *, const QPoint & ) ), this, SLOT(slotBreakpointContextMenu(KListView *, QListViewItem *, const QPoint &)));

  // Jump to bp
  connect(this, SIGNAL( doubleClicked( QListViewItem *, const QPoint &, int) ), this, SLOT(slotBreakpointDoubleClick( QListViewItem *, const QPoint &, int)));
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

  if(bp.type() == DebuggerBreakpoint::LineBreakpoint)
  {
    item->setText(DebuggerBreakpointViewColumns::Value, "");
    item->setText(DebuggerBreakpointViewColumns::Line, QString::number(bp.line() + 1));
  }
  else
  {
    item->setText(DebuggerBreakpointViewColumns::Value, bp.value());
    item->setText(DebuggerBreakpointViewColumns::Line, "");
  }
  item->setText(DebuggerBreakpointViewColumns::File, bp.filePath());
  item->setText(DebuggerBreakpointViewColumns::Expression, bp.condition());
  item->setText(DebuggerBreakpointViewColumns::Class, bp.inClass());
  item->setText(DebuggerBreakpointViewColumns::Function, bp.inFunction());
}

QListViewItem* DebuggerBreakpointView::findBreakpoint(const DebuggerBreakpoint& bp, bool addIfNotExist)
{
  // Find the old item if its there
  DebuggerBreakpointViewItem* item = dynamic_cast<DebuggerBreakpointViewItem*>(firstChild());
  while(item)
  {
    if(item->breakpoint() == bp)
      break;

    item =  dynamic_cast<DebuggerBreakpointViewItem*>(item->nextSibling());
  }

  // Insert a new item
  if(!item && addIfNotExist)
  {
    item = new DebuggerBreakpointViewItem(this);
    item->setBreakpoint(bp);
    insertItem(item);
  }

  return item;
}


DebuggerBreakpoint DebuggerBreakpointView::selected()
{
  DebuggerBreakpointViewItem* bpitem = dynamic_cast<DebuggerBreakpointViewItem*>(selectedItem());

  return bpitem->breakpoint();
}

void DebuggerBreakpointView::slotRemoveSelected()
{
  if(!selectedItem())
    return;

  DebuggerBreakpoint bp = selected();

  emit removeBreakpoint(&bp);
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

void DebuggerBreakpointView::slotBreakpointDoubleClick(QListViewItem *item, const QPoint &, int )
{
  if(!item)
    return;

  DebuggerBreakpointViewItem* bpitem = dynamic_cast<DebuggerBreakpointViewItem*>(item);
  
  if(!bpitem->breakpoint().filePath().isEmpty())
  {
    quantaApp->gotoFileAndLine(bpitem->breakpoint().filePath(), bpitem->breakpoint().line(), 0);
  }

}

void DebuggerBreakpointView::slotBreakpointContextMenu(KListView *, QListViewItem *, const QPoint& point)
{
  if(!selectedItem())
    return;

  m_breakpointPopup->exec(point);
}

#include "debuggerbreakpointview.moc"
