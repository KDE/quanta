/***************************************************************************
                          projectnewlocal.cpp  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2001, 2002 Andras Mantia <amantia@freemail.hu>
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
#include <qlabel.h>
#include <qfileinfo.h>
#include <qlistview.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qregexp.h>

// kde includes
#include <klocale.h>
#include <kiconloader.h>
#include <kfiledialog.h>
#include <kprogress.h>
#include <kmimetype.h>
#include <kdeversion.h>

//app includes
#include "projectnewlocal.h"
#include "projectnewgeneral.h"
#include "../dialogs/copyto.h"
#include "../qextfileinfo.h"
#include "../treeviews/uploadtreefolder.h"
#include "../treeviews/uploadtreeview.h"
#include "../resource.h"
#include "../quantacommon.h"

ProjectNewLocal::ProjectNewLocal(QWidget *parent, const char *name )
	: ProjectNewLocalS(parent,name)
{
	imagelabel->setPixmap( UserIcon("wiznewprjloc") );

//	listView->setColumnAlignment(1,Qt::AlignRight);
  listView->setColumnText(1, i18n("Add"));
  listView->removeColumn(2);
  listView->removeColumn(2);

	mask->setText("*");

	checkInsertWeb->setChecked( true );

	connect( checkInsert, SIGNAL(toggled(bool)), this, SLOT(slotSetFiles(bool)));
	connect( checkInsertWeb, SIGNAL(toggled(bool)), this, SLOT(slotSetFiles(bool)));
	connect( checkInsertWithMask, SIGNAL(toggled(bool)), this, SLOT(slotSetFiles(bool)));

	connect(addFiles, SIGNAL(clicked()),this,SLOT(slotAddFiles()));
	connect(addFolder, SIGNAL(clicked()),this,SLOT(slotAddFolder()));
	connect(clearList, SIGNAL(clicked()),this,SLOT(slotClearList()));
}

ProjectNewLocal::~ProjectNewLocal(){
}

void ProjectNewLocal::setBaseURL(const KURL& a_baseURL)
{
  baseURL = a_baseURL;
  baseURL.adjustPath(1);
	checkInsert->setText(i18n("Insert files from \"%1\"").arg(baseURL.prettyURL()));
  listView->clear();
  fileList.clear();
  checkInsert->setChecked(false);
}

KURL::List ProjectNewLocal::files()
{
  //return fileList;
	KURL::List r;

  QListViewItem *item;
  QListViewItemIterator it(listView);
  for ( ; it.current(); ++it )
  {
   item = it.current();
   if ( listView->isSelected( item ))
   {
     KURL u;
     if (dynamic_cast<UploadTreeFolder*>(item))
     {
      u = dynamic_cast<UploadTreeFolder*>(item)->url();
     } else
     {
      u = dynamic_cast<UploadTreeFile*>(item)->url();
     }

     if (!u.isEmpty()) r.append(u);
   }
  }
  return r;
}

KURL::List ProjectNewLocal::projectFiles()
{
	KURL::List list;

  if (!QExtFileInfo::exists(baseURL) ||
      !baseURL.path().endsWith("/")  ||
      !checkInsert->isChecked() ) return list;

	QString fmask = "*";
  if ( checkInsertWeb->isChecked() )
  {
    KMimeType::List list = KMimeType::allMimeTypes();
    KMimeType::List::iterator it;
    QString name;
    fmask = "";
    for ( it = list.begin(); it != list.end(); ++it )
    {
      name = (*it)->name();
      if (qConfig.markupMimeTypes.contains(name) ||
          qConfig.scriptMimeTypes.contains(name) ||
          qConfig.imageMimeTypes.contains(name))
      {
        QStringList patterns = (*it)->patterns();
        for (uint i = 0 ; i < patterns.count(); i++)
        {
          fmask = fmask+patterns[i]+" ";
        }
      }
    }
  } else
  {
    fmask = mask->text();
  }

	list = QExtFileInfo::allFilesRelative( baseURL, fmask);

	return list;
}

void ProjectNewLocal::slotSetFiles(bool)
{
	if ( checkInsert->isChecked() )
  {
  	KURL::List files = projectFiles();
    progressBar->setTotalSteps(files.count()-1);
    progressBar->setTextEnabled(true);
	  for (uint i = 0; i < files.count(); i++)
  	{
	     if ( !fileList.contains(files[i]) )
       {
         fileList.append(files[i]);
         QListViewItem *it = listView->addItem(files[i], "", "");
         if (it)  it->setSelected(true);
         progressBar->setValue(i);
       }
	  }
    progressBar->setValue(0);
    progressBar->setTextEnabled(false);
  }
}

void ProjectNewLocal::resizeEvent ( QResizeEvent *t )
{
  ProjectNewLocalS::resizeEvent(t);
  listView->setColumnWidth(0,listView->width()-listView->columnWidth(1)-20);
}

/** No descriptions */
void ProjectNewLocal::slotAddFiles()
{
  QExtFileInfo::createDir( baseURL );
	KURL::List list = KFileDialog::getOpenURLs(
		baseURL.url(),	i18n("*"), this, i18n("Insert Files in Project"));

	if ( !list.isEmpty() )
  {
  	KURL u = list.first();

  	u = QExtFileInfo::toRelative( u, baseURL );

  	if ( u.path().startsWith("..") || u.path().startsWith("/"))
  	{
  		CopyTo *dlg = new CopyTo( baseURL, this, i18n("Files: copy to project...") );

      	if ( dlg->exec() )
      	{
         		list = dlg->copy( list );
            connect(dlg, SIGNAL(addFilesToProject(const KURL&,CopyTo*)),
                         SLOT(slotInsertFilesAfterCopying(const KURL&,CopyTo*)));
            return;
       	} else
       	{
      		delete dlg;
         	 	return;
       	}
       	delete dlg;
  	}

    progressBar->setTotalSteps(list.count()-1);
    progressBar->setTextEnabled(true);
  	for (uint i = 0; i < list.count(); i++)
  	{
       list[i] = QExtFileInfo::toRelative(list[i], baseURL);
  	   if (!fileList.contains(list[i]))
       {
         fileList.append(list[i]);
         QListViewItem *it = listView->addItem(list[i], "", "");
         if (it)  it->setSelected(true);
         progressBar->setValue(i);
       }
  	}
   progressBar->setValue(0);
   progressBar->setTextEnabled(false);

  }
}

/** No descriptions */
void ProjectNewLocal::slotAddFolder()
{
//TODO/FIXME: This returns null if the selected directory is not on the local disk.
//I think this is a KDE bug
  QExtFileInfo::createDir( baseURL );
  KURL dirURL ;
  #if KDE_VERSION < 308
    QString dirName = KFileDialog::getExistingDirectory(
	   	baseURL.url(),  this, i18n("Insert Directory in Project"));
    QuantaCommon::setUrl(dirURL, dirName);
  #else
     dirURL = KFileDialog::getExistingURL(
	   	baseURL.url(),  this, i18n("Insert Directory in Project"));
  #endif

	if ( !dirURL.isEmpty() )
  {
    dirURL.adjustPath(1);

   	KURL sdir = dirURL;
    sdir = QExtFileInfo::toRelative( sdir, baseURL);

    if ( sdir.path().startsWith("..") || sdir.path().startsWith("/") )
    {
  	  CopyTo *dlg = new CopyTo( baseURL, this, i18n("%1: copy to project...").arg(dirURL.prettyURL()) );

      if ( dlg->exec() )
      {
        dirURL = dlg->copy(dirURL);
        connect(dlg, SIGNAL(addFilesToProject(const KURL&,CopyTo*)),
                     SLOT(slotInsertFilesAfterCopying(const KURL&,CopyTo*)));
        return;
      } else
      {
        delete dlg;
        return;
      }
      delete dlg;
    }

    slotInsertFilesAfterCopying(dirURL, 0);
  }
}

void ProjectNewLocal::slotInsertFilesAfterCopying(const KURL& rdir,CopyTo* dlg)
{
  KURL dirURL = rdir;
//The CopyTo dlg is deleted only here!!
  if (dlg) delete dlg;
  dirURL.adjustPath(1);
  KURL::List files = QExtFileInfo::allFilesRelative( dirURL, "*");
  progressBar->setTotalSteps(files.count()-1);
  progressBar->setTextEnabled(true);
  for (uint i = 0; i < files.count(); i++)
	{
     if ( !fileList.contains(files[i]) )
     {
       fileList.append(files[i]);
       QListViewItem *it = listView->addItem(files[i], "", "");
       if (it)  it->setSelected(true); 
       progressBar->setValue(i);
     }
  }
  progressBar->setValue(0);
  progressBar->setTextEnabled(false);
}

/** No descriptions */
void ProjectNewLocal::slotClearList()
{
  KIO::del( baseURL );
  listView->clear();
  fileList.clear();
  checkInsert->setChecked(false);
}
   
#include "projectnewlocal.moc"
