/*************************************************************************** 
                          xsldbgbreakpointsimpl.h  -  description 
                             ------------------- 
    begin                : Fri Jan 4 2002 
    copyright            : (C) 2002 by Keith Isdale 
    email                : k_isdale@tpg.com.au 
 ***************************************************************************/ 
 
/*********************************************************************************** 
 *                                                                         										* 
 *   This program is free software; you can redistribute it and/or modify       * 
 *   it under the terms of the GNU General Public License as published by  * 
 *   the Free Software Foundation; either version 2 of the License, or         * 
 *   (at your option) any later version.                                   							* 
 *                                                                         										* 
 ************************************************************************************/ 
 
#ifndef XSLDBGBREAKPOINTSIMPL_H 
#define XSLDBGBREAKPOINTSIMPL_H 
 
/** 
  *@author Keith Isdale 
  */ 
 
#include "xsldbgbreakpoints.h" 
#include "xsldbgdialogbase.h" 
 
class XsldbgDebugger; 
 
 
class XsldbgBreakpointsImpl : public XsldbgBreakpoints, public XsldbgDialogBase  { 
  Q_OBJECT 
 
public: 
	XsldbgBreakpointsImpl(XsldbgDebugger *debugger, QWidget *parent=0, const char *name=0); 
	~XsldbgBreakpointsImpl(); 
 
	/* Return >= 0 if line number entered is valid */ 
	int getLineNumber(); 
 
	/* Return >= 0 if breakpoint id entered is valid */	 
	int getId(); 
 
 
public slots: 
	 
	void slotAddBreakpoint(); 
	void slotDeleteBreakpoint(); 
	void slotDeleteAllBreakpoints(); 
	void slotEnableBreakpoint(); 
	void selectionChanged(QListViewItem *item); 
	 
	/** Recieve notification about breakpoint to add to view, First parameter is QString::null 
			to indicate start of breakpoint list notfication */ 
	void slotProcBreakpointItem(QString file, int lineNumber,  
				    QString templateName, QString modeName, 
				    bool enabled, int  id ); 
 
	/** refresh data from source */ 
	void refresh(); 
	 
	/**Clear data from all fields */ 
	void slotClear(); 
	 
private: 
	XsldbgDebugger *debugger; 
 
}; 
 
#endif 
