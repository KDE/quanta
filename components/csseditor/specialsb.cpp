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
  m_sb=new QSpinBox(this);
  m_cb=new QComboBox(this);
  connect(m_cb, SIGNAL(activated ( const QString & )), this, SLOT(cbValueSlot(const QString&)));
  connect(m_sb, SIGNAL(valueChanged ( const QString & )), this, SLOT(sbValueSlot(const QString&)));

}

specialSB::~specialSB(){
  delete m_cb;
  delete m_sb;
}

void specialSB::cbValueSlot(const QString& s){
  m_cbValue=s;
  emit valueChanged( m_sbValue + m_cbValue );
}
void specialSB::sbValueSlot(const QString& s){
  m_sbValue=s;
  emit valueChanged( m_sbValue + m_cbValue);
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
  getComboBox()->insertItem("in");
  getComboBox()->insertItem("cm");
  getComboBox()->insertItem("mm");
  getComboBox()->insertItem("pt");
  getComboBox()->insertItem("pc");
  getSpinBox()->setMaxValue(99999);
}





#include "specialsb.moc"
