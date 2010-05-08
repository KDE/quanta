/*
Copyright (C) 2010  Ruan Strydom <ruan@jcell.co.za>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HTMLDOCUMENTMODEL_H
#define HTMLDOCUMENTMODEL_H

#include <QtCore/QObject>
#include <QtXml/QDomDocument>

#include "abstractxmlmodel.h"

namespace KDevelop {
    class IDocument;
}

class HtmlDocumentModel : public AbstractXmlModel
{
    Q_OBJECT
    
    public:
        HtmlDocumentModel (QObject *parent = 0 );
        ~HtmlDocumentModel();
        //virtual bool parse (const KDevelop::IDocument *document, QString * errorMsg = 0, int * errorLine = 0, int * errorColumn = 0);
        virtual void update(const QDomDocument & doc);
        virtual void clear ();
        virtual QVariant data ( const QModelIndex &index, int role ) const;
        virtual Qt::ItemFlags flags ( const QModelIndex &index ) const;
        virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
        virtual QModelIndex index ( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
        virtual QModelIndex parent ( const QModelIndex &child ) const;
        virtual int rowCount ( const QModelIndex &parent = QModelIndex() ) const;
        virtual int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
        virtual AbstractXmlModelNode * getNode(const QModelIndex &index) const;
        
    private:
        QDomDocument *m_domDocument;
        class DomItem;
        DomItem *m_rootItem;
};


#endif // HTMLDOCUMENTMODEL_H
