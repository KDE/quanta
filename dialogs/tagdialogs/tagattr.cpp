/***************************************************************************
                          tagxml.cpp  -  description
                             -------------------
    begin                : ����25 14:34:07 EEST 2000
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

//app includes
#include "tagattr.h"
#include "node.h"
#include "colorcombo.h"
#include "qtag.h"
#include "quantacommon.h"

QString Attr::attrName() const
{
  return name;
}


Attr_list::Attr_list( QDomElement const& el, QWidget *w, QTag *dtdTag )
  : Attr(el, w, dtdTag)
{
   combo = (QComboBox *)w;

   QString source = el.attribute("source");
   if (source == "dcop") //fill the list with a result of a DCOP call
   {
     QString method = el.attribute("method");
     QString interface = el.attribute("interface", "QuantaIf");
     QString arguments = el.attribute("arguments");
     arguments.replace("%tagname%", m_dtdTag->name());
     DCOPReply reply = QuantaCommon::callDCOPMethod(interface, method, arguments);
     if (reply.isValid())
     {
        QStringList list = reply;
        combo->insertStringList(list);
     }
   }

   for ( QDomElement n = el.firstChild().toElement(); !n.isNull(); n = n.nextSibling().toElement() ) {
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

void Attr_list::setValue(const QString &value)
{

  for ( int i=0; i<combo->count(); i++ )
    if ( value == combo->text(i) ) {
      combo->setCurrentItem(i);
      return;
    }

  combo->insertItem(value);
  combo->setCurrentItem( combo->count() - 1 );

}


QDomNode findChild( QDomNode &parent, const QString &name )
{
  for ( QDomNode n = parent.firstChild(); !n.isNull(); n = n.nextSibling() )
    if ( n.nodeName() == name )
         return n;
  return QDomNode();
}

