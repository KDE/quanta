/***************************************************************************
                          filemanage.cpp  -  description
                             -------------------
    begin                : Mon Aug 7 2000
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
#include <qpopupmenu.h>
#include <qclipboard.h>
#include <qapp.h>
#include <qfileinfo.h>
#include <qstringlist.h>

// KDE includes
#include <klocale.h>
#include <kiconloader.h>
#include <kio/jobclasses.h>
#include <kio/paste.h>
#include <kio/global.h>
#include <kio/job.h>
#include <kpropsdlg.h>
#include <kopenwith.h>
#include <kmessagebox.h>

// Application includes
#include "filemanage.h"
#include "../quantacommon.h"

FileManage::FileManage()
{
}

FileManage::FileManage( QWidget *parent, const char *name)
	: QListView( parent, name )
{
	fileMenu = new QPopupMenu();
	
	fileMenu -> insertItem( UserIcon("open"),  i18n("&Open"), 		this ,SLOT(slotOpen()));
	fileMenu -> insertItem(					  			   i18n("Open With..."), 		this ,SLOT(slotOpenWith()));
	fileMenu -> insertItem(					  			   i18n("Open in Quanta"), 	this ,SLOT(slotOpenInQuanta()));
	fileMenu -> insertItem(					  			   i18n("Insert Tag"), 	this ,SLOT(slotInsertTag()));
	fileMenu -> insertSeparator();
	fileMenu -> insertItem(					  			   i18n("Insert in Project..."),this ,SLOT(slotInsertInProject()));
	fileMenu -> insertSeparator();
	fileMenu -> insertItem( UserIcon("copy"),  i18n("&Copy"), 		this ,SLOT(slotCopy()));
	fileMenu -> insertItem( UserIcon("paste"), i18n("&Paste"),		this ,SLOT(slotPaste()));
	fileMenu -> insertItem( UserIcon("delete"),i18n("&Delete..."),   this ,SLOT(slotDelete()));
	fileMenu -> insertItem( i18n("Properties..."),   this ,SLOT(slotProperties()));
	fileMenu -> insertSeparator();
	fileMenu -> insertItem( i18n("Reload"),   this ,SLOT(slotReload()));


	folderMenu = new QPopupMenu();

	folderMenu -> insertItem(					  			   i18n("Insert in Project..."),this ,SLOT(slotInsertDirInProject()));
	folderMenu -> insertSeparator();
	folderMenu -> insertItem( UserIcon("copy"),  i18n("&Copy"), 		this ,SLOT(slotCopy()));
	folderMenu -> insertItem( UserIcon("paste"), i18n("&Paste"),		this ,SLOT(slotPaste()));
	folderMenu -> insertItem( UserIcon("delete"),i18n("&Delete..."),   this ,SLOT(slotDelete()));
	folderMenu -> insertItem( i18n("Properties..."),   this ,SLOT(slotProperties()));
	folderMenu -> insertSeparator();
	folderMenu -> insertItem( i18n("Reload"),   this ,SLOT(slotReload()));

	//setShowSortIndicator(true);
}

FileManage::~FileManage()
{
}

// virtual method
void FileManage::slotReload()
{
}

// virtual method
QString FileManage::currentFileName()
{
	return("");
}

void FileManage::slotOpen()
{
	if ( !currentItem() ) return;
	emit open( currentItem() );
}

void FileManage::slotOpenInQuanta()
{
	if ( !currentItem() ) return;
	emit openInQuanta( currentItem() );
}

void FileManage::slotOpenWith()
{
	if ( !currentItem() ) return;
	
	QStringList list;
	QString fileToOpen = currentFileName();
	list.append( fileToOpen );
	
	KFileOpenWithHandler *kfowh = new KFileOpenWithHandler();
	
	kfowh -> displayOpenWithDialog( KURL::List( list ) );
}

void FileManage::slotCopy()
{
   if ( !currentItem() ) return;
   QClipboard *cb = QApplication::clipboard();
   cb->setText( currentFileName() );
}

void FileManage::slotPaste()
{
	if ( !currentItem() ) return;

  QClipboard *cb = QApplication::clipboard();

  QFileInfo cfile( currentFileName() );

  KURL::List list( QStringList::split( QChar('\n'), cb->text() ) );

  KURL url;
  QuantaCommon::setUrl(url,cfile.dirPath());
  KIO::Job *job = KIO::copy( list, url);
  connect( job, SIGNAL( result( KIO::Job *) ), this , SLOT( slotJobFinished( KIO::Job *) ) );
}

void FileManage::slotDelete()
{
  if ( !currentItem() ) return;
  
  if ( KMessageBox::warningYesNo(this,i18n("Do you really want to delete file \n%1 ?\n").arg(currentFileName())) == KMessageBox::Yes ) {


  KURL url;
  QuantaCommon::setUrl(url,currentFileName());
  KIO::Job *job = KIO::del(url);
   connect( job, SIGNAL( result( KIO::Job *) ), this , SLOT( slotJobFinished( KIO::Job *) ) );
  }
}

void FileManage::slotProperties()
{
  if ( !currentItem() ) return;

  KPropertiesDialog *propDlg = new KPropertiesDialog( KURL(currentFileName()), this, 0L, true  );

  connect( propDlg, SIGNAL( applied() ), this , SLOT( slotPropertiesApplied()) );
}


void FileManage::slotJobFinished( KIO::Job *)
{
//  slotReload();
 emit reloadTreeviews();
}

void FileManage::slotInsertInProject()
{
	if ( !currentItem() ) return;
	emit insertFileInProject( currentFileName() );
}

void FileManage::slotInsertDirInProject()
{
	if ( !currentItem() ) return;
	emit insertDirInProject( currentFileName() );
}

void FileManage::slotInsertTag() 
{
  if ( !currentItem() ) return;
  DirInfo dirInfo;

  dirInfo.mimeType = "";
  dirInfo.preText = "";
  dirInfo.postText = "";
  emit insertTag( currentFileName(), dirInfo );
}

/** No descriptions */
void FileManage::slotPropertiesApplied()
{

}

#include "filemanage.moc"
