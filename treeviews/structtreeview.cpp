/***************************************************************************
                          structtreeview.cpp  -  description
                             -------------------
    begin                : Sat Apr 29 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002 Andras Mantia
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

// KDE headers
#include <kapp.h>
#include <kiconloader.h>
#include <kpopupmenu.h>
#include <klocale.h>
#include <kconfig.h>

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
  addColumn( i18n("Name"), 600 );

	setFocusPolicy(QWidget::ClickFocus);

  dtdMenu = new QPopupMenu(this);
  
  QDictIterator<DTDStruct> it(*dtds);
  int id = 0;
  for( ; it.current(); ++it )
  {
    dtdMenu->insertItem(it.current()->nickName,id,-1);
    id++;
  }
  connect(dtdMenu, SIGNAL(activated(int)), this, SLOT(slotDTDChanged(int)));

 	popupMenu = new QPopupMenu();

  popupMenu -> insertItem( i18n("Parse As..."), dtdMenu);
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
}


StructTreeView::~StructTreeView(){
}

/** create items in the level */
void StructTreeView::createList(Node *node, StructTreeTag *parent, int openLevel )
{
	if ( !node ) return;

	if ( !parent ) {
		top = new StructTreeTag( this, i18n("Document Structure") );
    groupsCount = m_parsingDTD->structGroups.count();
    QString name;
    for (uint i=0; i < groupsCount; i++)
    {
      name = m_parsingDTD->structGroups[i];
      QString iconName = name.section('/',2,2);
      name = name.left(name.find('/')).stripWhiteSpace();
      groups[i] = new StructTreeTag(this, i18n(name));
      groups[i]->setPixmap(0, SmallIcon(iconName));
    }
		createList(node, top, openLevel-1 );
		top->setOpen( topOpened );
    for (uint i = 0; i < groupsCount; i++)
    {
      groups[i]->setOpen(groupOpened[i]);
    }
		return;
	}
  Node *currentNode = node;
  Node *lastNode;
  while (currentNode)
  {
    lastNode = currentNode;
    currentNode = currentNode->next;
  }
  
  currentNode = lastNode;
  while (currentNode)
  {
   StructTreeTag *item = 0L;
   {
     if (currentNode->tag->type == Tag::XmlTag)
     {
       //if the xml tag is part of a group, show it under the group
       QString groupTagName;
       for (uint i = 0; i < groupsCount; i++)     
       {
         groupTagName = m_parsingDTD->structGroups[i];
         groupTagName = groupTagName.mid(groupTagName.findRev('/')).stripWhiteSpace();
         groupTagName = groupTagName.mid(2, groupTagName.length()-3);
         QStringList tl = QStringList::split(',',groupTagName);
         if (currentNode->tag->name.lower() == tl[0])
         {
           QString text="";
           for (uint j = 1; j < tl.count(); j++)        
           {
             if (currentNode->tag->hasAttribute(tl[j]))
             {           
               text += currentNode->tag->attributeValue(tl[j]).left(50) + " | ";
             }
           }
           text = text.left(text.length()-3);
           item = new StructTreeTag(groups[i], currentNode, text);
         }         
       }
       /*
       //add the new xml tags to the userTagList       
       if (! QuantaCommon::isKnownTag(m_parsingDTD->name, currentNode->tag->name))
       {
         Document *w = currentNode->tag->write();       
         QTag *newTag = w->userTagList.find(currentNode->tag->name);
         bool insertNew = !newTag;     
         if (insertNew)
         {
           newTag = new QTag();
           newTag->setName(currentNode->tag->name);
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
             w->userTagList.insert(currentNode->tag->name, newTag);                 
       }*/
       item = new StructTreeTag( parent, currentNode, currentNode->tag->name );
     }

      if ( currentNode->tag->type == Tag::Text )
      {
        QString text = currentNode->tag->tagStr();      
        text = text.left(70).stripWhiteSpace();
        text.replace( QRegExp("&nbsp;|\\n")," ");
        item = new StructTreeTag(parent,currentNode,text);
      }
      if ( currentNode->tag->type == Tag::Comment )
      {
        item = new StructTreeTag( parent, currentNode, "comment" );
        QString text = currentNode->tag->tagStr();
        text.replace( QRegExp("&nbsp;|\\n")," ");
        text.replace(QRegExp("<!--"),"");
        text.replace(QRegExp("-->"),"");
        text.replace(QRegExp("/*"),"");
        text.replace(QRegExp("*/"),"");
        text.replace(QRegExp("^//"),"");
        text.replace(QRegExp("^#"),"");
        item->setText(0, text.stripWhiteSpace());
      }
      if ( currentNode->tag->type == Tag::CSS )
      {
        item = new StructTreeTag( parent, currentNode, "css" );
        QString text = currentNode->tag->tagStr();
        text.replace( QRegExp("&nbsp;|\\n")," ");
        text.replace(QRegExp("<!--"),"");
        text.replace(QRegExp("-->"),"");
        item->setText(0, text.stripWhiteSpace());
      }
      if ( currentNode->tag->type == Tag::ScriptStructureBegin )
      {
        QString text = currentNode->tag->name;

        item = new StructTreeTag(parent,currentNode,currentNode->tag->name);
      }
   }
   if (currentNode->child)
   {
     createList(currentNode->child, item, openLevel - 1);
     if (item && item->node->opened) item->setOpen( true );
     if (item && useOpenLevelSetting) item->setOpen(openLevel > 0);
   }

    if ( m_parsingDTD->family == Script &&
        ( currentNode->tag->type == Tag::Text ||
          currentNode->tag->type == Tag::ScriptStructureBegin) )
    {
      //parse the node for groups (function/variable/inclusion/etc.) definitions
      QString text;
      QString tagStr = currentNode->tag->tagStr();       
      QRegExp rx;
      for (uint i = 0; i < groupsCount; i++)
      {
        int pos = 0;
        while (pos != -1)
        {
          rx.setPattern(m_parsingDTD->groupsRxs[i]);
          pos = rx.search(tagStr, pos);
          if (pos != -1)
          {
            text = rx.cap(0);
            Tag *newTag = new Tag(*currentNode->tag);
            int n = tagStr.left(pos).contains('\n');
            int bl, bc, el, ec;
            newTag->beginPos(bl,bc);
            bl = bl + n;
            bc = 0;
            el = bl+1;
            ec = -1;
            newTag->setTagPosition(bl, bc, el, ec);
            newTag->setStr(text);
            pos += text.length();
            rx.setPattern(m_parsingDTD->groupsClearRxs[i]);
            text.replace(rx,"");
            Node *node = new Node(currentNode);
            node->prev = 0;
            node->next = 0;
            node->tag = newTag;
            
            //Find the group and the item (if there is already an item with this
            //name) under we should insert the new node
            QListViewItem *insertUnder = groups[i];
            QListViewItemIterator it(groups[i]);
            while ( it.current() ) 
            {      
              if (it.current()->text(0) == text) //we have already an item with this text
              {
                insertUnder = it.current();
                break;
              }
              ++it;
            }
            
            int bl2, bc2;
            //If the new node's tag points before the parent's tag in
            //the document, switch them!
            StructTreeTag* t = dynamic_cast<StructTreeTag*>(insertUnder);
            if (t->node) //check if it's a toplevel group or not
            {
              t->node->tag->beginPos(bl2, bc2);
              if (bl2 > bl)
              {
                Node *tmpNode = t->node;
                t->node = node;
                node = tmpNode;
              }
            }
            
            //now do some sorting by finding the item after we insert the new node.
            //In case of toplevel nodes (just under the group) the sorting is
            //alphabetical, and for items with the same text, 
            //the sorting is done according to the tag->beginLine attribute
            QListViewItem *item2 = insertUnder->firstChild();
            QListViewItem *insertAfter = 0L;
            while (item2)
            {
              Tag *tag = dynamic_cast<StructTreeTag*>(item2)->node->tag;
              tag->beginPos(bl2, bc2);
              QString text2 = item2->text(0);
              if ( (text == text2 && bl2 < bl) || 
                   (text != text2 && item2->text(0).lower() < text.lower()) )
              {
                insertAfter = item2;
                item2 = item2->nextSibling();
              } else
              {
                break;
              }
            }
            
            item = new StructTreeTag(dynamic_cast<StructTreeTag*>(insertUnder), node, text, insertAfter);
          }
        }
      }
    }
   
   currentNode = currentNode->prev;
  }
}

/** Delete the items */
void StructTreeView::deleteList()
{
	if ( top ) {
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
void StructTreeView::slotReparse(Node* node, int openLevel)
{
  deleteList();
  write = node->tag->write();
	createList(node,0L,openLevel);
  for (uint i = 0; i < groupsCount; i++)
  {
    if (groups[i]->childCount() == 0)  
    {
      QString noGroup = m_parsingDTD->structGroups[i].section('/',1,1);
      groups[i]->setText(0, i18n(noGroup));
    }
  }
  useOpenLevelSetting = false;
}

void StructTreeView::slotGotoTag( QListViewItem *item )
{
  StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
  if (!it || !it->node) return;

  int line, col;
  it->node->tag->beginPos(line, col);
  emit newCursorPosition( line, col);
}


void StructTreeView::slotMouseClicked(int button, QListViewItem *item, const QPoint& point, int)
{
  if ( !item ) return;

  config->setGroup("Parser options");

  QString handleMBM = config->readEntry("MBM", i18n("Select tag area"));
  QString handleLBM = config->readEntry("LBM", i18n("Find tag"));
  QString handleDoubleClick = config->readEntry("Double click", i18n("Select tag area"));

  setSelected(item, true);

  if ( button == Qt::RightButton ) {
      popupMenu->popup( point);
      return;
  }

  if ( button == Qt::LeftButton ) {

      if ( handleLBM == i18n("Find Tag && Open Tree"))
          setOpen( item, ! isOpen(item) );
      setSelected(item, true);
      slotGotoTag(item);
  }

  if ( button == Qt::MidButton ) {

      if ( handleMBM == i18n("nothing"))
          return;

      if ( handleMBM == i18n("Find Tag && Open Tree")) {
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


void StructTreeView::slotDoubleClicked( QListViewItem *)
{
    config->setGroup("Parser options");

    if ( config->readEntry("Double click") == i18n("nothing") )
        return;

    slotSelectTag();
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
	if (!it || !it->node)
		return;

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

void StructTreeView::slotSelectTag()
{
  QListViewItem *item = currentItem();
	StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
	if (!it || !it->node) return;
  int bLine, bCol, eLine, eCol;
  Tag *tag = it->node->tag;
  if (tag->single || !it->node->next)
  {
    tag->endPos(eLine, eCol);
  } else
  {
    if (tag->closingMissing)
    {
      Node *node = it->node;
      while (node->child) node = node->child;
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


/** Do the recursive opening or closing of the trees */
void StructTreeView::setOpenSubTree( QListViewItem *it, bool open)
{
   if ( !it ) return;

   it->setOpen(open);
   setOpenSubTree( it->nextSibling(), open );
   setOpenSubTree( it->firstChild(), open );
}

/** Recursively open the tree and all its subtrees */
void StructTreeView::slotOpenSubTree()
{
	QListViewItem *item = currentItem();
	if ( !item ) return;
	item->setOpen( true );
  setOpenSubTree( item->firstChild(), true );
}


/** Recursively close the tree and all its subtrees */
void StructTreeView::slotCloseSubTree()
{

	QListViewItem *item = currentItem();
	if ( !item ) return;
	item->setOpen( false );
  setOpenSubTree( item->firstChild(), false );
}

/** Show the element in tree according to cursor position (x,y) */
void StructTreeView::showTagAtPos(int x, int y)
{
  if ( followCursorFlag )
  {
    QListViewItemIterator it(this);
    StructTreeTag *curTag = 0L, *item;
    for ( ; it.current(); ++it )
    {
      item = dynamic_cast<StructTreeTag *>(it.current());
      if ( item  && item->node && item->node->tag)
      {
       QString s = item->text(0);
        Tag *tag = item->node->tag;
        int bLine, bCol, eLine, eCol;
        tag->beginPos(bLine,bCol);
        tag->endPos(eLine, eCol);
        if (QuantaCommon::isBetween(x,y,bLine,bCol+1,eLine,eCol) == 0)
        {
         curTag = item;
        }
      }
    } //for

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
	if (!it || !it->node)
		return;
  it->node->opened = true;
}

/** No descriptions */
void StructTreeView::slotCollapsed(QListViewItem *item)
{
	StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
	if (!it || !it->node)
		return;
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
  QDictIterator<DTDStruct> it(*dtds);
  int index = 0;
  for( ; it.current(); ++it )
  {
    dtdMenu->setItemChecked(index, it.current()->name == dtdName); 
    index++;
  }
  m_parsingDTD = dtds->find(dtdName); //this should always exist
}


