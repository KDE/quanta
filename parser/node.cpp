/***************************************************************************
                          node.cpp  -  description
                             -------------------
    begin                : Sun Apr 16 2000
    copyright            : (C) 2000 by Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2001-2003 Andras Mantia <amantia@kde.org>
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
#include "../quantacommon.h"
#ifdef BUILD_KAFKAPART
#include "../parts/kafka/kafkacommon.h"
#endif

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
  groupElementLists.clear();
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

QString Node::nodeName()
{
	if(tag)
		return tag->name;
	return "";
}

QString Node::nodeValue()
{
	if(tag)
		return tag->tagStr();
	return "";
}

void Node::setNodeValue(QString value)
{
	if(!tag)
		tag = new Tag();
	tag->setStr(value);
}

Node* Node::lastChild()
{
	Node *n, *m;
	n = child;
	while(n)
	{
		m = n;
		n = n->next;
	}
	return m;
}

 Node *Node::nextNE()
 {
	Node *n = next;
	while(n && n->tag->type == Tag::Empty)
		n = n->next;
	return n;
 }

 Node *Node::prevNE()
 {
	Node *n = prev;
	while(n && n->tag->type == Tag::Empty)
		n = n->prev;
	return n;
 }


#ifdef BUILD_KAFKAPART
bool Node::hasForChild(Node *node)
{
	Node *n;
	bool goUp = false;

	if(child)
	{
		n = child;
		goUp = false;
		while(n)
		{
			if(n == node)
				return true;
			n = kafkaCommon::getNextNode(n, goUp, this);
		}
	}
	return false;

}
#endif

Node *Node::getClosingNode()
{
	Node* n = next;

	if(next && tag && tag->type == Tag::XmlTag && !tag->single)
	{
		while(n && n->tag->type == Tag::Empty)
			n = n->next;
		if (n && n->tag->type == Tag::XmlTagEnd && QuantaCommon::closesTag(tag, n->tag))
			return n;
	}
	return 0L;
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
