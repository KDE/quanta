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

const uint CapBreakpoints=1;
const uint CapWatches=2;
const uint CapBacktraces=4;
const uint CapStepInto=16;
const uint CapStepOver=32;
const uint CapSkip=64;
const uint CapPause=128;
const uint CapKill=256;
const uint CapRun=512;
const uint CapRunDisplay=1024;
const uint CapConditionalBreaks=2048;
const uint CapStepOut=4096;
const uint CapClearAllBreakpoints=8192;
const uint CapConnect=8192*2;


class DebuggerInterface;
class DebuggerBreakpoint;

class DebuggerClient : public QObject
{
  private:
  protected:
    DebuggerClient(QObject *parent, const char* name);
    ~DebuggerClient();
    
    bool m_active;
    
  public:
  
    virtual const uint getCapabilities() = 0;
    virtual bool startSession() = 0;
    virtual bool endSession() = 0;
    virtual QString getName() = 0;
    
    virtual bool run();
    virtual bool leap();
    virtual bool skip();
    virtual bool stepOver();
    virtual bool stepInto();
    virtual bool stepOut();
    virtual bool kill();
    virtual bool pause();

    virtual void fileOpened(QString file);
    virtual bool addBreakpoint(DebuggerBreakpoint* breakpoint);
    virtual bool removeBreakpoint(DebuggerBreakpoint* breakpoint);

    bool isActive();    
    DebuggerInterface *debuggerInterface();
};

#endif
