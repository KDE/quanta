/***************************************************************************
                          structtreeview.cpp  -  description
                             -------------------
    begin                : Sat Apr 29 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2002, 2003 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// QT headers
#include <qpixmap.h>
#include <qheader.h>
#include <qregexp.h>
#include <qdatetime.h>

// KDE headers
#include <kapplication.h>
#include <kiconloader.h>
#include <kpopupmenu.h>
#include <klocale.h>
#include <kconfig.h>
#include <kdebug.h>

// app includes
#include "messageoutput.h"
#include "node.h"
#include "qtag.h"
#include "document.h"
#include "resource.h"
#include "qextfileinfo.h"
#include "quanta.h"
#include "quantaview.h"
#include "quantacommon.h"
#include "dtds.h"

#include "structtreetag.h"
#include "structtreeview.h"
#include "structtreeview.moc"
extern GroupElementMapList globalGroupMap;

StructTreeView::StructTreeView(QWidget *parent, const char *name )
: KListView(parent,name)
{
  for (int i = 0; i < 5; i++)
    groupOpened[i] = false;
  top = 0L;
  lastTag = 0L;
  groupsCount = 0;
  followCursorFlag = true;
  this->config = quantaApp->config();

  topOpened = true;
  useOpenLevelSetting = true;

  setRootIsDecorated( true );
  header()->hide();
  setSorting(-1,false);

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( i18n("Name"), -1 );

  setFocusPolicy(QWidget::ClickFocus);

  dtdMenu = new KPopupMenu(this);

  dtdList = DTDs::ref()->nickNameList();
  for(uint i = 0; i < dtdList.count(); i++ )
  {
    dtdMenu->insertItem(dtdList[i],i,-1);
  }

  connect(dtdMenu, SIGNAL(activated(int)), this, SLOT(slotDTDChanged(int)));

  popupMenu = new KPopupMenu(this);

  popupMenu -> insertItem( i18n("Show Groups For"), dtdMenu);
  popupMenu -> insertSeparator();
  popupMenu -> insertItem( i18n("Select Tag Area"), this ,SLOT(slotSelectTag()));
  popupMenu -> insertItem( i18n("Go to End of Tag"), this ,SLOT(slotGotoClosingTag()));
  openFileMenuId = popupMenu -> insertItem( i18n("Open File"), this ,SLOT(slotOpenFile()));
  popupMenu -> insertSeparator();
  popupMenu -> insertItem( i18n("Open Subtrees"), this ,SLOT(slotOpenSubTree()));
  popupMenu -> insertItem( i18n("Close Subtrees"),this ,SLOT(slotCloseSubTree()));
  popupMenu -> insertSeparator();
  popupMenu -> insertItem( UserIcon("repaint"),  i18n("&Reparse"),     this ,SLOT(slotReparseMenuItem()));
  followCursorId = popupMenu -> insertItem( i18n("Follow Cursor"), this ,SLOT(changeFollowCursor()));

  popupMenu -> setItemChecked ( followCursorId, followCursor() );


  connect( this, SIGNAL(mouseButtonClicked(int, QListViewItem*, const QPoint&, int)),
           this, SLOT  (slotMouseClicked(int, QListViewItem*, const QPoint&, int)));

  connect( this, SIGNAL(doubleClicked(QListViewItem *)), SLOT(slotDoubleClicked(QListViewItem *)));

  connect(this, SIGNAL(expanded(QListViewItem *)), SLOT(slotExpanded(QListViewItem *)));
  connect(this, SIGNAL(collapsed(QListViewItem *)), SLOT(slotCollapsed(QListViewItem *)));

  write = 0L;
  timer.start();
  m_dirty = true;
}


StructTreeView::~StructTreeView(){
}

/** builds the structure tree */
void StructTreeView::buildTree(Node *baseNode, int openLevel)
{
#ifdef DEBUG_PARSER
  kdDebug(24000) << "Starting to rebuild the structure tree." << endl;
#endif
  quantaApp->problemOutput()->clear();
  top = new StructTreeTag( this, i18n("Document Structure") );
  top->setOpen(topOpened);

  Node *currentNode = baseNode;
  StructTreeTag *currentItem = top; //after this
  StructTreeTag *item;
  StructTreeTag *parentItem = top; //under this
  int level = 0;
  QString title;
  QString tagStr;
  QString tmpStr;
  if (m_parsingDTD->family == Script)
  {
    StructTreeGroup group;
    for (uint i = 0; i < groupsCount; i++)
    {
        group = m_parsingDTD->structTreeGroups[i];
        groups[i] = new StructTreeTag(this, i18n(group.name.utf8()) + " ["+m_parsingDTD->nickName+"]");
        if (!group.icon.isEmpty())
        {
          groups[i]->setPixmap(0, SmallIcon(group.icon));
        }
        groups[i]->setOpen(groupOpened[i]);
    }
  } else
  {
    QMap<QString, XMLStructGroup>::ConstIterator it;
    uint i = 0;
    for (it = m_parsingDTD->xmlStructTreeGroups.begin(); it != m_parsingDTD->xmlStructTreeGroups.end(); ++it)
    {
      XMLStructGroup group = it.data();
      groups[i] = new StructTreeTag(this, i18n(group.name.utf8()) + " ["+m_parsingDTD->nickName+"]");
      if (!group.icon.isEmpty())
      {
        groups[i]->setPixmap(0, SmallIcon(group.icon));
      }
      groups[i]->setOpen(groupOpened[i]);
      groupIds.insert(group.name, i);
      i++;
    }

  }
  QMap<QString, QListViewItem*> lastItemInGroup;
  QMap<QString, QListViewItem*> groupItems;
  while (currentNode)
  {
    title = "";
    item = new StructTreeTag(parentItem, currentNode, title, currentItem);
    item->setOpen(level < openLevel);

    if ( (!qConfig.showEmptyNodes && currentNode->tag->type == Tag::Empty) ||
          (!qConfig.showClosingTags &&
          (currentNode->tag->type == Tag::XmlTagEnd ||
            currentNode->tag->type == Tag::ScriptStructureEnd) ) )
    {
      item->setVisible(false);
    }
    currentNode->listItem = item;
    if (currentNode->tag->dtd->family == Xml)
    {
      if (currentNode->groupTag &&
          groupIds.contains(currentNode->group->name))
      {
        XMLStructGroup *group = currentNode->group;
        StructTreeTag *groupItem = groups[groupIds[group->name]];
        QListViewItem* insertAfter = 0L;
        QListViewItem* insertUnder = groupItem;
        if (groupItems.contains(currentNode->groupTag->name))
            insertUnder = groupItems[currentNode->groupTag->name];
        if (lastItemInGroup.contains(group->name))
            insertAfter = lastItemInGroup[group->name];

        StructTreeTag *item = new StructTreeTag(static_cast<StructTreeTag*>(insertUnder), currentNode, currentNode->groupTag->name, insertAfter);
        item->groupTag = currentNode->groupTag;
        if (insertUnder == groupItem)
        {
          groupItems[currentNode->groupTag->name] = item;
          lastItemInGroup[group->name] = item;
        }
        item->hasOpenFileMenu = group->hasFileName;
        item->fileNameRx = group->fileNameRx;

      }
    }
    
    //go to the child node, if it exists
    if (currentNode->child)
    {
      currentNode = currentNode->child;
      parentItem = item;
      currentItem = 0L;
      level++;
    } else
    {
      //go to the next node if it exists
      if (currentNode->next)
      {
        currentNode = currentNode->next;
        currentItem = item;
      } else
      {
        //go up some levels, to the parent, if the node has no child or next
        while (currentNode)
        {
          level--;
          //parentItem = dynamic_cast<StructTreeTag*>(parentItem->parent());
          if (currentNode->parent && currentNode->parent->next)
          {
            currentNode = currentNode->parent->next;
            break;
          } else
          {
            currentNode = currentNode->parent;
          }
        }
        if (currentNode)
        {
          if (currentNode->prev)
              currentItem = static_cast<StructTreeTag*>(currentNode->prev->listItem);
          if (currentNode->parent)
          {
            parentItem = static_cast<StructTreeTag*>(currentNode->parent->listItem);
            if (!parentItem)
            {
              parentItem = top;
            }
          }
          else
          {
            parentItem = top;
          }
        }

      }
    }
  }

  GroupElementList* groupElementList;
  QListViewItem *insertUnder;
  QListViewItem *insertAfter;
  QListViewItem *listItem;
  GroupElementMapList::Iterator it;
  IncludedGroupElementsMap::Iterator externalIt;
  if (m_parsingDTD->family == Script)
  {
    StructTreeGroup group;
    for (uint i = 0; i < groupsCount; i++)
    {
      group = m_parsingDTD->structTreeGroups[i];
      QString name = group.name+"|";
      for (it = globalGroupMap.begin(); it != globalGroupMap.end(); ++it)
      {
        insertUnder = groups[i];
        insertAfter = insertUnder;
        bool first = true;
        if (it.key().startsWith(name))
        {
          groupElementList = & (it.data());
          QString title = it.key().section('|', -1);
          GroupElementList::Iterator groupElementIt;
          for (groupElementIt = groupElementList->begin(); groupElementIt != groupElementList->end(); ++groupElementIt)
          {
            listItem = new StructTreeTag(static_cast<StructTreeTag*>(insertUnder), (*groupElementIt).node, title, insertAfter);
            static_cast<StructTreeTag*>(listItem)->groupTag = (*groupElementIt).tag;
            static_cast<StructTreeTag*>(listItem)->hasOpenFileMenu = group.hasFileName;
            static_cast<StructTreeTag*>(listItem)->fileNameRx = group.fileNameRx;
            if (first)
            {
              insertUnder = listItem;
              first = false;
            }
            insertAfter = listItem;
          }
        }
      }
      for (externalIt = parser->includedMap.begin(); externalIt != parser->includedMap.end(); ++externalIt)
      {
        insertUnder = new StructTreeTag(static_cast<StructTreeTag*>(groups[i]), 0L, externalIt.key(), groups[i]);
        insertAfter = insertUnder;
        IncludedGroupElements elements = externalIt.data();
        GroupElementMapList::Iterator elIt;
        for (elIt = elements[group.name].begin(); elIt != elements[group.name].end(); ++elIt)
        {
          listItem = new StructTreeTag(static_cast<StructTreeTag*>(insertUnder), elIt.data()[0].node, elIt.key(), insertAfter);
          static_cast<StructTreeTag*>(listItem)->hasOpenFileMenu = group.hasFileName;
          static_cast<StructTreeTag*>(listItem)->fileNameRx = group.fileNameRx;
          insertAfter = listItem;
        }
        if (!insertUnder->firstChild())
            delete insertUnder;
        else
            insertUnder->sortChildItems(0, true);
      }
      groups[i]->sortChildItems(0, true);
    }
  }
}

/** Delete the items */
void StructTreeView::deleteList()
{
  if ( top )
  {
    topOpened = top->isOpen();
    delete top;
    top = 0L;
  }
  for (uint i = 0; i < groupsCount; i++)
  {
    groupOpened[i] = groups[i]->isOpen();
    delete groups[i];
    groups[i] = 0L;
  }
  groupIds.clear();
  groupsCount = 0;
}

/** repaint document structure */
void StructTreeView::slotReparse(Document *w, Node* node, int openLevel)
{
  timer.restart();
  if (typingInProgress)
    return;
  deleteList();
  if (!node)
    return;
  groupsCount = m_parsingDTD->structTreeGroups.count();
  write = w;
  write->clearErrorMarks();
  buildTree(node, openLevel);

  kdDebug(24000) << "StructTreeView building: " << timer.elapsed() << " ms\n";

  if (m_parsingDTD->family == Script)
  {
    for (uint i = 0; i < groupsCount; i++)
    {
      if (groups[i]->childCount() == 0)
      {
        groups[i]->setText(0, i18n(m_parsingDTD->structTreeGroups[i].noName.utf8()) + " ["+m_parsingDTD->nickName+"]");
      }
    }
  } else
  {
    QMap<QString, XMLStructGroup>::ConstIterator it;
    uint i = 0;
    for (it = m_parsingDTD->xmlStructTreeGroups.begin(); it != m_parsingDTD->xmlStructTreeGroups.end(); ++it)
    {
      if (groups[i]->childCount() == 0)
      {
        groups[i]->setText(0, i18n(it.data().noName.utf8()) + " ["+m_parsingDTD->nickName+"]");
      }
      i++;
    }
    groupsCount = i;
  }
  useOpenLevelSetting = false;
  m_dirty = false;
}

void StructTreeView::slotGotoTag( QListViewItem *item )
{
  StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
  if (!m_dirty && it && it->node && it->node->tag)
  {
    Tag *tag = new Tag(*it->node->tag);
    int line, col;
    tag->beginPos(line, col);
    if (!it->node->fileName.isEmpty())
    {
      KURL url;
      QuantaCommon::setUrl(url, it->node->fileName);
      emit openFile(url, quantaApp->defaultEncoding());
    }
    int el, ec;
    tag->endPos(el, ec);
/*    
    kdDebug(24000) << "Node area: " << line << ", " << col << ", " << el << ", " << ec << endl;
    kdDebug(24000) << "Node type: " << tag->type << endl;
    kdDebug(24000) << "Node str: " << tag->tagStr() << endl;
    kdDebug(24000) << "Node cleanstr: " << tag->cleanStr << endl;
*/    
    emit newCursorPosition(line, col + 1);
    if (quantaApp->view()->writeExists())
      quantaApp->view()->write()->view()->setFocus();
    delete tag;
  }
}


void StructTreeView::slotMouseClicked(int button, QListViewItem *item, const QPoint& point, int)
{
  if (item)
  {
    config->setGroup("Parser options");

    QString handleMBM = config->readEntry("MBM", i18n("Select tag area"));
    QString handleLBM = config->readEntry("LBM", i18n("Find tag"));
    QString handleDoubleClick = config->readEntry("Double click", i18n("Select tag area"));

    setSelected(item, true);

    if ( button == Qt::RightButton )
    {
      if (dynamic_cast<StructTreeTag*>(item))
      {
        popupMenu->setItemVisible(openFileMenuId, static_cast<StructTreeTag*>(item)->hasOpenFileMenu);
      }
      popupMenu->popup( point);
      return;
    }

    if ( button == Qt::LeftButton )
    {
      if ( handleLBM == i18n("Find Tag && Open Tree"))
           setOpen( item, ! isOpen(item) );
      setSelected(item, true);
      slotGotoTag(item);
    }

    if ( button == Qt::MidButton )
    {
      if ( handleMBM == i18n("nothing"))
           return;

      if ( handleMBM == i18n("Find Tag && Open Tree"))
      {
        setOpen( item, ! isOpen(item) );
        setSelected(item, true);
        slotGotoTag(item);
      }

      if ( handleMBM == i18n("Select Tag Area"))
           slotSelectTag();

      if ( handleMBM == i18n("Go to End of Tag"))
          slotGotoClosingTag();

      setSelected(item, true);
    }
  }
}


void StructTreeView::slotDoubleClicked( QListViewItem *item)
{
  config->setGroup("Parser options");

  if ( config->readEntry("Double click") != i18n("nothing") )
  {
    slotSelectTag();
  } else
  {
    item->setOpen(!item->isOpen());
  }
}



void StructTreeView::slotReparseMenuItem()
{
  useOpenLevelSetting = true;
  emit needReparse();
}

void StructTreeView::slotGotoClosingTag()
{
  QListViewItem *item = currentItem();
  StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
  if (!m_dirty && it && it->node)
  {
    int newLine, newCol;
    Tag *tag = it->node->tag;
    if (tag->single || !it->node->next)
    {
      tag->endPos(newLine, newCol);
    } else
    {
      if (tag->closingMissing)
      {
        Node *node = it->node;
        while (node->child) node = node->child;
        node->tag->endPos(newLine, newCol);
      } else
      {
        it->node->next->tag->endPos(newLine, newCol);
      }
    }

    emit newCursorPosition( newLine, newCol + 1 );
  }
}

void StructTreeView::slotSelectTag()
{
  QListViewItem *item = currentItem();
  StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
  if (!m_dirty && it && it->node)
  {
    int bLine, bCol, eLine, eCol;
    if (it->node->fileName.isEmpty())
    {
      if (it->groupTag)
      {
        Tag *tag = it->groupTag;
        tag->beginPos(bLine, bCol);
        tag->endPos(eLine, eCol);
      } else
      {
        Tag *tag = it->node->tag;
        if (tag->single || !it->node->next)
        {
          tag->endPos(eLine, eCol);
        } else
        { 
          emit selectTagArea(it->node);
          return;
        }
        tag->beginPos(bLine, bCol);
      }
    } else
    {
      KURL url;
      QuantaCommon::setUrl(url, it->node->fileName);
      it->node->tag->beginPos(bLine, bCol);
      it->node->tag->endPos(eLine, eCol);
      eCol--;
      emit openFile(url, quantaApp->defaultEncoding());

    }
    emit selectArea( bLine, bCol, eLine, eCol + 1);

    setSelected(item, true);
    it->node->tag->write()->view()->setFocus();
  }
}


/** Do the recursive opening or closing of the trees */
void StructTreeView::setOpenSubTree( QListViewItem *it, bool open)
{
  if (it)
  {
    it->setOpen(open);
    setOpenSubTree( it->nextSibling(), open );
    setOpenSubTree( it->firstChild(), open );
  }
}

/** Recursively open the tree and all its subtrees */
void StructTreeView::slotOpenSubTree()
{
  QListViewItem *item = currentItem();
  if (item)
  {
    item->setOpen( true );
    setOpenSubTree( item->firstChild(), true );
  }
}


/** Recursively close the tree and all its subtrees */
void StructTreeView::slotCloseSubTree()
{
  QListViewItem *item = currentItem();
  if (item)
  {
    item->setOpen( false );
    setOpenSubTree( item->firstChild(), false );
  }
}

/** Show the element in tree according to cursor position (x,y) */
void StructTreeView::showTagAtPos(Node *node)
{
  if (followCursorFlag)
  {
    if (node->listItem)
    {
      ensureItemVisible(node->listItem);
      setSelected(node->listItem, true);
    }
  } //if (followCursorFlag)
}

void StructTreeView::setFollowCursor(bool follow)
{
   followCursorFlag = follow;
   popupMenu -> setItemChecked ( followCursorId, follow );
}

/** No descriptions */
void StructTreeView::slotExpanded(QListViewItem *item)
{
 StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
 if (!m_dirty && it && it->node)
     it->node->opened = true;
}

/** No descriptions */
void StructTreeView::slotCollapsed(QListViewItem *item)
{
 StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
 if (!m_dirty && it && it->node)
     it->node->opened = false;
}
/** Do a reparse before showing. */
void StructTreeView::showEvent(QShowEvent* /*ev*/)
{
  slotReparseMenuItem();
}

/** The treeview DTD  has changed to id. */
void StructTreeView::slotDTDChanged(int id)
{
  QString text = dtdMenu->text(id);
  QString dtdName = DTDs::ref()->getDTDNameFromNickName(text);
  emit parsingDTDChanged(dtdName.lower());
}
/** Set the Parse As... menu to dtdName. */
void StructTreeView::setParsingDTD(const QString dtdName)
{
  QString dtdNickName = DTDs::ref()->getDTDNickNameFromName(dtdName);
  for (uint i = 0; i < dtdList.count(); i++)
  {
    dtdMenu->setItemChecked(i, dtdList[i] == dtdNickName);
  }
  m_parsingDTD = DTDs::ref()->find(dtdName); //this should always exist
}

void StructTreeView::slotOpenFile()
{
  StructTreeTag *item = dynamic_cast<StructTreeTag*>(currentItem());
  if (!m_dirty && item->node)
  {
    QString text = item->groupTag->name;
    text.remove(item->fileNameRx);
    KURL url;
    QuantaCommon::setUrl(url, text.stripWhiteSpace());
    KURL baseUrl = QExtFileInfo::path(write->url());
    url = QExtFileInfo::toAbsolute(url, baseUrl);
    if ( QuantaCommon::checkMimeGroup(url,"text" ) )
    {
      emit openFile( url, quantaApp->defaultEncoding() );
    }
    else if ( QuantaCommon::checkMimeGroup(url,"image" ) )
    {
      emit openImage( url );
    }
  }
}

void StructTreeView::slotNodeTreeChanged()
{
  m_dirty = true;
}
