/***************************************************************************
                          quantapluginconfig.h  -  description
                             -------------------
    begin                : Sat Sep 21 2002
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

#ifndef QUANTAPLUGINCONFIG_H
#define QUANTAPLUGINCONFIG_H

#include "pluginconfig.h"

/**Dialog to configure a plugin's options
  *@author Marc Britton
  */

class QString;

class QuantaPluginConfig : public PluginConfig
{
  Q_OBJECT
public:
  QuantaPluginConfig(QWidget *, const char *);
  ~QuantaPluginConfig();
public slots:
  /** Validates the dialog before accepting */
  void accept();
  /** Gets the plugin location */
  void selectLocation();
  /** Updates other widgets based on the name */
  void nameChanged(const QString &);
  /** Updates the windows combobox based on the type */
  void updateWindows(const QString &);
};

#endif
