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

#include <dom/dom_node.h>
#include "node.h"

Node::Node( Node *parent )
{
  this->parent = parent;
  prev = next = child = 0L;
  tag = 0L;
  listItem = 0L;
  opened = false;
  removeAll = true;
  closesPrevious = false;
  insideSpecial = false;
  kafkaNode = 0L;
}


Node::~Node()
{
  if (prev)
      prev->next = 0L;
  if (parent && parent->child == this)
      parent->child = 0L;
  if (removeAll)
  {
   if (child) { delete child; child = 0L;}
   if (next)  { delete next; next = 0L;}
  }
  if ( tag ) { delete tag; tag = 0L;}
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
