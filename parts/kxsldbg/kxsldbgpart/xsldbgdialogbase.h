/*************************************************************************** 
                          xsldbgdialogbase.h  -  description 
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
 
#ifndef XSLDBGDIALOGBASE_H 
#define XSLDBGDIALOGBASE_H 
 
 
/** 
  *@author Keith Isdale 
  */ 
 
#include <qfiledialog.h> 
 
class XsldbgDialogBase { 
 
public:  
	XsldbgDialogBase(); 
	virtual ~XsldbgDialogBase(); 
 
public : 
	/** return true if all data ok */ 
	virtual bool isValid(); 
 
	/** Update changes to xsldbg*/ 
	virtual void update(); 
	 
	/** refresh data from source */ 
	virtual void refresh(); 
 
}; 
 
#endif 
