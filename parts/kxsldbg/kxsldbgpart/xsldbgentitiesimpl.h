/*************************************************************************** 
                          xsldbgentitiesimpl.h  -  description 
                             ------------------- 
    begin                : Sun Jan 20 2002 
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
 
#ifndef XSLDBGENTITIESIMPL_H 
#define XSLDBGENTITIESIMPL_H 
 
/** 
  *@author Keith Isdale 
  */ 
 
#include "xsldbgentities.h" 
#include "xsldbgdialogbase.h" 
 
class XsldbgDebugger; 
 
class XsldbgEntitiesImpl : public XsldbgEntities, public XsldbgDialogBase  { 
  Q_OBJECT 
 
public: 
	XsldbgEntitiesImpl(XsldbgDebugger *debugger, QWidget *parent=0, const char *name=0); 
	~XsldbgEntitiesImpl(); 
 
public slots: 
	/** Process request to add entity to view, First parameter is QString::null 
			to indicate start of entity list notfication */ 
	void slotProcEntityItem(QString SystemID,  QString PublicID); 
	 
	void selectionChanged(QListViewItem *item);	 
	 
	/** refresh data from source */ 
	void refresh(); 
 
	 
private: 
	XsldbgDebugger *debugger; 
 
}; 
 
#endif 
