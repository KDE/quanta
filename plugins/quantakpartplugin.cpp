/***************************************************************************
                          quantakpartplugin.cpp  -  description
                             -------------------
    begin                : Tue Sep 17 2002
    copyright            : (C) 2002 by Marc Britton
                           (C) 2003 by Andras Mantia
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
#include <kdockwidget.h>
#include <kdebug.h>

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
#include "../quantaview.h"
#include "../quantacommon.h"
#include "../resource.h"
#include "../messages/messageoutput.h"

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
    return false;

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

  if(loc.isEmpty())
  {
    kdWarning() << "QuantaKPartPlugin::run - null location\n";
    return false;
  }

  QFileInfo partInfo(loc);
  QString ow = outputWindow();
  QWidget *targetWidget;
  if(ow == i18n("Editor Tab"))
  {
    targetWidget = quantaApp->view();
  } else
  if(ow == i18n("Editor Frame"))
  {
    targetWidget = quantaApp->rightWidget();
  }
  else if(ow == i18n("Message Frame"))
  {
    targetWidget = quantaApp->bottomWidget();
  }
  else
  {
    kdWarning() << "QuantaKPartPlugin::load - Unknown output window:" << ow << "!\n";
    return false;
  }
  m_part = KParts::ComponentFactory::createPartInstanceFromLibrary<KParts::ReadOnlyPart>(partInfo.baseName().latin1(), targetWidget, 0, targetWidget, 0, QStringList(arguments()));
  if(!m_part)
  {
    kdDebug(24000) << "Failed to create KPart\n";
    return false;
  }
  return true;
}

bool QuantaKPartPlugin::run()
{
  if(!isLoaded())
    load();

  if(isLoaded())
  {
    showGui(true);
    addWidget();
    setRunning(true);

    emit pluginStarted();

    return true;
  }
  return false;
}

bool QuantaKPartPlugin::unload()
{
  if(!isLoaded())
    return FALSE;

  showGui(false);
  removeWidget();
  delete m_part;
  m_part = 0;
  setRunning(false);

  emit pluginStopped();

  return true;
}

void QuantaKPartPlugin::addWidget()
{
  QString ow = outputWindow();
  if(ow == i18n("Message Frame"))
  {
    QWidgetStack *stack = quantaApp->bottomWidget();
    stack->addWidget(m_part->widget(), 3);
    stack->raiseWidget(m_part->widget());
  }
  else if (ow == i18n("Editor Frame"))
  {
    QWidgetStack *stack = quantaApp->rightWidget();
    stack->addWidget(m_part->widget(), 3);
    stack->raiseWidget(m_part->widget());
  } else if (ow == i18n("Editor Tab"))
  {
    QuantaView *view = quantaApp->view();
    view->addWrite(m_part->widget(), m_name);
    view->writeTab()->showPage(m_part->widget());
  }

  m_part->widget()->show();
}

void QuantaKPartPlugin::removeWidget()
{
  QString ow = outputWindow();
  if(ow == i18n("Message Frame"))
  {
    QWidgetStack *stack = quantaApp->bottomWidget();
    stack->removeWidget(m_part->widget());
    stack->raiseWidget(0);
  }
  else if(ow == i18n("Editor Frame"))
  {
    QWidgetStack *stack = quantaApp->rightWidget();
    stack->removeWidget(m_part->widget());
    stack->raiseWidget(0);
  }
  else if(ow == i18n("Editor Tab"))
  {
    quantaApp->view()->writeTab()->removePage(m_part->widget());
  }
}

void QuantaKPartPlugin::showGui(bool show)
{
  if (show)
  {
    quantaApp->guiFactory()->addClient(m_part);
  } else
  {
    quantaApp->guiFactory()->removeClient(m_part);
  }
}

#include "quantakpartplugin.moc"
