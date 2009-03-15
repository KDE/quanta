/***************************************************************************
 *   Copyright (C) 2009 by Niko Sams <niko.sams@gmail.com>                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QtNetwork/QTcpSocket>
#include <QtCore/QTime>

#include <KProcess>
#include <KDebug>

#include <interfaces/icore.h>
#include "gdb/ibreakpointcontroller.h"

#include "debuggercontroller.h"
#include "breakpoints.h"
#include "breakpointcontroller.h"
#include <QDate>

namespace XDebug {

DebuggerController::DebuggerController(QObject* parent)
    : QObject(parent), m_status(UnknownState), m_process(0)
{
    m_breakpointController = new BreakpointController(this);
    
    m_connection = new Connection(this);
    connect(m_connection, SIGNAL(showStepInSource(QString, int)), this, SLOT(showStepInSource(QString, int)));
    connect(m_connection, SIGNAL(stateChanged(DebuggerState)), this, SLOT(stateChanged(DebuggerState)));
    m_connection->listen(9000);
}

void DebuggerController::showStepInSource(const QString& fileName, int lineNum)
{
    if (KDevelop::ICore::self()) {
        m_breakpointController->gotoExecutionPoint(fileName, lineNum);
    }
}

void DebuggerController::stateChanged(DebuggerState state)
{
    if (state == StoppedState) {
        if (KDevelop::ICore::self()) {
            m_breakpointController->clearExecutionPoint();
        }
    }
}

bool DebuggerController::startDebugging(const QString& filename)
{
    if (!m_connection->isListening()) {
        return false;
    }
    
    if (m_process) {
        m_process->kill();
    }

    kDebug() << filename;

    QStringList env;
    env << "XDEBUG_CONFIG=\"idekey=testsession\"";

    m_process = new KProcess(this);
    connect(m_process, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
    connect(m_process, SIGNAL(finished(int)), this, SLOT(processFinished(int)));

    QStringList e = QProcess::systemEnvironment();
    e.append(env);
    m_process->setEnvironment(e);
    *m_process << "php";
    *m_process << filename;
    m_process->setOutputChannelMode(KProcess::SeparateChannels);
    m_process->start();

    return true;
}

bool DebuggerController::waitForFinished(int msecs) {
    QTime stopWatch;
    stopWatch.start();
    if (!waitForState(StoppedState, msecs)) return false;
    if (msecs != -1) msecs = msecs - stopWatch.elapsed();
    return m_process->waitForFinished(msecs);
}

bool DebuggerController::waitForConnected(int msecs)
{
    QTime stopWatch;
    stopWatch.start();
    if (!m_connection->currentClient()) {
        if (!m_connection->waitForNewConnection(msecs)) return false;
    }
    if (msecs != -1) msecs = msecs - stopWatch.elapsed();
    return m_connection->currentClient()->waitForConnected(msecs);
}

void DebuggerController::processReadyRead()
{
    kDebug() << m_process->readAll();
}

void DebuggerController::processFinished(int exitCode)
{
    Q_UNUSED(exitCode); //TODO: use it :D
    m_process->deleteLater();
    m_process = 0;
}


void DebuggerController::run() {
    m_connection->sendCommand("run -i 123");
}
void DebuggerController::stepInto() {
    m_connection->sendCommand("step_into -i 123");
}

void DebuggerController::eval(QByteArray source) {
    m_connection->sendCommand("eval -i 123", QStringList(), source);
}

Connection* DebuggerController::connection() {
    return m_connection;
}

bool DebuggerController::waitForState(XDebug::DebuggerState state, int msecs)
{
    QTime stopWatch;
    stopWatch.start();
    if (!waitForConnected(msecs)) return false;
    while (m_connection->currentState() != state) {
        if (!m_connection->currentClient()) break;
        m_connection->currentClient()->waitForReadyRead(100);
        if (msecs != -1 && stopWatch.elapsed() > msecs) {
            return false;
        }
    }
    return true;
}

}
