/***************************************************************************
                          dcopsettings.cpp  -  description
                             -------------------
    begin                : Thu Jan 29 2004
    copyright            : (C) 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "dcopsettings.h"
#include "quanta.h"
#include "quantaview.h"
#include "document.h"
#include "resource.h"
#include "quantacommon.h"
#include "project.h"
#include "dtds.h"

DCOPSettings::DCOPSettings() : DCOPObject("SettingsIf")
{
}

QString DCOPSettings::encoding()
{
  QString encoding = quantaApp->defaultEncoding();
  encoding.replace("iso ", "iso-"); //it's said that "iso-8859-x" is the valid format
  return encoding;
}

QString DCOPSettings::dtep()
{
  if (Project::ref()->hasProject())
  {
    return DTDs::ref()->getDTDNickNameFromName(Project::ref()->defaultDTD());
  }
  else
    return qConfig.defaultDocType;
}



