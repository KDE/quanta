/***************************************************************************
                          docfolder.h  -  description
                             -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2002 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DOCFOLDER_H
#define DOCFOLDER_H

#include <qwidget.h>

#include <klistview.h>

/**
Class for forlder for
Doc tree

  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class KConfig;

class DocFolder : public KListViewItem  {
public:
  DocFolder(QListViewItem *parent, const QString &_name, KConfig *config, const QString &basePath);
  DocFolder(QListView *parent, const QString &_name, KConfig *config, const QString &basePath);
  ~DocFolder();
  QString text( int column ) const;
  void setup();
  /**  */
  void setOpen( bool o);

public:
  QString url;
  QString name;
  bool topLevel;
};

#endif
