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

#ifndef PHPDebuggerInterface_H
#define PHPDebuggerInterface_H

#include "phpdebugsocket.h"
#include "messageoutput.h"
#include <qobject.h>
#include <qstring.h>

class PHPDebuggerInterface : public QObject {
  Q_OBJECT

  private:
    PHPDebugSocket *m_debugsocket;

    // Keep track of current position
    QString m_currentFile;
    long m_currentLine;

    // Internal help functions
    void setMark(const QString&, long, bool, int);
    QString mapServerPathToLocal(QString serverpath);
    QString mapLocalPathToServer(QString localpath);
    
  public:
    PHPDebuggerInterface();

    // Breakpoints
    bool setBreakpoint (const KURL& file, int line);
    void haveBreakpoint (const QString& file, int line);
    bool removeBreakpoint ( const KURL& file, int line );
    
    // Public help functions
    bool showStatus(QString message, bool log);
    bool setActiveLine (const KURL& file, int line);
    void fileOpened(const KURL& file);
  
    void enableAction(const QString& action, bool enable);
  
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
    
    // Connection related slots
    void slotDebugConnect();
    void slotDebugDisconnect();    
};

#endif

