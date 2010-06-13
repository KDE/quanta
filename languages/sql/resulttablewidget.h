/*
   Copyright (C) 2010 Niko Sams <niko.sams@gmail.com>

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

#ifndef SQL_RESULTTABLEWIDGET_H
#define SQL_RESULTTABLEWIDGET_H

#include <QWidget>
#include <QSqlDatabase>

class QSqlQueryModel;
namespace Ui {
    class Results;
}


class QTableView;

namespace Sql {

class ConnectionsAllProjectsModel;

class ResultTableWidget : public QWidget
{
    Q_OBJECT
public:
    ResultTableWidget(QWidget* parent = 0);
    ~ResultTableWidget();

    void runSql(QString sql);

private slots:
    void currentConnectionChanged(int index);
    void connectionChanged();

private:
    Ui::Results *m_ui;
    QSqlQueryModel *m_model;
    QSqlDatabase m_db;
    ConnectionsAllProjectsModel* m_connectionsModel;
};

}

#endif
