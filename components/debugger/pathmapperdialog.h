/***************************************************************************
                             pathmapperdialog.h
                             ------------------
    begin                : 2004-04-05
    copyright            : (C) 2004 Linus McCabe <linus@mccabe.nu>
 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/

#ifndef PATHMAPPERDIALOG_H
#define PATHMAPPERDIALOG_H

#include "pathmapperdialogs.h"

class PathMapperDialog : public PathMapperDialogS
{
  Q_OBJECT

  public:
    enum Direction
    {
      ServerToLocal = 0,
      LocalToServer
    };
  
    PathMapperDialog(const QString& path, const PathMapperDialog::Direction direction);
    ~PathMapperDialog();

    void addHistory(const QString &serverdir, const QString &localdir);

    QString serverPath();
    QString localPath();
    
  public slots:
    void slotSelectionChanged();
    void slotPathsChanged();
  
  private:
    QString m_path;
    Direction m_direction;
    
};

#endif // PATHMAPPERDIALOG_H

