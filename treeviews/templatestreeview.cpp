/***************************************************************************
                          templatestreeview.cpp  -  description
                             -------------------
    begin                : Thu Dec 20 2001
    copyright            : (C) 2001-2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <sys/types.h>

// QT includes
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qclipboard.h>
#include <qdir.h>
#include <qpixmap.h>
#include <qheader.h>
#include <qpoint.h>
#include <qlayout.h>
#include <qtextedit.h>
#include <qregexp.h>
#include <qlabel.h>
#include <qmap.h>
#include <qwidgetstack.h>

// KDE includes
#include <kapplication.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kstandarddirs.h>
#include <kmainwindow.h>
#include <kmimetype.h>
#include <kmessagebox.h>
#include <kcombobox.h>
#include <kpopupmenu.h>
#include <kpropertiesdialog.h>
#include <kurlrequester.h>
#include <kurlrequesterdlg.h>
#include <ktempdir.h>
#include <ktempfile.h>
#include <kio/netaccess.h>
#include <kurldrag.h>
#include <ktar.h>
#include <kdebug.h>
#include <kfile.h>


#include "templatestreeview.h"
#include "copyto.h"
#include "newtemplatedirdlg.h"
#include "qextfileinfo.h"
#include "quantanetaccess.h"
#include "quantapropertiespage.h"
#include "resource.h"
#include "tagaction.h"
#include "tagmaildlg.h"

#define EXCLUDE ".*\\.tmpl$"
#define TMPL ".tmpl"
#define NONE "None"


const QString textMenu = I18N_NOOP("Insert as &Text");
const QString binaryMenu = I18N_NOOP("Insert &Link to File");
const QString docMenu = I18N_NOOP("&New Document Based on This");
const QString siteMenu = I18N_NOOP("&Extract Site Template To...");
QMap<QString, QString> typeToi18n;
QMap<QString, QString> i18nToType;


//TemplatesTreeBranch implementation
TemplatesTreeBranch::TemplatesTreeBranch(KFileTreeView *parent, const KURL& url,
                                         const QString& name, const QPixmap& pix,
                                         bool showHidden,
                                         KFileTreeViewItem *branchRoot)
    : BaseTreeBranch(parent, url, name, pix, showHidden, branchRoot)
{
}

KFileTreeViewItem* TemplatesTreeBranch::createTreeViewItem(KFileTreeViewItem *parent,
                                                           KFileItem *fileItem )
{
  BaseTreeViewItem  *tvi = 0;
  if( parent && fileItem )
  {
    KURL url = fileItem->url();
    tvi = new BaseTreeViewItem( parent, fileItem, this );
    if (tvi && fileItem->isDir())
    {
      if (url.isLocalFile())
      {
        QDir dir (url.path(), "", QDir::All);
        tvi->setExpandable(dir.count() != 2);     //   . and .. are always there
      } else {
        tvi->setExpandable(true);     //   we assume there is something
      }
    } else
    {
      url = static_cast<BaseTreeViewItem*>(parent)->url();
    }
    QFileInfo dotFileInfo(url.path() + "/.dirinfo");
    while ((!dotFileInfo.exists()) && (dotFileInfo.dirPath() != "/"))
    {
    dotFileInfo.setFile(QFileInfo(dotFileInfo.dirPath()).dirPath()+"/.dirinfo");
    }
    if (dotFileInfo.exists())
    {
      KConfig *config = new KConfig(dotFileInfo.filePath());
      QString s = config->readEntry("Type");
      tvi->setText(1, typeToi18n[s]);
      delete config;
    }

  }
  else
    kdDebug(24000) << "TemplatesTreeBranch::createTreeViewItem: Have no parent" << endl;
  return tvi;
}



TemplatesTreeView::TemplatesTreeView(KMainWindow *parent, const char *name )
  : BaseTreeView(parent,name), m_projectDir(0), m_mainWindow(parent)
{
  typeToi18n["text/all"] = i18n("Text Snippet");
  typeToi18n["file/all"] = i18n("Binary File");
  typeToi18n["template/all"] = i18n("Document Template");
  typeToi18n["site/all"] = i18n("Site Template");
  i18nToType[i18n("Text Snippet")] = "text/all";
  i18nToType[i18n("Binary File")] = "file/all";
  i18nToType[i18n("Document Template")] = "template/all";
  i18nToType[i18n("Site Template")] = "site/all";

  m_fileMenu = new KPopupMenu(this);

  m_openId = m_fileMenu->insertItem(i18n("Open"), this ,SLOT(slotInsert()));
  m_fileMenu->insertItem(SmallIcon("fileopen"), i18n("&Open"), this ,SLOT(slotOpen()));
  m_fileMenu->insertItem(SmallIcon("mail_send"), i18n("Send in E&mail..."), this, SLOT(slotSendInMail()));
  m_fileMenu->insertItem(SmallIcon("network"), i18n("&Upload Template..."), this, SLOT(slotUploadTemplate()));
  m_insertFileInProject = m_fileMenu->insertItem(i18n("&Insert in Project..."), this, SLOT(slotInsertInProject()));
  m_menuClose = m_fileMenu->insertItem(SmallIcon("fileclose"), i18n("Clos&e"), this, SLOT(slotClose()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));
  m_fileMenu->insertItem(SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(SmallIcon("info"), i18n("&Properties"), this, SLOT(slotProperties()));

  m_folderMenu = new KPopupMenu(this);

  m_folderMenu->insertItem(SmallIcon("folder_new"), i18n("&New Folder..."), this, SLOT(slotNewDir()));
  m_folderMenu->insertItem(SmallIcon("mail_send"), i18n("Send in E&mail..."), this, SLOT(slotSendInMail()));
  m_folderMenu->insertItem(SmallIcon("network"), i18n("&Upload Template..."), this, SLOT(slotUploadTemplate()));
  m_downloadMenuId = m_folderMenu->insertItem(SmallIcon("network"), i18n("&Download Template..."), this, SIGNAL(downloadTemplate()));
  m_insertFolderInProject = m_folderMenu->insertItem(i18n("&Insert in Project..."), this, SLOT(slotInsertDirInProject()));
  m_folderMenu->insertSeparator();
  m_folderMenu->insertItem(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));
  m_menuPasteFolder = m_folderMenu->insertItem(SmallIcon("editpaste"), i18n("&Paste"), this, SLOT(slotPaste()));
  m_deleteMenuId = m_folderMenu->insertItem(SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_folderMenu->insertSeparator();
  m_folderMenu->insertItem(SmallIcon("info"), i18n("&Properties"), this, SLOT(slotProperties()));
  m_reloadMenuId = m_folderMenu->insertItem(SmallIcon("revert"), i18n("&Reload"), this, SLOT(slotReload()));

  m_emptyAreaMenu = new KPopupMenu(this);
  m_emptyAreaMenu->insertItem(SmallIcon("network"), i18n("&Download Template..."), this, SIGNAL(downloadTemplate()));

  addColumn(i18n("Templates"), -1);
  addColumn(i18n("Group"), -1);

  globalURL.setPath(qConfig.globalDataDir + resourceDir + "templates/");
  newBranch(globalURL);

  localURL.setPath(locateLocal("data", resourceDir + "templates/"));
  newBranch(localURL);

  connect(this, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
          this, SLOT(slotMenu(KListView*, QListViewItem*, const QPoint&)));

  connect(this, SIGNAL(open(QListViewItem *)),
          this,  SLOT(slotSelectFile(QListViewItem *)));

  setAcceptDrops(true);
  setSelectionMode(QListView::Single);
  setDragEnabled(true);
  setSaveOpenFolder(true);
  restoreLayout( kapp->config(), className() );
  // the restored size of the first column might be too large for the current content
  // we set it to 10 and the listview will adjust it to the size of the largest entry
  setColumnWidth(0, 10);
}

TemplatesTreeView::~TemplatesTreeView()
{
}


KFileTreeBranch* TemplatesTreeView::newBranch(const KURL& url)
{
  BaseTreeBranch *newBrnch;
  if (url == globalURL)
  {
    newBrnch = new TemplatesTreeBranch(this, url, i18n("Global Templates"), SmallIcon("ttab"));
  } else
  {
    if (url == localURL)
      newBrnch = new TemplatesTreeBranch(this, url, i18n("Local Templates"), SmallIcon("ttab"));
    else
    {
      newBrnch = new TemplatesTreeBranch(this, url, i18n("Project Templates"), SmallIcon("ptab"));
      m_projectDir = newBrnch;
    }
  }
  newBrnch->excludeFilterRx.setPattern(EXCLUDE);
  addBranch(newBrnch);
  if (url.isLocalFile())
  {
    QDir dir (url.path(), "", QDir::All);
    newBrnch->root()->setExpandable(dir.count() != 2);     //   . and .. are always there
  } else {
    newBrnch->root()->setExpandable(true);     //   we assume there is something
  }

  return newBrnch;
}


/** No descriptions */
void TemplatesTreeView::slotInsertInDocument()
{

 KURL url = filterTemplate();
 if (QuantaCommon::checkMimeGroup(url, "text"))
 {
   emit insertFile(url);
 } else
 {
   if (QuantaCommon::denyBinaryInsert(this) == KMessageBox::Yes)
   {
     emit insertFile(url);
   }
 }

}

void TemplatesTreeView::slotMenu(KListView*, QListViewItem *item, const QPoint &point)
{
  if (!item)
  {
    m_emptyAreaMenu->popup(point);
    return;
  }
  setSelected(item, true);
  bool hasProject = m_projectName;
  m_folderMenu->setItemVisible(m_insertFolderInProject, hasProject);
  m_fileMenu->setItemVisible(m_insertFileInProject, hasProject);

  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if ( curItem->isDir() )
  {
    m_folderMenu->setItemVisible(m_menuPasteFolder, isPathInClipboard());
    if ( curItem == curItem->branch()->root())
    {
      m_folderMenu ->setItemVisible(m_deleteMenuId, false);
      m_folderMenu ->setItemVisible(m_reloadMenuId, true);
      m_folderMenu ->setItemVisible(m_downloadMenuId, true);
    } else
    {
      m_folderMenu ->setItemVisible(m_deleteMenuId, true);
      m_folderMenu ->setItemVisible(m_reloadMenuId, false);
      m_folderMenu ->setItemVisible(m_downloadMenuId, false);
    }
    m_folderMenu ->popup(point);
  } else
  {
   m_dirInfo = readDirInfo();

   QString menuText = "";

   if (m_dirInfo.mimeType.upper().contains("TEXT"))
     menuText = i18n(textMenu.utf8());
   else if (m_dirInfo.mimeType.upper().contains("FILE"))
     menuText = i18n(binaryMenu.utf8());
   else if (m_dirInfo.mimeType.upper().contains("TEMPLATE"))
     menuText = i18n(docMenu.utf8());
   else if (m_dirInfo.mimeType.upper().contains("SITE"))
     menuText = i18n(siteMenu.utf8());

   if (menuText.isEmpty())
   {
     m_fileMenu->setItemVisible(m_openId, false);
   } else
   {
     m_fileMenu->setItemVisible(m_openId, true);
     m_fileMenu->changeItem(m_openId, menuText);
   }
   m_fileMenu->setItemVisible(m_menuClose, isFileOpen(currentURL()));

   m_fileMenu->popup( point);
  }
}
/** No descriptions */
void TemplatesTreeView::slotNewDocument()
{
 KURL url = filterTemplate();
 if (QuantaCommon::checkMimeGroup(url, "text") || QuantaCommon::denyBinaryInsert(this) == KMessageBox::Yes)
 {
   QListViewItem *item = currentItem();
   if (item )
   {
     if ( currentKFileTreeViewItem() != currentKFileTreeViewItem()->branch()->root())
     {
       emit openFile(KURL());
       emit insertFile(url);
     }
   }
 }
}

/** Insert the template as text, image, new document. */
void TemplatesTreeView::slotInsert()
{
 QString menuText = m_fileMenu->text(m_openId);

 if (menuText == i18n(textMenu.utf8()))
   slotInsertInDocument();
 else if (menuText == i18n(binaryMenu.utf8()))
   slotInsertTag();
 else if (menuText == i18n(docMenu.utf8()))
   slotNewDocument();
 if (menuText == i18n(siteMenu.utf8()))
   slotExtractSiteTemplate();
}

void TemplatesTreeView::slotSelectFile(QListViewItem *item)
{
  if ( !item ) return;

  KFileTreeViewItem *kftvItem = currentKFileTreeViewItem();
  if ( !kftvItem ) return;

  if ( !kftvItem->isDir() )
  {
    m_dirInfo = readDirInfo();
/*    if (m_dirInfo.mimeType.upper().contains("SITE"))
    {
      slotExtractSiteTemplate();
      return;
    }*/
    if (expandArchiv(kftvItem)) return;
    if (m_dirInfo.mimeType.upper().contains("TEXT"))
        slotInsertInDocument();
    else if (m_dirInfo.mimeType.upper().contains("FILE"))
        slotInsertTag();
    else if (m_dirInfo.mimeType.upper().contains("TEMPLATE"))
        slotNewDocument();
  }
}

void TemplatesTreeView::slotOpen()
{
  BaseTreeView::slotSelectFile(currentItem());
}

/** No descriptions */
void TemplatesTreeView::slotNewDir()
{
  NewTemplateDirDlg *createDirDlg = new NewTemplateDirDlg(this,i18n("Create New Template Folder"));
  createDirDlg->typesCombo->insertItem(typeToi18n["text/all"]);
  createDirDlg->typesCombo->insertItem(typeToi18n["file/all"]);
  createDirDlg->typesCombo->insertItem(typeToi18n["template/all"]);
  createDirDlg->typesCombo->insertItem(typeToi18n["site/all"]);

  m_dirInfo = readDirInfo();

   if (m_dirInfo.mimeType.isEmpty())
   {
    createDirDlg->parentAttr->setText(i18n("&Inherit parent attribute (nothing)"));
   } else
   {
     createDirDlg->parentAttr->setText(i18n("&Inherit parent attribute (%1)").arg(typeToi18n[m_dirInfo.mimeType]));
   }
   if (createDirDlg->exec())
   {
    QDir dir;

   QString startDir = "";
   if ( !currentKFileTreeViewItem()->isDir() )
   {
    startDir = currentURL().path();
   } else
   {
    startDir = currentURL().path() + "/dummy_file";
   }
   startDir = QFileInfo(startDir).dirPath();
   if (!dir.mkdir(startDir+"/"+createDirDlg->dirName->text()))
   {
      KMessageBox::error(this,i18n("Error while creating the new folder.\n \
                   Maybe you do not have permission to write in the %1 folder.").arg(startDir));
      return;
   }
   if (! createDirDlg->parentAttr->isChecked())
   {
      m_dirInfo.mimeType = i18nToType[createDirDlg->typesCombo->currentText()];
      m_dirInfo.preText = "";
      m_dirInfo.postText = "";
      writeDirInfo(startDir+"/"+createDirDlg->dirName->text()+"/.dirinfo");
   }
  }
}
/** No descriptions */

QDragObject * TemplatesTreeView::dragObject ()
{
  // don't drag folders
  if ( ! currentKFileTreeViewItem() ||  currentKFileTreeViewItem()->isDir() ) return 0;

  m_dirInfo = readDirInfo();
  if(!m_dirInfo.mimeType.isEmpty()) // only drag when the template type is specified
  {
    KURLDrag *drag = new KURLDrag(KURL::List(currentURL()), this);
    return drag;
  }
  return 0;
}

/** No descriptions */
void TemplatesTreeView::contentsDropEvent(QDropEvent *e)
{
  if (KURLDrag::canDecode(e))
  {
    // handles url drops
    BaseTreeView::contentsDropEvent(e);
    return;
  }
  if (QTextDrag::canDecode(e))
  {
    QListViewItem *item = itemAt(contentsToViewport(e->pos()));
    if (item)
    {
      KURL dest;
      if ( currentKFileTreeViewItem()->isDir() )
        dest = currentURL();
      else
        dest = currentURL().directory(false);
      dest.adjustPath(+1);
      QString content;
      QTextDrag::decode(e, content);
      KURL url =KURLRequesterDlg::getURL( dest.path() + "template.txt",
                                          this, i18n("Save selection as template file:"));
      if ( !url.isEmpty() )
      {
        //now save the file
        KTempFile* tempFile = new KTempFile(tmpDir);
        tempFile->setAutoDelete(true);
        tempFile->textStream()->setEncoding(QTextStream::UnicodeUTF8);
        *(tempFile->textStream()) << content;
        tempFile->close();
        bool proceed = true;
        if (QExtFileInfo::exists(url, false, this))
        {
          proceed = KMessageBox::warningContinueCancel(this, i18n("<qt>The file <b>%1</b> already exists.<br>Do you want to overwrite it?</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)),i18n("Overwrite"), i18n("Overwrite")) == KMessageBox::Continue;
        }
        if (proceed)
        {
          if (!QuantaNetAccess::upload(tempFile->name(), url, m_parent, false))
          {
            KMessageBox::error(this,i18n("<qt>Could not write to file <b>%1</b>.<br>Check if you have rights to write there or that your connection is working.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)));
          }
        }
        delete tempFile;
      }
    }
  }
  // must be done to reset timer etc.
  BaseTreeView::contentsDropEvent(e);
}

/** Reads a .dirinfo file from the selected item's path */
DirInfo TemplatesTreeView::readDirInfo(const QString& dir)
{
  DirInfo dirInfo;
  QString startDir = dir;

  if (startDir.isEmpty())
  {
    if (!currentKFileTreeViewItem()->isDir())
    {
      startDir = currentURL().path();
    } else
    {
     startDir = currentURL().path() + "/dummy_file";
    }
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
  dirInfo.usePrePostText = config->readBoolEntry("UsePrePostText", false);

  delete config;
  return dirInfo;
}

/** Writes a .dirinfo file from the selected item's path */
bool TemplatesTreeView::writeDirInfo(const QString& m_dirInfoFile)
{
  QString startDir = "";

  if (m_dirInfoFile.isEmpty())
  {
    if ( !currentKFileTreeViewItem()->isDir() )
    {
      startDir = currentURL().path();
    } else
    {
      startDir = currentURL().path() + "/dummy_file";
    }
  } else
  {
    startDir = m_dirInfoFile;
  }

  QFileInfo dotFileInfo(QFileInfo(startDir).dirPath()+"/.dirinfo");

  bool success = false;
  KConfig *config = new KConfig(dotFileInfo.filePath());
  if (!config->isReadOnly())
  {
    config->writeEntry("Type", m_dirInfo.mimeType);
    config->writeEntry("PreText", m_dirInfo.preText);
    config->writeEntry("PostText", m_dirInfo.postText);
    config->writeEntry("UsePrePostText", m_dirInfo.usePrePostText);
    config->sync();
    success = true;
  }
  delete config;
  return success;
}

void TemplatesTreeView::slotProperties()
{
  if ( !currentItem() ) return;
  KURL url = currentURL();

  KPropertiesDialog *propDlg = new KPropertiesDialog( url, this, 0L, false, false); //autodeletes itself

//Always add the Quanta directory page
  QFrame *quantaDirPage = propDlg->addPage(i18n("Quanta Template"));
  QVBoxLayout *topLayout = new QVBoxLayout( quantaDirPage);
  m_quantaProperties = new QuantaPropertiesPage( quantaDirPage, i18n("Quanta") );

  m_quantaProperties->typesCombo->insertItem(typeToi18n["text/all"]);
  m_quantaProperties->typesCombo->insertItem(typeToi18n["file/all"]);
  m_quantaProperties->typesCombo->insertItem(typeToi18n["template/all"]);
  m_quantaProperties->typesCombo->insertItem(typeToi18n["site/all"]);

  m_dirInfo = readDirInfo();

  m_quantaProperties->typesCombo->setCurrentItem(typeToi18n[m_dirInfo.mimeType]);

  KIO::UDSEntry entry;
  KIO::NetAccess::stat(url, entry, this);
  KFileItem fItem(entry, url);
  QString permissions = fItem.permissionsString();
  QString userName;
  struct passwd *user = getpwuid(getuid());
  if (user)
    userName = QString::fromLocal8Bit(user->pw_name);
  QString groupName;
  gid_t gid = getgid();
  struct group *ge = getgrgid(gid);
  if (ge)
  {
    groupName = QString::fromLocal8Bit(ge->gr_name);
    if (groupName.isEmpty())
        groupName.sprintf("%d", ge->gr_gid);
  } else
    groupName.sprintf("%d", gid);
  bool writable = false;
  if (permissions[8] == 'w' || (permissions[2] == 'w' && userName == fItem.user()) || (permissions[5] == 'w' && groupName == fItem.group()))
    writable = true;

  QString startDir = "";
  if (!currentKFileTreeViewItem()->isDir())
  {
    startDir = url.path();
    m_quantaProperties->typeStack->raiseWidget(1);
  } else
  {
    startDir = url.path() + "/dummy_file";
    m_quantaProperties->typeStack->raiseWidget(0);
  }
  m_quantaProperties->setEnabled(writable);
  QFileInfo dotFileInfo(QFileInfo(startDir).dirPath()+"/.dirinfo");
  m_parentDirInfo = readDirInfo(dotFileInfo.dirPath());
  if (!dotFileInfo.exists() || m_dirInfo.mimeType == m_parentDirInfo.mimeType)
  {
     m_quantaProperties->parentAttr->setChecked(true);
  }
  if (m_parentDirInfo.mimeType.isEmpty())
   {
    m_quantaProperties->parentAttr->setText(i18n("&Inherit parent attribute (nothing)"));
   } else
   {
    m_quantaProperties->parentAttr->setText(i18n("&Inherit parent attribute (%1)").arg(typeToi18n[m_parentDirInfo.mimeType]));
   }
   m_quantaProperties->preTextEdit->setText(m_dirInfo.preText);
   m_quantaProperties->postTextEdit->setText(m_dirInfo.postText);
   if (m_dirInfo.usePrePostText)
   {
    m_quantaProperties->usePrePostText->setChecked(true);
   }

   topLayout->addWidget( m_quantaProperties );
   connect( propDlg, SIGNAL( applied() ), this , SLOT( slotPropertiesApplied()) );

  QString name = url.path() + TMPL;
  KConfig config(name);
  config.setGroup("Filtering");
  name = config.readEntry("Action", NONE);
  if ( name == NONE )
     name = i18n(NONE);
  uint pos = 0;
  uint j = 1;
  m_quantaProperties->actionCombo->insertItem(i18n(NONE));
  QString tmpStr;
  KActionCollection *ac = m_mainWindow->actionCollection();
  uint acCount = ac->count();
  for (uint i = 0; i < acCount; i++)
  {
    TagAction *action = dynamic_cast<TagAction*>(ac->action(i));
    if (action)
    {
      QDomElement el = action->data();
      QString type = el.attribute("type", "tag");
      if (type == "script")
      {
        tmpStr = action->text().replace(QRegExp("\\&(?!\\&)"),"");
        m_quantaProperties->actionCombo->insertItem(tmpStr);
        if (tmpStr == name)
            pos = j;
        j++;
      }
    }
  }
  m_quantaProperties->actionCombo->setCurrentItem(pos);

//If the item is a file, add the Quanta file info page
  addFileInfoPage(propDlg);
  if (propDlg->exec() == QDialog::Accepted)
   {
//TODO: move to slotPropertiesApplied
    if (url != propDlg->kurl())
    {
      itemRenamed(url, propDlg->kurl());
    }
   }
}


/** No descriptions */
void TemplatesTreeView::slotPropertiesApplied()
{
  DirInfo m_localDirInfo;
  QString typeString = "";

  if (!m_quantaProperties->parentAttr->isChecked())
  {
    m_localDirInfo.mimeType = m_quantaProperties->typesCombo->currentText();
    typeString = m_localDirInfo.mimeType;
    m_localDirInfo.mimeType = i18nToType[m_localDirInfo.mimeType];

  } else
  {
    if (m_dirInfo.mimeType != m_parentDirInfo.mimeType)
      typeString = typeToi18n[m_parentDirInfo.mimeType];
    m_localDirInfo.mimeType = m_parentDirInfo.mimeType;
  }

  m_localDirInfo.usePrePostText = m_quantaProperties->usePrePostText->isChecked();
  m_localDirInfo.preText = m_quantaProperties->preTextEdit->text();
  m_localDirInfo.postText = m_quantaProperties->postTextEdit->text();

  if ( (m_dirInfo.mimeType != m_localDirInfo.mimeType) ||
       (m_dirInfo.preText != m_localDirInfo.preText) ||
       (m_dirInfo.postText != m_localDirInfo.postText))
  {
    m_dirInfo.mimeType = m_localDirInfo.mimeType;
    m_dirInfo.preText = m_localDirInfo.preText;
    m_dirInfo.postText = m_localDirInfo.postText;
    m_dirInfo.usePrePostText = m_localDirInfo.usePrePostText;
    bool result = writeDirInfo();
    KFileTreeViewItem *item = currentKFileTreeViewItem();
    if (item && !item->isDir())
      item = static_cast<KFileTreeViewItem *>(item->parent());
    if (result && item && !typeString.isEmpty())
    {
      if (item->parent() && item->isDir())
          item->setText(1, typeString);
      updateTypeDescription(item, typeString);
    }
  }

  writeTemplateInfo();
}

void TemplatesTreeView::updateTypeDescription(KFileTreeViewItem *item, const QString &typeString)
{
  if (item->parent() && item->isDir())
      item->setText(1, typeString);
  KFileTreeViewItem *curItem = static_cast<KFileTreeViewItem *>(item->firstChild());
  while (curItem && curItem != static_cast<KFileTreeViewItem *>(item->nextSibling()))
  {
    if (!curItem->isDir())
    {
      curItem->setText(1, typeString);
    } else
    {
       QFileInfo dotFileInfo(curItem->url().path() + "/.dirinfo");
       if (!dotFileInfo.exists())
        updateTypeDescription(curItem, typeString);
    }
    curItem = static_cast<KFileTreeViewItem *>(curItem->nextSibling());
  }
}

/** No descriptions */
void TemplatesTreeView::slotInsertTag()
{
 if (currentItem())
 {
  m_dirInfo = readDirInfo();
  KURL url = currentURL();
  emit insertTag( url, m_dirInfo);
 }
}
/*
  Attention, this is called whenever a drop on a kate window happens!
*/
void TemplatesTreeView::slotDragInsert(QDropEvent *e)
{
 if (KURLDrag::canDecode(e))
 {
   KURL::List fileList;
   KURLDrag::decode(e, fileList);

   if(fileList.empty())
    return;

   KURL url = fileList.front();

   QString localFileName;
   if (url.isLocalFile()) 
   {
     localFileName = url.path();
     m_dirInfo = readDirInfo(localFileName);
   }
   QString mimeType = KMimeType::findByURL(url)->name();

   /* First, see if the type of the file is specified in the .dirinfo file */
   if(m_dirInfo.mimeType.isEmpty())
   {
     // no .dirinfo file present, so we insert it as tag
     emit insertTag(url, m_dirInfo);
   } else
   {
     if(m_dirInfo.mimeType == "text/all") // default to inserting in document
     {
      if(!mimeType.contains("text", false) && QuantaCommon::denyBinaryInsert(this) != KMessageBox::Yes)
      {
        return;
      }
      emit insertFile(KURL::fromPathOrURL( localFileName ));
     }

     if(m_dirInfo.mimeType == "file/all")
     {
       // whatever this is, insert it with a tag (image or link or prefix/postfix)
       emit insertTag(KURL::fromPathOrURL( localFileName ), m_dirInfo);
     }
     else
     if(m_dirInfo.mimeType == "template/all")
     {
       if(!mimeType.contains("text", false) && QuantaCommon::denyBinaryInsert(this) != KMessageBox::Yes)
       {
         return;
       }
       emit openFile(KURL());
       emit insertFile(KURL::fromPathOrURL( localFileName ));
     }
   }
 } else
   if (QTextDrag::canDecode(e))
   {
     QString s;
     QTextDrag::decode(e, s);
     KMessageBox::information(this,s, "Decode");
   }
}

void TemplatesTreeView::slotNewProjectLoaded(const QString &projectName, const KURL &baseURL, const KURL &templateURL)
{
  BaseTreeView::slotNewProjectLoaded(projectName, baseURL, templateURL); // set m_projectName and m_projectBaseURL
  if (m_projectDir)
    removeBranch(m_projectDir);
  if (!templateURL.isEmpty())
  {
    newBranch(templateURL);
  }
}


/*!
    \fn TemplatesTreeView::writeTemplateInfo()
 */
void TemplatesTreeView::writeTemplateInfo()
{
  QString fileName = currentURL().path() + TMPL;
  KConfig config(fileName);
  config.setGroup("Filtering");
  if ( m_quantaProperties->actionCombo->currentText() == i18n(NONE) )
    config.writeEntry("Action", NONE);
  else
    config.writeEntry("Action", m_quantaProperties->actionCombo->currentText());
  config.sync();
}

void TemplatesTreeView::slotPaste()
{
  if (currentItem())
  {
    QClipboard *cb = QApplication::clipboard();
    KURL::List list( QStringList::split( QChar('\n'), cb->text() ) );

    KURL url;
    uint j = list.count();
    for (uint i = 0; i < j; i++)
    {
      url = list[i];
      url.setFileName(url.fileName() + TMPL);
      if (url.isLocalFile() && QFileInfo(url.path()).exists())
          list += url;
    }
    url = currentURL();
    if ( ! currentKFileTreeViewItem()->isDir() )
      url.setFileName("");   // don't paste on files but in dirs
    KIO::Job *job = KIO::copy( list, url);
    connect( job, SIGNAL( result( KIO::Job *) ), this , SLOT( slotJobFinished( KIO::Job *) ) );
  }
}

void TemplatesTreeView::slotDelete()
{
  if (currentItem())
  {
    KURL url = currentURL();
    QString msg;
    if ( currentKFileTreeViewItem()->isDir() )
      msg = i18n("Do you really want to delete folder \n%1 ?\n").arg(url.path());
    else
      msg = i18n("Do you really want to delete file \n%1 ?\n").arg(url.path());

    if ( KMessageBox::warningContinueCancel(this, msg, QString::null, KStdGuiItem::del()) == KMessageBox::Continue )
    {
      KIO::Job *job = KIO::del(url);
      connect( job, SIGNAL( result( KIO::Job *) ), this , SLOT( slotJobFinished( KIO::Job *) ) );
      url.setFileName(url.fileName()+ TMPL);
      if ( QFileInfo(url.path()).exists() ) {
        KIO::Job *job2 = KIO::del(url);
        connect( job2, SIGNAL( result( KIO::Job *) ), this , SLOT( slotJobFinished( KIO::Job *) ) );
      };
    }
  }
}

/** Filters the template through and action, and returns the modified/filtered
template file */
KURL TemplatesTreeView::filterTemplate()
{
 KURL url = currentURL();
 QString name = url.path() + TMPL;
 KConfig config(name);
 config.setGroup("Filtering");
 name = config.readEntry("Action", NONE);
 TagAction *filterAction = 0L;
 KActionCollection *ac = m_mainWindow->actionCollection();
 uint acCount = ac->count();
 QString tmpStr;
 for (uint i = 0; i < acCount; i++)
 {
   TagAction *action = dynamic_cast<TagAction*>(ac->action(i));
   if (action)
   {
     QDomElement el = action->data();
     QString type = el.attribute("type", "tag");
     tmpStr = action->text();
     tmpStr.replace(QRegExp("\\&(?!\\&)"),"");
     if (type == "script" && tmpStr == name)
     {
       filterAction = action;
     }
    }
 }
 if (filterAction)
 {
   KTempFile* tempFile = new KTempFile(tmpDir);
   filterAction->setOutputFile(tempFile->file());
   filterAction->setInputFileName(url.path());
   filterAction->execute(true);
   filterAction->setOutputFile(0L);
   filterAction->setInputFileName(QString::null);
   tempFile->close();
   tempFileList.append(tempFile);
   url.setPath(tempFile->name());
 }
 return url;
}

QString TemplatesTreeView::createTemplateTarball()
{
  KURL url = currentURL();
  KURL dirURL (url);
  if (!currentKFileTreeViewItem()->isDir())
    dirURL.setPath(dirURL.directory(false));

  KTempDir* tempDir = new KTempDir(tmpDir);
  tempDir->setAutoDelete(true);
  tempDirList.append(tempDir);
  QString tempFileName=tempDir->name() + url.fileName() + ".tgz";
  //pack the files into a .tgz file
  KTar tar(tempFileName, "application/x-gzip");
  tar.open(IO_WriteOnly);
//    tar.setOrigFileName("");

  KURL::List files;
  if ( ! currentKFileTreeViewItem()->isDir() )
    files.append(url);
  else {
    files = QExtFileInfo::allFiles(dirURL, "*", this) ;
    dirURL = dirURL.upURL();
  }

  for ( KURL::List::Iterator it_f = files.begin(); it_f != files.end(); ++it_f )
  {
    if (!(*it_f).fileName(false).isEmpty()) {
      url = QExtFileInfo::toRelative( (*it_f), dirURL) ;

      QFile file((*it_f).path());
      file.open(IO_ReadOnly);
      QByteArray bArray = file.readAll();
      tar.writeFile(url.path(), "user", "group", bArray.size(), bArray.data());
      file.close();
    };
  }
  tar.close();

  return tempFileName;
}

void TemplatesTreeView::slotSendInMail()
{
  if ( ! currentKFileTreeViewItem() ) return;


    QStringList attachmentFile;
    attachmentFile += createTemplateTarball();

    TagMailDlg *mailDlg = new TagMailDlg( this, i18n("Send template in email"));
    QString toStr;
    QString message = i18n("Hi,\n This is a Quanta Plus [http://quanta.kdewebdev.org] template tarball.\n\nHave fun.\n");
    QString titleStr;
    QString subjectStr;

    mailDlg->TitleLabel->setText(i18n("Content:"));
/*    mailDlg->titleEdit->setFixedHeight(60);
    mailDlg->titleEdit->setVScrollBarMode(QTextEdit::Auto);
    mailDlg->titleEdit->setHScrollBarMode(QTextEdit::Auto);*/
    if ( mailDlg->exec() )
    {
      if ( !mailDlg->lineEmail->text().isEmpty())
      {
        toStr = mailDlg->lineEmail->text();
        subjectStr = (mailDlg->lineSubject->text().isEmpty())?i18n("Quanta Plus Template"):mailDlg->lineSubject->text();
        if ( !mailDlg->titleEdit->text().isEmpty())
            message = mailDlg->titleEdit->text();
      } else
      {
        KMessageBox::error(this,i18n("No destination address was specified.\n Sending is aborted."),i18n("Error Sending Email"));
        delete mailDlg;
        return;
      }
      kapp->invokeMailer(toStr, QString::null, QString::null, subjectStr, message, QString::null, attachmentFile);
    }
    delete mailDlg;
}

void TemplatesTreeView::slotUploadTemplate()
{
  if ( ! currentKFileTreeViewItem() ) return;
  emit uploadTemplate(createTemplateTarball());
}

bool TemplatesTreeView::acceptDrag(QDropEvent* e ) const
{
 return (BaseTreeView::acceptDrag(e) || QTextDrag::canDecode(e));
}

void TemplatesTreeView::slotExtractSiteTemplate()
{
   QString startDir = m_projectBaseURL.url();
   if (startDir.isEmpty())
     startDir = QDir::homeDirPath();
   bool error = false;
   KURLRequesterDlg urlRequester(startDir, i18n("Target folder"), this, "req", true);
   urlRequester.urlRequester()->setMode(KFile::Directory);
   if (urlRequester.exec())
   {
      KURL targetURL = urlRequester.selectedURL();
      KURL url = currentURL();
      QString tempFile;
      if (KIO::NetAccess::download(url, tempFile, this))
      {
          KTar tar(tempFile);
          if (tar.open(IO_ReadOnly))
          {
            const KArchiveDirectory *directory = tar.directory();
            if (targetURL.protocol() == "file")
              directory->copyTo(targetURL.path(), true);
            else
            {
                KTempDir* tempDir = new KTempDir(tmpDir);
                tempDir->setAutoDelete(true);
                QString tempDirName = tempDir->name();
                directory->copyTo(tempDirName, true);
                QStringList entries = directory->entries();
                KURL::List fileList;
                for (QStringList::Iterator it = entries.begin(); it != entries.end(); ++it)
                  fileList.append(KURL::fromPathOrURL(tempDirName + "/" + *it));
                if (!KIO::NetAccess::dircopy(fileList, targetURL, this))
                    error = true;
                KIO::NetAccess::del(KURL().fromPathOrURL(tempDirName), this);
                delete tempDir;
            }
            tar.close();
          } else
             error = true;
          KIO::NetAccess::removeTempFile(tempFile);
          if (!m_projectBaseURL.isEmpty() && !QExtFileInfo::toRelative(targetURL, m_projectBaseURL).url().startsWith("."))
          {
             if (KMessageBox::questionYesNo(this, i18n("You have extracted the site template to a folder which is not under your main project folder.\nDo you want to copy the folder into the main project folder?"), QString::null, i18n("Copy Folder"), i18n("Do Not Copy")) == KMessageBox::Yes)
             {
                 emit insertDirInProject(targetURL);
             }
          }
      } else
         error = true;
      if (error)
        KMessageBox::error(this, i18n("<qt>Some error happened while extracting the <i>%1</i> site template file.<br>Check that you have write permission for <i>%2</i> and that there is enough free space in your temporary folder.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)).arg(targetURL.prettyURL(0, KURL::StripFileProtocol)));
   }
}

#include "templatestreeview.moc"
