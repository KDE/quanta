/***************************************************************************
                          quicktablecolumnedit.h  -  description
                             -------------------
    begin                : Wed Jul 17 2002
    copyright            : (C) 2002 by Andras Mantia
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

#ifndef QUICKTABLECOLUMNEDIT_H
#define QUICKTABLECOLUMNEDIT_H

#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qwidget.h>
#include <tablecolumndialog.h>

/**
  *@author Andras Mantia
  */

class QuickTableColumnEdit : public TableColumnDialog  {
   Q_OBJECT
public: 
	QuickTableColumnEdit(QWidget *parent=0, const char *name=0);
	~QuickTableColumnEdit();
};

#endif
