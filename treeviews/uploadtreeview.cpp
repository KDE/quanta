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


#include <klocale.h>
#include <kurl.h>

#include "uploadtreeview.h"
#include "../quantacommon.h"

UploadTreeView::UploadTreeView( QWidget *parent, const char *name ) :
	QListView(parent, name)
{
	setRootIsDecorated( true );
	setSorting( 0 );

	setFrameStyle( Panel | Sunken );
	setLineWidth( 2 );
	addColumn( i18n("Name") );
  addColumn( i18n("Upload") );
	addColumn( i18n("Size") );
	addColumn( i18n("Date") );

	setFocusPolicy(QWidget::ClickFocus);

	connect(  this, SIGNAL(selectionChanged(QListViewItem *)),
						this, SLOT(slotSelectFile(QListViewItem *)));
	connect(  this, SIGNAL(clicked(QListViewItem *)),
						this, SLOT(slotSelectFile(QListViewItem *)));
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

  int bitFlags = 3;

  if ( itIter != 0 )
  {
    for( ; itIter != 0; itIter = itIter->nextSibling() )
    {
      if ( dynamic_cast<UploadTreeFolder *>(itIter) )
      {
        int hadCheckFlags = checkboxTree( itIter );
        bitFlags &= hadCheckFlags;
        UploadTreeFolder *itF = dynamic_cast<UploadTreeFolder *>(itIter);

        if (hadCheckFlags == 2) {
          // All children exist.
          itF->setWhichPixmap( "check" );
          itF->setSelected( true );
        }
        else if (hadCheckFlags == 1) {
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
  }
  else
  {
    return 1;
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

//TODO: This should search based on url's rather than on text(0)
UploadTreeFolder* UploadTreeView::findFolder( UploadTreeFolder *it, const QString& folderName )
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
//    cout << indent << itIter->text(0) << endl;
    if ( dynamic_cast<UploadTreeFolder *>(itIter) )
    	printTree( (UploadTreeFolder *)itIter, indent + "  " );
	}
  return 0;
}

// :NOTE: AFAIK, safe to use only if you are sure the item searched for
// is already in here.  It might be safe otherwise, but use at your own
// peril.
QListViewItem* UploadTreeView::findItem(const QString& path )
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

UploadTreeFile* UploadTreeView::addItem(const KURL &a_url, QString date, QString size )
{ 
  QString item = a_url.path(); //TODO: do with real KURL's
	QString fname = item;
	int i;
  uint col = 0;
	UploadTreeFolder *it = 0;
  KURL u;
	while ( ( i = item.find('/', col) ) >= 0 )
	{
		UploadTreeFolder *itTemp = findFolder(it, item.mid(col, i - col));
		if ( itTemp == 0 )
		{
      u = a_url;
      QuantaCommon::setUrl(u,item.left(i)+"/");
			if ( it == 0 )
			{
				it = new UploadTreeFolder(u, this, "");
			}
			else {
				it = new UploadTreeFolder(u, it, "");
			}
		}
		else
		{
			it = itTemp;
		}
    col = i + 1;
	}
  UploadTreeFile *file = 0;
  if ( col < item.length() )
  {
    if (it == 0)
    {
    file = new UploadTreeFile(this, a_url, date, size);
    }
    else
    {
      file = new UploadTreeFile(it, a_url, date, size);
    }
  }
	return file; 
}

void UploadTreeView::expandAll( QListViewItem *it )
{
  QListViewItem *itIter = it;
  if (it == 0) itIter = firstChild();
  else itIter = it->firstChild();

  for( ; itIter != 0; itIter = itIter->nextSibling() )
  {
    if ( dynamic_cast<UploadTreeFolder *>(itIter) )
    {
      itIter->setOpen( true );
      expandAll( itIter );
    }
  }
}

void UploadTreeView::collapseAll( QListViewItem *it )
{
  QListViewItem *itIter = it;
  if (it == 0) itIter = firstChild();
  else itIter = it->firstChild();

  for( ; itIter != 0; itIter = itIter->nextSibling() )
  {
    if ( dynamic_cast<UploadTreeFolder *>(itIter) )
    {
      itIter->setOpen( false );
      expandAll( itIter );
    }
  }
}

void UploadTreeView::invertAll( QListViewItem *it )
{
  QListViewItem *itIter = it;
  if (it == 0) itIter = firstChild();
  else itIter = it->firstChild();

  for( ; itIter != 0; itIter = itIter->nextSibling() )
  {
    if ( dynamic_cast<UploadTreeFile *>(itIter) )
    {
      itIter->setSelected( !itIter->isSelected() );
    }
    else
    {
      invertAll( itIter );
    }
  }
}

void UploadTreeView::selectAll( bool select )
{
  QListViewItemIterator it(this);

  for ( ; it.current(); ++it )
  {
    if ( dynamic_cast<UploadTreeFile *>(it.current()) )
    {
      it.current()->setSelected( select);
    }
  }
}


/** No descriptions */
QListViewItem* UploadTreeView::itemByUrl(const KURL& a_url)
{
  QListViewItemIterator it(this);
  UploadTreeFile *fileItem;
  UploadTreeFolder *folderItem;
  KURL url;
  for ( ; it.current(); ++it )
  {
    if ( (fileItem = dynamic_cast<UploadTreeFile *>(it.current()) ) !=0)
    {
      url = fileItem->m_url;
    } else
    {
      folderItem = dynamic_cast<UploadTreeFolder*>(it.current());
      url = folderItem->m_url;
    }
    if (url == a_url)
    {
      return it.current();
    }
  }

  return 0L;
}

#include "uploadtreeview.moc"
