/***************************************************************************
                              gubedsettings.cpp
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


#include "qlineedit.h"
#include "gubedsettings.h"

GubedSettings::GubedSettings(QWidget* parent, const char* name, bool modal, WFlags fl)
		: GubedSettingsS(parent,name, modal,fl)
{}

GubedSettings::~GubedSettings()
{}

void GubedSettings::slotUseProxyToggle( bool useproxy)
{
    lineServerHost->setEnabled(useproxy);
    lineServerPort->setEnabled(useproxy);
    lineServerListenPort->setEnabled(!useproxy);
}


#include "gubedsettings.moc"

