/***************************************************************************
                          minieditor.h  -  description
                             -------------------
    begin                : lun ago 9 2004
    copyright            : (C) 2004 by gulmini luciano
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

#ifndef MINIEDITOR_H
#define MINIEDITOR_H

#include <qhbox.h>

/**
  *@author gulmini luciano
  */

class propertySetter;  
  
class miniEditor : public QHBox{
  public:
    miniEditor(QWidget *parent=0, const char *name=0):QHBox(parent,name){}
    ~miniEditor(){}
    virtual void connectToPropertySetter(propertySetter* p)=0;
};
  

#endif
