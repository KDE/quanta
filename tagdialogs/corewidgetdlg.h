/***************************************************************************
                          corewidgetdlg.h  -  description
                             -------------------
    begin                : Thu Mar 30 2000
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

#ifndef COREWIDGETDLG_H
#define COREWIDGETDLG_H

#include <qdict.h>

#include "tagwidget.h"
#include "corewidgetdlgs.h"

/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class CoreWidgetDlg : public CoreWidgetDlgS, TagWidget {
   Q_OBJECT
public: 
	CoreWidgetDlg(QWidget *parent=0, const char *name=0);
	~CoreWidgetDlg();
	
  void readAttributes( QDict<QString> *d );
  void writeAttributes( QDict<QString> *d );
	
public:
  void disableI18nAttribs();
  void disableCoreAttribs();

};

#endif
