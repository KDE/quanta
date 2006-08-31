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

#ifndef TEMPLATESTREE_PROJECT_CONFIG_H
#define TEMPLATESTREE_PROJECT_CONFIG_H

#include "ui_templatestreeprojectconfigbase.h"

class TemplatesTreePart;

class TemplatesTreeProjectConfig: public QWidget, public Ui::TemplatesTreeProjectConfigBase
{
    Q_OBJECT
public:
    explicit TemplatesTreeProjectConfig(TemplatesTreePart *part, QWidget *parent = 0);

public slots:
    void accept();

private:
    TemplatesTreePart *m_part;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
