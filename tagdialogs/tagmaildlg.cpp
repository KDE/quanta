/***************************************************************************
                          tagmaildlg.cpp  -  description
                             -------------------
    begin                : Fri Nov 26 1999
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
#include "tagmaildlg.h"

TagMailDlg::TagMailDlg(QString email, QWidget *parent, const char *name)
    : QDialog(parent,name,true)
{
	setCaption(name);
	
	initDialog();
	comboBoxMail->insertItem( email );
	
	connect( QPushButtonOk, SIGNAL(clicked()), SLOT(accept()) );
	connect( QPushButtonCancel, SIGNAL(clicked()), SLOT(reject()) );
}

TagMailDlg::~TagMailDlg(){
}
