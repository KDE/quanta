/***************************************************************************
                          tagxml.cpp  -  description
                             -------------------
    begin                : Пнд Сен 25 14:34:07 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev
    email                : pdima@users.sourceforge.net,yshurik@linuxfan.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tagattr.h"

#include <qlineedit.h>
#include "colorcombo.h"


#include <qdom.h>


QString Attr::attrName()
{
	return name;
}


Attr_list::Attr_list( QDomElement *el, QWidget *w )
	: Attr(el,w)
{
	 combo = (QComboBox *)w;
	
	 		
   for ( QDomElement n = el->firstChild().toElement(); !n.isNull(); n = n.nextSibling().toElement() ) {
  	  if ( n.tagName() == "items" ) {
  	     QDomElement item = n.firstChild().toElement();
  	     while ( !item.isNull() ) {
  	     		combo->insertItem( item.text() );
  	     		item = item.nextSibling().toElement();
  	     }
  	
  	  }
   }

   setValue("");
}

void Attr_list::setValue( QString s )
{
	
	for ( int i=0; i<combo->count(); i++ )
		if ( s == combo->text(i) ) {
			combo->setCurrentItem(i);
			return;
		}
		
	combo->insertItem(s);
	combo->setCurrentItem( combo->count() - 1 );
	
}


QDomNode findChild( QDomNode &parent, QString name )
{
	for ( QDomNode n = parent.firstChild(); !n.isNull(); n = n.nextSibling() )
    if ( n.nodeName() == name )
  	     return n;  	
  return QDomNode();
}

