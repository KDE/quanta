/***************************************************************************
                          newstuff.h  -  description
                             -------------------
    begin                : Tue Jun 22 12:19:55 2004
    copyright          : (C) 2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef NEWSTUFF_H
#define NEWSTUFF_H

//qt includes
#include <qobject.h>

//app includes
#include "qnewstuff.h"

/**
Makes possible downloading and installing a DTEP resource files from a server.

@author Andras Mantia
*/
class QNewDTEPStuff: public QNewStuff
{
   Q_OBJECT

public:
    QNewDTEPStuff(const QString &type,  QWidget *parentWidget=0)
      :QNewStuff(type, parentWidget){};
    ~QNewDTEPStuff() {};

private:
     virtual void installResource();
};

/**
Makes possible downloading and installing a Toolbar resource files from a server.

@author Andras Mantia
*/
class QNewToolbarStuff: public QNewStuff
{
   Q_OBJECT

public:
    QNewToolbarStuff(const QString &type,  QWidget *parentWidget=0)
      :QNewStuff(type, parentWidget){};
    ~QNewToolbarStuff() {};

private:
     virtual void installResource();
};

#endif
