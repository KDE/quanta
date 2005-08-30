/***************************************************************************
                         debuggerbreakpointlist.h
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

#ifndef DEBUGGERBREAKPOINTLIST_H
#define DEBUGGERBREAKPOINTLIST_H

#include <qobject.h>
#include <qstring.h>
#include <qvaluelist.h>

class DebuggerBreakpoint;
    typedef QValueList<DebuggerBreakpoint*> BreakpointList_t;

class DebuggerBreakpointList
{

  private:
    BreakpointList_t* m_breakpointList;
    BreakpointList_t::iterator m_current;
    BreakpointList_t::iterator find(const DebuggerBreakpoint &bp);

  public:
    DebuggerBreakpointList();
    ~DebuggerBreakpointList();

    void add(DebuggerBreakpoint*);
    void remove(DebuggerBreakpoint*);
    //int remove(QString filePath, int line);
    void clear();
    bool exists(DebuggerBreakpoint*);
    //bool exists(QString filePath, int line);

    DebuggerBreakpoint* retrieve(const QString& filePath, int line);

    DebuggerBreakpoint * findDebuggerBreakpoint(const QString& key);
    void updateBreakpointKey(const DebuggerBreakpoint &bp, const QString& newkey);

    size_t count();
    void rewind();
    DebuggerBreakpoint* next();

};

#endif
