/***************************************************************************
                          phpdebuggerinterface.h
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

#ifndef PHPDEBUGGERINTERFACE_H
#define PHPDEBUGGERINTERFACE_H

#include <qobject.h>
#include <qstring.h>

class DebuggerClient;
class QuantaDebuggerInterface; 
class DebuggerBreakpointList; 

class DebuggerManager : public QObject {
  Q_OBJECT 

  private:
    // client
    DebuggerClient *m_client;
    QuantaDebuggerInterface * m_interface;
    DebuggerBreakpointList *m_breakpointList;
    
    // Internal help functions
    void setMark(QString, long, bool, int);
    void initActions();
    void initClientActions();
    QString mapServerPathToLocal(QString serverpath);
    QString mapLocalPathToServer(QString localpath);
    QString m_currentFile;
    long m_currentLine;
    
  public:
    DebuggerManager(QObject *myparent);
    ~DebuggerManager();
    
    // Breakpoints
    void haveBreakpoint (QString file, int line);
        
    // Public help functions
    bool showStatus(QString message, bool log);
    bool setActiveLine (QString file, int line);
  
    void enableAction(QString action, bool enable);
    void fileOpened(QString file);
  
  public slots:
    /** Execution control slots **/
    void slotDebugRun();
    void slotDebugLeap();
    void slotDebugSkip();
    void slotDebugStepOver();
    void slotDebugStepInto();
    void slotDebugStepOut();
    void slotDebugPause();
    void slotDebugKill();
    
    //Breakpoint
    void toggleBreakpoint();
    void clearBreakpoints();
    
    // Connection related slots
    void slotDebugStartSession();
    void slotDebugEndSession();  
};

#endif
