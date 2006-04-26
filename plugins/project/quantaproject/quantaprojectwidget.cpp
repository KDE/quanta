/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia   *
 *   amantia@kde.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.           *
 ***************************************************************************/

#include "quantaprojectwidget.h"

#include <kurl.h>
#include <klibloader.h>
#include <kparts/part.h>

#include <kdevcore.h>

#include "quantaprojectpart.h"

QuantaProjectWidget::QuantaProjectWidget(QuantaProjectPart *part)
    : QWidget(0, "quantaproject widget"), m_part(part)
{
}

QuantaProjectWidget::~QuantaProjectWidget()
{
}

#include "quantaprojectwidget.moc"
