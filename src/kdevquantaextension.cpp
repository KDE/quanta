/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "kdevquantaextension.h"

#include <QCheckBox>
#include <QButtonGroup>
#include <QRadioButton>

#include <klocale.h>
#include <kconfig.h>
#include <kdialog.h>
#include <kiconloader.h>
#include <kurlrequester.h>
#include <kapplication.h>

//kdevelop includes


KDevQuantaExtension::KDevQuantaExtension()
 : KDevelop::ShellExtension()
{
}

void KDevQuantaExtension::init()
{
    s_instance = new KDevQuantaExtension();
}

void KDevQuantaExtension::createGlobalSettingsPage(KDialog */*dlg*/)
{
//     KSharedConfig::Ptr config = KGlobal::config();
//     QVBox *vbox = dlg->addVBoxPage(i18n("General"), i18n("General"), BarIcon("kdevelop", KIcon::SizeMedium) );
//     gsw = new SettingsWidget(vbox, "general settings widget");
//
//     gsw->projectsURL->setMode((int)KFile::Directory);
//
//     config->setGroup("General Options");
//     gsw->lastProjectCheckbox->setChecked(config->readEntry("Read Last Project On Startup",true));
//     gsw->outputViewFontCombo->setCurrentFont( config->readFontEntry( "OutputViewFont" ).family() );
//     config->setGroup("MakeOutputView");
//     gsw->lineWrappingCheckBox->setChecked(config->readEntry("LineWrapping",true));
//     gsw->dirNavigMsgCheckBox->setChecked(config->readEntry("ShowDirNavigMsg",false));
//     gsw->compileOutputCombo->setCurrentIndex(config->readEntry("CompilerOutputLevel",2));
//     config->setGroup("General Options");
//     gsw->projectsURL->setURL(config->readPathEntry("DefaultProjectsDir", QDir::homePath()+"/"));
//     gsw->designerButtonGroup->setButton( config->readEntry( "DesignerApp", 0 ) );
//
//     config->setGroup("TerminalEmulator");
//     gsw->terminalButtonGroup->setButton( config->readEntry( "UseKDESetting", 0 ) );
//     gsw->terminalEdit->setText( config->readEntry( "TerminalApplication", QString::fromLatin1("konsole") ) );
}

void KDevQuantaExtension::acceptGlobalSettingsPage(KDialog */*dlg*/)
{
//     KSharedConfig::Ptr config = KGlobal::config();
//
//     config->setGroup("General Options");
//     config->writeEntry("DesignerApp", gsw->designerButtonGroup->selectedId());
//     config->writeEntry("Read Last Project On Startup",gsw->lastProjectCheckbox->isChecked());
//     config->writePathEntry("DefaultProjectsDir", gsw->projectsURL->url());
//     config->writeEntry("OutputViewFont", gsw->outputViewFontCombo->currentFont());
//     config->setGroup("MakeOutputView");
//     config->writeEntry("LineWrapping",gsw->lineWrappingCheckBox->isChecked());
//     config->writeEntry("ShowDirNavigMsg",gsw->dirNavigMsgCheckBox->isChecked());
//     //current item id must be in sync with the enum!
//     config->writeEntry("CompilerOutputLevel",gsw->compileOutputCombo->currentItem());
//     config->sync();
//     if( Koncrete::Plugin *makeExt = API::getInstance()->pluginController()->extension("KDevelop/MakeFrontend"))
//     {
//         static_cast<KDevMakeFrontend*>(makeExt)->updateSettingsFromConfig();
//     }
//
//     config->setGroup("TerminalEmulator");
//     config->writeEntry("UseKDESetting", gsw->useKDETerminal->isChecked() );
//     config->writeEntry("TerminalApplication", gsw->terminalEdit->text().trimmed() );
}

QString KDevQuantaExtension::xmlFile()
{
    return "quantaui.rc";
}

QString KDevQuantaExtension::defaultProfile()
{
    return "quanta";
}

KDevelop::AreaParams KDevQuantaExtension::defaultArea()
{
  KDevelop::AreaParams params = {"code", i18n("Code")};
  return params;
}
