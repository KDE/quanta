/***************************************************************************
                    dcopsettingsif.h  -  description
                             -------------------
    begin                : Thu Jan 29 2004
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef DCOPSETTINGSIF_H
#define DCOPSETTINGSIF_H

#include <dcopobject.h>
#include <qstringlist.h>

class DCOPSettingsIf : virtual public DCOPObject
{
  K_DCOP

  k_dcop:

  virtual QString encoding() = 0;
  virtual QString dtep() = 0;
  virtual QString quotationChar() = 0;
};

#endif

