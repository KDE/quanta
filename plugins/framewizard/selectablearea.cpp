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

SelectableArea::SelectableArea(QWidget *parent, const char *name ) : QTextBrowser(parent,name) {
    setFrameShape(QFrame::NoFrame);
    connect(this, SIGNAL(clicked(int, int)), SLOT(slotClicked(int, int)));

}
SelectableArea::~SelectableArea(){
}

void SelectableArea::slotClicked(int, int)
{
    setFrameShape(QFrame::Box);
    setFrameShadow ( QFrame::Plain );
    setLineWidth(2);
    emit selected(idLabel);                             // the widget has been selected

}

void SelectableArea::focusOutEvent ( QFocusEvent * )
{
    setFrameShape(QFrame::NoFrame);
}


void SelectableArea::resizeEvent ( QResizeEvent * e)
{
  QTextBrowser::resizeEvent(e);
  //QString dim="Width : "+QString::number(frameGeometry().width(),10)+"\nHeight : "+QString::number(frameGeometry().height(),10);
  //setText(dim);
  emit Resized(geometry());
}

#include "selectablearea.moc"






