/*************************************************************************** 
                          xsldbgtemplatesimpl.cpp  -  description 
                             ------------------- 
    begin                : Fri Jan 4 2002 
    copyright            : (C) 2002 by keith Isdale 
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
#include "xsldbgtemplatesimpl.h" 
#include "xsldbgtemplatelistitem.h" 
 
XsldbgTemplatesImpl::XsldbgTemplatesImpl(XsldbgDebugger *debugger, 
	QWidget *parent /*=0*/, const char *name /*=0*/) 
		:  XsldbgTemplates(parent, name), XsldbgDialogBase() 
{	 
	this->debugger = debugger; 
	 
	connect(debugger, SIGNAL(templateItem(QString /* name*/, QString /*mode*/, QString /* fileName */, int /* lineNumber */)), 
		this, SLOT(slotProcTemplateItem(QString /* name*/,  QString /*mode*/, QString /* fileName */, int /* lineNumber */))); 
	connect( templatesListView,  SIGNAL(selectionChanged(QListViewItem *)), 
						this, SLOT(selectionChanged(QListViewItem*))); 
						 
	show(); 
	refresh(); 
} 
 
XsldbgTemplatesImpl::~XsldbgTemplatesImpl(){ 
    debugger = 0L; 
} 
 
 
void XsldbgTemplatesImpl::slotProcTemplateItem(QString  name, QString mode, QString  fileName , int  lineNumber ) 
{ 
	if (name == QString::null) 
		  templatesListView->clear(); 
	else 
		templatesListView->insertItem(new XsldbgTemplateListItem(templatesListView, 
																					fileName, lineNumber, name, mode)); 
} 
 
 
void XsldbgTemplatesImpl::selectionChanged(QListViewItem *item) 
{ 
	XsldbgTemplateListItem *templateItem = dynamic_cast<XsldbgTemplateListItem*>(item); 
	if (templateItem){ 
		debugger->gotoLine(templateItem->getFileName(), templateItem->getLineNumber());			 
	} 
} 
 
void XsldbgTemplatesImpl::refresh() 
{ 
	debugger->fakeInput("templates", true) ; 
} 
 
 
