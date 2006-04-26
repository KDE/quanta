/***************************************************************************
    begin                : Fri Jul 1 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef ENVIRONMENTCONFIG_H
#define ENVIRONMENTCONFIG_H

#include "environmentconfigbase.h"

/**
Configuration dialog for environmental settings.

@author Andras Mantia
*/
class EnvironmentConfig : public EnvironmentConfigBase
{
Q_OBJECT
public:
    EnvironmentConfig(QWidget *parent = 0, const char *name = 0);

    ~EnvironmentConfig();
public slots:
  void accept();

};

#endif
