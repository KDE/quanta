/***************************************************************************
                          templatestreeview.cpp  -  description
                             -------------------
    begin                : Thu Dec 20 2001
    copyright            : (C) 2001-2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

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

// KDE includes
#include <krun.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kopenwith.h>
#include <kstandarddirs.h>
#include <kmimetype.h>
#include <kmessagebox.h>
#include <kcombobox.h>
#include <kpopupmenu.h>
#include <kpropertiesdialog.h>
#include <kurlrequesterdlg.h>
#include <ktempfile.h>
#include <kio/netaccess.h>
#include <kurldrag.h>
#include <ktar.h>
#include <kdebug.h>


#include "templatestreeview.h"
#include "newtemplatedirdlg.h"
#include "quantapropertiespage.h"
#include "tagaction.h"
#include "copyto.h"
#include "resource.h"
#include "qextfileinfo.h"
#include "quanta.h"
#include "tagmaildlg.h"
#include "project.h"

#define EXCLUDE ".*\\.tmpl$"
#define TMPL ".tmpl"
#define NONE "None"


const QString textMenu = I18N_NOOP("Insert as Text");
const QString binaryMenu = I18N_NOOP("Insert Link to File");
const QString docMenu = I18N_NOOP("New Document Based on This");

//TemplatesTreeBranch implementation
TemplatesTreeBranch::TemplatesTreeBranch(KFileTreeView *parent, const KURL& url,
                                         const QString& name, const QPixmap& pix,
                                         bool showHidden,
                                         KFileTreeViewItem *branchRoot)
    : FilesTreeBranch(parent, url, name, pix, showHidden, branchRoot)
{
}

KFileTreeViewItem* TemplatesTreeBranch::createTreeViewItem(KFileTreeViewItem *parent,
                                                           KFileItem *fileItem )
{
  FilesTreeViewItem  *tvi = 0;
  if( parent && fileItem )
  {
    tvi = new FilesTreeViewItem( parent, fileItem, this );
    if (tvi && fileItem->isDir())
    {
      KURL url = fileItem->url();
      if (url.isLocalFile())
      {
        QDir dir (url.path(), "", QDir::All & !QDir::Hidden);
        tvi->setExpandable(dir.count() != 2);     //   . and .. are always there
      } else {
        tvi->setExpandable(true);     //   we assume there is something
      }
      QFileInfo dotFileInfo(fileItem->url().path() + "/.dirinfo");
      if (dotFileInfo.exists())
      {
        KConfig *config = new KConfig(dotFileInfo.filePath());
        tvi->setText(1, config->readEntry("Type"));
        delete config;
      }
    }
  }
  else
    kdDebug(24000) << "TemplatesTreeBranch::createTreeViewItem: Have no parent" << endl;
  return tvi;
}

TemplatesTreeView::TemplatesTreeView(QWidget *parent, const char *name )
  : FilesTreeView(parent,name), m_projectDir(0)
{
  m_fileMenu = new KPopupMenu();

  m_openId = m_fileMenu->insertItem(SmallIcon("fileopen"), i18n("&Open"), this ,SLOT(slotInsert()));
  m_fileMenu->insertItem(i18n("Open for Editing"), this, SLOT(slotOpen()));
  m_fileMenu->insertItem(SmallIcon("mail_send"), i18n("Send in E-Mail"), this, SLOT(slotSendInMail()));
  m_insertFileInProject = m_fileMenu->insertItem(i18n("Insert in Project..."), this, SLOT(slotInsertInProject()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));
  m_fileMenu->insertItem(SmallIcon("editpaste"), i18n("&Paste"), this, SLOT(slotPaste()));
  m_fileMenu->insertItem(SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_fileMenu->insertItem(SmallIcon("info"), i18n("Properties"), this, SLOT(slotProperties()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(SmallIcon("reload"), i18n("Reload"), this, SLOT(slotReload()));

  m_folderMenu = new KPopupMenu();

  m_folderMenu->insertItem(SmallIcon("folder_new"), i18n("&New Folder..."), this, SLOT(slotNewDir()));
  m_folderMenu->insertItem(SmallIcon("mail_send"), i18n("Send in E-Mail"), this, SLOT(slotSendInMail()));
  m_insertFolderInProject = m_folderMenu->insertItem(i18n("Insert in Project..."), this, SLOT(slotInsertDirInProject()));
  m_folderMenu->insertSeparator();
  m_folderMenu->insertItem(SmallIcon("editcopy"), i18n("&Copy"), this, SLOT(slotCopy()));
  m_folderMenu->insertItem(SmallIcon("editpaste"), i18n("&Paste"), this, SLOT(slotPaste()));
  m_deleteMenuId = m_folderMenu->insertItem(SmallIcon("editdelete"), i18n("&Delete"), this, SLOT(slotDelete()));
  m_folderMenu->insertItem(SmallIcon("info"), i18n("Properties"), this, SLOT(slotProperties()));
  m_folderMenu->insertSeparator();
  m_folderMenu->insertItem(SmallIcon("reload"), i18n("Reload"), this, SLOT(slotReload()));

  setRootIsDecorated( true );
  //header()->hide();
  setSorting( 0 );
  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn(i18n("Templates"), -1);
  addColumn("");
  setFullWidth(true);
  setShowSortIndicator(true);

  globalURL.setPath(qConfig.globalDataDir + resourceDir + "templates/");
  newBranch(globalURL);

  localURL.setPath(locateLocal("data", resourceDir + "templates/"));
  newBranch(localURL);

  setFocusPolicy(QWidget::ClickFocus);

  connect(this, SIGNAL(executed(QListViewItem *)),
          this, SLOT(slotSelectFile(QListViewItem *)));

  connect(this, SIGNAL(returnPressed(QListViewItem *)),
          this, SLOT(slotReturnPressed(QListViewItem *)));

  connect(this, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
          this, SLOT(slotMenu(KListView*, QListViewItem*, const QPoint&)));

  connect(this, SIGNAL(open(QListViewItem *)),
          this,  SLOT(slotSelectFile(QListViewItem *)));

  setAcceptDrops(true);
  connect(this, SIGNAL(dropped(KURL::List&, KURL&)),
          this, SLOT(slotDropped(KURL::List&, KURL&)));

  setSelectionMode(QListView::Single);
  setDropHighlighter(true);
  setDragEnabled(true);
}

TemplatesTreeView::~TemplatesTreeView()
{
}


KFileTreeBranch* TemplatesTreeView::newBranch(const KURL& url)
{
  FilesTreeBranch *newBrnch;
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
    QDir dir (url.path(), "", QDir::All & !QDir::Hidden);
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
   if (QuantaCommon::denyBinaryInsert() == KMessageBox::Yes)
   {
     emit insertFile(url);
   }
 }

}

void TemplatesTreeView::slotMenu(KListView*, QListViewItem *item, const QPoint &point)
{
  if ( !item ) return;
  setSelected(item, true);
  bool hasProject = m_projectName;
  m_folderMenu->setItemVisible(m_insertFolderInProject, hasProject);
  m_fileMenu->setItemVisible(m_insertFileInProject, hasProject);

  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if ( curItem->isDir() )
  {
    if ( curItem == curItem->branch()->root())
    {
      m_folderMenu ->setItemVisible(m_deleteMenuId, false);
    } else
    {
      m_folderMenu ->setItemVisible(m_deleteMenuId, true);
    }
    m_folderMenu ->popup(point);
  } else
  {
   readDirInfo();

   QString menuText = "";

   if (m_dirInfo.mimeType.upper().contains("TEXT"))
     menuText = i18n(textMenu.utf8());
   if (m_dirInfo.mimeType.upper().contains("FILE"))
     menuText = i18n(binaryMenu.utf8());
   if (m_dirInfo.mimeType.upper().contains("TEMPLATE"))
     menuText = i18n(docMenu.utf8());

   if (menuText.isEmpty())
   {
     m_fileMenu->setItemVisible(m_openId, false);
   } else
   {
     m_fileMenu->setItemVisible(m_openId, true);
     m_fileMenu->changeItem(m_openId, menuText);
   }

   m_fileMenu->popup( point);
  }
}
/** No descriptions */
void TemplatesTreeView::slotNewDocument()
{
 KURL url = filterTemplate();
 if (QuantaCommon::checkMimeGroup(url, "text") || QuantaCommon::denyBinaryInsert() == KMessageBox::Yes)
 {
   QListViewItem *item = currentItem();
   if (item )
   {
     if ( currentKFileTreeViewItem() != currentKFileTreeViewItem()->branch()->root())
     {
       emit openFile(KURL(), quantaApp->defaultEncoding());
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
 if (menuText == i18n(binaryMenu.utf8()))
   slotInsertTag();
 if (menuText == i18n(docMenu.utf8()))
   slotNewDocument();
}

void TemplatesTreeView::slotSelectFile(QListViewItem *item)
{
  if ( !item ) return;

  KFileTreeViewItem *kftvItem = currentKFileTreeViewItem();
  if ( !kftvItem ) return;

  if (expandArchiv(kftvItem)) return;

  if ( !kftvItem->isDir() )
  {
    readDirInfo();
    if (m_dirInfo.mimeType.upper().contains("TEXT"))
        slotInsertInDocument();
    if (m_dirInfo.mimeType.upper().contains("FILE"))
        slotInsertTag();
    if (m_dirInfo.mimeType.upper().contains("TEMPLATE"))
        slotNewDocument();
  }
}

void TemplatesTreeView::slotOpen()
{
  FilesTreeView::slotSelectFile(currentItem());
}

/** No descriptions */
void TemplatesTreeView::slotNewDir()
{
  NewTemplateDirDlg *createDirDlg = new NewTemplateDirDlg(this,i18n("Create New Template Folder"));
  createDirDlg->typesCombo->insertItem("text/all");
  createDirDlg->typesCombo->insertItem("file/all");
  createDirDlg->typesCombo->insertItem("template/all");

  readDirInfo();

   if (m_dirInfo.mimeType.isEmpty())
   {
    createDirDlg->parentAttr->setText(i18n("&Inherit parent attribute (nothing)"));
   } else
   {
     createDirDlg->parentAttr->setText(i18n("&Inherit parent attribute (%1)").arg(m_dirInfo.mimeType));
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
                   Maybe you don't have permission to write in the %1 folder.").arg(startDir));
      return;
   }
   if (! createDirDlg->parentAttr->isChecked())
   {
      m_dirInfo.mimeType = createDirDlg->typesCombo->currentText();
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

  readDirInfo();
  if(!m_dirInfo.mimeType.isEmpty()) // only drag when the template type is specified
  {
    KURLDrag *drag = KURLDrag::newDrag(KURL::List(currentURL()), this);
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
    FilesTreeView::contentsDropEvent(e);
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
                                          this, i18n("Save selection as template file: "));
      if ( !url.isEmpty() )
      {
        //now save the file
        KTempFile* tempFile = new KTempFile(tmpDir);
        tempFile->setAutoDelete(true);
        tempFile->textStream()->setEncoding(QTextStream::UnicodeUTF8);
        *(tempFile->textStream()) << content;
        tempFile->close();
        bool proceed = true;
        if (QExtFileInfo::exists(url))
        {
          proceed = KMessageBox::warningYesNo(this, i18n("<qt>The file <b>%1</b> already exists.<br>Do you want to overwrite it?</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)),i18n("Overwrite")) == KMessageBox::Yes;
        }
        if (proceed)
        {
          if (!KIO::NetAccess::upload(tempFile->name(),  url))
          {
            KMessageBox::error(this,i18n("<qt>Couldn't write to file <b>%1</b>.<br>Check if you have rights to write there or that your connection is working.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)));
          }
        }
        delete tempFile;
      }
    }
  }
  // must be done to reset timer etc.
  FilesTreeView::contentsDropEvent(e);
}

/** Reads a .dirinfo file from the selected item's path */
void TemplatesTreeView::readDirInfo(const QString& dir)
{
  QString startDir = dir;

  if (startDir.isEmpty())
  {
    if ( !currentKFileTreeViewItem()->isDir() )
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
  m_dirInfo.mimeType = config->readEntry("Type");
  m_dirInfo.preText = config->readEntry("PreText");
  m_dirInfo.postText = config->readEntry("PostText");
  m_dirInfo.usePrePostText = config->readBoolEntry("UsePrePostText", false);

  delete config;
}

/** Writes a .dirinfo file from the selected item's path */
void TemplatesTreeView::writeDirInfo(const QString& m_dirInfoFile)
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

  KConfig *config = new KConfig(dotFileInfo.filePath());
  config->writeEntry("Type", m_dirInfo.mimeType);
  config->writeEntry("PreText", m_dirInfo.preText);
  config->writeEntry("PostText", m_dirInfo.postText);
  config->writeEntry("UsePrePostText", m_dirInfo.usePrePostText);
  config->sync();

  delete config;
}

void TemplatesTreeView::slotProperties()
{
  if ( !currentItem() ) return;
  KURL url = currentURL();

  KPropertiesDialog *propDlg = new KPropertiesDialog( url, this, 0L, false, false); //autodeletes itself

//Always add the Quanta directory page
  QFrame *quantaDirPage = propDlg->dialog()->addPage(i18n("Quanta Template"));
  QVBoxLayout *topLayout = new QVBoxLayout( quantaDirPage);
  m_quantaProperties = new QuantaPropertiesPage( quantaDirPage, i18n("Quanta") );

  m_quantaProperties->typesCombo->insertItem("text/all");
  m_quantaProperties->typesCombo->insertItem("file/all");
  m_quantaProperties->typesCombo->insertItem("template/all");

  readDirInfo();

  m_quantaProperties->typesCombo->setCurrentItem(m_dirInfo.mimeType);

  QString startDir = "";
  if (  !currentKFileTreeViewItem()->isDir() )
  {
   startDir = url.path();
   m_quantaProperties->prePostGroup->hide();
  } else
  {
   startDir = url.path() + "/dummy_file";
  }
  QFileInfo dotFileInfo(QFileInfo(startDir).dirPath()+"/.dirinfo");
  if (!dotFileInfo.exists()) m_quantaProperties->parentAttr->setChecked(true);
  if (m_dirInfo.mimeType.isEmpty())
   {
    m_quantaProperties->parentAttr->setText(i18n("&Inherit parent attribute (nothing)"));
   } else
   {
    m_quantaProperties->parentAttr->setText(i18n("&Inherit parent attribute (%1)").arg(m_dirInfo.mimeType));
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
  KActionCollection *ac = quantaApp->actionCollection();
  for (uint i = 0; i < ac->count(); i++)
  {
    TagAction *action = dynamic_cast<TagAction*>(ac->action(i));
    if (action)
    {
      QDomElement el = action->data();
      QString type = el.attribute("type", "tag");
      if (type == "script")
      {
        m_quantaProperties->actionCombo->insertItem(action->text());
        if (action->text() == name)
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
//    slotPropertiesApplied();
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

  if (!m_quantaProperties->parentAttr->isChecked())
  {
    m_localDirInfo.mimeType = m_quantaProperties->typesCombo->currentText();
  } else
  {
    m_localDirInfo.mimeType = m_dirInfo.mimeType;
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
    writeDirInfo();
    if (currentItem())
    {
      currentItem()->setText(1, m_dirInfo.mimeType);
    }
  }

  writeTemplateInfo();
}

/** No descriptions */
void TemplatesTreeView::slotInsertTag()
{
 if (currentItem())
 {
  readDirInfo();
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
   if(!url.isLocalFile())
    return;

   QString localFileName = url.path();

   readDirInfo(localFileName);
   QString mimeType = KMimeType::findByPath(localFileName)->name();

   /* First, see if the type of the file is specified in the .dirinfo file */
   if(m_dirInfo.mimeType.isEmpty())
   {
     // no .dirinfo file present, so we insert it as tag
     emit insertTag(KURL::fromPathOrURL( localFileName ), m_dirInfo);
   } else
   {
     if(m_dirInfo.mimeType == "text/all") // default to inserting in document
     {
      if(!mimeType.contains("text", false) && QuantaCommon::denyBinaryInsert() != KMessageBox::Yes)
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
       if(!mimeType.contains("text", false) && QuantaCommon::denyBinaryInsert() != KMessageBox::Yes)
       {
         return;
       }
       emit openFile(KURL(), quantaApp->defaultEncoding());
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
  Q_UNUSED(baseURL);
  m_projectName = projectName;
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

    if ( KMessageBox::warningYesNo(this, msg) == KMessageBox::Yes )
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
 KActionCollection *ac = quantaApp->actionCollection();
 for (uint i = 0; i < ac->count(); i++)
 {
   TagAction *action = dynamic_cast<TagAction*>(ac->action(i));
   if (action)
   {
     QDomElement el = action->data();
     QString type = el.attribute("type", "tag");
     if (type == "script" && action->text() == name)
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
   filterAction->execute();
   tempFile->close();
   tempFileList.append(tempFile);
   url.setPath(tempFile->name());
 }
 return url;
}


void TemplatesTreeView::slotSendInMail()
{
  if ( ! currentKFileTreeViewItem() ) return;

    KURL url = currentURL();
    KURL dirURL (url);
    dirURL.setPath(dirURL.directory(false));

    QString prefix="quanta";
    KTempFile* tempFile = new KTempFile(tmpDir, ".tgz");
    tempFile->setAutoDelete(true);

  //pack the files into a .tgz file
    KTar tar(tempFile->name(), "application/x-gzip");
    tar.open(IO_WriteOnly);
//    tar.setOrigFileName("");

    KURL::List files;
    if ( ! currentKFileTreeViewItem()->isDir() )
      files.append(url);
    else {
      files = QExtFileInfo::allFiles(dirURL, "*") ;
      dirURL = dirURL.upURL();
    }

    for ( KURL::List::Iterator it_f = files.begin(); it_f != files.end(); ++it_f )
    {
      if ( (*it_f).fileName(false) != "" ) {
        url = QExtFileInfo::toRelative( (*it_f), dirURL) ;

        QFile file((*it_f).path());
        file.open(IO_ReadOnly);
        QByteArray bArray = file.readAll();
        tar.writeFile(url.path(), "user", "group", bArray.size(), bArray.data());
        file.close();
      };
    }
    tar.close();

    tempFile->close();
    tempFileList.append(tempFile);

    QStringList attachmentFile;
    attachmentFile += tempFile->name();

    TagMailDlg *mailDlg = new TagMailDlg( this, i18n("Send template in email"));
    QString toStr;
    QString message = i18n("Hi,\n This is a Quanta Plus [http://quanta.sourceforge.net] template tarball.\n\nHave fun.\n");
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

      QString nullString="";
      kapp->invokeMailer(toStr, nullString, nullString, subjectStr, message, nullString, attachmentFile);
    }
    delete mailDlg;
}

bool TemplatesTreeView::acceptDrag(QDropEvent* e ) const
{
 return (FilesTreeView::acceptDrag(e) || QTextDrag::canDecode(e));
}


#include "templatestreeview.moc"
