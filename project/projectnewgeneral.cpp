/***************************************************************************
                          projectnewgeneral.cpp  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    								  (C)	 2001 by Andras Mantia
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
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>

// kde includes
#include <kfiledialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include "projectnewgeneral.h"

ProjectNewGeneral::ProjectNewGeneral(QWidget *parent, const char *name )
	: ProjectNewGeneralS(parent,name)
{
	imagelabel->setPixmap( UserIcon("wiznewprjglb") );
	linePrjName->setFocus();
	
	connect( linePrjFile, SIGNAL(textChanged(const QString &)),
					 this,				SLOT(slotLinePrjFile(const QString &)));
	connect( linePrjName, SIGNAL(textChanged(const QString &)),
					 this,				SLOT(slotLinePrjFile(const QString &)));
	connect( linePrjDir,  SIGNAL(textChanged(const QString &)),
					 this,				SLOT(slotLinePrjFile(const QString &)));
  connect( linePrjDir,  SIGNAL(textChanged(const QString &)),
           this,        SLOT(slotLinePrjTmpl(const QString &)));
	connect( buttonDir,		SIGNAL(clicked()),
					 this,				SLOT(slotButtonDir()));
	connect( linePrjName, SIGNAL(textChanged(const QString &)),
					 this,				SLOT(slotChangeNames(const QString &)));
  connect( linePrjTmpl, SIGNAL(textChanged(const QString &)), SLOT(slotLinePrjFile(const QString &)));
  connect( buttonTmpl,  SIGNAL(clicked()), SLOT(slotButtonTmpl()));
  connect( linePrjToolbar, SIGNAL(textChanged(const QString &)), SLOT(slotLinePrjFile(const QString &)));
  connect( buttonToolbar,  SIGNAL(clicked()), SLOT(slotButtonToolbar()));
}

ProjectNewGeneral::~ProjectNewGeneral(){
}

void ProjectNewGeneral::slotButtonDir()
{
	linePrjDir->setText( KFileDialog::getExistingDirectory(
		linePrjDir->text(),
		this,
		i18n("Select Project Directory")
	));
}

void ProjectNewGeneral::slotLinePrjFile( const QString & )
{
	if ( 	linePrjFile->text().isEmpty() ||
				linePrjName->text().isEmpty() ||
				linePrjDir ->text().isEmpty() ||
        linePrjTmpl->text().isEmpty() ||
        linePrjToolbar->text().isEmpty())
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

	emit setBasePath(linePrjDir->text());
}

QString ProjectNewGeneral::type()
{
	if ( radioCvs  ->isChecked() ) return "CVS";
	if ( radioLocal->isChecked() ) return "Local";
	if ( radioWeb  ->isChecked() ) return "Web";
  return "Local";
}

void ProjectNewGeneral::setMargin(int i)
{
  layout()->setMargin(i);
}

void ProjectNewGeneral::slotButtonTmpl()
{
  linePrjTmpl->setText( KFileDialog::getExistingDirectory(linePrjTmpl->text(), this,
  i18n("Select Project Template Directory")));
}

void ProjectNewGeneral::slotButtonToolbar()
{
  linePrjToolbar->setText( KFileDialog::getExistingDirectory(linePrjToolbar->text(), this,
  i18n("Select Project Toolbar & Actions Directory")));
}

void ProjectNewGeneral::slotLinePrjToolbar(const QString &Str)
{
  linePrjToolbar->setText(Str + "/toolbars");
}

void ProjectNewGeneral::slotLinePrjTmpl(const QString &Str)
{
  linePrjTmpl->setText(Str + "/templates");
}


#include "projectnewgeneral.moc"


