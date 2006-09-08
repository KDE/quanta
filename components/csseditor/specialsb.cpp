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

#include <klineedit.h>

specialSB::specialSB(QWidget *parent, const char *name, bool useLineEdit ) : miniEditor(parent,name) {
  if (useLineEdit)
  {
    m_lineEdit = new KLineEdit(this);
    m_sb = 0L;
    connect(m_lineEdit, SIGNAL(textChanged ( const QString & )), this, SLOT(lineEditValueSlot ( const QString & )));
  }
  else
  {
    m_sb=new mySpinBox(this);
    connect(m_sb, SIGNAL(valueChanged ( const QString & )), this, SLOT(sbValueSlot(const QString&)));
    m_lineEdit = 0L;
  }
  m_cb = new QComboBox(this);
  connect(m_cb, SIGNAL(activated ( const QString & )), this, SLOT(cbValueSlot(const QString&)));
}

specialSB::~specialSB(){
  delete m_cb;
  delete m_sb;
  delete m_lineEdit;
}

void specialSB::connectToPropertySetter(propertySetter* p){
  connect(this, SIGNAL(valueChanged(const QString&)), p,SIGNAL(valueChanged(const QString&)));
}


void specialSB::cbValueSlot(const QString& s){
  if (m_sb)
    emit valueChanged( m_sb->text() +s );
  else
    emit valueChanged( m_lineEdit->text() +s );
}

void specialSB::sbValueSlot(const QString& s){
  emit valueChanged( s + m_cb->currentText());
}

void specialSB::lineEditValueSlot(const QString& s){
  emit valueChanged( s + m_cb->currentText());
}

void specialSB::setInitialValue(const QString& s){

  QRegExp pattern("\\d("+ cbValueList().join("|")+")");

  if (pattern.search(s) != -1) {
    QString temp(s.stripWhiteSpace());
    QString cbValue = pattern.cap(1);

    if (m_sb)
      m_sb->setValue(temp.remove(cbValue).toInt());
    else
      m_lineEdit->setText(temp.remove(cbValue));
    m_cb->setCurrentText(cbValue);
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

lengthEditor::lengthEditor(QWidget *parent, const char *name ) : specialSB(parent,name, true) {
  m_cb->insertItem("px");
  m_cb->insertItem("em");
  m_cb->insertItem("ex");
  m_cb->insertItem("in");
  m_cb->insertItem("cm");
  m_cb->insertItem("mm");
  m_cb->insertItem("pt");
  m_cb->insertItem("pc");
  if (m_sb)
    m_sb->setMaxValue(99999);
}





#include "specialsb.moc"
