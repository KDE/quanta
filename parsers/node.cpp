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
#include "quantacommon.h"
#include "structtreetag.h"
#ifdef BUILD_KAFKAPART
#include "kafkacommon.h"
#endif

GroupElementMapList globalGroupMap;

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
  m_rootNode = 0L;
  m_leafNode = 0L;
#endif
  groupElementLists.clear();
}


Node::~Node()
{
#ifdef BUILD_KAFKAPART
  //It has no use, except to know when it crash why it has crashed.
  //If it has crashed here, the Node doesn't exist anymore.
  // If it has crashed the next line, it is a GroupElements bug.
  tag->cleanStrBuilt = false;
#endif
  //Remove the references to this node from the list of group elements.
  //They are actually stored in globalGroupMap.
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
       // it = groupElementList->erase(it);
        (*it).node = 0L;
        (*it).tag = 0L;
        (*it).deleted = true;        
      } else
        ++it;
    }
  }
  if (listItem)
  {
    static_cast<StructTreeTag*>(listItem)->node = 0L;
    static_cast<StructTreeTag*>(listItem)->groupTag = 0L;
  }
  
  groupElementLists.clear();
  if (prev && prev->next == this)
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
  
  delete tag; 
  tag = 0L;
  delete groupTag; 
  groupTag = 0L;
#ifdef BUILD_KAFKAPART
  if(m_rootNode)
    delete m_rootNode;
  if(m_leafNode)
    delete m_leafNode;
#endif
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
  return QString::null;
}

QString Node::nodeValue()
{
  if(tag)
    return tag->tagStr();
  return QString::null;
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

Node *Node::firstChildNE()
{
  Node *n = child;
  while(n && n->tag->type == Tag::Empty)
    n = n->next;
  return n;
}

Node *Node::lastChildNE()
{
  Node *n = lastChild();
  while(n && n->tag->type == Tag::Empty)
    n = n->prev;
  return n;
}

Node *Node::SPrev()
{
  Node *node = prev;
  int bCol, bLine, eCol, eLine, col, line;

  if(parent)
  {
    parent->tag->beginPos(bLine, bCol);
    parent->tag->endPos(eLine, eCol);
  }

  while(node && node->tag->type != Tag::XmlTag && node->tag->type != Tag::Text)
  {
    if(node->tag->type == Tag::ScriptTag)
    {
       //Check if it is an embedded ScriptTag. If it is, continue.
       node->tag->beginPos(line, col);
       if(QuantaCommon::isBetween(line, col, bLine, bCol, eLine, eCol) != 0)
         break;
    }
    node = node->prev;
  }

  return node;
}

Node *Node::SNext()
{
  Node *node = next;
  int bCol, bLine, eCol, eLine, col, line;

  if(parent)
  {
    tag->beginPos(bLine, bCol);
    tag->endPos(eLine, eCol);
  }

  while(node && node->tag->type != Tag::XmlTag && node->tag->type != Tag::Text)
  {
    if(node->tag->type == Tag::ScriptTag)
    {
       //Check if it is an embedded ScriptTag. If it is, continue.
       node->tag->beginPos(line, col);
       if(QuantaCommon::isBetween(line, col, bLine, bCol, eLine, eCol) != 0)
         break;
    }
     node = node->next;
  }

  return node;
}

Node *Node::SFirstChild()
{
  Node *node = child;
  int bCol, bLine, eCol, eLine, col, line;

  tag->beginPos(bLine, bCol);
  tag->endPos(eLine, eCol);
  while(node && node->tag->type != Tag::XmlTag && node->tag->type != Tag::Text)
  {
    if(node->tag->type == Tag::ScriptTag)
    {
       //Check if it is an embedded ScriptTag. If it is, continue.
       node->tag->beginPos(line, col);
       if(QuantaCommon::isBetween(line, col, bLine, bCol, eLine, eCol) != 0)
         break;
    }
     node = node->next;
  }

  return node;
}

Node *Node::SLastChild()
{
  Node *node = lastChild();
  int bCol, bLine, eCol, eLine, col, line;

  tag->beginPos(bLine, bCol);
  tag->endPos(eLine, eCol);
  while(node && node->tag->type != Tag::XmlTag && node->tag->type != Tag::Text)
  {
    if(node->tag->type == Tag::ScriptTag)
    {
       //Check if it is an embedded ScriptTag. If it is, continue.
       node->tag->beginPos(line, col);
       if(QuantaCommon::isBetween(line, col, bLine, bCol, eLine, eCol) != 0)
         break;
    }
     node = node->prev;
  }

  return node;
}

#ifdef BUILD_KAFKAPART
bool Node::hasForChild(Node *node)
{
  //TODO: NOT EFFICIENT AT ALL!! Change by using kafkaCommon::getLocation() and compare!
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

  if(next && tag && (tag->type == Tag::XmlTag || tag->type == Tag::ScriptTag) && !tag->single)
  {
    while (n && n->tag->type == Tag::Empty)
      n = n->next;
    if (n && n->tag->type == Tag::XmlTagEnd && ((tag->type == Tag::XmlTag && QuantaCommon::closesTag(tag, n->tag)) || (tag->type == Tag::ScriptTag && n->tag->name == "")))
      return n;
  }
  return 0L;
}

Node *Node::getOpeningNode()
{
  Node *n = prev;
  if(prev && tag && tag->type == Tag::XmlTagEnd)
  {
    while(n && n->tag->type == Tag::Empty)
      n = n->prev;
    if(n && ((n->tag->type == Tag::XmlTag && QuantaCommon::closesTag(n->tag, tag))
      || (n->tag->type == Tag::ScriptTag && tag->name == "")))
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

void Node::operator =(Node* node)
{
  (*this) = (*node);
  prev = 0L;
  next = 0L;
  parent = 0L;
  child = 0L;
  listItem = 0L;
  groupElementLists.clear();
  group = 0L;
  groupTag = 0L;
#ifdef BUILD_KAFKAPART
  setRootNode(0L);
  setLeafNode(0L);
#endif
  tag = new Tag(*(node->tag));
}
