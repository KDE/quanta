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
#include <kio/netaccess.h>
#include <kfiledialog.h>
#include <kiconloader.h>
#include <kurlrequester.h>

//app includes
#include "copyto.h"
#include "../quantacommon.h"
#include "../qextfileinfo.h"

CopyTo::CopyTo(const KURL& dirURL, QWidget *parent, const char *name)
    : CopyToS(parent,name,true)
{

	mInitialDirUrl = dirURL;

	urlRequester->setMode( KFile::Directory | KFile::ExistingOnly);
	urlRequester->setURL( dirURL.prettyURL() );
	urlRequester->setFocus();
	
	connect( buttonOk,    SIGNAL(clicked()), SLOT(accept()) );
	connect( buttonCancel,SIGNAL(clicked()), SLOT(reject()) );
}

CopyTo::~CopyTo(){
}

KURL CopyTo::copy(const KURL& urlToCopy)
{
  KURL targetDirURL = KURL();
  if ( urlRequester->url().isEmpty() )
  {
    targetDirURL = mInitialDirUrl;
  } else
  {
	  QuantaCommon::setUrl(targetDirURL, urlRequester->url());
  }
  targetDirURL.adjustPath(1);

  bool doCopy = true;
  if (!QExtFileInfo::exists(targetDirURL))
  {
    doCopy = QExtFileInfo::createDir(targetDirURL);
  }

  KURL destURL;
  if (doCopy)
  {
    destURL = targetDirURL;
    destURL.setPath(destURL.path(1)+urlToCopy.fileName());

    KIO::CopyJob *job = KIO::copy( urlToCopy, destURL, true );
    connect( job, SIGNAL(result( KIO::Job *)),
                  SLOT  (slotResult( KIO::Job *)));

    copiedURL = destURL;
  }
  
  return destURL;
}

void CopyTo::endCopy( KIO::Job *,const KURL&,const KURL&, bool, bool)
{
  emit addFilesToProject(copiedURL,this);
}

void CopyTo::slotResult( KIO::Job *)
{
  emit addFilesToProject(copiedURL,this);
}

KURL::List CopyTo::copy( KURL::List sourceList )
{
  KURL targetDirURL = KURL();
  if ( urlRequester->url().isEmpty() )
  {
    targetDirURL = mInitialDirUrl;
  } else
  {
	  QuantaCommon::setUrl(targetDirURL, urlRequester->url());
  }
  bool doCopy = true;
  if (!QExtFileInfo::exists(targetDirURL))
  {
    doCopy = QExtFileInfo::createDir(targetDirURL);
  }

  KURL::List destList;
  if (doCopy)
  {
    destList = sourceList;
    KURL::List::iterator it;
    for (it = destList.begin(); it != destList.end(); ++it)
    {
      (*it).setPath(targetDirURL.path(1)+(*it).fileName());
    }

    KIO::copy(sourceList, targetDirURL, true);
  }
  
  return destList;

}
#include "copyto.moc"
