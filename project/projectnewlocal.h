/***************************************************************************
                          projectnewlocal.h  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PROJECTNEWLOCAL_H
#define PROJECTNEWLOCAL_H

#include <qwidget.h>
#include <qstringlist.h>
#include <projectnewlocals.h>

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class ProjectNewLocal : public ProjectNewLocalS  {
   Q_OBJECT
public: 
	ProjectNewLocal(QWidget *parent=0, const char *name=0);
	~ProjectNewLocal();
	
	QStringList files(bool relative=false);
	
public slots:
	void setFiles(bool);
  void setDestDir(QWidget*,bool);

private:
	QString dir;
};

#endif
