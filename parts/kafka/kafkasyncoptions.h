/***************************************************************************
                             kafkasyncoptions.h
                             -------------------

    copyright            : (C) 2003 - Nicolas Deschildre
    email                : ndeschildre@kdewebdev.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KAFKASYNCOPTIONS_H
#define KAFKASYNCOPTIONS_H

#include "kafkasyncoptionsui.h"

class KConfig;

class KafkaSyncOptions : public KafkaSyncOptionsUI
{ 
    Q_OBJECT

public:
    KafkaSyncOptions( KConfig *config, QWidget* parent = 0, const char* name = 0);
    ~KafkaSyncOptions();

    void updateConfig();

protected:
    KConfig *config;
};

#endif
