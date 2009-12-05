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
#include <KLocalizedString>

#include <debugger/breakpoint/breakpoint.h>

#include "debugsession.h"
#include "connection.h"

namespace Crossfire {

BreakpointController::BreakpointController(DebugSession* parent)
    : IBreakpointController(parent)
{
    connect(debugSession(), SIGNAL(stateChanged(KDevelop::IDebugSession::DebuggerState)),
            SLOT(stateChanged(KDevelop::IDebugSession::DebuggerState)));
}

void BreakpointController::sendMaybe(KDevelop::Breakpoint* breakpoint)
{
    if (breakpoint->deleted()) {
        if (m_ids.contains(breakpoint)) {
            QString cmd("clearbreakpoint");
            /*
            QStringList args;
            args << "-d "+m_ids[breakpoint];
            debugSession()->connection()->sendCommand(cmd, args);
            */
        }
    } else if (m_dirty[breakpoint].contains(KDevelop::Breakpoint::LocationColumn)) {
        if (breakpoint->enabled()) {
            QString cmd = m_ids.contains(breakpoint) ? "changebreakpoint" : "setbreakpoint";
            QVariantMap args;
            kDebug() << "breakpoint kind" << breakpoint->kind();
            if (breakpoint->kind() == KDevelop::Breakpoint::CodeBreakpoint) {
                if (m_ids.contains(breakpoint)) {
                    //args << "-d "+m_ids[breakpoint];
                }
                if (breakpoint->line() != -1) {
                    args["type"] = "line";
                } else {
                    //args << "-t call";
                }
                if (breakpoint->line() != -1) {
                    KUrl url = breakpoint->url();
                    url = debugSession()->convertToRemoteUrl(url);
                    args["target"] = url.url();
                    args["line"] = breakpoint->line()+1;
                } else {
                    //args << "-m "+breakpoint->expression();
                }
            //} else if (breakpoint->kind() == KDevelop::Breakpoint::WriteBreakpoint) {
            //    args << "-t watch";
            //    args << "-m "+breakpoint->expression();
            } else {
                error(breakpoint, i18n("breakpoint type is not supported"), KDevelop::Breakpoint::LocationColumn);
                return;
            }
            if (breakpoint->ignoreHits()) {
                args["ignoreCount"] = breakpoint->ignoreHits();
            }
            //CallbackWithCookie<BreakpointController, KDevelop::Breakpoint>* cb =
            //    new CallbackWithCookie<BreakpointController, KDevelop::Breakpoint>
            //        (this, &BreakpointController::handleSetBreakpoint, breakpoint);
            debugSession()->sendCommand(cmd, args/*, cb*/);
        }
    } else if (m_dirty[breakpoint].contains(KDevelop::Breakpoint::EnableColumn)) {
        Q_ASSERT(m_ids.contains(breakpoint));
        QString cmd = "changebreakpoint";
        QStringList args;
        //args << "-d "+m_ids[breakpoint];
        //args << QString("-s %0").arg(breakpoint->enabled() ? "enabled" : "disabled");
        //debugSession()->connection()->sendCommand(cmd, args);
    }
    m_dirty[breakpoint].clear();
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
    } else if (state == KDevelop::IDebugSession::PausedState) {
        //Callback<BreakpointController>* cb =
        //    new Callback<BreakpointController>(this, &BreakpointController::handleBreakpointList);
        //debugSession()->connection()->sendCommand("breakpoint_list", QStringList(), QByteArray(), cb);
    }
}

}

#include "breakpointcontroller.moc"
