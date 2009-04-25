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

#include "stackmodel.h"
#include <klocalizedstring.h>
#include <debugger/util/treeitem.h>
#include <debugger/interfaces/stackitem.h>
#include "debugsession.h"
#include "connection.h"
#include <KDebug>

using namespace KDevelop;

namespace XDebug {

class StackItem : public KDevelop::TreeItem {
public:
    StackItem(KDevelop::StackModel* model, TreeItem *parent = 0)
        : KDevelop::TreeItem(model, parent)
    {
    }

    virtual void fetchMoreChildren() {}
    
public:
    using KDevelop::TreeItem::appendChild;
};

//We do not have multiple threads, make it dumb
struct XDebugThreadItem : public ThreadItem
{
    XDebugThreadItem(KDevelop::StackModel* model) : ThreadItem(model) {}
    virtual void fetchMoreChildren() {}
};


StackModel::StackModel(Connection* connection)
    : KDevelop::StackModel(connection),
      m_connection(connection)
{
    StackItem* r = new StackItem(this);
    setRootItem(r);
    r->appendChild(new XDebugThreadItem(this));
    m_frames = modelForThread(0);
    
    connect(connection, SIGNAL(paused()), SLOT(paused()));
    connect(connection, SIGNAL(stateChanged(KDevelop::IDebugSession::DebuggerState)), SLOT(stateChanged(KDevelop::IDebugSession::DebuggerState)));
    
}

void StackModel::paused()
{
    if (m_autoUpdate) {
        update();
    }
}

void StackModel::stateChanged(KDevelop::IDebugSession::DebuggerState state) 
{
    if (state != KDevelop::IDebugSession::PausedState) {
        clear();
    }
}

void StackModel::clear()
{
    m_frames->removeAll();
}

void StackModel::appendItem(int depth, const QString& function, const QString& file, int line)
{
    FrameItem* i = new FrameItem(m_frames);
    i->setInformation(depth, function, qMakePair<QString, int>(file, line));
    m_frames->addFrame(i);
}

void StackModel::update()
{
    kDebug();
    m_connection->sendCommand("stack_get -i 123");
}


}

#include "stackmodel.moc"
