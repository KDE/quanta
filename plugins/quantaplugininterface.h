/***************************************************************************
                          quantaplugininterface.h  -  description
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

class QuantaApp;

/**Provides an interface to the installed plugins
  *@author Marc Britton
  */
class QuantaPluginInterface : public QObject
{
  Q_OBJECT
public: 
	QuantaPluginInterface(QuantaApp *);
	~QuantaPluginInterface();
  /** Returns a list of the known types */
  static QStringList pluginTypes();
  /** Returns the list of the available output windows */
  static QStringList outputWindows(const QString &);
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
#if 0
  /** Sets the plugin names */
  virtual void setPluginNames(QStringList);
#endif
  /** Gets the plugin specified by a_name */
  virtual QuantaPlugin *plugin(const QString &);
  /** Gets the plugin names */
  virtual QStringList pluginNames() const;
  /** Gets the search paths */
  virtual QStringList searchPaths();
  /** Sets the search paths */
  virtual void setSearchPaths(QStringList);
  /** Sets the app pointer, to pass to plugins */
  void setApp(QuantaApp *);
protected:
  QuantaApp *m_app;    
  QDict<QuantaPlugin> m_plugins;
  QStringList m_pluginNames;
};

#endif
