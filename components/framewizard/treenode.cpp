/***************************************************************************
                          treenode.cpp  -  description
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

#include "treenode.h"

static const int SIZE = 101;

treeNode::treeNode(const QString &l, const QString &pl) : m_label(l), m_parentLabel(pl), m_splitType(NONE){
  m_childrenList.setAutoDelete(true);
  m_atts = new areaAttribute;
}

treeNode::~treeNode(){
  delete m_atts;
}

void treeNode::addChildNode(const QString &l) {
  m_childrenList.append( new treeNode(l,m_label) );
}

void treeNode::removeChildNode(const QString &l,bool autoDelete) {
  m_childrenList.setAutoDelete(autoDelete);
  m_childrenList.remove(findChild(l));
}

treeNode* treeNode::findChild(const QString &l){
  QPtrListIterator<treeNode> it( m_childrenList );
  treeNode *node;
  while ( (node = it.current()) != 0 ) {
    ++it;
    if(node->label() == l) return node;
  }
  return 0L;
}

int tree::nodeId = 0;

tree::tree(){
  m_root = new treeNode(QString::number(nodeId,10));
  m_nodeList.resize(SIZE);
}

tree::~tree(){
  delete m_root;
}

void tree::refreshGeometries(treeNode *n){
  int dim = -6;// so we won't add exceeding pixels

  if(n->hasChildren()){
    n->firstChild();
    while(n->currentChild()){
      refreshGeometries(n->currentChild());
      n->nextChild();
    }

    QPtrList<treeNode> list = n->childrenList();
    QPtrListIterator<treeNode> it( list );
    treeNode *node= it.current();
    QRect newGeometry = n->atts()->geometry();
    if(n->splitType()==VERTICAL){
      newGeometry.setHeight(node->atts()->geometry().height());
      while ( (node = it.current()) != 0 ) {
        ++it;
        dim += node->atts()->geometry().width();
        dim += 6;
      }
      newGeometry.setWidth(dim);
    }
    else
    if(n->splitType()==HORIZONTAL){
      newGeometry.setWidth(node->atts()->geometry().width());
      while ( (node = it.current()) != 0 ) {
        ++it;
        dim += node->atts()->geometry().height();
        dim += 6;
      }
      newGeometry.setHeight(dim);
    }

    n->atts()->setGeometry( newGeometry );
  }
}

treeNode* tree::findNode(const QString &l){
  if(l==m_root->label()) return m_root;
  return m_nodeList.find(l);
}

QString tree::addChildNode(const QString &l){
  treeNode *node;
  if( (node = findNode(l)) != 0) {
    ++nodeId;
    treeNode *newNode = new treeNode(QString::number(nodeId,10),node->label());
    newNode->atts()->setAttribute( "src",node->atts()->src() );
    node->addChildNode(newNode);
    m_nodeList.insert(QString::number(nodeId,10),newNode);
  }
  return QString::number(nodeId,10);
}

bool tree::insertChildNode(const QString &l){
  QString parent = findNode(l)->parentLabel();
  int pos=findNode( parent )->childPosition( findNode(l) );
  ++nodeId;
  treeNode *newNode = new treeNode(QString::number(nodeId,10),parent);
  newNode->atts()->setAttribute( "src",findNode(l)->atts()->src() );
  m_nodeList.insert(QString::number(nodeId,10),newNode);
  return findNode( parent )->insertChild(pos,newNode);
}

void tree::reset(){
  nodeId = 1;
  m_root->removeChildren();
  m_root->atts()->resetAttributes();
  m_nodeList.clear();
}

void tree::removeChildNode(const QString &pl,const QString &l,bool autoDelete){
  findNode(pl)->removeChildNode(l,autoDelete);
}
