/***************************************************************************
                          quantapluginconfig.cpp  -  description
                             -------------------
    begin                : Sat Sep 21 2002
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

/* KDE INCLUDES */
#include <kdialogbase.h>
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
#include "pluginconfig.h"

QuantaPluginConfig::QuantaPluginConfig(QWidget *a_parent, const char *a_name)
  : KDialogBase(a_parent, a_name, true, i18n("Configure Plugin"), KDialogBase::Ok | KDialogBase::Cancel)
{
  m_pluginConfigWidget = new PluginConfig(this);

  QStringList windows;
  i18n("Message Area Tab");
  windows << i18n("Editor Tab") << i18n("Separate Toolview");
  m_pluginConfigWidget->outputWindow->insertStringList(windows);
  m_pluginConfigWidget->iconButton->setStrictIconSize(false);

  connect(m_pluginConfigWidget->pluginName, SIGNAL(textChanged(const QString &)), this, SLOT(nameChanged(const QString &)));
  connect(m_pluginConfigWidget->locationButton, SIGNAL(clicked()), this, SLOT(selectLocation()));

  setMainWidget(m_pluginConfigWidget);
}

QuantaPluginConfig::~QuantaPluginConfig()
{
}

void QuantaPluginConfig::accept()
{
  if(m_pluginConfigWidget->validateCheckBox->isChecked())
  {
    bool isValid = QuantaPlugin::validatePluginInfo(m_pluginConfigWidget->pluginName->text(), m_pluginConfigWidget->location->text(), m_pluginConfigWidget->pluginFileName->text(), m_pluginConfigWidget->outputWindow->currentText());

    if(!isValid)
    {
      int answer = KMessageBox::questionYesNo(this, i18n("The plugin information you entered appears to be invalid. Are you sure you want to apply these settings?"), i18n("Invalid Plugin"), KStdGuiItem::apply(), i18n("Do Not Apply"));

      if(answer == KMessageBox::No)
        return;
    }
  }
  KDialogBase::accept();
}

/** Gets the plugin location */
void QuantaPluginConfig::selectLocation()
{
  QString pluginLocation = KFileDialog::getExistingDirectory(QString::null, this, i18n("Select Plugin Folder"));
  if(!pluginLocation.isNull())
  {
    m_pluginConfigWidget->location->setText(pluginLocation);
  }
}

void QuantaPluginConfig::nameChanged(const QString &a_text)
{
  QString text = a_text;
  text = "lib" + text + ".la";
  m_pluginConfigWidget->pluginFileName->setText(text.lower());
}

#include "quantapluginconfig.moc"
