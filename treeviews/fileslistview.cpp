/***************************************************************************
                          fileslistview.cpp  -  description
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

// QT includes
#include <qdir.h>
#include <qpixmap.h>
#include <qheader.h>

// KDE includes
#include <krun.h>
#include <klocale.h>
#include <kiconloader.h>

// app includes
#include "filestreefile.h"
#include "filestreefolder.h"
#include "fileslistview.h"

extern QString fileMaskHtml;
extern QString fileMaskJava;
extern QString fileMaskText;
extern QString fileMaskImage;

FilesListView::FilesListView( QString dir, QStringList dirList, QWidget *parent, const char *name)
	: FileManage(parent,name)
{
	this->dir	= dir;
	this->dirList = dirList;	
	
	setRootIsDecorated( true );
  header()->hide();
  setSorting( 0 );

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( "Name" );

  loadDir( dir );

	setFocusPolicy(QWidget::ClickFocus);
	
	fileMenu -> insertItem( i18n("Tree mode"), this ,SLOT(slotNewMode()));
	
	folderMenu -> insertItem( i18n("Tree mode"), this ,SLOT(slotNewMode()));
	
	connect(  this, SIGNAL(clicked(QListViewItem *)),	
						this, SLOT(slotClicked(QListViewItem *)));
	connect(  this, SIGNAL(doubleClicked(QListViewItem *)),
						this, SLOT(slotSelectFile(QListViewItem *)));
	connect(  this, SIGNAL(selectionChanged(QListViewItem *)),
						this, SLOT(slotSelectImage(QListViewItem *)));
	connect(	this, SIGNAL(returnPressed(QListViewItem *)),
						this, SLOT(slotSelectFile(QListViewItem *)));
	connect(  this, SIGNAL(returnPressed(QListViewItem *)),
						this, SLOT(slotClicked(QListViewItem *)));
						
	connect( this, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)),
					 this, SLOT(slotMenu(QListViewItem*, const QPoint&, int)));

	connect(	this, SIGNAL(open(QListViewItem *)),
						this,	SLOT(slotSelectFile(QListViewItem *)));
	connect(	this, SIGNAL(openInQuanta(QListViewItem *)),
						this,	SLOT(slotSelectAnyFile(QListViewItem *)));
}

FilesListView::~FilesListView()
{
}

void FilesListView::loadDir( QString path )
{
	QDir thisDir( path );
	
	QStringList dirList;
	QStringList fileList;
	
	QStringList::Iterator it;
	
	if ( !thisDir.exists()) return;
	if ( !thisDir.isReadable() ) return;
	
	thisDir.setSorting( QDir::Name);
	thisDir.setFilter ( QDir::Dirs);
	
	dirList = thisDir.entryList();
	
	dirList.first();
	dirList.remove(".");
	
	thisDir.setFilter( QDir::Files);
	
	fileList = thisDir.entryList();
	
	FilesTreeFile* item;
	
	for ( it = fileList.begin(); it != fileList.end(); ++it )
	{
	  item = new FilesTreeFile( this, *it, dir+(*it) );
	  item->setIcon( *it );
	}
	
	FilesTreeFile *ditem;
		
	for ( it = dirList.begin(); it != dirList.end(); ++it )
	{
	  ditem = new FilesTreeFile( this, *it, dir+(*it) );
	  ditem->setDirIcon();
	}
}

void FilesListView::slotNewMode()
{
	emit changeMode();
}

void FilesListView::slotReload()
{
	QListViewItem *child;
  while ( (child = firstChild()) )
    removeItem( child );

  loadDir( dir );
}

void FilesListView::slotMenu(QListViewItem *item, const QPoint &point, int)
{
	if ( !item ) return;
	setSelected(item, true);
	
	FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
	if ( !f ) return;
	
	if ( f->isDir ) folderMenu->popup( point);
	else							fileMenu->popup( point);
}

QString FilesListView::currentFileName()
{
	if ( !currentItem() ) return dir;
	FilesTreeFile *f = dynamic_cast<FilesTreeFile *>(currentItem());
	if ( f ) return dir+f->text(0);
	FilesTreeFolder *d = dynamic_cast<FilesTreeFolder *>(currentItem());
	if ( f ) return dir+d->text(0);
	return dir;
}

void FilesListView::slotClicked(QListViewItem *item)
{
	if ( !item ) return;
	FilesTreeFile *file = (FilesTreeFile *)item;
	if ( !file->isDir ) return;
	
	if ( file->text(0) == ".." )
	{
		QDir thisDir( dir );
		thisDir.cdUp();
		dir = thisDir.absPath();
		if ( dir.right(1) != "/" ) dir += "/";
	}
	else
		dir = dir + file->text(0)+"/";
	
	slotReload();
}

void FilesListView::slotSelectFile(QListViewItem *item)
{
	if ( !item ) return;
	FilesTreeFile *file = (FilesTreeFile *)item;
	if ( file->isDir ) return;
	if ( !currentItem() ) return;
	
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

void FilesListView::slotSelectAnyFile(QListViewItem *item)
{
	if ( !item ) return;
	FilesTreeFile *file = (FilesTreeFile *)item;
	if ( file->isDir ) return;
	if ( !currentItem() ) return;
	
	KURL url(currentFileName());
	
	emit openFile( url );
}


void FilesListView::slotSelectImage(QListViewItem *item)
{
	if ( !item ) return;
	FilesTreeFile *file = (FilesTreeFile *)item;
	if ( file->isDir ) return;
	if ( !currentItem() ) return;
	
	QString nameToOpen = currentFileName();
	
	if ( QDir::match( fileMaskImage, nameToOpen) )
	{
		emit openImage( nameToOpen );
	}
}
