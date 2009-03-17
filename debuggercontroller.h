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

#ifndef DEBUGGERCONTROLLER_H
#define DEBUGGERCONTROLLER_H

#include <QObject>
#include "common.h"
#include "connection.h"

class KProcess;

namespace KDevelop {
    class IBreakpointController;
}

namespace XDebug {

class DebuggerController : public QObject
{
    Q_OBJECT

public:
    DebuggerController(QObject* parent = 0);

    Connection* connection();

    bool startDebugging(const QString& filename);
    bool waitForState(DebuggerState state, int msecs = 30000);
    bool waitForFinished(int msecs = 30000);
    
public Q_SLOTS:
    void run();
    void stepInto();
    void eval(QByteArray source);

private:
    Connection* m_connection;
    KProcess* m_process;
    DebuggerState m_status;

    bool waitForConnected(int msecs = 30000);

private Q_SLOTS:
    void processReadyRead();
    void processFinished(int extCode);
};

}
#endif
