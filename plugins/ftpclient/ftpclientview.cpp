/****************************************************************************
** $Id$
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "ftpclientview.h"
#include "ftpclientview.moc"
#include <qdir.h>
#include <qpixmap.h>
#include <qvaluelist.h>

// include images
/*
extern QString fileMaskHtml;
extern QString fileMaskText;
extern QString fileMaskJava;
extern QString fileMaskImage;
*/
QPixmap *folderIcon    = 0;
QPixmap *fileIconTxt   = 0;
QPixmap *fileIconHtml  = 0;
QPixmap *fileIconImage = 0;
QPixmap *fileIconJava  = 0;

FtpClientViewItem::FtpClientViewItem( QListView *parent, const QUrlInfo &i )
    : QListViewItem( parent, i.name() ), info( i )
{
}
/*
QString FtpClientViewItem::key( int c, bool ) const
{
    switch ( c ) {
    case 0:
	if ( info.isDir() )
	    return "0000" + info.name();
	return info.name();
    case 1: {
	QString s;
	s.sprintf( "%08d", info.size() );
	return s;
    }
    case 2: {
	QDateTime epoch( QDate( 1980, 1, 1 ) );
	QString s;
	s.sprintf( "%08d", epoch.secsTo( info.lastModified() ) );
	return s;
    }
    }

    return text( c );
}

QString FtpClientViewItem::text( int c ) const
{
	switch ( c ) {
    case 0:
			return info.name();
    case 1:
			return QString::number( info.size() );
    case 2:
			return info.lastModified().toString();
  }

  return "????";
}

const QPixmap *FtpClientViewItem::pixmap( int c ) const
{
  if ( !folderIcon	 )	folderIcon		= new QPixmap( (const char**)folder_xpm );
  if ( !fileIconTxt  )	fileIconTxt		= new QPixmap( (const char**)txt_xpm   );
  if ( !fileIconHtml )	fileIconHtml	= new QPixmap( (const char**)html_xpm  );
  if ( !fileIconImage)	fileIconImage	= new QPixmap( (const char**)image_xpm );
  if ( !fileIconJava )	fileIconJava	= new QPixmap( (const char**)java_xpm  );

  if      ( info.isDir()  && c == 0 )		return folderIcon;
  else if ( info.isFile() && c == 0 )
  {
  	if ( QDir::match( fileMaskHtml,  info.name()) ) return fileIconHtml;
	  if ( QDir::match( fileMaskImage, info.name()) ) return fileIconImage;
  	if ( QDir::match( fileMaskJava,  info.name()) ) return fileIconJava;

  	return fileIconTxt;
  }

  return 0;
}
*/

FtpClientView::FtpClientView( QWidget *parent )
    : QListView( parent )
{
  setRootIsDecorated    ( false );
  setColumnAlignment		 ( 1, Qt::AlignRight );
  setShowSortIndicator	 ( true );
  setAllColumnsShowFocus( true );
  setMultiSelection		 ( true );

  connect( this, SIGNAL( doubleClicked( QListViewItem * ) ),
     this, SLOT( slotSelected( QListViewItem * ) ) );
  connect( this, SIGNAL( returnPressed( QListViewItem * ) ),
     this, SLOT( slotSelected( QListViewItem * ) ) );
}

void FtpClientView::slotInsertEntries( const QValueList<QUrlInfo> &info )
{
  QValueList<QUrlInfo>::ConstIterator it;

  for( it = info.begin(); it != info.end(); ++it )
  {
		if ( (*it).name() != ".." &&
		     (*it).name() != "." &&
		     (*it).name()[ 0 ] == '.' )
     	continue;

		FtpClientViewItem *item = new FtpClientViewItem( this, (*it) );

		if ( (*it).isDir() )
	  	  item->setSelectable( false );
  }
}

void FtpClientView::slotSelected( QListViewItem *item )
{
  if ( !item )
	return;

  FtpClientViewItem *i = (FtpClientViewItem*)item;
  if ( i->entryInfo().isDir() )
		emit itemSelected( i->entryInfo() );
}

QValueList<QUrlInfo> FtpClientView::selectedItems() const
{
  QValueList<QUrlInfo> lst;
  QListViewItemIterator it( (QListView*)this );
  for ( ; it.current(); ++it ) {
		if ( it.current()->isSelected() ) {
	    lst << ( (FtpClientViewItem*)it.current() )->entryInfo();
		}
  }

  return lst;
}
