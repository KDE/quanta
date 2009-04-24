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

#ifndef XDEBUG_STACKMODEL_H
#define XDEBUG_STACKMODEL_H

#include <debugger/interfaces/stackmodel.h>
#include <debugger/interfaces/idebugsession.h>

namespace KDevelop { class StackItem; }

namespace XDebug {
class Connection;
class DebugSession;

class StackModel : public KDevelop::StackModel
{
    Q_OBJECT
public:
    StackModel(Connection* connection);
    void clear();
    void appendItem(int depth, const QString& function, const QString& file, int line);

    virtual void update();
    
private slots:
    void paused();
    void stateChanged(KDevelop::IDebugSession::DebuggerState state);

private:
    KDevelop::FramesModel* m_frames;
    Connection* m_connection;
};

}

#endif // XDEBUG_STACKMODEL_H
