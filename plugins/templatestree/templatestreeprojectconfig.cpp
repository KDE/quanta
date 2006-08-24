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

#include "templatestreeprojectconfig.h"

#include "templatestreepart.h"

TemplatesTreeProjectConfig::TemplatesTreeProjectConfig(TemplatesTreePart *part, QWidget *parent)
    : QWidget(parent), m_part(part)
{
  setupUi(this);
}

void TemplatesTreeProjectConfig::accept()
{
}

#include "templatestreeprojectconfig.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
