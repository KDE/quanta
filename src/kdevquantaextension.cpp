/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "kdevquantaextension.h"

#include <KLocalizedString>

#include <QApplication>

KDevQuantaExtension::KDevQuantaExtension()
 : KDevelop::ShellExtension()
{
}

void KDevQuantaExtension::init()
{
    s_instance = new KDevQuantaExtension();
}

QString KDevQuantaExtension::xmlFile()
{
    return "quantaui.rc";
}

QString KDevQuantaExtension::binaryPath()
{
    return QApplication::applicationDirPath() + "/quanta";
}

KDevelop::AreaParams KDevQuantaExtension::defaultArea()
{
  KDevelop::AreaParams params = {"code", i18n("Code")};
  return params;
}

QString KDevQuantaExtension::projectFileExtension()
{
    return "kdev4";
}

QString KDevQuantaExtension::projectFileDescription()
{
    return i18n("KDevelop4 Project Files");
}

