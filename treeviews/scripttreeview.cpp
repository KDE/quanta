/***************************************************************************
                          scripttreeview.cpp  -  description
                             -------------------
    begin                : Thu Sep 16 2003
    copyright            : (C) 2003 by Andras Mantia
    email                : amantia@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//kde includes
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kprocess.h>
#include <krun.h>
#include <kstandarddirs.h>
#include <ktar.h>
#include <ktempfile.h>
#include <kurl.h>


//qt includes
#include <qdom.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpopupmenu.h>
#include <qstringlist.h>
#include <qtextedit.h>

//app includes
#include "filestreefile.h"
#include "filestreefolder.h"
#include "scripttreeview.h"
#include "../resource.h"
#include "../quanta.h"
#include "../tagdialogs/tagmaildlg.h"

ScriptTreeView::ScriptTreeView(QWidget *parent, const char *name )
  : FilesTreeView(parent,name)
{
  excludeFilterRx.setPattern(".*\\.info$|.*\\.css$");

  KURL url;
  url.setPath(qConfig.globalDataDir +"quanta/scripts/");
  m_globalDir = new FilesTreeFolder( this , i18n("Global scripts"), url);
  m_globalDir->setPixmap( 0, SmallIcon("folder"));
  m_globalDir->setOpen( true );

  url.setPath(locateLocal("data","quanta/scripts/"));
  m_localDir = new FilesTreeFolder( this , i18n("Local scripts"), url);
  m_localDir->setPixmap( 0, SmallIcon("folder"));
  m_localDir->setOpen( true );

  setRootIsDecorated( true );
  //header()->hide();
  setSorting( 0 );

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( i18n("Scripts"), 600 );

  setFocusPolicy(QWidget::ClickFocus);

  fileMenu = new QPopupMenu();
  fileMenu->insertItem(i18n("Run Script"), this ,SLOT(slotRun()));
  fileMenu->insertSeparator();
  fileMenu->insertItem(i18n("Edit Script"), this ,SLOT(slotEditScript()));
  fileMenu->insertItem(i18n("Edit in Quanta"), this ,SLOT(slotEditInQuanta()));
  fileMenu->insertItem(i18n("Edit Description"), this ,SLOT(slotEditDescription()));
  fileMenu->insertSeparator();
  fileMenu->insertItem(i18n("Assign Action"), this ,SLOT(slotAssignAction()));
  fileMenu->insertItem(i18n("Send in E-Mail"), this ,SLOT(slotSendScriptInMail()));


  connect(  this, SIGNAL(executed(QListViewItem *)),
            this, SLOT(slotSelectFile(QListViewItem *)));
  connect( this, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)),
           this, SLOT(slotMenu(QListViewItem*, const QPoint&, int)));
}

ScriptTreeView::~ScriptTreeView()
{
}

void ScriptTreeView::slotMenu(QListViewItem *item, const QPoint &point, int)
{
  if (!item)
      return;
  setSelected(item, true);

  FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
  if (f)
  {
   fileMenu->popup( point);
  }
}

void ScriptTreeView::slotSelectFile(QListViewItem *item)
{
  FilesTreeFile *f = dynamic_cast<FilesTreeFile*>(item);
  if (f)
  {
    KURL urlToOpen = infoFile(currentURL());
    emit openFileInPreview(urlToOpen);
  }
}

void ScriptTreeView::slotEditDescription()
{
  FilesTreeFile *f = dynamic_cast<FilesTreeFile*>(currentItem());
  if (f)
  {
    KURL urlToOpen = infoFile(currentURL());
    emit showPreviewWidget(false);
    emit openFile(urlToOpen, quantaApp->defaultEncoding());
  }
}

void ScriptTreeView::slotEditScript()
{
  FilesTreeFile *f = dynamic_cast<FilesTreeFile*>(currentItem());
  if (f)
  {
    emit showPreviewWidget(false);
    KURL urlToOpen = currentURL();
    KURL infoUrl = infoFile(urlToOpen);
    QString editApp = infoOptionValue(infoUrl, "editor");
    if (editApp.isEmpty())
        emit openFile(urlToOpen, quantaApp->defaultEncoding());
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
  FilesTreeFile *f = dynamic_cast<FilesTreeFile*>(currentItem());
  if (f)
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
  FilesTreeFile *f = dynamic_cast<FilesTreeFile*>(currentItem());
  if (f)
  {
    KURL urlToOpen = currentURL();
    emit showPreviewWidget(false);
    emit openFile(urlToOpen, quantaApp->defaultEncoding());
  }
}

void ScriptTreeView::slotAssignAction()
{
  FilesTreeFile *f = dynamic_cast<FilesTreeFile*>(currentItem());
  if (f)
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
  FilesTreeFile *f = dynamic_cast<FilesTreeFile*>(currentItem());
  if (f)
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
        toStr = +mailDlg->lineEmail->text();
        subjectStr = (mailDlg->lineSubject->text().isEmpty())?i18n("Quanta Plus Script"):mailDlg->lineSubject->text();
        if ( !mailDlg->titleEdit->text().isEmpty())
            message = mailDlg->titleEdit->text();
      } else
      {
        KMessageBox::error(this,i18n("No destination address was specified./n Sending is aborted."),i18n("Error Sending Email"));
        delete mailDlg;
        return;
      }

      QString nullString="";
      kapp->invokeMailer(toStr, nullString, nullString, subjectStr, message, nullString, attachmentFile);
    }
    delete mailDlg;

  }
}

KURL ScriptTreeView::infoFile(const KURL& a_url)
{
  KURL url = a_url;
  QString fileName = url.fileName();
  //fileName.truncate(fileName.length() - QFileInfo(fileName).extension().length() - 1);
  fileName.append(".info");
  url.setFileName(fileName);
  return url;
}

QString ScriptTreeView::infoOptionValue(const KURL& a_infoURL, const QString& a_optionName)
{
  QString value;
  QFile f(a_infoURL.path());
  if (f.open(IO_ReadOnly))
  {
    QDomDocument doc;
    doc.setContent(&f);
    f.close();
    QDomNodeList nodes = doc.elementsByTagName("options");
    if (nodes.count() > 0)
    {
      QDomElement el = nodes.item(0).toElement();
      value = el.attribute(a_optionName);
    }
  }
  return value;
}

 #include "scripttreeview.moc"
