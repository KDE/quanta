/*************************************************************************** 
                          xsldbgglobalvariablesimpl.cpp  -  description 
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
 
#include <qlistview.h> 
#include <qlineedit.h> 
 
#include "xsldbgglobalvariablesimpl.h" 
#include "xsldbgdebugger.h" 
#include "xsldbggloballistitem.h" 
 
 
XsldbgGlobalVariablesImpl::XsldbgGlobalVariablesImpl(XsldbgDebugger *debugger, 
	QWidget *parent /*=0*/, const char *name /*=0*/) 
		:  XsldbgGlobalVariables(parent, name),  XsldbgDialogBase() 
{	 
	this->debugger = debugger; 
	connect(debugger, SIGNAL(globalVariableItem(QString /*name */, QString /* fileName */, int /*lineNumber */)), 
		this, SLOT(slotProcGlobalVariableItem(QString /*name */, QString /* fileName */, int /*lineNumber */))); 
	connect( varsListView,  SIGNAL(selectionChanged(QListViewItem *)), 
						this, SLOT(selectionChanged(QListViewItem*))); 
	show(); 
} 
 
 
XsldbgGlobalVariablesImpl::~XsldbgGlobalVariablesImpl(){ 
    debugger = 0L; 
} 
 
void XsldbgGlobalVariablesImpl::slotProcGlobalVariableItem(QString name , QString fileName, int lineNumber) 
{ 
	if (name == QString::null) 
		  varsListView->clear(); 
	else{ 
		varsListView->insertItem(new XsldbgGlobalListItem(varsListView, 
																					fileName, lineNumber, name)); 
	} 
 
} 
 
 
void XsldbgGlobalVariablesImpl::selectionChanged(QListViewItem *item) 
{ 
	XsldbgGlobalListItem *globalItem = dynamic_cast<XsldbgGlobalListItem*>(item); 
	if (globalItem != 0 && debugger != 0) { 
		debugger->gotoLine(globalItem->getFileName(), globalItem->getLineNumber());	 
	} 
} 
 
void XsldbgGlobalVariablesImpl::refresh() 
{
    if (debugger != 0) 
	debugger->fakeInput("globals", true) ; 
} 
 
void XsldbgGlobalVariablesImpl::slotEvaluate()
{
  if (debugger != 0L)
    debugger->slotCatCmd( expressionEdit->text() );
} 
 
 
