/***************************************************************************
                          specialsb.cpp  -  description
                             -------------------
    begin                : dom ago 3 2003
    copyright            : (C) 2003 by simone
    email                : simone@localhost
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "specialsb.h"
#include <qcombobox.h>
#include <qspinbox.h>

specialSB::specialSB(QWidget *parent, const char *name ) : QHBox(parent,name) {
  sb=new QSpinBox(this);
  cb=new QComboBox(this);
  connect(cb, SIGNAL(activated ( const QString & )), this, SLOT(cbValueSlot(const QString&)));
  connect(sb, SIGNAL(valueChanged ( const QString & )), this, SLOT(sbValueSlot(const QString&)));

}

specialSB::~specialSB(){
  if(cb) delete cb;
  cb = 0;
  if(sb) delete sb;
  sb = 0;
}

void specialSB::cbValueSlot(const QString& s){
  cbValue=s;
  emit valueChanged( sbValue + cbValue );
}
void specialSB::sbValueSlot(const QString& s){
  sbValue=s;
  emit valueChanged( sbValue + cbValue);
}

frequencyEditor::frequencyEditor(QWidget *parent, const char *name ) : specialSB(parent,name) {
  getComboBox()->insertItem("Hz");
  getComboBox()->insertItem("kHz");
  getSpinBox()->setMaxValue(9999);
}

angleEditor::angleEditor(QWidget *parent, const char *name) : specialSB(parent,name){
  getComboBox()->insertItem("deg");
  getComboBox()->insertItem("grad");
  getComboBox()->insertItem("rad");
  getSpinBox()->setMaxValue(-360);
  getSpinBox()->setMaxValue(360);
}

timeEditor::timeEditor(QWidget *parent, const char *name ) : specialSB(parent,name) {
  getComboBox()->insertItem("ms");
  getComboBox()->insertItem("s");
  getSpinBox()->setMaxValue(99999);
}

lengthEditor::lengthEditor(QWidget *parent, const char *name ) : specialSB(parent,name) {
  getComboBox()->insertItem("px");
  getComboBox()->insertItem("em");
  getComboBox()->insertItem("ex");
  getSpinBox()->setMaxValue(99999);
}





#include "specialsb.moc"
