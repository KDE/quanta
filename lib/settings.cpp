/***************************************************************************
    begin                : Sun Jul 24 2005
    copyright            : (C) 2005 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "settings.h"

#include <kstaticdeleter.h>
#include <kconfig.h>


Settings * Settings::m_prjSettings = 0;
static KStaticDeleter<Settings> staticSettingsPrjDeleter;

Settings *Settings::m_Self = 0;
static KStaticDeleter<Settings> staticSettingsDeleter;

Settings *Settings::self()
{
  if (! m_Self)
  {
    staticSettingsDeleter.setObject(m_Self, new Settings());
    m_Self->readConfig();
  }
  return m_Self;
}


Settings::Settings()
  : SettingsBase()
{
}


Settings::~Settings()
{
}


void Settings::deletePrjSettings()
{
  if (m_prjSettings)
    staticSettingsDeleter.destructObject();
}


void Settings::createPrjSettings()
{
  if (m_prjSettings)
    staticSettingsPrjDeleter.destructObject();

  staticSettingsPrjDeleter.setObject(m_prjSettings, new Settings());
  m_prjSettings->readConfig(); // read global values as default
}


// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
