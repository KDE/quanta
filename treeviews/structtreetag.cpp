/***************************************************************************
                          structtreetag.cpp  -  description
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

// KDE includes
#include <kiconloader.h>

// app includes
#include "structtreetag.h"
#include "../parser/tag.h"
#include "../parser/node.h"


StructTreeTag::StructTreeTag(QListView *parent, QString name )
	: QListViewItem(parent,name)
{
	node = 0L;
	
}

StructTreeTag::StructTreeTag(StructTreeTag *parent, Node *p_node, QString name )
	: QListViewItem(parent,name)
{
	node = p_node;
	
  if (!node) return;
  Tag *tag = node->tag;
	QString sname(name);
	QString space = " ";
	
	if ( sname.left(4) == "font" ) {
		setPixmap( 0, UserIcon("tag_font_small") );
		if ( tag->attrCount )
			setText(0, space + tag->attribute(0) + "=" + tag->attributeValue(0));
		else
			setText(0,"");
	} else
	
	if ( sname.left(3) == "img" ) {
		setPixmap( 0, SmallIcon("image") );
		setText(0, space + tag->attributeValue("src") );
	} else
		
	if ( sname == "a" ) {
		setPixmap( 0, SmallIcon("www") );
		if ( tag->hasAttribute("href") )
			setText(0,space + "href "+ tag->attributeValue("href"));
		if ( tag->hasAttribute("name") )
			setText(0,space + "name "+ tag->attributeValue("name"));	
	} else
		
	/*
	if ( sname == "p") {
		setPixmap( 0, UserIcon("tag_p") );
		setText(0,"");
	}*/
	
	if ( sname == "br") {
		setPixmap( 0, UserIcon("tag_br_small") );
		setText(0,"");
	} else
	
	if ( sname == "hr") {
		setPixmap( 0, UserIcon("tag_hr_small") );
		setText(0,"");
	} else
	
	if ( sname == "li") {
		setPixmap( 0, UserIcon("ball") );
		setText(0,"");
	} else
	
	if ( sname == "php" ) {
		setText(0,"< php >");
	} else
	
  if ( sname == "comment" ) {
	  setPixmap( 0, UserIcon("tag_comm") );
		setText(0,"");
	}

//  setText(0, text(0)+QString(" ["+tag->tagStr()+" ; %1]").arg(tag->type)); //debug
}

StructTreeTag::StructTreeTag(StructTreeTag *parent, QString name )
	: QListViewItem(parent,name)
{
  node = 0L;
}


StructTreeTag::~StructTreeTag(){
}

/** set pixmap of tag */
//void StructTreeTag::setPixmap(QString name){
//}
