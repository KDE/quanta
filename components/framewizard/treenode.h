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
#include <qstring.h>
#include <qptrlist.h>
#include <qdict.h>
#include <qstringlist.h>

#include "areaattributedb.h"

/**a node in the tree
  *@author gulmini luciano
  */
class treeNode {
   private:
      QString label,
              parentLabel,
              split;
      QPtrList<treeNode> childrenList;
      areaAttribute *atts;
   public:
     treeNode(QString l="",QString pl="");
     ~treeNode();
      void addChildNode(QString);
      void addChildNode(treeNode *n){ childrenList.append(n); }
      void removeChildNode(QString l, bool autoDelete);
      void setSplit(QString s) { split = s; }
      void setLabel(QString l) { label = l; }
      void removeChildren() { childrenList.clear(); }
      void setParentLabel(QString s){ parentLabel = s;}
      int  childPosition(treeNode* n){ return childrenList.find(n); }
      bool insertChild(uint pos, treeNode* n) { return childrenList.insert( pos, n); }
      QString getLabel() const { return label; }
      QString getParentLabel() const { return parentLabel; }
      QString getSplit() const { return split; }
      QPtrList<treeNode> getChildrenList() { return childrenList; }

      treeNode* firstChild() { return childrenList.first(); }
      treeNode* nextChild() { return childrenList.next(); }
      treeNode* lastChild() { return childrenList.last(); }
      treeNode* getCurrentChild()  { return childrenList.current(); }
      treeNode* getLastChild()  { return childrenList.getLast(); }
      treeNode* findChild(QString);

      areaAttribute* getAtts() { return atts; }
      QMap<QString,QString> getAttributeMap() { return atts->getAttributeMap(); }
      void setAtts(areaAttribute *a) { atts = a; }
      void setAtts(const QMap<QString,QString>& map){ atts->setAllAttributes(map); }
      void setAtts(QString name, QString value) { atts->setAttribute(name,value); }
      void setGeometry(QRect r){ atts->setGeometry(r); }

      int countChildren() const { return childrenList.count(); }
      bool hasChildren() const { return !childrenList.isEmpty(); }
};


class tree{
   private:
      QString reldPath;// Percorso da "relativizzare"
      treeNode *root;
      QDict<treeNode> nodeList;
      void createStructure(treeNode*);
      QString RCvalue(treeNode *n);
      QString formatStructure();
      QString createFrameTag(areaAttribute*);
      QStringList nonFormattedStructure;
      void refreshGeometries(treeNode*);
      static int nodeId;
      static int tabNum;
   public:
      tree();
      ~tree();
      treeNode* getRoot() const { return root; }
      QString addChildNode(QString l);
      bool insertChildNode(QString);
      void removeChildNode(QString l,QString ll,bool autoDelete);//parent node,child node
      treeNode* findNode(QString);
      areaAttribute* findAreaAttribute(QString l){ return findNode(l)->getAtts(); };
      void reinitialize();
      QString framesetStructure(QString path) { reldPath=path; return formatStructure(); }

};


#endif
