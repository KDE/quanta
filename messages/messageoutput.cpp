/***************************************************************************
                          messageoutput.cpp  -  description
                             -------------------
    begin                : Thu Feb 24 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski <pdima@mail.univ.kiev.ua>
                           (C) 2003 Andras Mantia <amantia@kde.org>
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


#include "messageoutput.moc"
