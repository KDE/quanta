/***************************************************************************
                               pathmapper.h
                         ------------------------
    begin                : 2004-10-10
    copyright            : (C) 2004 Linus McCabe

 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/

#include "pathmapper.h"

#include "quantacommon.h"
#include "quanta.h"
#include "resource.h"

#include <klocale.h>
#include <qstring.h>
#include <qextfileinfo.h>

PathMapper::PathMapper(QObject *parent, const char *name)
    : QObject(parent, name)
{

}

PathMapper::PathMapper()
{

}

QString PathMapper::mapLocalPathToServer(const QString &localpath)
{
  QString newpath;

  if(localpath.startsWith(m_localBasedir, false))
  {
    newpath = localpath;
    newpath.remove(0, m_localBasedir.length());
    newpath = m_serverBasedir + newpath;

    return newpath;
  }

  return localpath;

}

QString PathMapper::mapServerPathToLocal(const QString &serverpath)
{
  QString newpath;

  if(serverpath.startsWith(m_serverBasedir, false))
  {
    newpath = serverpath;
    newpath.remove(0, m_serverBasedir.length());
    newpath = m_localBasedir + newpath;

    if(QExtFileInfo::exists(newpath))
      return newpath;
  }
  return serverpath;
}

void PathMapper::setLocalBasedir(const QString &localpath)
{
  m_localBasedir = localpath;
}
void PathMapper::setServerBasedir(const QString &serverpath)
{
  m_serverBasedir = serverpath;
}


#include "pathmapper.moc"
