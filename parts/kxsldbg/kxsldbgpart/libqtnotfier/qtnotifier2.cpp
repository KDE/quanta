/***************************************************************************
                          qtnotifier2.cpp  -  description
                             -------------------
    begin                : Sun Dec 23 2001
    copyright            : (C) 2001 by keith
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qapplication.h>

#include "../libxsldbg/xsldbgevent.h"
#include "../libxsldbg/qtnotifier2.h"
#include "../libxsldbg/xsldbgdebuggerbase.h"
#include "../libxsldbg/xsldbgthread.h"

XsldbgDebuggerBase *_debugger = 0L;

int notifyXsldbgApp(XsldbgMessageEnum type, const void *data)
{
    int result = 0;
    if (::getThreadStatus() == XSLDBG_MSG_THREAD_NOTUSED){
      return 1;
    }

    /* clear the input ready flag as quickly as possible*/
     if ( ::getInputStatus() == XSLDBG_MSG_READ_INPUT)
     	  ::setInputReady(0);

     /* state of the thread */
     if (_debugger != 0L){
       XsldbgEvent *e = new XsldbgEvent(type, data);
       if (e != 0L) {
	 /* The application will now have this event in its event queue
	    that all that is needed from here*/
	 QApplication::postEvent(_debugger, e);
       }
     }

    if (::getThreadStatus() == XSLDBG_MSG_THREAD_STOP)
        ::xsldbgThreadCleanup();        /* thread has died so cleanup after it */

    result++; /* at the moment this function will always work */

    return result;
}



void connectNotifier(XsldbgDebuggerBase *debugger){
  _debugger = debugger;
}


