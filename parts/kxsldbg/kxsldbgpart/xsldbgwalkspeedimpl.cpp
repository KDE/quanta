/*************************************************************************** 
                          xsldbgwalkspeedimpl.cpp  -  description 
                             ------------------- 
    begin                : Fri Jan 25 2002 
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
 
#include <qslider.h> 
 
#include "xsldbgdebugger.h" 
#include "xsldbgwalkspeedimpl.h" 
 
XsldbgWalkSpeedImpl::XsldbgWalkSpeedImpl(XsldbgDebugger *debugger, 
																							QWidget *parent /*=0 */, const char *name /*=0*/ ) 
	: XsldbgWalkSpeed(parent, name, FALSE)	 
{ 
	this->debugger = debugger; 
} 
 
XsldbgWalkSpeedImpl::~XsldbgWalkSpeedImpl() 
{ 
} 
 
void XsldbgWalkSpeedImpl::accept() 
{ 
	/* For xsldbg 1 means fast speed  9 means slow so invert the slider value to suit 
		ie : a slider value of 1 results in walk speed of 9 */ 
	debugger->slotWalkSpeed(10 - walkSpeedSlider->value()); 
	hide(); 
} 
 
void XsldbgWalkSpeedImpl::reject() 
{ 
	hide(); 
} 
 
 
