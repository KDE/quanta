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
#include "../parser/qtag.h"
#include "../parser/node.h"
#include "../parser/parser.h"
#include "../quantacommon.h"
#include "../document.h"

TagAttributeTree::TagAttributeTree(QWidget *parent, const char *name)
:KListView(parent, name)
{
  addColumn(i18n("Attribute name"));
  addColumn(i18n("Value"));
  setResizeMode(QListView::AllColumns);
  setRootIsDecorated(true);
  setItemsRenameable(true);
  setRenameable(0, false);
  setRenameable(1, true);
  m_node = 0L;
  connect(this, SIGNAL(returnPressed(QListViewItem*)), SLOT(slotItemRenamed(QListViewItem*)));
}

TagAttributeTree::~TagAttributeTree()
{
}

void TagAttributeTree::newCursorPosition(Node *node)
{
  m_node = node;
  clear();
  KListViewItem *item;
  QTag *qTag = QuantaCommon::tagFromDTD(node->tag->dtd, node->tag->name);
  if (qTag)
  {
    TopLevelItem *group = new TopLevelItem(this, i18n("Attributes"));
    for (int i = 0; i < qTag->attributeCount(); i++)
    {
      item = new KListViewItem(group, qTag->attributeAt(i)->name, node->tag->attributeValue(qTag->attributeAt(i)->name));
      item->setRenameEnabled(1, true);
    }
    group->setOpen(true);
    for (uint i = 0; i < qTag->commonGroups.count(); i++)
    {
      group = new TopLevelItem(this, group, i18n(qTag->commonGroups[i]));
      AttributeList *groupAttrs = qTag->parentDTD->commonAttrs->find(qTag->commonGroups[i]);
      for (uint j = 0; j < groupAttrs->count(); j++)
      {
        QString attrName = groupAttrs->at(j)->name;
        item = new KListViewItem(group, attrName, node->tag->attributeValue(attrName));
        item->setRenameEnabled(1, true);
      }
      group->setOpen(true);
    }

  }
}

void TagAttributeTree::slotItemRenamed(QListViewItem *item)
{
  if (m_node && item)
  {
    m_node->tag->write()->changeTagAttribute(m_node->tag, item->text(0), item->text(1));
  }
}


TopLevelItem::TopLevelItem(TagAttributeTree* parent, const QString &title)
: KListViewItem(parent, title)
{
}

TopLevelItem::TopLevelItem(TagAttributeTree* parent, QListViewItem* after, const QString &title)
: KListViewItem(parent, after, title)
{
}

TopLevelItem::~TopLevelItem()
{
}


void TopLevelItem::paintCell(QPainter *p, const QColorGroup &cg,
                             int column, int width, int align)
{
    if ( !p )
        return;
    // make toplevel item names bold
    if (column == 0 && !parent())
    {
        QFont f = p->font();
        f.setBold(true);
        p->setFont(f);
    }
    QListViewItem::paintCell( p, cg, column, width, align );
}


#include "tagattributetree.moc"
