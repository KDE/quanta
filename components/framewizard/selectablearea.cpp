/***************************************************************************
                          selectablearea.cpp  -  description
                             -------------------
    begin                : mer mar 5 2003
    copyright            : (C) 2003 by gulmini luciano
    email                : gulmini.luciano@student.unife.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "selectablearea.h"
#include <qframe.h>
//#include <kdebug.h>
#include "fwglobal.h"

SelectableArea::SelectableArea(QWidget *parent, const char *name ) : KHTMLPart(parent,name) {
  view()->setFrameShape(QFrame::NoFrame);
  view()->setMinimumSize(QSize(1,1));
  view()->installEventFilter(this);
}

SelectableArea::~SelectableArea(){}

bool SelectableArea::eventFilter(QObject *o, QEvent *event){
  switch ( event->type() ) {
    case QEvent::FocusIn : {
                                             view()->setFrameShape(QFrame::Box);
                                             view()->setFrameShadow ( QFrame::Plain );
                                             view()->setLineWidth(2);
                                             emit selected(m_idLabel);
                                             return true;
                                           };
                                           break;
    case QEvent::FocusOut : {
                                               view()->setFrameShape(QFrame::NoFrame);
                                               return true;
                                             }
                                             break;
    case QEvent::Resize : {
                                          emit Resized( view()->geometry() );
                                          view()->hide();
                                          view()->show();
                                          return true;
                                         }
                                         break;
      default: return KHTMLPart::eventFilter( o, event );
  }
}

#include "selectablearea.moc"
