/***************************************************************************
    quantaplugininterface.cpp  - General interface to the plugin system
                             -------------------
    begin                : Mon Sep 16 2002
    copyright            : (C) 2002 by Marc Britton
                           (C) 2003 by Andras Mantia <amantia@kde.org>
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
#include <kmessagebox.h>

/* QT INCLUDES */
#include <qdict.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qapplication.h>
#include <qfileinfo.h>

/* OTHER INCLUDES */
#include "quantaplugininterface.h"
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

void QuantaPluginInterface::readConfigFile(const QString& configFile)
{
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
    if (m_plugins.find(*it))
        continue;
    config->setGroup(*it);

    QuantaPlugin *newPlugin = 0;
    QString pluginType = config->readEntry("Type", "KPart");
    bool isStandard = config->readBoolEntry("Standard",false);
/*    if (isStandard)
    {
      QString stdName = config->readEntry("Standard Name");
      if (newPlugin)
      {
        newPlugin->setStandardName(stdName);
      }
    } else */
    {
      if (pluginType == "Command Line")
      {
        KMessageBox::information(quantaApp, i18n("<qt><b>%1</b> is a command line plugin. We have removed support for command line plugins. However, the functionality has not been lost as script actions can still be used to run commandline tools. </qt>").arg(*it), i18n("Unsupported plugin type"), "CommandLinePluginWarning");
        continue;
      }

      newPlugin = new QuantaPlugin();
    }
    newPlugin->setStandard(isStandard);
    newPlugin->setPluginName(*it);
    newPlugin->setFileName(config->readEntry("FileName"));
    newPlugin->setLocation(config->readEntry("Location"));
    newPlugin->setIcon(config->readEntry("Icon"));
    QString type = config->readEntry("OutputWindow");
    if (type == "Editor Tab" || type == "Editor View")
      type = i18n("Editor Tab"); //TODO: Remove the check against Editor View after 3.2
    if (type == "Editor Frame")
      type = i18n("Editor Frame");
    if (type == "Message Frame" || type == "Output Dock")
      type = i18n("Message Frame");//TODO: Remove the check against Output Dock after 3.2
    if (type == "Message Window")
      type = i18n("Message Window");
    if (type == "Konsole")
      type = i18n("Konsole");
    newPlugin->setOutputWindow(type);
    newPlugin->setInput(config->readNumEntry("Input", 0));
    newPlugin->setReadOnlyPart(config->readBoolEntry("ReadOnly", true));

    m_plugins.insert(*it, newPlugin);
  }
  delete config;
}

/** Reads the plugin settings from the rc file */
void QuantaPluginInterface::readConfig()
{
  m_plugins.clear();

  // read the local plugins.rc
  QString configFile = locateLocal("appdata", "plugins.rc");
  if (QFileInfo(configFile).exists())
      readConfigFile(configFile);
  // read the global plugins.rc
  configFile = qConfig.globalDataDir +"quanta/plugins.rc";
  readConfigFile(configFile);
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
      config->writeEntry("Type", "KPart"); //not used, but just to be compatible
      config->writeEntry("Location", curPlugin->location());
      config->writeEntry("Icon", curPlugin->icon());
      QString type = curPlugin->outputWindow();
      if (type == i18n("Editor Tab")) type = "Editor Tab";
      if (type == i18n("Editor Frame")) type = "Editor Frame";
      if (type == i18n("Message Frame")) type = "Message Frame";
      if (type == i18n("Message Window")) type = "Message Window";
      if (type == i18n("Konsole")) type = "Konsole";
      config->writeEntry("OutputWindow", type);
      config->writeEntry("Input", curPlugin->input());
      config->writeEntry("Standard", curPlugin->isStandard());
      if (curPlugin->isStandard()) config->writeEntry("Standard Name", curPlugin->standardName());
      config->writeEntry("ReadOnly", curPlugin->readOnlyPart());
    }
  }
  config->sync();
  if (config->isReadOnly())
  {
    kdWarning() << "Plugin config file " << locateLocal("appdata", "plugins.rc") << " is read only! Plugin settings were not saved!" << endl;
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

/** Gets the plugin specified by a_name */
QuantaPlugin *QuantaPluginInterface::plugin(const QString &a_name)
{
  return m_plugins[a_name];
}

#include "quantaplugininterface.moc"
