//
//
// C++ Interface: h
//
// Description:
//
//
// Author: Andras Mantia <amantia@freemail.hu>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TAGATTRIBUTETREE_H
#define TAGATTRIBUTETREE_H

//qt includes
#include <qwidget.h>

//kde includes

//app includes

class KListView;
class KListViewItem;
class ParentItem;
class Node;


class TagAttributeTree : public KListView  {
   Q_OBJECT

public:
  TagAttributeTree(QWidget *parent=0, const char *name=0);
  ~TagAttributeTree();

  void setCurrentNode(Node *node);
  virtual void setCurrentItem( QListViewItem *i );
  Node *node() const {return m_node;}

public slots:
  void slotParentSelected(int index);
  void editorContentChanged();

private slots:
  void slotCollapsed(QListViewItem *item);
  void slotExpanded(QListViewItem *item);

signals:
  void newNodeSelected(Node *node);

private:
  Node *m_node;
  bool rebuildEnabled;
  ParentItem *m_parentItem;
};

#endif

