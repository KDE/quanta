/***************************************************************************
                          ProjectTreeFolder.h  -  description
                             -------------------
    begin                : Wed Mar 15 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002 Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ProjectTreeFolder_H
#define ProjectTreeFolder_H

#include <qptrlist.h>

#include "filestreefolder.h"
#include "projecttreefile.h"

/**folder in project window
  *@author Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia
  */

class ProjectTreeView;

class ProjectTreeFolder : public FilesTreeFolder  {
friend class Project;

public:
  ProjectTreeFolder( QListView *parentListView, ProjectTreeFolder *parent, const KURL& p_url);
  ProjectTreeFolder( QListView *parentListView, const QString& name, const KURL& p_url);
  ~ProjectTreeFolder();

  virtual void setOpen( bool open);
  void setup();

  /** insert item in file list */
  void insertItem(ProjectTreeFile *item, const KURL& url);
  /**  */
  bool contains(const KURL& url);
  virtual void paintCell( QPainter *, const QColorGroup &, int, int, int);

public:
  QPtrList<ProjectTreeFile> filesTreeList;
  /**  */
  QString path;

protected:
  ProjectTreeView *parentView;
};

#endif
