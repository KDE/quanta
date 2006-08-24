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

#ifndef PROJECTTREE_PROJECT_CONFIG_H
#define PROJECTTREE_PROJECT_CONFIG_H

#include "ui_projecttreeprojectconfigbase.h"

class ProjectTreePart;

class ProjectTreeProjectConfig: public QWidget, public Ui::ProjectTreeProjectConfigBase
{
    Q_OBJECT
public:
    ProjectTreeProjectConfig(ProjectTreePart *part, QWidget *parent = 0);

public slots:
    void accept();

private:
    ProjectTreePart *m_part;
};

#endif
