/*************************************************************************** 
                          xsldbglocalvariablesimpl.cpp  -  description 
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
 
#include <qlistview.h> 
#include <qlineedit.h> 
 
#include "xsldbglocalvariablesimpl.h" 
#include "xsldbglocallistitem.h" 
#include "xsldbgdebugger.h" 
 
 
XsldbgLocalVariablesImpl::XsldbgLocalVariablesImpl(XsldbgDebugger *debugger, 
	QWidget *parent /*=0*/, const char *name /*=0*/) 
		:  XsldbgLocalVariables(parent, name), XsldbgDialogBase() 
{	 
	this->debugger = debugger; 
	connect(debugger, SIGNAL(localVariableItem(QString /*name */, QString /* templateContext*/, 
								QString /* file*/, int /*lineNumber */)), 
			this, SLOT(slotProcLocalVariableItem(QString /*name */, QString /* templateContext*/, 
								QString /* file*/, int /*lineNumber */))); 
	connect(varsListView,  SIGNAL(selectionChanged(QListViewItem *)), 
						this, SLOT(selectionChanged(QListViewItem*))); 
	show(); 
} 
 
 
XsldbgLocalVariablesImpl::~XsldbgLocalVariablesImpl() 
{ 
  debugger = 0L; 
} 
 
void XsldbgLocalVariablesImpl::slotProcLocalVariableItem(QString name , 
	QString templateContext, QString  fileName, int lineNumber) 
{ 
 
	if (name == QString::null) 
		  varsListView->clear(); 
	else{ 
		varsListView->insertItem(new XsldbgLocalListItem(varsListView, 
																					fileName, lineNumber, name, templateContext)); 
	} 
 
}	 
 
void XsldbgLocalVariablesImpl::selectionChanged(QListViewItem *item) 
{ 
	XsldbgLocalListItem *localItem = dynamic_cast<XsldbgLocalListItem*>(item); 
	if (localItem){ 
		debugger->gotoLine(localItem->getFileName(), localItem->getLineNumber());	 
	} 
 
} 
 
void XsldbgLocalVariablesImpl::refresh() 
{ 
	debugger->fakeInput("locals", true) ; 
} 
 
void XsldbgLocalVariablesImpl::slotEvaluate()
{
  if (debugger != 0L)
    debugger->slotCatCmd( expressionEdit->text() );
}

 
 
 

#include "xsldbglocalvariablesimpl.moc"
