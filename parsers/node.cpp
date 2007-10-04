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
#include <qdom.h>

#include <kdebug.h>

#include "node.h"
#include "tag.h"
#include "qtag.h"
#include "quantacommon.h"
#include "structtreetag.h"
#include "kafkacommon.h"

QMap<Node*, int> nodes; //list of all created nodes. Used to do some own memory management and avoid double deletes, for whatever reason they happen...

int NN = 0; //for debugging purposes: count the Node objects

GroupElementMapList globalGroupMap;

Node::Node(Node *parent)
{
  this->parent = parent;
  prev = next = child = 0L;
  tag = 0L;
  mainListItem = 0L;
  opened = false;
  removeAll = true;
  closesPrevious = false;
  insideSpecial = false;
  _closingNode = 0L;
  m_rootNode = 0L;
  m_leafNode = 0L;
  m_groupElements.clear();
  NN++;
//   if (nodes.contains(this) == 0)
  nodes[this] = 1;
//   else
//   {
//     kdError(24000) << "A node with this address " << this << " already exists!" << endl; 
//   }
}

bool Node::deleteNode(Node *node)
{
  if (!node)
    return true;
  if (!nodes.contains(node))
  {     
    kdDebug(24000) << "Trying to delete a node with address " << node << " that was not allocated!" << endl; 
    return false;
  }
  delete node;
  return true;
}

Node::~Node()
{
//   if (!nodes.contains(this))
//   {     
//     kdError(24000) << "No node with this address " << this << " was allocated!" << endl; 
//     return;
//   }
    
  //It has no use, except to know when it crash why it has crashed.
  //If it has crashed here, the Node doesn't exist anymore.
  // If it has crashed the next line, it is a GroupElements bug.
  //FIXME: Andras: or it is a VPL undo/redo bug...
  Q_ASSERT(tag); 
  if (tag)
    tag->setCleanStrBuilt(false);

  detachNode();
  nodes.erase(this);
  if (prev && prev->next == this)
      prev->next = 0L;
  if (parent && parent->child == this)
      parent->child = 0L;
  if (removeAll)
  {
    deleteNode(child);
    child = 0L;
    deleteNode(next);
    next = 0L;
  } else
  {
    if (next && next->prev == this)
      next->prev = 0L;
    if (child && child->parent == this)
      child->parent = 0L;
  }

  delete tag;
  tag = 0L;
  delete m_rootNode;
  delete m_leafNode;
  NN--;
}

void Node::save(QDomElement& element) const
{
    //kdDebug(25001) << "Save:\n" << element.ownerDocument().toString() << endl;
    QDomElement child_element;
    if(next)
    {
        child_element = element.ownerDocument().createElement("nodeNext");
        element.appendChild(child_element);
        next->save(child_element);
    }
    if(child)
    {
        child_element = element.ownerDocument().createElement("nodeChild");
        element.appendChild(child_element);
        child->save(child_element);
    }
    if(_closingNode)
    {
        if(_closingNode != next)
        {
            child_element = element.ownerDocument().createElement("nodeClosing");
            element.appendChild(child_element);
            _closingNode->save(child_element);
        }
    }

    Q_ASSERT(tag);
    child_element = element.ownerDocument().createElement("tag");
    element.appendChild(child_element);
    tag->save(child_element);

    element.setAttribute("closesPrevious", closesPrevious);           // bool
    element.setAttribute("opened", opened);     // bool
    element.setAttribute("removeAll", removeAll); // bool
    element.setAttribute("insideSpecial", insideSpecial);             // bool
    element.setAttribute("specialInsideXml", specialInsideXml);             // bool
    element.setAttribute("fileName", fileName);                         // QString

/*    QString s_element;
    QTextStream stream(&s_element, IO_WriteOnly);
    element.save(stream, 3);*/
    //kdDebug(25001) << "Load:\n" << s_element << endl;
    //kdDebug(25001) << "Save:\n" << element.ownerDocument().toString() << endl;
}

bool Node::load(QDomElement const& element)
{
/*    QString s_element;
    QTextStream stream(&s_element, IO_WriteOnly);
    element.save(stream, 3);*/
    //kdDebug(25001) << "Load:\n" << s_element << endl;

    QDomNodeList list = element.childNodes();
    for(unsigned int i = 0; i != list.count(); ++i)
    {
        if(list.item(i).isElement())
        {
            QDomElement e = list.item(i).toElement();
            if(e.tagName() == "nodeNext")
            {
                next = new Node(0);
                next->prev = this;
                next->parent = this->parent;
                next->load(e);
            }
            else if(e.tagName() == "nodeChild")
            {
                child = new Node(0);
                child->parent = this;
                child->load(e);
            }
            else if(e.tagName() == "nodeClosing")
            {
                _closingNode = new Node(0);
                _closingNode->load(e);
            }
            else if(e.tagName() == "tag")
            {
                tag = new Tag();
                tag->load(e);
            }
        }
    }

    closesPrevious = QString(element.attribute("closesPrevious")).toInt();  // bool
    opened = QString(element.attribute("opened")).toInt();     // bool
    removeAll = QString(element.attribute("removeAll")).toInt(); // bool
    insideSpecial = QString(element.attribute("insideSpecial")).toInt();    // bool
    specialInsideXml = QString(element.attribute("specialInsideXml")).toInt();  // bool
    fileName = element.attribute("fileName");                         // QString

    //kafkaCommon::coutTree(this, 3);

    return true;
}

Node *Node::nextSibling()
{
    Node *result = 0L;
    if (child)
    {
        result = child;
    }
    else
        if (next)
        {
            result = next;
        }
        else
        {
            Node *n = this;
            while (n)
            {
                if (n->parent && n->parent->next)
                {
                    result = n->parent->next;
                    break;
                }
                else
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
    }
    else
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
            }
            else
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

void Node::setNodeValue(const QString &value)
{
    if(!tag)
        tag = new Tag();
    tag->setStr(value);
    kdDebug(24000) << "Node::setNodeValue: dtd is 0L for " << value << endl;
}

Node* Node::lastChild()
{
    Node *n, *m = 0;
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
        if (parent && node->tag->type == Tag::ScriptTag)
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
        if (parent && node->tag->type == Tag::ScriptTag)
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

Node *Node::getClosingNode()
{
    Node* n = next;

    if(next && tag && (tag->type == Tag::XmlTag || tag->type == Tag::ScriptTag) && !tag->single)
    {
        while (n && n->tag->type == Tag::Empty)
            n = n->next;
        if (n && n->tag->type == Tag::XmlTagEnd && ((tag->type == Tag::XmlTag && QuantaCommon::closesTag(tag, n->tag)) || (tag->type == Tag::ScriptTag && n->tag->name.isEmpty())))
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
                 || (n->tag->type == Tag::ScriptTag && tag->name.isEmpty())))
            return n;
    }
    return 0L;
}

int Node::size()
{
    int l = tag->size();
    l += 5*sizeof(Node*) + sizeof(QListViewItem*) + 2*sizeof(Tag*) + 2*sizeof(DOM::Node);
    return l;
}

void Node::operator =(Node* node)
{
  (*this) = (*node);
  prev = 0L;
  next = 0L;
  parent = 0L;
  child = 0L;
  mainListItem = 0L;
  m_groupElements.clear();
  setRootNode(0L);
  setLeafNode(0L);
  tag = new Tag(*(node->tag));
}

void Node::detachNode()
{
  if (nodes.contains(this) == 0)
  {
    kdError(24000) << "No node with this address " << this << " was allocated!" << endl; 
    return;
  }    

  int count = 0;
  //kdDebug(24000) << &m_groupElements << " " << this << endl;
  //Remove the references to this node from the list of group elements.
  //They are actually stored in globalGroupMap.
  for (QValueListIterator<GroupElement*> it = m_groupElements.begin(); it != m_groupElements.end(); ++it)
  {
    GroupElement *groupElement = (*it);
    groupElement->node = 0L;
    groupElement->deleted = true;
    groupElement->group = 0L;
#ifdef DEBUG_PARSER
    kdDebug(24001) << "GroupElement scheduled for deletion: " << groupElement << " "<< groupElement->tag->area().bLine << " " << groupElement->tag->area().bCol << " "<< groupElement->tag->area().eLine << " "<< groupElement->tag->area().eCol << " " << groupElement->tag->tagStr() << " " << groupElement->type << endl;
#endif
    count++;
  }
#ifdef DEBUG_PARSER
  if (count > 0) 
      kdDebug(24001) << count << " GroupElement scheduled for deletion. " << &m_groupElements << endl;
#endif

  QValueListIterator<QListViewItem*> listItem;
  for ( listItem = listItems.begin(); listItem != listItems.end(); ++listItem)
  {
    static_cast<StructTreeTag*>(*listItem)->node = 0L;
    static_cast<StructTreeTag*>(*listItem)->groupTag = 0L;
  }
  mainListItem = 0L;
  listItems.clear();
  m_groupElements.clear();
  //kdDebug(24000) << m_groupElements.count() << " " << this << endl;
}
