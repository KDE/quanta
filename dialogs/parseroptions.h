/***************************************************************************
                          parseroptions.h  -  description
                             -------------------
    begin                : Fri Aug 23 2002
    copyright            : (C) 2002 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
