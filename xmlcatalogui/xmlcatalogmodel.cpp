/*****************************************************************************
 * Copyright (c) 2009 Ruan Strydom <rm3dom@gmail.com>                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *****************************************************************************/

#include "xmlcatalogmodel.h"

#include <QtCore/QDir>
#include <QtGui/QIcon>

#include <KDE/KLocalizedString>
#include <KDE/KMessageBox>

#include <xmlcatalog/icatalog.h>
#include <xmlcatalog/icatalogentry.h>
#include <xmlcatalog/icataloggroup.h>
#include <xmlcatalog/icatalogmanager.h>
#include <KDE/KUrl>

XmlCatalogModelNode::XmlCatalogModelNode(XmlCatalogModelNode* parent, int row) : m_parent(parent), m_row(row) {}

XmlCatalogModelNode::~XmlCatalogModelNode() {}

class XmlCatalogModelEntry;
class XmlCatalogModelCatalog;
class XmlCatalogModelRoot;

class XmlCatalogModelEntry : public XmlCatalogModelNode {
public:
    explicit XmlCatalogModelEntry(XmlCatalogModelNode *parent, ICatalogEntry* entry, int row) : XmlCatalogModelNode(parent, row), m_entry(entry) {}

    virtual ~XmlCatalogModelEntry() {}

    QString name() const {
        if (!m_entry) return QString::null;
        QString name = m_entry->publicId();
        if (!name.isNull())
            return name;
        name = m_entry->systemId();
        if (!name.isNull())
            return name;
        name = m_entry->URI();
        if (!name.isNull())
            return name;
        return "Entry [Unknown]";
    }

    QString info() const {
        if (!m_entry) return QString::null;
        QString name = m_entry->publicId();
        if (!name.isNull() && m_entry->catalog()) {
            return m_entry->catalog()->resolvePublicId(name);
        }
        name = m_entry->systemId();
        if (!name.isNull() && m_entry->catalog()) {
            return m_entry->catalog()->resolveSystemId(name);
        }
        name = m_entry->URI();
        if (!name.isNull() && m_entry->catalog()) {
            return m_entry->catalog()->resolveUri(name);
        }
        return m_entry->URL();
    }

    ICatalogEntry* entry() const {
        return m_entry;
    }

    void setEntry( ICatalogEntry* e ) {
        m_entry = e;
    }

protected:
    ICatalogEntry * m_entry;
};

class XmlCatalogModelCatalog : public XmlCatalogModelNode {
public:
    explicit XmlCatalogModelCatalog(XmlCatalogModelNode * parent, ICatalog * catalog, int row) :
            XmlCatalogModelNode(parent, row),
            m_catalog(catalog)
    {
        QList<ICatalogEntry *> entries = m_catalog->entries();
        for(int i=0; i < entries.size(); i++) {
            children.append(new XmlCatalogModelEntry(this, entries[i], i));
        }
    }

    virtual ~XmlCatalogModelCatalog() {
        for ( int i = 0; i < children.size(); i++)
            delete children[i];
    }

    QString name() const {
        QString mode ="";
        if (m_catalog->parameter(ICatalogManager::ParameterReadonly).toBool())
            mode = i18n("[read-only]");
        KUrl url = m_catalog->parameter(ICatalogManager::ParameterFile).toString();
        QString fileName = url.fileName();
        fileName = fileName.mid(0, fileName.lastIndexOf('.'));
        return QString("%1 %2").arg(fileName, mode);
    }

    QString info() const {
        return m_catalog->parameter(ICatalogManager::ParameterFile).toString();
    }

    int childCount() const {
        if (!m_catalog) return 0;
        return m_catalog->entries().size();
    }

    XmlCatalogModelNode * child(int i) {
        if(i > children.size() || i < 0)
            return 0;
        return children.at(i);
    }

    virtual void removeChild(XmlCatalogModelNode* child) {
        delete children.takeAt(child->row());
        for(int i= 0; i < children.size(); i++) {
            children[i]->setRow(i);
        }
    }

    ICatalog* catalog() const {
        return m_catalog;
    }

    void setCatalog( ICatalog* c ) {
        m_catalog = c;
    }
protected:
    ICatalog * m_catalog;
    QList<XmlCatalogModelNode *> children;
};



class XmlCatalogModelRoot : public XmlCatalogModelNode {
public:
    explicit XmlCatalogModelRoot() : XmlCatalogModelNode(0, 0) {}

    virtual ~XmlCatalogModelRoot() {
        QHash<int, XmlCatalogModelNode*>::iterator it;
        for ( it = children.begin(); it != children.end(); ++it )
            delete it.value();
    }

    QString name() const {
        return QString();
    }

    QString info() const {
        return QString();
    }

    int childCount() const {
        return ICatalogManager::self()->catalogs().count();
    }

    XmlCatalogModelNode * parent() const {
        return 0;
    }

    XmlCatalogModelNode * child(int i) {
        if (children.contains(i))
            return children[i];
        XmlCatalogModelCatalog * n = new XmlCatalogModelCatalog(this, ICatalogManager::self()->catalogs()[i], i);
        children.insert(i,n);
        return n;
    }
protected:
    QHash<int, XmlCatalogModelNode *> children;
};

XmlCatalogModel::XmlCatalogModel()
{
    m_root = new XmlCatalogModelRoot();
}

XmlCatalogModel::~XmlCatalogModel()
{
    delete m_root;
}

const QIcon & XmlCatalogModel::getIcon ( const QString &fileNamePart ) const
{
    static QHash <QString, QIcon> icons;
    if ( icons.contains ( fileNamePart ) )
        return icons[fileNamePart];
    QIcon i ( QString ( ":/xml/%1.png" ).arg ( fileNamePart ) );
    icons.insert ( fileNamePart, i );
    return icons[fileNamePart];
}

QVariant XmlCatalogModel::data(const QModelIndex& index, int role) const
{

    if ( !index.isValid() )
        return QVariant();

    if ( role != Qt::DecorationRole && role != Qt::DisplayRole ) {
        return QVariant();
    }

    XmlCatalogModelNode *item = static_cast<XmlCatalogModelNode*> ( index.internalPointer() );
    if ( role == Qt::DecorationRole ) {
        if (index.column() != 0)
            return QVariant();
        if (item->entry()) {
            if (!item->entry()->publicId().isEmpty()) return getIcon("publicId");
            if (!item->entry()->systemId().isEmpty()) return getIcon("systemId");
            if (!item->entry()->URI().isEmpty()) return getIcon("uri");
        }
        if (item->catalog())
            return getIcon("catalog");
        return QVariant();
    }

    switch ( index.column() ) {
    case 0:
        return item->name();
    case 1:
        return item->info();
    default:
        return QVariant();
    }
}

Qt::ItemFlags XmlCatalogModel::flags ( const QModelIndex &index ) const {
    if (!m_root)
        return 0;

    if ( !index.isValid() )
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant XmlCatalogModel::headerData ( int section, Qt::Orientation orientation, int role ) const {
    if ( !m_root)
        return QVariant();

    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
        switch ( section ) {
        case 0:
            return "Item";
        case 1:
            return "Info";
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QModelIndex XmlCatalogModel::index ( int row, int column, const QModelIndex &parent ) const {
    if ( !m_root)
        return QModelIndex();

    if ( !hasIndex ( row, column, parent ) )
        return QModelIndex();

    XmlCatalogModelNode *parentItem;

    if ( !parent.isValid() )
        parentItem = m_root;
    else
        parentItem = static_cast<XmlCatalogModelNode*> ( parent.internalPointer() );

    XmlCatalogModelNode *childItem = parentItem->child ( row );
    if ( childItem )
        return createIndex ( row, column, childItem );
    else
        return QModelIndex();
}

QModelIndex XmlCatalogModel::parent ( const QModelIndex &child ) const {
    if ( !m_root)
        return QModelIndex();

    if ( !child.isValid() )
        return QModelIndex();

    XmlCatalogModelNode *childItem = static_cast<XmlCatalogModelNode*> ( child.internalPointer() );
    if(!childItem)
        return QModelIndex();
    XmlCatalogModelNode *parentItem = childItem->parent();

    if ( !parentItem || parentItem == m_root )
        return QModelIndex();

    return createIndex ( parentItem->row(), 0, parentItem );
}

int XmlCatalogModel::rowCount ( const QModelIndex &parent ) const {
    if ( !m_root)
        return 0;

    if ( parent.column() > 0 )
        return 0;

    if ( !parent.isValid() )
        return m_root->childCount();

    return (static_cast<XmlCatalogModelNode*> ( parent.internalPointer() ))->childCount();
}


