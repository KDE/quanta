
/***************************************************************************
                          xsldbgnotifier.h  -  description
                             -------------------
    begin                : Thu Dec 20 2001
    copyright            : (C) 2001 by keith
    email                : keith@linux
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef XSLDBGNOTIFIER_H
#define XSLDBGNOTIFIER_H

#include "xsldbgmsg.h"

#ifndef __cplusplus
#error "Must only be used with a c++ compiler"
#endif



/**
  *@author keith
  */

class XsldbgNotifier {
  public:
    XsldbgNotifier(void);
      virtual ~ XsldbgNotifier(void);

    virtual void doNotify(XsldbgMessageEnum type, const void *data);
};



/* get the notifer  */
void setNotifier(XsldbgNotifier * notifier);


#endif
