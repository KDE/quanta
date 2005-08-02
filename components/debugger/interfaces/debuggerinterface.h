/***************************************************************************
                            debuggerinterface.h
                            -------------------
    begin                : 2004-03-12
    copyright            : (C) 2004 Linus McCabe <linus@mccabe.nu>

 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/

#ifndef DEBUGGERINTERFACE_H
#define DEBUGGERINTERFACE_H

#include <qobject.h>
#include <qstring.h>
#include <kurl.h>

#include "debuggervariable.h"
#include "debuggerbreakpoint.h"
#include "backtracelistview.h"

class PathMapper;

class DebuggerInterface : public QObject
{
    Q_OBJECT

  private:


  public:
    DebuggerInterface(QObject *parent, const char* name);

    // Breakpoints
    virtual void haveBreakpoint (const QString& file, int line) = 0;
    virtual void havenoBreakpoint (const QString& file, int line) = 0;

    // Public help functions
    virtual bool showStatus(const QString& message, bool log) = 0;
    virtual bool setActiveLine (const QString& file, int line) = 0;

    virtual void enableAction(const QString& action, bool enable) = 0;
    virtual void fileOpened(const QString& file) = 0;
    virtual void sendRequest(const KURL &url) = 0;
    virtual const QString activeFileParts(const QString & str) = 0;

    // Watch handling
    //virtual void preWatchUpdate() = 0;
    //virtual void postWatchUpdate() = 0;

    virtual DebuggerVariable* newDebuggerVariable(const QString& name, const QString& value, int type) = 0;
    virtual DebuggerVariable* newDebuggerVariable(const QString& name, const ValueList_t& values, int type) = 0;
    virtual void showVariable(DebuggerVariable*) = 0;

    // Backtrace
    virtual void backtraceClear() = 0;
    virtual void backtraceShow(long level, BacktraceType type, const QString &filename, long line, const QString& func) = 0;

    
    // Breakpoints
    virtual void showBreakpoint(const DebuggerBreakpoint &bp) = 0;
    virtual void refreshBreakpoints() = 0;
    virtual DebuggerBreakpoint * newDebuggerBreakpoint() = 0;
    virtual DebuggerBreakpoint * findDebuggerBreakpoint(const QString& key) = 0;
    virtual void updateBreakpointKey(const DebuggerBreakpoint &bp, const QString& newkey) = 0;

    // Path mapping
    virtual PathMapper* Mapper() = 0;
};

#endif


