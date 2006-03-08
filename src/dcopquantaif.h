/***************************************************************************
                          dcopquantaif.h  -  description
                             -------------------
    begin                : Thu Mar 4 2004
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

#ifndef DCOPQUANTAIF_H
#define DCOPQUANTAIF_H
#include <dcopobject.h>
#include <qstringlist.h>

class DCOPQuantaIf : virtual public DCOPObject
{
  K_DCOP

  k_dcop:

  virtual QStringList selectors(const QString &tag) = 0;
  virtual QStringList idSelectors() = 0;
  virtual QStringList groupElements(const QString &group) = 0;
};

#endif
