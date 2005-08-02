/***************************************************************************
                               dbgpsettings.h
                             -------------------
    begin                : 2005-08-01
    copyright            : (C) 2005 Linus McCabe <linus@mccabe.nu>
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

  public:
    DBGpSettings(const QString &protocolversion);
    ~DBGpSettings();

  public slots:
    virtual void slotLocalProjectToggle(bool localproject);
};

#endif

