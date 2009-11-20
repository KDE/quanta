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
}

void BreakpointController::sendMaybe(KDevelop::Breakpoint* breakpoint)
{
}

DebugSession* BreakpointController::debugSession()
{
    return static_cast<DebugSession*>(KDevelop::IBreakpointController::debugSession());
}

}

#include "breakpointcontroller.moc"
