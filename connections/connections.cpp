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

#include "connections.h"

#include <QVBoxLayout>
#include <QTextDocument>
#include <QSqlDatabase>
#include <QSqlError>

#include <kpluginfactory.h>
#include <kpluginloader.h>
#include <kurl.h>
#include <kdebug.h>

#include <interfaces/icore.h>
#include <interfaces/iplugincontroller.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/iproject.h>

#include "ui_connections.h"
#include "connectionsmodel.h"

using namespace Sql;

K_PLUGIN_FACTORY(ConnectionsFactory, registerPlugin<Connections>(); )
K_EXPORT_PLUGIN(ConnectionsFactory("kcm_kdev_sqlconnections"))

Connections::Connections( QWidget *parent, const QVariantList &args )
    : KCModule( ConnectionsFactory::componentData(), parent, args )
{
    KDevelop::IProject* project = 0;
    Q_FOREACH (KDevelop::IProject* p, KDevelop::ICore::self()->projectController()->projects()) {
        if (p->projectFileUrl() == args.at(1).toString()) {
            project = p;
            break;
        }
    }
    Q_ASSERT(project);

    setButtons(Help | Apply);

    QVBoxLayout * l = new QVBoxLayout( this );
    setLayout(l);
    QWidget* w = new QWidget();
    l->addWidget(w);

    m_ui = new Ui::Connections();
    m_ui->setupUi(w);

    m_model = new ConnectionsModel(project, this);
    m_ui->list->setModel(m_model);
    connect(m_model, SIGNAL(dataChanged(QModelIndex, QModelIndex)),
            this, SLOT(changed()));
    connect(m_model, SIGNAL(rowsRemoved(QModelIndex, int, int)),
            this, SLOT(changed()));
    connect(m_ui->list->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(currentRowChanged(QModelIndex)));

    connect(m_ui->hostName, SIGNAL(textEdited(QString)), SLOT(connectionEdited()));
    connect(m_ui->database, SIGNAL(textEdited(QString)), SLOT(connectionEdited()));
    connect(m_ui->userName, SIGNAL(textEdited(QString)), SLOT(connectionEdited()));
    connect(m_ui->password, SIGNAL(textEdited(QString)), SLOT(connectionEdited()));

    m_ui->testResult->setText("");

    load();
}

Connections::~Connections( )
{
    delete m_ui;
}

void Connections::save()
{
    m_model->submit();
}

void Connections::load()
{
    m_model->revert();
    m_ui->list->setCurrentIndex(m_model->index(0, 0));
}

void Connections::currentRowChanged(const QModelIndex& index)
{
    ConnectionsModel::Connection c = m_model->connection(index.row());
    m_ui->hostName->setText(c.hostName);
    m_ui->database->setText(c.databaseName);
    m_ui->userName->setText(c.userName);
    m_ui->password->setText(c.password);
    testConnection();
}

void Connections::connectionEdited()
{
    ConnectionsModel::Connection c;
    c.hostName = m_ui->hostName->text();
    c.databaseName = m_ui->database->text();
    c.userName = m_ui->userName->text();
    c.password = m_ui->password->text();
    m_model->setConnection(m_ui->list->currentIndex().row(), c);

    testConnection(); //TODO buffer
}

void Connections::testConnection()
{
    if (m_ui->list->currentIndex().row() == m_model->rowCount()-1) {
        //new connection
        m_ui->testResult->setText("");
        return;
    }
    {
        ConnectionsModel::Connection c = m_model->connection(m_ui->list->currentIndex().row());
        QSqlDatabase testDb = QSqlDatabase::addDatabase("QMYSQL", "kdevsqltest"); //TODO configurable driver
        testDb.setHostName(c.hostName);
        testDb.setUserName(c.userName);
        testDb.setPassword(c.password);
        testDb.setDatabaseName(c.databaseName);
        if (!testDb.open()) {
            m_ui->testResult->setText("<b>"+i18n("Connect failed:")+"</b><br>"+Qt::escape(testDb.lastError().text()));
        } else {
            m_ui->testResult->setText(i18n("Successfully connected."));
        }
        testDb.close();
    }
    QSqlDatabase::removeDatabase("kdevsqltest");
}


#include "connections.moc"
