#ifndef MESSAGEITEMWEBLINT_H
#define MESSAGEITEMWEBLINT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "messageitem.h"

class MessageItemWebLint : public MessageItem {

public:

  MessageItemWebLint(QListBox *listbox, const QString &text);
  ~MessageItemWebLint();
  
//protected:
//  virtual void paint( QPainter * );
  
};

#endif

