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


#ifndef PATHMAPPER_H
#define PATHMAPPER_H

#include <qobject.h>

class KURL;

class PathMapper : public QObject
{
    Q_OBJECT

  public:
    PathMapper(QObject *parent = 0, const char *name = 0);
    PathMapper();
    virtual QString mapLocalPathToServer(const QString &localpath);
    virtual QString mapServerPathToLocal(const QString &serverpath);

    virtual void setLocalBasedir(const QString &localpath);
    virtual void setServerBasedir(const QString &serverpath);

  private:
    QString m_localBasedir;
    QString m_serverBasedir;
};

#endif
