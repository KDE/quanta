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
#include <qlineedit.h>
#include <qfont.h>
#include <qpainter.h>
#include <qevent.h>


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
#include "resource.h"
#include "quanta.h"
#include "quantacommon.h"
#include "quantadoc.h"
#include "qextfileinfo.h"


//FilesTreeViewItem implementation
FilesTreeViewItem::FilesTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch )
: KFileTreeViewItem( parent, item, brnch)
{
}

QString FilesTreeViewItem::key(int column, bool ascending) const
{
  if (ascending) 
    if (isDir())
      return text(column).prepend(" ");
    else
      return text(column);
  else  
    if (isDir())
      return text(column);
    else
      return text(column).prepend(" ");
}

int FilesTreeViewItem::compare( QListViewItem *i, int col,
                                bool ascending ) const
{
  return key( col, ascending ).compare( i->key( col, ascending) );
}

void FilesTreeViewItem::paintCell(QPainter *p, const QColorGroup &cg,
                                  int column, int width, int align)
{
  QColorGroup _cg( cg );
  if (column == 0)
  {
    QFont f = p->font();
    f.setBold(quantaApp->doc()->isOpened(url()));
    p->setFont(f);
  } else
  {
    int h, s, v;
#if KDE_IS_VERSION(3,1,90)    
    p->pen().color().getHsv(&h, &s, &v);
#else
    p->pen().color().getHsv(h, s, v);
#endif    
    v = (v < 155 ? v + 100 : 255);
    _cg.setColor(QColorGroup::Text, QColor(h, s, v, QColor::Hsv));
  };
  KFileTreeViewItem::paintCell( p, _cg, column, width, align );
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
    {
      tvi->setExpandable(tvi->isDir());
    }
  }
  else
    kdDebug(24000) << "createTreeViewItem: Have no parent" << endl;
  return tvi;
}

//FilesTreeView implementation
FilesTreeView::FilesTreeView(QWidget *parent, const char *name)
: KFileTreeView(parent, name)
{
  m_dock = 0L;
  m_action = 0L;
}

FilesTreeView::FilesTreeView(KConfig *config, KActionCollection *ac, KDockWidget *parent, const char *name)
  : KFileTreeView(parent, name)
{
  m_dock = parent;
  m_action = new KToggleAction( i18n("Show Files Tree"), UserIcon("ftab"), 0,
                                this, SLOT( slotToggleShow() ),
                                ac, "show_ftab_tree" );
  setRootIsDecorated( true );
  setSorting(0);
  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  setFocusPolicy(QWidget::ClickFocus);
  setAcceptDrops(true);
  setShowSortIndicator(true);
  setDragEnabled(true);
  
  connect(this, SIGNAL(dropped(KURL::List&, KURL&)),
          this, SLOT(slotDropped(KURL::List&, KURL&)));

  m_config = config;
  m_config->setGroup("General Options");
  QStringList topStrList;
#if KDE_IS_VERSION(3,1,3)
  topStrList = m_config->readPathListEntry("Top folders");
#else
  topStrList = m_config->readListEntry("Top folders");
#endif
  KURL url;
  for (uint i = 0; i < topStrList.count(); i++)
  {
    url = KURL();
    QuantaCommon::setUrl(url, topStrList[i]);
    topURLList.append(url);
  }

  m_config->setGroup("General Options");
  if (m_config->readBoolEntry("Home-Root Folder On", true))
  {
    url = KURL();
    url.setPath("/");
    if (!topURLList.contains(url))
        topURLList.append(url);
    url = KURL();
    url.setPath(QDir::homeDirPath()+"/");
    if (!topURLList.contains(url))
        topURLList.append(url);
  }

  m_fileMenu = new KPopupMenu();

  m_fileMenu->insertItem(SmallIcon("fileopen"), i18n("&Open"), this ,SLOT(slotOpen()));
  m_fileMenu->insertItem(i18n("Open &With..."), this, SLOT(slotOpenWith()));
  m_fileMenu->insertItem(i18n("Insert &Tag"), this, SLOT(slotInsertTag()));
  m_menuClose = m_fileMenu->insertItem(SmallIcon("fileclose"), i18n("Clos&e"), this, SLOT(slotClose()));
  m_fileMenu->insertSeparator();
  m_insertFileInProject = m_fileMenu->insertItem(i18n("&Insert in Project..."), this, SLOT(slotInsertInProject()));
  m_fileMenu->insertItem(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));
  m_fileMenu->insertItem(SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(SmallIcon("info"), i18n("&Properties..."), this, SLOT(slotProperties()));

  m_folderMenu = new KPopupMenu();

  m_folderMenu->insertItem(SmallIcon("folder_new"), i18n("&New Top Folder..."), this, SLOT(slotNewTopFolder()));
  m_menuTop = m_folderMenu->insertItem(i18n("&Add Folder to Top"), this, SLOT(slotAddToTop()));
  m_folderMenu->insertSeparator();
  m_insertFolderInProject = m_folderMenu->insertItem(i18n("&Insert in Project..."), this, SLOT(slotInsertDirInProject()));
  m_folderMenu->insertItem(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));
  m_menuPasteFolder = m_folderMenu->insertItem(SmallIcon("editpaste"), i18n("&Paste"), this, SLOT(slotPaste()));
  m_menuDel = m_folderMenu->insertItem( SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_folderMenu->insertSeparator();
  m_folderMenu->insertItem(SmallIcon("info"), i18n("&Properties..."), this, SLOT(slotProperties()));
  m_reloadMenuId = m_folderMenu->insertItem(i18n("&Reload"), this, SLOT(slotReload()));

  m_emptyMenu = new KPopupMenu();

  m_emptyMenu->insertItem(i18n("&New Top Folder..."), this, SLOT(slotNewTopFolder()), 0, -1 , 0);

  addColumn(i18n("Files Tree"), -1);
  addColumn("");
  setFullWidth(true);

  connect(this, SIGNAL(executed(QListViewItem *)),
          this, SLOT(slotSelectFile(QListViewItem *)));
  connect(this, SIGNAL(returnPressed(QListViewItem *)),
          this, SLOT(slotReturnPressed(QListViewItem *)));
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
    newBrnch = new FilesTreeBranch(this, url, i18n("Root Folder"), SmallIcon("folder_red"), true);
  } else
  {
    if (url.isLocalFile() && url.equals(KURL(QDir::homeDirPath()), true))
    {
      newBrnch = new FilesTreeBranch(this, url, i18n("Home Folder"), SmallIcon("home_blue"), true);
    } else
    {
      QString s = url.fileName();
      if (s.isEmpty())
        s = "/";
      s += " ["+url.prettyURL()+"]";
      KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, url);
      newBrnch = new FilesTreeBranch(this, url, s, SmallIcon(fileItem.iconName()), true);
    }
  }
  addBranch(newBrnch);
  return newBrnch;
}

void FilesTreeView::itemRenamed(const KURL& oldURL, const KURL& newURL )
{
  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if (! curItem) return;

  if (curItem->isDir())
  {
    KURL n = newURL;
    n.adjustPath(1);
    KURL o = oldURL;
    o.adjustPath(1);
    emit renamed(o, n);
  }
  else
  {
    emit renamed(oldURL, newURL);
  }
}

void FilesTreeView::itemDescChanged(KFileTreeViewItem* , const QString& )
{
}

/** RMB pressed, bring up the menu */
void FilesTreeView::slotMenu(KListView* listView, QListViewItem *item, const QPoint &point)
{
  Q_UNUSED(listView);
  if (item)
  {
    bool hasProject = !m_projectName.isNull();
    m_folderMenu->setItemVisible(m_insertFolderInProject, hasProject);
    m_fileMenu->setItemVisible(m_insertFileInProject, hasProject);
    setSelected(item, true);
    KFileTreeViewItem *curItem = currentKFileTreeViewItem();
    if ( !curItem->isDir() )
    {
      m_fileMenu->setItemVisible(m_menuClose, isFileOpen(currentURL()));
      m_fileMenu->popup( point);
    } else {
      m_folderMenu ->setItemVisible( m_menuDel, true );
      m_folderMenu ->setItemVisible( m_menuTop, true );
      m_folderMenu->setItemVisible(m_menuPasteFolder, isPathInClipboard());
      if ( curItem == curItem->branch()->root() )
      {
        m_folderMenu ->setItemVisible( m_menuDel, false);
        m_folderMenu ->changeItem( m_menuTop, i18n("Remove From Top"));

        m_config->setGroup("General Options");
        KURL url = curItem->url();
        if ((url == KURL("file:/") || url == KURL("file:" + QDir::homeDirPath()+"/")) &&
            m_config->readBoolEntry("Home-Root Folder On", true) )
          m_folderMenu ->setItemVisible(m_menuTop, false);
        m_folderMenu ->setItemVisible(m_reloadMenuId, true);
      }
      else
      {
        m_folderMenu ->changeItem( m_menuTop, i18n("Add Folder to Top"));
        m_folderMenu ->setItemVisible(m_reloadMenuId, false);
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
  Q_UNUSED(item);

  KFileTreeViewItem* kftvi = currentKFileTreeViewItem();
  if (!kftvi || kftvi->isDir() ) return;

  KURL urlToOpen = kftvi->url();
  if (!urlToOpen.isEmpty())
  {
    if ( QuantaCommon::checkMimeGroup(urlToOpen,"text") )
    {
      emit openFile(urlToOpen);
    }
    else if ( QuantaCommon::checkMimeGroup(urlToOpen, "image") ) //it may be an image
      {
         emit openImage( urlToOpen );
      }
      else if ( expandArchiv(kftvi) ) //it may be an archiv
        {
        }
        else //it is an unknown type, maybe binary
        {
          if (QuantaCommon::denyBinaryInsert() == KMessageBox::Yes)
          {
            emit openFile(urlToOpen);
          }
        }
   }
}


/** expands an archiv, if possible */
bool FilesTreeView::expandArchiv (KFileTreeViewItem *item)
{
  if (!item) return false;
  KURL urlToOpen = item->url();

  if ( ! urlToOpen.isLocalFile()) return false;

  QString mimeType = KMimeType::findByURL(urlToOpen)->name();

  if ( mimeType == "application/x-tgz" ||
       mimeType == "application/x-tbz" ||
       mimeType == "application/x-tar" ) //it is an archiv
    urlToOpen.setProtocol("tar");
  else
    if ( mimeType == "application/x-zip" ) //it is an archiv
      urlToOpen.setProtocol("zip");
    else
      return false;

  // change status if there is already a sub branch
  if (item->isExpandable()) {
    item->setOpen( ! item->isOpen());
    return true;
  };

  KFileTreeBranch *kftb = new FilesTreeBranch(this, urlToOpen, item->text(0), *(item->pixmap(0)), true, item);
  addBranch(kftb);  // connecting some signals
  kftb->populate(urlToOpen, item);
  item->setExpandable(true);
  item->setOpen(true);
  return true;
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
    if ( curItem != curItem->branch()->root() )  //it is not a top folder
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
  url.adjustPath(+1);
  if (!url.isEmpty() && topURLList.findIndex(url) == -1)
  {
    newBranch(url);
    topURLList.append(url);
  }
}

/** Properties dialog addon*/
const FileInfoDlg* FilesTreeView::addFileInfoPage(KPropertiesDialog* propDlg)
{
  //If the item is a file, add the Quanta file info page
  FileInfoDlg *quantaFileProperties = 0L;
  if ( !currentKFileTreeViewItem()->isDir() )
  {

    QFrame *quantaFilePage = propDlg->addPage(i18n("Quanta File Info"));
    QVBoxLayout *topLayout = new QVBoxLayout( quantaFilePage);
    quantaFileProperties = new FileInfoDlg( quantaFilePage, i18n("Quanta") );

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
              KURL v(KURL::fromPathOrURL( path ),imgname);
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
        else if (mimetype.contains("image"))
        {              // assume it's an image file
          QImage imagefile=QImage(nameForInfo);
          quantaFileProperties->lineNum->setText(i18n("Image size: %1 x %2").arg(imagefile.width()).arg(imagefile.height()));
          quantaFileProperties->imageNum->hide();
          quantaFileProperties->imageSize->hide();
          quantaFileProperties->totalSize->hide();
          quantaFileProperties->includedLabel->hide();
          quantaFileProperties->imageList->hide();
        }
        if (isProjectView()) {
          quantaFileProperties->fileDescLbl->setText(i18n("Description:"));
          quantaFileProperties->fileDesc->setText(currentKFileTreeViewItem()->text(1));
        }
        else {
          quantaFileProperties->fileDescLbl->hide();
          quantaFileProperties->fileDesc->hide();
        }
        topLayout->addWidget(quantaFileProperties);
    } //if localfile
  }
  return quantaFileProperties;
}

void FilesTreeView::slotClose()
{
  if (currentItem())
  {
    emit closeFile(currentURL());
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
    if ( ! currentKFileTreeViewItem()->isDir() )
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
    if (m_projectBaseURL.isParentOf(url))
    {
      msg = i18n("<qt><b>%1</b> might be part of your project! Do you really want to delete it?</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol));
    } else
    {
      if ( currentKFileTreeViewItem()->isDir() )
        msg = i18n("<qt>Do you really want to delete folder <b>%1</b> ?</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol));
      else
        msg = i18n("<qt>Do you really want to delete file <b>%1</b> ?</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol));
    }
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
  progressBar->setTotalSteps(1);
  progressBar->setValue(0);
  progressBar->setTextEnabled(false);

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
    
  KPropertiesDialog *propDlg = new KPropertiesDialog( url, this, 0L, false, false); //autodeletes itself
  const FileInfoDlg* fileInfoDlg = 0L;
  if (!currentKFileTreeViewItem()->isDir())
  {
    fileInfoDlg = addFileInfoPage(propDlg);
  }
  if (propDlg->exec())
  {
    KURL newURL = propDlg->kurl();
    if (fileInfoDlg)
    {
      QString newDesc = fileInfoDlg->fileDesc->text();
      if (currentKFileTreeViewItem()->text(1) != newDesc)
        itemDescChanged(currentKFileTreeViewItem(), newDesc);
    }
    if (url != newURL)
    {
      itemRenamed(url, newURL);
    }
  }
//    delete propDlg;
}



void FilesTreeView::slotReload()
{
  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if (!curItem) return;

  KURL url = curItem->branch()->rootUrl();
  removeBranch(curItem->branch());
  newBranch(url);
}


void FilesTreeView::slotJobFinished(KIO::Job *job)
{
  if ( job->error() )
      job->showErrorDialog(this);

  progressBar->reset();
}


void FilesTreeView::slotReturnPressed(QListViewItem *item)
{
  emit executed (item);
}


bool FilesTreeView::acceptDrag(QDropEvent* e ) const
{
  return (KFileTreeView::acceptDrag(e));
}

void FilesTreeView::slotDropped (KURL::List& fileList, KURL& dest)
{
  if(fileList.empty()) return;

  KIO::Job *job = KIO::copy(fileList,dest);
  connect( job, SIGNAL( result( KIO::Job *) ), this , SLOT( slotJobFinished( KIO::Job *) ) );
  progressBar->setTotalSteps(100);
  connect( job, SIGNAL( percent( KIO::Job *, unsigned long)),
          this, SLOT( slotPercent( KIO::Job *, unsigned long)));
}

void FilesTreeView::findDrop(const QPoint &pos, QListViewItem *&parent, QListViewItem *&after)
{
  QPoint p (contentsToViewport(pos));
  QListViewItem *atpos = itemAt(p);
  if (atpos && atpos->parent()) {
    after = atpos;
    parent = atpos->parent();
  } else {
    after = atpos;
    parent = atpos;
  }
}


void FilesTreeView::slotNewProjectLoaded(const QString &name, const KURL &baseURL, const KURL &)
{
  m_projectName = name;
  m_projectBaseURL = baseURL;
}

bool FilesTreeView::isFileOpen(const KURL &url)
{
  return quantaApp->doc()->isOpened(url);
}

bool FilesTreeView::isPathInClipboard()
{
  KURL url(QApplication::clipboard()->text());
  if (!url.isValid())
    return false;
  else
    return QExtFileInfo::exists(url);
}

void FilesTreeView::showEvent ( QShowEvent * )
{
  if (m_action) m_action->setChecked(true);
};

void FilesTreeView::hideEvent ( QHideEvent * )
{
  if (m_action) m_action->setChecked(false);
};


void FilesTreeView::slotDocumentClosed()
{
  QListViewItemIterator iter(this);
  for ( ; iter.current(); ++iter )
  {
    iter.current()->repaint();
  }
}

#include "filestreeview.moc"
