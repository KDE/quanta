/***************************************************************************
                          quantapluginconfig.cpp  -  description
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

/* KDE INCLUDES */
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kicondialog.h>

/* QT INCLUDES */
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qcombobox.h>

/* OTHER INCLUDES */
#include "quantapluginconfig.h"
#include "quantaplugin.h"
#include "quantakpartplugin.h"
#include "quantacmdplugin.h"
#include "quantaplugininterface.h"

QuantaPluginConfig::QuantaPluginConfig(QWidget *a_parent, const char *a_name)
  : PluginConfig(a_parent, a_name)
{
  connect(pluginName, SIGNAL(textChanged(const QString &)), this, SLOT(nameChanged(const QString &)));
  connect(locationButton, SIGNAL(clicked()), this, SLOT(selectLocation()));

  connect(pluginType, SIGNAL(activated(const QString &)), this, SLOT(updateWindows(const QString &)));
 
  pluginType->insertStringList(QuantaPluginInterface::pluginTypes());
  updateWindows(pluginType->currentText()); //force an update

  iconButton->setStrictIconSize(false);
//  iconButton->
}

QuantaPluginConfig::~QuantaPluginConfig()
{
}

void QuantaPluginConfig::accept()
{
  if(validateCheckBox->isChecked())
  {
    bool isValid = QuantaPlugin::validatePluginInfo(pluginName->text(),
                   pluginType->currentText(), location->text(), pluginFileName->text(),
                   arguments->text(), outputWindow->currentText());

      if(!isValid)
      {
        int answer = KMessageBox::questionYesNo(this, i18n("The plugin information you entered appears to be invalid. Are you sure you want to apply these settings?"), i18n("Invalid Plugin"));

        if(answer == KMessageBox::No)
          return;
      }
  }
  QDialog::accept();
}

/** Gets the plugin location */
void QuantaPluginConfig::selectLocation()
{
  QString pluginLocation = KFileDialog::getExistingDirectory(QString::null, this, i18n("Select Plugin Directory"));
  if(!pluginLocation.isNull())
  {
    location->setText(pluginLocation);
  }
}

void QuantaPluginConfig::nameChanged(const QString &a_text)
{
  QString type = pluginType->currentText();

  QString text = a_text;
  if(type == i18n("KPart"))
    text = QString("lib") + text + ".so";
  else if(type == i18n("Command Line"))
    ;
    
  else
    qWarning("QuantaPluginConfig::nameChanged - Unknown plugin type \'%s\'", type.latin1());
  
  pluginFileName->setText(text.lower());
}

/** Updates the windows combobox based on the type */
void QuantaPluginConfig::updateWindows(const QString &a_type)
{
  QStringList windows = QuantaPluginInterface::outputWindows(a_type);
  outputWindow->clear();
  outputWindow->insertStringList(windows);  
}

#include "quantapluginconfig.moc"
