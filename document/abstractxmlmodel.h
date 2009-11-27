/*
Copyright (C) 2009  Ruan Strydom <ruan@jcell.co.za>

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


#ifndef ABSTRACTXMLMODEL_H
#define ABSTRACTXMLMODEL_H

#include "abstractxmlmodelnode.h"

#include <QtCore/QAbstractItemModel>
#include <QtXml/QDomNode>

#include <interfaces/idocument.h>

class AbstractXmlModel : public QAbstractItemModel {
    Q_OBJECT
public:
    AbstractXmlModel ( QObject * parent = 0 ) : QAbstractItemModel ( parent ) {};
    virtual ~AbstractXmlModel() {};
    //virtual bool parse (const KDevelop::IDocument *document, QString * errorMsg = 0, int * errorLine = 0, int * errorColumn = 0) = 0;
    virtual void update ( const QDomDocument & doc ) = 0;
    virtual void clear () = 0;
    virtual AbstractXmlModelNode * getNode ( const QModelIndex &index ) const = 0;
    virtual const QIcon & getIcon ( const QString &fileNamePart ) const;
};

#endif // ABSTRACTXMLMODEL_H
