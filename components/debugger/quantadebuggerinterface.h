/***************************************************************************
                               debugmanager.h
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

#ifndef MYDEBUGMANAGER_H
#define MYDEBUGMANAGER_H

#include <qobject.h>
#include <qstring.h>
#include "debuggerinterface.h"
#include "debuggervariable.h"

class DebuggerManager;

class QuantaDebuggerInterface : public DebuggerInterface
{
    Q_OBJECT

  private:
    DebuggerManager *m_manager;

  public:
    QuantaDebuggerInterface(QObject *myparent, const char* name);
    ~QuantaDebuggerInterface();

    // Breakpoints
    void haveBreakpoint (QString file, int line) ;
    void refreshBreakpoints();

    // Public help functions
    bool showStatus(QString message, bool log);
    bool setActiveLine (QString file, int line) ;

    void enableAction(QString action, bool enable);
    void fileOpened(QString file) ;

    // Watches handling
    //void preWatchUpdate();
    //void postWatchUpdate();
    //DebuggerVariable* newDebuggerVariable(const QString& name, const QString& value, int type);
    //void addVariable(DebuggerVariable*);
    void parsePHPVariables(const QString &);

};

#endif

