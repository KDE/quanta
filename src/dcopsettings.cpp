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
 
DCOPSettings::DCOPSettings() : DCOPObject("SettingsIf")
{
}
 
QString DCOPSettings::encoding()
{
  return quantaApp->defaultEncoding();
}

QString DCOPSettings::dtep()
{
  if (quantaApp->view()->writeExists()) 
    return quantaApp->view()->write()->defaultDTD()->nickName;
  else
    return qConfig.defaultDocType;  
}



