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

#ifndef SQL_CONNECTIONS_CONNECTIONS_H
#define SQL_CONNECTIONS_CONNECTIONS_H

#include <kcmodule.h>
#include <ksharedconfig.h>
#include <QVariant>

class QModelIndex;
namespace Ui {
    class Connections;
}

namespace Sql {

class ConnectionsModel;

class Connections : public KCModule
{
    Q_OBJECT
public:

    Connections( QWidget *parent, const QVariantList &args = QVariantList() );
    virtual ~Connections();

    void save();
    void load();

private slots:
    void currentRowChanged(const QModelIndex& index);
    void connectionEdited();
    void testConnection();

private:
    Ui::Connections* m_ui;
    ConnectionsModel *m_model;
};

}

#endif
