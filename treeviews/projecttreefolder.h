/***************************************************************************
                          ProjectTreeFolder.h  -  description
                             -------------------
    begin                : Wed Mar 15 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia
    email                : pdima@mail.univ.kiev.ua
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

#include <qlist.h>

#include "filestreefolder.h"
#include "projecttreefile.h"

/**folder in project window
  *@author Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia
  */

class ProjectTreeFolder : public FilesTreeFolder  {
friend class Project;

public:
	ProjectTreeFolder( ProjectTreeFolder *parent, const char *);
	ProjectTreeFolder( QListView *parent, const char *, const char *);
	~ProjectTreeFolder();
	
  virtual void setOpen( bool );
  void setup();
	
  /** return fullname of child element item */
  QString fullName(QListViewItem *item = 0);
  /** insert item in file list */
  void insertItem(ProjectTreeFile *item, QString name);
  /**  */
  int find(QString name);
  virtual void paintCell( QPainter *, const QColorGroup &, int, int, int);
  
public:
  QList<ProjectTreeFile> fileList;
  /**  */
  QString path;
};

#endif
