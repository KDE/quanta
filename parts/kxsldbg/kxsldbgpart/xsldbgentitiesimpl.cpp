/*************************************************************************** 
                          xsldbgentitiesimplimpl.cpp  -  description 
                             ------------------- 
    begin                : Sun Jan20 2002 
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
 
#include "xsldbgentitiesimpl.h" 
#include "xsldbgdebugger.h" 
#include "xsldbggloballistitem.h" 
 
 
XsldbgEntitiesImpl::XsldbgEntitiesImpl(XsldbgDebugger *debugger, 
	QWidget *parent /*=0*/, const char *name /*=0*/) 
		:  XsldbgEntities(parent, name),  XsldbgDialogBase() 
{	 
	this->debugger = debugger; 
	connect(debugger,  
		SIGNAL(entityItem(QString /*SystemID*/,   
				  QString /*PublicID*/)), 
		this,  
		SLOT(slotProcEntityItem(QString /*SystemID*/,   
					QString /*PublicID*/)));	 
		 
	connect( entitiesListView,   
		 SIGNAL(selectionChanged(QListViewItem *)), 
		 this, SLOT(selectionChanged(QListViewItem*))); 
	show(); 
} 
 
 
XsldbgEntitiesImpl::~XsldbgEntitiesImpl(){ 
} 
 
 
void XsldbgEntitiesImpl::slotProcEntityItem(QString SystemID,  QString PublicID) 
{ 
	if (SystemID == QString::null) 
		  entitiesListView->clear(); 
	else{		 
		entitiesListView->insertItem( 
		        new XsldbgGlobalListItem(entitiesListView, 
			       SystemID, -1, PublicID)); 
	} 
 
} 
 
 
void XsldbgEntitiesImpl::selectionChanged(QListViewItem *item) 
{ 
	XsldbgGlobalListItem *globalItem = dynamic_cast<XsldbgGlobalListItem*>(item); 
	if (globalItem){ 
		debugger->gotoLine(globalItem->getFileName(), 1); 
	} 
} 
 
void XsldbgEntitiesImpl::refresh() 
{ 
	debugger->fakeInput("entities", true) ; 
} 
 
 
 
 
