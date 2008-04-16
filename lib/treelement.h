/***************************************************************************
    begin                : Wed Apr 16 2008
    copyright            : (C) 2008 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/


//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
#ifndef TREELEMENT_H
#define TREELEMENT_H

#include "quantaexport.h"

#include <QXmlAttributes>
#include <QVector>


namespace KTextEditor {
  class Range;
}


/**
	@author Andras Mantia <amantia@kde.org>
*/
class KDEVQUANTA_EXPORT TreeElement {
public:
  
  enum Type {
    TagStart = 0,
    TagEnd,
    Text,
    Comment,
    Empty
  };
  
  TreeElement(Type type);
  ~TreeElement();
  
  void setNext(TreeElement *element);
  TreeElement *next() { return m_next; }
  void setPrevious(TreeElement *element);
  TreeElement *previous() { return m_previous; }
  TreeElement *parent() { return m_parent; }   
  TreeElement *firstChild() { return m_child; }
  void setChild(TreeElement *child);

  void insertAfter(TreeElement *element);
  void insertBelow(TreeElement *parent);
  
  void setRange(KTextEditor::Range *range);
  KTextEditor::Range *range() { return m_range; }
  Type type() { return m_type; }
  
  void setName(const QString &name);
  QString name() const { return m_name;} 
  void setNameSpace(const QString &nameSpace);
  QString nameSpace() const { return m_nameSpace; } 
  void setAttributes(const QXmlAttributes & attributes);
  QXmlAttributes attributes() { return m_attributes; }
  void setAttributeRanges(const QVector<KTextEditor::Range> & attrRanges); 
    
  
private:
 
  TreeElement *m_child;
  TreeElement *m_parent;
  TreeElement *m_next;
  TreeElement *m_previous;
  Type m_type;
  KTextEditor::Range *m_range;
  QVector<KTextEditor::Range> m_attributeRanges;
  
  QString m_name;
  QString m_nameSpace;
  QXmlAttributes m_attributes;
};

#endif
