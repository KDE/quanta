/*************************************************************************** 
                          xsldbgsourcesimpl.cpp  -  description 
                             ------------------- 
    begin                : Fri Jan 4 2002 
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
 
#include "xsldbgsourcesimpl.h" 
#include "xsldbgdebugger.h" 
#include "xsldbggloballistitem.h" 
 
 
XsldbgSourcesImpl::XsldbgSourcesImpl(XsldbgDebugger *debugger, 
	QWidget *parent /*=0*/, const char *name /*=0*/) 
		:  XsldbgSources(parent, name), XsldbgDialogBase() 
{	 
	this->debugger = debugger; 
	connect(debugger, SIGNAL(sourceItem(QString /* fileName */, QString /* parentFileName */, int /*lineNumber */)), 
		this, SLOT(slotProcSourceItem(QString /* fileName */, QString /* parentFileName */, int /*lineNumber */))); 
		 
	connect( sourceListView,  SIGNAL(selectionChanged(QListViewItem *)), 
						this, SLOT(selectionChanged(QListViewItem*))); 
						 
	show(); 
	refresh(); 
} 
 
XsldbgSourcesImpl::~XsldbgSourcesImpl(){ 
    debugger = 0L; 
} 
 
void XsldbgSourcesImpl::slotProcSourceItem(QString  fileName , QString  parentFileName , int lineNumber ) 
{ 
	if (fileName == QString::null){ 
		  sourceListView->clear(); 
	}else{ 
		sourceListView->insertItem(new XsldbgGlobalListItem(sourceListView, 
																					parentFileName, lineNumber, fileName)); 
	} 
} 
 
 
void XsldbgSourcesImpl::selectionChanged(QListViewItem *item) 
{ 
	XsldbgGlobalListItem *sourceItem = dynamic_cast<XsldbgGlobalListItem*>(item); 
	if (sourceItem){ 
		debugger->gotoLine(sourceItem->getVarName(), 1);	 
	} 
} 
 
void XsldbgSourcesImpl::refresh() 
{ 
	debugger->fakeInput("stylesheets", true) ; 
} 
 
 
 
 

#include "xsldbgsourcesimpl.moc"
