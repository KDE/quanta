#ifndef MESSAGEITEMPHP4_H
#define MESSAGEITEMPHP4_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "messageitem.h"
#include <qcolor.h>


class MessageItemPHP4 : public MessageItem {

public:

  MessageItemPHP4(QListBox *listbox, const QString &text);
  ~MessageItemPHP4();

protected:
//  virtual void paint( QPainter * );

private:

  QString stype;
  QString message;

};


#endif

