/***************************************************************************
                          filecombo.cpp  -  description
                             -------------------
    begin                : Wed Sep 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// QT includes
#include <qlayout.h>
#include <qcombobox.h>
#include <qpushbutton.h>

// KDE includes
#include <klocale.h>
#include <kfiledialog.h>

// app include
#include "filecombo.h"
#include "filecombo.moc"
#include "../qextfileinfo.h"

FileCombo::FileCombo(QString basePath, QWidget *parent, const char *name )
  :QWidget(parent,name)
{
  this->basePath = basePath;

  QHBoxLayout *layout = new QHBoxLayout(this);

  combo   = new QComboBox(true,this);
  button  = new QPushButton(this);

  button  ->setFixedSize(35,25);
  button  ->setText(i18n("..."));

  layout  ->addWidget( combo );
  layout  ->addWidget( button );

  connect( button, SIGNAL(clicked()), this, SLOT(slotFileSelect()) );
}

FileCombo::FileCombo( QWidget *parent, const char *name )
   :QWidget( parent, name )
{
  this->basePath = ".";

  QHBoxLayout *layout = new QHBoxLayout(this);

  combo   = new QComboBox(true,this);
  button  = new QPushButton(this);

  button  ->setFixedSize(35,25);
  button  ->setText(i18n("..."));

  layout  ->addWidget( combo );
  layout  ->addWidget( button );

  connect( button, SIGNAL(clicked()), this, SLOT(slotFileSelect()) );
}

FileCombo::~FileCombo(){
}

QString FileCombo::text()
{
  return combo->currentText();
}

void FileCombo::setText( QString _txt )
{
  combo ->setEditText( _txt );
}

void FileCombo::slotFileSelect()
{
 		QString fileName = KFileDialog::getOpenFileName(
 		  basePath, i18n("*|All files"));

		if ( fileName.isEmpty() ) return;

		QExtFileInfo file(fileName);
		file.convertToRelative( basePath );

		QString text = file.filePath();
		combo  ->setEditText( text );
}
