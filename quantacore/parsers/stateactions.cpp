/***************************************************************************
    begin                : Aug 24 2006
    copyright            : (C) 2006 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include <QString>

#include "stateactions.h"

StateActions::ActionFunctPtr StateActions::factory(const QString &name)
{
  QString id = name.toLower();
  if (id == "always") return &always;
  
  return &crashMe; // in case name is wrong
}


bool StateActions::crashMe(const ParserStatus &parser, const QString &argument)
{
  Q_ASSERT(false);
  return false;
}


bool StateActions::always(const ParserStatus &parser, const QString &argument)
{
  return true;
}

//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
