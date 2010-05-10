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

#include "xmldocumentmodel.h"

#include <QtGui>
#include <QtXml>

#include <QDomNode>
#include <QHash>

#include <KTextEditor/Document>

#include <interfaces/idocument.h>



class XmlDocumentModel::DomItem : public AbstractXmlModelNode {
public:
    DomItem ( const QDomNode &node, int row, DomItem *parent = 0 ) {
        domNode = node;
        // Record the item's location within its parent.
        rowNumber = row;
        parentItem = parent;
    }

    virtual ~DomItem() {
        QHash<int, DomItem*>::iterator it;
        for ( it = childItems.begin(); it != childItems.end(); ++it )
            delete it.value();
    }

    virtual QDomNode node() {
        return domNode;
    }

    virtual DomItem *parent() {
        return parentItem;
    }

    virtual DomItem *child ( int i ) {
        if ( childItems.contains ( i ) )
            return childItems[i];

        if ( i >= 0 && i < domNode.childNodes().count() ) {
            QDomNode childNode = domNode.childNodes().item ( i );
            DomItem *childItem = new DomItem ( childNode, i, this );
            childItems[i] = childItem;
            return childItem;
        }
        return 0;
    }

    virtual int row() {
        return rowNumber;
    }

    virtual int childCount() {
        return node().childNodes().count();
    }

    virtual int lineNumber() {
        return node().lineNumber() - 1;
    }

    virtual int columnNumber() {
        return node().lineNumber();
    }
private:
    QDomNode domNode;
    QHash<int, DomItem*> childItems;
    DomItem *parentItem;
    int rowNumber;
};

XmlDocumentModel::XmlDocumentModel ( QObject *parent )
        : AbstractXmlModel ( parent ),
        m_domDocument ( 0 ),
        m_rootItem ( 0 ) {
}

XmlDocumentModel::~XmlDocumentModel() {
    if ( m_rootItem )
        delete m_rootItem;
    if ( m_domDocument )
        delete m_domDocument;

}

void XmlDocumentModel::clear() {
    if ( m_rootItem )
        delete m_rootItem;
    if ( m_domDocument )
        delete m_domDocument;
}

void XmlDocumentModel::update ( const QDomDocument& doc ) {
    m_rootItem = new DomItem ( doc, 0 );
}


int XmlDocumentModel::columnCount ( const QModelIndex &/*parent*/ ) const {
    return 3;
}

QVariant XmlDocumentModel::data ( const QModelIndex &index, int role ) const {

    if ( !m_rootItem && !m_domDocument )
        return QVariant();

    if ( !index.isValid() )
        return QVariant();

    if ( role != Qt::DecorationRole && role != Qt::DisplayRole )
        return QVariant();

    DomItem *item = static_cast<DomItem*> ( index.internalPointer() );
    QDomNode node = item->node();
    QStringList attributes;
    QDomNamedNodeMap attributeMap = node.attributes();


    if ( role == Qt::DecorationRole ) {
        switch ( index.column() ) {
            case 0:
                if ( node.isElement() )
                    return getIcon ( "element" );
                if ( node.isText() )
                    return getIcon ( "text" );
                if ( node.isProcessingInstruction() )
                    return getIcon ( "processing" );
                return QVariant();
                break;
            default:
                return QVariant();
        }
    }

    switch ( index.column() ) {
        case 0:
            return node.nodeName();
        case 1:
            for ( int i = 0; i < attributeMap.count(); ++i ) {
                QDomNode attribute = attributeMap.item ( i );
                QString s = QString ( "\"%1\"" ).arg ( attribute.nodeValue().split ( "\n" ).join ( " " ) );
                if ( s.size() > 32 )
                    s = s.mid ( 0, 30 ) + "..";
                attributes << attribute.nodeName() + "=" + s;
            }
            return attributes.join ( "  " );
        case 2: {
                QStringList sl = node.nodeValue().split ( "\n" );
                for(int i = 0; i < sl.count(); i++)
                  sl[i] = sl[i].trimmed();
                QString s = sl.join ( " " );
                if ( s.size() > 64 )
                    s = s.mid ( 0, 62 ) + "..";
                return s;
            }
        default:
            return QVariant();
    }
}

Qt::ItemFlags XmlDocumentModel::flags ( const QModelIndex &index ) const {
    if ( !m_rootItem && !m_domDocument )
        return 0;

    if ( !index.isValid() )
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


AbstractXmlModelNode * XmlDocumentModel::getNode ( const QModelIndex& index ) const {
    if ( !m_rootItem && !m_domDocument )
        return 0;
    DomItem *item = static_cast<DomItem*> ( index.internalPointer() );
    return item;
}


QVariant XmlDocumentModel::headerData ( int section, Qt::Orientation orientation,
                                        int role ) const {
    if ( !m_rootItem && !m_domDocument )
        return QVariant();

    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
        switch ( section ) {
            case 0:
                return tr ( "Name" );
            case 1:
                return tr ( "Attributes" );
            case 2:
                return tr ( "Value" );
            default:
                return QVariant();
        }
    }

    return QVariant();
}

QModelIndex XmlDocumentModel::index ( int row, int column, const QModelIndex &parent ) const {
    if ( !m_rootItem && !m_domDocument )
        return QModelIndex();

    if ( !hasIndex ( row, column, parent ) )
        return QModelIndex();

    DomItem *parentItem;

    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<DomItem*> ( parent.internalPointer() );

    DomItem *childItem = parentItem->child ( row );
    if ( childItem )
        return createIndex ( row, column, childItem );
    else
        return QModelIndex();
}

QModelIndex XmlDocumentModel::parent ( const QModelIndex &child ) const {
    if ( !m_rootItem && !m_domDocument )
        return QModelIndex();

    if ( !child.isValid() )
        return QModelIndex();

    DomItem *childItem = static_cast<DomItem*> ( child.internalPointer() );
    DomItem *parentItem = childItem->parent();

    if ( !parentItem || parentItem == m_rootItem )
        return QModelIndex();

    return createIndex ( parentItem->row(), 0, parentItem );
}

int XmlDocumentModel::rowCount ( const QModelIndex &parent ) const {
    if ( !m_rootItem && !m_domDocument )
        return 0;

    if ( parent.column() > 0 )
        return 0;

    DomItem *parentItem;

    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<DomItem*> ( parent.internalPointer() );

    return parentItem->node().childNodes().count();
}

#include "xmldocumentmodel.moc"

