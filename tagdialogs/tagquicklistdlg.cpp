/***************************************************************************
                          tagquicklistdlg.cpp  -  description
                             -------------------
    begin                : Sun Nov 28 1999
    copyright            : (C) 1999 by Yacovlev Alexander & Dmitry Poplavsky
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
#include "tagquicklistdlg.h"
#include "tagquicklistdlg.moc"
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qgroupbox.h>

TagQuickListDlg::TagQuickListDlg(QWidget *parent, const char *name)
    : QDialog(parent,name,true)
{
  setCaption(name);

  initDialog();

  groupBox->insert(radioOrdered);
  groupBox->insert(radioUnordered);

  connect( buttonOk, SIGNAL(clicked()), SLOT(accept()) );
  connect( buttonCancel, SIGNAL(clicked()), SLOT(reject()) );
}

TagQuickListDlg::~TagQuickListDlg(){
}
