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
#include <qlineedit.h>

//kde includes
//#include <klistview.h>
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
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <dom/dom_node.h>
#include <kdebug.h>
#include <kpushbutton.h>
#include <kaction.h>
#include <kiconloader.h>
#include "../resource.h"
#include "../quanta.h"
#include "../parser/tag.h"
#include "../parser/node.h"
#include "../parts/kafka/wkafkapart.h"
#include "../parts/kafka/kafkacommon.h"
#include "../parts/kafka/undoredo.h"
#endif

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

void DualEditableTree::resizeEvent(QResizeEvent *)
{
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
        quantaApp->view()->write()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);
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
}

EnhancedTagAttributeTree::EnhancedTagAttributeTree(QWidget *parent, const char *name)
: QWidget(parent, name)
{
#ifdef BUILD_KAFKAPART
  mainLayout = new QVBoxLayout(this, 11, 6, "Main Layout");
  topLayout = new QHBoxLayout(0, 0, 6, "Top Layout");

  nodeName = new QLabel(this, "Node Name");
  /**nodeName->setSizePolicy(QSizePolicy((QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, 0, 0,
    nodeName->sizePolicy().hasHeightForWidth()));*/
  topLayout->addWidget(nodeName);
  /**QSpacerItem *spacer = new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  topLayout->addItem(spacer);*/

  deleteTag = new KPushButton(this, "Delete Tag");
  deleteTag->setPixmap(SmallIcon("editdelete"));
  deleteTag->setMaximumHeight(32);
  deleteTag->setMaximumWidth(32);
  QToolTip::add(deleteTag, i18n("Delete the current tag only."));
  topLayout->addWidget(deleteTag);

  deleteAll = new KPushButton(this, "Delete All");
  deleteAll->setPixmap(SmallIcon("editdelete"));
  deleteAll->setMaximumHeight(32);
  deleteAll->setMaximumWidth(32);
  QToolTip::add(deleteAll, i18n("Delete the current tag and all its children."));
  topLayout->addWidget(deleteAll);

  mainLayout->addLayout(topLayout);

  attrTree = new TagAttributeTree(this, "TagAttributeTree");
  mainLayout->addWidget(attrTree);
  //resize( QSize(100, 100).expandedTo(minimumSizeHint()) );


  connect(attrTree, SIGNAL(newNodeSelected(Node *)), this, SLOT(NodeSelected(Node *)));
  connect(deleteTag, SIGNAL(clicked()), this, SLOT(deleteNode()));
  connect(deleteAll, SIGNAL(clicked()), this, SLOT(deleteSubTree()));
#endif
}

EnhancedTagAttributeTree::~EnhancedTagAttributeTree()
{

}

void EnhancedTagAttributeTree::setCurrentNode(Node *node)
{
#ifdef BUILD_KAFKAPART
  curNode = node;
  attrTree->setCurrentNode(node);
  showCaption();
  #endif
}

void EnhancedTagAttributeTree::NodeSelected(Node *node)
{
#ifdef BUILD_KAFKAPART
  curNode = node;
  showCaption();
  emit newNodeSelected(node);
#endif
}

void EnhancedTagAttributeTree::showCaption()
{
#ifdef BUILD_KAFKAPART
  if(curNode)
  {
    if(curNode->tag->type == Tag::XmlTag || curNode->tag->type == Tag::XmlTagEnd ||
      curNode->tag->type == Tag::ScriptTag)
      nodeName->setText(QString(i18n("Current Tag : <b>%1</b>")).arg(curNode->tag->name));
    else if(curNode->tag->type == Tag::Text)
      nodeName->setText(i18n("Current Tag : <b>text</b>"));
    else if(curNode->tag->type == Tag::Comment)
      nodeName->setText(i18n("Current Tag : <b>comment</b>"));
    else
      nodeName->setText(i18n("Current Tag :"));
  }
 #endif
}

void EnhancedTagAttributeTree::deleteSubTree()
{
#ifdef BUILD_KAFKAPART
  Node *oldCurNode;
  NodeModifsSet modifs;
  modifs.cursorX = 0;
  modifs.cursorY = 0;
  modifs.cursorX2 = 0;
  modifs.cursorY2 = 0;
  modifs.isModified = true;//TODO

  oldCurNode = curNode;
  /**if(curNode)
    curNode = curNode->parent;
  else*/
    curNode = 0L;
  attrTree->setCurrentNode(curNode);
  kafkaCommon::extractAndDeleteNode(oldCurNode, modifs);

  quantaApp->view()->write()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);
  //quantaApp->view()->reloadBothViews();
  #endif
}

void EnhancedTagAttributeTree::deleteNode()
{
#ifdef BUILD_KAFKAPART
  Node *oldCurNode;
  NodeModifsSet modifs;
  modifs.cursorX = 0;
  modifs.cursorY = 0;
  modifs.cursorX2 = 0;
  modifs.cursorY2 = 0;
  modifs.isModified = true;//TODO

  oldCurNode = curNode;
  /**if(curNode && curNode->child)
    curNode = curNode->child;
  else if(curNode)
    curNode = curNode->parent;
  else*/
    curNode = 0L;
  attrTree->setCurrentNode(curNode);
  kafkaCommon::extractAndDeleteNode(oldCurNode, modifs, false);

  quantaApp->view()->write()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);
  //quantaApp->view()->reloadBothViews();
  #endif
}

#include "tagattributetree.moc"
