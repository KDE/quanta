/***************************************************************************
                          parseroptions.h  -  description
                             -------------------
    begin                : Fri Aug 23 2002
    copyright            : (C) 2002 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef PARSEROPTIONS_H
#define PARSEROPTIONS_H

#include "parseroptionsui.h"

class KConfig;

class ParserOptions : public ParserOptionsUI
{
    Q_OBJECT

public:
    ParserOptions( KConfig *config, QWidget* parent = 0, const char* name = 0);
    ~ParserOptions();

    void updateConfig();

protected:
    KConfig *config;
};

#endif // PARSEROPTIONS_H
