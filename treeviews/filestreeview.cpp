/***************************************************************************
                          filestreeview.cpp  -  description
                             -------------------
    begin                : Thu Jun 1 2000
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

#define ID_TOP  777

// QT includes
#include <qdir.h>
#include <qpixmap.h>
#include <qheader.h>

// KDE includes
#include <krun.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kopenwith.h>
#include <kmimetype.h>
#include <kmessagebox.h>

// app includes
#include "filemanage.h"
#include "filestreefile.h"
#include "filestreefolder.h"
#include "filestreeview.h"

extern QString fileMaskHtml;
extern QString fileMaskJava;
extern QString fileMaskText;
extern QString fileMaskImage;

FilesTreeView::FilesTreeView()
{
}

FilesTreeView::FilesTreeView( QString dir, QStrList topList, QWidget *parent, const char *name)
	: FileManage(parent,name)
{
	this->dirList = topList;	
	
	setRootIsDecorated( true );
  header()->hide();
  setSorting( 0 );

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( "Name" );

	setFocusPolicy(QWidget::ClickFocus);

	rootDir = new FilesTreeFolder( this , "Root Directory", "/");
	rootDir->setPixmap( 0, SmallIcon("folder"));
	rootDir->setOpen( false);
  	
 	homeDir = new FilesTreeFolder( this , "Home Directory", dir);
	homeDir->setPixmap( 0, SmallIcon("folder_home"));
	homeDir->setOpen( true );
	
	fileMenu -> insertItem( i18n("List mode"), this ,SLOT(slotNewMode()));
	
	folderMenu -> insertItem( i18n("List mode"), this ,SLOT(slotNewMode()));
	folderMenu -> insertItem( i18n("Add folder to top"), this ,SLOT(slotAddToTop()), 0, ID_TOP, 0);
	
	connect(  this, SIGNAL(doubleClicked(QListViewItem *)),
						this, SLOT(slotSelectFile(QListViewItem *)));
	connect(  this, SIGNAL(selectionChanged(QListViewItem *)),
						this, SLOT(slotSelectImage(QListViewItem *)));
	connect(	this, SIGNAL(returnPressed(QListViewItem *)),
						this, SLOT(slotSelectFile(QListViewItem *)));
						
	connect( this, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)),
					 this, SLOT(slotMenu(QListViewItem*, const QPoint&, int)));
					
	connect(	this, SIGNAL(open(QListViewItem *)),
						this,	SLOT(slotSelectFile(QListViewItem *)));
	connect(	this, SIGNAL(openInQuanta(QListViewItem *)),
						this,	SLOT(slotSelectAnyFile(QListViewItem *)));
						
	// generate top list of directories
	for ( topList.first(); topList.current(); topList.next() )
	{
	  QString f = topList.current();
	  QString fname = f;
    int i;
    while ( ( i = f.find('/')) >= 0 ) f.remove(0,i+1);
	
	  FilesTreeFolder *dir = new FilesTreeFolder( this , " "+f, fname+"/");
	  dir->setPixmap( 0, SmallIcon("kdisknav") );
	  dir->setOpen( false);
	}
}

FilesTreeView::~FilesTreeView()
{
}

void FilesTreeView::slotNewMode()
{
	emit changeMode();
}

void FilesTreeView::slotReload()
{

  FilesTreeFolder *d = dynamic_cast<FilesTreeFolder *>( currentItem()->parent());

  if (d)
    d->reloadList();

}

void FilesTreeView::slotMenu(QListViewItem *item, const QPoint &point, int)
{
	if ( !item ) return;
	setSelected(item, true);
	
	FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
	if ( f ) fileMenu->popup( point);
	
	FilesTreeFolder *d = dynamic_cast<FilesTreeFolder *>( item);
	if ( d )
	{
	  if ( !d->parentFolder )folderMenu ->changeItem( ID_TOP, i18n("Remove from top"));
	  else                   folderMenu ->changeItem( ID_TOP, i18n("Add folder to top"));
	
	  if ( d->text(0) == "Home" || d->text(0) == "Root" )
	        folderMenu ->setItemEnabled( ID_TOP, false);
	  else  folderMenu ->setItemEnabled( ID_TOP, true );
	
	  folderMenu ->popup     ( point);
	}
}

QString FilesTreeView::currentFileName()
{
	if ( !currentItem() ) return "";
	QListViewItem *item = currentItem();
	FilesTreeFolder *parent = dynamic_cast<FilesTreeFolder *> (item->parent());
	
	if ( !parent ) // top level element
		return ((FilesTreeFolder *)item)->fullName( item );
		
	return parent->fullName( item )+item->text(0);
}

void FilesTreeView::slotSelectFile(QListViewItem *item)
{
	if ( !item ) return;
	if ( !currentItem() ) return;
	FilesTreeFolder *parent = (FilesTreeFolder *) item->parent();
	if ( !parent ) return;
	if ( dynamic_cast<FilesTreeFolder *>(item) ) return;
	
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
//The file type is unknown, so do some Mimetype magic

 QString mimetype = KMimeType::findByFileContent(nameToOpen)->name();

 if (mimetype.contains("text"))
 {
		KURL url(nameToOpen);
		emit openFile( url );
 } else
 {
   if (KMessageBox::questionYesNo(this,"This file cannot be opened in Quanta. \n \
       Do you want to open with an external program or run it?","Unknown type") == KMessageBox::Yes)
  {
   KFileOpenWithHandler fowh;
	 new KRun( KURL(nameToOpen), 0, true );
  }
 }
}

void FilesTreeView::slotSelectAnyFile(QListViewItem *item)
{
	if ( !item ) return;
	if ( !currentItem() ) return;
	FilesTreeFolder *parent = (FilesTreeFolder *) item->parent();
	if ( !parent ) return;
	if ( dynamic_cast<FilesTreeFolder *>(item) ) return;
	
	KURL url(currentFileName());
	
 QString mimetype = KMimeType::findByFileContent(currentFileName())->name();

 if (! mimetype.contains("text"))
 {
   if (KMessageBox::questionYesNo(this,"This file may be a binary file, thus cannot be opened \
    in Quanta correctly.\n Do you still want to open it?","Wrong type") != KMessageBox::Yes)
  {
    return;
  }
 }
 emit openFile( url );
}

void FilesTreeView::slotSelectImage(QListViewItem *item)
{
	if ( !item ) return;
	if ( !currentItem() ) return;
	FilesTreeFolder *parent = (FilesTreeFolder *) item->parent();
	if ( !parent ) return;
	if ( dynamic_cast<FilesTreeFolder *>(item) ) return;
	
	QString nameToOpen = parent->fullName( item )+item->text(0);
	
	if ( QDir::match( fileMaskImage, nameToOpen) )
	{
		emit openImage( nameToOpen );
	}
}

void FilesTreeView::slotAddToTop()
{
  FilesTreeFolder *d = dynamic_cast<FilesTreeFolder *>( currentItem() );
	if ( d )
	{
	  if ( d->parentFolder )
	  { // add
	    QString f = currentFileName();
	    QString fname = f;
      int i;
      while ( ( i = f.find('/')) >= 0 ) f.remove(0,i+1);
	
      if ( dirList.find( fname ) != -1) return;

	    FilesTreeFolder *dir = new FilesTreeFolder( this , " "+f, fname+"/");
  	    dir->setPixmap( 0, SmallIcon("kdisknav") );
	    dir->setOpen( false);
	    dirList.append( fname );
	  }
	  else { // remove
	    QString f = currentFileName();
	    f = f.remove( f.length()-1, 1 );
	    dirList.remove( f );
	    delete( d );
	  }
	}
}
#include "filestreeview.moc"
