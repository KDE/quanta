/***************************************************************************
                               dbgpsettings.h
                             -------------------
    begin                : 2004-04-05
    copyright            : (C) 2004 Linus McCabe <linus@mccabe.nu>
 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/

#ifndef DBGPSETTINGS_H
#define DBGPSETTINGS_H

#include "dbgpsettingss.h"

class DBGpSettings : public DBGpSettingsS
{
    Q_OBJECT

  public slots:
    virtual void slotUseProxyToggle( bool useproxy);

  public:
    DBGpSettings(const QString &protocolversion);
    ~DBGpSettings();

};

#endif

