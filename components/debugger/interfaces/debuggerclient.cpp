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
#include <kmessagebox.h>


// CTor
DebuggerClient::DebuggerClient(QObject *parent, const char* name) 
 : QObject(parent, name)
{
  m_active = false;
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
void DebuggerClient::pause()
{
  KMessageBox::error(NULL, i18n("The current debugger, %1, does not support the \"%2\" instruction.").arg(this->getName()).arg(i18n("Pause")), i18n("Unsupported debugger function"));

}

// Unimplemented defaults - step over
void DebuggerClient::stepOver()
{
  KMessageBox::error(NULL, i18n("The current debugger, %1, does not support the \"%2\" instruction.").arg(this->getName()).arg(i18n("Step Over")), i18n("Unsupported debugger function"));

}

// Unimplemented defaults - step out
void DebuggerClient::stepOut()
{
  KMessageBox::error(NULL, i18n("The current debugger, %1, does not support the \"%2\" instruction.").arg(this->getName()).arg(i18n("Step Out")), i18n("Unsupported debugger function"));

}

// Unimplemented defaults - run
void DebuggerClient::run()
{
  KMessageBox::error(NULL, i18n("The current debugger, %1, does not support the \"%2\" instruction.").arg(this->getName()).arg(i18n("Run")), i18n("Unsupported debugger function"));
}
// Unimplemented defaults - leap
void DebuggerClient::leap()
{
  KMessageBox::error(NULL, i18n("The current debugger, %1, does not support the \"%2\" instruction.").arg(this->getName()).arg(i18n("Leap")), i18n("Unsupported debugger function"));
}
// Unimplemented defaults - skip
void DebuggerClient::skip()
{
  KMessageBox::error(NULL, i18n("The current debugger, %1, does not support the \"%2\" instruction.").arg(this->getName()).arg(i18n("Skip")), i18n("Unsupported debugger function"));
}
// Unimplemented defaults - stepInto
void DebuggerClient::stepInto()
{
  KMessageBox::error(NULL, i18n("The current debugger, %1, does not support the \"%2\" instruction.").arg(this->getName()).arg(i18n("Step Into")), i18n("Unsupported debugger function"));
}
// Unimplemented defaults - kill
void DebuggerClient::kill()
{
  KMessageBox::error(NULL, i18n("The current debugger, %1, does not support the \"%2\" instruction.").arg(this->getName()).arg(i18n("Kill")), i18n("Unsupported debugger function"));

}

// Unimplemented defaults
void DebuggerClient::fileOpened(QString)
{
   return;
}

// Unimplemented defaults
void DebuggerClient::addBreakpoint(DebuggerBreakpoint*)
{
  KMessageBox::error(NULL, i18n("The current debugger, %1, does not support the \"%2\" instruction.").arg(this->getName()).arg(i18n("Set Breakpoint")), i18n("Unsupported debugger function"));

} 

// Unimplemented defaults
void DebuggerClient::removeBreakpoint(DebuggerBreakpoint*)
{
  KMessageBox::error(NULL, i18n("The current debugger, %1, does not support the \"%2\" instruction.").arg(this->getName()).arg(i18n("Remove Breakpoint")), i18n("Unsupported debugger function"));
}


// Unimplemented defaults
void DebuggerClient::showConfig(QDomNode)
{
   KMessageBox::error(NULL, i18n("%1 does not have any specific settings.").arg(this->getName()), i18n("Settings"));
}

// Unimplemented defaults
void DebuggerClient::readConfig(QDomNode)
{
   
}


