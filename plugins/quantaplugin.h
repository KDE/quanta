/***************************************************************************
                          quantaplugin.h  -  description
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

#ifndef QUANTAPLUGIN_H
#define QUANTAPLUGIN_H

/* KDE INCLUDES */

/* QT INCLUDES */
#include <qobject.h>
#include <qstring.h>


/**Base class for quanta plugins
  *@author Marc Britton
  */

class KToggleAction;

class QuantaPlugin : public QObject
{
  Q_OBJECT
public: 
  QuantaPlugin();
  QuantaPlugin(const QString &, const QString &, const QString &, const QString &, const QString &, const QString &, const QString &);
  ~QuantaPlugin();
  virtual bool isRunning() const;
  /** Gets the superficial plugin name */
  virtual QString pluginName() const;
  /** Gets the location of the plugin */
  virtual QString location() const;
  /** Gets the plugin's type */
  virtual QString type() const;
  /** Gets the plugin's standard attribute */
  virtual bool isStandard() const;
  /** Gets the program's arguments */
  virtual QString arguments() const;
  /** Gets the output window */
  virtual QString outputWindow() const;
  /** Returns true if the plugin specified by a_plugin is valid, false otherwise */
  static bool validatePlugin(QuantaPlugin *);
  /** Returns true if the options of a plugin are valid, otherwise false */
  static bool validatePluginInfo(const QString &, const QString &, const QString &,
              const QString &, const QString &, const QString &);

  KToggleAction *m_action;
public slots:
  /** Sets whether the plugin is running or not */
  virtual void setRunning(bool);
  /** Loads the plugin */
  virtual bool load();
  /** Unloads te plugin */
  virtual bool unload();
  /** Runs the plugin */
  virtual bool run();
  /** Runs the plugin if it's not running yet, unloads otherwise.*/
  virtual bool toggle();
  /** Sets the superficial name of the plugin */
  virtual void setPluginName(const QString &);
  /** Sets the output window */
  virtual void setOutputWindow(const QString &);
  /** Sets the plugin's arguments */
  virtual void setArguments(const QString &);
  /** Sets the plugin's type */
  virtual void setType(const QString &);
  /** Sets the plugin's standard attribute */
  virtual void setStandard(bool isStandard);
  /** Sets the location of the plugin */
  virtual void setLocation(const QString &);
  /** Sets the file name of the plugin */
  virtual void setFileName(const QString &);
  /** Returns the file name of the plugin */
  virtual QString fileName() const;
  /** Returns whether the plugin is loaded or not */
  virtual bool isLoaded();
  /** No descriptions */
  void setIcon(const QString& a_icon);
  /** No descriptions */
  QString icon();
  /** No descriptions */
  void setStandardName(const QString& a_stdName);
  /** No descriptions */
  QString standardName();
signals:
  void pluginStarted();
  void pluginStopped();
protected:
  /** Superficial name of plugin */
  QString m_fileName;
  QString m_name;
  QString m_location;
  QString m_type;
  QString m_icon;
  QString m_arguments;
  QString m_outputWindow;
  /** True, if it is not a user added plugin. Special handling code may be necessary for standard plugins. */
  bool m_standard;
  bool m_isRunning;
private: // Private attributes
  /**  */
  QString m_standardName;
};

#endif
