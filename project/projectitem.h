/***************************************************************************
                          projectitem.h  -  description
                             -------------------
    begin                : Thu Mar 16 2000
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

#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include <qwidget.h>
#include <time.h>

/**project file properties
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class ProjectItem {
public: 
 	ProjectItem( QString basePath);
	~ProjectItem();
  /** return modified time */
  time_t modifiedTime();
  /** size of file */
  uint size();
  /**  */
  QString fullName();
  /** need upload ??? */
  bool needUpload();
  /**  */
  void updateUploadTime();
public: // Public attributes
  /** file name */
  QString fname;
  /** base dir */
  QString basePath;
  /** url for upload file */
  QString URL;
  /** time of last upload */
  time_t time;
};

#endif
