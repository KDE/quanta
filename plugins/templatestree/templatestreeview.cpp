/***************************************************************************
    begin                : Thu Dec 20 2001
    copyright            : (C) 2001-2004 by Andras Mantia <amantia@kde.org>
                           (C) 2005         Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/


//FIXME: Get rid of Q3TextDrag and K3URLDrag

#include "templatestreeview.h"
#include "templatestreepart.h"
#include "newtemplatedirdlg.h"
#include "extfileinfo.h"
#include "quantanetaccess.h"
#include "ui_quantapropertiespage.h"
#include "quantacoreif.h"
// #include "tagaction.h"
// #include "tagmaildlg.h"
#include "helper.h"
#include "settings.h"

//kdevelop includes
#include <core.h>
#include <iplugin.h>
#include <iproject.h>
#include <iprojectcontroller.h>

#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <sys/types.h>

// QT includes
#include <QButtonGroup>
#include <QCheckBox>
#include <QClipboard>
#include <QDir>
#include <QPixmap>
#include <q3header.h>
#include <qpoint.h>
#include <QLayout>
#include <QTextEdit>
#include <QRegExp>
#include <QLabel>
#include <qmap.h>
#include <qstackedwidget.h>
#include <QTextStream>
#include <Q3TextDrag>

// KDE includes
#include <kapplication.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kstandarddirs.h>
#include <kxmlguiwindow.h>
#include <kmimetype.h>
#include <kmessagebox.h>
#include <kcombobox.h>
#include <kmenu.h>
#include <kpropertiesdialog.h>
#include <kurlrequester.h>
#include <kurlrequesterdialog.h>
#include <ktempdir.h>
#include <ktemporaryfile.h>
#include <kio/netaccess.h>
#include <kio/copyjob.h>
#include <kio/deletejob.h>
#include <ktar.h>
#include <kdebug.h>
#include <kfile.h>
#include <kactioncollection.h>
#include <k3urldrag.h>
#include <kconfiggroup.h>



#define EXCLUDE ".*\\.tmpl$"
#define TMPL ".tmpl"
#define NONE "None"


QMap<QString, QString> typeToi18n;
QMap<QString, QString> i18nToType;


//TemplatesTreeBranch implementation
TemplatesTreeBranch::TemplatesTreeBranch(BaseTreeView *parent, const KUrl& url,
                                         const QString& name, const QPixmap& pix,
                                         bool showHidden,
                                         K3FileTreeViewItem *branchRoot)
    : BaseTreeBranch(parent, url, name, pix, showHidden, branchRoot)
{
}

K3FileTreeViewItem* TemplatesTreeBranch::createTreeViewItem(K3FileTreeViewItem *parent,
                                                            const KFileItem &fileItem )
{
  BaseTreeViewItem  *tvi = 0;
  if( parent && !fileItem.isNull() )
  {
    KUrl url = fileItem.url();
    tvi = new BaseTreeViewItem( parent, fileItem, this );
    if (tvi && fileItem.isDir())
    {
      if (url.isLocalFile())
      {
        QDir dir (url.path(), "", QDir::Name | QDir::IgnoreCase, QDir::AllEntries);
        tvi->setExpandable(dir.count() != 2);     //   . and .. are always there
      } else {
        tvi->setExpandable(true);     //   we assume there is something
      }
    } else
    {
      url = static_cast<BaseTreeViewItem*>(parent)->url();
    }
    QFileInfo dotFileInfo(url.path() + "/.dirinfo");
    while ( !dotFileInfo.exists() )
    {
      QFileInfo parent(dotFileInfo.path());
      if (parent.isRoot())
        break;
      dotFileInfo.setFile(parent.path() + "/.dirinfo");
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
    kDebug(24000) << "TemplatesTreeBranch::createTreeViewItem: Have no parent";
  return tvi;
}



TemplatesTreeView::TemplatesTreeView(TemplatesTreePart *plugin)
  : BaseTreeView(plugin, 0), m_projectDir(0), m_part(plugin)
{
  typeToi18n["text/all"] = i18n("Text Snippet");
  typeToi18n["file/all"] = i18n("Binary File");
  typeToi18n["template/all"] = i18n("Document Template");
  typeToi18n["site/all"] = i18n("Site Template");
  i18nToType[i18n("Text Snippet")] = "text/all";
  i18nToType[i18n("Binary File")] = "file/all";
  i18nToType[i18n("Document Template")] = "template/all";
  i18nToType[i18n("Site Template")] = "site/all";

  addColumn(i18n("Templates"), -1);
  addColumn(i18n("Group"), -1);

  globalURL.setPath(Helper::dataDir() + Helper::resourceDir() + "templates/");
  newBranch(globalURL);

  localURL.setPath(KStandardDirs::locateLocal("data", Helper::resourceDir() + "templates/"));
  newBranch(localURL);

  connect(this, SIGNAL(contextMenu(K3ListView*, Q3ListViewItem*, const QPoint&)),
          this, SLOT(slotMenu(K3ListView*, Q3ListViewItem*, const QPoint&)));

  connect(this, SIGNAL(open(Q3ListViewItem *)),
          this,  SLOT(slotSelectFile(Q3ListViewItem *)));

  setAcceptDrops(true);
  setSelectionMode(Q3ListView::Single);
  setDragEnabled(true);
  setShowToolTips(Settings::self()->templatesTreeTooltips());
  setSaveOpenFolder(Settings::self()->templatesTreeSave());

  KConfigGroup configGroup(KGlobal::config(), metaObject()->className());
  restoreLayout(configGroup);
  // the restored size of the first column might be too large for the current content
  // we set it to 10 and the listview will adjust it to the size of the largest entry
  setColumnWidth(0, 10);
}

TemplatesTreeView::~TemplatesTreeView()
{
  qDeleteAll(tempFileList.begin(), tempFileList.end());
  qDeleteAll(tempDirList.begin(), tempDirList.end());
  KConfigGroup configGroup(KGlobal::config(), metaObject()->className());
  saveLayout(configGroup);
}


KFileTreeBranch* TemplatesTreeView::newBranch(const KUrl& url)
{
  BaseTreeBranch *newBrnch;
  if (url == globalURL)
  {
    //FIXME: how to get the icon?
  //  newBrnch = new TemplatesTreeBranch(this, url, i18n("Global"), SmallIcon(m_part->info()->icon()));
    newBrnch = new TemplatesTreeBranch(this, url, i18n("Global"), SmallIcon(""));
  } else
  {
    //FIXME: how to get the icon?
    if (url == localURL)
//      newBrnch = new TemplatesTreeBranch(this, url, i18n("Local"), SmallIcon(m_part->info()->icon()));
      newBrnch = new TemplatesTreeBranch(this, url, i18n("Local"), SmallIcon(""));
    else
    {
      newBrnch = new TemplatesTreeBranch(this, url, i18n("Project"), SmallIcon("folder_green"));
      m_projectDir = newBrnch;
    }
  }
  newBrnch->excludeFilterRx.setPattern(EXCLUDE);
  if (url.isLocalFile())
    newBrnch->root()->setText(1, url.path());
  else
    newBrnch->root()->setText(1, url.prettyUrl());
  addBranch(newBrnch);
  if (url.isLocalFile())
  {
    QDir dir (url.path(), "", QDir::Name | QDir::IgnoreCase, QDir::AllEntries);
    newBrnch->root()->setExpandable(dir.count() != 2);     //   . and .. are always there
  } else {
    newBrnch->root()->setExpandable(true);     //   we assume there is something
  }
  return newBrnch;
}


/** No descriptions */
void TemplatesTreeView::slotInsertInDocument()
{
  KUrl url = filterTemplate();
  if (KMimeType::findByUrl(url)->is("text") || (Helper::denyBinaryInsert() == KMessageBox::Yes))
  {
    emit insertFile(url);
  }
}

void TemplatesTreeView::emptyMenu(const QPoint &point)
{
  KMenu popup(this);

  popup.addAction(SmallIcon("network-wired"), i18n("&Download Template..."), this, SIGNAL(downloadTemplate()));
  popup.exec(point);
}


void TemplatesTreeView::folderMenu(const QPoint &point)
{
  KMenu popup(this);
  popup.addTitle(i18n("Templates Tree"));

  popup.addAction(SmallIcon("folder-new"), i18n("&New Folder..."), this, SLOT(slotNewDir()));
  popup.addAction(SmallIcon("mail-send"), i18n("Send in E&mail..."), this, SLOT(slotSendInMail()));
  popup.addAction(SmallIcon("network-wired"), i18n("&Upload Template..."), this, SLOT(slotUploadTemplate()));

  bool rootItem = (currentKFileTreeViewItem() == currentKFileTreeViewItem()->branch()->root());
  if (rootItem)
    popup.addAction(SmallIcon("network-wired"), i18n("&Download Template..."), this, SIGNAL(downloadTemplate()));

  bool hasProject = !m_projectName.isEmpty();
  if (hasProject)
    popup.addAction(i18n("&Insert in Project..."), this, SLOT(slotInsertDirInProject()));

  popup.addSeparator();
  popup.addAction(SmallIcon("edit-copy"), i18n("&Copy"), this, SLOT(slotCopy()));
  if (isPathInClipboard())
    popup.addAction(SmallIcon("edit-paste"), i18n("&Paste"), this, SLOT(slotPaste()));

  if (!rootItem)
    popup.addAction(SmallIcon("edit-delete"), i18n("&Delete"), this, SLOT(slotDelete()));

  popup.addSeparator();
  popup.addAction(SmallIcon("document-properties"), i18n("&Properties"), this, SLOT(slotProperties()));
  if (rootItem)
    popup.addAction(SmallIcon("file-revert"), i18n("&Reload"), this, SLOT(slotReload()));

  // ask other plugins for menu entries
  //FIXME: context menu handling was changed!
  /*
  KUrl menuURL(currentKFileTreeViewItem()->url());
  menuURL.adjustPath(KUrl::AddTrailingSlash);
  KUrl::List urlList(menuURL);
  Koncrete::FileContext context(urlList);
  Koncrete::Core::mainWindow()->fillContextMenu(&popup, &context);
*/
  popup.exec(point);
}


void TemplatesTreeView::fileMenu(const QPoint &point)
{
  KMenu popup(this);
  popup.addTitle(i18n("Templates Tree"));

  m_dirInfo = readDirInfo();
  if (m_part->quantaCore()->activeEditorSource())
  {
    if (m_dirInfo.mimeType.contains("TEXT", Qt::CaseInsensitive))
      popup.addAction(i18n("Insert as &Text"), this, SLOT(slotInsertInDocument()));
/*  this is already on quanta core
    else
      if (m_dirInfo.mimeType.toUpper().contains("FILE"))
        popup.insertItem(i18n("Insert &Link to File"), this, SLOT(slotInsertTag()));*/
  }
  if (m_dirInfo.mimeType.contains("TEMPLATE", Qt::CaseInsensitive))
    popup.addAction(i18n("&New Document Based on This"), this, SLOT(slotNewDocument()));
  else
    if (m_dirInfo.mimeType.contains("SITE", Qt::CaseInsensitive))
      popup.addAction(i18n("&Extract Site Template To..."), this, SLOT(slotExtractSiteTemplate()));

  popup.addAction(SmallIcon("document-open"), i18n("&Open"), this ,SLOT(slotOpen()));
  popup.addAction(SmallIcon("mail-send"), i18n("Send in E&mail..."), this, SLOT(slotSendInMail()));
  popup.addAction(SmallIcon("network-wired"), i18n("&Upload Template..."), this, SLOT(slotUploadTemplate()));

  bool hasProject = !m_projectName.isEmpty();
  if (hasProject)
    popup.addAction(i18n("&Insert in Project..."), this, SLOT(slotInsertInProject()));

  if (isFileOpen(currentUrl()))
    popup.addAction(SmallIcon("window-close"), i18n("Clos&e"), this, SLOT(slotClose()));

  popup.addSeparator();
  popup.addAction(SmallIcon("edit-copy"), i18n("&Copy"), this, SLOT(slotCopy()));
  popup.addAction(SmallIcon("edit-delete"), i18n("&Delete"), this, SLOT(slotDelete()));
  popup.addSeparator();
  popup.addAction(SmallIcon("document-properties"), i18n("&Properties"), this, SLOT(slotProperties()));


  // ask other plugins for menu entries
//FIXME: context menu handling was changed!
  /*  
  KUrl::List urlList(currentKFileTreeViewItem()->url());
  Koncrete::FileContext context(urlList);
  Koncrete::Core::mainWindow()->fillContextMenu(&popup, &context);
*/
  popup.exec(point);
}


void TemplatesTreeView::slotNewDocument()
{
//  FIXME KUrl url = filterTemplate();
//  if (QuantaCommon::checkMimeGroup(url, "text") || QuantaCommon::denyBinaryInsert() == KMessageBox::Yes)
//  {
//    Q3ListViewItem *item = currentItem();
//    if (item )
//    {
//      if ( currentKFileTreeViewItem() != currentKFileTreeViewItem()->branch()->root())
//      {
//        emit openFile(KUrl());
//        emit insertFile(url);
//      }
//    }
//  }
}


void TemplatesTreeView::slotSelectFile(Q3ListViewItem *item)
{
  if ( !item )
    return;

  K3FileTreeViewItem *kftvItem = currentKFileTreeViewItem();
  if ( !kftvItem || kftvItem->isDir())
    return;

  m_dirInfo = readDirInfo();
/*    if (m_dirInfo.mimeType.contains("SITE", Qt::CaseInsensitive))
    {
      slotExtractSiteTemplate();
      return;
    }*/
  if (expandArchiv(kftvItem))
    return;

  if (m_dirInfo.mimeType.contains("TEXT", Qt::CaseInsensitive))
    slotInsertInDocument();
  else
    if (m_dirInfo.mimeType.contains("FILE", Qt::CaseInsensitive))
      slotInsertTag();
  else
    if (m_dirInfo.mimeType.contains("TEMPLATE", Qt::CaseInsensitive))
      slotNewDocument();
}

void TemplatesTreeView::slotOpen()
{
  BaseTreeView::slotSelectFile(currentItem());
}

/** No descriptions */
void TemplatesTreeView::slotNewDir()
{
  NewTemplateDirDlg *createDirDlg = new NewTemplateDirDlg(this);
  createDirDlg->setWindowTitle(i18n("Create New Template Folder"));
  createDirDlg->typesCombo->addItem(typeToi18n["text/all"]);
  createDirDlg->typesCombo->addItem(typeToi18n["file/all"]);
  createDirDlg->typesCombo->addItem(typeToi18n["template/all"]);
  createDirDlg->typesCombo->addItem(typeToi18n["site/all"]);

  m_dirInfo = readDirInfo();

   if (m_dirInfo.mimeType.isEmpty())
   {
    createDirDlg->parentAttr->setText(i18n("&Inherit parent attribute (nothing)"));
   } else
   {
     createDirDlg->parentAttr->setText(i18n("&Inherit parent attribute (%1)", typeToi18n[m_dirInfo.mimeType]));
   }
   if (createDirDlg->exec())
   {
    QDir dir;

   QString startDir = "";
   if ( !currentKFileTreeViewItem()->isDir() )
   {
    startDir = currentUrl().path();
   } else
   {
    startDir = currentUrl().path() + "/dummy_file";
   }
   startDir = QFileInfo(startDir).path();
   if (!dir.mkdir(startDir + '/' + createDirDlg->dirName->text()))
   {
      KMessageBox::error(this,i18n("Error while creating the new folder.\n \
                   Maybe you do not have permission to write in the %1 folder.", startDir));
      return;
   }
   if (! createDirDlg->parentAttr->isChecked())
   {
      m_dirInfo.mimeType = i18nToType[createDirDlg->typesCombo->currentText()];
      m_dirInfo.preText = "";
      m_dirInfo.postText = "";
      writeDirInfo(startDir + '/' + createDirDlg->dirName->text() + "/.dirinfo");
   }
  }
}
/** No descriptions */

Q3DragObject * TemplatesTreeView::dragObject ()
{
  // don't drag folders
  if ( ! currentKFileTreeViewItem() ||  currentKFileTreeViewItem()->isDir() )
    return 0;

  m_dirInfo = readDirInfo();
  if(!m_dirInfo.mimeType.isEmpty()) // only drag when the template type is specified
  {
    K3URLDrag *drag = new K3URLDrag(KUrl::List(currentUrl()), this);
    return drag;
  }
  return 0;
}

/** No descriptions */
void TemplatesTreeView::contentsDropEvent(QDropEvent *e)
{
  if (K3URLDrag::canDecode(e))
  {
    // handles url drops
    BaseTreeView::contentsDropEvent(e);
    return;
  }
  if (Q3TextDrag::canDecode(e))
  {
    Q3ListViewItem *item = itemAt(contentsToViewport(e->pos()));
    if (item)
    {
      KUrl dest;
      if ( currentKFileTreeViewItem()->isDir() )
        dest = currentUrl();
      else
        dest = currentUrl().directory(KUrl::AppendTrailingSlash);
      dest.adjustPath(KUrl::AddTrailingSlash);
      QString content;
      Q3TextDrag::decode(e, content);
      KUrl url =KUrlRequesterDialog::getUrl( dest.path() + "template.txt",
                                          this, i18n("Save selection as template file:"));
      if ( !url.isEmpty() )
      {
        //now save the file
        KTemporaryFile* tempFile = new KTemporaryFile();
        tempFile->setPrefix(Helper::tmpFilePrefix());
        tempFile->open();
        QTextStream stream(tempFile);
        stream.setCodec(QTextCodec::codecForName("UTF-8"));
        stream << content;
        stream.flush();
        bool proceed = true;
        if (ExtFileInfo::exists(url))
        {
          proceed = KMessageBox::warningYesNo(this, i18n("<qt>The file <b>%1</b> already exists.<br />Do you want to overwrite it?</qt>", url.pathOrUrl()),i18n("Overwrite")) == KMessageBox::Yes;
        }
        if (proceed)
        {
          if (!QuantaNetAccess::upload(tempFile->fileName(), url, m_plugin, false))
          {
            KMessageBox::error(this,i18n("<qt>Could not write to file <b>%1</b>.<br />Check if you have rights to write there or that your connection is working.</qt>", url.pathOrUrl()));
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
Helper::DirInfo TemplatesTreeView::readDirInfo(const QString& dir)
{
  Helper::DirInfo dirInfo;
  QString startDir = dir;

  if (startDir.isEmpty())
  {
    if (!currentKFileTreeViewItem()->isDir())
    {
      startDir = currentUrl().path();
    } else
    {
      startDir = currentUrl().path() + "/dummy_file";
    }
  }

  QFileInfo dotFileInfo(QFileInfo(startDir).path() + "/.dirinfo");

  KConfig *config = new KConfig(dotFileInfo.filePath());
  dirInfo.mimeType = config->readEntry("Type");
  dirInfo.preText = config->readEntry("PreText");
  dirInfo.postText = config->readEntry("PostText");
  dirInfo.usePrePostText = config->readEntry("UsePrePostText", false);

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
      startDir = currentUrl().path();
    } else
    {
      startDir = currentUrl().path() + "/dummy_file";
    }
  } else
  {
    startDir = m_dirInfoFile;
  }

  QFileInfo dotFileInfo(QFileInfo(startDir).path()+"/.dirinfo");

  bool success = false;
  KConfig *config = new KConfig(dotFileInfo.filePath());
  config->writeEntry("Type", m_dirInfo.mimeType);
  config->writeEntry("PreText", m_dirInfo.preText);
  config->writeEntry("PostText", m_dirInfo.postText);
  config->writeEntry("UsePrePostText", m_dirInfo.usePrePostText);
  config->sync();
  success = true;
  delete config;
  return success;
}

void TemplatesTreeView::slotProperties()
{
  if ( !currentItem() )
    return;
  KUrl url = currentUrl();

  KPropertiesDialog *propDlg = new KPropertiesDialog( url, this ); //autodeletes itself

//Always add the Quanta directory page
  QFrame *quantaDirPage = new QFrame();
  propDlg->addPage(quantaDirPage, i18n("Quanta Template"));
  QVBoxLayout *topLayout = new QVBoxLayout(quantaDirPage);
  QWidget *w = new QWidget(quantaDirPage);
  m_quantaProperties = new Ui::QuantaPropertiesPage;
  m_quantaProperties->setupUi(w);
  w->setWindowTitle(i18n("Quanta"));

  m_quantaProperties->typesCombo->addItem(typeToi18n["text/all"]);
  m_quantaProperties->typesCombo->addItem(typeToi18n["file/all"]);
  m_quantaProperties->typesCombo->addItem(typeToi18n["template/all"]);
  m_quantaProperties->typesCombo->addItem(typeToi18n["site/all"]);

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
    m_quantaProperties->typeStack->setCurrentIndex(1);
  } else
  {
    startDir = url.path() + "/dummy_file";
    m_quantaProperties->typeStack->setCurrentIndex(0);
  }
  w->setEnabled(writable);
  QFileInfo dotFileInfo(QFileInfo(startDir).path() + "/.dirinfo");
  m_parentDirInfo = readDirInfo(dotFileInfo.path());
  if (!dotFileInfo.exists() || m_dirInfo.mimeType == m_parentDirInfo.mimeType)
  {
     m_quantaProperties->parentAttr->setChecked(true);
  }
  if (m_parentDirInfo.mimeType.isEmpty())
   {
    m_quantaProperties->parentAttr->setText(i18n("&Inherit parent attribute (nothing)"));
   } else
   {
    m_quantaProperties->parentAttr->setText(i18n("&Inherit parent attribute (%1)", typeToi18n[m_parentDirInfo.mimeType]));
   }
   m_quantaProperties->preTextEdit->setText(m_dirInfo.preText);
   m_quantaProperties->postTextEdit->setText(m_dirInfo.postText);
   if (m_dirInfo.usePrePostText)
   {
    m_quantaProperties->usePrePostText->setChecked(true);
   }

   topLayout->addWidget(w);
   connect( propDlg, SIGNAL( applied() ), this , SLOT( slotPropertiesApplied()) );

  QString name = url.path() + TMPL;
  KConfig config(name);
  config.setGroup("Filtering");
  name = config.readEntry("Action", NONE);
  if ( name == NONE )
     name = i18n(NONE);
  uint pos = 0;
//   uint j = 1;
  m_quantaProperties->actionCombo->addItem(i18n(NONE));
  QString tmpStr;
/*  KActionCollection *ac = m_mainWindow->actionCollection();
  uint acCount = ac->count();*/
/*FIXME  for (uint i = 0; i < acCount; i++)
  {
    TagAction *action = dynamic_cast<TagAction*>(ac->action(i));
    if (action)
    {
      QDomElement el = action->data();
      QString type = el.attribute("type", "tag");
      if (type == "script")
      {
        tmpStr = action->text().replace(QRegExp("\\&(?!\\&)"),"");
        m_quantaProperties->actionCombo->addItem(tmpStr);
        if (tmpStr == name)
            pos = j;
        j++;
      }
    }
  }*/
  m_quantaProperties->actionCombo->setCurrentIndex(pos);

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
  Helper::DirInfo m_localDirInfo;
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
    K3FileTreeViewItem *item = currentKFileTreeViewItem();
    if (item && !item->isDir())
      item = static_cast<K3FileTreeViewItem *>(item->parent());
    if (result && item && !typeString.isEmpty())
    {
      if (item->parent() && item->isDir())
          item->setText(1, typeString);
      updateTypeDescription(item, typeString);
    }
  }

  writeTemplateInfo();
}

void TemplatesTreeView::updateTypeDescription(K3FileTreeViewItem *item, const QString &typeString)
{
  if (item->parent() && item->isDir())
      item->setText(1, typeString);
  K3FileTreeViewItem *curItem = static_cast<K3FileTreeViewItem *>(item->firstChild());
  while (curItem && curItem != static_cast<K3FileTreeViewItem *>(item->nextSibling()))
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
    curItem = static_cast<K3FileTreeViewItem *>(curItem->nextSibling());
  }
}

/** No descriptions */
void TemplatesTreeView::slotInsertTag()
{
 if (currentItem())
 {
   m_dirInfo = readDirInfo();
   KUrl url = currentUrl();
   emit insertTag(url, &m_dirInfo);
 }
}
/*
  Attention, this is called whenever a drop on a kate window happens!
*/
void TemplatesTreeView::slotDragInsert(QDropEvent *e)
{
  if (K3URLDrag::canDecode(e))
  {
    KUrl::List fileList;
    K3URLDrag::decode(e, fileList);

    if(fileList.empty())
      return;

    KUrl url = fileList.front();

    QString localFileName;
    if (url.isLocalFile())
    {
      localFileName = url.path();
      m_dirInfo = readDirInfo(localFileName);
    }
    QString mimeType = KMimeType::findByUrl(url)->name();

    /* First, see if the type of the file is specified in the .dirinfo file */
    if(m_dirInfo.mimeType.isEmpty())
    {
      // no .dirinfo file present, so we insert it as tag
      emit insertTag(url, &m_dirInfo);
    } else
    {
      if(m_dirInfo.mimeType == "text/all") // default to inserting in document
      {
        if(!mimeType.contains("text", Qt::CaseInsensitive) && Helper::denyBinaryInsert() != KMessageBox::Yes)
        {
          return;
        }
        emit insertFile(KUrl( localFileName ));
      }

      if(m_dirInfo.mimeType == "file/all")
      {
       // whatever this is, insert it with a tag (image or link or prefix/postfix)
        emit insertTag(KUrl( localFileName ), &m_dirInfo);
      }
      else
      if(m_dirInfo.mimeType == "template/all")
      {
        if(!mimeType.contains("text", Qt::CaseInsensitive) && Helper::denyBinaryInsert() != KMessageBox::Yes)
        {
          return;
        }
  //        emit openFile(KUrl());  FIXME
        emit insertFile(KUrl( localFileName ));
      }
    }
  } else
    if (Q3TextDrag::canDecode(e))
    {
      QString s;
      Q3TextDrag::decode(e, s);
      KMessageBox::information(this,s, "Decode");
    }
}


void TemplatesTreeView::slotProjectOpened()
{
  //FIXME: no active project! Possibly hook to the signal which is emmitted when a project is opened
  m_projectName = KDevelop::Core::self()->projectController()->projectAt(0)->name();
  m_projectBaseURL = KDevelop::Core::self()->projectController()->projectAt(0)->folder();

  if (m_projectDir)
    removeBranch(m_projectDir);

  KUrl templateURL; // FIXME get the url
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
  QString fileName = currentUrl().path() + TMPL;
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
    KUrl::List list( cb->text().split(QChar('\n')) );

    KUrl url;
    uint j = list.count();
    for (uint i = 0; i < j; i++)
    {
      url = list[i];
      url.setFileName(url.fileName() + TMPL);
      if (url.isLocalFile() && QFileInfo(url.path()).exists())
          list += url;
    }
    url = currentUrl();
    if ( ! currentKFileTreeViewItem()->isDir() )
      url.setFileName("");   // don't paste on files but in dirs
    KIO::Job *job = KIO::copy( list, url);
    connect( job, SIGNAL( result( KJob *) ), this , SLOT( slotJobFinished( KJob *) ) );
  }
}

void TemplatesTreeView::slotDelete()
{
  if (currentItem())
  {
    KUrl url = currentUrl();
    QString msg;
    if ( currentKFileTreeViewItem()->isDir() )
      msg = i18n("Do you really want to delete folder \n%1 ?\n", url.path());
    else
      msg = i18n("Do you really want to delete file \n%1 ?\n", url.path());

    if ( KMessageBox::warningContinueCancel(this, msg, QString(), KStandardGuiItem::del()) == KMessageBox::Continue )
    {
      KIO::Job *job = KIO::del(url);
      connect( job, SIGNAL( result( KJob *) ), this , SLOT( slotJobFinished( KJob *) ) );
      url.setFileName(url.fileName()+ TMPL);
      if ( QFileInfo(url.path()).exists() ) {
        KIO::Job *job2 = KIO::del(url);
        connect( job2, SIGNAL( result( KJob *) ), this , SLOT( slotJobFinished( KJob *) ) );
      };
    }
  }
}

/** Filters the template through and action, and returns the modified/filtered
template file */
KUrl TemplatesTreeView::filterTemplate()
{
  KUrl url = currentUrl();
//   QString name = url.path() + TMPL;
//   KConfig config(name);
//   config.setGroup("Filtering");
//   name = config.readEntry("Action", NONE);
//   TagAction *filterAction = 0L;
//   KActionCollection *ac = m_mainWindow->actionCollection();
//   uint acCount = ac->count();
//   QString tmpStr;
/*  for (uint i = 0; i < acCount; i++)
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
    KTempFile* tempFile = new KTempFile(Helper::tmpFilePrefix());
    filterAction->setOutputFile(tempFile->file());
    filterAction->setInputFileName(url.path());
    filterAction->execute(true);
    filterAction->setOutputFile(0L);
    filterAction->setInputFileName(QString());
    tempFile->close();
    tempFileList.append(tempFile);
    url.setPath(tempFile->name());
  }*/
  return url;
}

QString TemplatesTreeView::createTemplateTarball()
{
  KUrl url = currentUrl();
  KUrl dirURL (url);
  if (!currentKFileTreeViewItem()->isDir())
    dirURL.setPath(dirURL.directory(KUrl::AppendTrailingSlash));

  KTempDir* tempDir = new KTempDir(Helper::tmpFilePrefix());
  tempDirList.append(tempDir);
  QString tempFileName = tempDir->name() + url.fileName() + ".tgz";
  //pack the files into a .tgz file
  KTar tar(tempFileName, "application/x-gzip");
  tar.open(IO_WriteOnly);
//    tar.setOrigFileName("");

//  FIXME directly insert the files with relative path

  KUrl::List files;
  if ( ! currentKFileTreeViewItem()->isDir() )
    files.append(url);
  else {
    files = ExtFileInfo::allFiles(dirURL, "*") ;
    dirURL = dirURL.upUrl();
  }

  for ( KUrl::List::Iterator it_f = files.begin(); it_f != files.end(); ++it_f )
  {
    if (!(*it_f).fileName(false).isEmpty()) {
      url = KUrl::relativeUrl(dirURL, *it_f) ;

      QFile file((*it_f).path());
      file.open(IO_ReadOnly);
      QByteArray bArray = file.readAll();
      tar.writeFile(url.path(), "user", "group", bArray.data(), bArray.size());
      file.close();
    };
  }
  tar.close();

  return tempFileName;
}

void TemplatesTreeView::slotSendInMail()
{
  if ( ! currentKFileTreeViewItem() ) return;

/*
    QStringList attachmentFile;
    attachmentFile += createTemplateTarball();

    TagMailDlg *mailDlg = new TagMailDlg( this, i18n("Send template in email"));
    QString toStr;
    QString message = i18n("Hi,\n This is a Quanta Plus [http://quanta.kdewebdev.org/] template tarball.\n\nHave fun.\n");
    QString titleStr;
    QString subjectStr;

    mailDlg->TitleLabel->setText(i18n("Content:"));
    mailDlg->titleEdit->setFixedHeight(60);
    mailDlg->titleEdit->setVScrollBarMode(QTextEdit::Auto);
    mailDlg->titleEdit->setHScrollBarMode(QTextEdit::Auto);
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
      kapp->invokeMailer(toStr, QString(), QString(), subjectStr, message, QString(), attachmentFile);
    }
    delete mailDlg;*/
}

void TemplatesTreeView::slotUploadTemplate()
{
  if ( ! currentKFileTreeViewItem() ) return;
  emit uploadTemplate(createTemplateTarball());
}

bool TemplatesTreeView::acceptDrag(QDropEvent* e ) const
{
 return (BaseTreeView::acceptDrag(e) || Q3TextDrag::canDecode(e));
}

void TemplatesTreeView::slotExtractSiteTemplate()
{
   QString startDir = m_projectBaseURL.url();
   if (startDir.isEmpty())
     startDir = QDir::homePath();
   bool error = false;
   KUrlRequesterDialog urlRequester(startDir, i18n("Target folder"), this);
   urlRequester.urlRequester()->setMode(KFile::Directory);
   if (urlRequester.exec())
   {
      KUrl targetUrl = urlRequester.selectedUrl();
      KUrl url = currentUrl();
      QString tempFile;
      if (KIO::NetAccess::download(url, tempFile, this))
      {
          KTar tar(tempFile);
          if (tar.open(IO_ReadOnly))
          {
            const KArchiveDirectory *directory = tar.directory();
            if (targetUrl.protocol() == "file")
              directory->copyTo(targetUrl.path(), true);
            else
            {
                KTempDir* tempDir = new KTempDir(Helper::tmpFilePrefix());
                QString tempDirName = tempDir->name();
                directory->copyTo(tempDirName, true);
                QStringList entries = directory->entries();
                KUrl::List fileList;
                for (QStringList::Iterator it = entries.begin(); it != entries.end(); ++it)
                  fileList.append(KUrl(tempDirName + '/' + *it));
                if (!KIO::NetAccess::dircopy(fileList, targetUrl, this))
                    error = true;
                KIO::NetAccess::del(KUrl(tempDirName), this);
                delete tempDir;
            }
            tar.close();
          } else
             error = true;
          KIO::NetAccess::removeTempFile(tempFile);
          if (!m_projectBaseURL.isEmpty() && !KUrl::relativeUrl(m_projectBaseURL, targetUrl).startsWith('.'))
          {
             if (KMessageBox::questionYesNo(this, i18n("You have extracted the site template to a folder which is not under your main project folder.\nDo you want to copy the folder into the main project folder?")) == KMessageBox::Yes)
             {
// FIXME                 emit insertDirInProject(targetURL);
             }
          }
      } else
         error = true;
      if (error)
        KMessageBox::error(this, i18n("<qt>Some error happened while extracting the <i>%1</i> site template file.<br />Check that you have write permission for <i>%2</i> and that there is enough free space in your temporary folder.</qt>", url.pathOrUrl(), targetUrl.pathOrUrl()));
   }
}


void TemplatesTreeView::slotSettingsChanged()
{
  setShowToolTips(Settings::self()->templatesTreeTooltips());
  setSaveOpenFolder(Settings::self()->templatesTreeSave());
}

#include "templatestreeview.moc"
