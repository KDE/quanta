/***************************************************************************
                          quantaplugineditor.cpp  -  description
                             -------------------
    begin                : Fri Sep 20 2002
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
#include <kfiledialog.h>
#include <kiconloader.h>
#include <kglobal.h>
#include <klocale.h>
#include <kicondialog.h>

/* QT INCLUDES */
#include <qwidget.h>
#include <qobject.h>
#include <qdict.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qlistview.h>

/* OTHER INCLUDES */
#include "quantaplugineditor.h"
#include "quantaplugin.h"
#include "quantapluginconfig.h"
#include "quantaplugininterface.h"
#include "../quanta.h"
#include "../resource.h"

QuantaPluginEditor::QuantaPluginEditor(QWidget *a_parent, const char *a_name)
  : PluginEditor(a_parent, a_name)
{
 // m_plugins.setAutoDelete(TRUE);

  pluginList->setSelectionMode(QListView::Single);
  pluginList->setAllColumnsShowFocus(TRUE);
  pluginList->setColumnAlignment(2, Qt::AlignHCenter);

  connect(this, SIGNAL(pluginsChanged()), SLOT(refreshPlugins()));
  connect(refreshButton, SIGNAL(clicked()), SLOT(refreshPlugins()));
  connect(addButton, SIGNAL(clicked()), SLOT(addPlugin()));
  connect(removeButton, SIGNAL(clicked()), SLOT(removePlugin()));
  connect(addSearchPathButton, SIGNAL(clicked()), SLOT(addSearchPath()));
  connect(configureButton, SIGNAL(clicked()), SLOT(configurePlugin()));
}

QuantaPluginEditor::~QuantaPluginEditor()
{
}

QDict<QuantaPlugin> QuantaPluginEditor::plugins()
{
  return m_plugins;
}

void QuantaPluginEditor::accept()
{
  QDialog::accept();
}

void QuantaPluginEditor::addPlugin()
{
  // new plugin config menu
  // create new plugin and add to list
  QuantaPluginConfig *configDlg = new QuantaPluginConfig(this, "pluginConfig");

  if(configDlg->exec())
  {
    // plugin accepted, create the proper type
    QuantaPlugin *newPlugin = 0;
    newPlugin = new QuantaPlugin();

    newPlugin->setPluginName(configDlg->pluginName->text());
    newPlugin->setFileName(configDlg->pluginFileName->text());
    newPlugin->setLocation(configDlg->location->text());
    newPlugin->setOutputWindow(configDlg->outputWindow->currentText());
    newPlugin->setStandard(false);
    newPlugin->setIcon(configDlg->iconButton->icon());
    QString pluginName = newPlugin->pluginName();
    m_plugins.insert(pluginName, newPlugin);
    emit pluginsChanged();
  }
  delete configDlg;
}

void QuantaPluginEditor::addSearchPath()
{
  // Get a dir and append it to the line edit
  QString appendDir = KFileDialog::getExistingDirectory(QString::null, 0, i18n("Select Directory"));
  if(!appendDir.isNull())
  {
   searchPaths->setText(searchPaths->text()+appendDir+":");
  }
}

void QuantaPluginEditor::configurePlugin()
{
  // create a new config dialog. populate with plugin data
  QString key;
  QListViewItem *item = pluginList->currentItem();
  if(item)
    key = item->text(0);

  if(!key.isNull() && m_plugins.find(key))
  {
    QuantaPluginConfig *configDlg = new QuantaPluginConfig(this, "pluginConfig");
    QuantaPlugin *curPlugin = m_plugins.find(key);

    configDlg->outputWindow->setCurrentText(curPlugin->outputWindow());
    configDlg->pluginName->setText(curPlugin->pluginName());
    configDlg->pluginFileName->setText(curPlugin->fileName());
    configDlg->location->setText(curPlugin->location());
    configDlg->iconButton->setIcon(curPlugin->icon());
    configDlg->inputBox->setCurrentItem(curPlugin->input());
    configDlg->readOnlyBox->setChecked(curPlugin->readOnlyPart());

    if(configDlg->exec())
    {
      curPlugin->setPluginName(configDlg->pluginName->text());
      curPlugin->setFileName(configDlg->pluginFileName->text());
      curPlugin->setLocation(configDlg->location->text());
      curPlugin->setOutputWindow(configDlg->outputWindow->currentText());
      curPlugin->setIcon(configDlg->iconButton->icon());
      curPlugin->setInput(configDlg->inputBox->currentItem());
      curPlugin->setReadOnlyPart(configDlg->readOnlyBox->isChecked());
    }

    delete configDlg;

    emit pluginsChanged();
  }
}

void QuantaPluginEditor::refreshPlugins()
{
  // call validate on all plugins to refresh their status
  pluginList->clear();

  QDictIterator<QuantaPlugin> it(m_plugins);
  for(;it.current();++it)
  {
    QListViewItem *newItem = new PluginEditorItem(pluginList);
    QuantaPlugin *curPlugin = it.current();

    newItem->setText(0, curPlugin->pluginName());

    QPixmap statusIcon;
    bool isValid = QuantaPlugin::validatePlugin(curPlugin);
    if(isValid)
      statusIcon = SmallIcon("pluginValid", KGlobal::instance());
    else
      statusIcon = SmallIcon("pluginInvalid", KGlobal::instance());
    newItem->setPixmap(1, statusIcon);

    newItem->setText(2, curPlugin->location());
    newItem->setText(3, curPlugin->fileName());
    newItem->setText(4, curPlugin->outputWindow());

    pluginList->insertItem(newItem);
  }
}

void QuantaPluginEditor::removePlugin()
{
  QListViewItem *currentItem = pluginList->currentItem();
  if(currentItem)
  {
    QuantaPlugin *plugin = m_plugins.find(currentItem->text(0));
    if(plugin)
    {
       m_plugins.remove(plugin->pluginName());
      delete plugin;
      emit pluginsChanged();
    }
  }
}

void QuantaPluginEditor::setPlugins(QDict<QuantaPlugin> a_plugins)
{
  m_plugins = a_plugins;

  emit pluginsChanged();
}

PluginEditorItem::PluginEditorItem(QListView *a_parent)
  : QListViewItem(a_parent)
{
}

void PluginEditorItem::paintCell(QPainter *a_painter, const QColorGroup &a_cg, int a_column, int a_width, int a_align)
{
/*
  if(a_column == 2)
    a_align = Qt::AlignCenter;
  FIXME : Why won't QT let us align cell contents?
*/
  QListViewItem::paintCell(a_painter, a_cg, a_column, a_width, a_align);
}

/** No descriptions */
void QuantaPluginEditor::setSearchPaths(const QStringList& paths)
{
  QString str;
  for (uint i =0; i < paths.count(); i++)
  {
    str += paths[i] +":";
  }
  searchPaths->setText(str);
}
/** No descriptions */
QStringList QuantaPluginEditor::searchPathList()
{
  QString str = searchPaths->text();
  QStringList paths = QStringList::split(":",str);

  return paths;
}

#include "quantaplugineditor.moc"
