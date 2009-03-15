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


#include "breakpoints.h"
#include "gdb/ibreakpointcontroller.h"

namespace XDebug {

Breakpoints::Breakpoints(KDevelop::IBreakpointController* model)
    : KDevelop::IBreakpoints(model)
{
}


void Breakpoints::load() {

}

void Breakpoints::createHelperBreakpoint() {

}

KDevelop::INewBreakpoint* Breakpoints::addReadWatchpoint() {

}

KDevelop::INewBreakpoint* Breakpoints::addWatchpoint(const QString& expression) {

}

KDevelop::INewBreakpoint* Breakpoints::addWatchpoint() {

}

KDevelop::INewBreakpoint* Breakpoints::addCodeBreakpoint(const QString& location) {

}

KDevelop::INewBreakpoint* Breakpoints::addCodeBreakpoint() {

}

void Breakpoints::update() {

}

void Breakpoints::fetchMoreChildren() {

}

}
