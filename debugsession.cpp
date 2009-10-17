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

#include "debugsession.h"

#include <QTime>
#include <QTcpSocket>
#include <QTcpServer>

#include <KDebug>
#include <KProcess>

#include "connection.h"
#include "breakpointcontroller.h"
#include "framestackmodel.h"
#include "variablecontroller.h"

namespace XDebug {

DebugSession::DebugSession()
    : KDevelop::IDebugSession(), m_server(0), m_connection(0)
{
    m_breakpointController = new BreakpointController(this);
    m_variableController = new VariableController(this);
}

bool DebugSession::listenForConnection()
{
    Q_ASSERT(!m_server);
    m_server = new QTcpServer(this);
    if(m_server->listen(QHostAddress::Any, 9000)) {
        connect(m_server, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
    } else {
        kWarning() << "Error" << m_server->errorString();
        delete m_server;
        m_server = 0;
        return false;
    }
    return m_server->isListening();
}


void DebugSession::incomingConnection()
{
    QTcpSocket* client = m_server->nextPendingConnection();
    client->setParent(this); //don't delete it when server is deleted

    m_connection = new Connection(client, this);
    connect(m_connection, SIGNAL(output(QString)), SIGNAL(output(QString)));
    connect(m_connection, SIGNAL(outputLine(QString)), SIGNAL(outputLine(QString)));
    connect(m_connection, SIGNAL(initDone(QString)), SIGNAL(initDone(QString)));
    connect(m_connection, SIGNAL(stateChanged(KDevelop::IDebugSession::DebuggerState)), SIGNAL(stateChanged(KDevelop::IDebugSession::DebuggerState)));
    connect(m_connection, SIGNAL(stateChanged(KDevelop::IDebugSession::DebuggerState)), SLOT(_stateChanged(KDevelop::IDebugSession::DebuggerState)));
    connect(m_connection, SIGNAL(showStepInSource(KUrl,int)), SIGNAL(showStepInSource(KUrl,int)));

    m_server->close();
    m_server->deleteLater();
    m_server = 0;
}


void DebugSession::_stateChanged(KDevelop::IDebugSession::DebuggerState state)
{
    if (state == PausedState) {
        raiseEvent(program_state_changed);
    } else if (state == EndedState) {
        raiseEvent(debugger_exited);
        emit finished();
    }
}

DebugSession::DebuggerState DebugSession::state() const
{
    if (!m_connection) return NotStartedState;
    return m_connection->currentState();
}

void DebugSession::startDebugger()
{
}

void DebugSession::run() {
    m_connection->sendCommand("run");
}

void DebugSession::stepOut() {
    m_connection->sendCommand("step_out");
}

void DebugSession::stepOverInstruction() {

}

void DebugSession::stepInto() {
    m_connection->sendCommand("step_into");
}

void DebugSession::stepIntoInstruction() {

}

void DebugSession::stepOver() {
    m_connection->sendCommand("step_over");
}

void DebugSession::jumpToCursor() {

}

void DebugSession::runToCursor() {

}

void DebugSession::interruptDebugger() {

}

void DebugSession::stopDebugger()
{
    if (!m_connection || m_connection->currentState() == DebugSession::StoppedState) return;
    m_connection->sendCommand("stop");
}

void DebugSession::restartDebugger() {

}
void DebugSession::eval(QByteArray source) {
    m_connection->sendCommand("eval", QStringList(), source);
}

bool DebugSession::waitForFinished(int msecs)
{
    QTime stopWatch;
    stopWatch.start();
    if (!waitForState(DebugSession::StoppingState, msecs)) return false;
    if (msecs != -1) msecs = msecs - stopWatch.elapsed();
    return true;
}

bool DebugSession::waitForState(KDevelop::IDebugSession::DebuggerState state, int msecs)
{
    if (m_connection->currentState() == state) return true;
    QTime stopWatch;
    stopWatch.start();
    if (!waitForConnected(msecs)) return false;
    while (m_connection->currentState() != state) {
        if (!m_connection->socket()) break;
        if (!m_connection->socket()->isOpen()) break;
        m_connection->socket()->waitForReadyRead(100);
        if (msecs != -1 && stopWatch.elapsed() > msecs) {
            return false;
        }
    }
    return true;
}

bool DebugSession::waitForConnected(int msecs)
{
    if (!m_connection) {
        Q_ASSERT(m_server);
        if (!m_server->waitForNewConnection(msecs)) return false;
    }
    Q_ASSERT(m_connection);
    Q_ASSERT(m_connection->socket());
    return m_connection->socket()->waitForConnected(msecs);
}


Connection* DebugSession::connection() {
    return m_connection;
}

bool DebugSession::restartAvaliable() const
{
    //not supported at all by xdebug
    return false;
}

KDevelop::IFrameStackModel* DebugSession::createFrameStackModel()
{
    return new FrameStackModel(this);
}

}

#include "debugsession.moc"
