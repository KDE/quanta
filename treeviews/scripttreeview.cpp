/***************************************************************************
                          scripttreeview.cpp  -  description
                             -------------------
    begin                : Thu Sep 16 2003
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
//kde includes
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kprocess.h>
#include <kpropertiesdialog.h>
#include <krun.h>
#include <kstandarddirs.h>
#include <ktar.h>
#include <ktempfile.h>
#include <kurl.h>
#include <kqapp.h>


//qt includes
#include <qdir.h>
#include <qdom.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qregexp.h>
#include <qstringlist.h>
#include <qtextedit.h>

//app includes
#include "scripttreeview.h"
#include "resource.h"
#include "quantacommon.h"
#include "tagmaildlg.h"

ScriptTreeView::ScriptTreeView(KActionCollection *ac,
                               KDockWidget *parent, const char *name )
  : FilesTreeView(parent,name)
{
  m_dock = parent;
  m_action = new KToggleAction( i18n("Show Scripts Tree"), SmallIcon("run"), 0,
                                this, SLOT( slotToggleShow() ),
                                ac, "show_scripttab_tree" );
  setRootIsDecorated( true );
  //header()->hide();
  setSorting( 0 );
  setShowSortIndicator(true);
  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn(i18n("Scripts"), -1);
  addColumn("");
  setFullWidth(true);
  
  KURL url;
  url.setPath(qConfig.globalDataDir + resourceDir + "scripts/");

  FilesTreeBranch *m_globalDir;
  m_globalDir = new FilesTreeBranch(this, url, i18n("Global scripts"), SmallIcon("run"), true);
  addBranch(m_globalDir);

  QDir dir(url.path(), "", QDir::All & !QDir::Hidden);
  if ( dir.count() != 2 ) {
    m_globalDir->root()->setExpandable(true);     //   . and .. are always there
    m_globalDir->setOpen( true );
  }
  url.setPath(locateLocal("data", resourceDir + "scripts/"));

  FilesTreeBranch *m_localDir;
  m_localDir = new FilesTreeBranch(this, url, i18n("Local scripts"), SmallIcon("run"), true);
  addBranch(m_localDir);

  dir.setPath(url.path());
  if ( dir.count() != 2 ) {
    m_localDir->root()->setExpandable(true);     //   . and .. are always there
    m_localDir->setOpen( true );
  }

  // here you define which files should not be visible for the users
  const QString excludeString = ".*\\.info$|.*\\.css$";
  m_globalDir->excludeFilterRx.setPattern(excludeString);
  m_localDir->excludeFilterRx.setPattern(excludeString);

  setFocusPolicy(QWidget::ClickFocus);

  m_fileMenu = new KPopupMenu();
  m_fileMenu->insertItem(SmallIcon("info"), i18n("Description"), this, SLOT(slotProperties()));
  m_fileMenu->insertItem(SmallIcon("run"), i18n("Run Script"), this, SLOT(slotRun()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(i18n("Edit Script"), this, SLOT(slotEditScript()));
  m_fileMenu->insertItem(i18n("Edit in Quanta"), this, SLOT(slotEditInQuanta()));
  m_fileMenu->insertItem(i18n("Edit Description"), this, SLOT(slotEditDescription()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(UserIcon("ball"), i18n("Assign Action"), this, SLOT(slotAssignAction()));
  m_fileMenu->insertItem(SmallIcon("mail_send"), i18n("Send in E-Mail"), this, SLOT(slotSendScriptInMail()));


  connect(this, SIGNAL(executed(QListViewItem *)),
          this, SLOT(slotSelectFile(QListViewItem *)));

  connect(this, SIGNAL(returnPressed(QListViewItem *)),
          this, SLOT(slotReturnPressed(QListViewItem *)));

  connect(this, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
          this, SLOT(slotMenu(KListView*, QListViewItem*, const QPoint&)));

}

ScriptTreeView::~ScriptTreeView()
{
}

void ScriptTreeView::slotMenu(KListView *, QListViewItem *item, const QPoint &point)
{
  if (!item) return;
  setSelected(item, true);

  if ( !currentKFileTreeViewItem()->isDir() )
  {
    m_fileMenu->popup(point);
  }
}

void ScriptTreeView::slotSelectFile(QListViewItem *item)
{
  if (item) {
    if ( !currentKFileTreeViewItem()->isDir() )
    {
      KURL urlToOpen = infoFile(currentURL());
      emit openFileInPreview(urlToOpen);
    }
  }
}

void ScriptTreeView::slotEditDescription()
{
  if ( !currentKFileTreeViewItem()->isDir() )
  {
    KURL urlToOpen = infoFile(currentURL());
    emit showPreviewWidget(false);
    emit openFile(urlToOpen);
  }
}

void ScriptTreeView::slotEditScript()
{
  if ( !currentKFileTreeViewItem()->isDir() )
  {
    emit showPreviewWidget(false);
    KURL urlToOpen = currentURL();
    KURL infoUrl = infoFile(urlToOpen);
    QString editApp = infoOptionValue(infoUrl, "editor");
    if (editApp.isEmpty())
        emit openFile(urlToOpen);
    else
    {
      KProcess *proc = new KProcess();
      *proc << editApp << urlToOpen.path();
      proc->start(KProcess::DontCare);
    }
  }
}


void ScriptTreeView::slotRun()
{
  if ( !currentKFileTreeViewItem()->isDir() )
  {
    KURL urlToOpen = currentURL();
    KURL infoUrl = infoFile(urlToOpen);
    QString execApp = infoOptionValue(infoUrl, "interpreter");
    if (execApp.isEmpty())
    {
        KURL::List list;
        list.append(urlToOpen);
        KRun::displayOpenWithDialog(list);
    }
    else
    {
      KProcess *proc = new KProcess();
      *proc << execApp << urlToOpen.path();
      proc->start(KProcess::DontCare);
    }
  }
}


void ScriptTreeView::slotEditInQuanta()
{
  if ( !currentKFileTreeViewItem()->isDir() )
  {
    KURL urlToOpen = currentURL();
    emit showPreviewWidget(false);
    emit openFile(urlToOpen);
  }
}

void ScriptTreeView::slotAssignAction()
{
  if ( !currentKFileTreeViewItem()->isDir() )
  {
    KURL url = currentURL();
    KURL infoURL = infoFile(url);
    QString execApp = infoOptionValue(infoURL, "interpreter");
    if (execApp.isEmpty())
        execApp = "sh";
    emit assignActionToScript(url, execApp);
  }
}

void ScriptTreeView::slotSendScriptInMail()
{
  if ( !currentKFileTreeViewItem()->isDir() )
  {
    KURL url = currentURL();
    KURL infoURL = infoFile(url);

    QString prefix="quanta";
    KTempFile* tempFile = new KTempFile(tmpDir, ".tgz");
    tempFile->setAutoDelete(true);

  //pack the .tag files and the description.rc into a .tgz file
    KTar tar(tempFile->name(), "application/x-gzip");
    tar.open(IO_WriteOnly);

    KURL::List files;
    files.append(url);
    files.append(infoURL);
    for ( KURL::List::Iterator it_f = files.begin(); it_f != files.end(); ++it_f )
    {
      QFile file((*it_f).path());
      file.open(IO_ReadOnly);
      QByteArray bArray = file.readAll();
      tar.writeFile((*it_f).fileName(), "user", "group", bArray.size(), bArray.data());
      file.close();
    }
    tar.close();

    tempFile->close();
    tempFileList.append(tempFile);

    QStringList attachmentFile;
    attachmentFile += tempFile->name();

    TagMailDlg *mailDlg = new TagMailDlg( this, i18n("Send script in email"));
    QString toStr;
    QString message = i18n("Hi,\n This is a Quanta Plus [http://quanta.sourceforge.net] script tarball.\n\nHave fun.\n");
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
        subjectStr = (mailDlg->lineSubject->text().isEmpty())?i18n("Quanta Plus Script"):mailDlg->lineSubject->text();
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
}

KURL ScriptTreeView::infoFile(const KURL& url)
{
  KURL returnUrl = url;
  QString fileName = returnUrl.fileName();
  //fileName.truncate(fileName.length() - QFileInfo(fileName).extension().length() - 1);
  fileName.append(".info");
  returnUrl.setFileName(fileName);
  return returnUrl;
}

QString ScriptTreeView::infoOptionValue(const KURL& infoURL, const QString& optionName)
{
  QString value;
  QFile f(infoURL.path());
  if (f.open(IO_ReadOnly))
  {
    QDomDocument doc;
    doc.setContent(&f);
    f.close();
    QDomNodeList nodes = doc.elementsByTagName("options");
    if (nodes.count() > 0)
    {
      QDomElement el = nodes.item(0).toElement();
      value = el.attribute(optionName);
    }
  }
  return value;
}


void ScriptTreeView::slotProperties()
{
  KFileTreeViewItem *item = currentKFileTreeViewItem();
  if (item)
      slotSelectFile(item);
}


#include "scripttreeview.moc"
