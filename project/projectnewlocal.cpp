/***************************************************************************
                          projectnewlocal.cpp  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    								 (C) 2001 Andras Mantia
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
#include <qpushbutton.h>
#include <qregexp.h>

// kde includes
#include <klocale.h>
#include <kiconloader.h>
#include	 <kfiledialog.h>

#include "projectnewlocal.h"
#include "projectnewgeneral.h"
#include "../dialogs/copyto.h"
#include "../qextfileinfo.h"
#include "../treeviews/projecttreefolder.h"
#include "../treeviews/projecttreefile.h"
#include "../resource.h"
#include "../quantacommon.h"

//TODO: Redo this, using KURLs and possibly the ProjectTreeView code.
ProjectNewLocal::ProjectNewLocal(QWidget *parent, const char *name )
	: ProjectNewLocalS(parent,name)
{
	imagelabel->setPixmap( UserIcon("wiznewprjloc") );

	listFiles->setColumnAlignment(1,Qt::AlignRight);

    QString tmpString;
    tmpString = fileMaskPhp + fileMaskHtml + fileMaskJava + fileMaskImage;
	webmask->setText(tmpString.simplifyWhiteSpace());
	webmask->setCursorPosition(0);
	mask->setText("*");

	projectDirTree = 0L;

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

void ProjectNewLocal::slotSetDestDir(QWidget *w,bool)
{
	ProjectNewGeneral *png = (ProjectNewGeneral *)w;
	
	QString dirName = png->linePrjDir->text();
  dir = KURL();
  QuantaCommon::setUrl(dir, dirName);
  dir.setPath(dir.path(1));
	
	checkInsert->setText(i18n("Insert files from %1").arg(dirName));
}

KURL::List ProjectNewLocal::files()
{
  return fileList;
}

KURL::List ProjectNewLocal::projectFiles(bool relative)
{
	KURL::List list;
	
	QFileInfo fi( dir.path() ); //TODO
	if ( !fi.exists() || !fi.isDir() || !checkInsert->isChecked() ) return list;

	QString fmask = "*";
	if ( checkInsertWeb->isChecked() ) fmask = webmask->text();
	if ( checkInsertWithMask->isChecked() ) fmask = mask->text();

    if (relative) list = QExtFileInfo::allFilesRelative( dir, fmask);
		else          list = QExtFileInfo::allFiles( dir, fmask);

	return list;
}

void ProjectNewLocal::slotSetFiles(bool)
{
	if ( !checkInsert->isChecked() ) return;
	KURL::List files = projectFiles(false);
	for (uint i = 0; i < files.count(); i++)
	{
	   if (fileList.find(files[i]) == fileList.end()) fileList.append(files[i]);
	 }
	slotReloadTree(fileList,true);
}

void ProjectNewLocal::resizeEvent ( QResizeEvent *t )
{
  ProjectNewLocalS::resizeEvent(t);
  listFiles->setColumnWidth(0,listFiles->width()-listFiles->columnWidth(1)-20);
}

/** No descriptions */
void ProjectNewLocal::slotAddFiles()
{
	KURL::List list = KFileDialog::getOpenURLs(
		dir.url(),	i18n("*"), this, i18n("Insert Files in Project"));

	if ( !list.isEmpty() )
  {
  	KURL u = list.first();

  	u = QExtFileInfo::toRelative( u, dir );

  	if ( u.path().startsWith("..") || u.path().startsWith("/"))
  	{
  		CopyTo *dlg = new CopyTo( dir, this, i18n("Files: copy to project...") );

      	if ( dlg->exec() )
      	{
         		list = dlg->copy( list );
       	} else
       	{
      		delete dlg;
         	 	return;
       	}
       	delete dlg;
  	}
  	
  	for (uint i = 0; i < list.count(); i++)
  	{
  	   if (fileList.find(list[i]) == fileList.end())
       {
         fileList.append(list[i]);
       }
  	}
    slotReloadTree(fileList,false);	
  }
}

/** No descriptions */
void ProjectNewLocal::slotAddFolder()
{
	QString dirName = KFileDialog::getExistingDirectory(
		dir.prettyURL(), this, i18n("Insert Directory in Project"));
	
	if ( !dirName.isEmpty() )
  {
	  KURL dirURL;
    QuantaCommon::setUrl(dirURL, dirName);
    dirURL.adjustPath(1);
	
   	KURL sdir = dirURL;
    sdir = QExtFileInfo::toRelative( sdir, dir);
	
    if ( sdir.path().startsWith("..") || sdir.path().startsWith("/") )
    {
  	  CopyTo *dlg = new CopyTo( dir, this, i18n("%1: copy to project...").arg(dirName) );

      if ( dlg->exec() )
      {
//      	if ( dirName.right(1) == "/" ) dirName.remove( dirName.length()-1,1);
//        dirName = dlg->copy( dirName );
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

    KURL::List files = QExtFileInfo::allFiles( dirURL, "*");	
    for (uint i = 0; i < files.count(); i++)
	  {
	    if (fileList.find(files[i]) == fileList.end()) fileList.append(files[i]);
	  }
    slotReloadTree(fileList,false);	
  }
}

void ProjectNewLocal::slotInsertFilesAfterCopying(const KURL& rdir,CopyTo* dlg)
{
//The CopyTo dlg is deleted only here!!
  delete dlg;	
  KURL dirURL = rdir;
  dirURL.adjustPath(1);
  KURL::List files = QExtFileInfo::allFiles( dirURL, "*");	
  for (uint i = 0; i < files.count(); i++)
  {
    if (fileList.find(files[i]) == fileList.end()) fileList.append(files[i]);
  }
  slotReloadTree(fileList,false);	
}

/** No descriptions */
void ProjectNewLocal::slotClearList()
{
  if (projectDirTree != 0L) delete projectDirTree;
  listFiles->clear();
  fileList.clear();
  checkInsert->setChecked(false);
  projectDirTree = 0L;
}

void ProjectNewLocal::slotReloadTree( KURL::List fileList, bool newtree)
{
	QString projectName = QFileInfo(dir.path(-1)).fileName();
	if ( (projectDirTree == 0L) || ( newtree )  )
	{
		if ( projectDirTree !=0L ) delete projectDirTree;
		projectDirTree =  new ProjectTreeFolder( listFiles, projectName, KURL(dir));//TODO
		projectDirTree -> setPixmap( 0, SmallIcon("folder"));
		projectDirTree -> setOpen( true );
	}

	projectDirTree -> setOpen( false );
	projectDirTree->setText( 0, projectName );

	int pos;
	QString fname;

	ProjectTreeFolder *newFolder = 0L;
	ProjectTreeFolder *folder = projectDirTree;
	
	KURL::List::Iterator it;

  for ( it = fileList.begin(); it != fileList.end(); ++it )
	{
    folder = projectDirTree;
    fname = QExtFileInfo::toRelative(*it,dir).path(1); //TODO
    fname.replace(QRegExp("\\.\\./"),"");
    while ( ( pos = fname.find('/')) > 0 )
    {
      QString dir = fname.left(pos);

      newFolder = 0L;
      QListViewItem *item = folder->firstChild();

      while( item )
      {
        if ( dir == item->text(0) )
        	newFolder = dynamic_cast<ProjectTreeFolder *>(item);
        item = item->nextSibling();
      }

      if ( !newFolder )
      	newFolder = new ProjectTreeFolder( folder, KURL(dir)); //TODO
      	
      folder = newFolder;
      fname.remove(0,pos+1);
    }
    QListViewItem *item = folder->firstChild();
    bool neednew = true;

    if (folder->text(0) == "CVS") neednew = false;

    while( item )
    {
      if ( fname == item->text(0) ) neednew = false;
      item = item->nextSibling();
    }
    if (fname.isEmpty()) neednew = false;
    if ( neednew )
    {
      ProjectTreeFile *item = new ProjectTreeFile( folder, fname, KURL(fname) ); //TODO
      item->setIcon(KURL(fname)); //TODO
    }
  }

  projectDirTree->sortChildItems(0,true);
  projectDirTree->setOpen( true );
}
#include "projectnewlocal.moc"
