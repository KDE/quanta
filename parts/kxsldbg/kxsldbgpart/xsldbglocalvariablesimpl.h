/*************************************************************************** 
                          xsldbglocalvariablesimpl.h  -  description 
                             ------------------- 
    begin                : Sat Jan 5 2002 
    copyright            : (C) 2002 by Keith Isdale 
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
 
#ifndef XSLDBGLOCALVARIABLESIMPL_H 
#define XSLDBGLOCALVARIABLESIMPL_H 
 
/** 
  *@author Keith Isdale 
  */ 
 
#include "xsldbglocalvariables.h" 
#include "xsldbgdialogbase.h" 
 
class XsldbgDebugger; 
 
class XsldbgLocalVariablesImpl : public XsldbgLocalVariables, public XsldbgDialogBase  { 
  Q_OBJECT 
 
public:  
	XsldbgLocalVariablesImpl(XsldbgDebugger *debugger, QWidget *parent=0, const char *name=0); 
	~XsldbgLocalVariablesImpl(); 
	 
public slots: 
	/** Process request to add local varaible to view, First parameter is QString::null 
			to indicate start of local variable list notfication */ 
	void slotProcLocalVariableItem(QString name , QString templateContext, QString  file, int lineNumber); 
	 
	void selectionChanged(QListViewItem *item);	 
	 
	/** refresh data from source */ 
	void refresh(); 

	void slotEvaluate();
 
 
private: 
	XsldbgDebugger *debugger; 
 
}; 
 
#endif 
