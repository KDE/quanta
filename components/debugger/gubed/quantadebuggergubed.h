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

#include <qserversocket.h>
#include <qsocket.h>
#include <kextsock.h>
#include <qptrlist.h>
#include <kurl.h>
#include <qdom.h>

#include "debuggerclient.h"

typedef QValueList<QString> WatchList;

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
      RunDisplay,
      RunNoDisplay
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
    void leap();
    void skip();
    void stepInto();
    void pause();
    void kill();
    void setExecutionState(State newstate);

    // Connection
    void startSession();
    void endSession();

    // Return name of debugger
    QString getName();

    // New file opened in quanta
    void fileOpened(QString file);

    // Settings
    void readConfig(QDomNode node);
    void showConfig(QDomNode node);

    // Breakpoints
    void addBreakpoint(DebuggerBreakpoint* breakpoint);
    void removeBreakpoint(DebuggerBreakpoint* breakpoint);
    void showCondition(const QString &expression);

    // Variables
    void addWatch(const QString &variable);
    void removeWatch(DebuggerVariable *var);
    void variableSetValue(const DebuggerVariable &variable);

  private:
    KExtendedSocket *m_socket;
    KExtendedSocket *m_server;
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

    bool sendCommand(QString, QString);
    void processCommand(QString);
    void sendWatches();
    void sendBreakpoints();
    void debuggingState(bool enable);

    QString mapServerPathToLocal(QString serverpath);
    QString mapLocalPathToServer(QString localpath);
    void showWatch(QString data);

  public slots:
    // Socket slots
    void slotConnected();
    void slotConnectionClosed(int state);
    void slotError(int);
    void slotReadyRead();
    void slotReadyAccept();

  signals:

};

#endif
