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

//qt includes
#include <qcheckbox.h>

UserToolbarsGlobalConfig::UserToolbarsGlobalConfig(UserToolbarsPart *part, QWidget *parent, const char *name)
    : UserToolbarsGlobalConfigBase(parent, name), m_part(part)
{
  separateToolbars->setChecked(m_part->separateToolbars());
  createActionsMenu->setChecked(m_part->createActionsMenu());
}

void UserToolbarsGlobalConfig::accept()
{
  m_part->setSeparateToolbars(separateToolbars->isChecked());
  m_part->setCreateActionsMenu(createActionsMenu->isChecked());
  m_part->saveConfig();
}

#include "usertoolbarsglobalconfig.moc"
