#include "messageitemweblint.h"

MessageItemWebLint::MessageItemWebLint( QListBox *listbox, const QString &text )
  :MessageItem( listbox )
{
  QString s = text;
  int pos = s.find("(");
  if ( pos != -1 ) {
    s.remove(0,pos+1);
    pos = s.find(")");
    bool isNumber = true;
    int line = s.left(pos).toInt(&isNumber);
    if ( isNumber ) {
      setLine(line);
      s.remove(pos,1);
    }
  }
  setText(s);
}

MessageItemWebLint::~MessageItemWebLint() {}

