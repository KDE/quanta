/***************************************************************************
                          toolbarsdlg.h  -  description
                             -------------------
    begin                : Fri Jul 19 2002
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

#ifndef TOOLBARSDLG_H
#define TOOLBARSDLG_H

#include "edittoolbars.h"

/**
  *@author Andras Mantia
  */

class ToolBarsDlg : public EditToolbars  {
   Q_OBJECT
public: 
	ToolBarsDlg(QWidget *parent=0, const char *name=0);
	~ToolBarsDlg();
};

#endif
