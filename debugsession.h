/*
 * Crossfire Debugger Support
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

#ifndef CROSSFIRE_DEBUGSESSION_H
#define CROSSFIRE_DEBUGSESSION_H

#include <debugger/interfaces/idebugsession.h>

class QTcpServer;
namespace KDevelop {
    class ILaunchConfiguration;
}

namespace Crossfire {

class Connection;

class DebugSession : public KDevelop::IDebugSession
{
    Q_OBJECT
public:
    DebugSession();

    void setLaunchConfiguration(KDevelop::ILaunchConfiguration *cfg);
    bool listenForConnection();
    void setCurrentContext(const QString &context);

    virtual bool restartAvaliable() const { return false; }
    virtual DebuggerState state() const;

    virtual KUrl convertToLocalUrl(const KUrl& url) const;
    virtual KUrl convertToRemoteUrl(const KUrl& url) const;

    bool waitForConnected(int timeout = 30000);
    bool waitForHandshake(int timeout = 30000);

private:
    virtual KDevelop::IFrameStackModel* createFrameStackModel();

Q_SIGNALS:
    void outputLine(QString line);

public Q_SLOTS:
    virtual void run();
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

private Q_SLOTS:
    void incomingConnection();

private:
    QTcpServer *m_server;
    Connection *m_connection;
    KDevelop::ILaunchConfiguration *m_launchConfiguration;
    QString m_currentContext;

};

}

#endif // CROSSFIRE_DEBUGSESSION_H
