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

void DebuggerClient::unSupportedAction(const QString &action)
{
  KMessageBox::error(NULL, i18n("The current debugger, %1, does not support the \"%2\" instruction.").arg(this->getName()).arg(action), i18n("Unsupported Debugger Function"));

}

// Unimplemented defaults - Pause execution
void DebuggerClient::pause()
{
 unSupportedAction(i18n("Pause"));
}

// Unimplemented defaults - Send Request
void DebuggerClient::request()
{
 unSupportedAction(i18n("Send HTTP Request"));
}

// Unimplemented defaults - step over
void DebuggerClient::stepOver()
{
  unSupportedAction(i18n("Step Over"));

}

// Unimplemented defaults - step out
void DebuggerClient::stepOut()
{
  unSupportedAction(i18n("Step Out"));

}

// Unimplemented defaults - trace
void DebuggerClient::trace()
{
  unSupportedAction(i18n("Trace"));
}
// Unimplemented defaults - Run
void DebuggerClient::run()
{
  unSupportedAction(i18n("Run"));
}
// Unimplemented defaults - skip
void DebuggerClient::skip()
{
  unSupportedAction(i18n("Skip"));
}
// Unimplemented defaults - stepInto
void DebuggerClient::stepInto()
{
  unSupportedAction(i18n("Step Into"));
}
// Unimplemented defaults - kill
void DebuggerClient::kill()
{
  unSupportedAction(i18n("Kill"));

}

// Unimplemented defaults
void DebuggerClient::profilerOpen( )
{
  unSupportedAction(i18n("Open Profiler Output"));
}

// Unimplemented defaults
void DebuggerClient::fileOpened(const QString&)
{
   return;
}

// Unimplemented defaults
void DebuggerClient::addBreakpoint(DebuggerBreakpoint*)
{
  unSupportedAction(i18n("Set Breakpoint"));

}

// Unimplemented defaults
void DebuggerClient::removeBreakpoint(DebuggerBreakpoint*)
{
  unSupportedAction(i18n("Remove Breakpoint"));
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

//  Unimplemented defaults: add watch
void DebuggerClient::addWatch(const QString &)
{
  KMessageBox::error(NULL, i18n("%1 does not support watches.").arg(this->getName()), i18n("Unsupported Debugger Function"));
}

//  Unimplemented defaults: Remove watch
void DebuggerClient::removeWatch(DebuggerVariable *)
{
  // Giving an error seems pointless, since you shouldnt be able to add a watch in the first place...
  KMessageBox::error(NULL, i18n("%1 does not support watches.").arg(this->getName()), i18n("Unsupported Debugger Function"));
}

// Unimplemented defaults: set value of varialbe
void DebuggerClient::variableSetValue(const DebuggerVariable &)
{
  KMessageBox::error(NULL, i18n("%1 does not support setting the value of variables.").arg(this->getName()), i18n("Unsupported Debugger Function"));
}

#include "debuggerclient.moc"
