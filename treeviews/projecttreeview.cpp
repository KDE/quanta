/***************************************************************************
                          projecttreeview.cpp  -  description
                             -------------------
    begin                : Tue Mar 14 2000
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

// QT includes
#include <qdir.h>
#include <qpixmap.h>
#include <qheader.h>
#include <qpopupmenu.h>
#include <qstringlist.h>

// KDE includes
#include <krun.h>
#include <klocale.h>
#include <kio/job.h>
#include <kopenwith.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kpropsdlg.h>

// app includes
#include "projecttreefile.h"
#include "projecttreefolder.h"
#include "projecttreeview.h"

extern QString fileMaskHtml;
extern QString fileMaskJava;
extern QString fileMaskText;
extern QString fileMaskImage;

ProjectTreeView::ProjectTreeView(QWidget *parent, const char *name )
  : QListView(parent,name)
{
  setRootIsDecorated( true );
  header()->hide();
  setSorting( 0 );

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( "Name" );

	setFocusPolicy(QWidget::ClickFocus);
	
	projectDir =  new ProjectTreeFolder( this, "No project", basePath);
	projectDir -> setPixmap( 0, SmallIcon("folder"));
	projectDir -> setOpen( true );
	
	fileMenu = new QPopupMenu();
	
	fileMenu -> insertItem( UserIcon("open"),  i18n("&Open"), 		this ,SLOT(slotOpen()));
	fileMenu -> insertItem(					  			   i18n("Open with..."), 		this ,SLOT(slotOpenWith()));
	fileMenu -> insertItem(					  			   i18n("Open in Quanta"), 	this ,SLOT(slotOpenInQuanta()));
	fileMenu -> insertSeparator();
	fileMenu -> insertItem(	UserIcon("delete"),i18n("Remove from disc (and project)"), 	 this ,SLOT(slotRemove()));
	fileMenu -> insertItem(					  			   i18n("Remove from project"),this ,SLOT(slotRemoveFromProject(int)));
	fileMenu -> insertItem( i18n("Rename"),   this ,SLOT(slotRenameFile()));
	fileMenu -> insertSeparator();
	fileMenu -> insertItem(i18n("Upload file..."),this,SLOT(slotUploadSingleFile()));

	folderMenu = new QPopupMenu();
	
	folderMenu -> insertItem( UserIcon("open"),  i18n("&Open"), 		this ,SLOT(slotOpen()));
 	folderMenu -> insertSeparator();
	folderMenu -> insertItem(	UserIcon("delete"),i18n("Remove from disc (and project)"), 	 this ,SLOT(slotRemove()));
	folderMenu -> insertItem(					  			   i18n("Remove from project"),this ,SLOT(slotRemoveFromProject(int)));
 	folderMenu -> insertItem( i18n("Rename"),   this ,SLOT(slotRenameFolder()));
	folderMenu -> insertSeparator();
	folderMenu -> insertItem(i18n("Upload folder..."),this,SLOT(slotUploadSingleFolder()));


	connect(  this, SIGNAL(doubleClicked(QListViewItem *)),
						this, SLOT  (slotSelectFile(QListViewItem *)));
	connect(  this, SIGNAL(selectionChanged(QListViewItem *)),
						this, SLOT  (slotSelectImage(QListViewItem *)));
	connect(	this, SIGNAL(returnPressed(QListViewItem *)),
						this, SLOT  (slotSelectFile(QListViewItem *)));
	
	connect(  this, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)),
					  this, SLOT  (slotMenu(QListViewItem*, const QPoint&, int)));
					
	connect(	this, SIGNAL(open(QListViewItem *)),
						this,	SLOT(slotSelectFile(QListViewItem *)));
}

ProjectTreeView::~ProjectTreeView(){
}

QString ProjectTreeView::currentFileName()
{
	if ( !currentItem() ) return "";
	QListViewItem *item = currentItem();
	ProjectTreeFolder *parent = dynamic_cast<ProjectTreeFolder *> (item->parent());
	
	if ( !parent ) // top level element
		return ((ProjectTreeFolder *)item)->path;
	
	ProjectTreeFolder *f = dynamic_cast<ProjectTreeFolder *>(item);
	
	if ( f ) return f->path;
	else		 return parent->path + ((ProjectTreeFile *)item)->fname;
}

/** slot for right utton menu */
void ProjectTreeView::slotMenu(QListViewItem *item, const QPoint& point, int)
{
  if ( !item ) return;
  if ( item == projectDir ) return;

	setSelected(item, true);
	
	ProjectTreeFile *f = dynamic_cast<ProjectTreeFile *>( item);
	if ( f ) fileMenu->popup( point);
	
	ProjectTreeFolder *d = dynamic_cast<ProjectTreeFolder *>( item);
	if ( d ) 
	  if ( d->text(0) != "CVS") folderMenu->popup( point);
}

void ProjectTreeView::slotFileTag()
{
  ProjectTreeFile 	*f = (ProjectTreeFile 	*)currentItem();
  ProjectTreeFolder *d = (ProjectTreeFolder *)(f->parent());
	
	QString name = d->path + f->text(0);

  emit insertTag( name );
}

/** open file */
void ProjectTreeView::slotSelectFile(QListViewItem *item)
{
	if ( !item ) return;
	if ( !currentItem() ) return;
	ProjectTreeFolder *parent = (ProjectTreeFolder *) item->parent();
	if ( !parent ) return;
	if ( dynamic_cast<ProjectTreeFolder *>(item) ) return;
	
	QString nameToOpen = currentFileName();
	
	if ( QDir::match( fileMaskHtml+fileMaskJava+fileMaskText, nameToOpen) )
	{
		KURL url(nameToOpen);
		emit openFile( url );
		return;
	}
	else if ( QDir::match( fileMaskImage, nameToOpen) )
	{
		emit activatePreview();
		emit openImage( nameToOpen );
		return;
	}
	new KRun( KURL(nameToOpen), 0, true );
}

/** open image in preview */
void ProjectTreeView::slotSelectImage(QListViewItem *item)
{
	if ( !item ) return;
	if ( !currentItem() ) return;
	ProjectTreeFolder *parent = (ProjectTreeFolder *) item->parent();
	if ( !parent ) return;
	if ( dynamic_cast<ProjectTreeFolder *>(item) ) return;
	
	QString nameToOpen = parent->fullName( item )+item->text(0);
	
	if ( QDir::match( fileMaskImage, nameToOpen) )
	{
		emit openImage( nameToOpen );
	}
}

void ProjectTreeView::slotSetBasePath( QString dir )
{
	basePath = dir;
}

void ProjectTreeView::slotSetProjectName( QString name )
{
	projectName = name;
	projectDir -> setText( 0, name);
}

void ProjectTreeView::slotReloadTree( QStringList fileList, bool newtree, bool )
{
	if ( newtree )
	{
		if ( projectDir ) delete projectDir;
		projectDir =  new ProjectTreeFolder( this, projectName, basePath);
		projectDir -> setPixmap( 0, SmallIcon("folder"));
		projectDir -> setOpen( true );
	}
	
	projectDir -> setOpen( false );
	projectDir->setText( 0, projectName );
	
	int pos;
	QString fname;
	
	ProjectTreeFolder *newFolder = 0L;
	ProjectTreeFolder *folder = projectDir;
	
	QStringList::Iterator it;

  for ( it = fileList.begin(); it != fileList.end(); ++it )
	{
    folder = projectDir;
    fname = *it;
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
      	newFolder = new ProjectTreeFolder( folder, dir);
      	
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
    if ( neednew ) new ProjectTreeFile( folder, fname, fname );
  }

  projectDir->sortChildItems(0,true);
  projectDir->setOpen( true );
}

void ProjectTreeView::slotOpen()
{
	if ( !currentItem() ) return;
	
	QListViewItem *item =  currentItem();
	
	if ( !item ) return;
	if ( !currentItem() ) return;
	
	ProjectTreeFolder *f = dynamic_cast<ProjectTreeFolder *>(item);
	if ( f ) return;
	
	QString nameToOpen = currentFileName();
	
	if ( QDir::match( fileMaskHtml+fileMaskJava+fileMaskText, nameToOpen) )
	{
		KURL url(nameToOpen);
		emit openFile( url );
		return;
	}
	else if ( QDir::match( fileMaskImage, nameToOpen) )
	{
		emit activatePreview();
		emit openImage( nameToOpen );
		return;
	}
	new KRun( KURL(nameToOpen), 0, true );
}

void ProjectTreeView::slotOpenWith()
{
	if ( !currentItem() ) return;
	
	QStringList list;
	QString fileToOpen = currentFileName();
	list.append( fileToOpen );
	
	KFileOpenWithHandler *kfowh = new KFileOpenWithHandler();
	
	kfowh -> displayOpenWithDialog( KURL::List( list ) );
}

void ProjectTreeView::slotOpenInQuanta()
{
	if ( !currentItem() ) return;
	
	KURL url(currentFileName());
	
	emit openFile( url );
}

void ProjectTreeView::slotRemove()
{
	if ( !currentItem() ) return;
	
  if ( KMessageBox::warningYesNo(this,"Do you really want to remove \n"+currentFileName()+"\nfrom disk ?") != KMessageBox::Yes ) 
	  return;

	
	ProjectTreeFolder *d = dynamic_cast<ProjectTreeFolder *>( currentItem() );
	if ( d ) {
		KIO::del( KURL( currentFileName() ) );
		emit removeFolderFromProject( currentFileName() );
		delete( d );
		return;
	}
	
	ProjectTreeFile *f = dynamic_cast<ProjectTreeFile *>( currentItem() );
	if ( f ) {
		KIO::del( KURL( currentFileName() ) );
		emit removeFileFromProject( currentFileName() );
		delete( f );
		return;
	}
}

void ProjectTreeView::slotRemoveFromProject(int askForRemove)
{
	if ( !currentItem() ) return;
	
	if ( (askForRemove) &&
	      ( KMessageBox::warningYesNo(this,"Do you really want to remove \n"+currentFileName()+"\nfrom project ?") != KMessageBox::Yes ) )
		return;
	
	ProjectTreeFolder *d = dynamic_cast<ProjectTreeFolder *>( currentItem() );
	if ( d ) {
		emit removeFolderFromProject( currentFileName() );
		delete( d );
		return;
	}
	
	ProjectTreeFile *f = dynamic_cast<ProjectTreeFile *>( currentItem() );
	if ( f ) {
		emit removeFileFromProject( currentFileName() );
		delete( f );
		return;
	}
}

void ProjectTreeView::slotUploadSingleFile()
{
	if ( !currentItem() ) return;
	emit uploadSingleFile( currentFileName() );
}

void ProjectTreeView::slotRenameFile()
{
  if ( !currentItem() ) return;
  emit renameFileInProject(currentFileName());
}

void ProjectTreeView::slotRenameFolder()
{
  if ( !currentItem() ) return;
  emit renameFolderInProject(currentFileName());
}

void ProjectTreeView::slotUploadSingleFolder()
{
	if ( !currentItem() ) return;
	emit uploadSingleFolder( currentFileName() );
}
#include "projecttreeview.moc"
