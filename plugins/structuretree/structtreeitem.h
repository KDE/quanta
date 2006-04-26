/***************************************************************************
    
    begin                : June 20 2005
    copyright            : (C) 2005 Andras Mantia <amantia@kde.org> 
                           (C) 2005 Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STRUCTTREEITEM_H
#define STRUCTTREEITEM_H

#include "filterableitemif.h"

#include <k3listview.h>

class Node;
class Tag;
class AreaStruct;
class QPoint;
class StructureTreeWidget;


/** @short a KListView item for a node in the structuretree
  *
  * Every instance can have a node associated and uses information from it.
  * @see Node
  *
  * @author Jens Herden
  */

class StructTreeItem : public K3ListViewItem, public FilterableItemIf {

public:
  
  StructTreeItem(StructTreeItem *parent, const Node *node, StructTreeItem *after = 0);
  
  StructTreeItem(StructureTreeWidget *parent, const Node *node, StructTreeItem *after = 0);
  
  ~StructTreeItem();

  /**
   * populates the tree with StructTreeItems for the children of the associated Node
   */
  void populate();
  
  /**
   * adjust myself and the children to new settings
   */
  void settingsChanged();
  
  /**
   * Do the recursive opening or closing of this item and all subtree
   * 
   * @param open  true == open
   */
  void setSubTreeOpen(bool open);

  /**
   * returns the area of the Tag
   *  
   * @param adjustForXML true = if the tag is XML than start column will get incremented by 1
   */
  AreaStruct tagArea(bool adjustForXML = false, bool excludeClosing = false) const;

  /**
   * reimplemented for internal reasons
   * @param open 
   */
  void setOpen(bool open);

  /**
   * search for the deepest StructreeItem that contains a Tag with an Area
   * which encloses the point
   * @param qp qp.x() = line and qp.y() = column
   * @return the item which must be visible
   */
  StructTreeItem * makeNodeVisible(const QPoint & qp);

  /**
   * 
   * @return true if this item should be visible
   */
  bool shouldBeVisible() const;

  /**
   * populate the first child and the next sibling
   */
  void populateAll();

  /**
   * @param s the string to compare with
   * @return true if this item should be visible
   */
  bool filter(const QString &s) const;

private:
  
  const Node *m_node;
  const Tag *m_tag;

  /**
   * sets the membervariables
   */
  void initItem();
  
  /**
   * Do the recursive opening or closing of the next sibling and the first child
   * 
   * @param open  true == open
   */
  void openSubTree(bool open);
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
