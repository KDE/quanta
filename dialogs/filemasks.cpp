/***************************************************************************
                          filesmask.cpp  -  description
                             -------------------
    begin                : Fri May 19 2000
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
#include "filemasks.h"

// qt includes
#include <qlineedit.h>
#include <qpushbutton.h>

FileMasks::FileMasks(QWidget *parent, const char *name)
	:FileMasksS(parent,name)
{
	connect(buttonDefault, SIGNAL(clicked()), this, SLOT(setToDefault()));
}

FileMasks::~FileMasks(){
}
/** set masks to default */
void FileMasks::setToDefault()
{
   lineHTML->setText("*.*html *.*htm *.php* *.asp *.cfm *.css *.inc* *.*HTML *.*HTM *.PHP* *.ASP *.CFM *.CSS *.INC* ");
   linePHP->setText( "*.php* *.PHP*" );
   lineImages->setText("*.gif *.jpg *.png *.jpeg *.bmp *.GIF *.JPG *.PNG *.JPEG *.BMP ");
   lineText->setText( "*.txt *.TXT " );
}
