/***************************************************************************
                          filestreeview.cpp  -  description
                             -------------------
    begin                : Thu Jun 1 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon & Andras Mantia
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
#include <qcheckbox.h>
#include <qdir.h>
#include <qpixmap.h>
#include <qheader.h>
#include <qframe.h>
#include <qlayout.h>
#include <qfileinfo.h>
#include <qtextedit.h>
#include <qlabel.h>
#include <qimage.h>

// KDE includes
#include <krun.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kopenwith.h>
#include <kmimetype.h>
#include <kmessagebox.h>
#include <kpropertiesdialog.h>
#include <kcombobox.h>

// app includes

#include "filemanage.h"
#include "filestreefile.h"
#include "filestreefolder.h"
#include "filestreeview.h"
#include "quantapropertiespagedlg.h"
#include "fileinfodlg.h"

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
		return ((FilesTreeFolder *)item)->fullName();
		
	return parent->fullName()+item->text(0);
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
   if (KMessageBox::questionYesNo(this,i18n("This file may be a binary file, thus cannot be opened \
    in Quanta correctly.\n Do you still want to open it?"),i18n("Wrong type")) != KMessageBox::Yes)
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
	
	QString nameToOpen = parent->fullName()+item->text(0);
	
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

void FilesTreeView::slotProperties()
{
  if ( !currentItem() ) return;

  KPropertiesDialog *propDlg = new KPropertiesDialog( KURL( currentFileName() ), this, 0L, false, false);

//Always add the Quanta directory page
  QFrame *quantaDirPage = propDlg->dialog()->addPage(i18n("Quanta directory"));
  QVBoxLayout *topLayout = new QVBoxLayout( quantaDirPage);
  quantaProperties = new QuantaPropertiesPageDlg( quantaDirPage, i18n("Quanta") );

  quantaProperties->typesCombo->insertItem("text/all");
  quantaProperties->typesCombo->insertItem("binary/all");
  quantaProperties->typesCombo->insertItem("template/all");

  readDirInfo();

  quantaProperties->typesCombo->setCurrentItem(dirInfo.mimeType);

  QListViewItem *item = currentItem();
  QString startDir = "";
	FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
	if ( f )
  {
   startDir = currentFileName();
  } else
  {
   startDir = currentFileName() + "/dummy_file";
  }
  QFileInfo dotFileInfo(QFileInfo(startDir).dirPath()+"/.dirinfo");
  if (!dotFileInfo.exists()) quantaProperties->parentAttr->setChecked(true);
  if (dirInfo.mimeType.isEmpty())
   {
    quantaProperties->parentAttr->setText(i18n("&Inherit parent attribute (nothing)"));
   } else
   {
    quantaProperties->parentAttr->setText(i18n("&Inherit parent attribute (%1)").arg(dirInfo.mimeType));
   }
   if (!dirInfo.preText.isEmpty())
   {
    quantaProperties->preTextEdit->setText(dirInfo.preText);
    quantaProperties->usePreText->setChecked(true);
   }
   if (!dirInfo.postText.isEmpty())
   {
    quantaProperties->postTextEdit->setText(dirInfo.postText);
    quantaProperties->usePostText->setChecked(true);
   }

   topLayout->addWidget( quantaProperties );
//   connect( propDlg, SIGNAL( applied() ), this , SLOT( slotPropertiesApplied) );


//If the item is a file, add the Quanta file info page
  if (f)
  {
    QFrame *quantaFilePage = propDlg->dialog()->addPage(i18n("Quanta file info"));
    QVBoxLayout *topLayout = new QVBoxLayout( quantaFilePage);
    FileInfoDlg *quantaFileProperties = new FileInfoDlg( quantaFilePage, i18n("Quanta") );

    int fsize,fimgsize=0;
    int ct=0,imgct=0,position=0;
    QString nameForInfo = currentFileName();
    KURL u(nameForInfo);
    QString path =u.directory(0,0);       //extract path for images
    QString sourcename=u.fileName(0);

    QFile qfile(nameForInfo);
    fsize=qfile.size();                              //html file size

    QString mimetype = KMimeType::findByFileContent(nameForInfo)->name();
    if (mimetype.contains("text"))
    {
     qfile.open(IO_ReadOnly);
     QString imgname,imgpath;
     while (qfile.readLine(imgname,200)!=-1)     //open & parse file
     {
       ct++;
       position=imgname.find("<img",0,FALSE);              //check for images
       if (position!=-1)
       {
         imgname.remove(0,position+4);
         position=imgname.find("src=",0,FALSE);              //extract images names
         imgname.remove(0,position+4);
         if (imgname.startsWith("\"")) imgname.remove(0,1);
         if (imgname.startsWith("'")) imgname.remove(0,1);
         position=imgname.find(" ",0,FALSE);
         if (position!=-1) imgname=imgname.left(position);
         position=imgname.find(">",0,FALSE);
         if (position!=-1) imgname=imgname.left(position);
         position=imgname.find("\"",0,FALSE);
         if (position!=-1) imgname=imgname.left(position);
         position=imgname.find("'",0,FALSE);
         if (position!=-1) imgname=imgname.left(position);
         if (!quantaFileProperties->imageList->findItem(imgname,Qt::ExactMatch))     //check if image was already counted
         {
           KURL v(path,imgname);
           imgpath=v.path();
           QFile qimage(imgpath);
           if (qimage.exists() && v.isLocalFile())
           {
            fimgsize+=qimage.size();
            quantaFileProperties->imageList->insertItem(imgname);
            imgct++;
           }
          }
        }
      }
      qfile.close();

      quantaFileProperties->lineNum->setText(i18n("Number of lines: %1").arg(ct));
      quantaFileProperties->imageNum->setText(i18n("Number of images included: %1").arg(imgct));
      quantaFileProperties->imageSize->setText(i18n("Size of the included images: %1 bytes").arg(fimgsize));
      quantaFileProperties->totalSize->setText(i18n("Total size with images: %1 bytes").arg(fsize+fimgsize));
     }
     else
     if (mimetype.contains("image"))
     {              // assume it's an image file
       QImage imagefile=QImage(nameForInfo);
       quantaFileProperties->lineNum->setText(i18n("Image size: %1 x %2").arg(imagefile.width()).arg(imagefile.height()));
       quantaFileProperties->imageNum->hide();
       quantaFileProperties->imageSize->hide();
       quantaFileProperties->totalSize->hide();
       quantaFileProperties->includedLabel->hide();
       quantaFileProperties->imageList->hide();
     }
    topLayout->addWidget(quantaFileProperties);
  }

   if (propDlg->exec())
   {
    slotPropertiesApplied();
    slotReload();
   }

  delete propDlg;
}


/** No descriptions */
void FilesTreeView::slotPropertiesApplied()
{
  bool changed = false;
  DirInfo localDirInfo;

  if (!quantaProperties->parentAttr->isChecked())
  {
    localDirInfo.mimeType = quantaProperties->typesCombo->currentText();
  } else
  {
    localDirInfo.mimeType = dirInfo.mimeType;
  }

  if (quantaProperties->usePreText->isChecked())
  {
    localDirInfo.preText = quantaProperties->preTextEdit->text();
  } else
  {
    localDirInfo.preText = dirInfo.preText;
  }
  if (quantaProperties->usePostText->isChecked())
  {
    localDirInfo.postText = quantaProperties->postTextEdit->text();
  } else
  {
    localDirInfo.postText = dirInfo.postText;
  }

  if ( (dirInfo.mimeType != localDirInfo.mimeType) ||
       (dirInfo.preText != localDirInfo.preText) ||
       (dirInfo.postText != localDirInfo.postText))
  {
    dirInfo.mimeType = localDirInfo.mimeType;
    dirInfo.preText = localDirInfo.preText;
    dirInfo.postText = localDirInfo.postText;
    writeDirInfo();
  }


}

/** No descriptions */
void FilesTreeView::readDirInfo()
{
  QListViewItem *item = currentItem();
  QString startDir = "";

	FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
	if ( f )
  {
   startDir = currentFileName();
  } else
  {
   startDir = currentFileName() + "/dummy_file";
  }

  QFileInfo dotFileInfo(QFileInfo(startDir).dirPath()+"/.dirinfo");

  while ((!dotFileInfo.exists()) && (dotFileInfo.dirPath() != "/"))
  {
   dotFileInfo.setFile(QFileInfo(dotFileInfo.dirPath()).dirPath()+"/.dirinfo");
  }

  KConfig *config = new KConfig(dotFileInfo.filePath());
  dirInfo.mimeType = config->readEntry("Type");
  dirInfo.preText = config->readEntry("PreText");
  dirInfo.postText = config->readEntry("PostText");

  delete config;
}

/** No descriptions */
void FilesTreeView::writeDirInfo(QString dirInfoFile)
{
  QListViewItem *item = currentItem();
  QString startDir = "";

  if (dirInfoFile.isEmpty())
  {
	  FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
	  if ( f )
    {
      startDir = currentFileName();
    } else
    {
      startDir = currentFileName() + "/dummy_file";
    }
  } else
  {
    startDir = dirInfoFile;
  }

  QFileInfo dotFileInfo(QFileInfo(startDir).dirPath()+"/.dirinfo");

  KConfig *config = new KConfig(dotFileInfo.filePath());
  config->writeEntry("Type", dirInfo.mimeType);
  config->writeEntry("PreText", dirInfo.preText);
  config->writeEntry("PostText", dirInfo.postText);
  config->sync();

  delete config;
}

#include "filestreeview.moc"
/** No descriptions */
void FilesTreeView::slotInsertTag()
{
 if (!currentItem()) return;

 readDirInfo();
 emit insertTag( currentFileName(), dirInfo);
}
