/*************************************************************************** 
                          xsldbgglobalvariablesimpl.h  -  description 
                             ------------------- 
    begin                : Sat Jan 5 2002 
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
 
#ifndef XSLDBGGLOBALVARIABLESIMPL_H 
#define XSLDBGGLOBALVARIABLESIMPL_H 
 
/** 
  *@author Keith Isdale 
  */ 
 
#include "xsldbgglobalvariables.h" 
#include "xsldbgdialogbase.h" 
 
class XsldbgDebugger; 
 
class XsldbgGlobalVariablesImpl : public XsldbgGlobalVariables, public XsldbgDialogBase  { 
  Q_OBJECT 
 
public:  
	XsldbgGlobalVariablesImpl(XsldbgDebugger *debugger, QWidget *parent=0, const char *name=0); 
	~XsldbgGlobalVariablesImpl(); 
 
public slots: 
	/** Process request to add global variable to view, First parameter is QString::null 
			to indicate start of global variable list notfication */ 
	void slotProcGlobalVariableItem(QString name , QString fileName, int lineNumber); 
	 
	void selectionChanged(QListViewItem *item);	 
	 
	/** refresh data from source */ 
	void refresh(); 

	void slotEvaluate();
 
	 
private: 
	XsldbgDebugger *debugger; 
 
}; 
 
#endif 
