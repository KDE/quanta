/***************************************************************************
                          toolbarsdlg.cpp  -  description
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

//qt includes

#include "toolbarsdlg.h"

#include <qpushbutton.h>

ToolBarsDlg::ToolBarsDlg(QWidget *parent, const char *name ) : EditToolbars(parent,name)
{
	setCaption(name);
  connect( buttonOk,     SIGNAL(clicked()), SLOT(accept()) );
  connect( buttonCancel, SIGNAL(clicked()), SLOT(reject()) );
}

ToolBarsDlg::~ToolBarsDlg(){
}
#include "toolbarsdlg.moc"
