/***************************************************************************
    begin                : Fri Jun 21 2002
    copyright            : (C) 2002, 2006 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include "newtemplatedirdlg.h"
#include "newtemplatedirdlg.moc"

NewTemplateDirDlg::NewTemplateDirDlg(QWidget *parent)
    : KDialog(parent)
{
    setButtons( KDialog::Ok | KDialog::Cancel );
    setDefaultButton( KDialog::Ok );
  QWidget *w = new QWidget(this);
  setupUi(w);
  setMainWidget(w);
}

NewTemplateDirDlg::~NewTemplateDirDlg()
{
}
