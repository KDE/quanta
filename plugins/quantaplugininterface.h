/***************************************************************************
                          quantaplugininterface.h  -  description
                             -------------------
    begin                : Mon Sep 16 2002
    copyright            : (C) 2002 by Marc Britton <consume@optushome.com.au>
                           (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUANTAPLUGININTERFACE_H
#define QUANTAPLUGININTERFACE_H

/* KDE INCLUDES */
#include <kconfig.h>
#include <kstandarddirs.h>

/* QT INCLUDES */
#include <qobject.h>
#include <qdict.h>
#include <qstringlist.h>

/* OTHER INCLUDES */
#include "quantaplugin.h"


/**Provides an interface to the installed plugins
  *@author Marc Britton
  */
class QuantaPluginInterface : public QObject
{
  Q_OBJECT
public:
  QuantaPluginInterface();
  ~QuantaPluginInterface();
  /** Reads the rc file */
  virtual void readConfig();
  /** Write the rc file */
  virtual void writeConfig();
  /* Returns TRUE if the plugin specified by a_name is available for us*/
  bool pluginAvailable(const QString &);
  /** Gets the plugins */
  QDict<QuantaPlugin> plugins();
  /** Sets the plugins */
  void setPlugins(QDict<QuantaPlugin>);
  /** Gets the plugin specified by a_name */
  virtual QuantaPlugin *plugin(const QString &);
  /** Gets the plugin names */
  virtual QStringList pluginNames() const;
  /** Gets the search paths */
  virtual QStringList searchPaths();
  /** Sets the search paths */
  virtual void setSearchPaths(QStringList);
protected:
  void readConfigFile(const QString& configFile);

  QDict<QuantaPlugin> m_plugins;
};

#endif
