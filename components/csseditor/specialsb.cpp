/***************************************************************************
                          specialsb.cpp  -  description
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

#include "specialsb.h"
#include "propertysetter.h"
#include "csseditor_globals.h"

specialSB::specialSB(QWidget *parent, const char *name ) : miniEditor(parent,name) {
  m_sb=new mySpinBox(this);
  m_cb=new QComboBox(this);
  connect(m_cb, SIGNAL(activated ( const QString & )), this, SLOT(cbValueSlot(const QString&)));
  connect(m_sb, SIGNAL(valueChanged ( const QString & )), this, SLOT(sbValueSlot(const QString&)));
}

specialSB::~specialSB(){
  delete m_cb;
  delete m_sb;
}

void specialSB::connectToPropertySetter(propertySetter* p){
  connect(this, SIGNAL(valueChanged(const QString&)), p,SIGNAL(valueChanged(const QString&)));
}
 

void specialSB::cbValueSlot(const QString& s){
  emit valueChanged( m_sb->text() +s );
}

void specialSB::sbValueSlot(const QString& s){
  emit valueChanged( s + m_cb->currentText());
}

void specialSB::setInitialValue(QString s){

  QRegExp pattern("\\d("+ cbValueList().join("|")+")");
  
  if(s.contains(pattern)) {
    QString temp1(s.stripWhiteSpace()),
                 temp2(s.stripWhiteSpace());
                 
    m_sb->setValue(temp1.remove(QRegExp("\\D")).toInt());
    m_cb->setCurrentText(temp2.remove(QRegExp("\\d")));              
  }
  else return;
}

QStringList specialSB::cbValueList(){
  QStringList l;
  for(int i=0; i<m_cb->count();i++) l.append(m_cb->text(i));
  return l;
}

frequencyEditor::frequencyEditor(QWidget *parent, const char *name ) : specialSB(parent,name) {
  m_cb->insertItem("Hz");
  m_cb->insertItem("kHz");
  m_sb->setMaxValue(9999);
}

angleEditor::angleEditor(QWidget *parent, const char *name) : specialSB(parent,name){
  m_cb->insertItem("deg");
  m_cb->insertItem("grad");
  m_cb->insertItem("rad");
  m_sb->setMaxValue(-400);
  m_sb->setMaxValue(400);
}

timeEditor::timeEditor(QWidget *parent, const char *name ) : specialSB(parent,name) {
  m_cb->insertItem("ms");
  m_cb->insertItem("s");
  m_sb->setMaxValue(99999);
}

lengthEditor::lengthEditor(QWidget *parent, const char *name ) : specialSB(parent,name) {
  m_cb->insertItem("px");
  m_cb->insertItem("em");
  m_cb->insertItem("ex");
  m_cb->insertItem("in");
  m_cb->insertItem("cm");
  m_cb->insertItem("mm");
  m_cb->insertItem("pt");
  m_cb->insertItem("pc");
  m_sb->setMaxValue(99999);
}





#include "specialsb.moc"
