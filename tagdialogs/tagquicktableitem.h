/***************************************************************************
                          tagquicktableitem.h  -  description
                             -------------------
    begin                : Wed Jul 17 2002
    copyright            : (C) 2002 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TAGQUICKTABLEITEM_H
#define TAGQUICKTABLEITEM_H

#include <qlistview.h>
#include <qstring.h>

/**
  *@author Andras Mantia
  */

class TagQuickTableItem : public QListViewItem  {
public:
  TagQuickTableItem(QListView *parent=0);
  TagQuickTableItem(QListView *parent, QString label1);
  ~TagQuickTableItem();
  /** No descriptions */
  virtual int compare(QListViewItem *i, int col, bool ascending) const;
};

#endif
