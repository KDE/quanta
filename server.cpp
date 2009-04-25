/*
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

#include "server.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>

#include <KDebug>
#include <KProcess>

#include "connection.h"
#include "debugsession.h"

namespace XDebug {

Server::Server(QObject* parent)
    : QObject(parent), m_server(0), m_lastSession(0), m_port(-1)
{
}


bool Server::listen(int port)
{
    kDebug() << port;
    Q_ASSERT(!m_server);
    m_server = new QTcpServer(this);
    if(m_server->listen(QHostAddress::Any, port)) {
        connect(m_server, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
        kDebug() << "listening on" << port;
        m_port = port;
    } else {
        kWarning() << "Error" << m_server->errorString();
        delete m_server;
        m_server = 0;
        return false;
    }

    return m_server->isListening();
}

void Server::incomingConnection()
{
    kDebug();
    
    QTcpSocket* client = m_server->nextPendingConnection();

    Connection* connection = new Connection(client, this);

    DebugSession* session = new DebugSession(connection);
    connect(session, SIGNAL(outputLine(QString,KDevelop::IRunProvider::OutputTypes)), SLOT(outputLine(QString,KDevelop::IRunProvider::OutputTypes)));
    connect(session, SIGNAL(initDone(QString)), SLOT(initDone(QString)));
    connect(session, SIGNAL(stateChanged(KDevelop::IDebugSession::DebuggerState)), SLOT(stateChanged(KDevelop::IDebugSession::DebuggerState)));

    m_lastSession = session;
    emit sessionStarted(session);
}

void Server::outputLine(QString line, KDevelop::IRunProvider::OutputTypes type)
{
    Q_ASSERT(dynamic_cast<DebugSession*>(sender()));
    DebugSession* session = static_cast<DebugSession*>(sender());
    emit outputLine(session, line, type);
}

void Server::initDone(const QString& ideKey)
{
    Q_ASSERT(dynamic_cast<DebugSession*>(sender()));
    DebugSession* session = static_cast<DebugSession*>(sender());
    m_sessions[ideKey] = session;
    if (m_processes.contains(ideKey)) {
        session->setProcess(m_processes[ideKey]);
    }
}

void Server::stateChanged(KDevelop::IDebugSession::DebuggerState state)
{
    Q_ASSERT(dynamic_cast<DebugSession*>(sender()));
    DebugSession* session = static_cast<DebugSession*>(sender());
    emit stateChanged(session, state);
    if (state == KDevelop::IDebugSession::StoppedState) {
        QString i = m_sessions.key(session);
        m_sessions.remove(i);
        if (m_processes.contains(i)) {
            m_processes[i]->kill();
        }
        session->deleteLater();
        if (m_lastSession == session) {
            m_lastSession = 0;
        }
    }
}


KProcess* Server::startDebugger(const QString& filename)
{
    Q_ASSERT(m_server);
    Q_ASSERT(m_server->isListening());

    kDebug() << filename << m_port;

    QString ideKey = "kdev"+QString::number(qrand());
    
    QStringList e = QProcess::systemEnvironment();
    e.append("XDEBUG_CONFIG=\"remote_enable=1 \"");

    KProcess* process = new KProcess(this);
    process->setEnvironment(e);    
    connect(process, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
    connect(process, SIGNAL(finished(int)), this, SLOT(processFinished(int)));

    *process << "php";
    *process << "-d xdebug.remote_enable=1";
    *process << "-d xdebug.remote_port="+QString::number(m_port);
    *process << "-d xdebug.idekey="+ideKey;
    *process << filename;
    process->setOutputChannelMode(KProcess::SeparateChannels);
    process->start();
    m_processes[ideKey] = process;
    return process;
}

void Server::processReadyRead()
{
    Q_ASSERT(dynamic_cast<KProcess*>(sender()));
    kDebug() << static_cast<KProcess*>(sender())->readAll();
}

void Server::processFinished(int exitCode)
{
    Q_UNUSED(exitCode); //TODO: use it :D
    Q_ASSERT(dynamic_cast<KProcess*>(sender()));
    KProcess* process = static_cast<KProcess*>(sender());
    process->deleteLater();
}

DebugSession* Server::lastSession()
{
    return m_lastSession;
}

bool Server::waitForConnected(int msecs)
{
    QTime stopWatch;
    stopWatch.start();
    if (!m_lastSession) {
        if (!m_server->waitForNewConnection(msecs)) return false;
    }
    if (msecs != -1) msecs = msecs - stopWatch.elapsed();
    return m_lastSession->waitForConnected(msecs);
}

}
#include "server.moc"
