/***************************************************************************
    quantaplugininterface.cpp  - General interface to the plugin system
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
#include <kactioncollection.h>
#include <kconfig.h>
#include <kstandarddirs.h>
#include <kmainwindow.h>
#include <klocale.h>
#include <kdebug.h>
#include <kmessagebox.h>

/* QT INCLUDES */
#include <qdict.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qfileinfo.h>

/* OTHER INCLUDES */
#include "quantaplugininterface.h"
#include "quantaplugineditor.h"
#include "quantaplugin.h"

#include "resource.h"
#include "quantacommon.h"

QuantaPluginInterface::QuantaPluginInterface(QWidget *parent)
{
  m_parent = parent;
   (void) new KAction( i18n( "Configure &Plugins..." ), 0, 0,
                        this, SLOT( slotPluginsEdit() ),
                        ((KMainWindow*)parent)->actionCollection(), "configure_plugins" );
  m_pluginMenu = 0L;
  // m_plugins.setAutoDelete(true);
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

  QStringList paths = QuantaCommon::readPathListEntry(config, "SearchPaths");
  for(QStringList::Iterator it = paths.begin();it != paths.end(); ++it)
    (*it) = (*it).stripWhiteSpace();
  qConfig.pluginSearchPaths = paths;

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
        emit hideSplash();
        KMessageBox::information(m_parent, i18n("<qt><b>%1</b> is a command line plugin. We have removed support for command-line plugins. However, the functionality has not been lost as script actions can still be used to run command-line tools. </qt>").arg(*it), i18n("Unsupported Plugin Type"), "CommandLinePluginWarning");
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
    if (type == "Message Frame" || type == "Separate Toolview")
      type = i18n("Separate Toolview");
    else
      type = i18n("Editor Tab");
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
  configFile = qConfig.globalDataDir + resourceDir + "plugins.rc";
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
  config->writePathEntry("SearchPaths", qConfig.pluginSearchPaths);

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
      if (type == i18n("Separate Toolview")) type = "Separate Toolview";
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


/** Returns true if the plugin is available */
bool QuantaPluginInterface::pluginAvailable(const QString &a_name)
{
  if (a_name.isEmpty())
    return false;
  QuantaPlugin *availPlugin = m_plugins.find(a_name);
  if(availPlugin && QuantaPlugin::validatePlugin(availPlugin))
    return true;

  return false;
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


/** Gets the plugin specified by a_name */
QuantaPlugin *QuantaPluginInterface::plugin(const QString &a_name)
{
  if (a_name.isEmpty())
    return 0L;
  return m_plugins[a_name];
}

/** Builds the plugins menu */
void QuantaPluginInterface::buildPluginMenu()
{
  m_pluginMenu->clear();
//  m_pluginMenu->setCheckable(true);

  QDictIterator<QuantaPlugin> it(m_plugins);
  for(;it.current() != 0;++it)
  {
       QuantaPlugin *curPlugin = it.current();
       if(curPlugin)
       {
//         int id = m_pluginMenu->insertItem(curPlugin->pluginName());
//         if(curPlugin->isRunning())
//           m_pluginMenu->setItemChecked(id, true);
           curPlugin->plugAction(m_pluginMenu);
       }
  }
}

void QuantaPluginInterface::slotPluginsEdit()
{
  QuantaPluginEditor *editor = new QuantaPluginEditor(m_parent, "plugin_editor");
  editor->setSearchPaths(qConfig.pluginSearchPaths);
  editor->setPlugins(plugins());

  editor->exec();
  qConfig.pluginSearchPaths = editor->searchPathList();
  setPlugins(editor->plugins());
  writeConfig();
  buildPluginMenu();
  slotPluginsValidate();
}

void QuantaPluginInterface::slotPluginsValidate()
{
  QValueList<QuantaPlugin*> invalidPlugins;
  QDictIterator<QuantaPlugin> it(m_plugins);
  for(;it.current();++it)
  {
    if(!QuantaPlugin::validatePlugin(it.current()))
    {
       invalidPlugins.append(it.current());
    }
  }
  uint invalidCount = invalidPlugins.count();
  if (invalidCount > 0)
  {
     QString invalidNames;
     for (uint i=0; i < invalidCount; i++)
     {
         invalidNames += "<br>" + invalidPlugins[i]->name();
     }
     int answer = KMessageBox::questionYesNo(m_parent, i18n("<qt>The following plugins seems to be invalid:<b>%1</b>.<br><br>Do you want to edit the plugins?</qt>").arg(invalidNames), i18n("Invalid Plugins"), i18n("Edit Plugins"), i18n("Do Not Edit"));
      if(answer == KMessageBox::Yes)
      {
        slotPluginsEdit();
      }
      return;
  } else
     emit statusMsg(i18n("All plugins validated successfully."));
}


#include "quantaplugininterface.moc"
