/***************************************************************************
                          doubleeditors.cpp  -  description
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
 #include "doubleeditors.h"
 #include "specialsb.h"
 #include <qcombobox.h>
 #include <qspinbox.h>
 
 doubleLengthEditor::doubleLengthEditor(QWidget *parent, const char *name) : QHBox(parent,name)
{
  ssbSx = new specialSB(this);
  ssbDx = new specialSB(this);
  ssbSx->getComboBox()->insertItem("px");
  ssbSx->getComboBox()->insertItem("em");
  ssbSx->getComboBox()->insertItem("ex");
  ssbDx->getComboBox()->insertItem("px");
  ssbDx->getComboBox()->insertItem("em");
  ssbDx->getComboBox()->insertItem("ex");
  connect(ssbSx, SIGNAL(valueChanged(const QString&)), this, SLOT(ssbSxValueSlot(const QString&)));
  connect(ssbDx, SIGNAL(valueChanged(const QString&)), this, SLOT(ssbDxValueSlot(const QString&)));
}

doubleLengthEditor::~doubleLengthEditor()
{
  if(ssbSx) delete ssbSx;
  ssbSx=0;
  if(ssbDx) delete ssbDx;
  ssbDx=0;
}

void doubleLengthEditor::ssbSxValueSlot(const QString& v)
{
  ssbSxValue=v;
  emit valueChanged( ssbSxValue +" " + ssbDxValue);
}

void doubleLengthEditor::ssbDxValueSlot(const QString& v){
  ssbDxValue=v;
  emit valueChanged( ssbSxValue +" " + ssbDxValue);
}

doubleComboBoxEditor::doubleComboBoxEditor(QWidget *parent, const char *name) : QHBox(parent,name)
{
  cbSx = new QComboBox(this);
  cbDx = new QComboBox(this);
  connect(cbSx, SIGNAL(activated ( const QString & )), this, SLOT(sxValueSlot(const QString&)));
  connect(cbDx, SIGNAL(activated ( const QString & )), this, SLOT(dxValueSlot(const QString&)));

}

doubleComboBoxEditor::~doubleComboBoxEditor()
{
  if(cbSx) delete cbSx;
  cbSx=0;
  if(cbDx) delete cbDx;
  cbDx=0;
}



void doubleComboBoxEditor::sxValueSlot(const QString& s){
  sxValue=s;
  emit valueChanged( sxValue + " " + dxValue );
}
void doubleComboBoxEditor::dxValueSlot(const QString& s){
  dxValue=s;
  emit valueChanged( sxValue + " " + dxValue);
}

doublePercentageEditor::doublePercentageEditor(QWidget *parent, const char *name) : QHBox/*multipleSpinBox*/(parent,name)
{
  sbSx = new QSpinBox(this);
  sbDx = new QSpinBox(this);
  sbSx->setSuffix("%");
  sbDx->setSuffix("%");
  connect(sbSx,SIGNAL(valueChanged(const QString&)),this,SLOT(sxSBValueSlot(const QString&)));
  connect(sbDx,SIGNAL(valueChanged(const QString&)),this,SLOT(dxSBValueSlot(const QString&)));
  //setSuffix("%");
}

doublePercentageEditor::~doublePercentageEditor()
{
  if(sbSx) delete sbSx;
  sbSx=0;
  if(sbDx) delete sbDx;
  sbDx=0;
}

void doublePercentageEditor::sxSBValueSlot(const QString& v){
  sxSBValue=v;
  emit valueChanged( sxSBValue +" " + dxSBValue);

}

void doublePercentageEditor::dxSBValueSlot(const QString& v){
  dxSBValue=v;
  emit valueChanged( sxSBValue +" " + dxSBValue);

}





#include "doubleeditors.moc"
