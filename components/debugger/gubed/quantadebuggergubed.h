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

#include "debuggerclient.h"

class QuantaDebuggerGubed : public DebuggerClient {
  Q_OBJECT
  
  public:
    QuantaDebuggerGubed(QObject *parent, const char* name, const QStringList&);
    const uint getCapabilities();
    
    // Execution control
    bool run();
    bool leap();
    bool skip();
    bool stepInto();
    bool pause();
    bool kill();
    
    // Connection
    bool startSession();
    bool endSession();
    
    // Return name of debugger
    QString getName();
    
    // New file opened in quanta
    void fileOpened(QString file);
    
    // Breakpoints
    bool addBreakpoint(DebuggerBreakpoint* breakpoint);
    bool removeBreakpoint(DebuggerBreakpoint* breakpoint);
  
  private:
    QSocket *m_socket;
    QString m_command;
    
    bool sendCommand(QString, QString);
    void debuggingState(bool enable);
    
  public slots:
    // Socket slots
    void slotConnected();
    void slotConnectionClosed();
    void slotError(int);
    void slotReadyRead();

  signals:

};

#endif
