#include "messageitemphp4.h"
#include <qstring.h>

MessageItemPHP4::MessageItemPHP4( QListBox *listbox, const QString &text )
  :MessageItem( listbox )
{
    QString s = text;
    
    int pos1  = s.find(":");
    if ( pos1 == -1 ) return;
      
    QString fname = s.left(pos1);
    if ( !fname.isEmpty() ) 
      setFileName(fname);
      
    int pos2 = s.find( ":", pos1+1 );
    if ( pos2 == -1 ) return;
    
    QString sline = s.mid( pos1+1, pos2-pos1-1);
    bool ok;
    int iline = sline.toInt(&ok);
    if (ok) 
      setLine(iline);
    
    setText(s);
}

MessageItemPHP4::~MessageItemPHP4() {}

/*
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
*/
