/***************************************************************************
               quantaplugin.cpp  -  Base class for quanta plugins
                             -------------------
    begin                : Mon Sep 16 2002
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
#include <kstandarddirs.h>

/* QT INCLUDES */
#include <qstring.h>
#include <qfile.h>
#include <qfileinfo.h>

/* OTHER INCLUDES */
#include "quantaplugin.h"
#include "../resource.h"

QuantaPlugin::QuantaPlugin()
 : m_isRunning(false)
{
}

QuantaPlugin::QuantaPlugin(const QString &a_name, const QString &a_type,
  const QString &a_location, const QString &a_fileName, const QString &a_arguments,
    const QString &a_outputWindow)
{
  setPluginName(a_name);
  setType(a_type);
  setLocation(a_location);
  setFileName(a_fileName);  
  setArguments(a_arguments);
  setOutputWindow(a_outputWindow);  
  m_standard = false;
  load();
}

QuantaPlugin::~QuantaPlugin()
{
}

/** Sets the superficial name of the plugin */
void QuantaPlugin::setPluginName(const QString &a_name)
{
  m_name = a_name;
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
		return FALSE;
}
  
bool QuantaPlugin::run()
{
		return FALSE;
}
  
/** Sets the plugin's type */
void QuantaPlugin::setType(const QString &a_type)
{
  m_type = a_type;
}

/** Gets the plugin's type */
QString QuantaPlugin::type() const
{
  return m_type;  
}

void QuantaPlugin::setArguments(const QString &a_arguments)
{
  m_arguments = a_arguments;
}

QString QuantaPlugin::arguments() const
{
  return m_arguments;
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
    return QuantaPlugin::validatePluginInfo(a_plugin->pluginName(), a_plugin->type(),
                         a_plugin->location(), a_plugin->fileName(), a_plugin->arguments(),
                         a_plugin->outputWindow());
  }
  return FALSE;
}

/** Returns true if the information of a plugin is valid, otherwise false */
bool QuantaPlugin::validatePluginInfo(const QString &a_name, const QString &a_type,
    const QString &a_location, const QString &a_fileName, const QString &a_arguments, const QString &a_outputWindow)
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
    // lookup
    QString lookupType;
    if(a_type == "KPart")
      lookupType = "lib";
    else if(a_type == "Command Line")
      lookupType = "exe";
    else
    {
      qWarning("QuantaPlugin::validatePluginInfo - Tried to lookup invalid plugin type \'%s\'", a_type.latin1());
      valid = false;  
    }

    if (valid)
    {
      KStandardDirs *dirs = QuantaCommon::pluginDirs(lookupType);
      if(dirs->findResource(lookupType, a_fileName) == QString::null)
        valid = false;
      delete dirs;         
    }
  }
  return valid; // past all tests, valid
}

/** Returns whether the plugin is loaded or not */
bool QuantaPlugin::isLoaded() const
{
  return FALSE;
}

bool QuantaPlugin::unload()
{
  return FALSE;
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
