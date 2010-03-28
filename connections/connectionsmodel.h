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

#ifndef SQL_CONNECTIONSMODEL_H
#define SQL_CONNECTIONSMODEL_H

#include <QModelIndex>

namespace KDevelop {
    class IProject;
}

namespace Sql {

class ConnectionsModel : public QAbstractListModel
{
public:
    ConnectionsModel(KDevelop::IProject *project, QObject *parent);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    struct Connection {
        QString hostName;
        QString databaseName;
        QString userName;
        QString password;
    };

    Connection connection(int row) const;
    void setConnection(int row, Connection connection);

public slots:
    virtual void revert();
    virtual bool submit();

private:
    KDevelop::IProject* project() const;

    KDevelop::IProject *m_project;
    QList<Connection> m_connections;
};

}

#endif // SQL_CONNECTIONSMODEL_H
