/***************************************************************************
                          quantapluginconfig.cpp  -  description
                             -------------------
    begin                : Sat Sep 21 2002
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
#include <qwidgetstack.h>

/* OTHER INCLUDES */
#include "quantapluginconfig.h"
#include "quantaplugin.h"
#include "quantaplugininterface.h"

QuantaPluginConfig::QuantaPluginConfig(QWidget *a_parent, const char *a_name)
  : PluginConfig(a_parent, a_name)
{
  connect(pluginName, SIGNAL(textChanged(const QString &)), this, SLOT(nameChanged(const QString &)));
  connect(locationButton, SIGNAL(clicked()), this, SLOT(selectLocation()));

  QStringList windows;
  windows << i18n("Editor Tab") << i18n("Editor Frame") << i18n("Message Frame");
  outputWindow->insertStringList(windows);
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
    bool isValid = QuantaPlugin::validatePluginInfo(pluginName->text(), location->text(), pluginFileName->text(), outputWindow->currentText());

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
  QString text = a_text;
  text = "lib" + text + ".la";
  pluginFileName->setText(text.lower());
}

#include "quantapluginconfig.moc"
