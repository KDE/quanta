/***************************************************************************
                          doubleeditors.cpp  -  description
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
 #include "doubleeditors.h"
 #include "specialsb.h"
 #include <qcombobox.h>
 #include "csseditor_globals.h"
 #include <qregexp.h>
 
doubleEditorBase::doubleEditorBase(QWidget *parent, const char *name) : QHBox(parent,name){
}

void doubleEditorBase::sxValueSlot(const QString& v){
  m_sxValue=v;
  emit valueChanged( m_sxValue +" " + m_dxValue);
}

void doubleEditorBase::dxValueSlot(const QString& v){
  m_dxValue=v;
  emit valueChanged( m_sxValue +" " + m_dxValue);
} 
 
 doubleLengthEditor::doubleLengthEditor(QWidget *parent, const char *name) : doubleEditorBase(parent,name){
  m_ssbSx = new specialSB(this);
  m_ssbSx->insertItem("cm");
  m_ssbSx->insertItem("em");
  m_ssbSx->insertItem("ex");
  m_ssbSx->insertItem("in");
  m_ssbSx->insertItem("mm");  
  m_ssbSx->insertItem("pc");
  m_ssbSx->insertItem("pt");
  m_ssbSx->insertItem("px");
  
  m_ssbDx = new specialSB(this);
  m_ssbDx->insertItem("cm"); 
  m_ssbDx->insertItem("em");
  m_ssbDx->insertItem("ex");
  m_ssbDx->insertItem("in");
  m_ssbDx->insertItem("mm"); 
  m_ssbDx->insertItem("pc");
  m_ssbDx->insertItem("pt"); 
  m_ssbDx->insertItem("px"); 
    
  connect(m_ssbSx, SIGNAL(valueChanged(const QString&)), this, SLOT(sxValueSlot(const QString&)));
  connect(m_ssbDx, SIGNAL(valueChanged(const QString&)), this, SLOT(dxValueSlot(const QString&)));
}

doubleLengthEditor::~doubleLengthEditor(){
  delete m_ssbSx;
  delete m_ssbDx;
}

void doubleLengthEditor::setInitialValue(QString sx, QString dx){
  QRegExp pattern("\\d"+m_ssbSx->cbValueList().join("|"));
  
  if(sx.contains(pattern)) {
    QString temp1(sx.stripWhiteSpace()),
                 temp2(sx.stripWhiteSpace());
    m_ssbSx->setInitialValue(temp1.remove(QRegExp("\\D")), temp2.remove(QRegExp("\\d")));
  }
  
  if(dx.contains(pattern)) {
    QString temp1(dx.stripWhiteSpace()),
                 temp2(dx.stripWhiteSpace());
    m_ssbDx->setInitialValue(temp1.remove(QRegExp("\\D")), temp2.remove(QRegExp("\\d")));
  }
}

doubleComboBoxEditor::doubleComboBoxEditor(QWidget *parent, const char *name) : doubleEditorBase(parent,name){
  m_cbSx = new QComboBox(this);
  m_cbDx = new QComboBox(this);
  connect(m_cbSx, SIGNAL(activated ( const QString & )), this, SLOT(sxValueSlot(const QString&)));
  connect(m_cbDx, SIGNAL(activated ( const QString & )), this, SLOT(dxValueSlot(const QString&)));
}

doubleComboBoxEditor::~doubleComboBoxEditor(){
  delete m_cbSx;
  delete m_cbDx;
}

doublePercentageEditor::doublePercentageEditor(QWidget *parent, const char *name) : doubleEditorBase(parent,name){
  m_sbSx = new mySpinBox(this);
  m_sbDx = new mySpinBox(this);
  m_sbSx->setSuffix("%");
  m_sbDx->setSuffix("%");
  connect(m_sbSx,SIGNAL(valueChanged(const QString&)),this,SLOT(sxValueSlot(const QString&)));
  connect(m_sbDx,SIGNAL(valueChanged(const QString&)),this,SLOT(dxValueSlot(const QString&)));
}

doublePercentageEditor::~doublePercentageEditor(){
  delete m_sbSx;
  delete m_sbDx;
}

void doublePercentageEditor::setInitialValue(QString sx, QString dx){
  m_sbSx->setValue(sx.remove("%").toInt());
  m_sbDx->setValue(dx.remove("%").toInt());
}

#include "doubleeditors.moc"