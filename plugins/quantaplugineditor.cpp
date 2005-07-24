/***************************************************************************
                          quantaplugineditor.cpp  -  description
                             -------------------
    begin                : Fri Sep 20 2002
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

/* KDE INCLUDES */
#include <kfiledialog.h>
#include <kiconloader.h>
#include <kglobal.h>
#include <klocale.h>
#include <kicondialog.h>
#include <kdialogbase.h>

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
#include "pluginconfig.h"
#include "plugineditor.h"
#include "resource.h"

QuantaPluginEditor::QuantaPluginEditor(QWidget *a_parent, const char *a_name)
  : KDialogBase(a_parent, a_name, true, i18n("Edit Plugins"), KDialogBase::Ok | KDialogBase::Cancel)
{
 // m_plugins.setAutoDelete(true);

  m_pluginEditorWidget = new PluginEditor(this);
  setMainWidget(m_pluginEditorWidget);
  m_pluginEditorWidget->pluginList->setSelectionMode(QListView::Single);
  m_pluginEditorWidget->pluginList->setAllColumnsShowFocus(true);
  m_pluginEditorWidget->pluginList->setColumnAlignment(2, Qt::AlignHCenter);

  connect(this, SIGNAL(pluginsChanged()), SLOT(refreshPlugins()));
  connect(m_pluginEditorWidget->refreshButton, SIGNAL(clicked()), SLOT(refreshPlugins()));
  connect(m_pluginEditorWidget->addButton, SIGNAL(clicked()), SLOT(addPlugin()));
  connect(m_pluginEditorWidget->removeButton, SIGNAL(clicked()), SLOT(removePlugin()));
  connect(m_pluginEditorWidget->addSearchPathButton, SIGNAL(clicked()), SLOT(addSearchPath()));
  connect(m_pluginEditorWidget->configureButton, SIGNAL(clicked()), SLOT(configurePlugin()));
  connect(m_pluginEditorWidget->pluginList, SIGNAL(doubleClicked(QListViewItem*, const QPoint&, int)),
     SLOT(configurePlugin(QListViewItem*, const QPoint&, int)));
}

QuantaPluginEditor::~QuantaPluginEditor()
{
}

QDict<QuantaPlugin> QuantaPluginEditor::plugins()
{
  return m_plugins;
}

void QuantaPluginEditor::addPlugin()
{
  // new plugin config menu
  // create new plugin and add to list
  QuantaPluginConfig *configDlg = new QuantaPluginConfig(this, "pluginConfig");
  PluginConfig *configWidget = static_cast<PluginConfig*>(configDlg->mainWidget());

  if(configDlg->exec())
  {
    // plugin accepted, create the proper type
    QuantaPlugin *newPlugin = 0;
    newPlugin = new QuantaPlugin();

    newPlugin->setPluginName(configWidget->pluginName->text());
    newPlugin->setFileName(configWidget->pluginFileName->text());
    newPlugin->setLocation(configWidget->location->text());
    newPlugin->setOutputWindow(configWidget->outputWindow->currentText());
    newPlugin->setStandard(false);
    newPlugin->setIcon(configWidget->iconButton->icon());
    QString pluginName = newPlugin->pluginName();
    m_plugins.insert(pluginName, newPlugin);
    emit pluginsChanged();
  }
  delete configDlg;
}

void QuantaPluginEditor::addSearchPath()
{
  // Get a dir and append it to the line edit
  QString appendDir = KFileDialog::getExistingDirectory(QString::null, 0, i18n("Select Folder"));
  if(!appendDir.isNull())
  {
    m_pluginEditorWidget->searchPaths->setText(m_pluginEditorWidget->searchPaths->text()+appendDir+":");
  }
}

void QuantaPluginEditor::configurePlugin()
{
  // create a new config dialog. populate with plugin data
  QString key;
  QListViewItem *item = m_pluginEditorWidget->pluginList->currentItem();
  if(item)
    key = item->text(0);

  if(!key.isNull() && m_plugins.find(key))
  {
    QuantaPluginConfig *configDlg = new QuantaPluginConfig(this, "pluginConfig");
    QuantaPlugin *curPlugin = m_plugins.find(key);
    PluginConfig *configWidget = static_cast<PluginConfig*>(configDlg->mainWidget());

    configWidget->outputWindow->setCurrentText(curPlugin->outputWindow());
    configWidget->pluginName->setText(curPlugin->pluginName());
    configWidget->pluginFileName->setText(curPlugin->fileName());
    configWidget->location->setText(curPlugin->location());
    configWidget->iconButton->setIcon(curPlugin->icon());
    configWidget->inputBox->setCurrentItem(curPlugin->input());
    configWidget->readOnlyBox->setChecked(curPlugin->readOnlyPart());

    if(configDlg->exec())
    {
      curPlugin->setPluginName(configWidget->pluginName->text());
      curPlugin->setFileName(configWidget->pluginFileName->text());
      curPlugin->setLocation(configWidget->location->text());
      curPlugin->setOutputWindow(configWidget->outputWindow->currentText());
      curPlugin->setIcon(configWidget->iconButton->icon());
      curPlugin->setInput(configWidget->inputBox->currentItem());
      curPlugin->setReadOnlyPart(configWidget->readOnlyBox->isChecked());
    }

    delete configDlg;

    emit pluginsChanged();
  }
}

void QuantaPluginEditor::configurePlugin(QListViewItem* item, const QPoint&, int)
{
  if (item)
    configurePlugin();
}

void QuantaPluginEditor::refreshPlugins()
{
  // call validate on all plugins to refresh their status
  m_pluginEditorWidget->pluginList->clear();

  QDictIterator<QuantaPlugin> it(m_plugins);
  for(;it.current();++it)
  {
    QListViewItem *newItem = new PluginEditorItem(m_pluginEditorWidget->pluginList);
    QuantaPlugin *curPlugin = it.current();

    newItem->setText(0, curPlugin->pluginName());

    QPixmap statusIcon;
    bool isValid = QuantaPlugin::validatePlugin(curPlugin);
    if(isValid)
      statusIcon = SmallIcon("ok", KGlobal::instance());
    else
      statusIcon = SmallIcon("no", KGlobal::instance());
    newItem->setPixmap(1, statusIcon);

    newItem->setText(2, curPlugin->location());
    newItem->setText(3, curPlugin->fileName());
    newItem->setText(4, curPlugin->outputWindow());

    m_pluginEditorWidget->pluginList->insertItem(newItem);
  }
}

void QuantaPluginEditor::removePlugin()
{
  QListViewItem *currentItem = m_pluginEditorWidget->pluginList->currentItem();
  if (currentItem)
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
  m_pluginEditorWidget->searchPaths->setText(str);
}
/** No descriptions */
QStringList QuantaPluginEditor::searchPathList()
{
  QString str = m_pluginEditorWidget->searchPaths->text();
  QStringList paths = QStringList::split(":",str);

  return paths;
}

#include "quantaplugineditor.moc"
