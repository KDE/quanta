/*************************************************************************** 
                          xsldbgbreakpointsimpl.cpp  -  description 
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
#include <qlistview.h> 
#include <qlineedit.h> 
#include <qmessagebox.h> 
#include "xsldbgdebugger.h" 
#include "xsldbgbreakpointsimpl.h" 
#include "xsldbgbreakpointlistitem.h" 
 
 
 
XsldbgBreakpointsImpl::XsldbgBreakpointsImpl(XsldbgDebugger *debugger, 
	QWidget *parent /*=0*/, const char *name /*=0*/) 
		:  XsldbgBreakpoints(parent, name), XsldbgDialogBase() 
{	 
	this->debugger = debugger; 
	connect(debugger, SIGNAL(breakpointItem(QString /* file*/,  
                         int /*line number */, QString /*templateName*/,  
                         QString /* modeName*/, 
	                 bool /* enabled */, int /* id */)), 
		this, SLOT(slotProcBreakpointItem(QString /* file*/,  
			 int /*line number */, QString /*templateName*/,  
			 QString /* modeName */,  
			 bool /* enabled */, int /* id */))); 
	connect( breakpointListView,  SIGNAL(selectionChanged(QListViewItem *)), 
						this, SLOT(selectionChanged(QListViewItem*))); 
	show(); 
	refresh(); 
} 
 
XsldbgBreakpointsImpl::~XsldbgBreakpointsImpl() 
{ 
    debugger = 0L; 
} 
 
int XsldbgBreakpointsImpl::getLineNumber() 
{ 
	bool isOk = false; 
	int lineNo = lineNumberEdit->text().toInt(&isOk); 
	if (isOk == false){ 
		lineNo = -1; 
		qDebug("Invalid line number"); 
	} 
 
	return lineNo; 
} 
 
int XsldbgBreakpointsImpl::getId() 
{ 
	bool isOk = false; 
	int id = idEdit->text().toInt(&isOk); 
	if (isOk == false){ 
		id = -1; 
		qDebug("Invalid line number"); 
	} 
 
	return id; 
} 
 
void XsldbgBreakpointsImpl::slotAddBreakpoint() 
{ 
	int lineNo = getLineNumber(); 
	if (lineNo != -1) { 
	  if (!sourceFileEdit->text().isEmpty()){ 
	        debugger->slotBreakCmd(sourceFileEdit->text(), lineNo);	 
	  }else { 
	      QMessageBox::information(this, "Operation failed",  
		  "A line number was provided without a file name",  
  	           QMessageBox::Ok); 
	  } 
	}else if (!templateNameEdit->text().isEmpty() ||  
		  !modeNameEdit->text().isEmpty()){ 
		debugger->slotBreakCmd(templateNameEdit->text(), 
				       modeNameEdit->text());	 	 
	}else{ 
	    QMessageBox::information(this, "Operation failed",  
		 "No details provided or an invalid line number was supplied",  
  	          QMessageBox::Ok); 
	} 
} 
 
void XsldbgBreakpointsImpl::slotDeleteBreakpoint() 
{ 
	int lineNo = getLineNumber(), id = getId(); 
	if (id != -1){ 
		debugger->slotDeleteCmd(id);	 
	}else if (lineNo != -1) { 
	  if (!sourceFileEdit->text().isEmpty()){ 
		debugger->slotDeleteCmd(sourceFileEdit->text(), lineNo); 
	  }else { 
	    QMessageBox::information(this, "Operation failed",  
	         "A line number was provided without a file name",  
  	         QMessageBox::Ok); 
	  } 
	}else { 
	    QMessageBox::information(this, "Operation failed",  
		 "No details provided or an invalid line or ID was supplied",  
  	          QMessageBox::Ok); 
	} 
} 
 
void  XsldbgBreakpointsImpl::slotDeleteAllBreakpoints() 
{ 
  if (debugger != 0L){ 
    debugger->fakeInput("delete *", true); 
    debugger->fakeInput("show", true);     
  } 
} 
 
void XsldbgBreakpointsImpl::slotEnableBreakpoint() 
{ 
	int lineNo = getLineNumber(), id = getId(); 
	if (id != -1){ 
		debugger->slotEnableCmd(id);	 
	}else if (lineNo != -1){ 
	  if (!sourceFileEdit->text().isEmpty()){ 
		debugger->slotEnableCmd(sourceFileEdit->text(), lineNo); 
	  }else { 
	       QMessageBox::information(this, "Operation failed",  
	           "A line number was provided without a file name",  
  	           QMessageBox::Ok); 
	  } 
	}else { 
	    QMessageBox::information(this, "Operation failed",  
		 "No details provided",  
  	          QMessageBox::Ok); 
	} 
} 
 
void XsldbgBreakpointsImpl::selectionChanged(QListViewItem *item) 
{ 
	XsldbgBreakpointListItem *breakItem =  
	       dynamic_cast<XsldbgBreakpointListItem*>(item); 
	if (breakItem){ 
	  idEdit->setText(QString::number(breakItem->getId())); 
	  templateNameEdit->setText(breakItem->getTemplateName()); 
	  modeNameEdit->setText(breakItem->getModeName()); 
	  sourceFileEdit->setText(breakItem->getFileName()); 
	  lineNumberEdit->setText(QString::number(breakItem->getLineNumber()));		} 
} 
 
 
void XsldbgBreakpointsImpl::refresh() 
{ 
	/* get xsldbg to tell what breakpoints are set,  
	   we'll get the notification back via slotProcBreakpointItem */ 
	debugger->fakeInput("showbreak", true); 
} 
 
 
void XsldbgBreakpointsImpl::slotClear() 
{ 
	idEdit->setText(""); 
	templateNameEdit->setText(""); 
	modeNameEdit->setText(""); 
 	sourceFileEdit->setText(""); 
 	lineNumberEdit->setText(""); 
} 
 
void XsldbgBreakpointsImpl::slotProcBreakpointItem(QString  fileName,  
						   int lineNumber , 
						   QString templateName, 
						   QString modeName, 
						   bool enabled, int id ) 
{ 
	if (fileName == QString::null) 
		  breakpointListView->clear(); 
	else{ 
	  breakpointListView->insertItem( 
	       new XsldbgBreakpointListItem(breakpointListView, 
	       fileName, lineNumber,templateName, modeName, enabled, id)); 
	} 
} 
 
 
 

#include "xsldbgbreakpointsimpl.moc"
