/***************************************************************************
                          phpdebugdbgp.cpp
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

#ifndef QUANTADEBUGGERGUBED_H
#define QUANTADEBUGGERGUBED_H

#include <kserversocket.h>
#include <kstreamsocket.h>
#include <qptrlist.h>
#include <kurl.h>
#include <qdom.h>

#include "debuggerclient.h"
#include "dbgpnetwork.h"

typedef QValueList<QString> WatchList;
typedef QMap<QString, QString> StringMap;

class QuantaDebuggerDBGp : public DebuggerClient
{
    Q_OBJECT

  public:
    QuantaDebuggerDBGp(QObject *parent, const char* name, const QStringList&);
    ~QuantaDebuggerDBGp();

    // Execution states
    enum State
    {
      Starting = 0,
      Stopping,
      Stopped,
      Running,
      Break
    };
    // Error codes
    enum Errors
    {
      Warning = 2,
      Notice = 8,
      User_Error = 256,
      User_Warning = 512,
      User_Notice = 1024
    };

    // Protocol version
    static const char protocolversion[];

    // Manager interaction
    const uint supports(DebuggerClientCapabilities::Capabilities);

    // Execution control
    void request();
    void run();
    void stepInto();
    void stepOver();
    void stepOut();
    void pause();
    void kill();
    void setExecutionState(const QString &state);
    void setExecutionState(const State &state, bool forcesend = false);

    // Connection
    void startSession();
    void endSession();

    // Return name of debugger
    QString getName();

    // Initiation
    void checkSupport(const QDomNode&node);

    // New file opened in quanta
    void fileOpened(const QString& file);

    // Settings
    void readConfig(QDomNode node);
    void showConfig(QDomNode node);

    // Breakpoints
    void addBreakpoint(DebuggerBreakpoint* breakpoint);
    void removeBreakpoint(DebuggerBreakpoint* breakpoint);
    void showCondition(const StringMap &args);

    // Variables
    void addWatch(const QString &variable);
    void removeWatch(DebuggerVariable *var);
    void variableSetValue(const DebuggerVariable &variable);

    // Call stack
    void showStack(const QDomNode&node);

  private:
    DBGpNetwork m_network;

    QString m_serverBasedir;
    QString m_localBasedir;
    QString m_serverPort;
    QString m_serverHost;
    QString m_startsession;
    QString m_listenPort;
    bool    m_useproxy;
    State   m_executionState, m_defaultExecutionState;
    long    m_errormask;
    long    m_displaydelay;
    bool    m_supportsasync;

    // Variable type mapping
    StringMap m_variabletypes;

    // Internal watchlist
    WatchList m_watchlist;

    void sendWatches();
    void debuggingState(bool enable);
    void connected();

    QString mapServerPathToLocal(const QString& serverpath);
    QString mapLocalPathToServer(const QString& localpath);
    void showWatch(const QString& data);
    QString bpToDBGp(DebuggerBreakpoint* breakpoint);
    void setBreakpointKey(const QDomNode& response);

    QString attribute(const QDomNode&node, const QString &attribute);
    void initiateSession(const QDomNode& initpacket);

  public slots:
    void slotNetworkActive(bool active);
    void slotNetworkConnected(bool connected);
    void slotNetworkError(const QString &errormsg, bool log);
    void processCommand(const QString&);

  signals:
    void updateStatus(DebuggerUI::DebuggerStatus);
};

#endif
