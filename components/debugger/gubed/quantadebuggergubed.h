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
#include <qptrlist.h>
#include <kurl.h>
#include <qdom.h>

#include "debuggerclient.h"

class QuantaDebuggerGubed : public DebuggerClient {
  Q_OBJECT
  
  public:
    QuantaDebuggerGubed(QObject *parent, const char* name, const QStringList&);
    ~QuantaDebuggerGubed();
    
    // Manager interaction
    const uint supports(DebuggerClient::Capabilities);
    
    // Execution control
    void run();
    void leap();
    void skip();
    void stepInto();
    void pause();
    void kill();
    
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
  
    // Variables
    void addWatch(const QString &variable);
    void removeWatch(DebuggerVariable *var);
    
  private:
    QSocket *m_socket;
    QString m_command;
    
    QString serverBasedir;
    QString localBasedir;
    QString serverPort;
    QString serverHost;    
    
    bool sendCommand(QString, QString);
    void debuggingState(bool enable);
    
    QString mapServerPathToLocal(QString serverpath);
    QString mapLocalPathToServer(QString localpath);
    void showWatch(QString data);

  public slots:
    // Socket slots
    void slotConnected();
    void slotConnectionClosed();
    void slotError(int);
    void slotReadyRead();

  signals:

};

#endif
