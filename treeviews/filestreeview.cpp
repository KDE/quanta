/***************************************************************************
                          filestreeview.cpp  -  description
                             -------------------
    begin                : Thu Jun 1 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2003 Andras Mantia <amantoa@kde.org>
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
#include <qlayout.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qimage.h>
#include <qregexp.h>
#include <qclipboard.h>
#include <qpoint.h>
#include <qregexp.h>


// KDE includes
#include <kdebug.h>
#include <krun.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kopenwith.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kpropertiesdialog.h>
#include <kfiledialog.h>
#include <kprogress.h>
#include <kurldrag.h>
#include <kurl.h>

// app includes

#include "filestreeview.h"
#include "fileinfodlg.h"
#include "../project/project.h"
#include "../resource.h"
#include "../quanta.h"

//FilesTreeViewItem implementation
FilesTreeViewItem::FilesTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch )
: KFileTreeViewItem( parent, item, brnch)
{
}

QString FilesTreeViewItem::key(int column, bool ascending) const
{
  Q_UNUSED(ascending);
  if (isDir())
    return text(column).prepend(" ");
  else
    return text(column);
}

//FilesTreeBranch implementation
FilesTreeBranch::FilesTreeBranch(KFileTreeView *parent, const KURL& url,
                                 const QString& name, const QPixmap& pix,
                                 bool showHidden ,
                                 KFileTreeViewItem *branchRoot)
  : KFileTreeBranch(parent, url, name, pix, showHidden, branchRoot)
{
  setAutoUpdate(url.isLocalFile());
  setChildRecurse(false);
}

bool FilesTreeBranch::matchesFilter( const KFileItem *item ) const
{
  if (!urlList.isEmpty())
    return urlList.contains(item->url());

  if (!excludeFilterRx.isEmpty())
    if (excludeFilterRx.exactMatch(item->url().path()))
      return false;

  return KFileTreeBranch::matchesFilter(item);
}

KFileTreeViewItem* FilesTreeBranch::createTreeViewItem( KFileTreeViewItem *parent,
                                                        KFileItem *fileItem )
{
  FilesTreeViewItem  *tvi = 0;
  if( parent && fileItem )
  {
    tvi = new FilesTreeViewItem( parent, fileItem, this );
    // we assume there are childs
    if (tvi)
      tvi->setExpandable(tvi->isDir());
  }
  else
    kdDebug(24000) << "createTreeViewItem: Have no parent" << endl;
  return tvi;
}

//FilesTreeView implementation
FilesTreeView::FilesTreeView(QWidget *parent, const char *name)
: KFileTreeView(parent, name)
{
}

FilesTreeView::FilesTreeView(KURL::List topList, QWidget *parent, const char *name)
  : KFileTreeView(parent, name)
{
  setRootIsDecorated( true );
  setSorting(0);
  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  setFocusPolicy(QWidget::ClickFocus);
  setAcceptDrops(true);

  topURLList = topList;

  m_fileMenu = new KPopupMenu();

  m_fileMenu->insertItem(SmallIcon("fileopen"), i18n("&Open"), this ,SLOT(slotOpen()));
  m_fileMenu->insertItem(i18n("Open With..."), this, SLOT(slotOpenWith()));
  m_fileMenu->insertItem(i18n("Insert Tag"), this, SLOT(slotInsertTag()));
  m_fileMenu->insertSeparator();
  m_insertFileInProject = m_fileMenu->insertItem(i18n("Insert in Project..."), this, SLOT(slotInsertInProject()));
  m_fileMenu->insertItem(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));
  m_fileMenu->insertItem(SmallIcon("editpaste"), i18n("&Paste"), this, SLOT(slotPaste()));
  m_fileMenu->insertItem(SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_fileMenu->insertItem(SmallIcon("info"), i18n("Properties"), this, SLOT(slotProperties()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(i18n("Reload"), this, SLOT(slotReload()));


  m_folderMenu = new KPopupMenu();

  m_folderMenu->insertItem(SmallIcon("folder_new"), i18n("New Top Folder..."), this, SLOT(slotNewTopFolder()));
  m_menuTop = m_folderMenu->insertItem(i18n("Add Folder to Top"), this, SLOT(slotAddToTop()));
  m_folderMenu->insertSeparator();
  m_insertFolderInProject = m_folderMenu->insertItem(i18n("Insert in Project..."), this, SLOT(slotInsertDirInProject()));
  m_folderMenu->insertItem(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));
  m_folderMenu->insertItem(SmallIcon("editpaste"), i18n("&Paste"), this, SLOT(slotPaste()));
  m_menuDel = m_folderMenu->insertItem( SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_folderMenu->insertItem(SmallIcon("info"), i18n("Properties"), this, SLOT(slotProperties()));
  m_folderMenu->insertSeparator();
  m_folderMenu->insertItem(i18n("Reload"), this, SLOT(slotReload()));

  m_emptyMenu = new KPopupMenu();

  m_emptyMenu->insertItem(i18n("New Top Folder..."), this, SLOT(slotNewTopFolder()), 0, -1 , 0);

  addColumn(i18n("Files Tree"), 600);

  connect(this, SIGNAL(executed(QListViewItem *)),
          this, SLOT(slotSelectFile(QListViewItem *)));
  connect(this, SIGNAL(returnPressed(QListViewItem *)),
          this, SLOT(slotSelectFile(QListViewItem *)));
  connect(this, SIGNAL(open(QListViewItem *)),
          this, SLOT(slotSelectFile(QListViewItem *)));
  connect(this, SIGNAL(openInQuanta(QListViewItem *)),
          this, SLOT(slotSelectFile(QListViewItem *)));
  connect(this, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
          this, SLOT(slotMenu(KListView*, QListViewItem*, const QPoint&)));

  // generate top list of directories
  for (uint i = 0; i <  topURLList.count(); i++)
  {
    KURL url = topURLList[i];
    newBranch(url);
  }
}

FilesTreeView::~FilesTreeView()
{
}

KFileTreeBranch* FilesTreeView::newBranch(const KURL& url)
{
  FilesTreeBranch* newBrnch = 0;
  if (url.isLocalFile() && url.path() == "/")
  {
    newBrnch = new FilesTreeBranch(this, url, i18n("Root Folder"), SmallIcon("folder_red"));
  } else
  {
    if (url.isLocalFile() && url.equals(KURL(QDir::homeDirPath()), true))
    {
      newBrnch = new FilesTreeBranch(this, url, i18n("Home Folder"), SmallIcon("home_blue"));
    } else
    {
      QString s = url.fileName();
      if (s.isEmpty())
        s = "/";
      s += " ["+url.prettyURL()+"]";
      KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, url);
      newBrnch = new FilesTreeBranch(this, url, s, SmallIcon(fileItem.iconName()));
    }
  }
  addBranch(newBrnch);
  return newBrnch;
}

void FilesTreeView::itemRenamed(const KURL& , const KURL& )
{
}

int FilesTreeView::denyBinaryInsert()
{
  int result = KMessageBox::warningYesNo(this, i18n("The file type is not recognized. \
  Opening binary files may confuse Quanta.\n Are you sure you want to open this file?"),
  i18n("Unknown Type"), KStdGuiItem::yes(), KStdGuiItem::no(), "Open Everything");
 return result;
}

/** RMB pressed, bring up the menu */
void FilesTreeView::slotMenu(KListView* listView, QListViewItem *item, const QPoint &point)
{
  Q_UNUSED(listView);
  if (item)
  {
    m_folderMenu->setItemEnabled(m_insertFolderInProject, quantaApp->project()->hasProject());
    m_fileMenu->setItemEnabled(m_insertFileInProject, quantaApp->project()->hasProject());
    setSelected(item, true);
    KFileTreeViewItem *curItem = currentKFileTreeViewItem();
    if ( !curItem->isDir() )
    {
      m_fileMenu->popup( point);
    } else {
      if ( curItem == curItem->branch()->root() )
      {
        m_folderMenu ->setItemEnabled( m_menuDel, false);
        m_folderMenu ->changeItem( m_menuTop, i18n("Remove From Top"));
      }
      else
      {
        m_folderMenu ->setItemEnabled( m_menuDel, true );
        m_folderMenu ->changeItem( m_menuTop, i18n("Add Folder to Top"));
      }
      m_folderMenu->popup( point);
   }
 } else {
   m_emptyMenu->popup( point );
 }
}

/** Called for: double click, return, Open */
void FilesTreeView::slotSelectFile(QListViewItem *item)
{
  KFileTreeViewItem* kftvi = dynamic_cast <KFileTreeViewItem*> (item);
  if (!kftvi || kftvi->isDir() ) return;

  KURL urlToOpen = kftvi->url();
  if (!urlToOpen.isEmpty())
  {
    if ( QuantaCommon::checkMimeGroup(urlToOpen,"text") )
    {
      emit openFile( urlToOpen, quantaApp->defaultEncoding() );
    }
    else if ( QuantaCommon::checkMimeGroup(urlToOpen, "image") ) //it may be an image
         {
            emit openImage( urlToOpen );
         } else //it is an unknown type, maybe binary
         {
           if (denyBinaryInsert() == KMessageBox::Yes)
           {
             emit openFile( urlToOpen, quantaApp->defaultEncoding() );
           }
         }
   }
}


/** Add or remove folders to/from the top list */
void FilesTreeView::slotAddToTop()
{
  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if ( !curItem ) return;

  if ( curItem->isDir() )
  {
    KURL url(currentURL().url());
    url.adjustPath(+1);
    if ( curItem != curItem->branch()->root() ) //it is not a top folder
    { // add
      if (topURLList.findIndex(url) == -1)
      {
        topURLList.append(url);
        newBranch(url);
      }
    } else
    { // remove
      topURLList.remove(url);
      removeBranch(curItem->branch());
    }
  }
}

void FilesTreeView::slotNewTopFolder()
{
  KURL url = KFileDialog::getExistingURL(QString::null, this, i18n("Choose Local or Remote Folder"));
  if (!url.isEmpty() && topURLList.findIndex(url) == -1)
  {
    newBranch(url);
    topURLList.append(url);
  }
}

/** Properties dialog addon*/
void FilesTreeView::addFileInfoPage(KPropertiesDialog* propDlg)
{
//If the item is a file, add the Quanta file info page
  if ( !currentKFileTreeViewItem()->isDir() )
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

void FilesTreeView::slotOpen()
{
  QListViewItem *item = currentItem();
  if (item)
  {
    emit open(item);
  }
}

void FilesTreeView::slotOpenWith()
{
  if (currentItem())
  {
    KURL::List list;
    KURL urlToOpen = currentURL();
    list.append( urlToOpen );
    KRun::displayOpenWithDialog( list );
  }
}

void FilesTreeView::slotInsertTag()
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

void FilesTreeView::slotCopy()
{
   if (currentItem())
   {
     QClipboard *cb = QApplication::clipboard();
     cb->setText( currentURL().prettyURL() );
   }
}

void FilesTreeView::slotPaste()
{
  if (currentItem())
  {
    QClipboard *cb = QApplication::clipboard();
    KURL::List list( QStringList::split( QChar('\n'), cb->text() ) );

    KURL url = currentURL();
    url.setFileName("");   // don't paste on files but in dirs
    KIO::Job *job = KIO::copy( list, url);
    connect( job, SIGNAL( result( KIO::Job *) ), this , SLOT( slotJobFinished( KIO::Job *) ) );
    progressBar->setTotalSteps(100);
    connect( job, SIGNAL( percent( KIO::Job *, unsigned long)),
             this, SLOT( slotPercent( KIO::Job *, unsigned long)));
  }
}

void FilesTreeView::slotPercent(KIO::Job *job, unsigned long value)
{
  Q_UNUSED(job);
  progressBar->setProgress(value);
}

void FilesTreeView::slotDelete()
{
  if (currentItem())
  {
    KURL url = currentURL();
    QString msg;
    if ( currentKFileTreeViewItem()->isDir() )
      msg = i18n("<qt>Do you really want to delete folder <b>%1</b> ?</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol));
    else
      msg = i18n("<qt>Do you really want to delete file <b>%1</b> ?</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol));

    if ( KMessageBox::warningYesNo(this, msg) == KMessageBox::Yes )
    {
      KIO::Job *job = KIO::del(url);
      connect( job, SIGNAL( result( KIO::Job *) ), this , SLOT( slotJobFinished( KIO::Job *) ) );
      progressBar->setTotalSteps(100);
      connect( job, SIGNAL( percent( KIO::Job *, unsigned long)),
               this, SLOT( slotPercent( KIO::Job *, unsigned long)));
    }
  }
}

void FilesTreeView::slotOpenInQuanta()
{
  QListViewItem *item = currentItem();
  if (item)
  {
    kdDebug(24000) << "Open In Quanta: " << endl;
    emit openInQuanta(item);
  }
}
void FilesTreeView::slotPopulateFinished(KFileTreeViewItem *item)
{
  progressBar->reset();

  if ( !item ) return;

  if( item->isDir() )
    stopAnimation( item );

  if (item->childCount() == 0) {
    item->setOpen(false);
    item->setExpandable(false);
  } else {
    if ( !item->isOpen() ) item->setOpen(true);
  }
}

void FilesTreeView::slotInsertInProject()
{
  if (currentItem() )
  {
    emit insertFileInProject( currentURL());
  }
}

void FilesTreeView::slotInsertDirInProject()
{
  if ( currentItem() )
  {
    emit insertDirInProject( currentURL() );
  }
}

/** Bring up the properites dialog, and extend it for files */
void FilesTreeView::slotProperties()
{
  KURL url = currentURL();
  if (url.isEmpty()) return;

  if (!currentKFileTreeViewItem()->isDir())
  {
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
    KPropertiesDialog *propDlg = new KPropertiesDialog(url, this);
    Q_UNUSED(propDlg);
  }
}



void FilesTreeView::slotReload()
{
    QListViewItemIterator it(this);
    KFileTreeViewItem *kftvi;
    for ( ; it.current(); ++it )
    {
      kftvi = dynamic_cast<KFileTreeViewItem*> (it.current());
      if ( kftvi && kftvi->isDir() && kftvi->isOpen()) {
        kftvi->setListed( false ); // force re-read
        kftvi->branch()->populate(kftvi->url(), kftvi);
      }
    }
}


void FilesTreeView::slotJobFinished(KIO::Job *job)
{
  if ( job->error() )
      job->showErrorDialog(this);

  progressBar->reset();

  slotReload();
//  emit reloadTreeviews();
}


/** No descriptions */
void FilesTreeView::contentsDropEvent(QDropEvent *e)
{
 KFileTreeViewItem *item = dynamic_cast <KFileTreeViewItem *> ( itemAt(contentsToViewport(e->pos())) );

 if ( ! item) return;

 KURL dest;
 if ( item->isDir() )
 {
   dest = item->url();
 }
 else
 {
   dest = item->url().directory(false);
 }
 if (KURLDrag::canDecode(e))
 {
//     KURL source;
     KURL::List fileList;

     KURLDrag::decode(e, fileList);    //TODO: Make it workable for non local files
     if(fileList.empty()) return;

     KIO::Job *job = KIO::copy(fileList,dest);
     connect( job, SIGNAL( result( KIO::Job *) ), this , SLOT( slotJobFinished( KIO::Job *) ) );
     progressBar->setTotalSteps(100);
     connect( job, SIGNAL( percent( KIO::Job *, unsigned long)),
              this, SLOT( slotPercent( KIO::Job *, unsigned long)));
 }
}

/** No descriptions */
void FilesTreeView::contentsDragEnterEvent(QDragEnterEvent *event)
{
 if (KURLDrag::canDecode(event) )
 {
    event->accept();
 }
}

#include "filestreeview.moc"
