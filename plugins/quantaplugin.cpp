/***************************************************************************
               quantaplugin.cpp  -  Base class for quanta plugins
                             -------------------
    begin                : Mon Sep 16 2002
    copyright            : (C) 2002 by Marc Britton <consume@optushome.com.au>
                           (C) 2003-2004 by Andras Mantia <amantia@kde.org>
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
#include <kstandarddirs.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kparts/componentfactory.h>
#include <kparts/part.h>
#include <klocale.h>
#include <kdockwidget.h>
#include <kdebug.h>
#include <kmessagebox.h>

/* QT INCLUDES */
#include <qstring.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qwidget.h>
#include <qwidgetstack.h>
#include <qstringlist.h>
#include <qobject.h>

/* OTHER INCLUDES */
#include "quantaplugin.h"
#include "resource.h"
#include "quanta.h"
#include "quantaview.h"
#include "document.h"
#include "quantacommon.h"
#include "resource.h"
#include "messageoutput.h"
#include "project.h"

#include "viewmanager.h"


QuantaPlugin::QuantaPlugin()
 : m_isRunning(false)
{
 m_readOnlyPart = true;
 m_action = 0L;
 m_icon = "";
 m_part = 0L;
 m_view = 0L;
 m_pluginToolView = 0L;
 m_targetWidget = 0L;
}

QuantaPlugin::~QuantaPlugin()
{
   unload();
}

/** Sets the superficial name of the plugin */
void QuantaPlugin::setPluginName(const QString &a_name)
{
  m_name = a_name;
  if (!m_action)
  {
    m_action = new KToggleAction(i18n(a_name.utf8()), 0, this, SLOT(toggle()), quantaApp->actionCollection(), a_name);
  }
  m_action->setText(a_name);
}

QString QuantaPlugin::pluginName() const
{
  return m_name;
}

void QuantaPlugin::setLocation(const QString &a_location)
{
  m_location = a_location;
}

QString QuantaPlugin::location() const
{
  return m_location;
}

bool QuantaPlugin::load()
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
  m_targetWidget =  new QWidget(quantaApp);
  if (m_readOnlyPart)
      m_part = KParts::ComponentFactory::createPartInstanceFromLibrary<KParts::ReadOnlyPart>(partInfo.baseName().latin1(), m_targetWidget, 0, m_targetWidget, 0 );
  else
      m_part = KParts::ComponentFactory::createPartInstanceFromLibrary<KParts::ReadWritePart>(partInfo.baseName().latin1(), m_targetWidget, 0, m_targetWidget, 0 );
  if(!m_part)
  {
    KMessageBox::error(quantaApp, i18n("<qt>The <b>%1</b> plugin could not be loaded.<br>Possible reasons are:<br>    - <b>%2</b> is not installed;<br>    - the file <i>%3</i> is not installed or it is not reachable.").arg(m_name).arg(m_name).arg(m_fileName));
    delete m_targetWidget;
    m_targetWidget = 0L;
    return false;
  }
  m_part->setName(m_name);  // for better debug output
  m_part->widget()->setName(m_name); //needed to be able to dock the same plugin twice in separate toolviews
  m_part->widget()->setFocusPolicy(QWidget::ClickFocus);
  quantaApp->slotNewPart(m_part, false);  // register the part in the partmanager
  return true;
}

bool QuantaPlugin::run()
{
  if(!isLoaded())
    load();

  if(isLoaded())
  {
    bool result = true;
    QuantaView *view = ViewManager::ref()->activeView();
    if (view->document())
    {
      switch (m_input)
      {
        case 1: { result = m_part->openURL(view->document()->url());
                  break;
                }
        case 2: { KURL url = view->document()->url();
                  url.setPath(url.directory());
                  url.adjustPath(1);
                  result = m_part->openURL(url);
                  break;
                }
        case 3: { KURL url;
                  if ( Project::ref()->hasProject() &&
                       (Project::ref()->contains(view->document()->url()) ||
                        view->document()->isUntitled())
                     )
                  {
                    url = Project::ref()->projectBaseURL();
                  }
                  else
                  {
                    url = view->document()->url();
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
      if (m_input == 3 && (Project::ref()->hasProject())) //open project dir
          result = m_part->openURL(Project::ref()->projectBaseURL());
    }
    if (!result)
    {
      unload(false);
      return false;
    }
    addWidget();
    setRunning(true);
    connect( m_part, SIGNAL(setStatusBarText(const QString &)),
           quantaApp, SLOT(slotStatusMsg( const QString & )));


    emit pluginStarted();

    return true;
  }
  return false;
}

bool QuantaPlugin::toggle()
{
  if (isLoaded())
  {
    return unload();
  } else
  {
    bool result = run();
    if (!result)
    {
      m_action->blockSignals(true);
      m_action->setChecked(false);
      m_action->blockSignals(false);
    }
    return result;
  }
}

/** Returns the file name of the plugin */
QString QuantaPlugin::fileName() const
{
  return m_fileName;
}

/** Sets the file name of the plugin */
void QuantaPlugin::setFileName(const QString &a_fileName)
{
  m_fileName = a_fileName;
}

/** Sets the output window */
void QuantaPlugin::setOutputWindow(const QString &a_outputWindow)
{
  m_outputWindow = a_outputWindow;
}

/** Gets the output window */
QString QuantaPlugin::outputWindow() const
{
  return m_outputWindow;
}

bool QuantaPlugin::validatePlugin(QuantaPlugin *a_plugin)
{
  if(a_plugin)
  {
    return QuantaPlugin::validatePluginInfo(a_plugin->pluginName(),
                         a_plugin->location(), a_plugin->fileName(),
                         a_plugin->outputWindow());
  }
  return false;
}

/** Returns true if the information of a plugin is valid, otherwise false */
bool QuantaPlugin::validatePluginInfo(const QString & /*a_name*/,
    const QString &a_location, const QString &a_fileName, const QString &/*a_outputWindow*/)
{
  bool valid = true;
  // look the file up if location is null
  if(!a_location.isEmpty())
  {
    QString path = a_location + QString("/") + a_fileName;

    QFileInfo pluginFile(path);
    if(!pluginFile.exists())
      valid = false;
      /* TODO : What other tests should we perform? Permissions etc? */
  }
  else
  {
    if (valid)
    {
      KStandardDirs *dirs = QuantaCommon::pluginDirs("lib");
      if(dirs->findResource("lib", a_fileName).isNull())
        valid = false;
      delete dirs;
    }
  }
  return valid; // past all tests, valid
}

/** Returns whether the plugin is loaded or not */
bool QuantaPlugin::isLoaded()
{
  return m_targetWidget != 0L;
}

bool QuantaPlugin::unload(bool remove)
{
  if(!isLoaded())
    return false;

  disconnect( m_part, SIGNAL(setStatusBarText(const QString &)),
           quantaApp, SLOT(slotStatusMsg( const QString & )));

  delete (KParts::ReadOnlyPart*) m_part;
  m_part = 0;
  delete m_targetWidget;
  m_targetWidget = 0L;
  setRunning(false);

  emit pluginStopped();

  m_action->setChecked(false);
  if (remove)
    removeWidget();
  return true;
}

bool QuantaPlugin::isRunning() const
{
  return m_isRunning;
}

void QuantaPlugin::setRunning(bool a_enable)
{
  m_isRunning = a_enable;
}

/** Sets the plugin's standard attribute */
void QuantaPlugin::setStandard(bool isStandard)
{
  m_standard = isStandard;
}

/** Gets the plugin's standard attribute */
bool QuantaPlugin::isStandard() const
{
  return m_standard;
}

/** No descriptions */
QString QuantaPlugin::standardName()
{
 return m_standardName;
}
/** No descriptions */
void QuantaPlugin::setStandardName(const QString& a_stdName)
{
 m_standardName = a_stdName;
}
/** No descriptions */
QString QuantaPlugin::icon()
{
  return m_icon;
}
/** No descriptions */
void QuantaPlugin::setIcon(const QString& a_icon)
{
  m_icon = a_icon;
  if (m_action) m_action->setIcon(a_icon);
}

void QuantaPlugin::addWidget()
{
  QString ow = outputWindow();
  if(ow == i18n("Separate Toolview"))
  {
    m_part->widget()->setCaption(m_name);
    m_part->widget()->setIcon(SmallIcon(m_icon));
    m_pluginToolView = quantaApp->addToolWindow(m_part->widget(), quantaApp->prevDockPosition(m_part->widget(), KDockWidget::DockBottom), quantaApp->getMainDockWidget());
    m_pluginToolView->show();
  } else
  {
    m_view = ViewManager::ref()->createView();
    m_view->addPlugin(this);
  }
}

void QuantaPlugin::removeWidget()
{
  QString ow = outputWindow();
  if(ow == i18n("Separate Toolview"))
  {
       delete m_pluginToolView;
       m_pluginToolView = 0L;
       ViewManager::ref()->activeView()->setFocus();
  } else
  {
    ViewManager::ref()->removeView(m_view, true);
  }
}

QWidget *QuantaPlugin::widget()
{
  if (isLoaded()) {
    return m_part->widget();
  } else {
    return NULL;
  }
}

void QuantaPlugin::plugAction(QPopupMenu *menu)
{
  m_action->plug(menu);
}

#include "quantaplugin.moc"
