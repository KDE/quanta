/***************************************************************************
                          projectnewgeneral.cpp  -  description
                             -------------------
    begin                : Fri Oct 27 2000
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

// qt includes
#include <qdir.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>

// kde includes
#include <kfiledialog.h>

#include "projectnewgeneral.h"

ProjectNewGeneral::ProjectNewGeneral(QWidget *parent, const char *name )
	: ProjectNewGeneralS(parent,name)
{
	connect( linePrjFile, SIGNAL(textChanged(const QString &)),
					 this,				SLOT(slotLinePrjFile(const QString &)));
	connect( linePrjName, SIGNAL(textChanged(const QString &)),
					 this,				SLOT(slotLinePrjFile(const QString &)));
	connect( linePrjDir,  SIGNAL(textChanged(const QString &)),
					 this,				SLOT(slotLinePrjFile(const QString &)));
	connect( buttonDir,		SIGNAL(clicked()),
					 this,				SLOT(slotButtonDir()));
	connect( linePrjName, SIGNAL(textChanged(const QString &)),
					 this,				SLOT(slotChangeNames(const QString &)));
}

ProjectNewGeneral::~ProjectNewGeneral(){
}

void ProjectNewGeneral::slotButtonDir()
{
	linePrjDir->setText( KFileDialog::getExistingDirectory(
		linePrjDir->text(),
		this,
		"Select project directory..."
	));
}

void ProjectNewGeneral::slotLinePrjFile( const QString & )
{
	if ( 	linePrjFile->text().isEmpty() ||
				linePrjName->text().isEmpty() ||
				linePrjDir ->text().isEmpty())
				emit enableNextButton( this, false );
	else	emit enableNextButton( this, true  );
}

void ProjectNewGeneral::slotChangeNames( const QString &text )
{
	int i;
	QString fname = text.lower();
	while( (i=fname.find(" ")) >=0 ) fname.remove(i,1);
	
	linePrjFile->setText( fname+".webprj" );
	linePrjDir ->setText( QDir::homeDirPath()+"/"+fname );
}

QString ProjectNewGeneral::type()
{
	if ( radioCvs  ->isChecked() ) return "CVS";
	if ( radioLocal->isChecked() ) return "Local";
	if ( radioWeb  ->isChecked() ) return "Web";
  return "Local";
}
