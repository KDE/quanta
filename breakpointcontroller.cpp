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

#include "breakpointcontroller.h"

#include <KDebug>

#include <debugger/breakpoint/breakpoint.h>

#include "debugsession.h"
#include "connection.h"
#include <QXmlStreamReader>

namespace XDebug {

BreakpointController::BreakpointController(DebugSession* parent)
    : IBreakpointController(parent)
{
    connect(debugSession(), SIGNAL(initDone(QString)), SLOT(initDone(QString)));
    connect(debugSession(), SIGNAL(stateChanged(KDevelop::IDebugSession::DebuggerState)),
                SLOT(stateChanged(KDevelop::IDebugSession::DebuggerState)));
}

void BreakpointController::sendMaybe(KDevelop::Breakpoint* breakpoint)
{
    if (breakpoint->kind() == KDevelop::Breakpoint::CodeBreakpoint) {
        if (breakpoint->deleted()) {
            if (m_ids.contains(breakpoint)) {
                QString cmd("breakpoint_remove");
                QStringList args;
                args << "-d "+m_ids[breakpoint];
                debugSession()->connection()->sendCommand(cmd, args);
            }
        } else if (m_dirty[breakpoint].contains(KDevelop::Breakpoint::LocationColumn)) {
            if (breakpoint->enabled()) {
                QString cmd = m_ids.contains(breakpoint) ? "breakpoint_update" : "breakpoint_set";
                QStringList args;
                if (m_ids.contains(breakpoint)) {
                    args << "-d "+m_ids[breakpoint];
                } else {
                    args << "-t line";
                }
                args << "-f "+breakpoint->url().url();
                args << "-n "+QString::number(breakpoint->line()+1);
                CallbackWithCookie<BreakpointController, KDevelop::Breakpoint>* cb = 0;
                if (!m_ids.contains(breakpoint)) {
                    cb = new CallbackWithCookie<BreakpointController, KDevelop::Breakpoint>
                            (this, &BreakpointController::handleSetBreakpoint, breakpoint);
                }
                debugSession()->connection()->sendCommand(cmd, args, QByteArray(), cb);
            }
        } else if (m_dirty[breakpoint].contains(KDevelop::Breakpoint::EnableColumn)) {
            Q_ASSERT(m_ids.contains(breakpoint));
            QString cmd = "breakpoint_update";
            QStringList args;
            args << "-d "+m_ids[breakpoint];
            args << QString("-s %0").arg(breakpoint->enabled() ? "enabled" : "disabled");
            debugSession()->connection()->sendCommand(cmd, args);
        }
        m_dirty[breakpoint].clear();
    }
}

void BreakpointController::handleSetBreakpoint(KDevelop::Breakpoint* breakpoint, QXmlStreamReader* xml)
{
    Q_ASSERT(xml->attributes().value("command") == "breakpoint_set");
    Q_ASSERT(breakpoint);
    m_ids[breakpoint] = xml->attributes().value("id").toString();
}


DebugSession* BreakpointController::debugSession()
{
    return static_cast<DebugSession*>(KDevelop::IBreakpointController::debugSession());
}

void BreakpointController::stateChanged(KDevelop::IDebugSession::DebuggerState state)
{
    kDebug() << state;
    if (state == KDevelop::IDebugSession::StartingState) {
        sendMaybeAll();
    }
}



}

#include "breakpointcontroller.moc"
