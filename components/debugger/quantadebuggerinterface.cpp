/***************************************************************************
                              debugmanager.cpp
                             ------------------
    begin                : 2004-03-12
    copyright            : (C) 2004 Linus McCabe <linus@mccabe.nu>
    
 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *                     
 *                                                                          *
 ***************************************************************************/

#include <ktexteditor/document.h>
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kmessagebox.h>

#include "quanta.h"
#include "quantadoc.h"
#include "document.h"
#include "resource.h"
#include "project.h"
#include "debuggerinterface.h"
#include "quantadebuggerinterface.h"
#include "debuggermanager.h"

QuantaDebuggerInterface::QuantaDebuggerInterface (QObject *myparent, const char* name) 
 : DebuggerInterface(myparent, name)
{
  m_manager = static_cast<DebuggerManager*>(parent());
} 

QuantaDebuggerInterface::~QuantaDebuggerInterface () 
{
}

QString QuantaDebuggerInterface::debugServerHost() 
{
  return Project::ref()->debugServerHost;
}

QString QuantaDebuggerInterface::debugServerPort()
{
  return Project::ref()->debugServerPort;
}

void QuantaDebuggerInterface::haveBreakpoint (QString file, int line) 
{
  return m_manager->haveBreakpoint(file, line);
}
    
// Public help functions
bool QuantaDebuggerInterface::showStatus(QString message, bool log)
{
  return m_manager->showStatus(message, log);
}
 
bool QuantaDebuggerInterface::setActiveLine(QString file, int line)
{
  return m_manager->setActiveLine(file, line);
}

void QuantaDebuggerInterface::enableAction(QString action, bool enable)
{
  m_manager->enableAction(action, enable);
}

void QuantaDebuggerInterface::fileOpened(QString file)
{
  m_manager->fileOpened(file);
}



#include "quantadebuggerinterface.moc"