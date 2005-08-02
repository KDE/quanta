/***************************************************************************
                              dbgpsettings.cpp
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
#include "qcheckbox.h"
#include "dbgpsettings.h"
#include <ktextbrowser.h>

DBGpSettings::DBGpSettings(const QString &protocolversion)
    : DBGpSettingsS(0, "DBGpSettings", false, 0)
{
  textAbout->setText(textAbout->text().replace("%PROTOCOLVERSION%", protocolversion));
  connect(checkLocalProject, SIGNAL(toggled(bool)), this, SLOT(slotLocalProjectToggle(bool)));
}

DBGpSettings::~DBGpSettings()
{
}

void DBGpSettings::slotLocalProjectToggle( bool localproject)
{
  lineServerBasedir->setEnabled(!localproject);
  lineLocalBasedir->setEnabled(!localproject);
}




#include "dbgpsettings.moc"

