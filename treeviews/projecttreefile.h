/***************************************************************************
                          projectfile.h  -  description
                             -------------------
    begin                : Wed Mar 15 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
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

#ifndef PROJECTTREEFILE_H
#define PROJECTTREEFILE_H

#include <qlistview.h>

class ProjectTreeFolder;

/**project file
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class ProjectTreeFile : public QListViewItem  {
friend class Project;
public: 
	ProjectTreeFile( ProjectTreeFolder *parent, const char *name, const char *filename = 0L);
	~ProjectTreeFile();
  /**  */
  void setIcon(QString name);
  /** used for sorting */
  virtual QString key ( int column, bool ascending ) const;
  virtual void paintCell( QPainter *, const QColorGroup &, int, int, int);
  
public:
  QString fname;
};

#endif
