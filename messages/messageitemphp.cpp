#include "messageitemphp.h"
#include <qpainter.h>

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
   QString l4 = getLine(text); // frames: ( if present )
   
   
   l1.remove(0,7);
   l2.remove(0,9);
   l3.remove(0,10);
   
   l1 = l1.stripWhiteSpace();
   l3 = l3.stripWhiteSpace();
   l4 = l4.stripWhiteSpace();
   
   int sepPos = l3.find(":"); // separatorof fname and line
   if ( sepPos != -1 ) {
      QString fname = l3.left(sepPos);
      QString sline = l3.mid(sepPos+1,100);
      if ( fname.find("(null)") == -1 )
        setFileName(fname);
      bool ok = true;
      int line = sline.toInt(&ok);
      if ( ok )
        setLine(line);
      
   }
   
   
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
   
   if ( l4.left(7) == "frames:" ) {
     l4.remove(0,8);
     bool ok = true;
     int fCount = l4.toInt( &ok );
     
     if ( ok ) 
        for ( int i=0; i<fCount; i++ ) {
          QString func = getLine(text);
          QString location = getLine(text);
          func.remove(0,10);
          location.remove(0,10);
          QString mes = "   from : "+func+" : "+location;
          
          QString fname;
          int line;
          int sepPos = location.find(":"); // separatorof fname and line
          if ( sepPos != -1 ) {
             fname = location.left(sepPos);
             QString sline = location.mid(sepPos+1,100);
             if ( fname.find("(null)") != -1 )
                fname =  QString::null;
             bool ok = true;
             line = sline.toInt(&ok);
             if ( !ok )
               line = -1;
          }      
          
          new MessageItem( listBox(), mes, line, fname);
        
        }
   }

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
