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
  rebuildEnabled = true;
}

TagAttributeTree::~TagAttributeTree()
{
}

void TagAttributeTree::setCurrentNode(Node *node)
{
  m_node = node;
  emit newNodeSelected(node);
  if (!rebuildEnabled)
      return;
  clear();
  AttributeItem *item = 0L;
  TopLevelItem *group;
  QString attrName;
  QTag *qTag = QuantaCommon::tagFromDTD(node->tag->dtd, node->tag->name);
  group = new TopLevelItem(this, i18n("Parent tags"));
  ParentItem *it = 0L;
  Node *n = node->parent;
  while (n)
  {
    if (n->tag->type == Tag::XmlTag)
        it = new ParentItem(group, it, n);
    n = n->parent;
  }
  group->setOpen(true);
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
}

void TagAttributeTree::editorContentChanged()
{
  AttributeItem *item = dynamic_cast<AttributeItem*>(currentItem());
  if (m_node && item )
  {
    rebuildEnabled = false;
    m_node->tag->write()->changeTagAttribute(m_node->tag, item->text(0), item->editorText());
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
    else
    if (dynamic_cast<ParentItem*>(it) )
        setCurrentNode(static_cast<ParentItem*>(it)->node());
  }
}


#include "tagattributetree.moc"
