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

//kde includes

class KListView;
class Node;

class TagAttributeTree : public KListView  {
   Q_OBJECT

public:
  TagAttributeTree(QWidget *parent=0, const char *name=0);
  ~TagAttributeTree();

  void newCursorPosition(Node *node);
};

#endif

