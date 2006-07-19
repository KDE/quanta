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
#include <QButtonGroup>
#include <QLabel>
#include <qspinbox.h>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>

TagQuickListDlg::TagQuickListDlg(QWidget *parent)
    : QDialog(parent, true)
{
  setWindowTitle(name);

  initDialog();

  groupBox->addButton(radioOrdered);
  groupBox->addButton(radioUnordered);

  connect( buttonOk, SIGNAL(clicked()), SLOT(accept()) );
  connect( buttonCancel, SIGNAL(clicked()), SLOT(reject()) );
}

TagQuickListDlg::~TagQuickListDlg(){
}
