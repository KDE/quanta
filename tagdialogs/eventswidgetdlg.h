/***************************************************************************
                          eventswidgetdlg.h  -  description
                             -------------------
    begin                : Thu Mar 30 2000
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

#ifndef EVENTSWIDGETDLG_H
#define EVENTSWIDGETDLG_H

#include "tagwidget.h"
#include "eventswidgetdlgs.h"

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class EventsWidgetDlg : public EventsWidgetDlgS, TagWidget {
   Q_OBJECT
public: 
	EventsWidgetDlg(QWidget *parent=0, const char *name=0);
	~EventsWidgetDlg();
	
  void readAttributes( QDict<QString> *d);
  void writeAttributes( QDict<QString> *d );	

};

#endif
