/***************************************************************************
                               gubedsettings.h
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

#ifndef GUBEDSETTINGS_H
#define GUBEDSETTINGS_H

#include "gubedsettingss.h"

class GubedSettings : public GubedSettingsS
{
    Q_OBJECT

  public slots:
    virtual void slotUseProxyToggle( bool useproxy);

  public:
    GubedSettings(const QString &protocolversion);
    ~GubedSettings();

};

#endif

