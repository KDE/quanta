/*************************************************************************** 
                          xsldbgwalkspeedimpl.h  -  description 
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
 
#ifndef XSLDBGWALKSPEEDIMPL_H 
#define XSLDBGWALKSPEEDIMPL_H 
 
/** 
  *@author Keith Isdale 
  */ 
 
#include "xsldbgwalkspeed.h" 
 
class XsldbgDebugger; 
 
class XsldbgWalkSpeedImpl : public XsldbgWalkSpeed  { 
  Q_OBJECT 
 
public:  
	XsldbgWalkSpeedImpl(XsldbgDebugger *debugger, QWidget *parent=0, const char *name=0); 
	~XsldbgWalkSpeedImpl(); 
	 
public slots:	 
	void accept()	; 
	void reject(); 
 
private: 
	XsldbgDebugger *debugger; 
	 
}; 
 
#endif 
