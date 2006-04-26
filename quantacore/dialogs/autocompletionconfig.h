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
#ifndef AUTOCOMPLETIONCONFIG_H
#define AUTOCOMPLETIONCONFIG_H

#include "autocompletionconfigbase.h"

/**
Configuration dialog for autocompletion settings.

@author Andras Mantia
*/
class AutocompletionConfig : public AutocompletionConfigBase
{
Q_OBJECT
public:
    AutocompletionConfig(QWidget *parent = 0, const char *name = 0);

    ~AutocompletionConfig();

public slots:
  void accept();

};

#endif
