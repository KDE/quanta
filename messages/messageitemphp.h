#ifndef MESSAGEITEMPHP_H
#define MESSAGEITEMPHP_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "messageitem.h"
#include <qcolor.h>


class MessageItemPHP : public MessageItem {

public:

  MessageItemPHP(QListBox *listbox, const QString &text);
  ~MessageItemPHP();
  
private:
  void parseData( QString text);
  
protected:
  virtual void paint( QPainter * );

private:
  
  enum { E_WARNING = 0, E_ERROR, E_PARSE, E_NOTICE, E_CORE_ERROR, E_CORE_WARNING, E_UNKNOWN };
  
  QString getLine( QString &s ); // remove first line ( before \n ) from s and return it ( line )
  
  QColor colors[7];
  QString stype;
  
  int type;
  QString message;

};


#endif

