/***************************************************************************
                          structtreeview.cpp  -  description
                             -------------------
    begin                : Sat Apr 29 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002, 2003 Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@freemail.hu
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
#include "../parser/node.h"
#include "../parser/qtag.h"
#include "../document.h"
#include "../resource.h"
#include "../qextfileinfo.h"
#include "../quanta.h"

#include "structtreetag.h"
#include "structtreeview.h"
#include "structtreeview.moc"

StructTreeView::StructTreeView(KConfig *config, QWidget *parent, const char *name )
: KListView(parent,name)
{
  top = 0L;
  lastTag = 0L;
  groupsCount = 0;
  followCursorFlag = true;
  this->config = config;

  topOpened = true;
  useOpenLevelSetting = true;

  setRootIsDecorated( true );
  header()->hide();
  setSorting(-1,false);

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( i18n("Name"), 1600 );

  setFocusPolicy(QWidget::ClickFocus);

  dtdMenu = new KPopupMenu(this);

  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    dtdList << it.current()->nickName;
  }
  dtdList.sort();

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
  popupMenu -> insertItem( UserIcon("repaint"),  i18n("&Reparse"),     this ,SLOT(slotReparse()));
  followCursorId = popupMenu -> insertItem( i18n("Follow Cursor"), this ,SLOT(changeFollowCursor()));

  popupMenu -> setItemChecked ( followCursorId, followCursor() );


  connect( this, SIGNAL(mouseButtonClicked(int, QListViewItem*, const QPoint&, int)),
           this, SLOT  (slotMouseClicked(int, QListViewItem*, const QPoint&, int)));

  connect( this, SIGNAL(doubleClicked(QListViewItem *)), SLOT(slotDoubleClicked(QListViewItem *)));

  connect(this, SIGNAL(expanded(QListViewItem *)), SLOT(slotExpanded(QListViewItem *)));
  connect(this, SIGNAL(collapsed(QListViewItem *)), SLOT(slotCollapsed(QListViewItem *)));

  write = 0L;
  timer.start();
}


StructTreeView::~StructTreeView(){
}

/** builds the structure tree */
void StructTreeView::buildTree(Node *baseNode, int openLevel)
{
//  if (baseNode)
  {
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
    StructTreeGroup group;
    for (uint i = 0; i < groupsCount; i++)
    {
       group = m_parsingDTD->structTreeGroups[i];
       groups[i] = new StructTreeTag(this, i18n(group.name) + " ["+m_parsingDTD->nickName+"]");
       if (!group.icon.isEmpty())
       {
         groups[i]->setPixmap(0, SmallIcon(group.icon));
       }
       groups[i]->setOpen(groupOpened[i]);
    }

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
                currentItem = dynamic_cast<StructTreeTag*>(currentNode->prev->listItem);
            if (currentNode->parent)
            {
              parentItem = dynamic_cast<StructTreeTag*>(currentNode->parent->listItem);
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
  }
  GroupElementList* groupElementList;
  GroupElementMapList* groupElementMapList;
  QListViewItem *insertUnder;
  QListViewItem *insertAfter;
  QListViewItem *item;
  StructTreeGroup group;
  GroupElementMapList::Iterator it;
  IncludedGroupElementsMap::Iterator externalIt;
  for (uint i = 0; i < groupsCount; i++)
  {
    group = m_parsingDTD->structTreeGroups[i];
    groupElementMapList = &(parser->m_groups[group.name]);
    for (it = groupElementMapList->begin(); it != groupElementMapList->end(); ++it)
    {
      insertUnder = groups[i];
      insertAfter = insertUnder;
      bool first = true;
      groupElementList = & (it.data());
      for (uint j = 0; j < groupElementList->count(); j++)
      {
        item = new StructTreeTag(static_cast<StructTreeTag*>(insertUnder), (*groupElementList)[j].node, (*groupElementList)[j].node->tag->name, insertAfter);
        static_cast<StructTreeTag*>(item)->hasOpenFileMenu = group.hasFileName;
        static_cast<StructTreeTag*>(item)->fileNameRx = group.fileNameRx;
        if (first)
        {
          insertUnder = item;
          first = false;
        }
        insertAfter = item;
      }
    }
    for (externalIt = parser->includedMap.begin(); externalIt != parser->includedMap.end(); ++externalIt)
    {
      insertUnder = new StructTreeTag(static_cast<StructTreeTag*>(groups[i]), 0L, externalIt.key(), groups[i]);
      insertAfter = insertUnder;
      IncludedGroupElements elements = externalIt.data();
      for (uint j = 0; j < elements[group.name].count(); j++)
      {
        item = new StructTreeTag(static_cast<StructTreeTag*>(insertUnder), 0L, elements[group.name][j], insertAfter);
        static_cast<StructTreeTag*>(item)->hasOpenFileMenu = group.hasFileName;
        static_cast<StructTreeTag*>(item)->fileNameRx = group.fileNameRx;
        insertAfter = item;
      }
      if (!insertUnder->firstChild())
          delete insertUnder;
      else
          insertUnder->sortChildItems(0, true);
    }
    groups[i]->sortChildItems(0, true);
  }

}

/*TODO: move this code to some other place
      case Tag::XmlTag:
           {
             QString name = currentNode->tag->name;
             //QString tagName = (m_parsingDTD->caseSensitive) ? name : name.upper();
             QString tagName = name.lower();
             //add the new xml tags to the userTagList
             if ( !QuantaCommon::isKnownTag(m_parsingDTD->name, tagName) &&
                   startWithLetterRx.exactMatch(name) )
             {
               Document *w = currentNode->tag->write();
               QTag *newTag = w->userTagList.find(tagName);
               bool insertNew = !newTag;
               if (insertNew)
               {
                 newTag = new QTag();
                 newTag->setName(name);
                 newTag->parentDTD = m_parsingDTD;
               }
               for (int i = 0; i < currentNode->tag->attrCount(); i++)
               {
                 Attribute *attr = new Attribute;
                 attr->name = currentNode->tag->attribute(i);
                 attr->values.append(currentNode->tag->attributeValue(i));
                 newTag->addAttribute(attr);
                 delete attr;
               }
               if (insertNew)
               {
                 w->userTagList.insert(tagName, newTag);
               }
             }

             item = new StructTreeTag(parent, currentNode, currentNode->tag->name, lastItem);
             parseForGroups = true;
             break;
           }
*/

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
 groupsCount = 0;
}

/** repaint document structure */
void StructTreeView::slotReparse(Document *w, Node* node, int openLevel)
{
  timer.restart();
  deleteList();
  groupsCount = m_parsingDTD->structTreeGroups.count();
  write = w;
  buildTree(node, openLevel);

  kdDebug(24000) << "StructTreeView building: " << timer.elapsed() << " ms\n";

  for (uint i = 0; i < groupsCount; i++)
  {
    if (groups[i]->childCount() == 0)
    {
      groups[i]->setText(0, i18n(m_parsingDTD->structTreeGroups[i].noName) + " ["+m_parsingDTD->nickName+"]");
    }
  }
  useOpenLevelSetting = false;
}

void StructTreeView::slotGotoTag( QListViewItem *item )
{
  StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
  if (it && it->node)
  {
    int line, col;
    it->node->tag->beginPos(line, col);
    emit newCursorPosition( line, col);
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



void StructTreeView::slotReparse()
{
  useOpenLevelSetting = true;
  emit needReparse();
}

void StructTreeView::slotGotoClosingTag()
{
  QListViewItem *item = currentItem();
  StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
  if (it && it->node)
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
  if (it && it->node)
  {
    int bLine, bCol, eLine, eCol;
    Tag *tag = it->node->tag;
    if (tag->single || !it->node->next)
    {
      tag->endPos(eLine, eCol);
    } else
    {
      if (tag->closingMissing && it->node->child)
      {
        Node *node = it->node->child;
        while (node->child || node->next)
        {
          if (node->next)
          {
            node = node->next;
          } else
          {
            node = node->child;
          }
        }
        node->tag->endPos(eLine, eCol);
      } else
      {
          it->node->next->tag->endPos(eLine, eCol);
      }
    }
    it->node->tag->beginPos(bLine, bCol);
    emit selectArea( bLine, bCol, eLine, eCol+1);

    setSelected(item, true);
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
  if ( followCursorFlag )
  {
    StructTreeTag *curTag = dynamic_cast<StructTreeTag *>(node->listItem);
    if ( curTag )
    {
      ensureItemVisible(curTag);
      setSelected(curTag, true);
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
 if (it && it->node)
     it->node->opened = true;
}

/** No descriptions */
void StructTreeView::slotCollapsed(QListViewItem *item)
{
 StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
 if (it && it->node)
     it->node->opened = false;
}
/** Do a reparse before showing. */
void StructTreeView::showEvent(QShowEvent* /*ev*/)
{
 slotReparse();
}

/** The treeview DTD  has changed to id. */
void StructTreeView::slotDTDChanged(int id)
{
  QString text = dtdMenu->text(id);
  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    if (it.current()->nickName == text)
    {
      QString dtdName = QuantaCommon::getDTDNameFromNickName(text);
      emit parsingDTDChanged(dtdName.lower());
      break;
    }
  }
}
/** Set the Parse As... menu to dtdName. */
void StructTreeView::setParsingDTD(const QString dtdName)
{
  QString dtdNickName = QuantaCommon::getDTDNickNameFromName(dtdName);
  for (uint i = 0; i < dtdList.count(); i++)
  {
    dtdMenu->setItemChecked(i, dtdList[i] == dtdNickName);
  }
  m_parsingDTD = dtds->find(dtdName); //this should always exist
}

void StructTreeView::slotOpenFile()
{
  StructTreeTag *item = dynamic_cast<StructTreeTag*>(currentItem());
  if (item->node)
  {
    QString text = item->node->tag->name;
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
      emit activatePreview();
      emit openImage( url );
    }
  }
}
