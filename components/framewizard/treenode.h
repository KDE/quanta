/***************************************************************************
                          treenode.h  -  description
                             -------------------
    begin                : lun mar 17 2003
    copyright            : (C) 2003 by gulmini luciano
    email                : gulmini.luciano@student.unife.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TREENODE_H
#define TREENODE_H

#include <qptrlist.h>
#include <qdict.h>
#include <qstringlist.h>

#include "fwglobal.h"
#include "areaattributedb.h"

/**a node in the tree
  *@author gulmini luciano
  */
class treeNode {
   private:
      QString m_label,
                   m_parentLabel;
      SplitType m_splitType;
      QPtrList<treeNode> m_childrenList;
      areaAttribute *m_atts;
     
   public:
     treeNode(QString l=QString::null,QString pl=QString::null);
     ~treeNode();
      void addChildNode(QString);
      void addChildNode(treeNode *n){ m_childrenList.append(n); }
      void removeChildNode(QString l, bool autoDelete);
      void setSplitType(SplitType s) { m_splitType = s; }
      void setLabel(QString l) { m_label = l; }
      void removeChildren() { m_childrenList.clear(); }
      void setParentLabel(QString s){ m_parentLabel = s;}
      int  childPosition(treeNode* n){ return m_childrenList.find(n); }
      bool insertChild(unsigned int pos, treeNode* n) { return m_childrenList.insert( pos, n); }
      QString label() const { return m_label; }
      QString parentLabel() const { return m_parentLabel; }
      SplitType splitType() const { return m_splitType; }
      QPtrList<treeNode> childrenList() { return m_childrenList; }

      treeNode* firstChild() { return m_childrenList.first(); }
      treeNode* nextChild() { return m_childrenList.next(); }
      treeNode* lastChild() { return m_childrenList.last(); }
      treeNode* currentChild()  { return m_childrenList.current(); }
      treeNode* findChild(QString);

      areaAttribute* atts() { return m_atts; }
        
      int countChildren() const { return m_childrenList.count(); }
      bool hasChildren() const { return !m_childrenList.isEmpty(); }
};

class tree{
   private:
      treeNode *m_root;
      QDict<treeNode> m_nodeList;
      static int nodeId;

   public:
      tree();
      ~tree();
      treeNode* root() const { return m_root; }
      QString addChildNode(QString l);
      bool insertChildNode(QString);
      void removeChildNode(QString pl,QString l,bool autoDelete);//parent node,child node
      treeNode* findNode(QString);
      areaAttribute* findAreaAttribute(QString l){ return findNode(l)->atts(); };
      void reset();
      void refreshGeometries(treeNode*);
};

#endif
