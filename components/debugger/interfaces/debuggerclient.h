/***************************************************************************
                          phpdebugsocket.h
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

#ifndef _DEBUGGERCLIENT_H
#define _DEBUGGERCLIENT_H

#include <qserversocket.h>
#include <qobject.h>
#include <kurl.h>
#include <qdom.h>

class DebuggerInterface;
class DebuggerBreakpoint;
class DebuggerVariable;

namespace DebuggerClientCapabilities 
{
  enum Capabilities 
  {
    // Session related
    StartSession = 1000,
    EndSession,
    
    // Breakpoint related
    LineBreakpoints = 2000,
    ConditionalBreakpoints,
    ClearAllBreakpoints,
    
    // Variable related
    Watches = 4000,
    VariableSetValue,
    
    // Execution related
    Run = 5000,
    RunDisplay,
    Pause,
    Kill,
    StepInto,
    StepOver,
    StepOut,
    Skip
  };
}

class DebuggerClient : public QObject
{
  private:
  protected:
    DebuggerClient(QObject *parent, const char* name);

    bool m_active;
    
  public:     
    virtual const uint supports(DebuggerClientCapabilities::Capabilities) = 0;
    virtual void startSession() = 0;
    virtual void endSession() = 0;
    virtual QString getName() = 0;
    
    // Execution control
    virtual void run();
    virtual void leap();
    virtual void skip();
    virtual void stepOver();
    virtual void stepInto();
    virtual void stepOut();
    virtual void kill();
    virtual void pause();

    // Settings
    virtual void readConfig(QDomNode node);
    virtual void showConfig(QDomNode node);
    
    // Misc
    virtual void fileOpened(QString file);
    virtual void addBreakpoint(DebuggerBreakpoint* breakpoint);
    virtual void removeBreakpoint(DebuggerBreakpoint* breakpoint);
    virtual void addWatch(const QString &);
    virtual void removeWatch(DebuggerVariable*);
    virtual void variableSetValue(DebuggerVariable *variable);
    
    bool isActive();    
    DebuggerInterface *debuggerInterface();


};

#endif
