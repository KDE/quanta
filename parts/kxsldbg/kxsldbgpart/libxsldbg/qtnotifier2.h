/***************************************************************************
                          qtnotifier.h  - notify the qt app of changes
                             -------------------
    begin                : Sun Dec 23 2001
    copyright            : (C) 2001 by Keith Isdale
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

#ifndef QTNOTIFIER_H
#define QTNOTIFIER_H

#if defined WIN32
#include <libxsldbg/xsldbgwin32config.h>
#endif

#include "xsldbgnotifier.h"


class  XsldbgDebuggerBase;

extern "C" {

 void connectNotifier(XsldbgDebuggerBase *debugger);

}

#endif
