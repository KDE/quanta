/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STRUCTURETREE_PROJECT_CONFIG_H
#define STRUCTURETREE_PROJECT_CONFIG_H

#include "ui_structuretreeprojectconfigbase.h"

class StructureTreePart;

class StructureTreeProjectConfig: public QWidget, public Ui::StructureTreeProjectConfigBase
{
    Q_OBJECT
public:
    explicit StructureTreeProjectConfig(StructureTreePart *part, QWidget *parent = 0);

public slots:
    void accept();

private:
    StructureTreePart *m_part;
};

#endif
