/*************************************************************************** 
                          xsldbggloballistitem.h  -  description 
                             ------------------- 
    begin                : Sun Jan 13 2002 
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
 
#ifndef XSLDBGGLOBALLISTITEM_H 
#define XSLDBGGLOBALLISTITEM_H 
 
/** 
  *@author Keith Isdale 
  */ 
 
#include "xsldbglistitem.h" 
 
 
class XsldbgGlobalListItem : public XsldbgListItem  { 
public:  
	XsldbgGlobalListItem(QListView *parent,	 
								QString fileName, int lineNumber, QString globalName); 
	~XsldbgGlobalListItem(); 
	 
	/** return the name of this variable */ 
	QString getVarName(); 
	 
private: 
	QString varName; 	 
}; 
 
#endif 
