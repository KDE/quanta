/***************************************************************************
                          percentageeditor.cpp  -  description
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
#include "percentageeditor.h"
#include "propertysetter.h"

percentageEditor::percentageEditor(const QString& initialValue, QWidget *parent, const char *name) : miniEditor(parent,name)
{
  QString temp(initialValue);
  m_sb = new mySpinBox(0,9999,1,this);
  m_sb->setValue(temp.remove("%").toInt());
  m_sb->setSuffix("%");
  connect(m_sb, SIGNAL(valueChanged ( const QString & )), this, SIGNAL(valueChanged(const QString&)));
}

percentageEditor::~percentageEditor()
{
  delete m_sb;
}

void percentageEditor::connectToPropertySetter(propertySetter* p){
  connect( this, SIGNAL(valueChanged(const QString&)), p, SIGNAL(valueChanged(const QString&)));
}

#include "percentageeditor.moc"
