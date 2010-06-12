/***************************************************************************
                          newtemplatedirdlg.h  -  description
                             -------------------
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

#ifndef NEWTEMPLATEDIRDLG_H
#define NEWTEMPLATEDIRDLG_H

#include "ui_templatedirform.h"

#include <kdialog.h>

/**
  *@author Andras Mantia
  */

class NewTemplateDirDlg : public KDialog, public Ui::TemplateDirForm
{
   Q_OBJECT
public:
  NewTemplateDirDlg(QWidget *parent=0);
  ~NewTemplateDirDlg();
};

#endif
