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


#include "node.h"
#include "groupstorage.h"
#include "tag.h"
#include "qtag.h"

//qt includes
#include <QListView>
#include <qdom.h>

//KDE includes
#include <kdebug.h>


Node::Node(Node *parent)
{
  this->parent = parent;
  prev = next = child = 0L;
  tag = 0L;
  opened = false;
  removeAll = true;
  closesPrevious = false;
  insideSpecial = false;
  _closingNode = 0L;
  m_groupStorage = 0L;
  //m_groupElements.clear();
}


Node::~Node()
{
 // kDebug(24000) << "Node destructor " << this << tag->name << endl;
  //It has no use, except to know when it crash why it has crashed.
  //If it has crashed here, the Node doesn't exist anymore.
  // If it has crashed the next line, it is a GroupElements bug.
  tag->setCleanStrBuilt(false);

//  detachNode();
  if (prev && prev->next == this)
      prev->next = 0L;
  if (parent && parent->child == this)
      parent->child = 0L;
  if (removeAll)
  {
    child->deleteNode(m_groupStorage);
    child = 0L;
    next->deleteNode(m_groupStorage);
    next = 0L;
  }

  delete tag;
  tag = 0L;
}

void Node::deleteNode(GroupStorage *groupStorage)
{
  if (!this)
    return;
  m_groupStorage = groupStorage;
  if (groupStorage)
  {
    groupStorage->detachNode(this);
  }
  delete this;
}

// void Node::save(QDomElement& element) const
// {
//     //kDebug(25001) << "Save:\n" << element.ownerDocument().toString() << endl;
//     QDomElement child_element;
//     if(next)
//     {
//         child_element = element.ownerDocument().createElement("nodeNext");
//         element.appendChild(child_element);
//         next->save(child_element);
//     }
//     if(child)
//     {
//         child_element = element.ownerDocument().createElement("nodeChild");
//         element.appendChild(child_element);
//         child->save(child_element);
//     }
//     if(_closingNode)
//     {
//         if(_closingNode != next)
//         {
//             child_element = element.ownerDocument().createElement("nodeClosing");
//             element.appendChild(child_element);
//             _closingNode->save(child_element);
//         }
//     }
//
//     Q_ASSERT(tag);
//     child_element = element.ownerDocument().createElement("tag");
//     element.appendChild(child_element);
//     tag->save(child_element);
//
//     element.setAttribute("closesPrevious", closesPrevious);           // bool
//     element.setAttribute("opened", opened);     // bool
//     element.setAttribute("removeAll", removeAll); // bool
//     element.setAttribute("insideSpecial", insideSpecial);             // bool
//     element.setAttribute("specialInsideXml", specialInsideXml);             // bool
//     element.setAttribute("fileName", fileName);                         // QString
//
// /*    QString s_element;
//     QTextStream stream(&s_element, IO_WriteOnly);
//     element.save(stream, 3);*/
//     //kDebug(25001) << "Load:\n" << s_element << endl;
//     //kDebug(25001) << "Save:\n" << element.ownerDocument().toString() << endl;
// }
//
// bool Node::load(QDomElement const& element)
// {
// /*    QString s_element;
//     QTextStream stream(&s_element, IO_WriteOnly);
//     element.save(stream, 3);*/
//     //kDebug(25001) << "Load:\n" << s_element << endl;
//
//     QDomNodeList list = element.childNodes();
//     for(unsigned int i = 0; i != list.count(); ++i)
//     {
//         if(list.item(i).isElement())
//         {
//             QDomElement e = list.item(i).toElement();
//             if(e.tagName() == "nodeNext")
//             {
//                 next = new Node(0);
//                 next->prev = this;
//                 next->parent = this->parent;
//                 next->load(e);
//             }
//             else if(e.tagName() == "nodeChild")
//             {
//                 child = new Node(0);
//                 child->parent = this;
//                 child->load(e);
//             }
//             else if(e.tagName() == "nodeClosing")
//             {
//                 _closingNode = new Node(0);
//                 _closingNode->load(e);
//             }
//             else if(e.tagName() == "tag")
//             {
//                 tag = new Tag();
//                 tag->load(e);
//             }
//         }
//     }
//
//     closesPrevious = QString(element.attribute("closesPrevious")).toInt();  // bool
//     opened = QString(element.attribute("opened")).toInt();     // bool
//     removeAll = QString(element.attribute("removeAll")).toInt(); // bool
//     insideSpecial = QString(element.attribute("insideSpecial")).toInt();    // bool
//     specialInsideXml = QString(element.attribute("specialInsideXml")).toInt();  // bool
//     fileName = element.attribute("fileName");                         // QString
//
//     //Node::coutTree(this, 3);
//
//     return true;
// }

Node *Node::nextSibling()
{
    if (child)
        return child;

    if (next)
        return next;

    Node *n = this;
    while (n)
    {
        if (n->parent && n->parent->next)
            return n->parent->next;

        n = n->parent;
    }
    return 0;
}


Node *Node::previousSibling()
{
    if (!prev)
      return parent;

    Node *n = prev;
    while (n->child)
    {
        n = n->child;
        while (n->next)
            n = n->next;
    }
    return n;
}

Node *Node::nextNotChild()
{
    if (next)
        return next;

    Node *n = this;
    while (n)
    {
        if (n->parent && n->parent->next)
            return n->parent->next;

        n = n->parent;
    }
    return n;
}

QString Node::nodeName()
{
    if (tag)
        return tag->name();
    return QString();
}

QString Node::nodeValue()
{
    if (tag)
        return tag->tagStr();
    return QString();
}

void Node::setNodeValue(const QString &value)
{
    if (!tag)
        tag = new Tag();
    tag->setStr(value);
    kDebug(24000) << "Node::setNodeValue: dtd is 0L for " << value << endl;
}

Node* Node::lastChild()
{
    Node *n = child;
    while (n && n->next)
    {
        n = n->next;
    }
    return n;
}


bool Node::hasForChild(Node */*node*/)
{
 return false; //FIXME!!!
    //TODO: NOT EFFICIENT AT ALL!! Change by using kafkaCommon::getLocation() and compare!
/*    Node *n;
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
    return false;*/
}

Node *Node::getClosingNode()
{
    Node* n = next;

    if(next && tag && (tag->isType(Tag::XmlTag) || tag->isType(Tag::ScriptTag)) && !tag->single)
    {
        while (n && n->tag->isType(Tag::Empty))
            n = n->next;
        if (n && n->tag->isType(Tag::XmlTagEnd) && (tag->isType(Tag::XmlTag) &&  n->tag->doesClose(tag)) || (tag->isType(Tag::ScriptTag) && n->tag->name().isEmpty()))
            return n;
    }
    return 0L;
}

Node *Node::getOpeningNode()
{
    Node *n = prev;
    if(prev && tag && tag->isType(Tag::XmlTagEnd))
    {
        while(n && n->tag->isType(Tag::Empty))
            n = n->prev;
        if(n && ((n->tag->isType(Tag::XmlTag) && tag->doesClose(n->tag))
                 || (n->tag->isType(Tag::ScriptTag) && tag->name().isEmpty())))
            return n;
    }
    return 0L;
}

// int Node::size()
// {
//     int l = tag->size();
//     l += 5*sizeof(Node*) + sizeof(Q3ListViewItem*) + 2*sizeof(Tag*) + 2*sizeof(DOM::Node);
//     return l;
// }

void Node::operator =(Node* node)
{
  (*this) = (*node);
  prev = 0L;
  next = 0L;
  parent = 0L;
  child = 0L;
//  m_groupElements.clear();
  tag = new Tag(*(node->tag));
}

/*
void Node::detachNode()
{
  int count = 0;
  //kDebug(24000) << &m_groupElements << " " << this << endl;
  //Remove the references to this node from the list of group elements.
  //They are actually stored in globalGroupMap.
  for (QValueListIterator<GroupElement*> it = m_groupElements.begin(); it != m_groupElements.end(); ++it)
  {
    GroupElement *groupElement = (*it);
    groupElement->node = 0L;
    groupElement->deleted = true;
    groupElement->group = 0L;
#ifdef DEBUG_PARSER
    kDebug(24001) << "GroupElement scheduled for deletion: " << groupElement << " "<< groupElement->tag->area().bLine << " " << groupElement->tag->area().bCol << " "<< groupElement->tag->area().eLine << " "<< groupElement->tag->area().eCol << " " << groupElement->tag->tagStr() << " " << groupElement->type << endl;
#endif
    count++;
  }
#ifdef DEBUG_PARSER
  if (count > 0)
      kDebug(24001) << count << " GroupElement scheduled for deletion. " << &m_groupElements << endl;
#endif

  m_groupElements.clear();
  //kDebug(24000) << m_groupElements.count() << " " << this << endl;
}*/

Node *Node::nodeAt(Node *base, int line, int col, bool findDeepest, bool exact)
{
  Node *node = base;
  int bl, bc, el, ec;
  int result;

  while (node)
  {
    node->tag->beginPos(bl, bc);
   // ++bc;
    result = 0;
    Node *n = node->nextNotChild();
    if (n && n->tag)
    {
      n->tag->beginPos(el, ec);
      ec--;
      result = AreaStruct::isBetween(line, col, bl, bc, el, ec);
    }
    //the cursor is between the beginning of the node and the beginning of the next, not child node
    if (result == 0)
    {
      if (node->child)
      {
        node = node->child;
      } else
      {
        if (node->parent)
        {
          int parentEl, parentEc;
          node->parent->tag->endPos(parentEl, parentEc);
          if (!exact && AreaStruct::isBetween(line, col, bl, bc, parentEl, parentEc) == 0)
          {
            node = node->parent;
          }
        }
        break; //we found the node
      }
    } else
    if (result == -1) //the coordinates are before the currently tested node
    {
      if (node->parent)
          node = node->parent;
      break; //we found the node
    } else //the coordinates are after the beginning of the next node, so move forward
    {
       node = node->next;
    }
  }

  bc = ec = el = bl = 0;
  if (node)
  {
    node->tag->beginPos(bl, bc);
    node->tag->endPos(el, ec);
  }
  if (node && node->tag->isType(Tag::Empty) &&
     (findDeepest && (bl == el && ec < bc)) )
  {
     if (node->parent)
     {
       node = node->parent;
     } else
     if (node->prev)
     {
       node = node->prev;
     }
  } else
  if (node && (el < line || (el == line && ec + 1 < col))) //the cursor is after the found node
  {
    Node *n = node->nextSibling();
    if (n /*&& n->nextSibling()*/) //don't set it to the last, always empty node
    {
      n->tag->beginPos(bl, bc);
      n->tag->endPos(el, ec);
      if (bl < line || (bl == line && bc < col)) //and it is inside the next sibling
          node = n;
    }
  }
  if (!node)
    node = base;
  return node;
}


/** Print the doc structure tree to the standard output.
    Only for debugging purposes. */
void Node::coutTree(Node *node, int indent)
{
  QString output;
  int bLine, bCol, eLine, eCol;
  if (!node)
      kDebug(24000)<< "Node::coutTree() - bad node!" << endl;
  while (node)
  {
    output = "";
    output.fill('.', indent);
    node->tag->beginPos(bLine, bCol);
    node->tag->endPos(eLine, eCol);
    if (!node->tag->isType(Tag::Text))
      output += node->tag->name().replace('\n'," ");
    else
      output+= node->tag->tagStr().replace('\n'," ");
    kDebug(24000) << output <<" (" << node->tag->type() << ") at pos " <<
            bLine << ":" << bCol << " - " << eLine << ":" << eCol << " This: "<< node << " Parent: " << node->parent << " Prev: " << node->prev << " Next: " << node->next << " Child: " << node->child << endl;
/*           for(j = 0; j < node->tag->attrCount(); j++)
            {
                    kDebug(24000)<< " attr" << j << " " <<
                            node->tag->getAttribute(j).nameLine << ":" <<
                            node->tag->getAttribute(j).nameCol << " - " <<
                            node->tag->getAttribute(j).valueLine << ":" <<
                            node->tag->getAttribute(j).valueCol << endl;
            }
*/
    if (node->child)
      coutTree(node->child, indent + 4);
    node = node->next;
  }
}


AreaStruct Node::tagArea(bool excludeClosing) const
{
  AreaStruct area;
  if (! tag)
    return area;

  area = tag->area();
  if (! tag->single && next)
  {
    AreaStruct endArea;
    if (tag->closingMissing && child)
    {
      Node *childNode = child;
      while (childNode->child || childNode->next)
      {
        if (childNode->next)
        {
          childNode = childNode->next;
        } else
        {
          childNode = childNode->child;
        }
      }
      endArea = childNode->tag->area();
    } else
    {
      endArea = next->tag->area();
    }
    if (excludeClosing)
      area.end = endArea.start;
    else
      area.end = endArea.end;
  }
  return area;
}
