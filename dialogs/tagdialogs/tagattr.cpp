/***************************************************************************
                          tagxml.cpp  -  description
                             -------------------
    begin                : Пнд Сен 25 14:34:07 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev <pdima@users.sourceforge.net,yshurik@linuxfan.com>
                           (C) 2004 Andras Mantia <amantia@kde.org>
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

#include <kdebug.h>
#include <klineedit.h>

#include "node.h"
#include "colorcombo.h"
#include "qtag.h"


#include <qdom.h>

extern GroupElementMapList globalGroupMap;

QString Attr::attrName() const
{
  return name;
}


Attr_list::Attr_list( QDomElement *el, QWidget *w, QTag *dtdTag )
  : Attr(el, w, dtdTag)
{
   combo = (QComboBox *)w;
   
   QString source = el->attribute("source");
   if (!source.isEmpty())
   {
      GroupElementMapList::Iterator it;
      for ( it = globalGroupMap.begin(); it != globalGroupMap.end(); ++it )
      {
        QString key = it.key();
        if (key.startsWith("Selectors|"))
        {
          QString selectorName = key.mid(10);
          kdDebug() << "selector = " << selectorName << endl;
          QString tmpStr;
          int index = selectorName.find(QRegExp("\\.|\\#|\\:"));          
          if (index != -1)
          {
            tmpStr = selectorName.left(index).lower();
          } else
          {
            tmpStr = selectorName;
          }
            kdDebug() << "tmpStr = " << tmpStr << " name = " << m_dtdTag->name().lower() << endl;
            if (tmpStr.isEmpty() || m_dtdTag->name().lower() == tmpStr || tmpStr == "*")
            {
              combo->insertItem(selectorName.mid(index + 1).replace('.',' '));
            }
         }
       }
   }
      
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

