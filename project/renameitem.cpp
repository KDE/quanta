/***************************************************************************
                          renameitem.cpp  -  description
                             -------------------
    begin                : Sat Nov 24 2001
    copyright            : (C) 2001 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//own includes
#include "renameitem.h"

// kde includes
#include <kio/job.h>
#include <klocale.h>

//qt includes
#include <qfileinfo.h>
#include <qlineedit.h>
#include <qpushbutton.h>

RenameItem::RenameItem(QWidget *parent,const char *name)
	:RenameDialog(parent,name,true)
{
  setCaption(i18n("Rename: %1").arg(name));
  NewNameEdit->setText(name);
  NewNameEdit->setFocus();

  connect( this->buttonOk,    SIGNAL(clicked()),   SLOT(accept()) );
  connect( buttonCancel,SIGNAL(clicked()),  SLOT(reject()) );
}

QString RenameItem::renameFile( QString oldName )
{
  QString newName = QFileInfo(oldName).dirPath() + '/'+NewNameEdit->text();
  if ( oldName != newName )
  {
    KIO::SimpleJob *job = KIO::rename( KURL( oldName ), KURL(  newName ), true );
    connect( job, SIGNAL(result( KIO::Job *)),
                 SLOT  (slotResult( KIO::Job *)));
  }

  return NewNameEdit->text();
}

QString RenameItem::renameFolder( QString oldName )
{
   //do this trick to get the path without the last directory name from oldName
   oldName.truncate(oldName.length()-1);
   QString newName = QFileInfo(oldName).dirPath() + '/'+NewNameEdit->text()+'/';
   oldName += '/';

  if ( oldName != newName )
  {
    KIO::SimpleJob *job = KIO::rename( KURL( oldName ), KURL(  newName ), true );
    connect( job, SIGNAL(result( KIO::Job *)),
                 SLOT  (slotResult( KIO::Job *)));
  }

  return newName;
}

void RenameItem::slotResult( KIO::Job *)
{
  emit renameFinished(this);
}


RenameItem::~RenameItem()
{
}
