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

namespace XDebug {

DebugSession::DebugSession(Connection* connection)
    : KDevelop::IDebugSession(), m_connection(connection), m_process(0)
{
    connect(m_connection, SIGNAL(output(QString,KDevelop::IRunProvider::OutputTypes)), SIGNAL(output(QString,KDevelop::IRunProvider::OutputTypes)));
    connect(m_connection, SIGNAL(outputLine(QString,KDevelop::IRunProvider::OutputTypes)), SIGNAL(outputLine(QString,KDevelop::IRunProvider::OutputTypes)));
    connect(m_connection, SIGNAL(initDone(QString)), SIGNAL(initDone(QString)));
    connect(m_connection, SIGNAL(initDone(QString)), SLOT(slotInitDone(QString)));
    connect(m_connection, SIGNAL(stateChanged(KDevelop::IDebugSession::DebuggerState)), SIGNAL(stateChanged(KDevelop::IDebugSession::DebuggerState)));
    connect(m_connection, SIGNAL(showStepInSource(KUrl,int)), SIGNAL(showStepInSource(KUrl,int)));
}

KDevelop::StackModel* DebugSession::stackModel() const
{
    return m_connection->stackModel();
}

KDevelop::IBreakpointController* DebugSession::breakpointController() const
{
    return 0;
}

DebugSession::DebuggerState DebugSession::state() const
{
    return m_connection->currentState();
}

void DebugSession::startDebugger()
{
}

void DebugSession::run() {
    m_connection->sendCommand("run -i 123");
}


void DebugSession::toggleBreakpoint() {

}

void DebugSession::stepOut() {
    m_connection->sendCommand("step_out -i 123");
}

void DebugSession::stepOverInstruction() {

}

void DebugSession::stepInto() {
    m_connection->sendCommand("step_into -i 123");
}

void DebugSession::stepIntoInstruction() {

}

void DebugSession::stepOver() {
    m_connection->sendCommand("step_over -i 123");
}

void DebugSession::jumpToCursor() {

}

void DebugSession::runToCursor() {

}

void DebugSession::interruptDebugger() {

}

void DebugSession::stopDebugger() {

}

void DebugSession::restartDebugger() {

}
void DebugSession::eval(QByteArray source) {
    m_connection->sendCommand("eval -i 123", QStringList(), source);
}

bool DebugSession::waitForFinished(int msecs) {
    QTime stopWatch;
    stopWatch.start();
    if (!waitForState(DebugSession::StoppedState, msecs)) return false;
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
    m_connection->sendCommand("stack_get -i 123");
}

bool DebugSession::restartAvaliable() const
{
    //not supported at all by xdebug
    return false;
}

void DebugSession::slotInitDone(const QString& ideKey)
{
    Q_UNUSED(ideKey);
    /*
    int cnt = m_breakpointController->breakpointsItem()->breakpointCount();
    for (int i=0; i<cnt; ++i) {
        KDevelop::IBreakpoint *breakpoint = m_breakpointController->breakpointsItem()->breakpoint(i);
        if (breakpoint->kind() == Breakpoint::CodeBreakpoint) {
            QString location = breakpoint->location();
            if (location.contains(':')) {
                QString cmd = "breakpoint_set -i 123 -t line";
                cmd.append(" -f "+location.left(location.indexOf(':', -2)));
                cmd.append(" -n "+location.mid(location.indexOf(':', -2)+1));
                m_connection->sendCommand(cmd);
            }
        }
    }
    */
}

}

#include "debugsession.moc"
