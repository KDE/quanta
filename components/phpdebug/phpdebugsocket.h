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

#ifndef _PHPDEBUGSOCKET_H
#define _PHPDEBUGSOCKET_H

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


class PHPDebuggerInterface;

class PHPDebugSocket : public QObject
{
  private:
  protected:
    PHPDebugSocket(PHPDebuggerInterface *);
    PHPDebuggerInterface *m_interface;

  public:
  
    virtual const uint getCapabilities() = 0;
    virtual bool run() = 0;
    virtual bool leap() = 0;
    virtual bool skip() = 0;
    virtual bool stepOver() = 0;
    virtual bool stepInto() = 0;
    virtual bool stepOut() = 0;
    virtual bool kill() = 0;
    virtual bool pause() = 0;
    virtual bool tryConnect() = 0;
    virtual bool disConnect() = 0;
    virtual QString getName() = 0;

    virtual void fileOpened(KURL file) = 0;
    virtual bool setBreakpoint ( QString file, int line) = 0;
    virtual bool removeBreakpoint ( QString file, int line ) = 0;
};

#endif
