/***************************************************************************
                             conditionalbreakpointdialog.h
                             ------------------
    begin                : 2004-04-05
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

#ifndef CONDITIONALBREAKPOINTDIALOG_H
#define CONDITIONALBREAKPOINTDIALOG_H

#include "conditionalbreakpointdialogs.h"
#include "debuggerbreakpoint.h"

class ConditionalBreakpointDialog : public ConditionalBreakpointDialogS
{
  Q_OBJECT

  public:
    enum Break
    {
      OnTrue = 0,
      OnChange
    };
  
    ConditionalBreakpointDialog(const QString& expression, const QString& inFile, const QString& inClass, const QString& inFunction);
    ~ConditionalBreakpointDialog();

//     QString expression();
//     QString inFile();
//     QString inClass();
//     QString inFunction();
//     DebuggerBreakpoint::Types type();
    
    DebuggerBreakpoint *breakpoint();
        
  public slots:
    void slotExpressionChanged();
    void slotClearFile();
    void slotClearClass();
    void slotClearFunction();
    
  private:
    
};

#endif // CONDITIONALBREAKPOINTDIALOG_H

