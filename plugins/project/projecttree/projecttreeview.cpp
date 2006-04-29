/***************************************************************************
    begin                : Tue Mar 14 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2001-2004 Andras Mantia <amantia@kde.org>
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
#include "projecttreeview.h"
#include "extfileinfo.h"
#include "fileinfodlg.h"
#include "helper.h"
#include "quantaprojectif.h"
#include "quantanetaccess.h"
#include "settings.h"
#include "hacks.h"

// QT includes
#include <qpainter.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qhash.h>
#include <qtimer.h>

// KDE includes
#include <klocale.h>
#include <kiconloader.h>
#include <kinputdialog.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kmenu.h>
#include <kpropertiesdialog.h>
#include <ktempfile.h>
#include <kapplication.h>
#include <kstringhandler.h>
#include <kparts/part.h>

#include <kdevplugin.h>
#include <kdevproject.h>
#include <kdevplugininfo.h>
#include <kdevcore.h>


const QString & ProjectTreeView::m_attrDesc = KGlobal::staticQString("description");
const QString & ProjectTreeView::m_attrOpen = KGlobal::staticQString("OpenInTreeView");


//ProjectTreeViewItem implementation
ProjectTreeViewItem::ProjectTreeViewItem( KFileTreeViewItem *parent, KFileItem* item, KFileTreeBranch *brnch )
  : BaseTreeViewItem( parent, item, brnch), FilterableItemIf()
{
  setVisible(shouldBeVisible());
}


void ProjectTreeViewItem::settingsChanged()
{
  setVisible(shouldBeVisible());

  ProjectTreeViewItem *item = dynamic_cast<ProjectTreeViewItem *>(firstChild());
  while (item)
  {
    item->settingsChanged();
    item = dynamic_cast<ProjectTreeViewItem *>(item->nextSibling());
  }
}

bool ProjectTreeViewItem::filter(const QString &s) const
{
  return shouldBeVisible() && text(0).contains(s, Qt::CaseInsensitive);
}


bool ProjectTreeViewItem::shouldBeVisible() const
{
  KDevProject * p = static_cast<ProjectTreeBranch *>(branch())->project();
  return p->isProjectFile(url().path());
}



///////////////////////////////////////////////
//
//ProjectTreeBranch implementation
//
///////////////////////////////////////////////

ProjectTreeBranch::ProjectTreeBranch(KDevProject * project, BaseTreeView *parent, const KUrl& url, const QString& name, const QPixmap& pix, bool showHidden, KFileTreeViewItem *branchRoot)
  : BaseTreeBranch(parent, url, name, pix, showHidden, branchRoot), m_project(project)
{
}

KFileTreeViewItem* ProjectTreeBranch::createTreeViewItem(KFileTreeViewItem *parent,
                                                         KFileItem *fileItem )
{
  BaseTreeViewItem  *tvi = 0;
  if( parent && fileItem )
  {
    tvi = new ProjectTreeViewItem( parent, fileItem, this );
    if (tvi)
    {
      // we assume there are children
      tvi->setExpandable(tvi->isDir());
    }
  }
  else
    kDebug(24000) << "ProjectTreeBranch::createTreeViewItem: Have no parent" << endl;
  return tvi;
}

////////////////////////////////////////////
//
//
// FIXME check the parent relations
/////////////////////////////////////////////

ProjectTreeView::ProjectTreeView(KDevPlugin * plugin, QWidget *parent)
  : BaseTreeView(plugin, parent), m_projectDir(0), m_plugin(plugin), m_quantaProject(dynamic_cast<QuantaProjectIf *>(m_plugin->project()))
{
  setShowToolTips(Settings::self()->projectTreeTooltips());
  //setSelectionModeExt(K3ListView::Extended);
  setRootIsDecorated(true);
  addColumn(i18n("Project Files"), -1);
  addColumn(i18n("Description"), -1);
  setDragEnabled(true);
  setAcceptDrops(true);
  setItemsRenameable(true);
  setRenameable(0, false);
  setRenameable(1, true);

  connect(this, SIGNAL(contextMenu(K3ListView*, Q3ListViewItem*, const QPoint&)),
          this, SLOT(slotMenu(K3ListView*, Q3ListViewItem*, const QPoint&)));

  connect(this, SIGNAL(open(Q3ListViewItem *)),
          this, SLOT(slotSelectFile(Q3ListViewItem *)));

  restoreLayout(KGlobal::config(), metaObject()->className());
  // the restored size of the first column might be too large for the current content
  // we set it to 10 and the listview will adjust it to the size of the largest entry
  setColumnWidth(0, 10);
}

ProjectTreeView::~ProjectTreeView(){
}


KFileTreeBranch* ProjectTreeView::newBranch(const KUrl& url)
{
  QString m_projectNameStr = m_projectName;
  if (!m_projectName.isEmpty())
  {
    if (! url.isLocalFile())
    {
      m_projectNameStr += " [" + url.protocol() + "://" + url.user() + "@" + url.host() + "]";
    }
    m_projectDir = new ProjectTreeBranch(m_quantaProject, this, url, m_projectNameStr, SmallIcon(m_plugin->info()->icon()), true);
    m_projectDir->root()->setText(1, url.pathOrURL());
    setDragEnabled(true);
  } else
  {
    m_projectDir = new ProjectTreeBranch(m_quantaProject, this, url, i18n("No Project"), SmallIcon(m_plugin->info()->icon()), true);
    m_projectDir->root()->setText(1, "");
    setDragEnabled(false);
  }

  connect(m_projectDir, SIGNAL(populateFinished(KFileTreeViewItem*)),       SLOT(slotPopulateFinished(KFileTreeViewItem*)));
  addBranch(m_projectDir);
  if (!m_projectName.isEmpty())
    m_projectDir->populate(m_projectDir->rootUrl(), m_projectDir->root());

  return m_projectDir;
}

void ProjectTreeView::fileMenu(const QPoint &point)
{
  KMenu popup(this);

  if (isFileOpen(currentKFileTreeViewItem()->url()))
    popup.addAction(SmallIcon("fileclose"), i18n("Clos&e"), this, SLOT(slotClose()));
  else
    popup.addAction(SmallIcon("fileopen"), i18n("&Open"), this, SLOT(slotOpen()));

  popup.addSeparator();
  popup.addAction(i18n("Re&name"), this, SLOT(slotStartRename()));
  popup.addAction(SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  popup.addSeparator();
  popup.addAction(SmallIcon("info"), i18n("&Properties"), this, SLOT(slotProperties()));

  // ask other plugins for menu entries
  KUrl::List urlList(currentKFileTreeViewItem()->url());
  FileContext context(urlList);
  m_plugin->core()->fillContextMenu(&popup, &context);

  popup.exec(point);
}


void ProjectTreeView::folderMenu(const QPoint &point)
{
  ProjectTreeViewItem *curItem = static_cast<ProjectTreeViewItem *>(currentKFileTreeViewItem());
  if (curItem == curItem->branch()->root())
    emptyMenu(point);

  KMenu createNewMenu(this);
  createNewMenu.setTitle(i18n("&Create New"));
  createNewMenu.setIcon(SmallIconSet("filenew"));
  createNewMenu.addAction(SmallIcon("folder_new"), i18n("F&older..."), this, SLOT(slotCreateFolder()));
  createNewMenu.addAction(SmallIcon("document"), i18n("&File..."), this, SLOT(slotCreateFile()));

  KMenu popup(this);
  Hack::KMenuAddTitle(&popup, i18n("Project Tree"));

  popup.addMenu(&createNewMenu);
  popup.addSeparator();
  popup.addAction(i18n("Re&name"), this, SLOT(slotStartRename()));
  popup.addAction(SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  popup.addSeparator();
  popup.addAction(SmallIcon("info"), i18n("&Properties"), this, SLOT(slotProperties()));

  // ask other plugins for menu entries
  KUrl menuURL(currentKFileTreeViewItem()->url());
  menuURL.adjustPath(+1);
  KUrl::List urlList(menuURL);
  FileContext context(urlList);
  m_plugin->core()->fillContextMenu(&popup, &context);

  popup.exec(point);
}


void ProjectTreeView::emptyMenu(const QPoint &point)
{
  setSelected(firstChild(), true);

  KMenu createNewMenu(this);
  createNewMenu.setTitle(i18n("&Create New"));
  createNewMenu.setIcon(SmallIconSet("filenew"));
  createNewMenu.addAction(SmallIcon("folder_new"), i18n("F&older..."), this, SLOT(slotCreateFolder()));
  createNewMenu.addAction(SmallIcon("document"), i18n("&File..."), this, SLOT(slotCreateFile()));

  KMenu popup(this);
  Hack::KMenuAddTitle(&popup, i18n("Project Tree"));

  popup.addMenu(&createNewMenu);
  popup.addSeparator();
//   popup.insertItem(SmallIcon("reload"), i18n("Re&scan Project Folder..."), this, SLOT(slotRescan()));
//   popup.insertItem(SmallIcon("configure"), i18n("Project &Properties"), this, SLOT(slotOptions()));
  popup.addAction(SmallIcon("revert"), i18n("&Reload"), this, SLOT(slotReload()));

  popup.exec(point);
}


void ProjectTreeView::slotProjectOpened()
{
  m_projectName = m_plugin->project()->projectName();
  if (m_quantaProject)
    m_projectBaseURL = m_quantaProject->projectBase();
  else
    m_projectBaseURL = KUrl::fromPathOrURL(m_plugin->project()->projectDirectory());

  if (m_projectDir)  // just in case we have already one
  {
    removeBranch(m_projectDir);  // the branch get deleted
    m_projectDir = 0L;
  }
  newBranch(m_projectBaseURL);
  m_projectDir->setOpen(true);

  // restore open folders ?
  if (! Settings::self()->projectTreeSave())
    return;

  Q3ListViewItemIterator it(m_projectDir->root());
  ++it; //skip the project base folder, it has no Dom element
  for ( ; it.current(); ++it )
  {
    KFileTreeViewItem * item = static_cast<KFileTreeViewItem *>(*it);
    QDomElement el = getDomElement(item);
    if (!el.isNull() && !el.attribute(m_attrOpen, false).isEmpty())
    {
      item->setOpen(true);
    }
  }
}


void ProjectTreeView::slotProjectClosed()
{
  // save open folders ?
  if (Settings::self()->projectTreeSave())
  {
    Q3ListViewItemIterator it(m_projectDir->root());
    for ( ; it.current(); ++it )
    {
      KFileTreeViewItem * item = static_cast<KFileTreeViewItem *>(*it);
      if (item->isDir())
      {
        QDomElement el = getDomElement(item);
        if (! el.isNull())
          if (item->isOpen())
            el.setAttribute(m_attrOpen, true);
          else
            el.removeAttribute(m_attrOpen);
      }
    }
  }
}


void ProjectTreeView::slotOpen()
{
  BaseTreeView::slotSelectFile(currentItem());
}


void ProjectTreeView::slotCreateFolder()
{
  QTimer::singleShot(0, this, SLOT(createFolder()));
}

void ProjectTreeView::createFolder()
{
  bool ok;
  QString folderName = KInputDialog::getText(i18n("Create New Folder"), i18n("Folder name:"), "", &ok, this);
  if (ok)
  {
     KUrl url = currentURL();
     if (currentKFileTreeViewItem()->isDir())
       url.setPath(url.path() + "/" + folderName + "/");
     else
       url.setPath(url.directory() + "/" + folderName +"/");

     QuantaNetAccess::mkdir(url, m_plugin, -1, false);
  }
}

void ProjectTreeView::slotCreateFile()
{
  bool ok;
  QString fileName = KInputDialog::getText(i18n("Create New File"), i18n("File name:"), "", &ok, this);
  if (ok)
  {
    KUrl url = currentURL();
    if (currentKFileTreeViewItem()->isDir())
      url.setPath(url.path() + "/" + fileName);
    else
      url.setPath(url.directory() + "/" + fileName);
    if (ExtFileInfo::exists(url))
    {
      KMessageBox::error(this, i18n("<qt>Cannot create file, because a file named <b>%1</b> already exists.</qt>", fileName), i18n("Error Creating File"));
      return;
    }
    KTempFile *tempFile = new KTempFile(Helper::tmpFilePrefix());
    tempFile->setAutoDelete(true);
    tempFile->close();
    if (ExtFileInfo::copy(KUrl::fromPathOrURL(tempFile->name()), url));
    {
// FIXME      emit insertToProject(url);
//       emit openFile(url);
    }
    delete tempFile;
  }
}


void ProjectTreeView::slotRescan()
{
  emit rescanProjectDir();
}

/** Bring up the project options dialog */
void ProjectTreeView::slotOptions()
{
  emit showProjectOptions();
}


void ProjectTreeView::slotPopulateFinished(KFileTreeViewItem* item)
{
  BaseTreeView::slotPopulateFinished(item);
  // populate descriptions
  QString desc;
  KFileTreeViewItem* file_item;
  Q3ListViewItemIterator it(item);
  ++it;
  for ( ; it.current(); ++it )
  {
    // we only want the subtree
    if (*it == item->nextSibling() || *it == item->parent())
      break;
    file_item = static_cast<KFileTreeViewItem*>(it.current());

    QDomElement el = getDomElement(file_item);
    if (! el.isNull())
    {
      desc = el.attribute(m_attrDesc);
      if (! desc.isEmpty())
      {
        file_item->setText(1, desc);
      }
    }
  }
}


void ProjectTreeView::itemDescChanged(KFileTreeViewItem* item, const QString& newDesc)
{
  if (! item)
    return;

  item->setText(1, newDesc);
  KUrl url = item->url();

  QDomElement el = getDomElement(item);
  if (el.isNull())
    return;

  if (newDesc.isEmpty())
    el.removeAttribute(m_attrDesc);
  else
    el.setAttribute(m_attrDesc, newDesc);
}


QDomElement ProjectTreeView::getDomElement(KFileTreeViewItem* item)
{
  if (! m_quantaProject)
    return QDomElement();

  KUrl url = item->url();
  if (item->isDir())
    url.adjustPath(1);

  QString path = m_quantaProject->relativeProjectFile(url.path());
  return m_quantaProject->domElementForPath(path);
}


void ProjectTreeView::slotRenameItem(Q3ListViewItem* kftvi, const QString& newText, int col)
{
  if (!kftvi)
    return;

  if (col == 1) {
    itemDescChanged(dynamic_cast <KFileTreeViewItem*> (kftvi), newText);
  } else {
    BaseTreeView::slotRenameItem(kftvi, newText, col);
  }
}


FileInfoDlg* ProjectTreeView::addFileInfoPage(KPropertiesDialog* propDlg)
{
  FileInfoDlg* dlg = BaseTreeView::addFileInfoPage(propDlg);
  if (dlg) {
    dlg->fileDescLbl->show();
    dlg->fileDesc->show();
  }
  return dlg;
}


void ProjectTreeView::slotActivePartChanged(KParts::Part *part)
{
  if (! m_projectDir && !isVisible())
    return;

  KParts::ReadOnlyPart * roPart = dynamic_cast<KParts::ReadOnlyPart *>(part);
  if (! roPart)
    return;

  KFileTreeViewItem* item = m_projectDir->findTVIByURL(roPart->url());
  if (item)
  {
    ensureItemVisible(item);
    setSelected(item, true);
  }
}


void ProjectTreeView::slotSettingsChanged()
{
  setShowToolTips(Settings::self()->projectTreeTooltips());
}

#include "projecttreeview.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
