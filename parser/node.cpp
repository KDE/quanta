/***************************************************************************
                          node.cpp  -  description
                             -------------------
    begin                : Sun Apr 16 2000
    copyright            : (C) 2000 by Dmitry Poplavsky
                           (C) 2001-2003 Andras Mantia <amantia@freemail.hu>
    email                : pdima@mail.univ.kiev.ua
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//qt includes
#include <qlistview.h>

#include "node.h"
#include "tag.h"
#include "qtag.h"

Node::Node( Node *parent )
{
  this->parent = parent;
  prev = next = child = 0L;
  tag = 0L;
  groupTag = 0L;
  group = 0L;
  listItem = 0L;
  opened = false;
  removeAll = true;
  closesPrevious = false;
  insideSpecial = false;
  #ifdef BUILD_KAFKAPART
  _closingNode = 0L;
  #endif
}


Node::~Node()
{
  //Remove the references to this node from the list of group elements.
  //They are actually stored in Parser::m_groups.
  QPtrListIterator<GroupElementList> iter(groupElementLists);
  GroupElementList *groupElementList;
  while ((groupElementList = iter.current()) != 0)
  {
    ++iter;
    GroupElementList::Iterator it = groupElementList->begin();
    while (it != groupElementList->end())
    {
      if ((*it).node == this)
      {
        it = groupElementList->erase(it);
      } else
        ++it;
    }
  }
  if (prev)
      prev->next = 0L;
  if (parent && parent->child == this)
      parent->child = 0L;
  if (removeAll)
  {
    if (child)
    {
      delete child;
      child = 0L;
    }
    if (next)
    {
      delete next;
      next = 0L;
    }
  }
  if ( tag ) { delete tag; tag = 0L;}
  if ( groupTag ) { delete groupTag; groupTag = 0L;}
}

Node *Node::nextSibling()
{
  Node *result = 0L;
  if (child)
  {
    result = child;
  } else
  if (next)
  {
    result = next;
  } else
  {
    Node *n = this;
    while (n)
    {
      if (n->parent && n->parent->next)
      {
        result = n->parent->next;
        break;
      } else
      {
        n = n->parent;
      }
    }
  }

  return result;
}


Node *Node::previousSibling()
{
  Node *result = 0L;
  if (prev)
  {
     Node *n = prev;
     while (n->child)
     {
       n = n->child;
       while (n->next)
         n = n->next;
     }
     result = n;
  } else
  {
    result = parent;
  }

  return result;
}

Node *Node::nextNotChild()
{
  if (next)
      return next;
  else
  {
    Node *n = this;
    while (n)
    {
      if (n->parent && n->parent->next)
      {
        n = n->parent->next;
        break;
      } else
      {
        n = n->parent;
      }
    }

    return n;
  }
}

int Node::size()
{
  int l = tag->size();
#ifdef BUILD_KAFKAPART
  l += 5*sizeof(Node*) + sizeof(QListViewItem*) + 2*sizeof(Tag*) + 2*sizeof(DOM::Node);
#else
  l += 4*sizeof(Node*) + sizeof(QListViewItem*) + 2*sizeof(Tag*);
#endif
  return l;
}
