
#include "messageitem.h"


MessageItem::MessageItem( QListBox * listbox, const QString &text, int line, const QString &fname )
  :QListBoxText(listbox,text),lineNumber(line),filename(fname)
{

}
  
