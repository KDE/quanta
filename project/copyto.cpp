/***************************************************************************
                          copytodlg.cpp  -  description
                             -------------------
    begin                : Mon Mar 27 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
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
#include "copyto.h"
#include <kfiledialog.h>
#include <kiconloader.h>

CopyTo::CopyTo(QString dir, QWidget *parent, const char *name)
    : QDialog(parent,name,true)
{
  resize( 360,90 );
	initDialog();
	setCaption(name);
	
	lineDir->setText( dir.data() );
	buttonDir->setPixmap( UserIcon("open") );
	
	connect( buttonOk,    SIGNAL(clicked()), SLOT(accept()) );
	connect( buttonCancel,SIGNAL(clicked()), SLOT(reject()) );
	connect( buttonDir,   SIGNAL(clicked()), SLOT(slotDirChange()));
}

CopyTo::~CopyTo(){
}

/** change dir */
void CopyTo::slotDirChange()
{
  QString dir = lineDir->text();
  dir = KFileDialog::getExistingDirectory( dir, this);
  if ( !dir.isEmpty() )
      lineDir->setText( dir );
}
