/***************************************************************************
                          dcopquanta.h  -  description
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

#ifndef DCOPQUANTA_H
#define DCOPQUANTA_H

#include "dcopquantaif.h"

class DCOPQuanta : public DCOPQuantaIf
{

public:
  DCOPQuanta();
  ~DCOPQuanta() {};

  virtual QStringList selectors(const QString& tag);
  virtual QStringList idSelectors();
  virtual QStringList groupElements(const QString &group);
};

#endif
