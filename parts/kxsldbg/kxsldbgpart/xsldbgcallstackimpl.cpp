/*************************************************************************** 
                          xsldbgcallstackimpl.cpp  -  description 
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
#include <qlistview.h> 
#include <qlineedit.h> 
 
#include "xsldbgdebugger.h" 
#include "xsldbgcallstackimpl.h" 
#include "xsldbggloballistitem.h" /* reuse global list item variable as a template list item */ 
 
XsldbgCallStackImpl::XsldbgCallStackImpl(XsldbgDebugger *debugger, 
	QWidget *parent /*=0*/, const char *name /*=0*/) 
		:  XsldbgCallStack(parent, name), XsldbgDialogBase() 
{	 
	this->debugger = debugger; 
	 
	connect(debugger, SIGNAL(callStackItem(QString /* templateName*/, QString /* fileName */, int /* lineNumber */)), 
		this, SLOT(slotProcCallStackItem(QString /* templateName*/, QString /* fileName */, int /* lineNumber */))); 
	connect( callStackListView,  SIGNAL(selectionChanged(QListViewItem *)), 
						this, SLOT(selectionChanged(QListViewItem*))); 
						 
	show(); 
	refresh(); 
} 
 
XsldbgCallStackImpl::~XsldbgCallStackImpl(){ 
    debugger = 0L; 
} 
 
void XsldbgCallStackImpl::slotProcCallStackItem(QString  templateName, QString  fileName , int  lineNumber ) 
{ 
	static int frameNo =0; 
	if (templateName == QString::null){ 
		  callStackListView->clear(); 
		  frameNo = 0;	  	 
	}else{ 
		/* each stack entry will have a FRAME# prefix */ 
		callStackListView->insertItem(new XsldbgGlobalListItem(callStackListView, 
																					fileName, lineNumber, templateName.prepend(QString::number(frameNo++)+ "# "))); 
	} 
} 
 
 
void XsldbgCallStackImpl::selectionChanged(QListViewItem *item) 
{ 
	XsldbgGlobalListItem *callStackItem = dynamic_cast<XsldbgGlobalListItem*>(item); 
	if (callStackItem){ 
		debugger->gotoLine(callStackItem->getFileName(), callStackItem->getLineNumber());	 
	} 
} 
 
void XsldbgCallStackImpl::refresh() 
{ 
	debugger->fakeInput("where", true) ; 
} 
 

#include "xsldbgcallstackimpl.moc"
