/***************************************************************************
                          quantapluginconfig.h  -  description
                             -------------------
    begin                : Sat Sep 21 2002
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

#ifndef QUANTAPLUGINCONFIG_H
#define QUANTAPLUGINCONFIG_H

class PluginConfig;
class KDialogBase;

/**Dialog to configure a plugin's options
  *@author Marc Britton
  */

class QString;

class QuantaPluginConfig : public KDialogBase
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

protected:
  PluginConfig *m_pluginConfigWidget;
};

#endif
