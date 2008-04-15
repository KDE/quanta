/***************************************************************************
    begin                : Tue May 17 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

//own includes
#include "usertoolbarsglobalconfig.h"
#include "usertoolbarspart.h"

#include <kglobal.h>
#include <kconfiggroup.h>
#include <kpluginfactory.h>

//qt includes
#include <QCheckBox>

K_PLUGIN_FACTORY(UserToolbarsConfigFactory, registerPlugin<UserToolbarsGlobalConfig>(); )
    
K_EXPORT_PLUGIN(UserToolbarsConfigFactory("kcm_kdev_usertoolbars"))

UserToolbarsGlobalConfig::UserToolbarsGlobalConfig(QWidget *parent, const QVariantList &args)
  : KCModule( UserToolbarsConfigFactory::componentData(), parent, args )
{
  m_ui = new Ui::UserToolbarsGlobalConfigBase;
  m_ui->setupUi(this);
  connect(m_ui->separateToolbars, SIGNAL(stateChanged(int)), this, SLOT(changed()));
  connect(m_ui->createActionsMenu, SIGNAL(stateChanged(int)), this, SLOT(changed()));
}

UserToolbarsGlobalConfig::~UserToolbarsGlobalConfig()
{
  delete m_ui;
  m_ui = 0L;
}

void  UserToolbarsGlobalConfig::load()
{
  KConfigGroup config( KGlobal::config(), "General" );
  bool separateToolbars = config.readEntry("Separate toolbars", false);
  bool createActionsMenu = config.readEntry("Create Actions menu", true);
  m_ui->separateToolbars->setChecked(separateToolbars);
  m_ui->createActionsMenu->setChecked(createActionsMenu);
}

void  UserToolbarsGlobalConfig::save()
{
  KConfigGroup config( KGlobal::config(), "General" );
  config.writeEntry("Separate toolbars", m_ui->separateToolbars->isChecked());
  config.writeEntry("Create Actions menu", (m_ui->createActionsMenu->isChecked()));
  config.sync();
}

void  UserToolbarsGlobalConfig::defaults()
{
  m_ui->separateToolbars->setChecked(false);
  m_ui->createActionsMenu->setChecked(true);
}

#include "usertoolbarsglobalconfig.moc"
