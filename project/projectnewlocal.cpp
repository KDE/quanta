/***************************************************************************
                          projectnewlocal.cpp  -  description
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
#include <qfileinfo.h>
#include <qlistview.h>
#include <qlineedit.h>
#include <qcheckbox.h>

// kde includes
#include <klocale.h>

#include "projectnewlocal.h"
#include "projectnewgeneral.h"
#include "../qextfileinfo.h"

extern QString fileMaskPhp;
extern QString fileMaskHtml;
extern QString fileMaskJava;
extern QString fileMaskImage;

ProjectNewLocal::ProjectNewLocal(QWidget *parent, const char *name )
	: ProjectNewLocalS(parent,name)
{
	mask->setText("*");
	webmask->setText( fileMaskPhp+
										fileMaskHtml+
										fileMaskJava+
										fileMaskImage);
									
	checkInsertWeb->setChecked( true );
										
	connect( checkInsert, SIGNAL(toggled(bool)),
					 this, 				SLOT(setFiles(bool)));
	connect( checkInsertWeb, SIGNAL(toggled(bool)),
					 this, 				   SLOT(setFiles(bool)));
	connect( checkInsertWithMask, SIGNAL(toggled(bool)),
					 this, 				        SLOT(setFiles(bool)));
}

ProjectNewLocal::~ProjectNewLocal(){
}

void ProjectNewLocal::setDestDir(QWidget *w,bool)
{
	ProjectNewGeneral *png = (ProjectNewGeneral *)w;
	
	dir = png->linePrjDir->text();
	
	checkInsert->setText(i18n("Insert files from ")+dir);
}

QStringList ProjectNewLocal::files(bool relative)
{
	QStringList list;
	
	QFileInfo fi( dir );
	if ( !fi.exists() || !fi.isDir() || !checkInsert->isChecked() ) return list;
	
	QString fmask = "*";
	if ( checkInsertWeb->isChecked() ) fmask = webmask->text();
	if ( checkInsertWithMask->isChecked() ) fmask = mask->text();
	
	if (relative) list = QExtFileInfo::allFilesRelative( dir, fmask);
	else          list = QExtFileInfo::allFiles( dir, fmask);
	
	return list;
}

void ProjectNewLocal::setFiles(bool)
{
	listFiles->clear();
	
	if ( !checkInsert->isChecked() ) return;
	
	QStringList list = files(true);
	for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
	{
		listFiles->insertItem( new QListViewItem( listFiles, *it ) );
  }
}
