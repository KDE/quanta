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


#include "resulttablewidget.h"

#include <qtextdocument.h>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QTimer>

#include <KDebug>
#include <KComponentData>
#include "kcomponentdata.h"
#include <KSettings/Dispatcher>

#include <interfaces/icore.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/iproject.h>

#include "connections/connectionsmodel.h"
#include "ui_results.h"


namespace Sql {

class ConnectionsAllProjectsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    ConnectionsAllProjectsModel(QObject *parent) : QAbstractListModel(parent)
    {
        connect(KDevelop::ICore::self()->projectController(), SIGNAL(projectOpened(KDevelop::IProject*)),
                SLOT(projectOpened(KDevelop::IProject*)));
        connect(KDevelop::ICore::self()->projectController(), SIGNAL(projectClosed(KDevelop::IProject*)),
                SLOT(projectClosed(KDevelop::IProject*)));

        foreach (KDevelop::IProject *p, KDevelop::ICore::self()->projectController()->projects())
            projectOpened(p);

        KSettings::Dispatcher::registerComponent(KComponentData("kdevplatformproject"), this, "reloadModels");
    }

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const
    {
        if (parent.isValid()) return 0;
        int ret = 0;
        foreach (ConnectionsModel *m, m_models) {
            ret += m->rowCount()-1;
        }
        return ret;
    }

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const
    {
        if (index.parent().isValid()) return QVariant();
        if (index.column() > 0) return QVariant();
        if (role != Qt::DisplayRole) return QVariant();
        int i = 0;
        QHashIterator<KDevelop::IProject*, ConnectionsModel*> it(m_models);
        while (it.hasNext()) {
            it.next();
            ConnectionsModel *m = it.value();
            KDevelop::IProject *p = it.key();
            if (index.row() < i + m->rowCount()-1) {
                return p->name() + ": " + m->data(m->index(index.row()-i, 0), role).toString();
            }
            i += m->rowCount()-1;
        }
        Q_ASSERT(0);
        return QVariant();
    }

    ConnectionsModel::Connection connection(int row)
    {
        int i = 0;
        foreach (ConnectionsModel *m, m_models) {
            if (row < i + m->rowCount()-1) {
                return m->connection(row - i);
            }
            i += m->rowCount()-1;
        }
        Q_ASSERT(0);
        return ConnectionsModel::Connection();
    }

private slots:
    void projectOpened(KDevelop::IProject* project)
    {
        m_models[project] = new ConnectionsModel(project, this);
        connect(m_models[project], SIGNAL(modelReset()), SLOT(childModelReset()));
        reset();
    }

    void childModelReset()
    {
        reset();
    }

    void projectClosed(KDevelop::IProject* project)
    {
        delete m_models[project];
        m_models.remove(project);
        reset();
    }

    void reloadModels()
    {
        foreach (ConnectionsModel *m, m_models) {
            m->revert();
        }
    }

private:
    QHash<KDevelop::IProject*, ConnectionsModel*> m_models;
};

ResultTableWidget::ResultTableWidget(QWidget* parent)
    : QWidget(parent)
{
    m_ui = new Ui::Results;
    m_ui->setupUi(this);
    m_model =  new QSqlQueryModel(this);
    m_ui->table->setModel(m_model);
    m_connectionsModel = new ConnectionsAllProjectsModel(this);
    m_ui->connection->setModel(m_connectionsModel);
    connect(m_ui->connection, SIGNAL(currentIndexChanged(int)), SLOT(currentConnectionChanged(int)));
    connect(m_connectionsModel, SIGNAL(modelReset()), SLOT(connectionChanged()));
    connect(m_connectionsModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(connectionChanged()));
}

ResultTableWidget::~ResultTableWidget()
{
    delete m_ui;
}

void ResultTableWidget::currentConnectionChanged(int index)
{
    if (QSqlDatabase::contains("kdevsql")) {
        if (m_db.isOpen()) m_db.close();
        m_db = QSqlDatabase();
        QSqlDatabase::removeDatabase("kdevsql");
    }
    if (index != -1) {
        ConnectionsModel::Connection c = m_connectionsModel->connection(index);
        m_db = QSqlDatabase::addDatabase(c.driver, "kdevsql");
        m_db.setHostName(c.hostName);
        m_db.setUserName(c.userName);
        m_db.setPassword(c.password);
        m_db.setDatabaseName(c.databaseName);
        if (!m_db.open()) {
            m_ui->errorLabel->setText(Qt::escape(m_db.lastError().text()));
        } else {
            m_ui->errorLabel->setText("");
        }
    } else {
        if (!m_connectionsModel->rowCount()) {
            m_ui->errorLabel->setText(i18n("No Database Connection avaliable. Open Project Configuration for to create one."));
        } else {
            m_ui->errorLabel->setText(i18n("No Database Connection selected."));
        }
    }
    m_ui->stackedWidget->setCurrentWidget(m_ui->errorPage);
}

void ResultTableWidget::connectionChanged()
{
    currentConnectionChanged(m_ui->connection->currentIndex());
}


void ResultTableWidget::runSql(QString sql)
{
    if (!m_db.isOpen()) {
        currentConnectionChanged(m_ui->connection->currentIndex());
    }
    if (m_db.isOpen()) {
        QSqlQuery query(sql, m_db);
        int elapsed;
        {
          QTime t;
          t.start();
          query.exec();
          elapsed = t.elapsed();
        }
        m_model->setQuery(query);
        if (m_model->lastError().isValid()) {
            m_ui->errorLabel->setText(Qt::escape(m_model->lastError().text()));
            m_ui->stackedWidget->setCurrentWidget(m_ui->errorPage);
        } else {
            m_ui->durationLabel->setText(i18n("Query finished in %1 ms", QString::number(elapsed)));
            m_ui->stackedWidget->setCurrentWidget(m_ui->resultsPage);
        }
    }
}


}

#include "resulttablewidget.moc"
#include "moc_resulttablewidget.cpp"
