/***************************************************************************
                          filesmask.cpp  -  description
                             -------------------
    begin                : Fri May 19 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2002 Andras Mantia <amantia@kde.org>
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
#include "filemasks.moc"
// qt includes
#include <qlineedit.h>
#include <qpushbutton.h>

//kde includes
#include <kmimetype.h>

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
 KMimeType::List list = KMimeType::allMimeTypes();
 KMimeType::List::iterator it;
 QString markup, script, image, text;
 markup = "text/css; text/html; text/sgml; text/xml; ";
 script = "text/x-perl; text/x-python; text/x-php; text/x-java; text/x-asp; ";
 QString name;
 for ( it = list.begin(); it != list.end(); ++it )
 {
    name = (*it)->name();
    if (name.contains("text"))
    {
      if (!markup.contains(name) && !script.contains(name))
         text += name+"; ";
    }
    if (name.contains("image"))
    {
      image += name +"; ";
    }
 }
 lineMarkup->setText(markup);
 lineScript->setText(script);
 lineImage->setText(image);
 lineText->setText(text);

/*
   lineHTML->setText("*.*html *.*htm *.php* *.asp *.cfm *.css *.inc* *.*HTML *.*HTM *.PHP* *.ASP *.CFM *.CSS *.INC* ");
   linePHP->setText( "*.php* *.PHP*" );
   lineImages->setText("*.gif *.jpg *.png *.jpeg *.bmp *.GIF *.JPG *.PNG *.JPEG *.BMP ");
   lineText->setText( "*.txt *.TXT " );
   */
}
