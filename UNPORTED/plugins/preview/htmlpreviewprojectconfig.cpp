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

#include "htmlpreviewprojectconfig.h"

#include "htmlpreviewpart.h"

HTMLPreviewProjectConfig::HTMLPreviewProjectConfig(HTMLPreviewPart *part, QWidget *parent)
    : QWidget(parent), m_part(part)
{
  setupUi(this);
}

void HTMLPreviewProjectConfig::accept()
{
}

#include "htmlpreviewprojectconfig.moc"
