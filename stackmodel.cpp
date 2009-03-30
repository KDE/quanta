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
#include "debugsession.h"
#include "connection.h"
#include <KDebug>

namespace XDebug {

class StackItem : public KDevelop::TreeItem {
public:
    StackItem(KDevelop::TreeModel* model, TreeItem *parent = 0)
        : KDevelop::TreeItem(model, parent)
    {}

    virtual void fetchMoreChildren() {}
    
public:
    using KDevelop::TreeItem::setData;
    using KDevelop::TreeItem::appendChild;
    using KDevelop::TreeItem::clear;
};

StackModel::StackModel(Connection* connection)
    : KDevelop::StackModel((QVector<QString>() << i18n("Depth") << i18n("Where") << i18n("Source")), connection),
      m_connection(connection)
{
    StackItem* rootItem = new StackItem(this);
    setRootItem(rootItem);
    m_rootItem = rootItem;
    
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
    m_rootItem->clear();
}

void StackModel::appendItem(int depth, const QString& function, const QString& file, int line)
{
    StackItem* i = new StackItem(this, m_rootItem);
    i->setData(QVector<QString>() << QString::number(depth) << function << file + ":" + QString::number(line));
    m_rootItem->appendChild(i);    
}

void StackModel::update()
{
    kDebug();
    m_connection->sendCommand("stack_get -i 123");
}


}

#include "stackmodel.moc"
