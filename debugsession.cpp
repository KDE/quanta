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

#include <KDebug>
#include <KProcess>

#include "connection.h"
#include "stackmodel.h"
#include "breakpointcontroller.h"

namespace XDebug {

DebugSession::DebugSession(Connection* connection)
    : KDevelop::IDebugSession(), m_connection(connection), m_process(0)
{
    new BreakpointController(this);
    connect(m_connection, SIGNAL(output(QString,KDevelop::IRunProvider::OutputTypes)), SIGNAL(output(QString,KDevelop::IRunProvider::OutputTypes)));
    connect(m_connection, SIGNAL(outputLine(QString,KDevelop::IRunProvider::OutputTypes)), SIGNAL(outputLine(QString,KDevelop::IRunProvider::OutputTypes)));
    connect(m_connection, SIGNAL(initDone(QString)), SIGNAL(initDone(QString)));
    connect(m_connection, SIGNAL(stateChanged(KDevelop::IDebugSession::DebuggerState)), SIGNAL(stateChanged(KDevelop::IDebugSession::DebuggerState)));
    connect(m_connection, SIGNAL(showStepInSource(KUrl,int)), SIGNAL(showStepInSource(KUrl,int)));
}

KDevelop::StackModel* DebugSession::stackModel() const
{
    return m_connection->stackModel();
}

DebugSession::DebuggerState DebugSession::state() const
{
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

void DebugSession::stopDebugger() {
    m_connection->sendCommand("stop");
}

void DebugSession::restartDebugger() {

}
void DebugSession::eval(QByteArray source) {
    m_connection->sendCommand("eval", QStringList(), source);
}

bool DebugSession::waitForFinished(int msecs) {
    QTime stopWatch;
    stopWatch.start();
    if (!waitForState(DebugSession::StoppingState, msecs)) return false;
    if (msecs != -1) msecs = msecs - stopWatch.elapsed();
    if (m_process) {
        return m_process->waitForFinished(msecs);
    }
    return true;
}

bool DebugSession::waitForState(KDevelop::IDebugSession::DebuggerState state, int msecs)
{
    if (m_connection->currentState() == state) return true;
    QTime stopWatch;
    stopWatch.start();
    if (!waitForConnected(msecs)) return false;
    while (m_connection->currentState() != state) {
        if (!m_connection->socket()->isOpen()) break;
        m_connection->socket()->waitForReadyRead(100);
        if (msecs != -1 && stopWatch.elapsed() > msecs) {
            return false;
        }
    }
    return true;
}

bool DebugSession::waitForConnected(int msecs) {
    Q_ASSERT(m_connection);
    Q_ASSERT(m_connection->socket());
    return m_connection->socket()->waitForConnected(msecs);
}


Connection* DebugSession::connection() {
    return m_connection;
}
KProcess* DebugSession::process() {
    return m_process;
}

void DebugSession::setProcess(KProcess* process) {
    m_process = process;
}

void DebugSession::stackGet() {
    m_connection->sendCommand("stack_get");
}

bool DebugSession::restartAvaliable() const
{
    //not supported at all by xdebug
    return false;
}

}

#include "debugsession.moc"
