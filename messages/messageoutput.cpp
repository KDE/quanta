/***************************************************************************
                          messageoutput.cpp  -  description
                             -------------------
    begin                : Thu Feb 24 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski
                           (C) 2003 Andras Mantia <amantia@freemail.hu>
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

#include <klocale.h>

#include "messageoutput.h"
#include "messageitem.h"
#include "messageitemphp.h"
#include "messageitemphp4.h"

MessageOutput::MessageOutput(QWidget *parent, const char *name )
  : QListBox(parent,name)
{
  max_items = 200;

  QPalette pal = palette();
  pal.setColor(QColorGroup::HighlightedText, pal.color(QPalette::Normal, QColorGroup::Text));
  pal.setColor(QColorGroup::Highlight,       pal.color(QPalette::Normal, QColorGroup::Mid));
  setPalette(pal);
  setFocusPolicy( NoFocus );

  connect( this, SIGNAL(selected(QListBoxItem*)), SLOT(clickItem(QListBoxItem*)) );
}

MessageOutput::~MessageOutput()
{
}

void MessageOutput::insertItem(const QString& s)
{
  checkMaxItems();
  new MessageItem(this, s);
  setBottomItem(count()>0?count()-1:0);
}

void MessageOutput::addToLastItem(const QString& s)
{
  int ind = count()-1;
  if ( ind != -1 ) {
    MessageItem *it = dynamic_cast<MessageItem*>( item(ind) );
    if ( it )
      it->addText( s );
    else
      changeItem( text( ind )+ s, ind );
  }
}


void MessageOutput::showMessage(const QString& s)
{
  QString message = s;
  int endPos;
  if ( !count() )
    insertItem("");
  while ( ( endPos = message.find('\n') ) != -1 ) {
    addToLastItem( message.left(endPos) );
    insertItem("");
    message.remove(0,endPos+1);
  }
  if (!message.isEmpty())
      addToLastItem( message);
  setBottomItem(count()>0?count()-1:0);
}


void MessageOutput::checkMaxItems()
{
  if ( count() >= max_items )
    removeItem( index(firstItem()) );
}

void MessageOutput::clickItem( QListBoxItem * l_item )
{
   MessageItem *item = dynamic_cast<MessageItem*>(l_item);
   if ( item )  {
     if ( item->line() != -1  )
       emit clicked( item->fileName(), item->line()-1 );
   }
}

void MessageOutput::phpDebug(const QString& s)
{
   static QString data = "";
   data += s;
   int pos1;
   QString res;

   if ( (pos1=data.find(") end: ")) != -1 )  {
     res = data.left(pos1+7);
     data.remove(0, pos1+7);
     if ( (pos1=res.find(") start: ")) != -1  )
       res.remove(0,pos1-1 );
     new MessageItemPHP( this, res );
     setBottomItem(count()>0?count()-1:0);
   }

}

void MessageOutput::php4Debug(const QString& s)
{
  new MessageItemPHP4( this, s );
  setBottomItem(count()-1);
}

void MessageOutput::newPhpConnect()
{
   insertItem(i18n("PHP debugger started"));
}

void MessageOutput::endPhpConnect()
{
   insertItem("");
}

#include "messageoutput.moc"
