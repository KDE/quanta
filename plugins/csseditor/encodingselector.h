/***************************************************************************
                          encodingselector.h  -  description
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

#ifndef ENCODINGSELECTOR_H
#define ENCODINGSELECTOR_H


#include "encodingselectors.h"
#include <qcombobox.h>

/**
  *@author gulmini luciano
  */

class encodingSelector : public encodingSelectorS {
  Q_OBJECT
                 
  public: 
    encodingSelector(QWidget *parent=0, const char* name=0);
    ~encodingSelector();
    QString encodingSet() const { return cbEncoding->currentText();}

   };

#endif
 
