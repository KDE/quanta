/***************************************************************************
                          wtoolbar.h  -  description
                             -------------------
    begin                : Sun May 28 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef WTOOLBAR_H
#define WTOOLBAR_H

#include <qwidget.h>
#include <ktoolbar.h>

/**new tool bar class
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class WToolBar : public KToolBar {
   Q_OBJECT
public: 
	WToolBar(QWidget *parent, const char *name=0);
	~WToolBar();
	
	void fixingSize();
	void insertSeparator();
		
public:
};

#endif
