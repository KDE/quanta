/***************************************************************************
                          tagquicktableitem.cpp  -  description
                             -------------------
    begin                : Wed Jul 17 2002
    copyright            : (C) 2002 by Andras Mantia
    email                : amantia@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tagquicktableitem.h"

TagQuickTableItem::TagQuickTableItem(QListView *parent) : QListViewItem(parent) {
}

TagQuickTableItem::TagQuickTableItem(QListView *parent,QString label1) : QListViewItem(parent, label1) {
}

TagQuickTableItem::~TagQuickTableItem(){
}

/** No descriptions */
int TagQuickTableItem::compare(QListViewItem *i, int col, bool ascending) const
{
 QString s1 = key(col, ascending).section('-',-1);
 QString s2 = i->key(col, ascending).section('-',-1);

 int result = s1.toInt()-s2.toInt();

 if (result < 0) result = -1;
 if (result > 0) result = 1;

 return result;
}
