/***************************************************************************
                    dcopwindowmanagerif.h  -  description
                             -------------------
    begin                : Wed Feb 5 2003
    copyright            : (C) 2003 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DCOPWINDOWMANAGERIF_H
#define DCOPWINDOWMANAGERIF_H

#include <dcopobject.h>

class DCOPWindowManagerIf : virtual public DCOPObject
{
  K_DCOP

  k_dcop:

  virtual int currentEditorIfNum() const = 0;
  virtual QString currentURL() const = 0;
  virtual QString projectURL() const = 0;
};

#endif
