/***************************************************************************
                          projectnewgeneral.cpp  -  description
                             -------------------
    begin                : Tue Aug 22 2000
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
#include "projectnewgeneral.h"

// QT includes
#include <qdir.h>

// KDE includes
#include <kiconloader.h>
#include <kfiledialog.h>

ProjectNewGeneral::ProjectNewGeneral(QWidget *parent, const char *name)
	: QWidget(parent,name)
{
	initDialog();
	
	buttonDir				->setPixmap( UserIcon("open.xpm") );
	buttonImagesDir	->setPixmap( UserIcon("open.xpm") );
	buttonCgiDir		->setPixmap( UserIcon("open.xpm") );
	buttonScriptsDir->setPixmap( UserIcon("open.xpm") );
	
	connect( linePrjName, SIGNAL(textChanged(const QString&)),
	         this, SLOT(slotNameChanged(const QString&)) );
	connect( buttonDir,   			SIGNAL(clicked()), SLOT(slotDirChange()));
	connect( buttonImagesDir,   SIGNAL(clicked()), SLOT(slotImagesDirChange()));
	connect( buttonCgiDir,   		SIGNAL(clicked()), SLOT(slotCgiDirChange()));
	connect( buttonScriptsDir,	SIGNAL(clicked()), SLOT(slotScriptsDirChange()));
	
	QString dir = QDir::homeDirPath()+"/";
  linePrjDir ->setText( dir );
}

ProjectNewGeneral::~ProjectNewGeneral(){
}

/** chenge file name of project */
void ProjectNewGeneral::slotNameChanged(const QString &text)
{
  int pos;
  QString name = linePrjName->text();
  QString fname = name.lower();
  while ( (pos = fname.find(' ')) >= 0) fname.remove(pos,1);
  fname += ".webprj";
  if ( fname != linePrjFile->text() ) linePrjFile->setText( fname );
}

/** change dir name */
void ProjectNewGeneral::slotDirChange()
{
  QString dir = linePrjDir->text();
  dir = KFileDialog::getExistingDirectory( dir, this, "Select Directory");
  linePrjDir->setText( dir );
}

/** change dir name */
void ProjectNewGeneral::slotImagesDirChange()
{
  QString dir = lineImagesDir->text();
  dir = KFileDialog::getExistingDirectory( dir, this, "Select Directory");
  lineImagesDir->setText( dir );
}

/** change dir name */
void ProjectNewGeneral::slotCgiDirChange()
{
  QString dir = lineCgiDir->text();
  dir = KFileDialog::getExistingDirectory( dir, this, "Select Directory");
  lineCgiDir->setText( dir );
}

/** change dir name */
void ProjectNewGeneral::slotScriptsDirChange()
{
  QString dir = lineScriptsDir->text();
  dir = KFileDialog::getExistingDirectory( dir, this, "Select Directory");
  lineScriptsDir->setText( dir );
}
