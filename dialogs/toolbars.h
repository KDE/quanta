/***************************************************************************
                          toolbars.h  -  description
                             -------------------
    begin                : Fri Oct 13 2000
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

#ifndef TOOLBARS_H
#define TOOLBARS_H

#include <qstring.h>
#include <qdom.h>


/**
  *@author Dmitry Poplavsky & Alexander Yakovlev
  */

/*struct Action {
	enum types { Empty, Standard, Separator, Tag, Script, Text };
	int id;
	int type;
	QString name;
};
*/

class ToolBars {
public: 
	ToolBars( QDomDocument doc );
	~ToolBars();
	
	/** return count of toolbars */
	int count() { return dtoolbars.count(); }
	
	/** return caption for t_id toolbar */
	QString toolbarName( int t_id ) { return dtoolbars.item(t_id).toElement().attribute("name",""); }
	
	QDomElement toolbar( int t_id ) { return dtoolbars.item(t_id).toElement(); }
	
	/** return list of actions ids for t_id toolbar */
	QDomNodeList actions( int t_id ) { return toolbar(t_id).childNodes(); }
	
	/** return action with id */
	QDomElement action( int id );
	
	QPixmap actionIcon( int id );
		
	QDomDocument d;
	
	QDomNodeList dtoolbars;
};

#endif
