/*************************************************************************** 
                          xsldbgtemplatelistitem.h  -  description 
                             ------------------- 
    begin                : Mon Jan 21 2002 
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
 
#ifndef XSLDBGTEMPLATELISTITEM_H 
#define XSLDBGTEMPLATELISTITEM_H 
 
/** 
  *@author Keith Isdale 
  */ 
#include "xsldbglistitem.h" 
 
class XsldbgTemplateListItem : public XsldbgListItem  { 
public:  
	XsldbgTemplateListItem(QListView *parent,	 
								QString fileName, int lineNumber, QString templateName, QString modeName); 
	~XsldbgTemplateListItem(); 
	 
	/** return the name of this template */ 
	QString getTemplateName(); 
 
	/** return the mode of this template */	 
	QString getModeName();	 
	 
private: 
	QString templateName; 
	QString modeName;	 
	 
}; 
 
#endif 
