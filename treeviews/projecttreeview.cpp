/***************************************************************************
                          projecttreeview.cpp  -  description
                             -------------------
    begin                : Tue Mar 14 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia
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
#include <kprogress.h>
#include <kstatusbar.h>

// app includes
#include "projecttreefile.h"
#include "projecttreefolder.h"
#include "projecttreeview.h"
#include "filestreeview.h"
#include "../quantacommon.h"
#include "../qextfileinfo.h"
#include "../resource.h"
#include "../quanta.h"

ProjectTreeView::ProjectTreeView(QWidget *parent, const char *name )
  //: QListView(parent,name)
{
  setRootIsDecorated( true );
  header()->hide();
  setSorting( 0 );

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( "Name" );

	setFocusPolicy(QWidget::ClickFocus);
	
	projectDir =  new ProjectTreeFolder( this, i18n("No project"), baseURL);
	projectDir -> setPixmap( 0, SmallIcon("folder"));
	projectDir -> setOpen( true );

	fileMenu = new QPopupMenu(this);

	fileMenu -> insertItem( UserIcon("open"), i18n("&Open"), this, SLOT(slotOpen()));
	fileMenu -> insertItem( i18n("Open With..."), this, SLOT(slotOpenWith()));
	openInQuantaId = fileMenu -> insertItem( i18n("Open in Quanta"), this, SLOT(slotOpenInQuanta()));
	fileMenu -> insertSeparator();
	fileMenu -> insertItem(	UserIcon("delete"),i18n("Remove From Disc (and project)..."), this, SLOT(slotRemove()));
	fileMenu -> insertItem( i18n("Remove From Project..."),this ,SLOT(slotRemoveFromProject(int)));
	fileMenu -> insertItem( i18n("Rename..."), this, SLOT(slotRename()));
	fileMenu -> insertSeparator();
	fileMenu -> insertItem( i18n("Upload File..."), this, SLOT(slotUploadSingleURL()));
	fileMenu -> insertSeparator();
	fileMenu -> insertItem( i18n("Properties..."), this, SLOT(slotProperties()));
	fileMenu -> insertSeparator();
	fileMenu -> insertItem(SmallIcon("reload"),i18n( "&Rescan Project Directory" ),  this, SLOT(slotRescan()));
	fileMenu -> insertItem(i18n( "Project &Options" ),  this, SLOT(slotOptions()));

	folderMenu = new QPopupMenu(this);

	folderMenu -> insertItem( UserIcon("open"), i18n("&Open"), this, SLOT(slotOpen()));
 	folderMenu -> insertSeparator();
	folderMenu -> insertItem( UserIcon("delete"),i18n("Remove From Disc (and project)..."), this, SLOT(slotRemove()));
	folderMenu -> insertItem( i18n("Remove From Project..."),this ,SLOT(slotRemoveFromProject(int)));
 	folderMenu -> insertItem( i18n("Rename..."), this, SLOT(slotRename()));
	folderMenu -> insertSeparator();
	folderMenu -> insertItem( i18n("Upload Folder..."), this, SLOT(slotUploadSingleURL()));
	folderMenu -> insertSeparator();
	folderMenu -> insertItem( i18n("Properties..."), this, SLOT(slotProperties()));
	folderMenu -> insertSeparator();
	folderMenu -> insertItem(SmallIcon("reload"),i18n( "&Rescan Project Directory" ),  this, SLOT(slotRescan()));
	folderMenu -> insertItem(i18n( "Project &Options" ),  this, SLOT(slotOptions()));

  projectMenu = new QPopupMenu(this);
	projectMenu -> insertItem(SmallIcon("reload"),i18n( "&Rescan Project Directory" ),  this, SLOT(slotRescan()));
	projectMenu -> insertItem(i18n( "Project &Options" ),  this, SLOT(slotOptions()));
  


  connect(this, SIGNAL(doubleClicked(QListViewItem *)),
          this, SLOT  (slotSelectFile(QListViewItem *)));
  connect(this, SIGNAL(selectionChanged(QListViewItem *)),
          this, SLOT  (slotSelectImage(QListViewItem *)));
  connect(this, SIGNAL(returnPressed(QListViewItem *)),
          this, SLOT  (slotSelectFile(QListViewItem *)));

	connect(this, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)),
            this, SLOT  (slotMenu(QListViewItem*, const QPoint&, int)));

	connect(this, SIGNAL(open(QListViewItem *)),
            this, SLOT(slotSelectFile(QListViewItem *)));
}

ProjectTreeView::~ProjectTreeView(){
}

KURL ProjectTreeView::currentURL()
{
  KURL url;
	QListViewItem *item = currentItem();
 	ProjectTreeFolder *folderItem = dynamic_cast<ProjectTreeFolder *> (item);
  if ( folderItem )
  {
    url = folderItem->url;
  } else
  {
   	ProjectTreeFile *fileItem = dynamic_cast<ProjectTreeFile *> (item);
    if ( fileItem )
    {
      url = fileItem->url;
    }
  }
  url = QExtFileInfo::toAbsolute(url, baseURL);
  return url;
}

/** slot for right button menu */
void ProjectTreeView::slotMenu(QListViewItem *item, const QPoint& point, int)
{
  if ( item )
  {
    if (item == projectDir)
    {
     projectMenu->popup( point);
    } else
    {
      setSelected(item, true);
      ProjectTreeFile *f = dynamic_cast<ProjectTreeFile *>( item);
      if ( f )
      {
        if (currentURL().fileName().endsWith(toolbarExtension))
        {
          fileMenu->changeItem(openInQuantaId, i18n("Load Toolbar File"));
        } else
        {
          fileMenu->changeItem(openInQuantaId, i18n("Open in Quanta"));
        }
        fileMenu->popup( point);
      }
      ProjectTreeFolder *d = dynamic_cast<ProjectTreeFolder *>( item);
      if ( d )
      {
        if ( d->text(0) != "CVS") folderMenu->popup( point);
      }
    }
  }
}

void ProjectTreeView::slotSetBaseURL( const KURL& url )
{
	baseURL = url;
}

void ProjectTreeView::slotSetProjectName( QString name )
{
	projectName = name;
	projectDir->setText( 0, name);
}

void ProjectTreeView::slotReloadTree( const KURL::List &a_urlList, bool buildNewTree)
{
  quantaApp->statusBar()->changeItem( i18n("Building the project tree...") , IDS_STATUS);
  progressBar->setTotalSteps(a_urlList.count()*2-2);
  progressBar->setValue(0);
  progressBar->setTextEnabled(true);
  if (buildNewTree)
  {
    progressBar->setTotalSteps(a_urlList.count()-1);
    if (projectDir) delete projectDir;
    QString projectNameStr = projectName+" ";
    if (projectName != i18n("No Project"))
    {
     if (baseURL.protocol() == "file")
     {
       projectNameStr += i18n("[local disk]");
     } else
     {
       projectNameStr += "["+baseURL.protocol()+"://"+baseURL.user()+"@"+baseURL.host()+"]";
     }
    }
    projectDir = new ProjectTreeFolder(this, projectNameStr, baseURL);
    projectDir->setPixmap( 0, UserIcon("mini-modules") );
    projectDir->setOpen( true );  
  }

//first add all the new files to the treeview
	int pos;

	ProjectTreeFolder *newFolder = 0L;
	ProjectTreeFolder *folder = projectDir;

  KURL::List urlList = a_urlList;
	KURL url;
	KURL::List::Iterator it;
  int col;
  for ( it = urlList.begin(); it != urlList.end(); ++it )
	{
    folder = projectDir;
    url = *it;
    QString path = url.path();
    col = 0;
    //insert first the directories
    while ( ( pos = path.find('/', col)) > 0 )
    {
      QString dir = path.mid(col, pos - col);
      newFolder = 0L;
      QListViewItem *item = folder->firstChild();
      while( item && !newFolder)
      {
        if ( dir == item->text(0) ) //get the correct dir to insert into
        {
        	newFolder = dynamic_cast<ProjectTreeFolder *>(item);
          break;
        }
        item = item->nextSibling();
      }

      if ( !newFolder )
      {
        KURL u = url;
        u.setPath(url.path().left(pos)+"/");
      	newFolder = new ProjectTreeFolder( folder, u); //no dir was found, so create it
      }

      folder = newFolder;
      col = pos+1;
    } //while

    if ( col < path.length())
    {
      QListViewItem *item = folder->firstChild();
      bool neednew = true;
      while( item && neednew)
      {
        if ( path == item->text(0) ) neednew = false; //it is already present
        item = item->nextSibling();
      }
      if (folder->text(0) == "CVS") neednew = false;
  //    if (fname.isEmpty()) neednew = false;

      if ( neednew )
      {
        ProjectTreeFile *item = new ProjectTreeFile( folder, url.fileName(), url );
        item->setIcon(url);//QExtFileInfo::toAbsolute(url, baseURL));
      }
    }
    progressBar->advance(1);
  }
    
//now remove all the invalid items for the treeview
 if (!buildNewTree)
 {
  ProjectTreeFolder *folderItem;
  ProjectTreeFile *fileItem;
  QListViewItem *item;

  QListViewItemIterator iter(this);
  for ( ; iter.current(); ++iter )
  {
    item = iter.current();
    folderItem = dynamic_cast<ProjectTreeFolder *> (item);
    if ( folderItem )
    {
      url = folderItem->url;
    } else
    {
      fileItem = dynamic_cast<ProjectTreeFile *> (item);
      if ( fileItem )
      {
        url = fileItem->url;
      }
    }
    if (!urlList.contains(url) && item != projectDir)
    {
      delete item;
    }
    progressBar->advance(1);
  }
 }
  progressBar->setValue(0);
  progressBar->setTextEnabled(false);

  projectDir->sortChildItems(0,true);
  quantaApp->slotStatusMsg( i18n("Ready."));
}

void ProjectTreeView::slotOpen()
{
	QListViewItem *item =  currentItem();
	ProjectTreeFolder *f = dynamic_cast<ProjectTreeFolder *>(item);

	if (item && !f)
  {
   	KURL urlToOpen = currentURL();

   	if ( QuantaCommon::checkMimeGroup(urlToOpen,"text" ) )
   	{
   		emit openFile( urlToOpen, qConfig.defaultEncoding );
   	}
   	else if ( QuantaCommon::checkMimeGroup(urlToOpen,"image" ) )
   	{
   		emit activatePreview();
   		emit openImage( urlToOpen ); 
   	} else
      if (KMessageBox::questionYesNo(this,i18n("This file cannot be opened in Quanta. \n \
          Do you want to open with an external program or run it?"),i18n("Unknown type")) == KMessageBox::Yes)
      {
         KFileOpenWithHandler fowh;
        new KRun( urlToOpen, 0, true );
      }
 }
}

void ProjectTreeView::slotOpenWith()
{
	if (currentItem())
  {
   	KURL::List list;
  	KURL urlToOpen = currentURL();
  	list.append( urlToOpen );

  	KFileOpenWithHandler *kfowh = new KFileOpenWithHandler();
  	kfowh -> displayOpenWithDialog( list);
  }
}

void ProjectTreeView::slotOpenInQuanta()
{
 if (currentItem())
 {
   KURL urlToOpen = currentURL();
   if (urlToOpen.fileName().endsWith(toolbarExtension)) 
   {
      emit loadToolbarFile(urlToOpen);
      return;
   }

   if (QuantaCommon::checkMimeGroup(urlToOpen,"text"))
   {
     emit openFile( urlToOpen, qConfig.defaultEncoding );
   } else
   {
     denyBinaryInsert();
   }
 }
}

void ProjectTreeView::slotRemove()
{
  KURL url = currentURL();
  QListViewItem *item = currentItem();
	if ( item &&
       KMessageBox::warningYesNo(this,i18n("Do you really want to remove \n%1\nfrom disk ?")
                                      .arg(url.prettyURL()))== KMessageBox::Yes )
  {
    KIO::del( url );
    emit removeFromProject( url);
  }
}

void ProjectTreeView::slotRemoveFromProject(int askForRemove)
{
  KURL url = currentURL();
  QListViewItem *item = currentItem();
  if (item)
  {
    if ( !askForRemove ||
         KMessageBox::warningYesNo(this,i18n("Do you really want to remove \n%1\nfrom project ?")
                                        .arg(url.prettyURL())) == KMessageBox::Yes )
    {
      emit removeFromProject(url);
    }
  }
}

void ProjectTreeView::slotRename()
{
  if ( currentItem() )
  {
    emit renameInProject(currentURL());
  }
}


void ProjectTreeView::slotUploadSingleURL()
{
	if (currentItem())
  {
	  emit uploadSingleURL(currentURL());
  }
}

/** No descriptions */
void ProjectTreeView::slotRescan()
{
  emit rescanProjectDir();
}

/** Bring up the project options dialog */
void ProjectTreeView::slotOptions()
{
  emit showProjectOptions();
}

#include "projecttreeview.moc"
