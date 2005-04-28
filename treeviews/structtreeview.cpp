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
#include <kmessagebox.h>
#include <kconfig.h>
#include <kdebug.h>
#include <ktexteditor/view.h>

// app includes
#include "messageoutput.h"
#include "node.h"
#include "qtag.h"
#include "document.h"
#include "resource.h"
#include "qextfileinfo.h"
#include "quantacommon.h"
#include "dtds.h"
#include "viewmanager.h"

#include "structtreetag.h"
#include "structtreeview.h"
#include "structtreeview.moc"
extern GroupElementMapList globalGroupMap;

StructTreeView::StructTreeView(QWidget *parent, const char *name )
: KListView(parent,name)
{
  for (int i = 0; i < 15; i++)
    groupOpened.append(false);
  top = 0L;
  lastTag = 0L;
  groupsCount = 0;
  followCursorFlag = true;
  config = kapp->config();

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

  dtdMenu->insertItem(i18n("All Present DTEP"));
  dtdMenu->insertSeparator();
  dtdList = DTDs::ref()->nickNameList();
  for(uint i = 0; i < dtdList.count(); i++ )
  {
    dtdMenu->insertItem(dtdList[i], i, -1);
  }

  connect(dtdMenu, SIGNAL(activated(int)), this, SLOT(slotDTDChanged(int)));
  
  emptyAreaMenu = new KPopupMenu(this);
  emptyAreaMenu->insertItem(i18n("Show Groups For"), dtdMenu);
  emptyAreaMenu->insertItem(SmallIcon("reload"), i18n("&Reparse"),     this, SLOT(slotReparseMenuItem()));

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
  popupMenu -> insertItem( SmallIcon("reload"),  i18n("&Reparse"),     this ,SLOT(slotReparseMenuItem()));
  followCursorId = popupMenu -> insertItem( i18n("Follow Cursor"), this ,SLOT(changeFollowCursor()));

  popupMenu -> setItemChecked ( followCursorId, followCursor() );


  connect( this, SIGNAL(mouseButtonPressed(int, QListViewItem*, const QPoint&, int)),
           this, SLOT  (slotMouseClicked(int, QListViewItem*, const QPoint&, int)));

  connect( this, SIGNAL(doubleClicked(QListViewItem *)), SLOT(slotDoubleClicked(QListViewItem *)));

  connect(this, SIGNAL(expanded(QListViewItem *)), SLOT(slotExpanded(QListViewItem *)));
  connect(this, SIGNAL(collapsed(QListViewItem *)), SLOT(slotCollapsed(QListViewItem *)));

  write = 0L;
  timer = new QTime();
  timer->start();
  m_dirty = true;
}


StructTreeView::~StructTreeView(){
  delete timer;
}

/** builds the structure tree */
void StructTreeView::buildTree(Node *baseNode, int openLevel, bool groupOnly)
{
#ifdef DEBUG_PARSER
  kdDebug(24000) << "Starting to rebuild the structure tree. Grouponly = " << groupOnly << endl;
#endif
  emit clearProblemOutput();
  if (!groupOnly)
  {
      top = new StructTreeTag( this, i18n("Document Structure") );
      top->setOpen(topOpened);
  }
  Node *currentNode = baseNode;
  StructTreeTag *currentItem = top; //after this
  StructTreeTag *item = 0L;
  StructTreeTag *parentItem = top; //under this
  int level = 0;
  QString title;
  QString tagStr;
  QString tmpStr;
  int groupId = 0;
  const DTDStruct* parsingDTD;
  for (uint index = 0; index < m_parsingDTDList.count(); index++)
  {
    parsingDTD = m_parsingDTDList[index];
    if (parsingDTD->family == Script)
    {
      StructTreeGroup group;
      uint gCount = parsingDTD->structTreeGroups.count();
      for (uint i = 0; i < gCount; i++)
      {
        group = parsingDTD->structTreeGroups[i];
        StructTreeTag *groupTag = new StructTreeTag(this, i18n(group.name.utf8()) + " [" + parsingDTD->nickName+"]");
        if (!group.icon.isEmpty())
        {
          groupTag->setPixmap(0, SmallIcon(group.icon));
        }
        groupTag->setOpen(groupOpened[groupId]);
#ifdef DEBUG_PARSER
        kdDebug(24001) << "Grouptag created: " << groupId << " " << groupTag->text(0) << " "<< groupTag << endl;
#endif
        groups.append(groupTag);
        groupIds.insert(group.name + parsingDTD->name, groupId);
        groupId++;
      }
    } else
    {
      QMap<QString, XMLStructGroup>::ConstIterator it;
      for (it = parsingDTD->xmlStructTreeGroups.begin(); it != parsingDTD->xmlStructTreeGroups.end(); ++it)
      {
        XMLStructGroup group = it.data();
        StructTreeTag *groupTag = new StructTreeTag(this, i18n(group.name.utf8()) + " [" + parsingDTD->nickName+"]");
        if (!group.icon.isEmpty())
        {
          groupTag->setPixmap(0, SmallIcon(group.icon));
        }
        groupTag->setOpen(groupOpened[groupId]);
#ifdef DEBUG_PARSER
        kdDebug(24001) << "Grouptag created: " << groupId << " " << groupTag->text(0) << " "<< groupTag << endl;
#endif
        groups.append(groupTag);
        groupIds.insert(group.name + parsingDTD->name, groupId);
        groupId++;
      }

    }
  }
  groupsCount = groupId;
  QMap<QString, QListViewItem*> lastItemInGroup;
  QMap<QString, QListViewItem*> groupItems;
  while (currentNode)
  {
    if (!groupOnly)
    {
      title = "";
      item = new StructTreeTag(parentItem, currentNode, title, currentItem);
      item->setOpen(level < openLevel);
      currentNode->mainListItem = item;

      if ( (!qConfig.showEmptyNodes && currentNode->tag->type == Tag::Empty) ||
            (!qConfig.showClosingTags &&
            (currentNode->tag->type == Tag::XmlTagEnd ||
              currentNode->tag->type == Tag::ScriptStructureEnd) ) )
      {
        item->setVisible(false);
      }
    }
    const DTDStruct *dtd = currentNode->tag->dtd();
    //add all the group elements belonging to this node to the tree
    for (QValueList<GroupElement*>::ConstIterator it = currentNode->m_groupElements.constBegin(); it != currentNode->m_groupElements.constEnd(); ++it)
    {
      GroupElement *groupElement = (*it);
      if (!groupIds.contains(groupElement->group->name + dtd->name))
        continue;
      StructTreeTag *groupItem = groups[groupIds[groupElement->group->name + dtd->name]];
      QListViewItem* insertAfter = 0L;
      QListViewItem* insertUnder = groupItem;
      if (groupItems.contains(groupElement->group->name + groupElement->tag->name))
        insertUnder = groupItems[groupElement->group->name  + groupElement->tag->name];
      if (lastItemInGroup.contains(groupElement->group->name))
        insertAfter = lastItemInGroup[groupElement->group->name];

      StructTreeTag *item = new StructTreeTag(static_cast<StructTreeTag*>(insertUnder), currentNode, groupElement->tag->name, insertAfter);
      item->groupTag = groupElement->tag;
      if (insertUnder == groupItem)
      {
        groupItems[groupElement->group->name + groupElement->tag->name] = item;
        lastItemInGroup[groupElement->group->name] = item;
      }
      item->hasOpenFileMenu = groupElement->group->hasFileName;
      item->fileNameRx = groupElement->group->fileNameRx;
#ifdef DEBUG_PARSER
      kdDebug(24001) << "Tree element "<< groupElement->tag->tagStr() << "[" << groupElement->group->name<<"]"<< " inserted: " << item << " under " <<insertUnder << " after " << insertAfter << endl;
#endif
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
        if (!groupOnly && currentNode)
        {
          if (currentNode->prev)
              currentItem = static_cast<StructTreeTag*>(currentNode->prev->mainListItem);
          if (currentNode->parent)
          {
            parentItem = static_cast<StructTreeTag*>(currentNode->parent->mainListItem);
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
  //add the externally found items to the tree
  QListViewItem *insertUnder;
  QListViewItem *insertAfter;
  QListViewItem *listItem;
  GroupElementMapList::Iterator it;
  IncludedGroupElementsMap::Iterator externalIt;
  for (uint index = 0; index < m_parsingDTDList.count(); index++)
  {
    parsingDTD = m_parsingDTDList[index];
    if (parsingDTD->family == Script)
    {
      StructTreeGroup group;
      uint gCount = parsingDTD->structTreeGroups.count();
      for (uint i = 0; i < gCount; i++)
      {
        group = parsingDTD->structTreeGroups[i];
        groupId = groupIds[group.name + parsingDTD->name];
        QString name = group.name+"|";
        StructTreeTag *groupTag = groups[groupId];
        for (externalIt = parser->includedMap.begin(); externalIt != parser->includedMap.end(); ++externalIt)
        {
          insertUnder = new StructTreeTag(static_cast<StructTreeTag*>(groupTag), 0L, externalIt.key(), groupTag);
          insertAfter = insertUnder;
          IncludedGroupElements elements = externalIt.data();
          GroupElementMapList::Iterator elIt;
          for (elIt = elements[group.name].begin(); elIt != elements[group.name].end(); ++elIt)
          {
            listItem = new StructTreeTag(static_cast<StructTreeTag*>(insertUnder), elIt.data()[0]->node, elIt.key(), insertAfter);
            static_cast<StructTreeTag*>(listItem)->hasOpenFileMenu = group.hasFileName;
            static_cast<StructTreeTag*>(listItem)->fileNameRx = group.fileNameRx;
            insertAfter = listItem;
          }
          if (!insertUnder->firstChild())
              delete insertUnder;
          else
              insertUnder->sortChildItems(0, true);
        }
        groupTag->sortChildItems(0, true);
      }
    }
  }
}

/** Delete the items */
void StructTreeView::deleteList(bool groupOnly)
{
  if (!groupOnly && top )
  {
    topOpened = top->isOpen();
    delete top;
    top = 0L;
  }
  for (uint i = 0; i < groupsCount; i++)
  {
    groupOpened.append(groups[i]->isOpen());
#ifdef DEBUG_PARSER
    kdDebug(24001) << "Grouptag deleted: " << i << " " <<  groups[i]->text(0) << endl;
#endif
    delete groups[i];
  }
  groups.clear();
  groupIds.clear();
  groupsCount = 0;
}

/** repaint document structure */
void StructTreeView::slotReparse(Document *w, Node* node, int openLevel, bool groupOnly)
{
  timer->restart();
  if (typingInProgress)
    return;
  emit clearAnnotationOutput();
  deleteList(groupOnly);
  if (!node)
    return;
  write = w;
  if (write)
    write->clearAnnotations();
  write->clearErrorMarks();
  buildTree(node, openLevel, groupOnly);

  kdDebug(24000) << "StructTreeView building: " << timer->elapsed() << " ms\n";

  const DTDStruct *parsingDTD;
  int groupId = 0;
  for (uint index = 0; index < m_parsingDTDList.count(); index++)
  {
    parsingDTD = m_parsingDTDList[index];
    if (parsingDTD->family == Script)
    {
      uint gCount = parsingDTD->structTreeGroups.count();
      for (uint i = 0; i < gCount; i++)
      {
        StructTreeTag *groupTag = groups[groupId];
        if (groupTag->childCount() == 0)
        {
          if (qConfig.showEmptyNodes)
          {
            //kdDebug(24000) << "No elements in group: " << groupId << " " << groupTag->text(0) << endl;
            groupTag->setText(0, i18n(parsingDTD->structTreeGroups[i].noName.utf8()) + " [" + parsingDTD->nickName+"]");
          } else
          {
            groupTag->setVisible(false);
          }
        }
        groupId++;
      }
    } else
    {
      QMap<QString, XMLStructGroup>::ConstIterator it;
      uint i = 0;
      for (it = parsingDTD->xmlStructTreeGroups.begin(); it != parsingDTD->xmlStructTreeGroups.end(); ++it)
      {
        StructTreeTag *groupTag = groups[groupId];
        if (groupTag->childCount() == 0)
        {
          if (qConfig.showEmptyNodes)
          {
            //kdDebug(24000) << "No elements in group: " << groupId << " " << groupTag->text(0) << endl;
            groupTag->setText(0, i18n(it.data().noName.utf8()) + " [" + parsingDTD->nickName+"]");
          } else
          {
            groupTag->setVisible(false);
          }
        }
        i++;
        groupId++;
      }
    }
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
      emit openFile(url);
    }
    int el, ec;
    tag->endPos(el, ec);
/*
    kdDebug(24000) << "Node area: " << line << ", " << col << ", " << el << ", " << ec << endl;
    kdDebug(24000) << "Node type: " << tag->type << endl;
    kdDebug(24000) << "Node str: " << tag->tagStr() << endl;
    kdDebug(24000) << "Node cleanstr: " << tag->cleanStr << endl;
*/
    if (tag->type == Tag::XmlTag || tag->type == Tag::XmlTagEnd)
      col++; //position the cursor inside the tag
    emit newCursorPosition(line, col);
    Document *w = ViewManager::ref()->activeDocument();
    if (w)
      w->view()->setFocus();
    delete tag;
  }
}


void StructTreeView::slotMouseClicked(int button, QListViewItem *item, const QPoint& point, int)
{
  if (item)
  {
    config->setGroup("Parser options");

    QString handleMBM = config->readEntry("MBM", i18n("Select Tag Area"));
    QString handleLBM = config->readEntry("LBM", i18n("Find tag"));
    QString handleDoubleClick = config->readEntry("Double click", i18n("Select Tag Area"));

    setSelected(item, true);

    if (button == Qt::RightButton)
    {
      if (dynamic_cast<StructTreeTag*>(item))
      {
        popupMenu->setItemVisible(openFileMenuId, static_cast<StructTreeTag*>(item)->hasOpenFileMenu);
      }
      popupMenu->popup(point);
      return;
    }

    if (button == Qt::LeftButton)
    {
      if (handleLBM == i18n("Find Tag && Open Tree"))
           setOpen(item, !isOpen(item));
      setSelected(item, true);
      slotGotoTag(item);
    }

    if (button == Qt::MidButton)
    {
      if (handleMBM == i18n("nothing"))
           return;

      if (handleMBM == i18n("Find Tag && Open Tree"))
      {
        setOpen(item, !isOpen(item));
        setSelected(item, true);
        slotGotoTag(item);
      }

      if (handleMBM == i18n("Select Tag Area"))
           slotSelectTag();

      if (handleMBM == i18n("Go to End of Tag"))
          slotGotoClosingTag();

      setSelected(item, true);
    }
  } else
    if (button == Qt::RightButton)
      emptyAreaMenu->popup(point);
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
  bool newFileOpened = false;
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
      emit openFile(url);
      newFileOpened = true;

    }
    emit selectArea( bLine, bCol, eLine, eCol + 1);

    if (!newFileOpened)
    {
      setSelected(item, true);
      it->node->tag->write()->view()->setFocus();
    }
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
    if (node && node->mainListItem)
    {
      ensureItemVisible(node->mainListItem);
      setSelected(node->mainListItem, true);
    }
  } //if (followCursorFlag)
}

void StructTreeView::setFollowCursor(bool follow)
{
   followCursorFlag = follow;
   popupMenu->setItemChecked(followCursorId, follow);
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

/** Do a reparse before showing. */
void StructTreeView::hideEvent(QHideEvent* /*ev*/)
{
  emit clearProblemOutput();
}

/** The treeview DTD  has changed to id. */
void StructTreeView::slotDTDChanged(int id)
{
  QString text = dtdMenu->text(id);
  if (dtdMenu->indexOf(id) > 0)
  {
    QString dtdName = DTDs::ref()->getDTDNameFromNickName(text);
    emit showGroupsForDTEP(dtdName, !dtdMenu->isItemChecked(id));
  } else
    emit showGroupsForDTEP("clear", true);
}

void StructTreeView::setParsingDTDs(const QStringList &parsingDTDList)
{
  m_parsingDTDList.clear();
  for (uint i = 0; i < dtdList.count(); i++)
  {
    dtdMenu->setItemChecked(i, false);
  }
  QString dtdNickName;
  for (QStringList::ConstIterator it = parsingDTDList.constBegin(); it != parsingDTDList.constEnd(); ++it)
  {
    dtdNickName = DTDs::ref()->getDTDNickNameFromName(*it);
    for (uint i = 0; i < dtdList.count(); i++)
    {
      if (dtdList[i] == dtdNickName)
        dtdMenu->setItemChecked(i, true);
    }
    m_parsingDTDList.append(DTDs::ref()->find(*it));
  }
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
    if (QExtFileInfo::exists(url))
    {
      if (QuantaCommon::checkMimeGroup(url, "text" ))
      {
        emit openFile(url);
      }
      else if (QuantaCommon::checkMimeGroup(url," image" ))
      {
        emit openImage(url);
      }
    } else
      KMessageBox::error(this, i18n("<qt>The file <b>%1</b> does not exist or is not a recognized mime type.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)));
  }
}

void StructTreeView::slotNodeTreeChanged()
{
  m_dirty = true;
}

void StructTreeView::showMessage(const QString& message)
{
  emit showProblemMessage(message);
}
