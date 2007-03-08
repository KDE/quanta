/***************************************************************************
    begin                : Thu Jun 1 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2004 Andras Mantia <amantoa@kde.org>
                           (C) 2004-2005 Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
// app includes
#include "basetreeview.h"
#include "fileinfodlg.h"
#include "extfileinfo.h"
#include "quantanetaccess.h"
#include "helper.h"

//kdevelop includes
#include <kdevcore.h>
#include <kdevplugin.h>
#include <kdevdocument.h>
#include <kdevdocumentcontroller.h>

// QT includes
#include <QDropEvent>
#include <QHelpEvent>

#include <QEventLoop>
#include <QLayout>
#include <QFileInfo>
#include <QLabel>
#include <QImage>
#include <QRegExp>
#include <QClipboard>
#include <qpoint.h>
#include <QRegExp>
#include <QLineEdit>
#include <qfont.h>
#include <QPainter>
#include <QToolTip>
#include <qstack.h>
#if defined Q_WS_X11
#include <qx11info_x11.h>
#endif
// KDE includes
#include <kaction.h>
#include <kicon.h>
#include <kconfigbase.h>
#include <kconfiggroup.h>
#include <kdebug.h>
#include <krun.h>
#include <kicon.h>
#include <kinputdialog.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kprotocolmanager.h>
#include <kmessagebox.h>
#include <kpropertiesdialog.h>
#include <kfiledialog.h>
#include <kstandarddirs.h>
#include <kurl.h>
#include <ktar.h>
#include <ktemporaryfile.h>
#include <kapplication.h>
#include <kcursor.h>
#include <kprotocolinfo.h>
#include <kmenu.h>
#include <kparts/part.h>
#include <kio/jobuidelegate.h>
#if defined Q_WS_X11
#include <X11/Xlib.h>
#endif
//BaseTreeViewItem implementation
BaseTreeViewItem::BaseTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch )
  : KFileTreeViewItem( parent, item, brnch)
{
  // cache this for speed
  m_baseTreeView = static_cast<BaseTreeBranch *>(branch())->baseTreeView();
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

int BaseTreeViewItem::compare(Q3ListViewItem *i, int col,
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
    // try to find out if the file is open
    if (m_baseTreeView)
      f.setBold(m_baseTreeView->isFileOpen(url()));
    p->setFont(f);
  } else
  {
    int h, s, v;
    p->pen().color().getHsv(&h, &s, &v);
    v = (v < 155 ? v + 100 : 255);
    _cg.setColor(QColorGroup::Text, QColor::fromHsv(h, s, v));
  };
  KFileTreeViewItem::paintCell( p, _cg, column, width, align );
}

void BaseTreeViewItem::refreshIcon()
{
  fileItem()->refreshMimeType();
}


//BaseTreeBranch implementation
BaseTreeBranch::BaseTreeBranch(BaseTreeView *parent, const KUrl& url,
                                 const QString& name, const QPixmap& pix,
                                 bool showHidden,
                                 KFileTreeViewItem *branchRoot)
  : KFileTreeBranch(parent, url, name, pix, showHidden, branchRoot), m_baseTreeView(parent)
{
  bool localFile = url.isLocalFile();
  setAutoUpdate(localFile);
  setChildRecurse(false);
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
    // we assume there are children
    if (tvi)
    {
      tvi->setExpandable(tvi->isDir());
    }
  }
  else
    kDebug(24000) << "createTreeViewItem: Have no parent" << endl;
  return tvi;
}


void BaseTreeBranch::addOpenFolder(QStringList* openFolder)
{
  if (! openFolder)  // just in case
    return;
  KFileTreeViewItem *newItem;
  KFileTreeViewItem *item = root();
  while (item) {
    if (item->isDir() && item->isOpen()) {
      openFolder->append( item->url().url() );
      // dive into the tree first
      newItem = dynamic_cast<KFileTreeViewItem *>(item->firstChild());
      if (newItem) {
        // found child go ahead
        item = newItem;
        continue;
      };
    };
    // move up in the tree
    while (item && item != root())
    {
      if (item->nextSibling())
      {
        item = dynamic_cast<KFileTreeViewItem *>(item->nextSibling());
        break;
      }
      else
      {
        item = dynamic_cast<KFileTreeViewItem *>(item->parent());
      }
    }
    if (item == root())
      break;
  };
}


void BaseTreeBranch::reopenFolder()
{
  if (folderToOpen.isEmpty())
    return;
  KFileTreeViewItem *item;
  QStringList::Iterator end(folderToOpen.end());
  for (QStringList::Iterator it = folderToOpen.begin(); it != end; ++it) {
    KUrl url( (*it) );
    item = findTVIByUrl(url);
    if (item) {
      // erase the url in the list
      (*it) = "";
      // open the folder
      item->setExpandable(true);
      item->setOpen(true);
    }
  }
}


void BaseTreeBranch::updateOpenFolder()
{
  KFileTreeViewItem *newItem;
  KFileTreeViewItem *item = root();
  while (item) {
    if (item->isDir() && item->isOpen()) {
      updateDirectory( item->url() );
      kapp->processEvents(QEventLoop::ExcludeUserInput | QEventLoop::ExcludeSocketNotifiers);
      // dive into the tree first
      newItem = dynamic_cast<KFileTreeViewItem *>(item->firstChild());
      if (newItem) {
        // found child go ahead
        item = newItem;
        continue;
      };
    };
    // go up if no sibling available
    if (! item->nextSibling())
      item = dynamic_cast<KFileTreeViewItem *>(item->parent());
    if (item == root())
      break;
    if (item)
      item = dynamic_cast<KFileTreeViewItem *>(item->nextSibling());
  };
}


////////////////////////////////////////////////////////////////////////////////////
//
//               BaseTreeView implementation
//
////////////////////////////////////////////////////////////////////////////////////

BaseTreeView::BaseTreeView(Koncrete::Plugin *plugin, QWidget * parent)
  : KFileTreeView(parent), fileInfoDlg(0), m_parent(parent), m_plugin(plugin), m_saveOpenFolder(false), m_partController(Koncrete::Core::documentController())
{
//   setTreeStepSize(15);
  setRootIsDecorated(true);
  setSorting(0);
  setFullWidth(true);
  setShowSortIndicator(true);
  setFocusPolicy(Qt::ClickFocus);
  setShowFolderOpenPixmap(false);

  connect(this, SIGNAL(returnPressed(Q3ListViewItem *)),
          this, SLOT(slotReturnPressed(Q3ListViewItem *)));

  connect(this, SIGNAL(dropped(QWidget *, QDropEvent *, KUrl::List&, KUrl&)),
          this, SLOT(slotDropped(QWidget *, QDropEvent *, KUrl::List&, KUrl&)));

  connect(this, SIGNAL(itemRenamed(Q3ListViewItem*, const QString &, int )),
          this, SLOT(slotRenameItem(Q3ListViewItem*, const QString &, int )));

  connect(this, SIGNAL(executed(Q3ListViewItem *)),
          this, SLOT(slotSelectFile(Q3ListViewItem *)));

}


BaseTreeView::~BaseTreeView()
{
  viewport()->setToolTip("");
}


void BaseTreeView::itemRenamed(const KUrl& oldURL, const KUrl& newURL)
{
  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if (! curItem) return;

  if (curItem->isDir())
  {
    KUrl n = newURL;
    n.adjustPath(KUrl::AddTrailingSlash);
    KUrl o = oldURL;
    o.adjustPath(KUrl::AddTrailingSlash);
    emit renamed(o, n);
  }
  else
  {
    emit renamed(oldURL, newURL);
  }
}


/** Called for: double click, return, Open */
void BaseTreeView::slotSelectFile(Q3ListViewItem *item)
{
  KFileTreeViewItem* kftvi = currentKFileTreeViewItem();
  if (!kftvi || kftvi->isDir())
    return;

  if (expandArchiv(kftvi))
    return;

  KUrl urlToOpen = kftvi->url();
  if (!urlToOpen.isEmpty())
  {
    m_partController->editDocument(urlToOpen);
    item->repaint();
  }
}


/** expands an archiv, if possible */
bool BaseTreeView::expandArchiv(KFileTreeViewItem *item)
{
  if (!item) return false;
  KUrl urlToOpen = item->url();

  if ( ! urlToOpen.isLocalFile()) return false;

  QString mimeType = KMimeType::findByUrl(urlToOpen)->name();

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

      QFrame *quantaFilePage = new QFrame();
      propDlg->addPage(quantaFilePage, i18n("Quanta File Info"));
    QVBoxLayout *topLayout = new QVBoxLayout( quantaFilePage);
    FileInfoDlg *quantaFileProperties = new FileInfoDlg(quantaFilePage);
    quantaFileProperties->setWindowTitle(i18n("Quanta") );

    int fsize,fimgsize=0;
    int ct=0, imgct=0/*, position=0*/;
    KUrl u = currentUrl();
    if (u.isLocalFile())   //get the file info only for local file. TODO: for non-local ones
    {
       QString nameForInfo = u.path();
       QString path = u.directory(KUrl::ObeyTrailingSlash);       //extract path for images
       QString sourcename = u.fileName(0);

       QFile qfile(nameForInfo);
       fsize=qfile.size();                              //html file size

       QString mimetype = KMimeType::findByFileContent(nameForInfo)->name();
       if (mimetype.contains("text"))
       {
//FIXME: Reimplement with QTextStream
         /*
        qfile.open(IO_ReadOnly);
        QString imgname,imgpath;
        while (qfile.readLine(imgname,200)!=-1)     //open & parse file
        {
          ct++;
          position=imgname.indexOf("<img",0,FALSE);              //check for images
          if (position!=-1)
          {
            imgname.remove(0,position+4);
            position=imgname.indexOf("src=",0,FALSE);              //extract images names
            imgname.remove(0,position+4);
            if (imgname.startsWith("\"")) imgname.remove(0,1);
            if (imgname.startsWith("'")) imgname.remove(0,1);
            position=imgname.indexOf(" ",0,FALSE);
            if (position!=-1) imgname=imgname.left(position);
            position=imgname.indexOf(">",0,FALSE);
            if (position!=-1) imgname=imgname.left(position);
            position=imgname.indexOf("\"",0,FALSE);
            if (position!=-1) imgname=imgname.left(position);
            position=imgname.indexOf("'",0,FALSE);
            if (position!=-1) imgname=imgname.left(position);
            if (!quantaFileProperties->imageList->findItem(imgname,Qt::ExactMatch))     //check if image was already counted
            {
              KUrl v(KUrl::fromPathOrUrl( path ), imgname);
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
         */
         quantaFileProperties->lineNum->setText(i18n("Number of lines: %1", ct));
         quantaFileProperties->imageNum->setText(i18n("Number of images included: %1", imgct));
         quantaFileProperties->imageSize->setText(i18np("Size of the included images: 1 byte", "Size of the included images: %1 bytes", fimgsize));
         quantaFileProperties->totalSize->setText(i18np("Total size with images: 1 byte", "Total size with images: %1 bytes", fsize+fimgsize));
        }
        else if (mimetype.contains("image"))
        {              // assume it's an image file
          QImage imagefile=QImage(nameForInfo);
          quantaFileProperties->lineNum->setText(i18n("Image size: %1 x %2", imagefile.width(), imagefile.height()));
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

bool BaseTreeView::event(QEvent *event)
{
  if (event->type() == QEvent::ToolTip)
  {
    if (!showToolTips() )
      return true;

    Q3ListViewItem *item = itemAt(static_cast<QHelpEvent*>(event)->globalPos());
    if (!item)
      return true;
    QString text;
    KFileTreeViewItem * kftvi = dynamic_cast<BaseTreeViewItem *> (item);
    if (kftvi) {
      QString desc = kftvi->text(1);
      text = kftvi->fileItem()->getToolTipText();
      if ( !desc.isEmpty() )
        text.prepend("&nbsp;<i>" + desc + "</i><br>");
    } else
    {  // show something for the branchroot
      text = item->text(0);
      if ( ! item->text(1).isEmpty() )
        text.append( "<br>" + item->text(1));
    }
    if (!text.isEmpty() )
      QToolTip::showText(static_cast<QHelpEvent*>(event)->globalPos(), text);
    return true;
  }
  return false;
}


void BaseTreeView::slotClose()
{
  if (currentItem())
  {
    Koncrete::Document * doc = m_partController->documentForUrl(currentUrl());
    if (doc)
      doc->close();
  }
}


void BaseTreeView::slotOpen()
{
  Q3ListViewItem *item = currentItem();
  if (item)
  {
    emit open(item);
    item->repaint();
  }
}


void BaseTreeView::slotCopy()
{
   if (currentItem())
   {
     QClipboard *cb = QApplication::clipboard();
     cb->setText( currentUrl().prettyUrl() );
   }
}


void BaseTreeView::slotPaste()
{
  if (currentItem())
  {
    QClipboard *cb = QApplication::clipboard();
    KUrl::List list( cb->text().split(QChar('\n')) );

    KUrl url = currentUrl();
    if ( ! currentKFileTreeViewItem()->isDir() )
      url.setFileName("");   // don't paste on files but in dirs
    QuantaNetAccess::dircopy(list, url, m_plugin, true);
  }
}


void BaseTreeView::slotPercent(KJob *job, unsigned long /*value*/)
{
  Q_UNUSED(job);
//   progressBar->setProgress(value);  FIXME how can I display a progress bar?
}


void BaseTreeView::slotDelete()
{
  if (!currentKFileTreeViewItem()) return;
  KUrl url = currentUrl();
  if (currentKFileTreeViewItem()->isDir())
    url.adjustPath(KUrl::AddTrailingSlash);
  QuantaNetAccess::del(url, m_plugin, m_parent, true);
}


void BaseTreeView::slotPopulateFinished(KFileTreeViewItem *item)
{
//   progressBar->setTotalSteps(1);  FIXME
//   progressBar->setProgress(-1);
//   progressBar->setTextEnabled(false);

  if ( !item ) return;

  if( item->isDir() )
    stopAnimation( item );

  if (item->childCount() == 0) {
    item->setExpandable(false);
  } else {
    QString url = item->url().url();
    BaseTreeBranch *btb = dynamic_cast<BaseTreeBranch *>(item->branch());
    if (btb && ! btb->folderToOpen.empty()) {
      btb->reopenFolder();
    }
  }
}


/** Bring up the properites dialog, and extend it for files */
void BaseTreeView::slotProperties()
{
  KUrl url = currentUrl();
  if (url.isEmpty()) return;
  KFileItem fileItem(KFileItem::Unknown, KFileItem::Unknown, url);
  propDlg = new KPropertiesDialog(&fileItem, this); //autodeletes itself
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
  KUrl url = currentUrl();
  if (url != propDlg->kurl())
  {
    itemRenamed(url, propDlg->kurl());
  }
  KFileTreeViewItem *kftvi = currentKFileTreeViewItem();
  if (fileInfoDlg && kftvi)
  {
    // has description changed?
    QString newDesc = fileInfoDlg->fileDesc->text();
    if (kftvi->text(1) != newDesc)
      itemDescChanged(kftvi, newDesc);
  }
  BaseTreeViewItem * btvi = dynamic_cast<BaseTreeViewItem *> (kftvi);
  if (btvi)
  {
    btvi->refreshIcon();
  }
/*  not working as expected
  if (kftvi && kftvi->url().isLocalFile()) {
    slotReloadAllTrees();  // refresh the icons for local url's, they might have changed
  }*/
}


void BaseTreeView::slotReloadAllTrees()
{
  QStack<BaseTreeBranch*> stack;
  BaseTreeBranch *btb;
  KFileTreeBranchIterator it( branches() );
  for ( ; it.current(); ++it)
  {
    btb = dynamic_cast<BaseTreeBranch *>( (*it) );
    if (btb && btb->rootUrl().isLocalFile()) {
      stack.push(btb);
    }
  }
  while (! stack.isEmpty())
  {
    reload(stack.pop());
  }
}


void BaseTreeView::slotReload()
{
  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if (curItem)
    reload(dynamic_cast<BaseTreeBranch *>(curItem->branch()));
}


void BaseTreeView::reload(BaseTreeBranch *btb)
{
  // remember the old status
  QStringList folderToOpen;
  btb->addOpenFolder(&folderToOpen);
  KUrl url = btb->rootUrl();
  // remove and open again
  removeBranch(btb);
  btb = dynamic_cast<BaseTreeBranch *>(newBranch(url));
  if (btb) {
    btb->folderToOpen = folderToOpen;
    btb->reopenFolder();
    btb->updateOpenFolder();
  }
}


void BaseTreeView::slotJobFinished(KJob *job)
{
  if ( job->error() )
  {
    static_cast<KIO::Job*>(job)->ui()->setWindow(this);
    static_cast<KIO::Job*>(job)->ui()->showErrorMessage();
  }
/*  progressBar->setTotalSteps(1);  FIXME
  progressBar->setProgress(-1);
  progressBar->setTextEnabled(false);*/
}


void BaseTreeView::slotReturnPressed(Q3ListViewItem *item)
{
  emit executed(item);
}


bool BaseTreeView::acceptDrag(QDropEvent* e) const
{
  QPoint p (contentsToViewport(e->pos()));
  Q3ListViewItem *atpos = itemAt(p);
  KFileTreeViewItem *kftvi = dynamic_cast <KFileTreeViewItem *> (atpos);
  if (kftvi)
    return (KFileTreeView::acceptDrag(e) && kftvi->isDir());  //  accept only on folders
  else
    return (KFileTreeView::acceptDrag(e));
}


void BaseTreeView::slotDropped (QWidget *, QDropEvent * /*e*/, KUrl::List& fileList, KUrl& dest)
{
  if(fileList.empty())
    return;

  dest.adjustPath(KUrl::AddTrailingSlash);
  // Check what the destination can do
  if (!KProtocolManager::supportsWriting(dest))
    return;
  bool sLinking = KProtocolManager::supportsLinking(dest);

  // Check what the source can do
  KUrl url = fileList.first(); // we'll assume it's the same for all URLs (hack)
  bool sReading = KProtocolManager::supportsReading(url);
  bool sDeleting = KProtocolManager::supportsDeleting(url);
  bool sMoving = KProtocolManager::supportsMoving(url);
#ifdef __GNUC__
#warning "kde: need to port it"
#endif
#if 0
  // Check the state of the modifiers key at the time of the drop
  uint keybstate;
#ifdef __GNUC__
#warning "KDE4: port it on macosx";
#endif
#if defined Q_WS_X11
  Window root;
  Window child;
  int root_x, root_y, win_x, win_y;
  XQueryPointer( QX11Info::display(), QX11Info::appRootWindow(), &root, &child,
                  &root_x, &root_y, &win_x, &win_y, &keybstate );
#endif
    KAction *moveAction  = new KAction(KIcon("goto-page"), i18n("&Move Here"), this);
    actionCollection()->addAction("move", moveAction );
    KAction *copyAction  = new KAction(KIcon("edit-copy"), i18n("&Copy Here"), this);
    actionCollection()->addAction("copy", copyAction );
    KAction *linkAction  = new KAction(KIcon("www"), i18n("&Link Here"), this);
    actionCollection()->addAction("link", linkAction );
    KAction *cancelAction  = new KAction(KIcon("cancel"), i18n("C&ancel"), this);
    actionCollection()->addAction("cancel", cancelAction );
  QAction *result;
#if defined Q_WS_X11
  if (keybstate & ControlMask) {
    result = copyAction;   // copy
  } else {
    if (keybstate & ShiftMask) {
      result = moveAction;  // move
    } else {
#endif
      // create the popup menu
      QMenu popup;
      if (sMoving || (sReading && sDeleting))
        popup.insertAction(0L, moveAction);
      if (sReading)
        popup.insertAction(0L, copyAction);
      if (sLinking)
        popup.insertAction(0L, linkAction);
      popup.addSeparator();
      popup.insertAction(0L, cancelAction);

      result = dynamic_cast<QAction *>(popup.exec(QCursor::pos()));
#if defined Q_WS_X11
    }
  }
#endif
  bool tooltip = showToolTips();
//   KIO::Job *job;
  if (result == copyAction)
  {
    setShowToolTips(false);
    setDragEnabled(false);
    QuantaNetAccess::dircopy(fileList, dest, m_plugin, true);
    setDragEnabled(true);
    setShowToolTips(tooltip);
  } else
  if (result == moveAction)
  {
    setShowToolTips(false);
    setDragEnabled(false);
    QuantaNetAccess::move(fileList, dest, m_plugin, true);
    setDragEnabled(true);
    setShowToolTips(tooltip);
  } else
  if (result == linkAction)
  {
    KIO::Job *job = KIO::link(fileList, dest);
    connect(job, SIGNAL( result(KJob *) ),
            this, SLOT( slotJobFinished(KJob *) ) );
  //   progressBar->setTotalSteps(100); FIXME
    connect(job, SIGNAL(percent(KJob *, unsigned long)),
            this, SLOT(slotPercent(KJob *, unsigned long)));
  }
  delete copyAction;
  delete moveAction;
  delete linkAction;
  delete cancelAction;
#endif
}


void BaseTreeView::findDrop(const QPoint &pos, Q3ListViewItem *&parent, Q3ListViewItem *&after)
{
  QPoint p (contentsToViewport(pos));
  Q3ListViewItem *atpos = itemAt(p);
//   if (atpos && atpos->parent()) {
//     after = atpos;
//     parent = atpos->parent();
//   } else {
    after = atpos;
    parent = atpos;
//   }
}


bool BaseTreeView::isPathInClipboard()
{
  QClipboard *cb = QApplication::clipboard();
  KUrl::List list( cb->text().split(QChar('\n')) );
  KUrl::List::Iterator end(list.end());
  for ( KUrl::List::Iterator it = list.begin(); it != end; ++it )
  {
    if ( !(*it).isValid() )
      return false;
  }
  return true;
}


void BaseTreeView::slotDocumentClosed(Koncrete::Document* document)
{
  if (!isVisible())
    return;

  KUrl url = document->url();
  KFileTreeViewItem * item;
  KFileTreeBranchIterator it( branches() );
  for ( ; it.current(); ++it)
  {
    item = (*it)->findTVIByUrl(url);
    if (item)
    {
      item->repaint();
    }
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


void BaseTreeView::slotRenameItem(Q3ListViewItem* item, const QString & newText, int col)
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

  KUrl oldURL = kftvi->url();
  KUrl newURL = oldURL;

  if (kftvi->isDir())
  {
    newURL.setPath(QFileInfo(oldURL.path(KUrl::RemoveTrailingSlash)).absoluteFilePath() + '/' + newName + '/');
    oldURL.adjustPath(KUrl::AddTrailingSlash);
  } else
  {
    newURL.setFileName(newName);
  }
  if ( oldURL != newURL )
  {
    bool proceed = true;
    if (ExtFileInfo::exists(newURL))
    {
      proceed = (KMessageBox::warningYesNo(this, i18n("<qt>The file <b>%1</b> already exists.<br>Do you want to overwrite it?</qt>", newURL.pathOrUrl()), i18n("Overwrite")) == KMessageBox::Yes);
    }
    if (proceed)
    {
    //start the rename job
      oldURL.adjustPath(KUrl::RemoveTrailingSlash);
      newURL.adjustPath(KUrl::RemoveTrailingSlash);
      if (!QuantaNetAccess::file_move(oldURL, newURL, -1, true, false, m_plugin, true))
      {
        kftvi->setText(0, kftvi->fileItem()->text());  // reset the text
      }
    } else
    {
       kftvi->setText(0, kftvi->fileItem()->text());  // reset the text
    }
  }
}


void BaseTreeView::saveLayout(KConfigGroup &configGroup)
{
  K3ListView::saveLayout(configGroup);
  if (! m_saveOpenFolder)
    return;

  BaseTreeBranch *btb;
  int i = 0;
  KFileTreeBranchIterator it( branches() );
  for ( ; it.current(); ++it)
  {
    btb = dynamic_cast<BaseTreeBranch *>( (*it) );
    if (btb && btb->rootUrl().isLocalFile()) {
      ++i;
      QStringList folderList;
      // remember the root url so that I find the branch on restore
      folderList.append(btb->rootUrl().url());
      btb->addOpenFolder(&folderList);
      configGroup.writePathEntry("OpenFolderList" + QString::number(i), folderList);
    }
  }
  configGroup.writeEntry("NumOpenFolderList", i);
}


void BaseTreeView::restoreLayout(KConfigGroup &configGroup)
{
  K3ListView::restoreLayout(configGroup);

  if (! m_saveOpenFolder)
    return;

  BaseTreeBranch *btb;
  KFileTreeBranchIterator it( branches() );
  int maxBranch = configGroup.readEntry("NumOpenFolderList", 0);
  for (int i = 1; i <= maxBranch; ++i)
  {
    QStringList folderList = configGroup.readPathListEntry("OpenFolderList" + QString::number(i));
    if (folderList.count() > 1) {
      KUrl rootURL = folderList.takeFirst();  // remove the root URL
      KFileTreeBranchIterator it( branches() );
      for ( ; it.current(); ++it)
      {
        if ((*it)->rootUrl() == rootURL) {
          btb = dynamic_cast<BaseTreeBranch *>( (*it) );
          if (btb) {
            btb->folderToOpen = folderList;
            btb->reopenFolder();
            break;
          }
        }
      }
    }
  }
}


void BaseTreeView::slotCreateFolder()
{
  bool ok;
  QString folderName = KInputDialog::getText(i18n("Create New Folder"), i18n("Folder name:"), "", &ok, this);
  if (ok)
  {
    KUrl url = currentUrl();
    if (currentKFileTreeViewItem()->isDir())
      url.setPath(url.path() + '/' + folderName + '/');
    else
      url.setPath(url.directory() + '/' + folderName +'/');
    QuantaNetAccess::mkdir(url, m_plugin, -1);
  }
}

void BaseTreeView::slotCreateFile()
{
  bool ok;
  QString fileName = KInputDialog::getText(i18n("Create New File"), i18n("File name:"), "", &ok, this);
  if (ok)
  {
    KUrl url = currentUrl();
    if (currentKFileTreeViewItem()->isDir())
      url.setPath(url.path() + '/' + fileName);
    else
      url.setPath(url.directory() + '/' + fileName);
    if (ExtFileInfo::exists(url))
    {
      KMessageBox::error(this, i18n("<qt>Cannot create file, because a file named <b>%1</b> already exists.</qt>", fileName), i18n("Error Creating File"));
      return;
    }
    KTemporaryFile *tempFile = new KTemporaryFile();
    tempFile->setPrefix(Helper::tmpFilePrefix());
    tempFile->open();
    if (QuantaNetAccess::copy(KUrl(tempFile->fileName()), url, m_plugin))
    {
      m_partController->editDocument(url);
    }
    delete tempFile;
  }
}


void BaseTreeView::slotMenu(K3ListView* listView, Q3ListViewItem *item, const QPoint &point)
{
  Q_UNUSED(listView);
  if (item)
  {
    setSelected(item, true);
    if (currentKFileTreeViewItem()->isDir())
      folderMenu(point);
    else
      fileMenu(point);
  } else
    emptyMenu(point);
}


QString BaseTreeView::iconNameForURL(const KUrl & url)
{
  if (url.isLocalFile())
    return KMimeType::iconNameForUrl(url);

  KUrl protURL(url);
  protURL.setPath("/");
  return KMimeType::iconNameForUrl(protURL);
}

bool BaseTreeView::isFileOpen(const KUrl & url) const
{
    return m_partController->documentForUrl(url);
}

#include "basetreeview.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
