/***************************************************************************
    begin                : Mon Jun 20 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include "tagdialogsif.h"

TagDialogsIf::TagDialogsIf(KInstance * info, QObject *parent)
  : Koncrete::Plugin(info, parent)
{
}


TagDialogsIf::~TagDialogsIf()
{
}

#include "tagdialogsif.moc"
