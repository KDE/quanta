/***************************************************************************
                          doctreeview.cpp  -  description
                             -------------------
    begin                : Sat Mar 4 2000
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

// QT clases
#include <qlistview.h>
#include <qstrlist.h>
#include <qheader.h>
#include <qpixmap.h>
#include <qdir.h>

// KDE clases
#include <kconfig.h>
#include <kapp.h>
#include <kstddirs.h>

// application clases
#include "doctreeview.h"
#include "docfolder.h"
#include "docitem.h"

// resources
#include "pix/folder_open.xpm"

DocTreeView::DocTreeView(QWidget *parent, const char *name )
  : QListView(parent,name)
{

  contextHelpDict = new QDict <QString> ( 101, false );

  setRootIsDecorated( true );
  header()->hide();
  setSorting(-1,false);

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( "Name" );

  QStringList docDirs = KGlobal::instance()->dirs()->findDirs("appdata", "doc");


  for ( QStringList::Iterator it = docDirs.begin(); it != docDirs.end(); ++it ) {
  	QString docDir = *it;
  	QDir dir(docDir, "*.docrc");
  	QStringList files = dir.entryList();
  	
  	for ( QStringList::Iterator it_f = files.begin(); it_f != files.end(); ++it_f ) {
  		KConfig config( docDir + *it_f );
  		config.setGroup("Tree");
  		
  		QString relDocDir = config.readEntry("Doc dir");
  		
  		DocFolder *folder = new DocFolder(this, config.readEntry("Top Element"), &config , docDir+relDocDir+"/");
  		folder->setPixmap( 0, QPixmap((const char**)folder_open_xpm) );
		  folder->topLevel = true;
		  folder->setOpen( true );
		
		  config.setGroup("Context");
		  QStrList list;
		  config.readListEntry("ContextList", list );
		
		  for ( unsigned int i=0; i<list.count(); i++ ) {
		  	QString keyword = list.at(i);
		  	QString *url = new QString( docDir + relDocDir + "/" + config.readEntry( list.at(i) ) );
		  	contextHelpDict->insert( keyword, url );
		  }
		
  	}
  }

  setFocusPolicy(QWidget::ClickFocus);

  connect( this, SIGNAL(clicked(QListViewItem *)), SLOT(clickItem(QListViewItem *)) );
}


DocTreeView::~DocTreeView(){
}

void DocTreeView::clickItem( QListViewItem *)
{
	QListViewItem *it = currentItem();
	if ( !it )
		return;
	DocItem *dit = dynamic_cast< DocItem *>(it);
	if ( dit )
		emit openURL( dit->url );
}


QString * DocTreeView::contextHelp( QString keyword )
{
	return contextHelpDict->find( keyword );
}
