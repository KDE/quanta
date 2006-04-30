/***************************************************************************
    begin                : Fri Jul 15 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef GROUPSTORAGE_H
#define GROUPSTORAGE_H

#include "qtag.h"
#include "quantaexport.h"

#include <qhash.h>
#include <qmap.h>
#include <qlist.h>

class GroupElement;
class Node;

/**
Holds and takes care of the group elements found in a document.

@author Andras Mantia
*/
typedef QList<GroupElement*> GroupElementList;
typedef QMap<QString, GroupElementList> GroupElementMap; ///<holds a list of occurences of elements with the same id (name). For example $i can appear in more than once place in the document, so QMap["$i"] will give a list with this appearances

class LIBQUANTA_EXPORT GroupStorage{
public:
  GroupStorage();
  ~GroupStorage();

  /**
   * Flags the group elements referring to a node as deleted.
   * @param node the node in question
   */
  void detachNode(Node *node);

  /**
   * Deletes all the internal data.
   * @param userTags: removes the deleted elements from the userTags dictionary.
   */
  void clear(QHash<QString, QTag*> *userTags);

  /**
   * Deletes all those elements which were marked as deleted.
   * @param userTags: removes the deleted elements from the userTags dictionary.
   */
  void cleanUp(QHash<QString, QTag*> *userTags);

  /**
   * Returns a list of elements associated with a node.
   * @param node The node in question
   * @return The list, or an empty GroupElementList if no list is associated with this node.
   */
  const GroupElementList elementsForNode(Node *node) const;

  /**
   * Returns a list of elements belonging to a group and having the specified name (id).
   * @param group The group in question
   * @param memberName The name (id) of the searched elements. If empty, the returned list will be empty as well.
   * @return The list, or an empty GroupElementList if no elements belong to this group.
   */
  const GroupElementList elementsFromGroup(const QString& group, const QString &memberName) const;

  /**
   * Returns a map of all elements belonging to a group.
   * @param group The group in question
   * @return The map, or null if no elements belong to this group.
   */
  const GroupElementMap *elementMapForGroup(const QString& group) const {return m_groups[group];};

  /**
   * Inserts a group element to the internal data structures.
   * @param group The name of the group the element belongs to
   * @param memberName The name (used as an id) of the element
   * @param element The element itself
   */
  void insert(const QString& group, const QString &memberName, GroupElement *element);

  /**
   * 
   * @return the number of elements in a group or 0 if the group does not exist
   */
  uint elementCount(const QString &group) const {const GroupElementMap *map = m_groups[group]; return map ? map->count() : 0;};
  
private:
  QHash<QString, GroupElementMap*> m_groups; ///<the main data structure. Holds pointers to a map for every group
  QMap<Node*, GroupElementList> m_nodeElements; ///< makes a relation between a node from the node tree and all the elements found in this node

};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
