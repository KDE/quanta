/*
 * XDebug Debugger Support
 *
 * Copyright 2009 Niko Sams <niko.sams@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef XDEBUG_DEBUGSESSION_H
#define XDEBUG_DEBUGSESSION_H

#include <QtCore/QString>
#include <debugger/interfaces/idebugsession.h>
#include <interfaces/irunprovider.h>

class KJob;
class QTcpServer;
class KProcess;

namespace XDebug {
class Connection;

class DebugSession : public KDevelop::IDebugSession
{
    Q_OBJECT
public:
    DebugSession(Connection* connection);
    
    bool waitForState(DebuggerState state, int msecs = 30000);
    bool waitForFinished(int msecs = 30000);
    bool waitForConnected(int msecs = 30000);

    Connection* connection();
    KProcess* process();
    void setProcess(KProcess* process);

    virtual DebuggerState state() const;
    virtual KDevelop::StackModel* stackModel() const;
    
    void stackGet();

    virtual bool restartAvaliable() const;

Q_SIGNALS:
    void output(QString line, KDevelop::IRunProvider::OutputTypes type);
    void outputLine(QString line,KDevelop::IRunProvider::OutputTypes type);
    void initDone(const QString& ideKey);

public Q_SLOTS:
    virtual void run();
    virtual void toggleBreakpoint();
    virtual void stepOut();
    virtual void stepOverInstruction();
    virtual void stepInto();
    virtual void stepIntoInstruction();
    virtual void stepOver();
    virtual void jumpToCursor();
    virtual void runToCursor();
    virtual void interruptDebugger();
    virtual void stopDebugger();
    virtual void restartDebugger();
    virtual void startDebugger();

    void eval(QByteArray source);

private:
    Connection* m_connection;
    KProcess* m_process;

};

}

#endif // XDEBUG_DEBUGSESSION_H
