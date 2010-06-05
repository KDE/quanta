/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia   *
 *   amantia@kde.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef QUANTAPROJECT_PROJECT_CONFIG_H
#define QUANTAPROJECT_PROJECT_CONFIG_H

#include "ui_quantaprojectprojectconfigbase.h"

class QuantaProjectPart;

class QuantaProjectProjectConfig: public QWidget, public Ui::QuantaProjectProjectConfigBase
{
    Q_OBJECT
public:
    explicit QuantaProjectProjectConfig(QuantaProjectPart *part, QWidget *parent = 0);

public slots:
    void accept();

private:
    QuantaProjectPart *m_part;
};

#endif
