/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden   *
 *   jens@kdewebdev.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef PROJECTTREE_GLOBAL_CONFIG_H
#define PROJECTTREE_GLOBAL_CONFIG_H

#include "projecttreeglobalconfigbase.h"

class ProjectTreePart;
class KConfigDialogManager;

class ProjectTreeGlobalConfig: public ProjectTreeGlobalConfigBase
{
  Q_OBJECT
public:
  ProjectTreeGlobalConfig(ProjectTreePart *part, QWidget *parent = 0, const char *name = 0);

  ~ProjectTreeGlobalConfig();

signals:
  /**
   * emitted when new configuration was accepted
   */
  void accepted();

public slots:
  void accept();

private:
  ProjectTreePart *m_part;
  KConfigDialogManager * manager;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;