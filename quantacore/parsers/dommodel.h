/***************************************************************************
    begin                : Sep 03 2006
    copyright            : (C) 2006 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef DOMMODEL_H
#define DOMMODEL_H

#include <QList>
#include <QAbstractItemModel>

#include "treelement.h"



class DomModelItem
{
  public:
    explicit DomModelItem(TreeElement *node = 0, DomModelItem * parent= 0);
    
    virtual ~DomModelItem();
    
    DomModelItem * child(int row) const;
    
    int childCount() const;
    
    int columnCount() const {return 2;};
    
    QVariant data(int column, int role) const;
    
    int row() const;

    DomModelItem * parentItem() const;
    
    void appendItem(DomModelItem * newChild);
    
    bool equals(DomModelItem item) { return m_node == item.m_node; }
    
    TreeElement *firstChild() { return m_node->firstChild(); }
    
  protected:
    TreeElement *m_node;
    
  private:
    QList<DomModelItem *> m_childItems;
    
    DomModelItem * m_parentItem;
};



class DomModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
    explicit DomModel(TreeElement *node, QObject *parent = 0);
    
    ~DomModel();

    QVariant data(const QModelIndex &index, int role) const;
    
    Qt::ItemFlags flags(const QModelIndex &index) const;
    
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    
    QModelIndex parent(const QModelIndex &index) const;
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

  private:
    void setupModelData(DomModelItem * parent);
    
    DomModelItem m_rootItem;
};

#endif

//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
