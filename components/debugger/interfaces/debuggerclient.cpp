/***************************************************************************
                          debuggerclient.cpp
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
 
#include "debuggerclient.h"
#include "debuggerinterface.h"
#include <kdebug.h>
#include <klocale.h>

// CTor
DebuggerClient::DebuggerClient(QObject *parent, const char* name) 
 : QObject(parent, name)
{
  m_active = false;
}

// DTor
DebuggerClient::~DebuggerClient() 
{
}

DebuggerInterface* DebuggerClient::debuggerInterface() 
{
  return static_cast<DebuggerInterface*>( parent()->child( 0, "DebuggerInterface" ) );
}

// Active state of session
bool DebuggerClient::isActive()
{
  return m_active;
}

// Unimplemented defaults - Pause execution
bool DebuggerClient::pause()
{
   return false;
}

// Unimplemented defaults - step over
bool DebuggerClient::stepOver()
{
   return false;
}

// Unimplemented defaults - step out
bool DebuggerClient::stepOut()
{
   return false;
}

// Unimplemented defaults - run
bool DebuggerClient::run()
{
   return false;
}
// Unimplemented defaults - leap
bool DebuggerClient::leap()
{
   return false;
}
// Unimplemented defaults - skip
bool DebuggerClient::skip()
{
   return false;
}
// Unimplemented defaults - stepInto
bool DebuggerClient::stepInto()
{
   return false;
}
// Unimplemented defaults - kill
bool DebuggerClient::kill()
{
   return false;
}

// Unimplemented defaults
void DebuggerClient::fileOpened(QString file)
{
   return;
}

// Unimplemented defaults
bool DebuggerClient::addBreakpoint(DebuggerBreakpoint* breakpoint)
{
   return false;
} 

// Unimplemented defaults
bool DebuggerClient::removeBreakpoint(DebuggerBreakpoint* breakpoint)
{
   return false;
}



