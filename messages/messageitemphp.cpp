#include "messageitemphp.h"

MessageItemPHP::MessageItemPHP( QListBox *listbox, const QString &text )
  :MessageItem( listbox )
{
  
  colors[ E_WARNING    ] = Qt::darkYellow;
  colors[ E_ERROR      ] = Qt::red;
  colors[ E_PARSE      ] = Qt::black;
  colors[ E_NOTICE     ] = Qt::blue;
  colors[ E_CORE_ERROR ] = Qt::red;
  colors[ E_CORE_WARNING]= Qt::darkYellow;
  colors[ E_UNKNOWN    ] = Qt::black;
  
  parseData(text);
}

MessageItemPHP::~MessageItemPHP() {}


void MessageItemPHP::parseData(QString text) 
{
   
   QString l1 = getLine(text); // start: notice
   QString l2 = getLine(text); // message:
   QString l3 = getLine(text); // location:
   
   l1.remove(0,7);
   l2.remove(0,9);
   l3.remove(0,10);
   
   l1 = l1.stripWhiteSpace();
   
   stype = l1;
   type = E_UNKNOWN;
   if ( l1 == "warning" ) type = E_WARNING;
   if ( l1 == "error" ) type = E_ERROR;
   if ( l1 == "notice" ) type = E_NOTICE;
   if ( l1 == "parse" ) type = E_PARSE;
   if ( l1 == "core-error" ) type = E_CORE_ERROR;
   if ( l1 == "core-warning" ) type = E_CORE_WARNING;
   
   message = l2 + " : " + l3;
   
   setText( stype + " : " + message );

}


QString MessageItemPHP::getLine( QString &s) 
{
  
  uint pos = s.find("\n");
  QString res;
  if ( pos == -1 ) {
    res = s;
    s = "";
  }
  else {
    res = s.left(pos);
    s.remove(0,pos+1);
  }
  
  uint pos2 = res.find( ")" );
  if ( pos2 != -1 )
    res.remove(0, pos2+2 );
  
  return res;
}


void MessageItemPHP::paint( QPainter *p )
{
  int x = 0;
  QFontMetrics fm = p->fontMetrics();
  
  p->setPen( colors[type] );
  
  p->drawText( 3,  fm.ascent() + fm.leading()/2, stype );
  x += fm.width( stype );
  p->setPen( Qt::darkBlue );
  p->drawText( 3 + x,  fm.ascent() + fm.leading()/2, " : " + message );

}
