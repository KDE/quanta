/***************************************************************************
    begin                : Mon Jun 27 2005
    copyright            : (C) 2005 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include "structtreebranch.h"
#include "lib/node.h"



StructTreeBranch::StructTreeBranch(QListView *parent, const QString &title, Node *firstChild, StructTreeBranch *after)
: StructTreeItem(parent, title, after)
{
  Node *node = new Node(0);
  node->child = firstChild;
  m_node = const_cast<Node *>(node);
}


StructTreeBranch::~StructTreeBranch()
{
}



