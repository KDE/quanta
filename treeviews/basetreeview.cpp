/***************************************************************************
                          basetreeview.cpp  -  description
                             -------------------
    begin                : Thu Jun 1 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2004 Andras Mantia <amantoa@kde.org>
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
#include <qtooltip.h>

// KDE includes
#include <kdebug.h>
#include <krun.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kopenwith.h>
#include <kmessagebox.h>
#include <kpropertiesdialog.h>
#include <kfiledialog.h>
#include <kprogress.h>
#include <kstandarddirs.h>
#include <kurldrag.h>
#include <kurl.h>
#include <ktar.h>
#include <ktempfile.h>
#include <kapplication.h>
#include <kcursor.h>
#include <kprotocolinfo.h>

// app includes
#include "basetreeview.h"
#include "fileinfodlg.h"
#include "project.h"
#include "resource.h"
#include "quanta.h"
#include "qextfileinfo.h"
#include "viewmanager.h"

#include <X11/Xlib.h>

class BaseTreeViewToolTip : public QToolTip
{
public:
    BaseTreeViewToolTip( QWidget *parent, BaseTreeView *lv );

    void maybeTip( const QPoint &pos );

private:
    BaseTreeView *m_view;

};

BaseTreeViewToolTip::BaseTreeViewToolTip( QWidget *parent, BaseTreeView *lv )
    : QToolTip( parent ), m_view( lv )
{
}

void BaseTreeViewToolTip::maybeTip( const QPoint &pos )
{
    if ( !parentWidget() || !m_view || !m_view->showToolTips() )
	return;

    QListViewItem *item = m_view->itemAt(pos);
    if (!item)
      return;
    QString text;
    KFileTreeViewItem * kftvi = dynamic_cast<BaseTreeViewItem *> (item);
    if (kftvi) {
      QString desc = kftvi->text(1);
      text = kftvi->fileItem()->getToolTipText();
      if ( !desc.isEmpty() )
        text.prepend("&nbsp;<b>" + desc + "</b><br>");
    } else {
      text = item->text(1);  // show something for the branchroot
    }
    if ( !text.isEmpty() )
      tip(m_view->itemRect(item), text);
}


//BaseTreeViewItem implementation
BaseTreeViewItem::BaseTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch )
: KFileTreeViewItem( parent, item, brnch)
{
}

QString BaseTreeViewItem::key(int column, bool ascending) const
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

int BaseTreeViewItem::compare(QListViewItem *i, int col,
                              bool ascending) const
{
  return key(col, ascending).compare(i->key(col, ascending));
}

void BaseTreeViewItem::paintCell(QPainter *p, const QColorGroup &cg,
                                 int column, int width, int align)
{
  QColorGroup _cg( cg );
  if (column == 0)
  {
    QFont f = p->font();
    if (quantaApp)
      f.setBold(ViewManager::ref()->isOpened(url()));
    p->setFont(f);
  } else
  {
    int h, s, v;
    p->pen().color().getHsv(&h, &s, &v);
    v = (v < 155 ? v + 100 : 255);
    _cg.setColor(QColorGroup::Text, QColor(h, s, v, QColor::Hsv));
  };
  KFileTreeViewItem::paintCell( p, _cg, column, width, align );
}

void BaseTreeViewItem::refreshIcon()
{
  setPixmap(0, fileItem()->pixmap(KIcon::SizeSmall));
}


//BaseTreeBranch implementation
BaseTreeBranch::BaseTreeBranch(KFileTreeView *parent, const KURL& url,
                                 const QString& name, const QPixmap& pix,
                                 bool showHidden ,
                                 KFileTreeViewItem *branchRoot)
  : KFileTreeBranch(parent, url, name, pix, showHidden, branchRoot)
{
  bool localFile = url.isLocalFile();
  setAutoUpdate(localFile);
  setChildRecurse(localFile);
}

bool BaseTreeBranch::matchesFilter(const KFileItem *item) const
{
  if (!excludeFilterRx.isEmpty())
    if (excludeFilterRx.exactMatch(item->url().path()))
      return false;

  return KFileTreeBranch::matchesFilter(item);
}

KFileTreeViewItem* BaseTreeBranch::createTreeViewItem(KFileTreeViewItem *parent,
                                                      KFileItem *fileItem)
{
  BaseTreeViewItem  *tvi = 0;
  if( parent && fileItem)
  {
    tvi = new BaseTreeViewItem(parent, fileItem, this);
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




//BaseTreeView implementation
BaseTreeView::BaseTreeView(QWidget *parent, const char *name)
: KFileTreeView(parent, name), fileInfoDlg(0)
{
  QToolTip::remove(viewport());  // remove the tooltip from QListView
  m_tooltip = new BaseTreeViewToolTip(viewport(), this);
  setFrameStyle( Panel | Sunken );
  setRootIsDecorated(true);
  setSorting(0);
  setLineWidth(2);
  setFullWidth(true);
  setShowSortIndicator(true);
  setFocusPolicy(QWidget::ClickFocus);
  setShowFolderOpenPixmap(false);

  connect(this, SIGNAL(returnPressed(QListViewItem *)),
          this, SLOT(slotReturnPressed(QListViewItem *)));

  connect(this, SIGNAL(dropped(QWidget *, QDropEvent *, KURL::List&, KURL&)),
          this, SLOT(slotDropped(QWidget *, QDropEvent *, KURL::List&, KURL&)));

  connect(this, SIGNAL(itemRenamed(QListViewItem*, const QString &, int )),
          this, SLOT(slotRenameItem(QListViewItem*, const QString &, int )));

  connect(this, SIGNAL(executed(QListViewItem *)),
          this, SLOT(slotSelectFile(QListViewItem *)));
}


BaseTreeView::~BaseTreeView()
{
  QToolTip::remove(viewport());
  delete m_tooltip;
}


void BaseTreeView::itemRenamed(const KURL& oldURL, const KURL& newURL)
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


/** Called for: double click, return, Open */
void BaseTreeView::slotSelectFile(QListViewItem *item)
{
  Q_UNUSED(item);

  KFileTreeViewItem* kftvi = currentKFileTreeViewItem();
  if (!kftvi || kftvi->isDir()) return;

  KURL urlToOpen = kftvi->url();
  if (!urlToOpen.isEmpty())
  {
    if ( QuantaCommon::checkMimeGroup(urlToOpen,"text") )
    {
      emit openFile(urlToOpen);
    }
    else if ( QuantaCommon::checkMimeGroup(urlToOpen, "image") ) //it may be an image
      {
         emit openImage( urlToOpen);
      }
      else if ( expandArchiv(kftvi)) //it may be an archiv
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
bool BaseTreeView::expandArchiv (KFileTreeViewItem *item)
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

  KFileTreeBranch *kftb = new BaseTreeBranch(this, urlToOpen, item->text(0), *(item->pixmap(0)), true, item);
  addBranch(kftb);  // connecting some signals
  kftb->populate(urlToOpen, item);
  item->setExpandable(true);
  item->setOpen(true);
  return true;
}


/** Properties dialog addon*/
FileInfoDlg* BaseTreeView::addFileInfoPage(KPropertiesDialog* propDlg)
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
        quantaFileProperties->fileDescLbl->setText(i18n("Description:"));
        quantaFileProperties->fileDesc->setText(currentKFileTreeViewItem()->text(1));
        // disable per default
        quantaFileProperties->fileDescLbl->hide();
        quantaFileProperties->fileDesc->hide();
        topLayout->addWidget(quantaFileProperties);
    } //if localfile
  }
  return quantaFileProperties;
}


void BaseTreeView::slotClose()
{
  if (currentItem())
  {
    emit closeFile(currentURL());
  }
}


void BaseTreeView::slotOpen()
{
  QListViewItem *item = currentItem();
  if (item)
  {
    emit open(item);
  }
}


void BaseTreeView::slotOpenWith()
{
  if (currentItem())
  {
    KURL::List list;
    KURL urlToOpen = currentURL();
    list.append( urlToOpen );
    KRun::displayOpenWithDialog( list );
  }
}


void BaseTreeView::slotInsertTag()
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


void BaseTreeView::slotCopy()
{
   if (currentItem())
   {
     QClipboard *cb = QApplication::clipboard();
     cb->setText( currentURL().prettyURL() );
   }
}


void BaseTreeView::slotPaste()
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


void BaseTreeView::slotPercent(KIO::Job *job, unsigned long value)
{
  Q_UNUSED(job);
  progressBar->setProgress(value);
}


void BaseTreeView::slotDelete()
{
  if (currentItem())
  {
    KURL url = currentURL();
    QString msg;
    if (m_projectBaseURL.isParentOf(url))
    {
      msg = i18n("<qt><b>%1</b> might be part of your project; do you really want to delete it?</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol));
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


void BaseTreeView::slotPopulateFinished(KFileTreeViewItem *item)
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
//  } else {
//    if ( !item->isOpen() ) item->setOpen(true);
  }
}


void BaseTreeView::slotInsertInProject()
{
  if (currentItem() )
  {
    emit insertFileInProject( currentURL());
  }
}


void BaseTreeView::slotInsertDirInProject()
{
  if ( currentItem() )
  {
    emit insertDirInProject( currentURL() );
  }
}


/** Bring up the properites dialog, and extend it for files */
void BaseTreeView::slotProperties()
{
  KURL url = currentURL();
  if (url.isEmpty()) return;
  KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, url);
  propDlg = new KPropertiesDialog(&fileItem, this, 0L, false, false); //autodeletes itself
  fileInfoDlg = 0L;
  if (!currentKFileTreeViewItem()->isDir())
  {
    fileInfoDlg = addFileInfoPage(propDlg);
  }
  connect(propDlg, SIGNAL( applied() ), this, SLOT( slotPropertiesApplied()) );
  propDlg->exec();
}


void BaseTreeView::slotPropertiesApplied()
{
  if (! propDlg) return;
  // check if renamed
  KURL url = currentURL();
  if (url != propDlg->kurl())
  {
    itemRenamed(url, propDlg->kurl());
  }
  if (fileInfoDlg)
  {
    // has description changed?
    QString newDesc = fileInfoDlg->fileDesc->text();
    if (currentKFileTreeViewItem()->text(1) != newDesc)
      itemDescChanged(currentKFileTreeViewItem(), newDesc);
  } /*else
  {*/
    // refresh icon in case it has changed
    BaseTreeViewItem * btvi = dynamic_cast<BaseTreeViewItem *> (currentKFileTreeViewItem());
    if (btvi)
    {
      btvi->refreshIcon();
    }
//   }
}


void BaseTreeView::slotReload()
{
  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if (!curItem) return;

  KURL url = curItem->branch()->rootUrl();
  removeBranch(curItem->branch());
  newBranch(url);
}


void BaseTreeView::slotJobFinished(KIO::Job *job)
{
  if ( job->error() )
      job->showErrorDialog(this);

  progressBar->reset();  //FIXME: this is not correct
}


void BaseTreeView::slotReturnPressed(QListViewItem *item)
{
  emit executed(item);
}


bool BaseTreeView::acceptDrag(QDropEvent* e ) const
{
  QPoint p (contentsToViewport(e->pos()));
  QListViewItem *atpos = itemAt(p);
  KFileTreeViewItem *kftvi = dynamic_cast <KFileTreeViewItem *> (atpos);
  if (kftvi)
    return (KFileTreeView::acceptDrag(e) && kftvi->isDir());  //  accept only on folders
  else
    return (KFileTreeView::acceptDrag(e));
}


void BaseTreeView::slotDropped (QWidget *, QDropEvent * /*e*/, KURL::List& fileList, KURL& dest)
{
  if(fileList.empty())
    return;

  // Check what the destination can do
  if (!KProtocolInfo::supportsWriting(dest))
    return;
  bool sLinking = KProtocolInfo::supportsLinking(dest);

  // Check what the source can do
  KURL url = fileList.first(); // we'll assume it's the same for all URLs (hack)
  bool sReading = KProtocolInfo::supportsReading(url);
  bool sDeleting = KProtocolInfo::supportsDeleting(url);
  bool sMoving = KProtocolInfo::supportsMoving(url);

  // Check the state of the modifiers key at the time of the drop
  Window root;
  Window child;
  int root_x, root_y, win_x, win_y;
  uint keybstate;
  XQueryPointer( qt_xdisplay(), qt_xrootwin(), &root, &child,
                  &root_x, &root_y, &win_x, &win_y, &keybstate );

  int result;
  if (keybstate & ControlMask) {
    result = 1;   // copy
  } else {
    if (keybstate & ShiftMask) {
      result = 2;  // move
    } else {

      // create the popup menu
      QPopupMenu popup;
      if (sMoving || (sReading && sDeleting))
        popup.insertItem(SmallIconSet("goto"), i18n("&Move Here"), 2 );
      if (sReading)
        popup.insertItem(SmallIconSet("editcopy"), i18n("&Copy Here"), 1 );
      if (sLinking)
        popup.insertItem(SmallIconSet("www"), i18n("&Link Here"), 3 );
      popup.insertSeparator();
      popup.insertItem(SmallIconSet("cancel"), i18n("C&ancel"), 5);

      result = popup.exec( /*mapToGlobal(e->pos())*/ QCursor::pos() );
    }
  }
  KIO::Job *job;
  switch ( result ) {
    case 1  : job = KIO::copy(fileList, dest);
              break;
    case 2  : job = KIO::move(fileList, dest);
              break;
    case 3  : job = KIO::link(fileList, dest);
              break;
    default : return ;
  }
  if (!job)
    return;

  connect(job, SIGNAL( result(KIO::Job *) ),
          this, SLOT( slotJobFinished(KIO::Job *) ) );
  progressBar->setTotalSteps(100);
  connect(job, SIGNAL(percent( KIO::Job *, unsigned long)),
          this, SLOT(slotPercent( KIO::Job *, unsigned long)));
}


void BaseTreeView::findDrop(const QPoint &pos, QListViewItem *&parent, QListViewItem *&after)
{
  QPoint p (contentsToViewport(pos));
  QListViewItem *atpos = itemAt(p);
//   if (atpos && atpos->parent()) {
//     after = atpos;
//     parent = atpos->parent();
//   } else {
    after = atpos;
    parent = atpos;
//   }
}


void BaseTreeView::slotNewProjectLoaded(const QString &name, const KURL &baseURL, const KURL &)
{
  m_projectName = name;
  m_projectBaseURL = baseURL;
}


bool BaseTreeView::isFileOpen(const KURL &url)
{
  return ViewManager::ref()->isOpened(url);
}


bool BaseTreeView::isPathInClipboard()
{
  KURL url(QApplication::clipboard()->text());
  if (!url.isValid())
    return false;
  else
    return QExtFileInfo::exists(url);
}


void BaseTreeView::slotDocumentClosed()
{
  QListViewItemIterator iter(this);
  for ( ; iter.current(); ++iter )
  {
    iter.current()->repaint();
  }
}


void BaseTreeView::slotStartRename()
{
  KFileTreeViewItem * item = currentKFileTreeViewItem();
  if (item) {
    setRenameable(0, true);
    rename(item, 0);
  };
}


void BaseTreeView::cancelRename(int col)
{
  // reset the inline rename option for file/folder name
  if (col == 0) {
    setRenameable(0, false);
  }
}


void BaseTreeView::slotRenameItem(QListViewItem* item, const QString & newText, int col)
{
  if (!item)
    return;

  // reset the inline rename option for file/folder name
  if (col == 0) {
    setRenameable(0, false);
    KFileTreeViewItem * kvtvi = dynamic_cast<KFileTreeViewItem *>(item);
    if (kvtvi)
      doRename(kvtvi, newText);
  }
}


void BaseTreeView::doRename(KFileTreeViewItem* kftvi, const QString & newName)
{
  if (! kftvi)
    return;

  // remember this for slotRenameFinished()
  m_kftvi = kftvi;
  m_oldURL = kftvi->url();
  m_newURL = m_oldURL;

  if (kftvi->isDir())
  {
    m_newURL.setPath(QFileInfo(m_oldURL.path(-1)).dirPath() + '/' + newName + '/');
    m_oldURL.adjustPath(1);
  } else
  {
    m_newURL.setFileName(newName);
  }
  if ( m_oldURL != m_newURL )
  {
    bool proceed = true;
    if (QExtFileInfo::exists(m_newURL))
    {
      proceed = KMessageBox::warningYesNo(this, i18n("<qt>The file <b>%1</b> already exists.<br>Do you want to overwrite it?</qt>").arg(m_newURL.prettyURL(0, KURL::StripFileProtocol)),i18n("Overwrite")) == KMessageBox::Yes;
    }
    if (proceed)
    {
    //start the rename job
      KURL oldURL = m_oldURL;
      oldURL.adjustPath(-1);
      KURL newURL = m_newURL;
      newURL.adjustPath(-1);
      KIO::SimpleJob *job = KIO::rename(oldURL, newURL, true);
      connect(job, SIGNAL( result( KIO::Job *) ), SLOT(slotRenameFinished( KIO::Job *)));
    }
  }
}


void BaseTreeView::slotRenameFinished(KIO::Job *job)
{
  if (job->error()) {
    job->showErrorDialog(this);
    m_kftvi->setText(0, m_kftvi->fileItem()->text());  // reset the text
  } else {
    emit renamed(m_oldURL, m_newURL);
  }
}

void BaseTreeView::slotCreateSiteTemplate()
{
   QString startDir;
   if (Project::ref()->hasProject())
   {
     startDir = Project::ref()->templateURL.url();
   } else
   {
     startDir = locateLocal("data", resourceDir + "templates/");
   }
   KURL targetURL;
   bool valid;
   do {
     valid = false;
     targetURL = KFileDialog::getSaveURL(startDir, "*.tgz", this, i18n("Create Site Template File"));
     if (targetURL.isEmpty())
       return;
     if (targetURL.url().startsWith(KURL::fromPathOrURL(locateLocal("data", resourceDir + "templates/")).url()))
       valid = true;
    if (Project::ref()->hasProject() && targetURL.url().startsWith(Project::ref()->templateURL.url()))
      valid = true;
    if (!valid)
      KMessageBox::error(this, i18n("Templates should be saved to the local or project template folder."));
   }while (!valid);

   KURL url = currentURL();
   //TODO: Implement creation from remote folders as well. Requires downloading of the files to a
   //temporary directory
   if (url.protocol() != "file")
   {
       KMessageBox::sorry(this, i18n("Currently you can create site templates only from local folders."), i18n("Unsupported Feature"));
       return;
   }

   KTempFile *tempFile = new KTempFile(tmpDir);
   tempFile->setAutoDelete(true);
   tempFile->close();
   KTar tar(tempFile->name(), "application/x-gzip");
   bool error = false;
   if (tar.open(IO_WriteOnly))
   {
      KURL::List fileList = QExtFileInfo::allFiles(url, "*");
      for (KURL::List::Iterator it = fileList.begin(); it != fileList.end(); ++it)
      {
         if (!(*it).path().endsWith("/"))
         {
           QFile f((*it).path());
           if (f.open(IO_ReadOnly))
           {
              QByteArray buffer(f.readAll());
              if (!tar.writeFile((*it).path().remove(url.path()), "user", "group", buffer.size(), buffer.data()))
              {
                  error = true;
              }
              f.close();
           } else
             error = true;
         }
      }
      tar.close();
   } else
      error = true;
   if (!QExtFileInfo::copy(KURL::fromPathOrURL(tempFile->name()), targetURL, -1, true, false, this))
     error = true;

   if (error)
     KMessageBox::error(this, i18n("<qt>There was an error while creating the site template tarball.<br>Check that you can read the files from <i>%1</i>, you have write access to <i>%2</i> and that you have enough free space in your temporary folder.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)).arg(targetURL.prettyURL(0, KURL::StripFileProtocol)), i18n("Template Creation Error"));
   delete tempFile;
}

#include "basetreeview.moc"