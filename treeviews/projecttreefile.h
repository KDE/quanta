/***************************************************************************
                          projectfile.h  -  description
                             -------------------
    begin                : Wed Mar 15 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2001, 2002 Andras Mantia
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

#ifndef PROJECTTREEFILE_H
#define PROJECTTREEFILE_H

#include "filestreefile.h"

class ProjectTreeFolder;

/**project file
  *@author Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia
  */

class ProjectTreeFile : public FilesTreeFile  {
friend class Project;
public: 
	ProjectTreeFile( ProjectTreeFolder *parent,  QString name, const KURL& p_url);
	~ProjectTreeFile();

  virtual void paintCell( QPainter *, const QColorGroup &, int, int, int);
  
public:
  QString fname;
};

#endif
