/***************************************************************************
                          whtmlpart.h  -  description
                             -------------------
    begin                : Fri Aug 18 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef WHTMLPART_H
#define WHTMLPART_H

#include <khtml_part.h>
#include <qstrlist.h>

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class WHTMLPart : public KHTMLPart  {
   Q_OBJECT
public: 
	WHTMLPart(QWidget *parent=0, const char *name=0);
	~WHTMLPart();
	
	bool backEnable();
	bool forwardEnable();

public slots:

/** back(), forward() - navigation, using history.
 */
	void forward();		
	void back();
	
	void addToHistory( QString url );
	
signals:
	void updateStatus( bool back, bool forward );

protected:	
	virtual void urlSelected( const QString &url, int button = 0, int state = 0, const QString &_target = QString::null );
	
private:
	QStrList history;	
	unsigned int hpos;
		
};

#endif
