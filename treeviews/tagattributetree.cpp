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

//kde includes
#include <klistview.h>
#include <klocale.h>

//app includes
#include "tagattributetree.h"
#include "../parser/qtag.h"
#include "../parser/node.h"
#include "../parser/parser.h"
#include "../quantacommon.h"

TagAttributeTree::TagAttributeTree(QWidget *parent, const char *name)
:KListView(parent, name)
{
  addColumn(i18n("Attribute name"));
  addColumn(i18n("Value"));
  setResizeMode(QListView::AllColumns);
  setRootIsDecorated(true);
}

TagAttributeTree::~TagAttributeTree()
{
}

void TagAttributeTree::newCursorPosition(Node *node)
{
  clear();
  QTag *qTag = QuantaCommon::tagFromDTD(node->tag->dtd, node->tag->name);
  if (qTag)
  {
    KListViewItem *group = new KListViewItem(this, i18n("Attributes"));
    for (int i = 0; i < qTag->attributeCount(); i++)
    {
      new KListViewItem(group, qTag->attributeAt(i)->name, node->tag->attributeValue(qTag->attributeAt(i)->name));
    }
    group->setOpen(true);
    for (uint i = 0; i < qTag->commonGroups.count(); i++)
    {
      group = new KListViewItem(this, group, i18n(qTag->commonGroups[i]));
      AttributeList *groupAttrs = qTag->parentDTD->commonAttrs->find(qTag->commonGroups[i]);
      for (uint j = 0; j < groupAttrs->count(); j++)
      {
        QString attrName = groupAttrs->at(j)->name;
        new KListViewItem(group, attrName, node->tag->attributeValue(attrName));
      }
      group->setOpen(true);
    }

  }
}

#include "tagattributetree.moc"
