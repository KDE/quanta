/***************************************************************************
                          docitem.h  -  description
                             -------------------
    begin                : Fri Mar 3 2000
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

#ifndef DOCITEM_H
#define DOCITEM_H

#include <qwidget.h>
#include <qlistview.h>

class KConfig;

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class DocItem : public QListViewItem  {
//   Q_OBJECT
public: 
	DocItem(QListViewItem *parent, QString _name, QString _url);
	DocItem(QListView *parent, QString _name, QString _url);
	~DocItem();
	QString text( int column ) const;
	
public: // Public attributes
  QString url;
  QString name;

};

#endif
