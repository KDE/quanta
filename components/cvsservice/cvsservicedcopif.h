/***************************************************************************
                                              cvsservicedcopif.h  -  description
                                             ------------------------------
    begin                : Sun May 16 22:50:25 2004
    copyright          : (C) 2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2
 *
 ***************************************************************************/

#ifndef CVSSERVICEDCOPIF_H
#define CVSSERVICEDCOPIF_H

#include <qstring.h>
#include <dcopobject.h>

class CVSServiceDCOPIf : virtual public DCOPObject
{
    K_DCOP
k_dcop:
    virtual void slotJobExited( bool normalExit, int exitStatus ) = 0;
    virtual void slotReceivedStdout( QString someOutput ) = 0;
};

#endif
