/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia   *
 *   amantia@kde.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "quantaprojectprojectconfig.h"

#include "quantaprojectpart.h"

QuantaProjectProjectConfig::QuantaProjectProjectConfig(QuantaProjectPart *part, QWidget *parent)
    : QWidget(parent), m_part(part)
{
  setupUi(this);
}

void QuantaProjectProjectConfig::accept()
{
}

#include "quantaprojectprojectconfig.moc"
