/*************************************************************************** 
                          xsldbgtemplatelistitem.cpp  -  description 
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
 
#include "xsldbgtemplatelistitem.h" 
 
XsldbgTemplateListItem::XsldbgTemplateListItem(QListView *parent,	 
								QString fileName, int lineNumber, QString templateName, QString modeName) 
		: XsldbgListItem(parent, 2, fileName, lineNumber) 
{ 
	this->templateName = templateName; 
	setText(0, templateName);	 
	this->modeName = modeName;		 
	setText(1, modeName); 
}								 
 
XsldbgTemplateListItem::~XsldbgTemplateListItem() 
{ 
} 
