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


//kde includes
#include <dcopref.h>
#include <kapplication.h> 
#include <kdebug.h>
#include <klineedit.h>

//qt includes
#include <qdom.h>
#include <qtextstream.h>

//system includes
#include <sys/types.h> 
#include <unistd.h> 

//app includes
#include "tagattr.h"
#include "node.h"
#include "colorcombo.h"
#include "qtag.h"

QString Attr::attrName() const
{
  return name;
}


Attr_list::Attr_list( QDomElement *el, QWidget *w, QTag *dtdTag )
  : Attr(el, w, dtdTag)
{
   combo = (QComboBox *)w;
   
   QString source = el->attribute("source");
   if (source == "dcop") //fill the list with a result of a DCOP call
   {
     QString method = el->attribute("method");
     QString interface = el->attribute("interface", "QuantaIf");
     QString arguments = el->attribute("arguments");
     QStringList argumentList = QStringList::split(",", arguments, true);
     for ( QStringList::Iterator it = argumentList.begin(); it != argumentList.end(); ++it ) 
     {
        (*it).replace("%tagname%", m_dtdTag->name());
     }
     QString app = "quanta";
     if (!kapp->inherits("KUniqueApplication"))
     {
       pid_t pid = ::getpid();
       app += QString("-%1").arg(pid);
     }
     DCOPRef quantaRef(app.utf8(), interface.utf8());
     DCOPReply reply;
     int argumentCount = argumentList.count();
     if (argumentCount == 0)
     {
       reply = quantaRef.call(method.utf8());
     }
     else if (argumentCount == 1)
     {
       reply = quantaRef.call(method.utf8(), argumentList[0]);
     }
     else if (argumentCount == 2)
       reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1]);
     else if (argumentCount == 3)
       reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1], argumentList[2]);
     else if (argumentCount == 4)
       reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3]);
     else if (argumentCount == 5)
       reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3], argumentList[4]);
     else if (argumentCount == 6)
       reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3], argumentList[4], argumentList[5]);
     else if (argumentCount == 7)
       reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3], argumentList[4], argumentList[5], argumentList[6]);
     else if (argumentCount == 8)
       reply = quantaRef.call(method.utf8(), argumentList[0], argumentList[1], argumentList[2], argumentList[3], argumentList[4], argumentList[5], argumentList[6], argumentList[7]);
      
     if (reply.isValid())
     {
        QStringList list = reply;
        combo->insertStringList(list);
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

