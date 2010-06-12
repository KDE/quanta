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


//app includes
#include "attributes.h"
#include "node.h"
#include "colorcombo.h"
#include "qtag.h"

//kde includes
#include <kapplication.h>
#include <kdebug.h>
#include <klineedit.h>

//qt includes
#include <qdom.h>
#include <QTextStream>

QString Attr::attrName() const
{
  return name;
}


Attr_list::Attr_list( QDomElement const& el, QWidget *w, QTag *dtdTag )
  : Attr(el, w, dtdTag)
{
   combo = (QComboBox *)w;

   QString source = el.attribute("source");
/*FIXME
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
        combo->addItems(list);
     }
   }
*/
   for ( QDomElement n = el.firstChild().toElement(); !n.isNull(); n = n.nextSibling().toElement() ) {
      if ( n.tagName() == "items" ) {
         QDomElement item = n.firstChild().toElement();
         while ( !item.isNull() ) {
             combo->addItem( item.text() );
             item = item.nextSibling().toElement();
         }
      }
   }

   setValue("");
}

void Attr_list::setValue(const QString &value)
{
  int i = combo->findText(value);
  if (i != -1)
    combo->setCurrentIndex(i);
  else
  {
    combo->addItem(value);
    combo->setCurrentIndex( combo->count() - 1 );
  }
}


QDomNode findChild( QDomNode &parent, const QString &name )
{
  for ( QDomNode n = parent.firstChild(); !n.isNull(); n = n.nextSibling() )
    if ( n.nodeName() == name )
         return n;
  return QDomNode();
}

