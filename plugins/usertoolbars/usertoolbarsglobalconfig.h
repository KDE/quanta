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


#ifndef USERTOOLBARS_GLOBAL_CONFIG_H
#define USERTOOLBARS_GLOBAL_CONFIG_H

#include "ui_usertoolbarsglobalconfigbase.h"

class UserToolbarsPart;

class UserToolbarsGlobalConfig: public QWidget, public Ui::UserToolbarsGlobalConfigBase
{
    Q_OBJECT
public:
    UserToolbarsGlobalConfig(UserToolbarsPart *part, QWidget *parent = 0);

public slots:
    void accept();

private:
    UserToolbarsPart *m_part;
};

#endif
