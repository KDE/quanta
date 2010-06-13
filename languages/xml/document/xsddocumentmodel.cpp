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

#include "xsddocumentmodel.h"

#include <QtCore/QHash>
#include <QtGui>
#include <QtXml>

#include <KDE/KMessageBox>
#include <KTextEditor/Document>

#include <interfaces/idocument.h>

class XsdDocumentModel::XsdTreeItem : public AbstractXmlModelNode {
public:
    virtual ~XsdTreeItem() {}
    virtual QString name() = 0;
    virtual QString value() = 0;
    virtual QString attributes() = 0;
    virtual QString icon() = 0;
    virtual bool isDomNode() = 0;
    virtual QDomNode node() = 0;
    virtual void setRow ( int r ) = 0;
};

class XsdDocumentModel::DomGroupItem : public XsdDocumentModel::XsdTreeItem {
public:
    explicit DomGroupItem ( const QString &name, int row, const QString &icon, XsdDocumentModel::XsdTreeItem * parent ) {
        m_row = row;
        m_name = name;
        m_icon = icon;
        m_parent = parent;
    }

    virtual ~DomGroupItem() {
        for ( int i = 0; i < m_children.size(); i++ ) {
            delete m_children[i];
        }
    }

    virtual int childCount() {
        return m_children.size();
    }

    virtual XsdDocumentModel::XsdTreeItem* child ( int i ) {
        if ( i > m_children.size() )
            return 0;
        return m_children[i];
    }

    virtual QString attributes() {
        return QString();
    }

    virtual QString icon() {
        return m_icon;
    }

    virtual bool isDomNode() {
        return false;
    }

    virtual int columnNumber() {
        return 0;
    }

    virtual int lineNumber() {
        return 0;
    }

    virtual QString name() {
        return m_name;
    }

    virtual QDomNode node() {
        return QDomNode();
    }

    virtual AbstractXmlModelNode* parent() {
        return m_parent;
    }

    virtual void setRow ( int r ) {
        m_row = r;
    }

    virtual int row() {
        return m_row;
    }

    virtual QString value() {
        return QString ( "(%1)" ).arg ( childCount() );
    }

    virtual void appendChild ( XsdDocumentModel::XsdTreeItem * item ) {
        m_children.append ( item );
    }

protected:
    QList<XsdDocumentModel::XsdTreeItem *> m_children;
    QString m_name;
    QString m_icon;
    int m_row;
    XsdDocumentModel::XsdTreeItem *m_parent;
};

class XsdDocumentModel::DomItem : public XsdDocumentModel::XsdTreeItem {
public:
    explicit DomItem ( const QDomNode &node, int row, XsdTreeItem *parent ) {
        m_domNode = node;
        m_row = row;
        m_parent = parent;
        m_count = -1;
        update();
    }

    virtual ~DomItem() {
        QHash<int, DomItem*>::iterator it;
        for ( it = m_children.begin(); it != m_children.end(); ++it )
            delete it.value();
    }

    virtual QString name() {
        return m_name;
    }

    virtual QString value() {
        return m_value;
    }

    virtual QString attributes() {
        return m_attributes;
    }

    virtual QString icon() {
        return m_icon;
    }

    virtual bool isDomNode() {
        return true;
    }

    virtual QDomNode node() {
        return m_domNode;
    }

    virtual AbstractXmlModelNode *parent() {
        return m_parent;
    }

    virtual XsdDocumentModel::DomItem *child ( int i ) {
        if ( m_children.contains ( i ) )
            return m_children[i];

        if ( i >= 0 && i < m_domNode.childNodes().count() ) {
            QDomNode childNode = m_domNode.childNodes().item ( i );
            DomItem *childItem = new DomItem ( childNode, i, this );
            m_children[i] = childItem;
            return childItem;
        }
        return 0;
    }

    virtual void setRow ( int r ) {
        m_row = r;
    }

    virtual int row() {
        return m_row;
    }

    virtual int childCount() {
        //return node().childNodes().count();
        if ( m_count < 0 ) {
            QDomNodeList l = m_domNode.childNodes();
            int count = 0;
            for ( int i = 0 ; i < l.size(); i++ ) {
                QDomNode n = l.at ( i );
                if ( !n.isProcessingInstruction() && !n.isComment() ) {
                    m_children.insert ( count, new DomItem ( n, count, this ) );
                    count++;
                }
            }
            m_count = count;
        }
        return m_count;
    }

    virtual int lineNumber() {
        return node().lineNumber() - 1;
    }

    virtual int columnNumber() {
        return node().columnNumber();
    }

private:
    void update() {
        //m_name
        {
            if ( !node().localName().isEmpty() )
                m_name = node().localName();
            else
                m_name = node().nodeName();
        }

        //m_value
        {
            QDomNamedNodeMap attributeMap = node().attributes();
            QDomNode attribute;
            if ( ( {attribute = attributeMap.namedItem ( "name" ); !attribute.isNull();} ) )
                m_value = attribute.nodeValue().split ( "\n" ).join ( " " );
            else if ( ( {attribute = attributeMap.namedItem ( "value" ); !attribute.isNull(); } ) )
                m_value = attribute.nodeValue().split ( "\n" ).join ( " " );
            else if ( ( {attribute = attributeMap.namedItem ( "base" ); !attribute.isNull(); } ) )
                m_value = attribute.nodeValue().split ( "\n" ).join ( " " );
            else if ( ( {attribute = attributeMap.namedItem ( "targetNamespace" ); !attribute.isNull(); } ) )
                m_value = attribute.nodeValue().split ( "\n" ).join ( " " );
            else if ( ( {attribute = attributeMap.namedItem ( "namespace" ); !attribute.isNull(); } ) )
                m_value = attribute.nodeValue().split ( "\n" ).join ( " " );
            else {
                QStringList sl = node().nodeValue().split ( "\n" );
                for(int i = 0; i < sl.count(); i++)
                  sl[i] = sl[i].trimmed();
                QString s = sl.join ( " " );
                if ( s.size() > 64 )
                    s = s.mid ( 0, 62 ) + "..";
                m_value = s;
            }
        }

        //m_icon
        {
            if ( node().isElement() ) {
                if ( node().namespaceURI() == "http://www.w3.org/2001/XMLSchema" || node().namespaceURI() == "http://www.w3.org/2001/XMLSchema-instance" )
                    m_icon = node().localName();
            } else if ( node().isText() )
                m_icon = "text";
            else if ( node().isProcessingInstruction() )
                m_icon = "processing";
        }

        //m_attributes
        {
            QDomNamedNodeMap attributeMap = node().attributes();
            QStringList al;
            for ( int i = 0; i < attributeMap.count(); ++i ) {
                QDomNode attribute = attributeMap.item ( i );
                QString s = QString ( "\"%1\"" ).arg ( attribute.nodeValue().split ( "\n" ).join ( " " ) );
                if ( s.size() > 32 )
                    s = s.mid ( 0, 30 ) + "..";
                al << attribute.nodeName() + "=" + s;
            }
            m_attributes = al.join ( "  " );
        }
    }

    QDomNode m_domNode;
    QHash<int, DomItem*> m_children;
    XsdTreeItem *m_parent;
    int m_row;
    int m_count;
    QString m_name;
    QString m_value;
    QString m_attributes;
    QString m_icon;
};

XsdDocumentModel::XsdDocumentModel ( QObject *parent )
        : AbstractXmlModel ( parent ),
        m_domDocument ( 0 ),
        m_rootItem ( 0 ) {}

XsdDocumentModel::~XsdDocumentModel() {
    if ( m_rootItem )
        delete m_rootItem;
    if ( m_domDocument )
        delete m_domDocument;
}

void XsdDocumentModel::clear() {
    if ( m_rootItem )
        delete m_rootItem;
    if ( m_domDocument )
        delete m_domDocument;
}

void removeComments ( QDomNodeList nl ) {
    for ( int i = 0; i < nl.size(); i++ ) {
        if ( nl.item ( i ).isComment() ) {
            nl.item ( i ).parentNode().removeChild ( nl.item ( i ) );
            i--;
        } else {
            removeComments ( nl.item ( i ).childNodes() );
        }
    }
}

void XsdDocumentModel::update ( const QDomDocument& doc ) {
    if ( m_rootItem ) {
        delete m_rootItem;
        m_rootItem = 0;
    }

    int rootCount=0;

    DomGroupItem *root = new DomGroupItem ( tr ( "Schema" ), 0, "schema" , 0 );

    DomGroupItem *ct = new DomGroupItem ( tr ( "Complex Types" ), rootCount++, "complexTypes", root );
    root->appendChild ( ct );

    DomGroupItem *st = new DomGroupItem ( tr ( "Simple Types" ), rootCount++, "simpleTypes", root );
    root->appendChild ( st );

    DomGroupItem *et = new DomGroupItem ( tr ( "Elements" ), rootCount++, "elements", root );
    root->appendChild ( et );

    DomGroupItem *ag = new DomGroupItem ( tr ( "Attribute Groups" ), rootCount++, "attributeGroups", root );
    root->appendChild ( ag );

    DomGroupItem *gr = new DomGroupItem ( tr ( "Groups" ), rootCount++, "groups", root );
    root->appendChild ( gr );

    QDomNodeList nl = doc.childNodes();
    QDomNode schema;
    for ( int i = 0; i < nl.size(); i++ ) {
        if ( ( nl.at ( i ).namespaceURI() == "http://www.w3.org/2001/XMLSchema" ||
                nl.at ( i ).namespaceURI() == "http://www.w3.org/2001/XMLSchema-instance" ) &&
                nl.at ( i ).localName() == "schema" )
            schema = nl.at ( i );
    }

    root->appendChild ( new DomItem ( schema, rootCount++, root ) );

    nl = schema.childNodes();
    for ( int i = 0; i < nl.count(); i++ ) {
        QDomNode node = nl.at ( i );
        if ( node.namespaceURI() == "http://www.w3.org/2001/XMLSchema" ) {
            if ( node.localName() == "complexType" ) {
                ct->appendChild ( new DomItem ( node, ct->childCount(), ct ) );
            } else if ( node.localName() == "simpleType" ) {
                st->appendChild ( new DomItem ( node, st->childCount(), st ) );
            } else if ( node.localName() == "element" ) {
                et->appendChild ( new DomItem ( node, et->childCount(), et ) );
            } else if ( node.localName() == "attributeGroup" ) {
                ag->appendChild ( new DomItem ( node, ag->childCount(), ag ) );
            } else if ( node.localName() == "group" ) {
                gr->appendChild ( new DomItem ( node, gr->childCount(), gr ) );
            }
        }
    }
    
    m_rootItem = root;
}

int XsdDocumentModel::columnCount ( const QModelIndex &/*parent*/ ) const {
    return 3;
}

QVariant XsdDocumentModel::data ( const QModelIndex &index, int role ) const {
    if ( !m_rootItem && !m_domDocument )
        return QVariant();

    if ( !index.isValid() )
        return QVariant();

    if ( role != Qt::DecorationRole && role != Qt::DisplayRole )
        return QVariant();

    XsdTreeItem *item = static_cast<XsdTreeItem*> ( index.internalPointer() );

    if ( role == Qt::DecorationRole ) {
        switch ( index.column() ) {
            case 0:
                if ( !item->icon().isNull() )
                    return getIcon ( item->icon() );
                return QVariant();
                break;
            default:
                return QVariant();
        }
    }

    switch ( index.column() ) {
        case 0: {
                return item->name();
            }
        case 1: {
                return item->value();
            }
        case 2: {
                return item->attributes();
            }
        default:
            return QVariant();
    }
}

Qt::ItemFlags XsdDocumentModel::flags ( const QModelIndex &index ) const {
    if ( !m_rootItem && !m_domDocument )
        return 0;

    if ( !index.isValid() )
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


AbstractXmlModelNode * XsdDocumentModel::getNode ( const QModelIndex& index ) const {
    if ( !m_rootItem && !m_domDocument )
        return 0;
    XsdTreeItem *item = static_cast<XsdTreeItem*> ( index.internalPointer() );
    return item;
}


QVariant XsdDocumentModel::headerData ( int section, Qt::Orientation orientation,
                                        int role ) const {
    if ( !m_rootItem && !m_domDocument )
        return QVariant();

    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
        switch ( section ) {
            case 0:
                return tr ( "Scema" );
            case 1:
                return "";
            case 2:
                return tr ( "Attributes" );
            default:
                return QVariant();
        }
    }

    return QVariant();
}

QModelIndex XsdDocumentModel::index ( int row, int column, const QModelIndex &parent ) const {
    if ( !m_rootItem && !m_domDocument )
        return QModelIndex();

    if ( !hasIndex ( row, column, parent ) )
        return QModelIndex();

    XsdTreeItem *parentItem;

    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<XsdTreeItem*> ( parent.internalPointer() );

    XsdTreeItem *childItem = static_cast<XsdTreeItem*> ( parentItem->child ( row ) );
    if ( childItem )
        return createIndex ( row, column, childItem );

    return QModelIndex();
}

QModelIndex XsdDocumentModel::parent ( const QModelIndex &child ) const {
    if ( !m_rootItem && !m_domDocument )
        return QModelIndex();

    if ( !child.isValid() )
        return QModelIndex();

    XsdTreeItem *childItem = static_cast<XsdTreeItem*> ( child.internalPointer() );
    XsdTreeItem *parentItem =  static_cast<XsdTreeItem*> ( childItem->parent() );

    if ( !parentItem || parentItem == m_rootItem )
        return QModelIndex();

    return createIndex ( parentItem->row(), 0, parentItem );
}

int XsdDocumentModel::rowCount ( const QModelIndex &parent ) const {
    if ( !m_rootItem && !m_domDocument )
        return 0;

    if ( parent.column() > 0 )
        return 0;

    XsdTreeItem *parentItem;

    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<XsdTreeItem*> ( parent.internalPointer() );

    return parentItem->childCount();
}

#include "xsddocumentmodel.moc"

