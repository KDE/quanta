/***************************************************************************
                          uploadtreeview.cpp  -  description
                             -------------------
    begin                : Sun Aug 25 2002
    copyright            : (C) 2002 by Andras Mantia
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

#include <iostream.h>

#include "uploadtreeview.h"

#include <klocale.h>

UploadTreeView::UploadTreeView( QWidget *parent, const char *name ) :
	QListView(parent, name)
{
	setRootIsDecorated( true );
	//header()->hide();
	setSorting( 0 );

	setFrameStyle( Panel | Sunken );
	setLineWidth( 2 );
	addColumn( tr2i18n("Name") );
  addColumn( tr2i18n("Upload") );
	addColumn( tr2i18n("Size") );
	addColumn( tr2i18n("Date") );

	setFocusPolicy(QWidget::ClickFocus);

	connect(  this, SIGNAL(selectionChanged(QListViewItem *)),
						this, SLOT(slotSelectFile(QListViewItem *)));
	connect(  this, SIGNAL(clicked(QListViewItem *)),
						this, SLOT(slotSelectFile(QListViewItem *)));

	//rootDir = new FilesTreeFolder( this , "Root Directory", "/");
}

UploadTreeView::~UploadTreeView()
{
}

int UploadTreeView::checkboxTree( QListViewItem *it = 0 )
{
	QListViewItem *itIter = 0;
	if (it == 0) itIter = firstChild();
	else itIter = it->firstChild();
  // bitFlag structure: (0/1)all children exist (0/1)no children exist.
  // We don't need some children as a bit flag, because that's implied if the bits are "00".
  int bitFlags = 1 << 1 | 1 << 0;

	for( ; itIter != 0; itIter = itIter->nextSibling() )
	{
    if ( dynamic_cast<UploadTreeFolder *>(itIter) )
    {
      int hadCheckFlags = checkboxTree( itIter );
      bitFlags &= hadCheckFlags;
      UploadTreeFolder *itF = dynamic_cast<UploadTreeFolder *>(itIter);

      if (bitFlags >> 1 == 1) {
        // All children exist.
        itF->setWhichPixmap( "check" );
        itF->setSelected( true );
      }
      else if (bitFlags % 2 == 1) {
        // No children exist.
        itF->setWhichPixmap( "check_clear" );
        itF->setSelected( false );
      }
      else {
        // Some children exist.
        itF->setWhichPixmap( "check_grey" );
        itF->setSelected( true );
      }

    }
    else if ( dynamic_cast<UploadTreeFile *>(itIter) )
    {
      UploadTreeFile *itF = dynamic_cast<UploadTreeFile *>(itIter);
      if ( itF->isSelected() )
      {
        itF->setWhichPixmap("check");
        // Turn off "no children"
        if ( bitFlags % 2 == 1 ) bitFlags -= 1;
      }
      else
      {
        itF->setWhichPixmap("check_clear");
        // Turn off "all children".
        if (bitFlags >> 1 == 1) bitFlags -= 2;
      }

    }
	}

  return bitFlags;
}

void UploadTreeView::selectAllUnderNode( QListViewItem* it, bool select )
{
	QListViewItem *itIter = 0;
	if (it == 0) itIter = firstChild();
	else itIter = it->firstChild();

	for( ; itIter != 0; itIter = itIter->nextSibling() )
	{
    itIter->setSelected(select);
		selectAllUnderNode(itIter, select);
	}
}

void UploadTreeView::slotSelectFile( QListViewItem *it )
{
  // This need a bit of special behavior for clicking on directories.
  if ( dynamic_cast<UploadTreeFolder *>(it) )
  {
    selectAllUnderNode( it, it->isSelected() );
  }

  slotSelectFile();
}

void UploadTreeView::slotSelectFile( )
{
  // All that is known is that a change occurred.  Need to traverse through the tree to do the update.
  checkboxTree( );
}

UploadTreeFolder* UploadTreeView::findFolder( UploadTreeFolder *it, QString folderName )
{
	QListViewItem *itIter = 0;
	if (it == 0) itIter = firstChild();
	else itIter = it->firstChild();

	for( ; itIter != 0; itIter = itIter->nextSibling() )
	{
		if ( itIter->text(0) == folderName )
		{
			return (UploadTreeFolder *)itIter;
		}
	}
  return 0;
}

UploadTreeFolder* UploadTreeView::printTree( UploadTreeFolder *it = 0, QString indent = "" )
{
	QListViewItem *itIter = 0;
	if (it == 0) itIter = firstChild();
	else itIter = it->firstChild();

	for( ; itIter != 0; itIter = itIter->nextSibling() )
	{
    cout << indent << itIter->text(0) << endl;
    if ( dynamic_cast<UploadTreeFolder *>(itIter) )
    	printTree( (UploadTreeFolder *)itIter, indent + "  " );
	}
  return 0;
}

// :NOTE: AFAIK, safe to use only if you are sure the item searched for
// is already in here.  It might be safe otherwise, but use at your own
// peril.
QListViewItem* UploadTreeView::findItem( QString path )
{
	QString item = path;
	UploadTreeFolder *it = 0;
	int i;
	while ( ( i = item.find('/') ) >= 0 )
	{
		it = findFolder( it, item.left(i) );
		if ( it == 0 ) return 0;
		item.remove(0,i+1);
	}

	QListViewItem *itIter = 0;
	if ( it == 0 ) itIter = firstChild();
	else itIter = it->firstChild();

	for( ; itIter != 0; itIter = itIter->nextSibling() )
	{
		if ( itIter->text(0) == item )
		{
			return itIter;
		}
	}
	return 0;
}

UploadTreeFile* UploadTreeView::addItem( QString item, QString date, QString size )
{
	QString fname = item;
	int i;
	UploadTreeFolder *it = 0;
	while ( ( i = item.find('/') ) >= 0 )
	{
		UploadTreeFolder *itTemp = findFolder(it, item.left(i));
		if ( itTemp == 0 )
		{
			if ( it == 0 )
			{
				it = new UploadTreeFolder(this, item.left(i));
			}
			else {
				it = new UploadTreeFolder(it, item.left(i));
			}
		}
		else
		{
			it = itTemp;
		}
		item.remove(0,i+1);
	}
  UploadTreeFile *file = 0;
	if (it == 0)
	{
		file = new UploadTreeFile(this, item, date, size);
	}
	else
	{
		file = new UploadTreeFile(it, item, date, size);
	}
	return file;
}
