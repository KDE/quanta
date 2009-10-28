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

#include "launchconfigurationpage.h"

#include "ui_pathmapping.h"
#include "launchconfig.h"
#include "debugsession.h"

namespace XDebug
{

class PathMappingModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const
    {
        if (parent.isValid()) return 0;
        return 2;
    }

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const
    {
        if (parent.isValid()) return 0;
        return m_paths.count() + 1;
    }

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            if (section == 0) {
                return i18n("Remote Path");
            } else if (section == 1) {
                return i18n("Local Path");
            }
        }
        return QAbstractTableModel::headerData(section, orientation, role);
    }

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const
    {
        if (!index.isValid()) return QVariant();
        if (index.parent().isValid()) return QVariant();
        if (index.column() > 1) return QVariant();
        if (index.row() > m_paths.count()) return QVariant();
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            if (index.row() == m_paths.count()) return QString();
            if (index.column() == 0) {
                return m_paths[index.row()].remote;
            } else if (index.column() == 1) {
                return m_paths[index.row()].local.pathOrUrl();
            }
        }
        return QVariant();
    }

    virtual Qt::ItemFlags flags(const QModelIndex& index) const
    {
        if (index.parent().isValid()) return Qt::NoItemFlags;
        if (!index.isValid()) return Qt::NoItemFlags;
        return ( Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled );

    }

    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole)
    {
        if (!index.isValid()) return false;
        if (index.parent().isValid()) return false;
        if (index.column() > 1) return false;
        if (index.row() > m_paths.count()) return false;
        if (role == Qt::EditRole) {
            if (index.row() == m_paths.count()) {
                beginInsertRows(QModelIndex(), index.row()+1, index.row()+1);
                m_paths.append(Path());
                endInsertRows();
            }
            if (index.column() == 0) {
                m_paths[index.row()].remote = value.toString();
            } else if (index.column() == 1) {
                m_paths[index.row()].local = KUrl(value.toString());
            }
            dataChanged(index, index);
            return true;
        }
        return false;
    }

    void loadFromConfiguration(const KConfigGroup &config)
    {
        m_paths.clear();
        KConfigGroup cfg = config.group(DebugSession::pathMappingsEntry);
        foreach (const QString &group, cfg.groupList()) {
            KConfigGroup pCfg = cfg.group(group);
            Path p;
            p.remote = pCfg.readEntry(DebugSession::pathMappingRemoteEntry, KUrl());
            p.local = pCfg.readEntry(DebugSession::pathMappingLocalEntry, KUrl());
            m_paths << p;
        }
        reset();
    }

    void saveToConfiguration(KConfigGroup config)
    {
        KConfigGroup cfg = config.group(DebugSession::pathMappingsEntry);
        foreach (const QString &group, cfg.groupList()) {
            bool ok;
            if (group.toInt(&ok) >= m_paths.count() || !ok) {
                cfg.deleteGroup(group);
            }
        }
        int i=0;
        foreach (const Path &p, m_paths) {
            i++;
            KConfigGroup pCfg = cfg.group(QString::number(i));
            pCfg.writeEntry(DebugSession::pathMappingRemoteEntry, p.remote);
            pCfg.writeEntry(DebugSession::pathMappingLocalEntry, p.local);
        }
        cfg.sync();
    }

private:
    struct Path {
        KUrl remote;
        KUrl local;
    };
    QList<Path> m_paths;
};

KDevelop::LaunchConfigurationPage* ConfigPageFactory::createWidget( QWidget* parent )
{
    return new ConfigPage( parent );
}

ConfigPage::ConfigPage( QWidget* parent )
    : LaunchConfigurationPage(parent), ui( new Ui::PathMappingWidget )
{
    ui->setupUi( this );
    ui->pathMappingTable->setModel(new PathMappingModel());
    connect(ui->pathMappingTable->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SIGNAL(changed()));
}

KIcon ConfigPage::icon() const
{
    return KIcon();
}

void ConfigPage::loadFromConfiguration( const KConfigGroup& cfg, KDevelop::IProject*  )
{
    static_cast<PathMappingModel*>(ui->pathMappingTable->model())->loadFromConfiguration(cfg);
}

void ConfigPage::saveToConfiguration( KConfigGroup cfg, KDevelop::IProject* ) const
{
    static_cast<PathMappingModel*>(ui->pathMappingTable->model())->saveToConfiguration(cfg);
}

QString ConfigPage::title() const
{
    return i18n( "XDebug Configuration" );
}

}

#include "launchconfigurationpage.moc"
#include "moc_launchconfigurationpage.cpp"
