/***************************************************************************
                          newtemplatedirdlg.cpp  -  description
                             -------------------
    begin                : Fri Jun 21 2002
    copyright            : (C) 2002 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include <qpushbutton.h>
#include "newtemplatedirdlg.h"
#include "newtemplatedirdlg.moc"

NewTemplateDirDlg::NewTemplateDirDlg(QWidget *parent, const char *name ) : TemplateDirForm(parent,name)
{
  setCaption(name);
  connect( buttonOk, SIGNAL(clicked()), SLOT(accept()) );
  connect( buttonCancel, SIGNAL(clicked()), SLOT(reject()) );
}

NewTemplateDirDlg::~NewTemplateDirDlg()
{
}
