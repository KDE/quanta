/***************************************************************************
                          wsplitter.h  -  description
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

#ifndef WSPLITTER_H
#define WSPLITTER_H

#include <qwidget.h>
#include <qsplitter.h>

/**new splitter class
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class WSplitter : public QSplitter  {
   Q_OBJECT
public: 
	WSplitter(QWidget *parent=0, const char *name=0);
	~WSplitter();
	
	void activate( QWidget *w1, QWidget *w2);
	
	void setPos( int pos);
	int getPos();
	
private:
	QWidget *w1,*w2;
	QValueList<int> ss;
};

#endif
