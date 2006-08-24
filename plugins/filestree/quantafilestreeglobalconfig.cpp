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

#include "quantafilestreeglobalconfig.h"

#include "quantafilestreepart.h"
#include "settings.h"

#include <kconfigdialogmanager.h>


QuantaFilesTreeGlobalConfig::QuantaFilesTreeGlobalConfig(QuantaFilesTreePart *part, QWidget *parent)
    : QWidget(parent), m_part(part)
{
  setupUi(this);
  manager = new KConfigDialogManager(this, Settings::self());
  manager->updateWidgets();
}


void QuantaFilesTreeGlobalConfig::accept()
{
  manager->updateSettings();
  emit accepted();
}


QuantaFilesTreeGlobalConfig::~QuantaFilesTreeGlobalConfig()
{
  delete manager;
}

#include "quantafilestreeglobalconfig.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
