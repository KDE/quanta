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


class VariablesListView;
class DebuggerVariable;

class DebuggerUI : public QObject
{
    Q_OBJECT

  public:
    DebuggerUI(QObject *parent = 0, const char *name = 0);
    ~DebuggerUI();

    // Watches
    void preWatchUpdate();
    void postWatchUpdate();

    void setVariables(const QPtrList<DebuggerVariable>&);
    void addVariable(DebuggerVariable* var);
    void parsePHPVariables(const QString &);

    void showMenu();
    void hideMenu();

  private:
    VariablesListView* m_variablesListView;
    KMdiToolViewAccessor* m_variableListViewTVA;
    int m_debuggerMenuID;
};

#endif
