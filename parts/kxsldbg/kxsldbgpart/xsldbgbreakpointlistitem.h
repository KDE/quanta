/*************************************************************************** 
                          xsldbgbreakpointlistitem.h  -  description 
                             ------------------- 
    begin                : Sun Jan 6 2002 
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
 
#ifndef XSLDBGBREAKPOINTLISTITEM_H 
#define XSLDBGBREAKPOINTLISTITEM_H 
 
/** 
  *@author Keith Isdale 
  */ 
 
#include "xsldbglistitem.h" 
 
class XsldbgBreakpointListItem : public XsldbgListItem  { 
public:  
	XsldbgBreakpointListItem(QListView *parent,	 
			QString fileName, int lineNumber,  
				 QString templateName, QString modeName, 
			bool enabled, int id); 
	~XsldbgBreakpointListItem(); 
 
	/** returns true if this breakpoint is enabled */ 
	bool getEnabled() {return __enabled; }; 
	 
	/** Returns the name of template for this breakpoiint */ 
	QString getTemplateName() { return templateName;}; 
	QString getModeName() {return modeName;}; 
 
	/** Return the breakpoint's id */ 
	int getId() { return id;}; 
 
private: 
	QString templateName, modeName; 
	bool __enabled; /*make sure the name does not clash with QT */ 
	int id; 
}; 
 
#endif 
