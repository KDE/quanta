/***************************************************************************
    begin                : Mon Jul 18 2005
    copyright            : (C) 2005 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef GROUPSTREEITEM_H
#define GROUPSTREEITEM_H

#include "groupstorage.h"

#include <k3listview.h>


class GroupsWidget;
class AreaStruct;
class Tag;
class KUrl;

/**
@short K3ListView for the groups tree

@author Jens Herden <jens@kdewebdev.org>
*/
class GroupsTreeItem : public K3ListViewItem {
public:
  GroupsTreeItem(GroupsWidget *parent, const GroupElementMap *groupElements, GroupsTreeItem *after = 0);

  GroupsTreeItem(GroupsTreeItem *parent, const GroupElementMap *groupElements, const Tag *tag, GroupsTreeItem *after = 0);

  ~GroupsTreeItem();
  
  /**
   * populates the tree with StructTreeItems for the children of the associated Node
   */
  void populate();
  
  /**
   * returns the area of the Tag
   *  
   * @param adjustForXML true = if the tag is XML than start column will get incremented by 1
   */
  AreaStruct tagArea(bool adjustForXML = false) const;
  
  /**
   * adjust myself and the children to new settings
   */
  void settingsChanged();
  
  /** try to find a KUrl to this item (e.g. for links or img tags) can be 
   *  relative or absolute
   * 
   * @return the KUrl associated to this item or KUrl()
   */
  KUrl getURL() const;

private:
 
  /**
   * @return true is the item should be shown
   */
  bool shouldBeVisible() const;
 
  /**
   * add the elements to the tree
   */
  void addElements();
  
  /**
   * add the sub elements to the tree
   */
  void addMoreElements();

  const Tag *m_tag;
  const GroupElementMap *m_groupElements;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
