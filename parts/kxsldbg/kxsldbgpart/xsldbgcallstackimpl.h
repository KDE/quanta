/*************************************************************************** 
                          xsldbgcallstackimpl.h  -  description 
                             ------------------- 
    begin                : Wed Jan 16 2002 
    copyright           : (C) 2002 by Keith lsdale 
    email                : k_isdale@tpg.com.au 
 ***************************************************************************/ 
 
/*************************************************************************** 
 *                                                                         * 
 *   This program is free software; you can redistribute it and/or modify  * 
 *   it under the terms of the GNU General Public License as published by  * 
 *   the Free Software Foundation; either version 2 of the License, or     * 
 *   (at your option) any later version.                                   * 
 *                                                                         * 
 ***************************************************************************/ 
 
#ifndef XSLDBGCALLSTACKIMPL_H 
#define XSLDBGCALLSTACKIMPL_H 
 
/** 
  *@author Keith lsdale 
  */ 
 
#include "xsldbgcallstack.h" 
#include "xsldbgdialogbase.h" 
 
class XsldbgDebugger; 
 
class XsldbgCallStackImpl : public XsldbgCallStack,  public XsldbgDialogBase   { 
  Q_OBJECT	 
 
public:  
	XsldbgCallStackImpl(XsldbgDebugger *debugger, QWidget *parent=0, const char *name=0); 
	~XsldbgCallStackImpl(); 
	 
public slots:	 
	void selectionChanged(QListViewItem *item);	 
	 
	/** Process request to add callstack to view, First parameter is QString::null 
			to indicate start of callstack list notfication */	 
	void slotProcCallStackItem(QString  templateName, QString  fileName , int  lineNumber );	 
	 
	/** refresh data from source */ 
	void refresh(); 
	 
 
private: 
	XsldbgDebugger *debugger; 
	 
}; 
 
#endif 
