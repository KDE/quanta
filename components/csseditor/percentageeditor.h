/***************************************************************************
                          percentageeeditor.h  -  description
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

#ifndef PERCENTAGEEDITOR_H
#define PERCENTAGEEDITOR_H

#include <qhbox.h>
#include <qptrlist.h>
#include "doubleeditors.h"
#include <qcombobox.h>
#include <qslider.h>

#include "csseditor_globals.h"
#include "minieditor.h"

class KPushButton;


/**
  *@author gulmini luciano
  */
 
class propertySetter;  
   
class percentageEditor : public miniEditor {
     Q_OBJECT
   private:
     mySpinBox *m_sb;
   public:
           percentageEditor(const QString& initialValue="0",QWidget *parent=0, const char *name=0);
           ~percentageEditor();
           virtual void connectToPropertySetter(propertySetter* p);
   signals:
     void valueChanged(const QString&);
};


#endif
