/***************************************************************************
                          copytodlg.cpp  -  description
                             -------------------
    begin                : Mon Mar 27 2000
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
// qt includes
#include <qdir.h>
#include <qlineedit.h>
#include <qpushbutton.h>

// kde includes
#include <kio/job.h>
#include <kio/jobclasses.h>
#include <kfiledialog.h>
#include <kiconloader.h>
#include <kurlrequester.h>

//app includes
#include "copyto.h"

CopyTo::CopyTo(QString dir, QWidget *parent, const char *name)
    : CopyToS(parent,name,true)
{

	mInitialDir = dir;

	urlRequester->setMode( KFile::Directory | KFile::ExistingOnly);
	urlRequester->setURL( dir );
	urlRequester->setFocus();
	
	connect( buttonOk,    SIGNAL(clicked()), SLOT(accept()) );
	connect( buttonCancel,SIGNAL(clicked()), SLOT(reject()) );
}

CopyTo::~CopyTo(){
}

QString CopyTo::copy( QString rname )
{
  if ( urlRequester->url().isEmpty() )
	  urlRequester->setURL(mInitialDir);
	
  QString path = urlRequester->url();

  if ( path.right(1) != "/" ) path.append("/");

  int i = 10;
  QDir dir( path);
  while ( !dir.exists() && i-- ) dir.mkdir( path);

  QString sname = rname;
  while ( ( i=sname.find('/')) >= 0 ) sname.remove(0,i+1);

  fname = path + sname;
  if ( rname != fname ) 
  {
    KIO::CopyJob *job = KIO::copy( KURL( rname ), KURL( fname ), true );
//    connect( job, SIGNAL(copyingDone( KIO::Job *,const KURL&,const KURL&,bool,bool)),
//                  SLOT  (endCopy( KIO::Job *,const KURL&,const KURL&,bool,bool)));
    connect( job, SIGNAL(result( KIO::Job *)),
                 SLOT  (slotResult( KIO::Job *)));
  }

  return fname;
}

void CopyTo::endCopy( KIO::Job *,const KURL&,const KURL&, bool, bool)
{
  emit addFilesToProject(fname,this);
}

void CopyTo::slotResult( KIO::Job *)
{
  emit addFilesToProject(fname,this);
}

QStringList CopyTo::copy( QStringList rfiles )
{
  QString path = urlRequester->url();

  if ( path.right(1) != "/" ) path.append("/");

  int i = 10;
  QDir dir( path);
  while ( !dir.exists() && i-- ) dir.mkdir( path);

  QStringList::Iterator it;
  QStringList sfiles = rfiles;

  for ( it = sfiles.begin(); it != sfiles.end(); ++it )
	{
		while ( ( i=(*it).find('/')) >= 0 ) (*it).remove(0,i+1);
		(*it) = path + (*it);
	}

  if ( rfiles != sfiles ) 
  {
    KIO::copy( KURL::List( rfiles ), KURL( path ), true );
  }

  return sfiles;
}
#include "copyto.moc"
