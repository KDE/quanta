/***************************************************************************
                          projectnewgetfiles.cpp  -  description
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
#include "projectnewgetfiles.h"

// Qt includes
#include <qdir.h>

// KDE includes
#include <kiconloader.h>

ProjectNewGetFiles::ProjectNewGetFiles(QWidget *parent, const char *name)
	: QWidget(parent,name)
{
	initDialog();
	
	buttonEditRcFile->setPixmap( UserIcon("configure.xpm") );
	
	connect( buttonEditRcFile, SIGNAL(clicked()), SLOT(slotEditRcFile()));
}

ProjectNewGetFiles::~ProjectNewGetFiles(){
}

void ProjectNewGetFiles::slotEditRcFile()
{
//  KFM kfm;
/*
  QString fname = QDir::homeDirPath()+"/.wgetrc";

  QFileInfo fi( fname.data() );
  if ( !fi.exists() ) {
    QFileInfo fietc("/etc/wgetrc");
    if ( fietc.exists() )
      kfm.copy("/etc/wgetrc",fname.data());
    else
      KMsgBox(this,"Error","Don't found /etc/wgetrc\nwget don't installed.");
  }

  kfm.openURL( fname.data() );*/
}

