/***************************************************************************
                          fmrceditor.h  -  description
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

#ifndef FMRCEDITOR_H
#define FMRCEDITOR_H

#include <fmrceditors.h>

/**
  *@author Gulmini Luciano
  */

class fmRCeditor : public fmRCeditorS  {
  Q_OBJECT
  public: 
    fmRCeditor();
    ~fmRCeditor();
  public:
    int spinBoxValue() const;
    void setLabelText(QString);

};

#endif
