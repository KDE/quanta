/***************************************************************************
                            debuggerinterface.h
                            -------------------
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

#ifndef DEBUGGERINTERFACE_H
#define DEBUGGERINTERFACE_H

#include <qobject.h>
#include <qstring.h>
#include <kurl.h>

class DebuggerInterface : public QObject {
  Q_OBJECT

 private:
  
 
  public:
    DebuggerInterface(QObject *parent, const char* name);

    // Breakpoints
    virtual void haveBreakpoint (QString file, int line) = 0;
        
    // Public help functions
    virtual bool showStatus(QString message, bool log) = 0;
    virtual bool setActiveLine (QString file, int line) = 0;
  
    virtual void enableAction(QString action, bool enable) = 0;
    virtual void fileOpened(QString file) = 0;
  
    virtual QString debugServerHost() = 0;
    virtual QString debugServerPort() = 0;
};

#endif


