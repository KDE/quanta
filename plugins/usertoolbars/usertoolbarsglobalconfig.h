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

#include <kcmodule.h>

#include "ui_usertoolbarsglobalconfigbase.h"

class UserToolbarsGlobalConfig: public KCModule
{
    Q_OBJECT
public:
     explicit UserToolbarsGlobalConfig(QWidget *parent, const QVariantList &args = QVariantList() );
    ~UserToolbarsGlobalConfig();

public slots:
    void accept();

private:
    Ui::UserToolbarsGlobalConfigBase *m_ui;
};

#endif
