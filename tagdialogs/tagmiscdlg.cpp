/***************************************************************************
                          tagmiscdlg.cpp  -  description
                             -------------------
    begin                : Thu Dec 13 2001
    copyright            : (C) 2001 by Andras Mantia
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

#include "tagmiscdlg.h"

//kde includes
#include <kapp.h>

//qt includes
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcheckbox.h>

TagMiscDlg::TagMiscDlg(QString element, bool closeTag, QWidget *parent, const char *name)
	:TagMisc(parent,name,true)
{
  setCaption(name);
  connect( buttonOk,     SIGNAL(clicked()), SLOT(accept()) );
  connect( buttonCancel, SIGNAL(clicked()), SLOT(reject()) );
  connect( buttonHelp,SIGNAL(clicked()),this,SLOT(slotShowHelp()));

  elementName->setFocus();
  elementName->setText(element);
  addClosingTag->setChecked(closeTag);
}

TagMiscDlg::~TagMiscDlg()
{
}

void TagMiscDlg::slotShowHelp()
{
   kapp->invokeHelp("tag-misc","quanta");
}

