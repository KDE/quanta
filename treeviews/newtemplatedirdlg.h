/***************************************************************************
                          newtemplatedirdlg.h  -  description
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

#ifndef NEWTEMPLATEDIRDLG_H
#define NEWTEMPLATEDIRDLG_H

#include <qwidget.h>
#include <templatedirform.h>

/**
  *@author Andras Mantia
  */

class NewTemplateDirDlg : public TemplateDirForm  {
   Q_OBJECT
public: 
  NewTemplateDirDlg(QWidget *parent=0, const char *name=0);
  ~NewTemplateDirDlg();
};

#endif
