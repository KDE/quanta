/***************************************************************************
                          eventswidgetdlg.cpp  -  description
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
#include "eventswidgetdlg.h"
#include "tagwidget.h"
#include <qdict.h>

EventsWidgetDlg::EventsWidgetDlg(QWidget *parent, const char *name)
  : QWidget(parent,name), TagWidget(parent,name)
{
	initDialog();
}

EventsWidgetDlg::~EventsWidgetDlg(){
}


void EventsWidgetDlg::readAttributes( QDict<QString> *d )
{
  dict = d;

  updateDict("onClick", lineClick );
  updateDict("onDblClick", lineDblClick );
  updateDict("onMouseDown", lineMouseDown );
  updateDict("onMouseUp", lineMouseUp );
  updateDict("onMouseOver", lineMouseOver );
  updateDict("onMouseMove", lineMouseMove );
  updateDict("onMouseOut", lineMouseOut );
  updateDict("onKeyPress", lineKeyPress );
  updateDict("onKeyDown", lineKeyDown );
  updateDict("onKeyUp", lineKeyUp );

}

void EventsWidgetDlg::writeAttributes( QDict<QString> *d )
{
  dict = d;
  QString *t; // value of attr.

  if (( t=d->find("onClick") ))     setValue(*t, lineClick);
  if (( t=d->find("onDblClick") ))  setValue(*t, lineDblClick);
  if (( t=d->find("onMouseDown") )) setValue(*t, lineMouseDown);
  if (( t=d->find("onMouseUp") ))   setValue(*t, lineMouseUp);
  if (( t=d->find("onMouseOver") )) setValue(*t, lineMouseOver);
  if (( t=d->find("onMouseMove") )) setValue(*t, lineMouseMove);
  if (( t=d->find("onMouseOut") ))  setValue(*t, lineMouseOut);
  if (( t=d->find("onKeyPress") ))  setValue(*t, lineKeyPress);
  if (( t=d->find("onKeyDown") ))   setValue(*t, lineKeyDown);
  if (( t=d->find("onKeyUp") ))     setValue(*t, lineKeyUp);

}

