/***************************************************************************
                             tagattributetree.cpp
                             ---------------------
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

//qt includes
#include <qfont.h>
#include <qpainter.h>
#include <qtimer.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>

//kde includes
#include <kaction.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kstringhandler.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <khtmlview.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/view.h>
#include <dom/dom_node.h>

//app includes
#include "tagattributetree.h"
#include "tagattributeitems.h"
#include "qtag.h"
#include "node.h"
#include "parser.h"
#include "quantacommon.h"
#include "document.h"
#include "quantaview.h"
#include "tag.h"
#include "wkafkapart.h"
#include "kafkacommon.h"
#include "undoredo.h"

#include "viewmanager.h"

EditableTree::EditableTree(QWidget *parent, const char *name)
: KListView(parent, name)
{
  m_editable = true;
}

EditableTree::~EditableTree()
{

}

void EditableTree::setCurrentItem( QListViewItem *item)
{
  if ( item && m_editable)
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

void EditableTree::editorContentChanged()
{

}

void EditableTree::focusInEvent(QFocusEvent *)
{
  /**QListViewItem *it = currentItem();
  if( dynamic_cast<AttributeItem*>(it))
  {
    static_cast<AttributeItem *>(it)->showEditor();
    static_cast<AttributeItem *>(it)->lin->setFocus();
  }*/
}

void EditableTree::focusOutEvent(QFocusEvent *)
{
  /**QListViewItem *it = currentItem();
  if( dynamic_cast<AttributeItem*>(it))
  {
    static_cast<AttributeItem *>(it)->hideEditor();
  }*/
}

DualEditableTree::DualEditableTree(QWidget *parent, const char *name)
: EditableTree(parent, name)
{
  curCol = 0;
  setFocusPolicy(QWidget::ClickFocus);
  this->installEventFilter(this);
  connect(this, SIGNAL(clicked(QListViewItem *, const QPoint &, int )),
    this, SLOT(itemClicked(QListViewItem *, const QPoint &, int )));
}

DualEditableTree::~DualEditableTree()
{

}

bool DualEditableTree::eventFilter(QObject *object, QEvent *event)
{
  AttributeItem *it = dynamic_cast<AttributeItem*>(currentItem());
  AttributeItem *up = 0L, *down = 0L;
  if(!it)
    return KListView::eventFilter(object, event);
  if(currentItem()->itemAbove())
    up = dynamic_cast<AttributeItem*>(currentItem()->itemAbove());
  if(currentItem()->itemBelow())
    down = dynamic_cast<AttributeItem *>(currentItem()->itemBelow());

  if(event->type() == QEvent::KeyPress && m_editable)
  {
    QKeyEvent *keyevent = static_cast<QKeyEvent *>(event);
    switch(keyevent->key())
    {
      case Key_Left:
      if(curCol == 1 && it->lin->cursorPosition() == 0 )
      {
        it->hideEditor(1);
        it->showEditor(0);
        it->lin2->setFocus();
        curCol = 0;
      }
      break;

      case Key_Right:
      if(curCol == 0 && (unsigned)it->lin2->cursorPosition() == it->lin2->text().length())
      {
        it->hideEditor(0);
        it->showEditor(1);
        it->lin->setFocus();
        curCol = 1;
      }
      break;

      case Key_Up:
      if(up)
      {
        it->hideEditor(curCol);
        up->showEditor(curCol);
      }
      break;

      case Key_Down:
      if(down)
      {
        it->hideEditor(curCol);
        down->showEditor(curCol);
      }
      break;
    }
  }
  return KListView::eventFilter(object, event);;
}

void DualEditableTree::resizeEvent(QResizeEvent *ev)
{
  KListView::resizeEvent(ev);
  if(!currentItem()) return;
  AttributeItem *item = dynamic_cast<AttributeItem*>(currentItem());
  if(item)
  {
    item->hideEditor(curCol);
    item->showEditor(curCol);
  }
}

void DualEditableTree::setCurrentItem(QListViewItem *item)
{
  if ( item && m_editable)
  {
    QListViewItem *it = currentItem();
    if ( dynamic_cast<AttributeItem*>(it) )
    {
         static_cast<AttributeItem*>(it)->hideEditor(0);
         static_cast<AttributeItem*>(it)->hideEditor(1);
    }

    KListView::setCurrentItem(item);
    it = currentItem();
    if ( dynamic_cast<AttributeItem*>(it) )
         static_cast<AttributeItem*>(it)->showEditor(curCol);
  }
}

void DualEditableTree::editorContentChanged()
{
  emit itemModified(dynamic_cast<AttributeItem*>(currentItem()));
}

void DualEditableTree::itemClicked(QListViewItem *item, const QPoint &, int column)
{
  if(item)
  {
    curCol = column;
    if(item == currentItem())
      setCurrentItem(item);
  }
}

TagAttributeTree::TagAttributeTree(QWidget *parent, const char *name)
: EditableTree(parent, name)
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
  if (m_node == node)
    return;
  m_node = node;
  emit newNodeSelected(node);
  if (!rebuildEnabled)
      return;
  clear();
  m_parentItem = 0L;
  //We don't want to be able to edit the text node but it's parent.
  if (node && node->tag->type == Tag::Text)
    m_node = node = node->parent;
  if (!node)
      return;
#ifdef HEAVY_DEBUG
  kafkaCommon::coutTree(baseNode, 2);
  KafkaDocument::ref()->coutLinkTree(baseNode, 2);
#endif
  AttributeItem *item = 0L;
  TopLevelItem *group = 0L;
  QString attrName;
  QTag *qTag = QuantaCommon::tagFromDTD(node);
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

  if(node->tag->type == Tag::XmlTag || node->tag->type == Tag::XmlTagEnd)
  {
    QString nameSpace = node->tag->nameSpace;
    if (node->tag->type == Tag::XmlTagEnd)
     nameSpace.remove('/');
    group = new TopLevelItem(this, group, i18n("Namespace"));
    item = new AttributeNameSpaceItem(this, group, i18n("prefix"), nameSpace);
    group->setOpen(true);
  }
   if (qTag)
  {
    group = new TopLevelItem(this, group, i18n("Attributes"));
    QStringList list;
    for (int i = 0; i < qTag->attributeCount(); i++)
    {
      list += qTag->attributeAt(i)->name;
    }
    list.sort();
    QStringList::Iterator it = list.end();
    --it;
    while (it != list.end())
    {
      Attribute *attr = qTag->attribute(*it);
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
      if (it != list.begin())
        --it;
      else
        break;
    }
    group->setOpen(true);
    for (uint i = 0; i < qTag->commonGroups.count(); i++)
    {
      group = new TopLevelItem(this, group, i18n(qTag->commonGroups[i].utf8()));
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
          item = new AttributeListItem(this, group, attr->name, node->tag->attributeValue(attr->name), attr);
        } else
        if (attr->type == "color")
        {
          item = new AttributeColorItem(this, group, attr->name, node->tag->attributeValue(attr->name));
        } else
        if (attr->type == "css-style")
        {
          item = new AttributeStyleItem(this, group, attr->name, node->tag->attributeValue(attr->name));
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
      QString nameSpace = item->editorText();
      m_node->tag->write()->changeTagNamespace(m_node->tag, nameSpace);
    } else
    {
    if(ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::SourceFocus)
    {
      m_node->tag->write()->changeTagAttribute(m_node->tag, item->text(0), item->editorText());
    }
    else
    {
      //edit the attribute
      NodeModifsSet *modifs = new NodeModifsSet();
      kafkaCommon::editNodeAttribute(m_node, item->text(0), item->editorText(), modifs);
      ViewManager::ref()->activeDocument()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);

#ifdef HEAVY_DEBUG
        kafkaCommon::coutTree(baseNode, 2);
#endif
      }
    }
    rebuildEnabled = true;
  }
}

/**void TagAttributeTree::setCurrentItem( QListViewItem *item )
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
}*/

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
  if (ViewManager::ref()->activeDocument())
  {
    if (ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::SourceFocus)
      ViewManager::ref()->activeDocument()->view()->setFocus();
    else
      KafkaDocument::ref()->getKafkaWidget()->view()->setFocus();
  }
}

EnhancedTagAttributeTree::EnhancedTagAttributeTree(QWidget *parent, const char *name)
: QWidget(parent, name)
{

  widgetLayout = new QGridLayout( this, 1, 1, 11, 6, "MainLayout");

  attrTree = new TagAttributeTree(this, "TagAttributeTree");
  attrTree->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
  widgetLayout->addMultiCellWidget( attrTree, 1, 1, 0, 3 );

  nodeName = new QLabel(this, i18n( "Node Name" ));
  nodeName->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed, 0, 0, nodeName->sizePolicy().hasHeightForWidth() ) );

  widgetLayout->addWidget( nodeName, 0, 0 );
  deleteTag = new KPushButton(this, i18n( "Delete Tag" ));
  deleteTag->setPixmap(SmallIcon("editdelete"));
  deleteTag->setMaximumHeight(32);
  deleteTag->setMaximumWidth(32);
  QToolTip::add(deleteTag, i18n("Delete the current tag only."));

  deleteAll = new KPushButton(this, i18n( "Delete All" ));
  deleteAll->setPixmap(SmallIcon("editdelete"));
  deleteAll->setMaximumHeight(32);
  deleteAll->setMaximumWidth(32);
  QToolTip::add(deleteAll, i18n("Delete the current tag and all its children."));

  widgetLayout->addWidget( deleteTag, 0, 2 );
  widgetLayout->addWidget( deleteAll, 0, 3 );
  clearWState( WState_Polished );

  connect(attrTree, SIGNAL(newNodeSelected(Node *)), this, SLOT(NodeSelected(Node *)));
  connect(deleteTag, SIGNAL(clicked()), this, SLOT(deleteNode()));
  connect(deleteAll, SIGNAL(clicked()), this, SLOT(deleteSubTree()));
}

EnhancedTagAttributeTree::~EnhancedTagAttributeTree()
{

}

void EnhancedTagAttributeTree::setCurrentNode(Node *node)
{
  curNode = node;
  attrTree->setCurrentNode(node);
  showCaption();
}

void EnhancedTagAttributeTree::NodeSelected(Node *node)
{
  curNode = node;
  //We don't want to be able to edit the text node but it's parent.
  if (node && node->tag->type == Tag::Text)
    curNode = node = node->parent;
  showCaption();
  emit newNodeSelected(node);
}

void EnhancedTagAttributeTree::showCaption()
{
  if(curNode)
  {
    if(curNode->tag->type == Tag::XmlTag || curNode->tag->type == Tag::XmlTagEnd ||
      curNode->tag->type == Tag::ScriptTag)
    {
      QString s = i18n("Current tag: <b>%1</b>").arg(curNode->tag->name);
      nodeName->setText(KStringHandler::rPixelSqueeze(s, nodeName->fontMetrics(), attrTree->width()- 50));
    }
    else if(curNode->tag->type == Tag::Text)
      nodeName->setText(i18n("Current tag: <b>text</b>"));
    else if(curNode->tag->type == Tag::Comment)
      nodeName->setText(i18n("Current tag: <b>comment</b>"));
    else
      nodeName->setText(i18n("Current tag:"));
  }
}

void EnhancedTagAttributeTree::deleteSubTree()
{
  QuantaView *view = ViewManager::ref()->activeView();
  if(!curNode || !view->document())
    return;
  Node *oldCurNode;
  NodeModifsSet *modifs;
  int curLine, curCol;
  long offset;
  DOM::Node domNode;
  QValueList<int> loc;

  //Save the cursor position in kafka/quanta
  if(view->hadLastFocus() == QuantaView::SourceFocus)
    curNode->tag->beginPos(curLine, curCol);
  else
  {
    KafkaDocument::ref()->getKafkaWidget()->getCurrentNode(domNode, offset);
    if(!domNode.previousSibling().isNull())
      domNode = domNode.previousSibling();
    else if(!domNode.parentNode().isNull())
      domNode = domNode.parentNode();
    else
      domNode = KafkaDocument::ref()->getKafkaWidget()->document();
    if(domNode.nodeType() == DOM::Node::TEXT_NODE)
      offset = domNode.nodeValue().length();
    else
     offset = 0;
    loc = kafkaCommon::getLocation(domNode);
  }

  //Remove the Nodes
  oldCurNode = curNode;
  curNode = 0L;
  attrTree->setCurrentNode(curNode);

  modifs = new NodeModifsSet();
  kafkaCommon::extractAndDeleteNode(oldCurNode, modifs);

  view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);

  //set the cursor position in kafka/quanta
  if(view->hadLastFocus() == QuantaView::SourceFocus)
    view->document()->viewCursorIf->setCursorPositionReal((uint)curLine, (uint)curCol);
  else
  {
    domNode = kafkaCommon::getNodeFromLocation(loc,
    KafkaDocument::ref()->getKafkaWidget()->document());
    KafkaDocument::ref()->getKafkaWidget()->setCurrentNode(domNode, offset);
  }
}

void EnhancedTagAttributeTree::deleteNode()
{
  QuantaView *view = ViewManager::ref()->activeView();
  if(!curNode || !view->document())
    return;

  Node *oldCurNode, *oldCurNodeParent, *child;
  QTag *oldCurNodeParentQTag;
  int curLine, curCol;
  long offset;
  DOM::Node domNode;
  QValueList<int> loc;
  NodeModifsSet *modifs;

  //Save the cursor position in kafka/quanta
  if(view->hadLastFocus() == QuantaView::SourceFocus)
    curNode->tag->beginPos(curLine, curCol);
  else
  {
    KafkaDocument::ref()->getKafkaWidget()->getCurrentNode(domNode, offset);
    if(!domNode.previousSibling().isNull())
      domNode = domNode.previousSibling();
    else if(!domNode.parentNode().isNull())
      domNode = domNode.parentNode();
    else
      domNode = KafkaDocument::ref()->getKafkaWidget()->document();
    if(domNode.nodeType() == DOM::Node::TEXT_NODE)
      offset = domNode.nodeValue().length();
    else
     offset = 0;
    loc = kafkaCommon::getLocation(domNode);
  }

  //remove the Nodes
  oldCurNode = curNode;
  oldCurNodeParent = curNode->parent;
  curNode = 0L;
  attrTree->setCurrentNode(curNode);

  modifs = new NodeModifsSet();
  kafkaCommon::extractAndDeleteNode(oldCurNode, modifs, false);

  //Then we see if the new parent - child relationships are valid, and if not, delete the child and restart
  if(oldCurNodeParent)
  {
    oldCurNodeParentQTag = QuantaCommon::tagFromDTD(oldCurNodeParent);
    if(oldCurNodeParentQTag)
    {
      child = oldCurNodeParent->child;
      while(child)
      {
        if(!oldCurNodeParentQTag->isChild(child))
        {
          kafkaCommon::extractAndDeleteNode(child, modifs, false);
          //too lazy to get the real next node ;-)
          child = oldCurNodeParent->child;
        }
        else
          child = child->next;
      }
    }
  }

  view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);

  //set the cursor position in kafka/quanta
  if(view->hadLastFocus() == QuantaView::SourceFocus)
    view->document()->viewCursorIf->setCursorPositionReal((uint)curLine, (uint)curCol);
  else
  {
    domNode = kafkaCommon::getNodeFromLocation(loc,
    KafkaDocument::ref()->getKafkaWidget()->document());
    KafkaDocument::ref()->getKafkaWidget()->setCurrentNode(domNode, offset);
  }

}

#include "tagattributetree.moc"
