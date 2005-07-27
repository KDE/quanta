/***************************************************************************
                          phpdebuggubed.cpp
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

typedef QValueList<QString> WatchList;
typedef QMap<QString, QString> StringMap;

class QuantaDebuggerGubed : public DebuggerClient
{
    Q_OBJECT

  public:
    QuantaDebuggerGubed(QObject *parent, const char* name, const QStringList&);
    ~QuantaDebuggerGubed();

    // Execution states
    enum State
    {
      Pause = 0,
      Trace,
      Run
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
    void trace();
    void run();
    void skip();
    void stepInto();
    void stepOver();
    void stepOut();
    void pause();
    void kill();
    void setExecutionState(State newstate);

    // Connection
    void startSession();
    void endSession();

    // Return name of debugger
    QString getName();

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

  private:
    KNetwork::KStreamSocket *m_socket;
    KNetwork::KServerSocket *m_server;
    QString m_command, m_buffer;
    long    m_datalen;

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

    WatchList m_watchlist;

//     bool sendCommand(const QString&, const QString&);
    bool sendCommand(const QString& command, StringMap args);
    bool sendCommand(const QString& command, char * firstarg, ...);

    void processCommand(const QString&);
    void sendWatches();
    void sendBreakpoints();
    void debuggingState(bool enable);
    void connected();

    QString mapServerPathToLocal(const QString& serverpath);
    QString mapLocalPathToServer(const QString& localpath);
    QString bpToGubed(DebuggerBreakpoint* breakpoint);

    // Communication helpers
    QString phpSerialize(StringMap args);
    StringMap parseArgs(const QString &args);

    // Variables
    DebuggerVariable* parsePHPVariables(const QString &varstring);
    DebuggerVariable* parsePHPVariables(QString &str);
    void showWatch(const QString& data);

    
  public slots:
    // Socket slots
    void slotConnected(const KNetwork::KResolverEntry &);
    void slotConnectionClosed();
    void slotError(int error);
    void slotReadyRead();
    void slotReadyAccept();

  signals:
    void updateStatus(DebuggerUI::DebuggerStatus);
};

#endif
