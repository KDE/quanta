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

//app includes
#include "groupstorage.h"
#include "groupelement.h"
#include "node.h"
#include "tag.h"

//qt includes

//kde includes
#include <kdebug.h>

GroupStorage::GroupStorage()
{
}


GroupStorage::~GroupStorage()
{
}

void GroupStorage::detachNode(Node* node)
{
  if (m_nodeElements.contains(node))
  {
    GroupElementList elements = m_nodeElements[node];
    GroupElementList::Iterator end(elements.end());
    for (GroupElementList::Iterator it = elements.begin(); it != end; ++it)
    {
      GroupElement *groupElement = (*it);
      groupElement->deleted = true;
      groupElement->node = 0L;
      groupElement->group = 0L;
#ifdef DEBUG_PARSER
      kDebug(24001) << "Detach group element " << groupElement << " from node " << node;
#endif
    }
    m_nodeElements.remove(node);
  }
}

void GroupStorage::clear(QHash<QString, QTag*> *userTags)
{
  m_nodeElements.clear();
  QHashIterator<QString, GroupElementMap*> groupsIt(m_groups);
  while (groupsIt.hasNext())
  {
    groupsIt.next();
    GroupElementMap *membersMap = groupsIt.value();
    GroupElementMap::Iterator mapEnd(membersMap->end());
    for (GroupElementMap::Iterator membersIt = membersMap->begin(); membersIt != mapEnd; ++membersIt)
    {
      GroupElementList *elements = &(membersIt.value());
      GroupElementList::Iterator end(elements->end());
      for (GroupElementList::Iterator elementsIt = elements->begin(); elementsIt != end; ++elementsIt)
      {
#ifdef DEBUG_PARSER
        kDebug(24001) << "Delete group element " << membersIt.key() << ", " << (*elementsIt) << " from group " << groupsIt.key();
#endif
        if (userTags)
          delete userTags->take((*elementsIt)->tag->name().toLower());
        delete (*elementsIt);
      }
    }
  }
  foreach (GroupElementMap* el, m_groups)
  {
    delete el;
  }
  m_groups.clear();
}

void GroupStorage::cleanUp(QHash<QString, QTag*> *userTags)
{
  QHashIterator<QString, GroupElementMap*> groupsIt(m_groups);
  while (groupsIt.hasNext())
  {
    groupsIt.next();
    GroupElementMap *membersMap = groupsIt.value();
    GroupElementMap::Iterator end(membersMap->end());
    GroupElementMap::Iterator deleteIt = end;
    for (GroupElementMap::Iterator membersIt = membersMap->begin(); membersIt != end; ++membersIt)
    {
      if (deleteIt != end)
      {
        membersMap->erase(deleteIt);
        deleteIt = end;
      }
      GroupElementList *elements = &(membersIt.value());
      GroupElementList::Iterator elementsIt = elements->begin();
      while (elementsIt != elements->end())
      {
        GroupElement *groupElement = (*elementsIt);
        if (groupElement->deleted)
        {
#ifdef DEBUG_PARSER
          kDebug(24001) << "Cleanup group element " << membersIt.key() << ", " << groupElement << " from group " << groupsIt.key();
#endif
          if (userTags)
            delete userTags->take(groupElement->tag->name().toLower());
          delete groupElement;
          elementsIt = elements->erase(elementsIt);
        } else
        {
          ++elementsIt;
        }
      }
      if (elements->isEmpty())
        deleteIt = membersIt;
#ifdef DEBUG_PARSER
          kDebug(24001) << "Size of elements list for " << membersIt.key() << ": " << elements->count();
#endif
    }
    if (deleteIt != end)
        membersMap->erase(deleteIt);
  }
}

const GroupElementList GroupStorage::elementsForNode(Node *node) const
{
  if (m_nodeElements.contains(node))
  {
    return m_nodeElements[node];
  } else
    return GroupElementList();
}

const GroupElementList GroupStorage::elementsFromGroup(const QString& group, const QString &memberName) const
{
  if (!m_groups.contains(group) || memberName.isEmpty())
  {
    return GroupElementList();
  }
  GroupElementMap *elementMap = m_groups[group];
  return (*elementMap)[memberName];
}


void GroupStorage::insert(const QString& group, const QString &memberName, GroupElement *element)
{
  if (!m_groups.contains(group))
  {
    m_groups.insert(group, new GroupElementMap());
  }
  GroupElementMap *elementMap = m_groups[group];
  GroupElementList *elements = &((*elementMap)[memberName]);
  elements->append(element);
  m_nodeElements[element->node].append(element);
#ifdef DEBUG_PARSER
  kDebug(24001) << "Insert group element " << memberName << ", " << element << " into " << group;
#endif
}

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
