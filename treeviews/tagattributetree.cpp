//
//
// C++ Implementation: cpp
//
// Description:
//
//
// Author: Andras Mantia <amantia@freemail.hu>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//


//qt includes
#include <qfont.h>
#include <qpainter.h>
#include <qtimer.h>

//kde includes
#include <klistview.h>
#include <klocale.h>

//app includes
#include "tagattributetree.h"
#include "tagattributeitems.h"
#include "../parser/qtag.h"
#include "../parser/node.h"
#include "../parser/parser.h"
#include "../quantacommon.h"
#include "../document.h"
#include "../quantaview.h"
#ifdef BUILD_KAFKAPART
#include "../resource.h"
#include "../quanta.h"
#include <dom/dom_node.h>
#include "../parser/tag.h"
#include "../parser/node.h"
#include "../parts/kafka/wkafkapart.h"
#include "../parts/kafka/kafkacommon.h"
#include "../parts/kafka/undoredo.h"
#endif

TagAttributeTree::TagAttributeTree(QWidget *parent, const char *name)
:KListView(parent, name)
{
  setRootIsDecorated( true );
  setSorting(-1);
  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  setFocusPolicy(QWidget::ClickFocus);
  addColumn(i18n("Attribute Name"));
  addColumn(i18n("Value"));
  setResizeMode(QListView::LastColumn);
  m_node = 0L;
  m_newNode = 0L;
  m_parentItem = 0L;
  rebuildEnabled = true;
}

TagAttributeTree::~TagAttributeTree()
{
}

void TagAttributeTree::setCurrentNode(Node *node)
{
  if(m_node == node)
    return;
  m_node = node;
  emit newNodeSelected(node);
  if (!rebuildEnabled)
      return;
  clear();
  m_parentItem = 0L;
  if (!node)
      return;
#ifdef BUILD_KAFKAPART
#ifdef HEAVY_DEBUG
  kafkaCommon::coutTree(baseNode, 2);
#endif
#endif
  AttributeItem *item = 0L;
  TopLevelItem *group = 0L;
  QString attrName;
  QTag *qTag = QuantaCommon::tagFromDTD(node->tag->dtd, node->tag->name);
  Node *n = node->parent;
  while (n)
  {
    if (n->tag->type == Tag::XmlTag)
    {
      if (!m_parentItem)
      {
        group = new TopLevelItem(this, 0L, i18n("Parent tags"));
        m_parentItem = new ParentItem(this, group);
      }
      m_parentItem->addNode(n);
    }
    n = n->parent;
  }
  if (m_parentItem)
      m_parentItem->showList(true);
  if (group)
     group->setOpen(true);
//  if (!node->tag->nameSpace.isEmpty())
  {
    group = new TopLevelItem(this, group, i18n("Namespace"));
    item = new AttributeNameSpaceItem(this, group, i18n("name"), node->tag->nameSpace);
    group->setOpen(true);
  }
  if (qTag)
  {
    group = new TopLevelItem(this, group, i18n("Attributes"));
    for (int i = 0; i < qTag->attributeCount(); i++)
    {
      Attribute *attr = qTag->attributeAt(i);
      if (attr->type == "check")
      {
        item = new AttributeBoolItem(this, group, attr->name, node->tag->attributeValue(attr->name));
      } else
      if (attr->type == "url")
      {
        item = new AttributeUrlItem(this, group, attr->name, node->tag->attributeValue(attr->name));
      } else
      if (attr->type == "list")
      {
        item = new AttributeListItem(this, group, attr->name, node->tag->attributeValue(attr->name));
      } else
      if (attr->type == "color")
      {
        item = new AttributeColorItem(this, group, attr->name, node->tag->attributeValue(attr->name));
      } else
      {
        item = new AttributeItem(this, group, attr->name, node->tag->attributeValue(attr->name));
      }
      item->setRenameEnabled(1, true);
    }
    group->setOpen(true);
    for (uint i = 0; i < qTag->commonGroups.count(); i++)
    {
      group = new TopLevelItem(this, group, i18n(qTag->commonGroups[i]));
      AttributeList *groupAttrs = qTag->parentDTD->commonAttrs->find(qTag->commonGroups[i]);
      for (uint j = 0; j < groupAttrs->count(); j++)
      {
        Attribute *attr = groupAttrs->at(j);
        attrName = attr->name;
        if (attr->type == "check")
        {
          item = new AttributeBoolItem(this, group, attrName, node->tag->attributeValue(attrName));
        } else
        if (attr->type == "url")
        {
          item = new AttributeUrlItem(this, group, attr->name, node->tag->attributeValue(attr->name));
        } else
        if (attr->type == "list")
        {
          item = new AttributeListItem(this, group, attr->name, node->tag->attributeValue(attr->name));
        } else
        if (attr->type == "color")
        {
          item = new AttributeColorItem(this, group, attr->name, node->tag->attributeValue(attr->name));
        } else
        {
          item = new AttributeItem(this, group, attrName, node->tag->attributeValue(attrName));
        }
        item->setRenameEnabled(1, true);
      }
      group->setOpen(true);
    }

  }
  connect(this, SIGNAL(collapsed(QListViewItem*)), SLOT(slotCollapsed(QListViewItem*)));
  connect(this, SIGNAL(expanded(QListViewItem*)), SLOT(slotExpanded(QListViewItem*)));
}

void TagAttributeTree::editorContentChanged()
{
  AttributeItem *item = dynamic_cast<AttributeItem*>(currentItem());
  if (m_node && item )
  {
    rebuildEnabled = false;
    if (dynamic_cast<AttributeNameSpaceItem*>(item))
    {
      m_node->tag->write()->changeTagNamespace(m_node->tag, item->editorText());
    } else
    {
#ifdef BUILD_KAFKAPART
    if(quantaApp->view()->hadLastFocus() == QuantaView::quantaFocus)
    {
#endif
      m_node->tag->write()->changeTagAttribute(m_node->tag, item->text(0), item->editorText());
#ifdef BUILD_KAFKAPART
    }
    else
    {
      //edit only the tag and its corresponding DOM::Node
      int i;
      TagAttr attr;
      DOM::Node domNode;
      bool foundAttr = false, nodeModified= false;
      NodeModifsSet modifs;
      NodeModif modif;
      modifs.cursorX = 0;
      modifs.cursorY = 0;
      modifs.cursorX2 = 0;
      modifs.cursorY2 = 0;
      modifs.isModified = true;//TODO:determine this
      modif.type = undoRedo::NodeModified;
      modif.tag = new Tag(*(m_node->tag));
      modif.location = kafkaCommon::getLocation(m_node);
      modif.node = 0L;

      for(i = 0; i < m_node->tag->attrCount(); i++)
      {
        if(m_node->tag->attribute(i) == item->text(0))
        {
          foundAttr = true;
#ifdef HEAVY_DEBUG
          kdDebug(25001)<< "Attribute " << item->text(0) << " found!" << endl;
#endif
          if(m_node->tag->attributeValue(i) != item->editorText())
          {
#ifdef HEAVY_DEBUG
          kdDebug(25001)<< "Different attribute value " << item->editorText() << ", deleting the attr." << endl;
#endif
            //delete the attribute in the Node
            attr = m_node->tag->getAttribute((unsigned)i);
            m_node->tag->deleteAttribute((unsigned)i);

            m_node->tag->cleanStrBuilt = false;
            nodeModified = true;
          }
          break;
        }
      }
      if((!foundAttr && item->editorText() != "") ||
        (foundAttr && item->editorText() != m_node->tag->attributeValue(i) && item->editorText() != ""))
      {
#ifdef HEAVY_DEBUG
         kdDebug(25001)<< "(re)creating the attr." << endl;
#endif
         //(re)create the attr.
         attr.name = item->text(0);
         attr.value = item->editorText();
         attr.quoted = true;
         m_node->tag->addAttribute(attr);

         m_node->tag->cleanStrBuilt = false;
         nodeModified = true;
      }
      if(nodeModified)
      {
        //delete the corresponding DOM::Node.
        domNode = m_node->_rootNode;
        quantaApp->view()->getKafkaInterface()->disconnectDomNodeFromQuantaNode(domNode);
        if(!domNode.isNull())
          domNode.parentNode().removeChild(domNode);
        if(m_node->_rootNode != m_node->_leafNode)
        {
          domNode = m_node->_leafNode;
          quantaApp->view()->getKafkaInterface()->disconnectDomNodeFromQuantaNode(domNode);
          if(!domNode.isNull())
            domNode.parentNode().removeChild(domNode);
        }
        m_node->_rootNode = 0L;
        m_node->_leafNode = 0L;

        quantaApp->view()->getKafkaInterface()->buildKafkaNodeFromNode(m_node, true);
        if(!domNode.isNull() && !m_node->_leafNode.isNull())
        {
           while(!domNode.firstChild().isNull())
             m_node->_leafNode.appendChild(domNode.firstChild());
        }

        modifs.NodeModifList.append(modif);
        quantaApp->view()->write()->docUndoRedo->addNewModifsSet(modifs, true);
      }
#ifdef HEAVY_DEBUG
        kafkaCommon::coutTree(baseNode, 2);
#endif
      }
#endif
    }
    rebuildEnabled = true;
  }
}

void TagAttributeTree::setCurrentItem( QListViewItem *item )
{
  if ( item )
  {
    QListViewItem *it = currentItem();
    if ( dynamic_cast<AttributeItem*>(it) )
         static_cast<AttributeItem*>(it)->hideEditor();

    KListView::setCurrentItem(item);
    it = currentItem();
    if ( dynamic_cast<AttributeItem*>(it) )
         static_cast<AttributeItem*>(it)->showEditor();
  }
}

void TagAttributeTree::slotParentSelected(int index)
{
  if (m_parentItem)
  {
    m_newNode = m_parentItem->node(index);
    QTimer::singleShot(0, this, SLOT(slotDelayedSetCurrentNode()));
  }
}

void TagAttributeTree::slotCollapsed(QListViewItem *item)
{
  if (m_parentItem && item == m_parentItem->parent())
      m_parentItem->showList(false);
}

void TagAttributeTree::slotExpanded(QListViewItem *item)
{
  if (m_parentItem && item == m_parentItem->parent())
      m_parentItem->showList(true);
}

void TagAttributeTree::slotDelayedSetCurrentNode()
{
  setCurrentNode(m_newNode);
}

#include "tagattributetree.moc"
