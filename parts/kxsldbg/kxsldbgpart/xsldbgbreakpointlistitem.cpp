/***************************************************************************
                          xsldbgbreakpointlistitem.cpp  -  description
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

#include <klocale.h>
#include "xsldbgbreakpointlistitem.h"

XsldbgBreakpointListItem::XsldbgBreakpointListItem(QListView *parent,
			QString fileName, int lineNumber,
			QString templateName, QString modeName,
			bool enabled, int id)
	: XsldbgListItem(parent, 3, fileName, lineNumber)
{
	this->id = id;
	setText(0, QString::number(id));

	this->templateName = templateName;
	setText(1, templateName);

	this->modeName = modeName;
	setText(2, modeName);

	__enabled = enabled;
	if (enabled == true)
	  setText(5, i18n("Enabled"));
	else
	  setText(5, i18n("Disabled"));
}

XsldbgBreakpointListItem::~XsldbgBreakpointListItem()
{
}

