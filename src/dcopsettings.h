/***************************************************************************
                          dcopsettings.h  -  description
                             -------------------
    begin                : Thu Jan 29 2004
    copyright            : (C) 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DCOPSETTINGS_H
#define DCOPSETTINGS_H


#include "dcopsettingsif.h"

class DCOPSettings : public DCOPSettingsIf
{

public:
  DCOPSettings();
  ~DCOPSettings() {};

  virtual QString encoding();
  virtual QString dtep();
  virtual QString quotationChar();
};

#endif
