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

#ifndef XMLCATALOGMODEL_H
#define XMLCATALOGMODEL_H

#include <QtCore/QAbstractItemModel>

#include <xmlcatalog/icatalog.h>

class XmlCatalogModelNode {
public:
    virtual ~XmlCatalogModelNode();
    virtual QString name() const = 0;
    virtual QString info() const = 0;
    virtual int childCount() const {return 0;}
    virtual XmlCatalogModelNode * child(int i) {return 0;}
    virtual void removeChild(XmlCatalogModelNode * child) {return;}
    virtual XmlCatalogModelNode * parent() const {return m_parent;}
    virtual int row() const {return m_row;}
    virtual void setRow(int row) {m_row = row;}
    virtual ICatalogEntry * entry() const {return 0;}
    virtual ICatalog * catalog() const {return 0;}
protected:
    explicit XmlCatalogModelNode(XmlCatalogModelNode *parent, int m_row);
    XmlCatalogModelNode *m_parent;
    int m_row;
};

class XmlCatalogModel : public QAbstractItemModel
{
  public:
    XmlCatalogModel();
    virtual ~XmlCatalogModel();
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const {return 2;}
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& child) const;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    const QIcon & getIcon ( const QString &fileNamePart ) const;
  private:
    XmlCatalogModelNode *m_root;
};

#endif // XMLCATALOGMODEL_H
