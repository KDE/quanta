
#include "messageitem.h"
#include <qregexp.h>


MessageItem::MessageItem( QListBox * listbox, const QString &text, int line, const QString &fname )
  :QListBoxText(listbox,text),lineNumber(line),filename(fname)
{
  lineDefined = ( line!=-1 );
}
  
int MessageItem::line()
{
  debug("line");
  if ( lineDefined ) {
    return lineNumber;
  }
  else {  // try to find first number in text
    QString s = text();
    int pos = s.find( QRegExp("[0-9]"));
    if ( pos == -1 )
      return lineNumber;
    int pos2 = pos;
    while ( s[pos2].isDigit() ) pos2++;
    
    QString snum = s.mid( pos, pos2-pos );
    debug("snum:\""+snum+"\"");
    return snum.toInt();
    
  }
}
