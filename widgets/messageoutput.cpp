/***************************************************************************
                          messageoutput.cpp  -  description
                             -------------------
    begin                : Thu Feb 24 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski
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

#include "messageoutput.h"

MessageOutput::MessageOutput(QWidget *parent, const char *name )
  : KEdit(parent,name)
{
  insertAtEnd( "Message window...\n\n" );
}

MessageOutput::~MessageOutput(){
}

void MessageOutput::insertAtEnd(QString s)
{
  int row = (numLines() == 0)? 0 : numLines()-1;
  int col = qstrlen(textLine(row));
  insertAt(s, row, col);
}

void MessageOutput::mouseReleaseEvent(QMouseEvent*){
  emit clicked();
}

void MessageOutput::keyPressEvent ( QKeyEvent* event){
  KEdit::keyPressEvent(event);
  emit keyPressed(event->ascii());
}
