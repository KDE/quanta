/***************************************************************************
    quantaplugininterface.cpp  - General interface to the plugin system
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
#include <kconfig.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>

/* QT INCLUDES */
#include <qdict.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qapplication.h>
#include <qfileinfo.h>

/* OTHER INCLUDES */
#include "quantaplugininterface.h"
#include "quantakpartplugin.h"
#include "quantacmdplugin.h"
#include "cervisiaplugin.h"
#include "../resource.h"
#include "../quanta.h"

QuantaPluginInterface::QuantaPluginInterface()
{
 // m_plugins.setAutoDelete(TRUE);

  readConfig();
}

QuantaPluginInterface::~QuantaPluginInterface()
{
  m_plugins.clear();
  // TODO : free plugins
}

/** Reads the plugin settings from the rc file */
void QuantaPluginInterface::readConfig()
{
  m_plugins.clear();

  // read the plugins.rc
  QString configFile = locateLocal("appdata", "plugins.rc");
  if (!QFileInfo(configFile).exists()) configFile = qConfig.globalDataDir +"quanta/plugins.rc";

  KConfig *config = new KConfig(configFile);
  config->setGroup("General");
  QStringList pList = config->readListEntry("Plugins");
  for(QStringList::Iterator it = pList.begin();it != pList.end(); ++it)
    (*it) = (*it).stripWhiteSpace();
  //setPluginNames(pList);

  QStringList paths = config->readListEntry("SearchPaths");
  for(QStringList::Iterator it = paths.begin();it != paths.end(); ++it)
    (*it) = (*it).stripWhiteSpace();
  setSearchPaths(paths);

  // now that we have a list of the plugins, go through and get the details of them
  for(QStringList::Iterator it = pList.begin();it != pList.end();++it)
  {
    config->setGroup(*it);

    QuantaPlugin *newPlugin = 0;
    QString pluginType = config->readEntry("Type", "Command Line");
    if (pluginType == "KPart") pluginType = i18n("KPart");
    if (pluginType == "Command Line") pluginType = i18n("Command Line");
    bool isStandard = config->readBoolEntry("Standard",false);
    if (isStandard)
    {
      QString stdName = config->readEntry("Standard Name");
      if (stdName == "cervisia_kpart")
      {
        newPlugin = new CervisiaPlugin();
      }
      if (newPlugin)
      {
        newPlugin->setStandardName(stdName);
      }
    } else
    {
      if(pluginType == i18n("KPart"))
        newPlugin = new QuantaKPartPlugin();
      else if(pluginType == "Command Line")
        newPlugin = new QuantaCmdPlugin();
    }
    if (!newPlugin)
    {
      qWarning("Unknown plugin type: %s", pluginType.latin1());
      continue;
    }
    newPlugin->setStandard(isStandard);
    newPlugin->setPluginName(*it);
    newPlugin->setFileName(config->readEntry("FileName"));
    newPlugin->setLocation(config->readEntry("Location"));
    newPlugin->setType(pluginType);
    newPlugin->setArguments(config->readEntry("Arguments"));
    newPlugin->setIcon(config->readEntry("Icon"));
    QString type = config->readEntry("OutputWindow");
    if (type == "Editor Tab" || type == "Editor View") type = i18n("Editor Tab"); //TODO: Remove the check against Editor View after 3.2
    if (type == "Editor Frame") type = i18n("Editor Frame");
    if (type == "Message Frame" || type == "Output Dock") type = i18n("Message Frame");//TODO: Remove the check against Output Dock after 3.2
    if (type == "Message Window") type = i18n("Message Window");
    if (type == "Konsole") type = i18n("Konsole");
    newPlugin->setOutputWindow(type);

    m_plugins.insert(newPlugin->pluginName(), newPlugin);
  }
  delete config;
}

/** Writes the plugin settings to the rc file */
void QuantaPluginInterface::writeConfig()
{
  // write the plugin settings to the rc file
  KConfig *config = new KConfig(locateLocal("appdata", "plugins.rc"));

  QStringList names = pluginNames();

  config->setGroup("General");
  config->writeEntry("Plugins", names);
  config->writeEntry("SearchPaths", searchPaths());

  for(QStringList::Iterator it = names.begin();it != names.end(); ++it)
  {
    config->setGroup(*it);

    QuantaPlugin *curPlugin = m_plugins[*it];
    if(curPlugin)
    {
      config->writeEntry("FileName", curPlugin->fileName());
      QString type =  curPlugin->type();
      if (type == i18n("KPart")) type = "KPart";
      if (type == i18n("Command Line")) type = "Command Line";
      config->writeEntry("Type", type);
      config->writeEntry("Location", curPlugin->location());
      config->writeEntry("Arguments", curPlugin->arguments());
      config->writeEntry("Icon", curPlugin->icon());
      type = curPlugin->outputWindow();
      if (type == i18n("Editor Tab")) type = "Editor Tab";
      if (type == i18n("Editor Frame")) type = "Editor Frame";
      if (type == i18n("Message Frame")) type = "Message Frame";
      if (type == i18n("Message Window")) type = "Message Window";
      if (type == i18n("Konsole")) type = "Konsole";
      config->writeEntry("OutputWindow", type);
      config->writeEntry("Standard", curPlugin->isStandard());
      if (curPlugin->isStandard()) config->writeEntry("Standard Name", curPlugin->standardName());
    }
  }
  if (config->isReadOnly())
  {
    kdWarning() << "Plugin config file " << locateLocal("appdata", "plugins.rc") << " is read only! Plugin settings were not saved!" << endl;
  }
  config->sync();
  if (config->isReadOnly())
  {
    kdWarning() << "Plugin config file " << locateLocal("appdata", "plugins.rc") << " is read only! Plugin settings were not saved!" << endl;
  } else
  {
    kdDebug(24000) << "Plugin settings saved to: " << locateLocal("appdata", "plugins.rc") << endl;
  }
  delete config;
}

/** Gets the plugins */
QDict<QuantaPlugin> QuantaPluginInterface::plugins()
{
    return m_plugins;
}

/** Sets the plugins */
void QuantaPluginInterface::setPlugins(QDict<QuantaPlugin> a_plugins)
{
  m_plugins = a_plugins;
}

/** Returns TRUE if the plugin is available */
bool QuantaPluginInterface::pluginAvailable(const QString &a_name)
{
  QuantaPlugin *availPlugin = m_plugins.find(a_name);
  if(availPlugin && QuantaPlugin::validatePlugin(availPlugin))
    return TRUE;

  return FALSE;
}

#if 0
/** Sets the plugin names */
void QuantaPluginInterface::setPluginNames(QStringList a_pluginNames)
{
  m_pluginNames = a_pluginNames;
}
#endif

/**  Gets the plugin names */
QStringList QuantaPluginInterface::pluginNames() const
{
  QStringList names;
  QDictIterator<QuantaPlugin> it(m_plugins);
  for(;it.current();++it)
  {
    names << (*it)->pluginName();
  }
  return names;
}

/** Sets the search paths */
void QuantaPluginInterface::setSearchPaths(QStringList a_searchPaths)
{
  qConfig.pluginSearchPaths = a_searchPaths;
}

/** Gets the search paths */
QStringList QuantaPluginInterface::searchPaths()
{
  return qConfig.pluginSearchPaths;
}

QStringList QuantaPluginInterface::pluginTypes()
{
  QStringList types;
  types << i18n("KPart");
  types << i18n("Command Line");

  return types;
}

QStringList QuantaPluginInterface::outputWindows(const QString &a_type)
{
  QStringList windows;

  if(a_type == i18n("KPart"))
  {
    windows << i18n("Editor Tab") << i18n("Editor Frame") << i18n("Message Frame"); // TODO
  }
  else if(a_type == i18n("Command Line"))
  {
    windows << i18n("Message Window") << i18n("Konsole") << i18n("None");
  }
  else
    qWarning("QuantaPluginInterface::outputWindows - Unknown type %s", a_type.latin1());
  return windows;
}

/** Gets the plugin specified by a_name */
QuantaPlugin *QuantaPluginInterface::plugin(const QString &a_name)
{
  return m_plugins[a_name];
}

#include "quantaplugininterface.moc"
