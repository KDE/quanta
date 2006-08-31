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

#ifndef PROJECTTREE_WIDGET_H
#define PROJECTTREE_WIDGET_H

#include <qwidget.h>
#include <QString>

class KDevProject;
class ProjectTreePart;
class ProjectTreeView;

class ProjectTreeWidget: public QWidget
{
  Q_OBJECT
public:
  explicit ProjectTreeWidget(ProjectTreePart *part);
  ~ProjectTreeWidget();

  ProjectTreeView * tree() {return m_tree;}
  
private:
  ProjectTreePart * m_part;
  ProjectTreeView * m_tree;
};


#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
