/***************************************************************************
                          quicktablecolumnedit.cpp  -  description
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

#include "quicktablecolumnedit.h"

QuickTableColumnEdit::QuickTableColumnEdit(QWidget *parent, const char *name ) : TableColumnDialog(parent,name)
{
	setCaption(name);
  connect( buttonOk, SIGNAL(clicked()), SLOT(accept()) );
	connect( buttonCancel, SIGNAL(clicked()), SLOT(reject()) );
}

QuickTableColumnEdit::~QuickTableColumnEdit(){
}
