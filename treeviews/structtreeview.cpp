/***************************************************************************
                          structtreeview.cpp  -  description
                             -------------------
    begin                : Sat Apr 29 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
    email                : pdima@mail.univ.kiev.ua
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

#include "structtreetag.h"
#include "structtreeview.h"


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
	
	setRootIsDecorated( true );
  header()->hide();
  setSorting(-1,false);

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( "Name" );

	setFocusPolicy(QWidget::ClickFocus);
	
	
	
/*	
	RBMenuFile = new KPopupMenu("Project's file");
	
	RBMenuFile -> insertItem( UserIcon("open"),  i18n("&Open"),       this, SLOT(slotFileOpen()), 0, ID_PROJECT_FILE_OPEN);
	RBMenuFile -> insertItem( i18n("&Insert tag"), this, SLOT(slotFileTag()), 0, ID_PROJECT_FILE_TAG);
	RBMenuFile -> insertSeparator();
	RBMenuFile -> insertItem( i18n("Remove file from project"), this, SLOT(slotRemoveFromProject()), 0, ID_PROJECT_REMOVE_FROM_PROJECT);
*/	

 	popupMenu = new QPopupMenu();
 	
	popupMenu -> insertItem( i18n("Select tag area"), this ,SLOT(slotSelectTag()));
	popupMenu -> insertItem( i18n("End of tag"), 		  this ,SLOT(slotGotoEndOfTag()));
	popupMenu -> insertSeparator();
	popupMenu -> insertItem( i18n("Open"), 		this ,SLOT(slotOpenSubTree()));
	popupMenu -> insertItem( i18n("Close"),		this ,SLOT(slotCloseSubTree()));
	popupMenu -> insertSeparator();
	popupMenu -> insertItem( UserIcon("repaint"),  i18n("&reparse"), 		this ,SLOT(slotReparse()));
	followCursorId = popupMenu -> insertItem( i18n("follow cursor"), this ,SLOT(changeFollowCursor()));
	
	popupMenu -> setItemChecked ( followCursorId, followCursor() );
	
	
	
	
//	connect(this, SIGNAL(clicked(QListViewItem *)), SLOT(slotFollowTag(QListViewItem *)));
	
	connect( this, SIGNAL(mouseButtonClicked(int, QListViewItem*, const QPoint&, int)),
					 this, SLOT  (slotMouseClicked(int, QListViewItem*, const QPoint&, int)));
					
	connect( this, SIGNAL(onItem(QListViewItem *)), SLOT(slotOnTag(QListViewItem *)));
	connect( this, SIGNAL(doubleClicked(QListViewItem *)), SLOT(slotDoubleClicked(QListViewItem *)));

}


StructTreeView::~StructTreeView(){
}

/** create items in the level */
void StructTreeView::createList(Node *node, StructTreeTag *parent, int openLevel )
{
	if ( !node ) return;

	if ( !parent ) {
		top = new StructTreeTag( this, "Document structure" );
		images = new StructTreeTag( this, "Images" );
		images->setPixmap( 0, SmallIcon("image") );
		links = new StructTreeTag( this, "Links" );
		links->setPixmap( 0, SmallIcon("www") );
		
		createList(node, top, openLevel-1 );
		top->setOpen( topOpened );
		images->setOpen( imagesOpened );
		links->setOpen( linksOpened );
		return;
	}
	
	Node *endnode = 0L;
	Node *tnode = node;
	tnode->prev = 0L;
	while ( tnode)
	{
		if ( tnode->next ) tnode->next->prev = tnode;
		else endnode = tnode;
		
		tnode = tnode->next;
	}
	
	tnode = endnode;
	while ( tnode )
	{
		StructTreeTag *item = 0L;
		if ( tnode->type == Node::tTag ){
//			item = new StructTreeTag( parent, tnode->tag, tnode->tag.name.data() );
//			item->pos = tnode->start;
			
			if ( tnode->tag->name == "img") {
				item = new StructTreeTag( images, tnode->tag, tnode->tag->attrValue("src").left(50) );
				item->pos1 = tnode->start-1;
				item->pos2 = tnode->end;
				imagesCount++;
			}
			
			if ( tnode->tag->name == "a") {
			  QString text = "";
			  
			  if ( tnode->tag->haveAttrib("name") )
			    text = tnode->tag->attrValue("name").left(50);
			    
			  if ( tnode->tag->haveAttrib("href") )
			    text = tnode->tag->attrValue("href").left(50);
			  
				item = new StructTreeTag( links, tnode->tag, text );
				item->pos1 = tnode->start-1;
				item->pos2 = tnode->end;
				linksCount++;
				
			}
			
			item = new StructTreeTag( parent, tnode->tag, tnode->tag->name.data() );
			item->pos1 = tnode->start-1;
			item->pos2 = tnode->end;

				
			//item->setPos( tnode->start );
		}
		else 
		  if ( tnode->type == Node::tText ) {
		    QString text = parser->s.mid( tnode->start , tnode->end - tnode->start + 1);
		    text = text.left(70);
		    text = text.replace( QRegExp("&nbsp;")," ");
		    int endlPos;
		    if ( ( endlPos = text.find('\n') ) != -1 )
		      text = text.left( endlPos );
		      
		    item = new StructTreeTag(parent,text);
		    item->pos1 = tnode->start-1;
				item->pos2 = tnode->end;
		  }
		  
		if ( tnode->child ) {
			createList( tnode->child, item ,openLevel-1);
			item->setOpen( openLevel > 0 );
		}
		
		tnode = tnode->prev;
	}

}

/** repaint document structure */
void StructTreeView::slotReparse(Node* node, int openLevel)
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
	
	if ( !node ) {
		top = 0L;
		return;
	}
	
	imagesCount = linksCount = 0;
	createList(node,0L,openLevel);
	if ( !imagesCount )
	  images->setText(0,"no Images");
	if ( !linksCount )
	  links->setText(0,"no Links");
}

void StructTreeView::slotFollowTag( QListViewItem *item )
{
	if ( !item ) return;
	StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
	if (!it)
		return;
		
	emit newCursorPosition( parser->pos2y( it->pos1 ), parser->pos2x( it->pos1 ) );	
}


void StructTreeView::slotMouseClicked(int button, QListViewItem *item, const QPoint& point, int)
{
  if ( !item ) return;

  config->setGroup("Parser options");
		
  QString handleMBM = config->readEntry("MBM","Find tag");
	QString handleLBM = config->readEntry("LBM","Find tag and open tree");
	QString handleDoubleClick = config->readEntry("Double click","Select tag area");

	setSelected(item, true);
	
	if ( button == Qt::RightButton ) {
		popupMenu->popup( point);
		return;
	}
	
	if ( button == Qt::LeftButton ) {
	
    if ( handleLBM == i18n("Find tag and open tree"))
    	setOpen( item, ! isOpen(item) );	
    setSelected(item, true);
    slotFollowTag(item);
  }

  if ( button == Qt::MidButton ) {
	
    if ( handleMBM == i18n("nothing"))
    	return;
    	
    if ( handleMBM == i18n("Find tag and open tree")) {
    	setOpen( item, ! isOpen(item) );	
    	setSelected(item, true);
    	slotFollowTag(item);
    }
    	
    if ( handleMBM == i18n("Select tag area"))
    	slotSelectTag();

    if ( handleMBM == i18n("Go to end of tag"))
    	slotGotoEndOfTag();
    	
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
  emit needReparse();
}

void StructTreeView::slotGotoEndOfTag()
{
  QListViewItem *item = currentItem();

	if ( !item ) return;
	StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
	if (!it)
		return;
		
	emit newCursorPosition( parser->pos2y( it->pos2 ), parser->pos2x( it->pos2 ) );	

}

void StructTreeView::slotSelectTag()
{
  QListViewItem *item = currentItem();

	if ( !item ) return;
	StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
	if (!it)
		return;
		
	emit selectArea( parser->pos2x( it->pos1 ), parser->pos2y( it->pos1 ),
	                 parser->pos2x( it->pos2 ), parser->pos2y( it->pos2 ) );	
	
  setSelected(item, true);
}

void StructTreeView::slotOnTag( QListViewItem * item)
{

	if ( !item ) return;
	StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
	if (!it)
		return;
		
	QString text = parser->s.mid( it->pos1+1, it->pos2 - it->pos1 );
  text = text.left(70);
  text = text.replace( QRegExp("&nbsp;")," ");
  int endlPos;
  if ( ( endlPos = text.find('\n') ) != -1 )
     text = text.left( endlPos );

  emit onTag( text );
		
}

void StructTreeView::setOpenSubTree( QListViewItem *it, bool open)
{
   if ( !it )
    	return;
   	
   it->setOpen(open);
   setOpenSubTree( it->nextSibling(), open );
   setOpenSubTree( it->firstChild(), open );
}

void StructTreeView::slotOpenSubTree()
{

	QListViewItem *item = currentItem();

	if ( !item ) return;
	StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
	if (!it)
		return;
		
	it->setOpen( true );	
  setOpenSubTree( it->firstChild(), true );
}


void StructTreeView::slotCloseSubTree()
{

	QListViewItem *item = currentItem();

	if ( !item ) return;
	StructTreeTag *it = dynamic_cast<StructTreeTag*>(item);
	if (!it)
		return;
		
	it->setOpen( false );	
  setOpenSubTree( it->firstChild(), false );
}

void StructTreeView::showTagAtPos(int x, int y)
{
  if ( !followCursor() )
  	return;
  	
	int pos = parser->xy2pos(x,y);
	
	QListViewItemIterator it(this);
	
	StructTreeTag *curTag = 0L, *tTag;
	
	for ( ; it.current(); ++it ) {
	    tTag = dynamic_cast<StructTreeTag *>(it.current());
      if ( tTag ) {
        if ( pos > tTag->pos1 && pos < tTag->pos2 )
        	curTag = tTag;
      }
  }

  if ( lastTag == curTag || !curTag )
  	return;
  	
  lastTag = curTag;
  	
  ensureItemVisible(lastTag);
  setSelected(lastTag, true);
}

void StructTreeView::setFollowCursor(bool follow)
{
   followCursorFlag = follow;
   popupMenu -> setItemChecked ( followCursorId, follow );
}

