/***************************************************************************
                          newtemplatedirdlg.cpp  -  description
                             -------------------
    begin                : Fri Jun 21 2002
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

#include <qpushbutton.h>
#include "newtemplatedirdlg.h"

NewTemplateDirDlg::NewTemplateDirDlg(QWidget *parent, const char *name ) : TemplateDirForm(parent,name)
{
	setCaption(name);
  connect( buttonOk, SIGNAL(clicked()), SLOT(accept()) );
	connect( buttonCancel, SIGNAL(clicked()), SLOT(reject()) );
}

NewTemplateDirDlg::~NewTemplateDirDlg()
{
}
