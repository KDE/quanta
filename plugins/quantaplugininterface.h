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
#include <qpopupmenu.h>

/* OTHER INCLUDES */

class QuantaPlugin;

/**Provides an interface to the installed plugins
  *@author Marc Britton
  */
class QuantaPluginInterface : public QObject
{
  Q_OBJECT
public:
  /**
   *  since this class is a singleton you must use this function to access it
   *
   *  the parameters are only used at the first call to create the class
   *
   */
  static QuantaPluginInterface* const ref(QWidget *parent = 0L)
  {
    static QuantaPluginInterface *m_ref;
    if (!m_ref) m_ref = new QuantaPluginInterface (parent);
    return m_ref;
  }

  ~QuantaPluginInterface();
  /** Reads the rc file */
  virtual void readConfig();
  /** Write the rc file */
  virtual void writeConfig();
  /* Returns TRUE if the plugin specified by a_name is available for us*/
  bool pluginAvailable(const QString &);
  /** Gets the plugins */
  QDict<QuantaPlugin> plugins() {return m_plugins;};
  /** Sets the plugins */
  void setPlugins(QDict<QuantaPlugin> plugins) {m_plugins = plugins;};
  /** Gets the plugin specified by a_name */
  virtual QuantaPlugin *plugin(const QString &);
  /** Gets the plugin menu */
  virtual QPopupMenu *pluginMenu() {return m_pluginMenu;};
  void setPluginMenu(QPopupMenu *pluginMenu) {m_pluginMenu = pluginMenu;}
  /** Builds the plugins menu dynamically */
  void buildPluginMenu();

private:
  /** The constructor is privat because we use singleton patter.
   *  If you need the class use QuantaPluginInterface::ref() for
   *  construction and reference
   */
  QuantaPluginInterface(QWidget *parent);

protected slots:
  /** slot for the menu: validate */
  void slotPluginsValidate();
  /** slot for the menu: edit */
  void slotPluginsEdit();

protected:
  /** Gets the plugin names */
  virtual QStringList pluginNames() const;
  void readConfigFile(const QString& configFile);

  QDict<QuantaPlugin> m_plugins;

  QWidget *m_parent;

  QPopupMenu *m_pluginMenu;

signals:
  void hideSplash();
  void statusMsg(const QString &);
};

#endif
