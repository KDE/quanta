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
#include <QXmlAttributes>
#include <QVector>

#include "quantaexport.h"

namespace KTextEditor {
  class Range;
}

class KDEVQUANTA_EXPORT DomModelItem
{
  public:
    enum Type {
      TagStart = 0,
      TagEnd,
      Text,
      Comment,
      DocType,
      Empty
    };
    
    explicit DomModelItem(DomModelItem * parent= 0);
    
    virtual ~DomModelItem();
    
    DomModelItem * child(int row) const;
    
    int childCount() const;
    
    int columnCount() const {return 1;};
    
    QVariant data(int column, int role) const;
    
    int row() const;

    DomModelItem * parent() const;
    
    void appendItem(DomModelItem * newChild);
    
    void setRange(KTextEditor::Range *range);
    KTextEditor::Range *range() { return m_range; }
    void setType(Type type) { m_type = type; }
    Type type() { return m_type; }
  
    void setName(const QString &name);
    QString name() const { return m_name;} 
    void setNameSpace(const QString &nameSpace);
    QString nameSpace() const { return m_nameSpace; } 
    void setAttributes(const QXmlAttributes & attributes);
    QXmlAttributes attributes() { return m_attributes; }
    void setAttributeRanges(const QVector<KTextEditor::Range> & attrRanges); 
  
  private:
//relationship inside of the model    
    QList<DomModelItem *> m_childItems;
    DomModelItem * m_parentItem;

//data    
    Type m_type;
    KTextEditor::Range *m_range;
    QVector<KTextEditor::Range> m_attributeRanges;
  
    QString m_name;
    QString m_nameSpace;
    QXmlAttributes m_attributes;
};



class KDEVQUANTA_EXPORT DomModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
    explicit DomModel(DomModelItem *rootItem, QObject *parent = 0);
    
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
    
    DomModelItem *m_rootItem;
};

#endif

//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
