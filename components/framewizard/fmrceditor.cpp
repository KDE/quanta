/***************************************************************************
                          fmrceditor.cpp  -  description
                             -------------------
    begin                : mer giu 4 2003
    copyright            : (C) 2003 by Gulmini Luciano
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

#include "fmrceditor.h"
#include <qspinbox.h>
#include <qlabel.h>

fmRCeditor::fmRCeditor() : fmRCeditorS(){}

fmRCeditor::~fmRCeditor(){}

int fmRCeditor::spinBoxValue() const {
  return sb->value();
}

void fmRCeditor::setLabelText(QString s){
  tl->setText(s);
}

#include "fmrceditor.moc"
