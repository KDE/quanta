/***************************************************************************
                          tagadlg.cpp  -  description
                             -------------------
    begin                : Sat Nov 27 1999
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
#include "tagadlg.h"

#include <kapp.h>
#include <kiconloader.h>
#include <kfiledialog.h>

#include "../qextfileinfo.h"


TagADlg::TagADlg(QWidget *parent, const char *name)
  : TagWidget(parent,name)
{
	setCaption(name);
	initDialog();
	buttonHREF  ->setPixmap( UserIcon("open.xpm") );
	
	connect( buttonHREF, SIGNAL(clicked()), this, SLOT(slotFileSelect()) );
}

TagADlg::~TagADlg(){
}

/**  */
void TagADlg::slotFileSelect()
{
		QString fileName = KFileDialog::getOpenFileName( basePath, "*.*htm* *.php* *.asp *.cfm *.css *.jss *.js *.inc* *.*HTM* *.PHP* *.ASP *.CFM *.CSS *.JSS *.JS *.INC*|HTML files\n*|All files");
		if (fileName.isEmpty()) return;
		
		QExtFileInfo file(fileName);
		file.convertToRelative( basePath );
		QString shortName = file.filePath();
		comboHREF->setEditText(shortName);
}

void TagADlg::readAttributes( QDict<char> *d )
{
  dict = d;

  updateDict("href",  comboHREF );
  updateDict("target",comboTarget );
  updateDict("name",  lineName );
}

void TagADlg::writeAttributes( QDict<char> *d )
{
  dict = d;
  char *t; // value of attr.

  if (( t=d->find("href") ))    setValue(t, comboHREF);
  if (( t=d->find("target") ))  setValue(t, comboTarget);
  if (( t=d->find("name") ))    setValue(t, lineName);

  lineDescription->setEnabled(false);
}

