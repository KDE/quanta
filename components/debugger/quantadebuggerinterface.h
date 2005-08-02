/***************************************************************************
                               debugmanager.h
                             ------------------
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

#ifndef MYDEBUGMANAGER_H
#define MYDEBUGMANAGER_H

#include <qobject.h>
#include <qstring.h>
#include "debuggerinterface.h"
#include "debuggervariable.h"

class DebuggerManager;

class QuantaDebuggerInterface : public DebuggerInterface
{
    Q_OBJECT

  private:
    DebuggerManager *m_manager;

  public:
    QuantaDebuggerInterface(QObject *myparent, const char* name);
    ~QuantaDebuggerInterface();

    // Breakpoints
    void haveBreakpoint (const QString& file, int line) ;
    void havenoBreakpoint (const QString& file, int line) ;
    void refreshBreakpoints();

    // Public help functions
    bool showStatus(const QString& message, bool log);
    bool setActiveLine (const QString& file, int line) ;

    void enableAction(const QString& action, bool enable);
    void fileOpened(const QString& file) ;
    void sendRequest(const KURL &url);
    virtual const QString activeFileParts(const QString & str);

    // Watches handling
    //void preWatchUpdate();
    //void postWatchUpdate();
    DebuggerVariable* newDebuggerVariable(const QString& name, const QString& value, int type);
    DebuggerVariable* newDebuggerVariable(const QString& name, const ValueList_t& values, int type);
    void showVariable(DebuggerVariable*);

    // Breakpoints
    void showBreakpoint(const DebuggerBreakpoint &bp);
    DebuggerBreakpoint * newDebuggerBreakpoint();
    DebuggerBreakpoint * findDebuggerBreakpoint(const QString& key);
    void updateBreakpointKey(const DebuggerBreakpoint &bp, const QString& newkey);

    // Backtrace
    void backtraceClear();
    void backtraceShow(long level, BacktraceType type, const QString &filename, long line, const QString& func);

    // Path mapping
    PathMapper* Mapper();
};

#endif

