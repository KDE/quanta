/*************************************************************************** 
                          xsldbglistitem.h  -  description 
																								 
                             ------------------- 
    begin                : Sun Jan 6 2002 
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
 
#ifndef XSLDBGLISTITEM_H 
#define XSLDBGLISTITEM_H 
 
/** 
  *@author Keith Isdale 
  */ 
 
 
#include <qlistview.h> 
 
class XsldbgListItem : public QListViewItem  { 
public: 
	/** column offset is the index of the fileName column */ 
	XsldbgListItem(QListView *parent, int columnOffset, 
					QString fileName, int lineNumber); 
	~XsldbgListItem(); 
 
 
	QString getFileName(); 
	int getLineNumber(); 
 
private: 
	QString fileName; 
	int lineNumber;		 
}; 
 
#endif 
