/***************************************************************************
                          filestreeview.cpp  -  description
                             -------------------
    begin                : Thu Jun 1 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2001-2003  Andras Mantia
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
#include <qpopupmenu.h>
#include <qregexp.h>


// KDE includes
#include <krun.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kopenwith.h>
#include <kmessagebox.h>
#include <kpropertiesdialog.h>
#include <kcombobox.h>
#include <kurl.h>
#include <kdirlister.h>
#include <kfiledialog.h>

// app includes

#include "filemanage.h"
#include "filestreefile.h"
#include "filestreefolder.h"
#include "filestreeview.h"
#include "fileinfodlg.h"
#include "../resource.h"
#include "../quantacommon.h"
#include "../quanta.h"

FilesTreeView::FilesTreeView(KURL::List topList, QWidget *parent, const char *name)
  : FileManage(parent,name)
{
  topURLList = topList;

  setRootIsDecorated( true );
  setSorting(0);

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( i18n("Files Tree"), 600 );

  setFocusPolicy(QWidget::ClickFocus);
  folderMenu->insertItem(i18n("New Top Folder..."), this, SLOT(slotNewTopFolder()), 0, -1 , 0);
  folderMenu->insertItem(i18n("Add Folder to Top"), this ,SLOT(slotAddToTop()), 0, ID_TOP, 1);

  connect(  this, SIGNAL(executed(QListViewItem *)),
            this, SLOT(slotSelectFile(QListViewItem *)));
  connect(  this, SIGNAL(returnPressed(QListViewItem *)),
            this, SLOT(slotSelectFile(QListViewItem *)));
  connect(  this, SIGNAL(open(QListViewItem *)),
            this, SLOT(slotSelectFile(QListViewItem *)));

  connect(  this, SIGNAL(openInQuanta(QListViewItem *)),
            this, SLOT(slotSelectAnyFile(QListViewItem *)));

  connect(  this, SIGNAL(selectionChanged(QListViewItem *)),
            this, SLOT(slotSelectImage(QListViewItem *)));

  connect( this, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)),
           this, SLOT(slotMenu(QListViewItem*, const QPoint&, int)));


  // generate top list of directories
  QString s;
  for (uint i = 0; i <  topList.count(); i++)
  {
    KURL url = topURLList[i];
    if (url.isLocalFile() && url.path() == "/")
    {
      FilesTreeFolder *dir = new FilesTreeFolder( this, i18n("Root Directory"), url);
      dir->setPixmap( 0, SmallIcon("folder_red"));
       dir->setOpen( false);
    } else
      if (url.isLocalFile() && url.path() == QDir::homeDirPath()+"/")
      {
        FilesTreeFolder *dir = new FilesTreeFolder( this, i18n("Home Directory"), url);
        dir->setPixmap( 0, SmallIcon("folder_home"));
        dir->setOpen( true );
      } else
      {
        s = url.fileName();
        if (s.isEmpty())
          s = "/";
        s += " ["+url.prettyURL()+"]";
        FilesTreeFolder *dir = new FilesTreeFolder( this, s, url);
        dir->setPixmap( 0, SmallIcon("folder") );
        dir->setOpen( false);
      }
  }
}

FilesTreeView::~FilesTreeView()
{
}

/** Reload the current directory (not the full tree-view) */
void FilesTreeView::slotReload()
{

  FilesTreeFolder *d = dynamic_cast<FilesTreeFolder *>( currentItem()->parent());
  if (d)
  {
    d->reloadList();
  }
}

/** RMB pressed, bring up the menu */
void FilesTreeView::slotMenu(QListViewItem *item, const QPoint &point, int)
{
  if (item)
  {
    setSelected(item, true);

    FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
    if ( f )
    {
      fileMenu->popup( point);
    }

    FilesTreeFolder *d = dynamic_cast<FilesTreeFolder *>( item);
    if ( d )
    {
      if ( !d->parentFolder )
      {
        folderMenu ->changeItem( ID_TOP, i18n("Remove From Top"));
      }
      else
      {
        folderMenu ->changeItem( ID_TOP, i18n("Add Folder to Top"));
      }

      if ( d->text(0) == i18n("Home Directory") || d->text(0) == i18n("Root Directory") )
      {
        folderMenu ->setItemEnabled( ID_TOP, false);
      }
      else
      {
        folderMenu ->setItemEnabled( ID_TOP, true );
      }

      folderMenu->popup( point);
   }
 }
}

/** Returns the current url for files, and the full name with path for dirs */
KURL FilesTreeView::currentURL()
{
  KURL url;
  QListViewItem *item = currentItem();
   FilesTreeFolder *folderItem = dynamic_cast<FilesTreeFolder *> (item);
  if ( folderItem )
  {
    url = folderItem->url;
  } else
  {
     FilesTreeFile *fileItem = dynamic_cast<FilesTreeFile *> (item);
    if ( fileItem )
    {
      url = fileItem->url;
    }
  }

  return url;
}

/** Called for: double click, return, Open */
void FilesTreeView::slotSelectFile(QListViewItem *item)
{
  if (item)
  {
    FilesTreeFolder *parent = (FilesTreeFolder *) item->parent();
    if ( parent && !dynamic_cast<FilesTreeFolder *>(item) )
    {
       KURL urlToOpen = currentURL();
      if (!urlToOpen.isEmpty())
      {
        if ( QuantaCommon::checkMimeGroup(urlToOpen,"text") )
        {
          emit openFile( urlToOpen, quantaApp->defaultEncoding() );
        }
        else if ( QuantaCommon::checkMimeGroup(urlToOpen, "image") ) //it may be an image
             {
                emit activatePreview();
                emit openImage( urlToOpen );
             } else //it is an unknown type, maybe binary
             {
               if (denyBinaryInsert() == KMessageBox::Yes)
               {
                 emit openFile( urlToOpen, quantaApp->defaultEncoding() );
               }
             }
       }
    } //if ( parent && !dynamic_cast<FilesTreeFolder *>(item) )
  }//if (item)
}

/** Open in Quanta*/
void FilesTreeView::slotSelectAnyFile(QListViewItem *item)
{
  if (item )
  {
    FilesTreeFolder *parent = (FilesTreeFolder *) item->parent();
    if ( parent && !dynamic_cast<FilesTreeFolder *>(item) )
    {
       KURL urlToOpen = currentURL();
       if (!urlToOpen.isEmpty())
       {
         if (QuantaCommon::checkMimeGroup(urlToOpen, "text"))
         {
           emit openFile( urlToOpen, quantaApp->defaultEncoding() );
         } else     //Don't allow to open binary files
         {
           if (denyBinaryInsert() == KMessageBox::Yes)
           {
             emit openFile( urlToOpen, quantaApp->defaultEncoding() );
           }
         }
       } //if (!urlToOpen.isEmpty())
    }
  }
}

/** Load the image in preview area */
void FilesTreeView::slotSelectImage(QListViewItem *item)
{
  if (item)
  {
    KURL urlToOpen = currentURL();
    FilesTreeFolder *parent = (FilesTreeFolder *) item->parent();
    if ( parent && !dynamic_cast<FilesTreeFolder *>(item) && !urlToOpen.isEmpty() )
    {
      if ( QuantaCommon::checkMimeGroup(urlToOpen,"image") )
      {
        emit openImage(urlToOpen );
      }
    }
  }
}

/** Add or remove folders to/from the top list */
void FilesTreeView::slotAddToTop()
{
  FilesTreeFolder *d = dynamic_cast<FilesTreeFolder *>( currentItem() );
  if ( d )
  {
    KURL url(currentURL().url());
    if ( d->parentFolder )      //it is not a top folder
    { // add
      if (topURLList.findIndex(url) == -1)
      {
        url.setPath(url.path(-1));
        QString s = url.fileName();
        if (s.isEmpty())
            s = "/";
        s += " ["+url.prettyURL()+"]";
        FilesTreeFolder *dir = new FilesTreeFolder(this , s, url); //FIXME: Why doesn't add to the TOP as the first item??
         dir->setPixmap( 0, SmallIcon("folder") );
        dir->setOpen( false);
        topURLList.append(url);
      }
    } else
    { // remove
      url.adjustPath(-1);
      topURLList.remove(url);
      delete(d);
    }
  }
}

void FilesTreeView::slotNewTopFolder()
{
  KURL url = KFileDialog::getExistingURL(QString::null, this, i18n("Choose a local or remote directory"));
  if (topURLList.findIndex(url) == -1)
  {
    url.setPath(url.path(-1));
    QString s = url.fileName();
    if (s.isEmpty())
      s = "/";
    s += " ["+url.prettyURL()+"]";
    FilesTreeFolder *dir = new FilesTreeFolder(this , s, url);
    dir->setPixmap( 0, SmallIcon("folder") );
    dir->setOpen( false);
    topURLList.append(url);
  }
}

/** Properties dialog addon*/
void FilesTreeView::addFileInfoPage(KPropertiesDialog* propDlg)
{
  QListViewItem *item = currentItem();
  FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
//If the item is a file, add the Quanta file info page
  if (f)
  {

    QFrame *quantaFilePage = propDlg->dialog()->addPage(i18n("Quanta File Info"));
    QVBoxLayout *topLayout = new QVBoxLayout( quantaFilePage);
    FileInfoDlg *quantaFileProperties = new FileInfoDlg( quantaFilePage, i18n("Quanta") );

    int fsize,fimgsize=0;
    int ct=0,imgct=0,position=0;
    KURL u = currentURL();
    if (u.isLocalFile())   //get the file info only for local file. TODO: for non-local ones
    {
       QString nameForInfo = u.path();
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
    } //if localfile
  }
}

/** Bring up the properites dialog, and extend it for files */
void FilesTreeView::slotProperties()
{
  QListViewItem *item = currentItem();
  FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
  if (f)
  {
    KURL url = currentURL();
    KPropertiesDialog *propDlg = new KPropertiesDialog( url, this, 0L, false, false);
    addFileInfoPage(propDlg);
    if (propDlg->exec())
    {
      if (url != propDlg->kurl())
      {
        itemRenamed(url, propDlg->kurl());
      }
    }
    delete propDlg;
  } else
  {
    FileManage::slotProperties();
  }
}

/** No descriptions */
void FilesTreeView::slotInsertTag()
{
 if (currentItem())
 {
  // readDirInfo();
  emit insertTag( currentURL(), dirInfo);
 }
}

/** New items appeard to be inserted in the treeview */
void FilesTreeView::slotDirListNewItems(const KFileItemList& items)
{
  KFileItem * firstItem = const_cast<KFileItemList&>(items).first();
  KURL parentUrl = firstItem->url();
  QString parentKey = parentUrl.path();
  parentKey = parentKey.left(parentKey.findRev("/"))+"/";
  parentUrl.setPath(parentKey);
  QPtrList<QListViewItem> parents = getItems(parentUrl);

  QListViewItem *listItem;
  QPtrListIterator<KFileItem> it ( items );
  for( ; it.current(); ++it )
  {
    KFileItem *item = *it;
    KURL url = item->url();
    if (item->isDir())
    {
      FilesTreeFolder *dirItem;
      if (parents.count() > 0)  //add as a subdir for parents
      {
        for (listItem = parents.first(); listItem; listItem = parents.next())
        {
          FilesTreeFolder* parent = dynamic_cast<FilesTreeFolder*>(listItem);
          dirItem= new FilesTreeFolder(this, parent, url);
          dirItem->setPixmap( 0, SmallIcon("folder") );
        }
      } else
      {
        dirItem = new FilesTreeFolder(this, url.fileName(), url);
        dirItem->setPixmap( 0, SmallIcon("folder") );
      }
    } else
    if (!excludeFilterRx.exactMatch(item->name()))
    {
       FilesTreeFile *fileItem;
       if (parents.count() > 0) //add the files under the parents
       {
         for (listItem = parents.first(); listItem; listItem = parents.next())
         {
           FilesTreeFolder* parent = dynamic_cast<FilesTreeFolder*>(listItem);
           fileItem = new FilesTreeFile(parent, item->name(), url);
           fileItem->setIcon(url);
         }
       } else
       {
         fileItem = new FilesTreeFile(this, item->name(), url);
         fileItem->setIcon(url);
       }
    }
  }
}

/** An item was deleted */
void FilesTreeView::slotDirListDeleteItem(KFileItem *item)
{
  KURL url = item->url();
  if (item->isDir()) url.adjustPath(1);
  QPtrList<QListViewItem> items = getItems(url);

  //Delete all instances of this item
  QListViewItem *listItem;
  for (listItem = items.first(); listItem; listItem = items.next())
  {
    delete listItem;
  }
}

/** Read the directory specified by url. */
void FilesTreeView::readDir(const KURL& url)
{
  QPtrList<QListViewItem> items = getItems(url);

  QListViewItem *item;
  for (item = items.first(); item; item = items.next())
  {
    if (item && item->firstChild())    //if it is in our list and it's childern is in the list, we just update it
    {
     dirLister->updateDirectory(url);
    } else                            //otherwise reload completely
    {
     dirLister->openURL(url, true);
    }
  }
}

/** Get a list of items for whom the url == p_url. */
QPtrList<QListViewItem> FilesTreeView::getItems(const KURL& p_url)
{
  QPtrList<QListViewItem> items;
  QListViewItem *it;
  FilesTreeFile *fileItem = 0L;
  FilesTreeFolder *folderItem = 0L;

  QListViewItemIterator iter(this);
  for ( ; iter.current(); ++iter )
  {
    it = iter.current();
    QString  s = it->text(0);
    fileItem = dynamic_cast<FilesTreeFile*>(it);
    if (fileItem)
    {
      if (fileItem->url.url() == p_url.url())
         items.append(it);
    } else
    {
      folderItem = dynamic_cast<FilesTreeFolder*>(it);
      if (folderItem)
      {
        if (folderItem->url.url() == p_url.url())
           items.append(it);
      }
    }
  }

  return items;
}


#include "filestreeview.moc"
