/***************************************************************************
                          projectnewgeneral.h  -  description
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

#ifndef PROJECTNEWGENERAL_H
#define PROJECTNEWGENERAL_H

#include "projectnewgenerals.h"

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class ProjectNewGeneral : public ProjectNewGeneralS  {
   Q_OBJECT
public: 
	ProjectNewGeneral(QWidget *parent=0, const char *name=0);
	~ProjectNewGeneral();
	
	QString type();
	void setMargin(int);
	
public slots:
	void slotButtonDir();
	void slotLinePrjFile(const QString &);
	void slotChangeNames(const QString &);
  void slotButtonTmpl();
  void slotButtonToolbar();
  void slotLinePrjTmpl(const QString &);
  void slotLinePrjToolbar(const QString &);

		
signals:
  void setBasePath(QString);
	void enableNextButton(QWidget *, bool);
};

#endif
