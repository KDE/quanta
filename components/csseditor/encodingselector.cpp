/***************************************************************************
                          encodingselector.cpp  -  description
                             -------------------
    begin                : mer ago 6 2003
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



#include "encodingselector.h"
#include <kglobal.h>
#include <kcharsets.h> 
#include <qtextcodec.h>

/**
  *@author gulmini luciano
  */

encodingSelector::encodingSelector(QWidget *parent, const char* name) :  encodingSelectorS(parent,name){
  QStringList encodings (KGlobal::charsets()->availableEncodingNames());
  int insert = 0;
  for (uint i=0; i < encodings.count(); i++) {
    bool found = false;
    QTextCodec *codecForEnc = KGlobal::charsets()->codecForName(encodings[i], found);
 
    if (found){
      cbEncoding->insertItem (encodings[i]);
      insert++;
    }
  }
}

encodingSelector::~encodingSelector(){}

 

#include "encodingselector.moc"
