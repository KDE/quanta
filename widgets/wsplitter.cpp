/***************************************************************************
                          wsplitter.cpp  -  description
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

#include "wsplitter.h"

#include <qmotifstyle.h>

WSplitter::WSplitter(QWidget *parent, const char *name )
	: QSplitter(parent,name)
{
	setStyle( new QMotifStyle() );
}

WSplitter::~WSplitter(){
}

void WSplitter::activate( QWidget *, QWidget *)
{
	ss = sizes();
}

void WSplitter::setPos( int pos)
{
	int s1 = ss[0];
	int s2 = ss[1];
	
	int ns1 = ((s1+s2)*pos)/1000;
	int ns2 = ((s1+s2)*(1000-pos))/1000;
	
	ss[0] = ns1;
	ss[1] = ns2;
	
	setSizes(ss);
}

int WSplitter::getPos()
{
	ss = sizes();
	
	int s1 = ss[0];
	int s2 = ss[1];
	
	return (s1*1000)/(s1+s2);
}
