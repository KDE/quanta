/***************************************************************************
                          quantaplugin.h  -  description
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

#ifndef QUANTAPLUGIN_H
#define QUANTAPLUGIN_H

/* KDE INCLUDES */
#include <kparts/part.h>

/* QT INCLUDES */
#include <qguardedptr.h>
#include <qobject.h>

class QString;

/**Base class for quanta plugins
  *@author Marc Britton
  */

class KToggleAction;
class KMdiToolViewAccessor;

class QuantaView;

class QuantaPlugin : public QObject
{
  Q_OBJECT
public:
  QuantaPlugin();
  ~QuantaPlugin();
  virtual bool isRunning() const;
  /** Gets the superficial plugin name */
  virtual QString pluginName() const;
  /** Gets the location of the plugin */
  virtual QString location() const;
  /** Gets the plugin's standard attribute */
  virtual bool isStandard() const;
  /** Gets the output window */
  virtual QString outputWindow() const;
  /** Returns true if the plugin specified by a_plugin is valid, false otherwise */
  static bool validatePlugin(QuantaPlugin *);
  /** Returns true if the options of a plugin are valid, otherwise false */
  static bool validatePluginInfo(const QString &, const QString &,
              const QString &, const QString &);

  virtual void addWidget();
  virtual void removeWidget();
  QWidget *widget();
  KParts::ReadOnlyPart *part() {return m_part;}
  QString name() {return m_name;}
  /** plugs action into menu */
  void plugAction(QPopupMenu *menu);
public slots:
  /** Sets whether the plugin is running or not */
  virtual void setRunning(bool);
  /** Loads the plugin */
  virtual bool load();
  /** Unloads te plugin. If remove is true, it will also remove the plugin widget. */
  virtual bool unload(bool remove = true);
  /** Runs the plugin */
  virtual bool run();
  /** Runs the plugin if it's not running yet, unloads otherwise.*/
  virtual bool toggle();
  /** Sets the superficial name of the plugin */
  virtual void setPluginName(const QString &);
  /** Sets the output window */
  virtual void setOutputWindow(const QString &);
  /** Sets the plugin's standard attribute */
  virtual void setStandard(bool isStandard);
  /** Sets the location of the plugin */
  virtual void setLocation(const QString &);
  /** Sets the file name of the plugin */
  virtual void setFileName(const QString &);
  /** Returns the file name of the plugin */
  virtual QString fileName() const;
  virtual int input() {return m_input;}
  virtual void setInput(int a_input) { m_input = a_input;}
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
  void setReadOnlyPart(bool a_readOnlyPart) {m_readOnlyPart = a_readOnlyPart;}
  bool readOnlyPart() {return m_readOnlyPart;}
signals:
  void pluginStarted();
  void pluginStopped();
protected:
  /** Superficial name of plugin */
  QString m_fileName;
  QString m_name;
  QString m_location;
  QString m_icon;
  QString m_outputWindow;
  int m_input;
  /** True, if it is not a user added plugin. Special handling code may be necessary for standard plugins. */
  bool m_standard;
  bool m_isRunning;
  QGuardedPtr<KParts::ReadOnlyPart> m_part;
  bool m_readOnlyPart;
  QuantaView *m_view;
  KToggleAction *m_action;
private: // Private attributes
  QString m_standardName;
  KMdiToolViewAccessor* m_pluginToolView;
  QWidget *m_targetWidget; //this will be the parent, used only to make the plugin widget hidden until addWidget is called
};

#endif
