/***************************************************************************
                          wtoolbar.cpp  -  description
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

#include "wtoolbar.h"
#include "wtoolbar.moc"
// QT includes
#include <qframe.h>
#include <qsizepolicy.h>

// KDE includes


WToolBar::WToolBar(QWidget *parent, const char *name )
	: KToolBar(parent,name)
{
	fixingSize();
}

WToolBar::~WToolBar()
{
}

void WToolBar::fixingSize()
{
	setBarPos( Floating);
	setBarPos( Top);

	//enableMoving(false);
	//enableFloating(false);

	setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum ) );
}

void WToolBar::insertSeparator()
{
#ifdef OLD_SEPARATORS
	QFrame *f  = new QFrame (this);
	QWidget *w1= new QWidget(this);
	QWidget *w2= new QWidget(this);

  f -> setFrameStyle( QFrame::VLine | QFrame::Sunken );
  w1-> setMinimumWidth( 7 );
  w2-> setMinimumWidth( 7 );

  insertWidget( 7,0,  w1);
  insertWidget( 0,16, f );
  insertWidget( 7,0,  w2);
#else
  insertLineSeparator();
#endif
}
