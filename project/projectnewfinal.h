/***************************************************************************
                          projectnewgeneral.h  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2001-2002 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PROJECTNEWFINAL_H
#define PROJECTNEWFINAL_H

#include "projectnewfinals.h"

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class ProjectNewFinal : public ProjectNewFinalS  {
   Q_OBJECT
public:
  ProjectNewFinal(QWidget *parent=0, const char *name=0);
  ~ProjectNewFinal();

  void setMargin(int);
};

#endif
