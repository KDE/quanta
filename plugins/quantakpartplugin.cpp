/***************************************************************************
                          quantakpartplugin.cpp  -  description
                             -------------------
    begin                : Tue Sep 17 2002
    copyright            : (C) 2002 by Marc Britton
    email                : consume@optushome.com.au
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
#include <klocale.h>
#include <kstandarddirs.h>

/* QT INCLUDES */
#include <qwidget.h>
#include <qwidgetstack.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qobject.h>
#include <qfileinfo.h>

/* OTHER INCLUDES */
#include "quantakpartplugin.h"
#include "quantaplugininterface.h"
#include "quantaplugin.h"
#include "../quanta.h"
#include "../quantacommon.h"
#include "../resource.h"

QuantaKPartPlugin::QuantaKPartPlugin()
  : QuantaPlugin(), m_part(0)
{
}

QuantaKPartPlugin::~QuantaKPartPlugin()
{
}

bool QuantaKPartPlugin::isLoaded() 
{
  return m_part != 0;
}

bool QuantaKPartPlugin::load()
{

  if(isLoaded())
    return FALSE;
    
  QString loc = location();
  if(loc.isEmpty())
  {
    KStandardDirs *dirs = QuantaCommon::pluginDirs("lib");
    loc = dirs->findResource("lib", fileName());
    delete dirs;
  }
  else
  {
    loc += QString("/") + fileName();
  }

  QString ow = outputWindow();
  if(ow == i18n("Editor View"))
  {
    QWidgetStack *stack = quantaApp->widgetStackOfHtmlPart();

    if(loc.isEmpty())
    {
      qWarning("QuantaKPartPlugin::run - null location");
      return FALSE;
    }

    QFileInfo partInfo(loc);
    m_part = KParts::ComponentFactory::createPartInstanceFromLibrary<KParts::ReadOnlyPart>(partInfo.baseName().latin1(), stack, 0, stack, 0, QStringList(arguments()));
    if(!m_part)
    {
      qWarning("Failed to create KPart");
      return FALSE;
    }

    stack->addWidget(m_part->widget());
  }
  else
  {
    qWarning("QuantaKPartPlugin::load - Unknown output window %s", ow.latin1());
    return FALSE;
  }
  return TRUE;
}

bool QuantaKPartPlugin::run()
{
  if(!isLoaded())
    load();

  if(isLoaded())
  {
    quantaApp->guiFactory()->addClient(m_part);
    QWidgetStack *stack = quantaApp->widgetStackOfHtmlPart();
    stack->raiseWidget(m_part->widget());
    m_part->widget()->show();
    setRunning(TRUE);
    return TRUE;
  }
  return FALSE;
}

bool QuantaKPartPlugin::unload()
{
  if(!isLoaded())
    return FALSE;

  quantaApp->guiFactory()->removeClient(m_part);
  QWidgetStack *stack = quantaApp->widgetStackOfHtmlPart();
  stack->removeWidget(m_part->widget());
  delete m_part;
  m_part = 0;

  stack->raiseWidget(0);

  setRunning(FALSE);
  
  return TRUE;
}
