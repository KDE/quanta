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
#include <kurl.h>
#include <kdirlister.h>

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
  dirLister = new KDirLister();
  dirLister->setShowingDotFiles(false);
  connect(dirLister, SIGNAL(clear()),SLOT(slotDirListClear()));
  connect(dirLister, SIGNAL(completed(const KURL&)),SLOT(slotDirListCompleted(const KURL&)));
  connect(dirLister, SIGNAL(newItems( const KFileItemList&)),SLOT(slotDirListNewItems(const KFileItemList&)));
  connect(dirLister, SIGNAL(deleteItem( KFileItem *)),SLOT(slotDirListDeleteItem(KFileItem *)));
  connect(dirLister, SIGNAL(refreshItems( const KFileItemList&)),SLOT(slotDirListRefreshItesm(const KFileItemList&)));
  connect(dirLister, SIGNAL(clear(const KURL&)),SLOT(slotDirListClearURL(const KURL&)));
}

FileManage::~FileManage()
{
//  delete dirLister;
}

// virtual method
void FileManage::slotReload()
{
}

// virtual method
KURL FileManage::currentURL()
{
	return KURL();
}

void FileManage::slotOpen()
{
	if (currentItem())
  {
	  emit open( currentItem() );
  }
}

void FileManage::slotOpenInQuanta()
{
	if (currentItem() )
  {
	  emit openInQuanta( currentItem() );
  }
}

void FileManage::slotOpenWith()
{
	if (currentItem())
  {
  	KURL::List list;
	  KURL urlToOpen = currentURL();
	  list.append( urlToOpen );
	
	  KFileOpenWithHandler *kfowh = new KFileOpenWithHandler();
  	kfowh->displayOpenWithDialog( list );
    delete kfowh;
  }
}

void FileManage::slotCopy()
{
   if (currentItem())
   {
     QClipboard *cb = QApplication::clipboard();
     cb->setText( currentURL().prettyURL() );
   }
}

void FileManage::slotPaste()
{
	if (currentItem())
  {
    QClipboard *cb = QApplication::clipboard();
    KURL::List list( QStringList::split( QChar('\n'), cb->text() ) );

    KURL url = currentURL();
    KIO::Job *job = KIO::copy( list, url);
    connect( job, SIGNAL( result( KIO::Job *) ), this , SLOT( slotJobFinished( KIO::Job *) ) );
  }
}

void FileManage::slotDelete()
{
  if (currentItem())
  {
    KURL url = currentURL();
    if ( KMessageBox::warningYesNo(this,i18n("Do you really want to delete file \n%1 ?\n").arg(url.path())) == KMessageBox::Yes )
    {
      KIO::Job *job = KIO::del(url);
      connect( job, SIGNAL( result( KIO::Job *) ), this , SLOT( slotJobFinished( KIO::Job *) ) );
    }
  }
}

void FileManage::slotProperties()
{
  if (currentItem())
  {
    KPropertiesDialog *propDlg = new KPropertiesDialog(currentURL(), this, 0L, true  );
    connect( propDlg, SIGNAL( applied() ), this , SLOT( slotPropertiesApplied()) );
  }
}


void FileManage::slotJobFinished( KIO::Job *)
{
//  slotReload();
// emit reloadTreeviews();
}

void FileManage::slotInsertInProject()
{
	if (currentItem() )
  {
	  emit insertFileInProject( currentURL());
  }
}

void FileManage::slotInsertDirInProject()
{
	if ( currentItem() )
  {
	  emit insertDirInProject( currentURL() );
  }
}

void FileManage::slotInsertTag() 
{
  if (currentItem() )
  {
    DirInfo dirInfo;

    dirInfo.mimeType = "";
    dirInfo.preText = "";
    dirInfo.postText = "";
    emit insertTag( currentURL(), dirInfo );
  }
}

/** No descriptions */
void FileManage::slotPropertiesApplied()
{

}

/** No descriptions */
int FileManage::denyBinaryInsert()
{
  KMessageBox::sorry(this, i18n("Can't insert binary file as text"), i18n("Wrong type"),
FALSE);

 return 1; //not used yet
}

/** No descriptions */
void FileManage::slotDirListClear(){
}
/** No descriptions */
void FileManage::slotDirListCompleted(const KURL&){
}
/** No descriptions */
void FileManage::slotDirListNewItems(const KFileItemList& )
{
}
/** No descriptions */
void FileManage::slotDirListDeleteItem(KFileItem*){
}
/** No descriptions */
void FileManage::slotDirListRefreshItesm(const KFileItemList& ){
}
/** No descriptions */
void FileManage::slotDirListClearURL(const KURL& ){
}


#include "filemanage.moc"
