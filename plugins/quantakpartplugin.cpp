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
#include <kmessagebox.h>

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
#include "../document.h"
#include "../quantacommon.h"
#include "../resource.h"
#include "../messages/messageoutput.h"
#include "../project/project.h"

QuantaKPartPlugin::QuantaKPartPlugin()
  : QuantaPlugin(), m_part(0)
{
  guiVisible = false;
  m_readOnlyPart = true;
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
    KMessageBox::error(quantaApp, i18n("Invalid output window setting!"));
    return false;
  }
  if (m_readOnlyPart)
    m_part = KParts::ComponentFactory::createPartInstanceFromLibrary<KParts::ReadOnlyPart>(partInfo.baseName().latin1(), targetWidget, 0, targetWidget, 0, QStringList(arguments()));
  else
    m_part = KParts::ComponentFactory::createPartInstanceFromLibrary<KParts::ReadWritePart>(partInfo.baseName().latin1(), targetWidget, 0, targetWidget, 0, QStringList(arguments()));
  if(!m_part)
  {
    KMessageBox::error(quantaApp, i18n("<qt>The <b>%1</b> plugin could not be loaded!<br>Possible reasons are:<br>    - <b>%2</b> is not installed;<br>    - the file <i>%3</i> is not installed or it is not reachable.").arg(m_name).arg(m_name).arg(m_fileName));
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
    bool result = true;
    if (quantaApp->view()->writeExists())
    {
      switch (m_input)
      {
        case 1: { result = m_part->openURL(quantaApp->view()->write()->url());
                  break;
                }
        case 2: { KURL url = quantaApp->view()->write()->url();
                  url.setPath(url.directory());
                  url.adjustPath(1);
                  result = m_part->openURL(url);
                  break;
                }
        case 3: { KURL url;
                  if (quantaApp->project()->hasProject() &&
                      quantaApp->project()->contains(quantaApp->view()->write()->url()))
                      url = quantaApp->projectBaseURL();
                  else
                  {
                    url = quantaApp->view()->write()->url();
                    url.setPath(url.directory());
                    url.adjustPath(1);
                  }
                  result = m_part->openURL(url);
                  break;
                }
        default: ;
      }
    } else
    {
      if (m_input == 3 && quantaApp->project()->hasProject()) //open project dir
          result = m_part->openURL(quantaApp->projectBaseURL());
    }
    if (!result)
    {
      unload();
      return false;
    }
    showGui(true);
    addWidget();
    setRunning(true);
    connect( m_part, SIGNAL(setStatusBarText(const QString &)),
           quantaApp, SLOT(slotStatusMsg( const QString & )));


    emit pluginStarted();

    return true;
  }
  return false;
}

bool QuantaKPartPlugin::unload()
{
  if(!isLoaded())
    return FALSE;

  disconnect( m_part, SIGNAL(setStatusBarText(const QString &)),
           quantaApp, SLOT(slotStatusMsg( const QString & )));

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
    if (!guiVisible)
       quantaApp->guiFactory()->addClient(m_part);
  } else
  {
    quantaApp->guiFactory()->removeClient(m_part);
  }
  guiVisible = show;
}

#include "quantakpartplugin.moc"
