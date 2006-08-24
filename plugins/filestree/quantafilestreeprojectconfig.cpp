/***************************************************************************
 *   Copyright (C) 2005-2006 by Jens Herden                                *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "quantafilestreeprojectconfig.h"

#include "quantafilestreepart.h"

QuantaFilesTreeProjectConfig::QuantaFilesTreeProjectConfig(QuantaFilesTreePart *part, QWidget *parent)
    : QWidget(parent), m_part(part)
{
  setupUi(this);
}

void QuantaFilesTreeProjectConfig::accept()
{
}

#include "quantafilestreeprojectconfig.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
