/***************************************************************************
                          cervisiaplugin.cpp  -  description
                             -------------------
    begin                : Tue Sep 24 2002
    copyright            : (C) 2002 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


/* KDE INCLUDES */
#include <kparts/componentfactory.h>
#include <kparts/part.h>
#include <kstandarddirs.h>

/* QT INCLUDES */
#include <qwidget.h>
#include <qwidgetstack.h>

/* APP INCLUDES */
#include "cervisiaplugin.h"
#include "../quanta.h"
#include "../quantadoc.h"
#include "../resource.h"

CervisiaPlugin::CervisiaPlugin():QuantaKPartPlugin()
{
  m_part = 0;
}

CervisiaPlugin::~CervisiaPlugin()
{
}

bool CervisiaPlugin::run()
{
  if(!isLoaded())
    load();

  if(isLoaded())
  {
    m_part->openURL(quantaApp->projectBaseURL());
    showGui(true);
    addWidget();

    setRunning(true);

    emit pluginStarted();

    return true;
  }
  return false;
}

bool CervisiaPlugin::unload()
{
  if(!isLoaded())
    return false;

  showGui(false);
  removeWidget();
  delete m_part;
  m_part = 0;
  setRunning(false);

  emit pluginStopped();

  return true;
}

