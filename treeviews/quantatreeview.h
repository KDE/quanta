/***************************************************************************
                          quantatreeview.h  -  description
                             -------------------
    begin                : Wed Dec 19 2001
    copyright            : (C) 2001 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QUANTATREEVIEW_H
#define QUANTATREEVIEW_H

#include <qwidget.h>
#include <qlistview.h>

/**
  *@author Andras Mantia
  */

class QStringList;
class ProjectTreeFolder;

class QuantaTreeView : public QListView  {
   Q_OBJECT
public: 
	QuantaTreeView(QWidget *parent=0, const char *name=0);
	~QuantaTreeView();
public slots: // Public slots
  /** No descriptions */
  void slotReloadTree( QStringList fileList, bool newtree);

public:
  ProjectTreeFolder *topFolder;
};

#endif
