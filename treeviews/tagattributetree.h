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
class Node;


class TagAttributeTree : public KListView  {
   Q_OBJECT

public:
  TagAttributeTree(QWidget *parent=0, const char *name=0);
  ~TagAttributeTree();

  void newCursorPosition(Node *node);

private slots:
  void slotItemRenamed(QListViewItem *item);

private:
  Node *m_node;

};


class TopLevelItem : public KListViewItem
{

public:
    TopLevelItem(TagAttributeTree *parent, const QString& title);
    TopLevelItem(TagAttributeTree *parent, QListViewItem* after, const QString& title);

    virtual ~TopLevelItem();

protected:

    void paintCell( QPainter *p, const QColorGroup &cg,
                    int column, int width, int align );
};


#endif

