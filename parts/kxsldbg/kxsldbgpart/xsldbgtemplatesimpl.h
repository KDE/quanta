/*************************************************************************** 
                          xsldbgtemplatesimpl.h  -  description 
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
 
#ifndef XSLDBGTEMPLATESIMPL_H 
#define XSLDBGTEMPLATESIMPL_H 
 
 
/** 
  *@author Keith Isdale 
  */ 
 
 
#include "xsldbgtemplates.h" 
#include "xsldbgdialogbase.h" 
 
class XsldbgDebugger; 
 
class XsldbgTemplatesImpl : public XsldbgTemplates, public XsldbgDialogBase  { 
  Q_OBJECT 
 
public:  
	XsldbgTemplatesImpl(XsldbgDebugger *debugger, QWidget *parent=0, const char *name=0); 
	~XsldbgTemplatesImpl(); 
	 
public slots:	 
	void selectionChanged(QListViewItem *item);	 
	 
	/** Process request to add template to view, First parameter is QString::null 
			to indicate start of template list notfication */	 
	void slotProcTemplateItem(QString  name, QString mode, QString  fileName , int  lineNumber );	 
	 
	/** refresh data from source */ 
	void refresh(); 
	 
 
private: 
	XsldbgDebugger *debugger; 
 
}; 
 
#endif 
