/***************************************************************************
                          phpdebugsocket.cpp
                             -------------------
    begin                : 2004-03-12
    copyright            : (C) 2004 Linus McCabe <linus@mccabe.nu>
    Based on work by Mathieu Kooiman
 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *                     
 *                                                                          *
 ***************************************************************************/

#include "phpdebugsocket.h"
#include <kdebug.h>
#include <klocale.h>

#include "quanta.h"
#include "resource.h"

// CTor
PHPDebugSocket::PHPDebugSocket(PHPDebuggerInterface * iface)
{
  // Save pointer to debugger interface
  m_interface = iface;
}

