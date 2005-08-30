/***************************************************************************
                               debuggerui.h
                         ------------------------
    begin                : 2004-04-04
    copyright            : (C) 2004 Thiago Silva

 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/


#ifndef DEBUGGERUI_H
#define DEBUGGERUI_H

#include <qobject.h>
#include <qptrlist.h>
#include <kmditoolviewaccessor.h>
#include "backtracelistview.h"

class VariablesListView;
class DebuggerBreakpointView;
class BacktraceListview;
class DebuggerBreakpoint;
class DebuggerVariable;
class WHTMLPart;
class KURL;

class DebuggerUI : public QObject
{
    Q_OBJECT

  public:
    enum DebuggerStatus
    {
      NoSession = 0,
      AwaitingConnection,
      Connected,
      Paused,
      Running,
      Tracing,
      HaltedOnError,
      HaltedOnBreakpoint
    };

    DebuggerUI(QObject *parent = 0, const char *name = 0);
    ~DebuggerUI();

    // Watches
    void addVariable(DebuggerVariable* var);
    void showBreakpoint(const DebuggerBreakpoint& bp);
    void deleteBreakpoint(const DebuggerBreakpoint& bp);
    void parsePHPVariables(const QString &);
    void sendRequest(const KURL &url);
    VariablesListView* watches() { return m_variablesListView; };
    
    void showMenu();
    void hideMenu();

    void backtraceClear();
    void backtraceShow(long level, BacktraceType type, const QString &filename, long line, const QString& func);
    
  private:
    VariablesListView* m_variablesListView;
    KMdiToolViewAccessor* m_variableListViewTVA;
    KMdiToolViewAccessor* m_previewTVA;
    
    DebuggerBreakpointView* m_debuggerBreakpointView;
    KMdiToolViewAccessor* m_debuggerBreakpointViewTVA;
    
    BacktraceListview* m_backtraceListview;
    KMdiToolViewAccessor* m_backtraceListviewTVA;

    int m_debuggerMenuID;

    WHTMLPart *m_preview;

  public slots:
    // Status indication 
    void slotStatus(DebuggerUI::DebuggerStatus status);

};

#endif
