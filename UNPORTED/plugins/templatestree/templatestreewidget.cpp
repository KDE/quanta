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

#include "templatestreewidget.h"

#include <kurl.h>
#include <klibloader.h>
#include <kparts/part.h>

#include <kdevcore.h>

#include "templatestreepart.h"

TemplatesTreeWidget::TemplatesTreeWidget(TemplatesTreePart *part)
    : QWidget(0, "templatestree widget"), m_part(part)
{
}

TemplatesTreeWidget::~TemplatesTreeWidget()
{
}

#include "templatestreewidget.moc"
