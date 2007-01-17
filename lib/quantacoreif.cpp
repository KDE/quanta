/***************************************************************************
    begin                : Mon Jun 20 2005
    copyright            : (C) 2005 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include "quantacoreif.h"

QuantaCoreIf::QuantaCoreIf(KInstance * info, QObject *parent)
  : Koncrete::Plugin(info, parent)
{
}


QuantaCoreIf::~QuantaCoreIf()
{
}

#include "quantacoreif.moc"
