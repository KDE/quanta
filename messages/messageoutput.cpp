/***************************************************************************
                          messageoutput.cpp  -  description
                             -------------------
    begin                : Thu Feb 24 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski
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

#include "messageoutput.h"
#include "messageitem.h"
#include "messageitemphp.h"
#include "messageitemweblint.h"

MessageOutput::MessageOutput(QWidget *parent, const char *name )
  : QListBox(parent,name)
{
  insertItem( "Message window..." );
  max_items = 200;
  
  QPalette pal = palette();
  pal.setColor(QColorGroup::HighlightedText, pal.color(QPalette::Normal, QColorGroup::Text));
  pal.setColor(QColorGroup::Highlight,       pal.color(QPalette::Normal, QColorGroup::Mid));
  setPalette(pal);
  setFocusPolicy( NoFocus );
  
  connect( this, SIGNAL(pressed(QListBoxItem*)), SLOT(clickItem(QListBoxItem*)) );
  
}

MessageOutput::~MessageOutput()
{
}

void MessageOutput::insertItem(QString s)
{
  checkMaxItems();
  new MessageItem(this,s);
  setBottomItem(count()-1);
}

void MessageOutput::showMessage( QString message ) 
{
  int endPos;
  if ( message.right(1) == "\n" ) 
    message.remove( message.length()-1,1 );
    
  if ( message.left(1) == "\n" ) 
    message.remove( 0,1 );
  
  
  while ( ( endPos = message.find('\n') ) != -1 ) {
    insertItem( message.left(endPos) );
    message.remove(0,endPos+1);
  }
  insertItem( message );
  setBottomItem(count()-1);
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
       emit clicked( item->fileName(), item->line() );  
   }
}

void MessageOutput::phpDebug( QString s)
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
     setBottomItem(count()-1);
   }
   
}

void MessageOutput::newPhpConnect()
{
   insertItem("PHP debugger started");
}

void MessageOutput::endPhpConnect()
{
   insertItem("");
}

// if p!=0 store output, else show it
void MessageOutput::processWebLint( KProcess *p, char *buffer, int len )
{
  static QString s = "";
  
  if ( p ) {
    QString text(buffer);
    text.truncate(len);
    s += text;
  } 
  else {
    int endPos;
    if ( s.right(1) == "\n" ) 
      s.remove( s.length()-1,1 );
    if ( s.left(1) == "\n" ) 
      s.remove( 0,1 );
    
    while ( ( endPos = s.find('\n') ) != -1 ) {
      new MessageItemWebLint( this, s.left(endPos) );
      s.remove(0,endPos+1);
    }
    new MessageItemWebLint( this, s );
   // setBottomItem(count()-1);
    s = ""; 
  }
       
}

void MessageOutput::weblintFinished() 
{
   clear();
   processWebLint(0,0,0); // show output
}
