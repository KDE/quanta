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
		: QListView(parent,name)
{
	top = 0L;
	images = 0L;
	links = 0L;
	lastTag = 0L;
	followCursorFlag = true;
	this->config = config;

	this->parser = parser;

	topOpened = true;
	imagesOpened = false;
	linksOpened = false;
  useOpenLevelSetting = true;

	setRootIsDecorated( true );
  header()->hide();
  setSorting(-1,false);

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( i18n("Name") );

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
    if (m_parsingDTD->nickName.find("html", 0, false) != -1)
    {
  		images = new StructTreeTag( this, i18n("Images") );
  		images->setPixmap( 0, SmallIcon("image") );
  		links = new StructTreeTag( this, i18n("Links") );
  		links->setPixmap( 0, SmallIcon("www") );
    }
    if (m_parsingDTD->family == Script)
    {
  		images = new StructTreeTag( this, i18n("Variables") );
  		images->setPixmap( 0, SmallIcon("abs") );
  		links = new StructTreeTag( this, i18n("Functions") );
  		links->setPixmap( 0, UserIcon("mini-modules") );
    }

		createList(node, top, openLevel-1 );
		top->setOpen( topOpened );
		if (images) images->setOpen( imagesOpened );
		if (links) links->setOpen( linksOpened );
		return;
	}
  Node *currentNode = node;
  Node *lastNode;
  while (currentNode)
  { lastNode = currentNode;
    currentNode = currentNode->next;
  }
  currentNode = lastNode;
  while (currentNode)
  {
   StructTreeTag *item = 0L;
   {
     if (currentNode->tag->type == Tag::XmlTag)// || currentNode->tag->type == "xmltagend")
     {
      //HTML specific tags
      if (m_parsingDTD->nickName.find("html", 0, false) != -1)
      {
        if ( currentNode->tag->name.lower() == "img")
        {
          item = new StructTreeTag( images, currentNode, currentNode->tag->attributeValue("src").left(50) );
          imagesCount++;
        }
        if ( currentNode->tag->name.lower() == "a")
        {
          QString text = "";
          if ( currentNode->tag->hasAttribute("name") )
              text += currentNode->tag->attributeValue("name").left(50);

          if ( currentNode->tag->hasAttribute("href") )
              text += currentNode->tag->attributeValue("href").left(50);

          item = new StructTreeTag( links, currentNode, text );
          linksCount++;
        }
      } //end of html specific tags
      item = new StructTreeTag( parent, currentNode, currentNode->tag->name );
     }

      if ( currentNode->tag->type == Tag::Text )
      {
        QString text = currentNode->tag->tagStr();
        text = text.left(70);
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
        item->setText(0, text);
      }
      if ( currentNode->tag->type == Tag::CSS )
      {
        item = new StructTreeTag( parent, currentNode, "css" );
        QString text = currentNode->tag->tagStr();
        text.replace( QRegExp("&nbsp;|\\n")," ");
        text.replace(QRegExp("<!--"),"");
        text.replace(QRegExp("-->"),"");
        item->setText(0, text);
      }
      if ( currentNode->tag->type == Tag::ScriptStructureBegin )
      {
        QString text = currentNode->tag->name;
        QRegExp fnRx = QRegExp("function[\\s]*",false);
        if (text.contains(fnRx))
        {
          text.replace(fnRx,"");
          item = new StructTreeTag( links, currentNode, text);
          linksCount++;
        }
        item = new StructTreeTag(parent,currentNode,currentNode->tag->name);
      }
   }
   if (currentNode->child)
   {
     createList(currentNode->child, item, openLevel - 1);
     if (item && item->node->opened) item->setOpen( true );
     if (item && useOpenLevelSetting) item->setOpen(openLevel > 0);
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

	if ( images ) {
		imagesOpened = images->isOpen();
		delete images;
		images = 0L;
	}

	if ( links ) {
		linksOpened = links->isOpen();
		delete links;
		links = 0L;
	}

}

/** repaint document structure */
void StructTreeView::slotReparse(Node* node, int openLevel)
{
  deleteList();
	imagesCount = linksCount = 0;
  write = node->tag->write();
	createList(node,0L,openLevel);
  if (m_parsingDTD->nickName.find("html", 0, false) != -1)
  {
  	if ( !imagesCount )
  	  images->setText(0, i18n("No Images"));
  	if ( !linksCount )
  	  links->setText(0, i18n("No Links"));
  } else
    if (m_parsingDTD->family == Script)
    {
    	if ( !linksCount )
    	  links->setText(0, i18n("No Functions"));

     if ( write->variableList.count() )
     {
       for (int i = write->variableList.count()-1; i >=0; i--)
       {
         new StructTreeTag( images, write->variableList[i] );
       }
     } else
     {
       images->setText(0, i18n("No Variables"));
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


