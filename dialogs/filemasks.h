/***************************************************************************
                          filesmask.h  -  description
                             -------------------
    begin                : Fri May 19 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002 Andras Mantia <amantia@freemail.hu>
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

#ifndef FILEMASKS_H
#define FILEMASKS_H

#include "filemaskss.h"


class FileMasks : public FileMasksS
{
   Q_OBJECT
public: 
	FileMasks(QWidget *parent=0, const char *name=0);
	~FileMasks();

public slots:
  void setToDefault();
};

#endif
