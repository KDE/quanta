/***************************************************************************
                          quantaplugineditor.h  -  description
                             -------------------
    begin                : Fri Sep 20 2002
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

#ifndef QUANTAPLUGINEDITOR_H
#define QUANTAPLUGINEDITOR_H

/* KDE INCLUDES */
/* QT INCLUDES */
#include <qdict.h>
#include <qlistview.h>

/* OTHER INCLUDES */
#include "quantaplugin.h"
#include "quantakpartplugin.h"
#include "quantacmdplugin.h"
#include "plugineditor.h"

/**Editor for the plugins that manages configuration
  *@author Marc Britton
  */

/* FORWARD DECLARATIONS */
class QWidget;

class QuantaPluginEditor : public PluginEditor
{
  Q_OBJECT
public: 
  QuantaPluginEditor(QWidget *, const char *);
  ~QuantaPluginEditor();
  QDict<QuantaPlugin> plugins();
  /** No descriptions */
  QStringList searchPathList();
  /** No descriptions */
  void setSearchPaths(const QStringList& paths);
signals:
  void pluginsChanged();
public slots:
  void setPlugins(QDict<QuantaPlugin>);
  void addSearchPath();
  void addPlugin();
  void removePlugin();
  void configurePlugin();
  void refreshPlugins();
protected slots:
  void accept();
protected:
  QDict<QuantaPlugin> m_plugins;
};

class PluginEditorItem : public QListViewItem
{
public:
  PluginEditorItem(QListView *);
  virtual void paintCell(QPainter *, const QColorGroup &, int, int, int);
};

#endif
