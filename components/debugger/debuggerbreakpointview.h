/***************************************************************************
                           Breakpoinlistview.h
                         ------------------------
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


#ifndef DEBUGGERBREAKPOINTVIEW_H
#define DEBUGGERBREAKPOINTVIEW_H

#include <klistview.h>
#include <kpopupmenu.h>
#include <qptrlist.h>

class DebuggerBreakpoint;

class DebuggerBreakpointView : public KListView
{
    Q_OBJECT

  public:
    DebuggerBreakpointView(QWidget *parent = 0, const char *name = 0);
    ~DebuggerBreakpointView();

    void showBreakpoint(const DebuggerBreakpoint& bp);
    void deleteBreakpoint(const DebuggerBreakpoint& bp);

    DebuggerBreakpoint* selected();

    void clear();

  public slots:
    void slotRemoveSelected();
    void slotBreakpointContextMenu(KListView *list, QListViewItem * item, const QPoint& point);

  signals:
    void removeBreakpoint(DebuggerBreakpoint*);

  private:
    void keyPressEvent(QKeyEvent *e);
    QListViewItem* findBreakpoint(const DebuggerBreakpoint& bp, bool addIfNotExist = true);

    KPopupMenu *m_breakpointPopup;
};

#endif
