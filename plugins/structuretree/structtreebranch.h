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
#ifndef STRUCTTREEBRANCH_H
#define STRUCTTREEBRANCH_H

#include <structtreeitem.h>

/**
@short the root item for a structtree branch



@author Jens Herden
*/
class StructTreeBranch : public StructTreeItem
{
public:
  StructTreeBranch(Q3ListView *parent, const QString &title, Node *firstChild, StructTreeBranch *after = 0);

  ~StructTreeBranch();

  /**
   * 
   * 
   * @param parent 
   * @param node 
   * @param title 
   * @param after 
   * @return the new item
   */
  StructTreeItem* createStrucTreeItem(StructTreeItem *parent, Node *node, const QString title = QString(), StructTreeItem *after = 0)
  {
    return new StructTreeItem(this, parent, node, title, after);
  };
};

#endif
