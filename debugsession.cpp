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

#include "debugsession.h"

#include <QTcpServer>
#include <QTcpSocket>

#include <KDebug>

#include <debugger/util/pathmappings.h>
#include <interfaces/ilaunchconfiguration.h>

#include "connection.h"
#include "breakpointcontroller.h"
#include "variablecontroller.h"
#include "framestackmodel.h"

namespace Crossfire {

DebugSession::DebugSession()
    : m_server(0), m_connection(0), m_currentState(KDevelop::IDebugSession::NotStartedState)
{
    m_breakpointController = new BreakpointController(this);
    m_variableController = new VariableController(this);
}

void DebugSession::setLaunchConfiguration(KDevelop::ILaunchConfiguration* cfg)
{
    m_launchConfiguration = cfg;
}


bool DebugSession::listenForConnection()
{
    Q_ASSERT(!m_server);
    m_server = new QTcpServer(this);
    if(m_server->listen(QHostAddress::Any, 5000)) {
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

    m_connection = new Connection(client, this);
    connect(m_connection, SIGNAL(outputLine(QString)), SIGNAL(outputLine(QString)));
    connect(m_connection, SIGNAL(eventReceived(QVariantMap)), SLOT(eventReceived(QVariantMap)));

    m_server->close();
    m_server->deleteLater();
    m_server = 0;
}

void DebugSession::eventReceived(const QVariantMap &event)
{
    kDebug() << event;
    if (event["event"] == "onContextCreated") {
        kDebug() << event["data"].toMap()["href"] << m_startUrl;
        if (event["data"].toMap()["href"] == m_startUrl.url()) {
            m_currentContext = event["context_id"].toString();
            setState(KDevelop::IDebugSession::StartingState);
            setState(KDevelop::IDebugSession::ActiveState);
        }
    }
    if (event["context_id"] == m_currentContext) {
        if (event["event"] == "onBreak") {
            setState(KDevelop::IDebugSession::PausedState);
            KUrl url(event["data"].toMap()["url"].toString());
            emit showStepInSource(url, event["data"].toMap()["line"].toInt()-1);
        }
    }
}

void DebugSession::setStartUrl(const KUrl& url)
{
    m_startUrl = url;
}

Connection* DebugSession::connection() const
{
    return m_connection;
}

void DebugSession::sendCommand(const QString& cmd, const QVariantMap& arguments, CallbackBase* callback)
{
    connection()->sendCommand(cmd, m_currentContext, arguments, callback);
}


void DebugSession::setState(DebugSession::DebuggerState state)
{
    kDebug() << state;
    if (m_currentState != state) {
        m_currentState = state;
        if (state == DebugSession::StoppedState) {
            //close();
        }
    }
    emit stateChanged(state);
}

KDevelop::IDebugSession::DebuggerState DebugSession::state() const
{
    return m_currentState;
}

KUrl DebugSession::convertToLocalUrl(const KUrl& remoteUrl) const
{
    Q_ASSERT(m_launchConfiguration);
    return KDevelop::PathMappings::convertToLocalUrl(m_launchConfiguration->config(), remoteUrl);
}

KUrl DebugSession::convertToRemoteUrl(const KUrl& localUrl) const
{
    Q_ASSERT(m_launchConfiguration);
    return KDevelop::PathMappings::convertToRemoteUrl(m_launchConfiguration->config(), localUrl);
}

KDevelop::IFrameStackModel* DebugSession::createFrameStackModel()
{
    return new FrameStackModel(this);
}

void DebugSession::run()
{
    m_connection->sendCommand("continue", m_currentContext);
}

void DebugSession::stepOut()
{
    QVariantMap args;
    args["stepaction"] = "out";
    m_connection->sendCommand("continue", m_currentContext, args);
}
void DebugSession::stepOverInstruction()
{
    QVariantMap args;
    args["stepaction"] = "next";
    m_connection->sendCommand("continue", m_currentContext, args);
}
void DebugSession::stepInto()
{
    QVariantMap args;
    args["stepaction"] = "in";
    m_connection->sendCommand("continue", m_currentContext, args);
}
void DebugSession::stepIntoInstruction()
{
    QVariantMap args;
    args["stepaction"] = "in";
    m_connection->sendCommand("continue", m_currentContext, args);
}
void DebugSession::stepOver()
{
    QVariantMap args;
    args["stepaction"] = "next";
    m_connection->sendCommand("continue", m_currentContext, args);
}
void DebugSession::jumpToCursor()
{
}
void DebugSession::runToCursor()
{
}
void DebugSession::interruptDebugger()
{
}
void DebugSession::stopDebugger()
{
    m_connection->sendCommand("suspend", m_currentContext);
}

void DebugSession::restartDebugger()
{
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

bool DebugSession::waitForHandshake(int timeout)
{
    waitForConnected(timeout);
    Q_ASSERT(m_connection);
    return m_connection->waitForHandshake(timeout);
}


}

#include "debugsession.moc"
