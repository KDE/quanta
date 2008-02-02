/***************************************************************************
                          scripttreeview.cpp  -  description
                             -------------------
    begin                : Thu Sep 16 2003
    copyright            : (C) 2003-2004  by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
//kde includes
#include <kapplication.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kprocess.h>
#include <krun.h>
#include <kstandarddirs.h>
#include <ktar.h>
#include <ktempdir.h>
#include <ktempfile.h>
#include <kurl.h>
#include <kdebug.h>

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

//other includes
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/DOCBparser.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

//app includes
#include "scripttreeview.h"
#include "resource.h"
#include "quantacommon.h"
#include "tagmaildlg.h"

extern int xmlLoadExtDtdDefaultValue;

ScriptTreeView::ScriptTreeView(QWidget *parent, const char *name )
  : BaseTreeView(parent,name)
{
  setSaveOpenFolder(true);
  addColumn(i18n("Scripts"), -1);
  addColumn("");

  KURL url;
  url.setPath(qConfig.globalDataDir + resourceDir + "scripts/");

  BaseTreeBranch *m_globalDir;
  m_globalDir = new BaseTreeBranch(this, url, i18n("Global Scripts"), SmallIcon("run"), true);
  addBranch(m_globalDir);

  url.setPath(locateLocal("data", resourceDir + "scripts/"));

  BaseTreeBranch *m_localDir;
  m_localDir = new BaseTreeBranch(this, url, i18n("Local Scripts"), SmallIcon("run"), true);
  addBranch(m_localDir);

  // here you define which files should not be visible for the users
  const QString excludeString = ".*\\.info$|.*\\.css$|.*\\.xsl$";
  m_globalDir->excludeFilterRx.setPattern(excludeString);
  m_localDir->excludeFilterRx.setPattern(excludeString);

  m_fileMenu = new KPopupMenu(this);
  m_fileMenu->insertItem(SmallIcon("info"), i18n("&Description"), this, SLOT(slotProperties()));
  m_fileMenu->insertItem(SmallIcon("run"), i18n("&Run Script"), this, SLOT(slotRun()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(i18n("&Edit Script"), this, SLOT(slotEditScript()));
  m_fileMenu->insertItem(i18n("Edit in &Quanta"), this, SLOT(slotEditInQuanta()));
  m_fileMenu->insertItem(i18n("Edi&t Description"), this, SLOT(slotEditDescription()));
  m_fileMenu->insertSeparator();
  m_fileMenu->insertItem(UserIcon("ball"), i18n("&Assign Action"), this, SLOT(slotAssignAction()));
  m_fileMenu->insertItem(SmallIcon("mail_send"), i18n("&Send in Email..."), this, SLOT(slotSendScriptInMail()));
  m_fileMenu->insertItem(SmallIcon("network"), i18n("&Upload Script..."), this, SLOT(slotUploadScript()));

  m_folderMenu = new KPopupMenu(this);
  m_downloadMenuId = m_folderMenu->insertItem(SmallIcon("network"), i18n("&Download Script..."), this, SIGNAL(downloadScript()));


  connect(this, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
          this, SLOT(slotMenu(KListView*, QListViewItem*, const QPoint&)));

  restoreLayout( kapp->config(), className() );
  // the restored size of the first column might be too large for the current content
  // we set it to 10 and the listview will adjust it to the size of the largest entry
  setColumnWidth(0, 10);
}

ScriptTreeView::~ScriptTreeView()
{
}

void ScriptTreeView::slotMenu(KListView *, QListViewItem *item, const QPoint &point)
{
  if (!item)
  {
    m_folderMenu->setItemVisible(m_downloadMenuId, true);
    m_folderMenu->popup(point);
    return;
  }
  setSelected(item, true);

  KFileTreeViewItem *curItem = currentKFileTreeViewItem();
  if (!curItem->isDir())
  {
    m_fileMenu->popup(point);
  } else
  {
    if (curItem == curItem->branch()->root())
    {
      m_folderMenu->setItemVisible(m_downloadMenuId, true);
    } else
    {
      m_folderMenu->setItemVisible(m_downloadMenuId, false);
    }
    m_folderMenu->popup(point);
  }
}

void ScriptTreeView::slotSelectFile(QListViewItem *item)
{
  if (item) {
    if ( !currentKFileTreeViewItem()->isDir() )
    {
      KURL urlToOpen = infoFile(currentURL(), true);
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
      QStringList argsList = QStringList::split(' ', execApp);
      *proc << argsList;
      *proc << urlToOpen.path();
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
    url.setPath(url.path().replace(locateLocal("data", resourceDir + "scripts/"), "%scriptdir/"));
    url.setPath(url.path().replace(qConfig.globalDataDir + resourceDir + "scripts/", "%scriptdir/"));
    emit assignActionToScript(url, execApp);
  }
}

QString ScriptTreeView::createScriptTarball()
{
  KURL url = currentURL();
  KURL infoURL = infoFile(url);

  KTempDir* tempDir = new KTempDir(tmpDir);
  tempDir->setAutoDelete(true);
  tempDirList.append(tempDir);
  QString tempFileName=tempDir->name() + url.fileName() + ".tgz";

  //pack the .tag files and the description.rc into a .tgz file
  KTar tar(tempFileName, "application/x-gzip");
  tar.open(IO_WriteOnly);

  KURL::List files;
  files.append(url);
  files.append(infoURL);
  files.append(KURL().fromPathOrURL(qConfig.globalDataDir + resourceDir + "scripts/info.xsl"));
  for ( KURL::List::Iterator it_f = files.begin(); it_f != files.end(); ++it_f )
  {
    QFile file((*it_f).path());
    file.open(IO_ReadOnly);
    QByteArray bArray = file.readAll();
    tar.writeFile((*it_f).fileName(), "user", "group", bArray.size(), bArray.data());
    file.close();
  }
  tar.close();
  
  return tempFileName;
}

void ScriptTreeView::slotSendScriptInMail()
{
  if ( !currentKFileTreeViewItem()->isDir() )
  {

    QStringList attachmentFile;
    attachmentFile += createScriptTarball();

    TagMailDlg *mailDlg = new TagMailDlg( this, i18n("Send script in email"));
    QString toStr;
    QString message = i18n("Hi,\n This is a Quanta Plus [http://quanta.kdewebdev.org] script tarball.\n\nHave fun.\n");
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
        subjectStr = (mailDlg->lineSubject->text().isEmpty())?i18n("Quanta Plus Script"):mailDlg->lineSubject->text();
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
}

void ScriptTreeView::slotUploadScript()
{
  if ( !currentKFileTreeViewItem()->isDir() )
  {
    QString fileName = createScriptTarball();
    emit uploadScript(fileName);
  }
}

KURL ScriptTreeView::infoFile(const KURL& url, bool htmlVersion)
{
  KURL returnUrl = url;
  QString fileName = returnUrl.fileName();
  //fileName.truncate(fileName.length() - QFileInfo(fileName).extension().length() - 1);
  fileName.append(".info");
  returnUrl.setFileName(fileName);
  if (!QFileInfo(returnUrl.path()).exists())
  {
    QFile f(returnUrl.path());
    if (f.open(IO_WriteOnly))
    {
      QTextStream str(&f);
      str.setEncoding(QTextStream::UnicodeUTF8);
      str << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
      str << "<?xml-stylesheet type=\"text/xsl\" href=\"info.xsl\" ?>" << endl;
      str << "<!DOCTYPE QuantaScriptInfo>" << endl;
      str << "<INFO>" << endl;
      str << "  <options editor=\"\" interpreter=\"\" />" << endl;
      str << "  <name>" << url.fileName() << "</name>" << endl;
      str << "  <author></author>" << endl;
      str << "  <email></email>" << endl;
      str << "  <website></website>" << endl;
      str << "  <version></version>" << endl;
      str << "  <license></license>" << endl;
      str << "  <about></about>" << endl;
      str << "</INFO>" << endl;
      f.close();
    }
  }

  if (htmlVersion)
  {
      KTempFile *tempInfoFile = 0L;
      if (!QFileInfo(returnUrl.path()).exists())
      {
        tempInfoFile = new KTempFile(tmpDir);
        tempInfoFile->setAutoDelete(true);
        returnUrl = KURL::fromPathOrURL(tempInfoFile->name());
        QTextStream str(tempInfoFile->file());
        str.setEncoding(QTextStream::UnicodeUTF8);
        str << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
        str << "<?xml-stylesheet type=\"text/xsl\" href=\"info.xsl\" ?>" << endl;
        str << "<!DOCTYPE QuantaScriptInfo>" << endl;
        str << "<INFO>" << endl;
        str << "  <options editor=\"\" interpreter=\"\" />" << endl;
        str << "  <name>" << url.fileName() << "</name>" << endl;
        str << "  <author></author>" << endl;
        str << "  <email></email>" << endl;
        str << "  <website></website>" << endl;
        str << "  <version></version>" << endl;
        str << "  <license></license>" << endl;
        str << "  <about></about>" << endl;
        str << "</INFO>" << endl;
        tempInfoFile->close();
        tempFileList.append(tempInfoFile);        
      }
      KTempFile *tempFile = new KTempFile(tmpDir);
      tempFile->setAutoDelete(true);
    //apply the stylesheet
      xsltStylesheetPtr cur = NULL;
      xmlDocPtr doc, res;
      xmlSubstituteEntitiesDefault(1);
      xmlLoadExtDtdDefaultValue = 1;
      QString xslFile = qConfig.globalDataDir + resourceDir + "scripts/info.xsl";
      cur = xsltParseStylesheetFile(xmlCharStrndup(xslFile.utf8(), xslFile.utf8().length()));
      doc = xmlParseFile(returnUrl.path().utf8());
      res = xsltApplyStylesheet(cur, doc, 0);
      xsltSaveResultToFile(tempFile->fstream(), res, cur);

      xsltFreeStylesheet(cur);
      xmlFreeDoc(res);
      xmlFreeDoc(doc);

      xsltCleanupGlobals();
      xmlCleanupParser();
      tempFile->close();

      tempFileList.append(tempFile);
      return KURL().fromPathOrURL(tempFile->name());
   } else
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
