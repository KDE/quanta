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
#include "../parser/parser.h"
#include "../parser/qtag.h"
#include "../document.h"
#include "../resource.h"

#include "structtreetag.h"
#include "structtreeview.h"
#include "structtreeview.moc"

StructTreeView::StructTreeView(Parser *parser, KConfig *config, QWidget *parent, const char *name )
: KListView(parent,name)
{
  top = 0L;
  lastTag = 0L;
  groupsCount = 0;
  followCursorFlag = true;
  this->config = config;

  this->parser = parser;

  topOpened = true;
  useOpenLevelSetting = true;

  setRootIsDecorated( true );
  header()->hide();
  setSorting(-1,false);

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( i18n("Name"), 1600 );

  setFocusPolicy(QWidget::ClickFocus);

  dtdMenu = new QPopupMenu(this);
  
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

  popupMenu = new QPopupMenu();

  popupMenu -> insertItem( i18n("Show Groups For..."), dtdMenu);
  popupMenu -> insertSeparator();
  popupMenu -> insertItem( i18n("Select Tag Area"), this ,SLOT(slotSelectTag()));
  popupMenu -> insertItem( i18n("Go To End Of Tag"), this ,SLOT(slotGotoClosingTag()));
  popupMenu -> insertSeparator();
  popupMenu -> insertItem( i18n("Open Subtrees"), this ,SLOT(slotOpenSubTree()));
  popupMenu -> insertItem( i18n("Close Subtrees"),this ,SLOT(slotCloseSubTree()));
  popupMenu -> insertSeparator();
  popupMenu -> insertItem( UserIcon("repaint"),  i18n("&Reparse"), 		this ,SLOT(slotReparse()));
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
       groups[i] = new StructTreeTag(this, i18n(group.name));
       if (!group.icon.isEmpty())
       {
         groups[i]->setPixmap(0, SmallIcon(group.icon));
       }
       groups[i]->setOpen(groupOpened[i]);
    }

    int count = 1;
    while (currentNode)
    {
      /*
      if (timer.elapsed() > 20*count)
      {
        kapp->processEvents();
        count++;
      }
      */
    //  if (!parentItem)
        //parentItem = top;
      title = "";
      item = new StructTreeTag(parentItem, currentNode, title, currentItem);
      item->setOpen(level < openLevel);

      if (currentNode->tag->type == Tag::Empty)
      {
         // item->setVisible(false);
      }

      if (m_parsingDTD->family == Xml)
      {
        for (uint i = 0; i < groupsCount; i++)
        {
          group = m_parsingDTD->structTreeGroups[i];
          if (currentNode->tag->name.lower() == group.tag)
          {
            title = "";
            for (uint j = 0; j < group.attributes.count(); j++)
            {
              if (currentNode->tag->hasAttribute(group.attributes[j]))
              {           
                title.append(currentNode->tag->attributeValue(group.attributes[j]).left(100));
                title.append(" | ");
              }
            }
            title = title.left(title.length()-3);
            title.replace(newLineRx,"");

            //Find the group and the item (if there is already an item with this
            //name) under we should insert the new node
            QListViewItem *insertUnder = groups[i];
            QListViewItem *insertAfter = insertUnder;
            QListViewItemIterator it(insertUnder);
            while ( it.current() && it.current() != insertUnder->nextSibling() )
            {
              if (it.current()->text(0) == title) //we have already an item with this text
              {
                insertUnder = it.current();
                break;
              }
              ++it;
            }
            insertAfter = insertUnder->firstChild();
            while (insertAfter && insertAfter->nextSibling())
                insertAfter = insertAfter->nextSibling();
            new StructTreeTag(dynamic_cast<StructTreeTag*>(insertUnder), currentNode, title, insertAfter);
          }
        }
      } else
      if (currentNode->tag->type == Tag::Text ||
            currentNode->tag->type == Tag::ScriptStructureBegin)
      {
        for (uint i = 0; i < groupsCount; i++)
        {
          group = m_parsingDTD->structTreeGroups[i];
          //parse the node for groups (function/variable/inclusion/etc.) definitions
          tagStr = currentNode->tag->tagStr();
          int pos = 0;
          while (pos != -1)
          {
            pos = group.searchRx.search(currentNode->tag->cleanStr, pos);
            if (pos != -1)
            {
              title = tagStr.mid(pos, group.searchRx.cap().length());
              //build a new tag for this group entry
              Tag *newTag = new Tag(*currentNode->tag);
              tmpStr = tagStr.left(pos);
              int newLines = tmpStr.contains('\n');
              int bl, bc, el, ec;
              newTag->beginPos(bl, bc);
              bl += newLines;
              int l = tmpStr.findRev('\n'); //the last EOL
              bc = (l == -1) ? bc+pos : pos - l - 1;
              newLines = title.contains('\n');
              l = title.length();
              el = bl + newLines;
              ec = (newLines > 0) ? l - title.findRev('\n') : bc + l - 1;
              newTag->setTagPosition(bl, bc, el, ec);
              newTag->setStr(title);
              pos += l;
              title.replace(group.clearRx,"");

              //build a node for the entry
              Node *node = new Node(currentNode);
              node->tag = newTag;

              //Find the group and the item (if there is already an item with this
              //name) under we should insert the new node
              QListViewItem *insertUnder = groups[i];
              QListViewItem *insertAfter = insertUnder;
              QListViewItemIterator it(insertUnder);
              while ( it.current() && it.current() != insertUnder->nextSibling() )
              {
                if (it.current()->text(0) == title) //we have already an item with this text
                {
                  insertUnder = it.current();
                  break;
                }
                ++it;
              }
              insertAfter = insertUnder->firstChild();
              while (insertAfter && insertAfter->nextSibling())
                  insertAfter = insertAfter->nextSibling();
              new StructTreeTag(dynamic_cast<StructTreeTag*>(insertUnder), node, title, insertAfter);
            }
          }
        }
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
               for (int i = 0; i < currentNode->tag->attrCount; i++)
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
      QString noGroup = m_parsingDTD->structGroups[i].section(';',1,1);
      groups[i]->setText(0, i18n(noGroup));
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


void StructTreeView::slotDoubleClicked( QListViewItem *)
{
  config->setGroup("Parser options");

  if ( config->readEntry("Double click") != i18n("nothing") )
  {
    slotSelectTag();
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
void StructTreeView::showTagAtPos(int x, int y)
{
  if ( followCursorFlag )
  {
    Node *node = parser->nodeAt(x, y);
    if (node)
    {
      StructTreeTag *curTag = dynamic_cast<StructTreeTag *>(node->listItem);
      if ( curTag )
      {
        ensureItemVisible(curTag);
        setSelected(curTag, true);
      }
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
