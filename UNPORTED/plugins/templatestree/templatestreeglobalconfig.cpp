/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "templatestreeglobalconfig.h"

#include "templatestreepart.h"

#include "settings.h"

#include <kconfigdialogmanager.h>

TemplatesTreeGlobalConfig::TemplatesTreeGlobalConfig(TemplatesTreePart *part, QWidget *parent)
    : QWidget(parent), m_part(part)
{
  setupUi(this);
  manager = new KConfigDialogManager(this, Settings::self());
  manager->updateWidgets();
}


void TemplatesTreeGlobalConfig::accept()
{
  manager->updateSettings();
  Settings::self()->writeConfig();
  emit accepted();
}


TemplatesTreeGlobalConfig::~TemplatesTreeGlobalConfig()
{
  delete manager;
}

#include "templatestreeglobalconfig.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
