/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden   *
 *   jens@kdewebdev.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUANTAFILESTREE_PROJECT_CONFIG_H
#define QUANTAFILESTREE_PROJECT_CONFIG_H

#include "ui_quantafilestreeprojectconfigbase.h"

class QuantaFilesTreePart;

class QuantaFilesTreeProjectConfig: public QWidget, public Ui::QuantaFilesTreeProjectConfigBase
{
    Q_OBJECT
public:
    explicit QuantaFilesTreeProjectConfig(QuantaFilesTreePart *part, QWidget *parent = 0);

public slots:
    void accept();

private:
    QuantaFilesTreePart *m_part;
};

#endif
