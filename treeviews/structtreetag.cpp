/***************************************************************************
                          structtreetag.cpp  -  description
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

// KDE includes
#include <kiconloader.h>

// app includes
#include "structtreetag.h"
#include "../parser/tag.h"


StructTreeTag::StructTreeTag(QListView *parent, const char *name )
	: QListViewItem(parent,name)
{
	pos1 = pos2 = 1;
	
}

StructTreeTag::StructTreeTag(StructTreeTag *parent, Tag *tag, const char *name )
	: QListViewItem(parent,name)
{
	pos1 = pos2 = 1;
	
	QString sname(name);
	QString space = " ";
	
	if ( sname.left(4) == "font" ) {
		setPixmap( 0, UserIcon("tag_font_small") );
		if ( tag->attrcount )
			setText(0, space + tag->attr[0] + "=" + tag->value[0]);
		else
			setText(0,"");
	}
	
	if ( sname.left(3) == "img" ) {
		setPixmap( 0, UserIcon("image") );
		setText(0, space + tag->attrValue("src") );
	}
		
	if ( sname == "a" ) {
		setPixmap( 0, UserIcon("html") );
		if ( tag->haveAttrib("href") )
			setText(0,space + "href "+ tag->attrValue("href"));
		if ( tag->haveAttrib("name") )
			setText(0,space + "name "+ tag->attrValue("name"));	
	}
		
	/*
	if ( sname == "p") {
		setPixmap( 0, UserIcon("tag_p") );
		setText(0,"");
	}*/
	
	if ( sname == "br") {
		setPixmap( 0, UserIcon("tag_br_small") );
		setText(0,"");
	}
	
	if ( sname == "hr") {
		setPixmap( 0, UserIcon("tag_hr_small") );
		setText(0,"");
	}
	
	if ( sname == "li") {
		setPixmap( 0, UserIcon("ball") );
	}
}

StructTreeTag::StructTreeTag(StructTreeTag *parent, const char *name )
	: QListViewItem(parent,name)
{
  pos1 = pos2 = 1;
}


StructTreeTag::~StructTreeTag(){
}

/** set pixmap of tag */
//void StructTreeTag::setPixmap(QString name){
//}
