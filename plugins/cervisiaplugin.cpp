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
    quantaApp->guiFactory()->addClient(m_part);
    QWidgetStack *stack = quantaApp->widgetStackOfHtmlPart();
    stack->raiseWidget(m_part->widget());
    m_part->widget()->show();

    setRunning(TRUE);

  emit pluginStarted();

    return TRUE;
  }
  return FALSE;
}

bool CervisiaPlugin::unload()
{
  if(!isLoaded())
    return FALSE;

  QWidgetStack *stack = quantaApp->widgetStackOfHtmlPart();
  stack->removeWidget(m_part->widget());
  quantaApp->guiFactory()->removeClient(m_part);
  delete m_part;
  m_part = 0;

  stack->raiseWidget(0);

  setRunning(FALSE);

  emit pluginStopped();

  return TRUE;
}

