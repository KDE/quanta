/***************************************************************************
                          csseditor_globals.cpp  -  description
                             -------------------
    begin                : dom ago 3 2003
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

#include <qlineedit.h>
#include "csseditor_globals.h"

mySpinBox::mySpinBox(QWidget * parent , const char * name ) : QSpinBox (parent, name){
  connect( editor(), SIGNAL(textChanged(const QString&)), this, SLOT(checkSuffix( const QString & )));
  connect( editor(), SIGNAL(textChanged(const QString&)), this, SIGNAL(valueChanged( const QString & )));
}

mySpinBox::mySpinBox( int minValue, int maxValue, int step, QWidget * parent, const char * name ) : QSpinBox( minValue, maxValue, step, parent,name ){
  connect( editor(), SIGNAL(textChanged(const QString&)), this, SLOT(checkSuffix( const QString & )));
  connect( editor(), SIGNAL(textChanged(const QString&)), this, SIGNAL(valueChanged( const QString & )));
}

mySpinBox::~mySpinBox(){}

void mySpinBox::checkSuffix(const QString&){// check if the suffix is present and how many times : it normalizes these times to one
  if(!suffix().isEmpty()){
    const QString suf(suffix());
    QString t(editor()->text()); 
    if( t.contains(suf) ==0 ) {
      editor()->setText( t + suf);
      editor()->setCursorPosition(editor()->cursorPosition() - 1);
    }
    else 
    if( t.contains(suf) >1 ) {
      editor()->setText( t.remove(suf) + suf);
      editor()->setCursorPosition(editor()->cursorPosition() - 1);
    }  
  }
}


#include "csseditor_globals.moc"
