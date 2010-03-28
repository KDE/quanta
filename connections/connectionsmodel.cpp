/*
    Copyright (C) 2010 Niko Sams <niko.sams@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/


#include "connectionsmodel.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KDebug>

#include <interfaces/iproject.h>
#include <KGlobal>

namespace Sql
{

ConnectionsModel::ConnectionsModel(KDevelop::IProject* project, QObject *parent)
    : QAbstractListModel(parent), m_project(project)
{
    revert();
}

KDevelop::IProject* ConnectionsModel::project() const
{
    return m_project;
}

QVariant ConnectionsModel::data(const QModelIndex& index, int role) const
{
    if (index.parent().isValid()) return QVariant();
    if (index.column() > 1) return QVariant();
    if (m_connections.count()+1 < index.row()) return QVariant();

    if (role != Qt::DisplayRole) return QVariant();
    if (m_connections.count() == index.row()) {
        return i18n("new connection");
    }
    Connection c = m_connections.at(index.row());
    QString ret;
    if (!c.userName.isEmpty()) ret += c.userName + "@";
    ret += c.hostName;
    ret += ": " + c.databaseName;
    return ret;
}

int ConnectionsModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return m_connections.count() + 1;
}

void ConnectionsModel::revert()
{
    m_connections.clear();
    KConfigGroup group = project()->projectConfiguration()->group("DatabaseConnections");
    int count = group.readEntry("Count", 0);
    kDebug() << count;
    for (int i=0; i < count; ++i) {
        Connection c;
        KConfigGroup g = group.group(QString("Connection %1").arg(i));
        c.hostName = g.readEntry("HostName", QString());
        c.databaseName = g.readEntry("DatabaseName", QString());
        c.userName = g.readEntry("UserName", QString());
        c.password = g.readEntry("Password", QString()); //TODO use KWallet
        m_connections << c;
    }
    reset();
}

bool ConnectionsModel::submit()
{
    KConfigGroup group = project()->projectConfiguration()->group("DatabaseConnections");
    kDebug() << m_connections.count();
    group.writeEntry("Count", m_connections.count());
    for (int i=0; i < m_connections.count(); ++i) {
        Connection c = m_connections.at(i);
        KConfigGroup g = group.group(QString("Connection %1").arg(i));
        g.writeEntry("HostName", c.hostName);
        g.writeEntry("DatabaseName", c.databaseName);
        g.writeEntry("UserName", c.userName);
        g.writeEntry("Password", c.password); //TODO use KWallet
    }
    group.sync();
    return true;
}

ConnectionsModel::Connection ConnectionsModel::connection(int row) const
{
    if (row == m_connections.count()) {
        return Connection();
    } else {
        Q_ASSERT(row < m_connections.count());
        return m_connections.at(row);
    }
}

void ConnectionsModel::setConnection(int row, ConnectionsModel::Connection connection)
{
    if (row == m_connections.count()) {
        beginInsertRows(QModelIndex(), row+1, row+1);
        m_connections.append(connection);
        dataChanged(createIndex(row, 0), createIndex(row, 0));
        endInsertRows();
    } else {
        Q_ASSERT(row < m_connections.count());
        m_connections[row] = connection;
        dataChanged(createIndex(row, 0), createIndex(row, 0));
    }
}


}
