/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "structuretreeprojectconfig.h"

#include "structuretreepart.h"

StructureTreeProjectConfig::StructureTreeProjectConfig(StructureTreePart *part, QWidget *parent)
    : QWidget(parent), m_part(part)
{
  setupUi(this);
}

void StructureTreeProjectConfig::accept()
{
}

#include "structuretreeprojectconfig.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
