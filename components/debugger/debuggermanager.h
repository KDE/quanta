/***************************************************************************
                          phpdebuggerinterface.h
                             -------------------
    begin                : 2004-03-12
    copyright            : (C) 2004 Linus McCabe <linus@mccabe.nu>
    Based on work by Mathieu Kooiman
 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/

#ifndef PHPDEBUGGERINTERFACE_H
#define PHPDEBUGGERINTERFACE_H

#include <qobject.h>
#include <qstring.h>

class DebuggerClient;
class QuantaDebuggerInterface;
class DebuggerBreakpointList;
class DebuggerUI;
class DebuggerVariable;
class DebuggerBreakpoint;
class PathMapper;

class DebuggerManager : public QObject
{
    Q_OBJECT

  private:
    // client
    DebuggerClient *m_client;
    QuantaDebuggerInterface * m_interface;
    DebuggerBreakpointList *m_breakpointList;
    DebuggerUI * m_debuggerui;
    PathMapper * m_pathmapper;

    // Internal help functions
    void initActions();
    void initClientActions();
    QString m_currentFile;
    long m_currentLine;

  public:
    DebuggerManager(QObject *myparent);
    ~DebuggerManager();

    // Access to memebers
    DebuggerUI * UI() { return m_debuggerui; };
    DebuggerClient * client() { return m_client; };
    PathMapper * Mapper() { return m_pathmapper; };

    // Breakpoints
    void haveBreakpoint (QString file, int line);
    void refreshBreakpoints();
    DebuggerBreakpoint *newDebuggerBreakpoint();

    // Public help functions
    bool showStatus(QString message, bool log);
    bool setActiveLine (QString file, int line);
    void setMark(QString filename, long line, bool set, int mark);

    void enableAction(QString action, bool enable);
    void fileOpened(QString file);

    bool hasClient() { return m_client != 0; };

  public slots:
    /** Execution control slots **/
    void slotDebugRequest();
    void slotDebugRun();
    void slotDebugLeap();
    void slotDebugSkip();
    void slotDebugStepOver();
    void slotDebugStepInto();
    void slotDebugStepOut();
    void slotDebugPause();
    void slotDebugKill();

    // Breakpoint
    void toggleBreakpoint();
    void clearBreakpoints();
    void slotConditionalBreakpoint();
    void slotRemoveBreakpoint(DebuggerBreakpoint* bp);

    // Watches
    void slotAddWatch();
    void slotRemoveVariable(DebuggerVariable* var);
    void slotVariableSet();

    // Connection related slots
    void slotDebugStartSession();
    void slotDebugEndSession();

    // Initiation
    void slotNewProjectLoaded(const QString &, const KURL &, const KURL &);

  signals:
    void hideSplash();
};

#endif

