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
#include <kdebug.h>
#include <kurl.h>
#include <khtmlview.h>

const QString blankpagePosition = "file:blankpage.html";

SelectableArea::SelectableArea(QWidget *parent, const char *name ) : KHTMLPart(parent,name) {
    view()->setFrameShape(QFrame::NoFrame);
}
SelectableArea::~SelectableArea(){
}



void SelectableArea::khtmlMousePressEvent( khtml::MousePressEvent *event ){
  KHTMLPart::khtmlMousePressEvent( event );
  view()->setFrameShape(QFrame::Box);
  view()->setFrameShadow ( QFrame::Plain );
  view()->setLineWidth(2);
  emit selected(idLabel);

}

/*void SelectableArea::focusOutEvent ( QFocusEvent * )
{
    setFrameShape(QFrame::NoFrame);
}*/


/*void SelectableArea::resizeEvent ( QResizeEvent * e)
{
  QTextBrowser::resizeEvent(e);
  //QString dim="Width : "+QString::number(frameGeometry().width(),10)+"\nHeight : "+QString::number(frameGeometry().height(),10);
  //setText(dim);
  emit Resized(geometry());
}
*/

void SelectableArea::setSource(const QString& s){
  if(!s.isEmpty()) openURL( KURL(s) );
  else openURL(blankpagePosition);
}





#include "selectablearea.moc"






