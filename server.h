/*
   Copyright 2009 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef XDEBUG_SERVER_H
#define XDEBUG_SERVER_H

#include <QObject>
#include <interfaces/irunprovider.h>
#include <debugger/interfaces/idebugsession.h>

class QTcpServer;
class KProcess;

namespace XDebug {
class DebugSession;

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QObject* parent = 0);
    bool listen(int port = 9000);
    KProcess* startDebugger(const QString& filename);
    bool waitForConnected(int msec = 30000);
    DebugSession* lastSession();

Q_SIGNALS:
    void sessionStarted(DebugSession* session);
    void outputLine(DebugSession* session, QString line, KDevelop::IRunProvider::OutputTypes type);
    void stateChanged(DebugSession*, KDevelop::IDebugSession::DebuggerState state);

private Q_SLOTS:
    void incomingConnection();
    void processReadyRead();
    void processFinished(int extCode);
    void outputLine(QString line,KDevelop::IRunProvider::OutputTypes type);
    void initDone(const QString& ideKey);
    void stateChanged(KDevelop::IDebugSession::DebuggerState state);

private:
    QTcpServer* m_server;
    DebugSession* m_lastSession;
    int m_port;
    QMap<QString, KProcess*> m_processes;
    QMap<QString, DebugSession*> m_sessions;
    QMap<KProcess*, KJob*> m_jobs;
};

}

#endif // XDEBUG_SERVER_H
