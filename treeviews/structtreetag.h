/***************************************************************************
                          structtreetag.h  -  description
                             -------------------
    begin                : Sat Apr 29 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002, 2003 Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@freemail.hu
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

#include <klistview.h>

class Tag;
class Node;
/**tag in structure widget
  *@author Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia
  */

class StructTreeTag : public KListViewItem  {

public: 
	StructTreeTag(QListView *parent, const QString a_title = QString::null );
	StructTreeTag(StructTreeTag *parent, Node *a_node, const QString a_title = QString::null, QListViewItem *after = 0L);
	StructTreeTag(StructTreeTag *parent, const QString a_title = QString::null );
	~StructTreeTag();
	
	Node *node;
};

#endif
