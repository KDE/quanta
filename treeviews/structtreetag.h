/***************************************************************************
                          structtreetag.h  -  description
                             -------------------
    begin                : Sat Apr 29 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
    email                : pdima@mail.univ.kiev.ua
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STRUCTTREETAG_H
#define STRUCTTREETAG_H

#include <qwidget.h>
#include <qlistview.h>
#include "../parser/tag.h"

/**tag in structure widget
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class StructTreeTag : public QListViewItem  {

public: 
	StructTreeTag(QListView *parent, QString name = QString::null );
	StructTreeTag(StructTreeTag *parent, Tag *tag, QString name = QString::null );
	StructTreeTag(StructTreeTag *parent, QString name = QString::null );
	~StructTreeTag();
	
	int pos1,pos2;
	
private: // Private methods
  /** set pixmap of tag */
//  void setPixmap(QString name);
};

#endif
